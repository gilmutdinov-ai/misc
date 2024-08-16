#include <catch2/catch_test_macros.hpp>

#include "misc/Time.h"
#include <iostream>

TEST_CASE("QueryLoop::_loopImpl waiting interval", "[chrono,QueryLoop]") {

  int query_interval_sec{10};
  auto now = misc::get_now();

  int expect_sleep{3};

  auto start =
      now - std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::seconds{query_interval_sec - expect_sleep});
  ;
  //_loopImpl();
  auto end = now;
  auto passed_secs =
      std::chrono::duration_cast<std::chrono::seconds>(end - start);
  std::cout << "passed secs: " << passed_secs.count() << std::endl;
  auto interval_secs = std::chrono::duration_cast<std::chrono::seconds>(
      std::chrono::seconds{query_interval_sec});
  std::cout << "interval secs: " << interval_secs.count() << std::endl;

  if (interval_secs > passed_secs) {
    int to_sleep = interval_secs.count() - passed_secs.count();
    std::cout << "will sleep secs: " << to_sleep << std::endl;
    REQUIRE(to_sleep == expect_sleep);
  } else
    std::cout << "will no sleep" << std::endl;
}

TEST_CASE("Chrono helpers in misc/Time.h", "[chrono]") {

  auto win =
      std::chrono::duration_cast<std::chrono::seconds>(std::chrono::days{7});
  auto now = misc::get_now();
  auto tp1 = now - std::chrono::days{14};
  auto dif = std::chrono::duration_cast<std::chrono::seconds>(now - tp1);
  /*std::cout << "now:" << to_readable(now) << std::endl;
  std::cout << "tp1:" << to_readable(tp1) << std::endl;
  std::cout << "dif:" << to_readable_dur(dif) << std::endl;
  std::cout << "win:" << to_readable_dur(win) << std::endl;
  std::cout << "dif > win:" << (dif > win) << std::endl;*/

  REQUIRE(dif > win);

  auto day_str = misc::get_day_str(now);
  auto day_str_restored = misc::get_day_str(misc::tp_from_day_str(day_str));
  /*
  std::cout << "day_ori: " << day_str << std::endl;
  std::cout << "day_res: " << day_str_restored << std::endl;
*/
  REQUIRE(day_str == day_str_restored);

  // get_day_end
  {
    auto day_end_expect = misc::tp_from_day_time_str("2019.02.19 23:59:59");
    auto tp = misc::tp_from_day_time_str("2019.02.19 12:35:34");

    // std::cout << "original tp: " << get_day_time_str(tp) << std::endl;
    auto day_end = misc::get_day_end(tp);
    // std::cout << "get_day_end: " << get_day_time_str(day_end) << std::endl;
    REQUIRE(misc::get_day_time_str(day_end_expect) ==
            misc::get_day_time_str(day_end));
  }
  // get_hour_end
  {
    auto hour_end_expect = misc::tp_from_day_time_str("2019.02.19 12:59:59");
    auto tp = misc::tp_from_day_time_str("2019.02.19 12:35:34");

    std::cout << "original tp: " << misc::get_day_time_str(tp) << std::endl;
    auto hour_end = misc::get_hour_end(tp);
    std::cout << "get_hour_end: " << misc::get_day_time_str(hour_end)
              << std::endl;
    REQUIRE(misc::get_day_time_str(hour_end_expect) ==
            misc::get_day_time_str(hour_end));
  }
  // get_hours_till_midnight
  {
    auto tp = misc::tp_from_day_time_str("2019.02.19 12:35:34");
    REQUIRE(misc::get_hours_till_midnight(tp) == 11);
  }
}
