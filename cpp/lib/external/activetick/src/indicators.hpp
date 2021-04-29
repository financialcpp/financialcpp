#include <string>
#include <vector>

// need to define this or else compiling won't work
// we'll get an error saying can't find the hana::string literals
#define BOOST_HANA_CONFIG_ENABLE_STRING_UDL

#include <boost/hana.hpp>
// #include <boost/hana/map.hpp>
namespace hana = boost::hana;
using namespace hana::literals;

// will prob put these constants in another file at some point if it needs to be used elsewhere but it's okay here for now
const int PREMARKETBEG = 40000000;    // premarket starts at 4:00AM
const int AFTERMARKETEND = 200000000; // aftermarket ends at 8PM

struct TransformPrice
{

    static void transform(auto &activepp, auto &tick_map, auto &p, int rate)
    {
        int preMarketBeg = PREMARKETBEG;     // premarket starts at 4:15AM
        int afterMarketEnd = AFTERMARKETEND; // aftermarket ends at 8PM

        int steps = (afterMarketEnd - preMarketBeg) / rate;

        p.resize(steps);

        int t = preMarketBeg;
        int i = 0;
        int idx = 0;
        auto &times = tick_map["time"_s];
        auto &prices = tick_map["price"_s];
        while (t < afterMarketEnd)
        {
            t += rate;
            // int volume_average = 0;
            float price_average = 0.0;
            int count = 0;
            while (idx < times.size() && times.at(idx) >= (t - rate) && times.at(idx) < t)
            {
                price_average += prices.at(idx);
                // volume_average += volumes[idx];
                count++;
                idx++;
            }
            if (count > 0)
            {
                p.at(i) = price_average / count;
                // toFileVolumes[i] = volume_average / count;
            }
            else
            {
                p.at(i) = 0;
                // toFileVolumes[i] = 0;
            }
            i++;
        }

        // need to backfill and forward fill the prices when there was no data
        float backfilled = false;
        int j = 0;
        if (p.at(0) > 0.000000001)
        {
            backfilled = true;
        }
        for (auto price : p)
        {

            // first backfill
            if (price > 0.00000001 && !backfilled && j != 0)
            {
                for (int k = 0; k < j; ++k)
                {
                    p.at(k) = price;
                }
                backfilled = true;
            }

            // forward fill the other prices
            if (price < 0.00000001 && backfilled)
            {
                p.at(j) = p.at(j - 1);
            }
            j++;
        }
    }
};

struct TransformVolume
{

    static void transform(auto &activepp, auto &tick_map, auto &p, int rate)
    {
        int preMarketBeg = PREMARKETBEG;     // premarket starts at 4:15AM
        int afterMarketEnd = AFTERMARKETEND; // aftermarket ends at 8PM

        int steps = (afterMarketEnd - preMarketBeg) / rate;

        p.resize(steps);

        int t = preMarketBeg;
        int i = 0;
        int idx = 0;
        auto &times = tick_map["time"_s];
        auto &volumes = tick_map["volume"_s];

        int count = 0;
        while (t < afterMarketEnd)
        {
            t += rate;
            int volume_sum = 0;
            count = 0;
            while (idx < times.size() && times.at(idx) >= (t - rate) && times.at(idx) < t)
            {
                volume_sum += volumes.at(idx);
                count++;
                idx++;
            }

            p.at(i) = volume_sum;
            i++;
        }
    }
};

struct TransformNumTrades
{

    static void transform(auto &activepp, auto &tick_map, auto &p, int rate)
    {
        int preMarketBeg = PREMARKETBEG;     // premarket starts at 4:15AM
        int afterMarketEnd = AFTERMARKETEND; // aftermarket ends at 8PM

        int steps = (afterMarketEnd - preMarketBeg) / rate;

        p.resize(steps);

        int t = preMarketBeg;
        int i = 0;
        int idx = 0;
        auto &times = tick_map["time"_s];

        int count = 0;
        while (t < afterMarketEnd)
        {
            t += rate;
            count = 0;
            while (idx < times.size() && times.at(idx) >= (t - rate) && times.at(idx) < t)
            {
                count++;
                idx++;
            }

            p.at(i) = count;
            i++;
        }
    }
};

template <typename T, typename TransformPolicy>
struct VectorFromRaw
{
    int rate;
    std::string name;
    bool is_raw;

    VectorFromRaw(std::string name, int rate) : name{name}, rate{rate} {}
    VectorFromRaw(const VectorFromRaw &other)
    {
        name = other.get_name();
        rate = other.get_rate();
    }

    void start_transform(auto &activepp)
    {
        // let our indicator do whatever they want to init the vector or whatever
        // we don't need it here but other indicators might use this
    }

    void transform(auto &activepp, auto &tick_map)
    {
        TransformPolicy::transform(activepp, tick_map, p, rate);
    }

    void end_transform(auto &activepp, auto &out)
    {
        out.write(reinterpret_cast<char *>(&p[0]), p.size() * sizeof(T));
    }

    void init_indicator(auto &activepp, auto &in)
    {
        p.resize(activepp.rate_to_timesteps(rate));
        in.read(reinterpret_cast<char *>(&p[0]), p.size() * sizeof(T));
    }

    int get_rate() const
    {
        return rate;
    }

    std::string get_name() const
    {
        return name;
    }

    std::vector<T> &get_vec()
    {
        return p;
    }

private:
    std::vector<T> p;
};

// Raw indicators
using Volume = VectorFromRaw<int, TransformVolume>;
using Price = VectorFromRaw<float, TransformPrice>;
using NumTrades = VectorFromRaw<int, TransformNumTrades>;

// Non-raw indicators

struct TransformCumuVol
{

    static void start_transform(auto &activepp)
    {

        auto symbol = activepp.get_current_symbol();
        auto date = activepp.get_current_date();

        // we need volume indicator to be in memory
        activepp.init_indicator("volume"_s, symbol, date);
    }

    static void transform(auto &activepp, auto &p, int rate)
    {
        auto symbol = activepp.get_current_symbol();
        auto date = activepp.get_current_date();
        auto &my_map = activepp.get_map();
        auto &vol_vec = my_map.at(symbol).at(date)["volume"_s].get_vec();

        int preMarketBeg = PREMARKETBEG;     // premarket starts at 4:15AM
        int afterMarketEnd = AFTERMARKETEND; // aftermarket ends at 8PM

        int steps = (afterMarketEnd - preMarketBeg) / rate;

        p.resize(steps);

        p.at(0) = vol_vec.at(0);
        for (auto i = 1; i < vol_vec.size(); ++i)
        {
            p.at(i) = vol_vec.at(i) + p.at(i - 1);
        }
    }
};

struct TransformCumuNumTrades
{

    static void start_transform(auto &activepp)
    {

        auto symbol = activepp.get_current_symbol();
        auto date = activepp.get_current_date();

        activepp.init_indicator("num_trades"_s, symbol, date);
    }

    static void transform(auto &activepp, auto &p, int rate)
    {
        auto symbol = activepp.get_current_symbol();
        auto date = activepp.get_current_date();
        auto &my_map = activepp.get_map();
        auto &num_trades_vec = my_map.at(symbol).at(date)["num_trades"_s].get_vec();

        int preMarketBeg = PREMARKETBEG;     // premarket starts at 4:15AM
        int afterMarketEnd = AFTERMARKETEND; // aftermarket ends at 8PM

        int steps = (afterMarketEnd - preMarketBeg) / rate;

        p.resize(steps);

        p.at(0) = num_trades_vec.at(0);
        for (auto i = 1; i < num_trades_vec.size(); ++i)
        {
            p.at(i) = num_trades_vec.at(i) + p.at(i - 1);
        }
    }
};

template <typename T, typename TransformPolicy>
struct VectorNonRaw
{
    int rate;
    std::string name;

    VectorNonRaw(std::string name, int rate) : name{name}, rate{rate} {}
    VectorNonRaw(const VectorNonRaw &other)
    {
        name = other.get_name();
        rate = other.get_rate();
    }

    void start_transform(auto &activepp)
    {
        TransformPolicy::start_transform(activepp);
    }

    void transform(auto &activepp)
    {

        TransformPolicy::transform(activepp, p, rate);
    }

    void end_transform(auto &activepp, auto &out)
    {
        out.write(reinterpret_cast<char *>(&p[0]), p.size() * sizeof(T));
    }

    void init_indicator(auto &activepp, auto &in)
    {
        p.resize(activepp.rate_to_timesteps(rate));
        in.read(reinterpret_cast<char *>(&p[0]), p.size() * sizeof(T));
    }

    int get_rate() const
    {
        return rate;
    }

    std::string get_name() const
    {
        return name;
    }

    std::vector<T> &get_vec()
    {
        return p;
    }

private:
    std::vector<T> p;
};

using CumuVolume = VectorNonRaw<int, TransformCumuVol>;
using CumuNumTrades = VectorNonRaw<int, TransformCumuNumTrades>;
