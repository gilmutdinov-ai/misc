#pragma once

#include "date/date.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdint.h>
#include <string>
#include <string_view>

namespace misc {

std::chrono::time_point<std::chrono::system_clock>
round_secs(std::chrono::time_point<std::chrono::system_clock> _tp);

int64_t to_int64(std::chrono::time_point<std::chrono::system_clock> _tp);

std::chrono::time_point<std::chrono::system_clock> from_int64(int64_t ts);

std::chrono::time_point<std::chrono::system_clock> get_now();

std::chrono::time_point<std::chrono::system_clock> get_day_end();

std::chrono::time_point<std::chrono::system_clock>
get_day_end(std::chrono::time_point<std::chrono::system_clock> _now);

std::chrono::time_point<std::chrono::system_clock> get_hour_end();

std::chrono::time_point<std::chrono::system_clock>
get_hour_end(std::chrono::time_point<std::chrono::system_clock> _now);

int get_hours_till_midnight();

int get_hours_till_midnight(
    std::chrono::time_point<std::chrono::system_clock> _now);

std::string to_readable(std::chrono::time_point<std::chrono::system_clock> _tp);

std::string to_readable_dur(std::chrono::seconds input_seconds);

std::string get_day_str(std::chrono::time_point<std::chrono::system_clock> _tp);

std::string
get_day_time_str(std::chrono::time_point<std::chrono::system_clock> _tp);

std::chrono::time_point<std::chrono::system_clock>
tp_from_day_str(std::string_view _str);

std::chrono::time_point<std::chrono::system_clock>
tp_from_day_time_str(std::string_view _day_time);

} // namespace misc
