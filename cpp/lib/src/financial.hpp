#include <filesystem>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "calendar.h"

// this is included here so we can use the "some_string"_s hana::literal
#define BOOST_HANA_CONFIG_ENABLE_STRING_UDL
// boost nation
#include <boost/hana.hpp>
namespace hana = boost::hana;
// using namespace std::literals;
using namespace hana::literals;

#include "readwriterqueue.h"
using namespace moodycamel;

constexpr int NUM_THREADS = 4;

namespace fpp {


class WorkerManager {
    // every WorkerManager needs to implement the following:

    // start is when a user does submitWork<WorkerType>(args...);
    // this is where the worker can do stuff to initialize the work
    // or store some variables about the work's progress

    /*
    for example, backtester. it will receive and list of symbols,
    dates, a strategy right? so what this work will need to keep track of
    is how the progress is going which means 
    */

    class Worker {
        
    };

    // void init(obj) {
    //     // obj will be some kind of object with some helpers
    //     // to help a workermanager send things to other threads
    //     // things like num active threads,
    //     // this is not where the work starts by the way. maybe ill call it init instead

    //     // the global loop manager running this init will call init first,
    //     // then actually submit the job to the other thread to start
    //     // what's nice is we are going to submit a "sub"-worker i guess 
    //     // or like a child worker 
    // };

    // every loop, we get the opportunity to check up on our threads and queues from the other threads to see if we need to replenish them or whatever
    // as well as react to other messages that the threads are sending back to this object.
    void run() {

    }
};







// template <typename... T>
// static inline auto make_storage_map(auto... t)
// {
//     return boost::hana::unpack(hana::tuple_t<T...>, [](auto... t) {
//         return boost::hana::make_map(boost::hana::make_pair(hana::first(t),
//         hana::second(t))...);
//     });
// }

// template <typename... T>
// using Storage = decltype(make_storage_map<T...>());

// constexpr auto extractConfigField = [](auto &&... x) {
//     return hana::find(hana::make_tuple(x...),
//     hana::type_c<TickerSymbolFilepath>);
// };

// Use this function to extract a Type from Tuple vals.
// if it doesn't exist, return a constructed value with args
template <typename Field, typename Tuple, typename... Args>
constexpr auto extractConfigField(Tuple &&vals, Args &&... args) {

  auto f = hana::find_if(vals, [](auto const &c) {
    return hana::first(c) == hana::type_c<Field>;
  });
  if constexpr (f == hana::nothing) {
    return Field{std::forward<Args>(args)...};
  } else {
    std::cout << hana::second(f.value()) << std::endl;
    return hana::second(f.value());
  }
};

// template <typename... Tn>
// using Modes = decltype(extract_mode_tags<Tn...>());

// template <typename... Tn>
// using extract = decltype(extractConfigField<Tn...>(declval(Tn)...));

template <typename Config> 
class Financialcpp {
  // Modes<Tn...> modes;

public:
  // extract<Tn...> extracted;

  // result enums
  struct SubmissionResult { 
      int id; // work id. It's unique across job types
  };



  std::vector<std::thread> threads;
  std::vector<BlockingReaderWriterQueue<int>> queues;
  int jobs = 0;
  int workers = 0;
  BlockingReaderWriterQueue<int> q;
  std::thread t;
  Financialcpp() {}
  Financialcpp(Config &&config) {

    // tickerSymbolFilepath_ = extractConfigField<TickerSymbolFilepath>(config);
    // auto &&test_ye = extractConfigField<ye>(config, 3);
    // std::cout << test_ye.x << std::endl;

    // }
    
    
  }

  // Financialcpp(Tn &&... tn)
  // {
  //     std::cout << "Constructing " << std::endl;
  //     // tickerSymbolFilepath_ = extractConfigField(tn...);
  //     tickerSymbolFilepath_ =
  //     extractConfigField<TickerSymbolFilepath>(std::forward<Tn>(tn)...);

  //     // we can now extract stuff. perfect forward seems to be working but
  //     how do we mix rvalue and lvalue in constructor params

  //     // extract<Dn...> extracted = extractConfigField<Dn...>();
  //     // std::cout << extractConfigField(dn...) << std::endl;
  // };

  void init() {
    // for (size_t i = 0; i < NUM_THREADS; i++) {
    //   std::thread t([]() {
    //     while (true) {
    //       bool succeeded =
    //           q.wait_dequeue_timed(item, std::chrono::microseconds(1));
    //       if (succeeded) {
    //         std::cout << "Lads it's: " << item << std::endl;
    //       }

    //       // Fully-blocking:
    //       // // q.wait_dequeue(item);
    //       // std::cout << "Got item from other thread thanks: " << item <<
    //       // std::endl;

    //       // } else {
    //       //   std::cout << "still waiting thanks" << std::endl;
    //       // }
    //     }
    //   });
    //   t.detach();
    //   this.threads.push_back(t);
    // }
  };

  void submitTick(std::string_view tick, int bin, std::string_view fromDate,
                  std::string_view toDate, std::string_view provider){
      // submit to the queue
  };


  SubmissionResult submitBacktest(std::string_view symbols,
                      int fromDate, int toDate,
                      std::string_view strategy,
                      std::string_view provider) {
                          return SubmissionResult(
                              1
                          );
  };

  void initWorker() {
    std::cout << "calling initWorker" << std::endl;

    if (workers == 0) {
      workers++;
      q = BlockingReaderWriterQueue<int>(100);
      bool succeeded =
          q.try_enqueue(18);         // Will only succeed if the queue has an
      succeeded = q.try_enqueue(19); // Will only succeed if the queue has an
      succeeded = q.try_enqueue(20); // Will only succeed if the queue has an
                                     // empty slot (never allocates)
      std::cout << "1: " << std::this_thread::get_id() << std::endl;

      // this has to be done later. I think "this" isn't fully valid yet!!

      t = std::thread([this]() {
        int item;
        int loop = 5;
        while (loop != 0) {
          bool succeeded =
              q.wait_dequeue_timed(item, std::chrono::microseconds(1));
          if (succeeded) {
            loop--;
            std::cout << "3: " << std::this_thread::get_id() << std::endl;
            std::cout << "Lads it's: " << item << std::endl;
          }

          // Fully-blocking:
          // // q.wait_dequeue(item);
          // std::cout << "Got item from other thread thanks: " << item <<
          // std::endl;

          // } else {
          //   std::cout << "still waiting thanks" << std::endl;
          // }
        }
      });
      t.detach();
    }
  };
  
private:
  std::filesystem::path tickerSymbolFilepath_;
  std::vector<std::string> tickerSymbols_;
}; // namespace fpp

template <typename... Tn> constexpr auto makeFinancialcpp(Tn &&... tn) {
  return Financialcpp{
      hana::make_basic_tuple(hana::make_pair(hana::type_c<Tn>, tn)...)};
};

template <typename... Tn>
using Financialcpp_t = decltype(makeFinancialcpp<Tn...>(std::declval<Tn>()...));
} // namespace fpp