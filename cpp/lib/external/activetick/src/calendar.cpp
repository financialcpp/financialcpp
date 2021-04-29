#include <cstdlib>
#include <ctime>
#include <map>
#include <string>
#include <vector>

#include <iostream>

namespace calendar
{
namespace dates
{
const std::map<int, std::map<int, std::vector<int>>> tradingDays{
    {2017,
     {{1, {3, 4, 5, 6,                        // 02: New Year's day observed
           9, 10, 11, 12, 13, 17, 18, 19, 20, // 16: Martin Luther King, Jr. Day
           23, 24, 25, 26, 27, 30, 31}},
      {2, {1, 2, 3, 6, 7, 8, 9, 10, 13, 14, 15, 16, 17, 21, 22, 23, 24, // 20: Washington's B-day
           27, 28}},
      {10, {2, 3, 4, 5, 6, 9, 10, 11, 12, 13, 16, 17, 18, 19, 20, 23, 24, 25, 26, 27, 30, 31}},
      {11, {1, 2, 3, 6, 7, 8, 9, 10, 13, 14, 15, 16, 17, 20, 21, 22, 24, // 23rd is not trading day (thanksgiving i think)
            27, 28, 29, 30}},
      {12, {
               1,
               4,
               5,
               6,
               7,
               8,

           }}}},
    {2018, {{1, {2, 3, 4, 5,                       // 01: New Year's day observed
                 8, 9, 10, 11, 12, 16, 17, 18, 19, // 15: Martin Luther King, Jr. Day
                 22, 23, 24, 25, 26, 29, 30, 31}},
            {2, {1, 2, 5, 6, 7, 8, 9, 12, 13, 14, 15, 16, 20, 21, 22, 23, // 19: Washington's B-day
                 26, 27, 28}},
            {3, {
                    1, 2, 5, 6, 7, 8, 9, 12, 13, 14, 15, 16, 19, 20, 21, 22, 23, 26, 27, 28, 29 // 30: Good Friday
                }},
            {4, {30}},
            {5, {1, 2, 4, 7, 8, 9, 10, 17, 18, 21, 22, 23}},
            {6, {6, 7, 8, 11, 12, 13, 14, 15, 26, 27}},
            {7, {20, 23, 24, 25, 26, 27, 30, 31}},
            {8, {1, 2, 3, 6, 7, 8, 9, 10, 13, 14, 15, 16, 17, 20, 21, 22, 23, 24, 27, 28, 29, 30, 31}},
            {9, {4, 5, 6}}}},
    {2021, {{1, {4, 5, 6, 7, 8,
                 11, 12, 13, 14, 15,
                 19, 20, 21, 22,
                 25, 26, 27, 28, 29
                 }}
            }}        
            };
}

//private functions for calendar namespace
namespace
{
bool dateIsInTradingCalendar(const int year, const int month, const int day)
{
  if (dates::tradingDays.count(year) > 0)
  {
    if (dates::tradingDays.at(year).count(month) > 0)
    {
      for (size_t i = 0; i < dates::tradingDays.at(year).at(month).size(); i++)
      {
        if (day == dates::tradingDays.at(year).at(month).at(i))
        {
          return true;
          break;
        }
      }
    }
  }
  return false;
}
constexpr bool is_digit(char c)
{
  return c <= '9' && c >= '0';
}

constexpr int stoi_impl(const char *str, int value = 0)
{
  return *str ? is_digit(*str) ? stoi_impl(str + 1, (*str - '0') + value * 10)
                               : throw "compile-time-error: not a digit"
              : value;
}

constexpr int strToInt(const std::string_view str)
{
  return stoi_impl(str.data());
}

bool olderOrSameAsEnd(const int year, const int month, const int day, const int year_end, const int month_end, const int day_end)
{
  if (year == year_end && month < month_end)
  {
    return true;
  }
  else if (year == year_end && month == month_end)
  {
    return day <= day_end;
  }
  else if (year == year_end && month < month_end)
  {
    return true;
  }
  else if (year < year_end)
  {
    return true;
  }

  return false;
}
} // namespace

std::vector<std::string> parseDateRange(const std::string &dateRange)
{

  std::vector<std::string> dates;

  // Match: YYYYMMDD-YYYYMMDD
  if (dateRange.length() == 17)
  {
    auto year_start = strToInt(dateRange.substr(0, 4));
    auto month_start = strToInt(dateRange.substr(4, 2));
    auto day_start = strToInt(dateRange.substr(6, 2));

    auto year_end = strToInt(dateRange.substr(9, 4));
    auto month_end = strToInt(dateRange.substr(13, 2));
    auto day_end = strToInt(dateRange.substr(15, 2));

    // only continue if our date isn't past the end
    while (olderOrSameAsEnd(year_start, month_start, day_start, year_end, month_end, day_end))
    {
      if (dateIsInTradingCalendar(year_start, month_start, day_start))
      {
        auto year = std::to_string(year_start);

        auto month = std::to_string(month_start).length() == 1 ? "0" + std::to_string(month_start) : std::to_string(month_start);

        auto day = std::to_string(day_start).length() == 1 ? "0" + std::to_string(day_start) : std::to_string(day_start);
        dates.push_back(year + month + day);
      }

      // Naive implementation of incrementing the days
      // Obviously some of these days aren't valid (e.g Feb 31st) but it
      // won't matter
      day_start++;
      if (day_start > 31)
      {
        day_start = 1;
        month_start++;
        if (month_start > 12)
        {
          month_start = 1;
          year_start++;
        }
      }
    }
    // std::cout << dates::tradingDays[year_start][month_start][0] << std::endl;
  }
  else if (dateRange.length() == 8)
  {
    auto year_start = strToInt(dateRange.substr(0, 4));
    auto month_start = strToInt(dateRange.substr(4, 2));
    auto day_start = strToInt(dateRange.substr(6, 2));
    if (dateIsInTradingCalendar(year_start, month_start, day_start))
    {
      dates.push_back(dateRange);
    }
  }
  return dates;
}

} // namespace calendar
