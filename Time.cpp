#include "Time.h"

namespace misc {

std::chrono::time_point<std::chrono::system_clock>
round_secs(std::chrono::time_point<std::chrono::system_clock> _tp) {
  auto tp_s = std::chrono::time_point_cast<std::chrono::seconds>(_tp);
  int64_t ts = tp_s.time_since_epoch().count();
  std::chrono::seconds dur{ts};
  std::chrono::time_point<std::chrono::system_clock> tp_r{dur};
  return tp_r;
}

int64_t to_int64(std::chrono::time_point<std::chrono::system_clock> _tp) {
  auto tp_s = std::chrono::time_point_cast<std::chrono::seconds>(_tp);
  int64_t ts = tp_s.time_since_epoch().count();
  return ts;
}

std::chrono::time_point<std::chrono::system_clock> from_int64(int64_t ts) {
  std::chrono::seconds dur{ts};
  std::chrono::time_point<std::chrono::system_clock> tp{dur};
  return tp;
}

std::chrono::time_point<std::chrono::system_clock> get_now() {
  return std::chrono::system_clock::now();
}

std::chrono::time_point<std::chrono::system_clock> get_day_end() {
  return get_day_end(get_now());
}

std::chrono::time_point<std::chrono::system_clock>
get_day_end(std::chrono::time_point<std::chrono::system_clock> _now) {

  time_t tm = std::chrono::system_clock::to_time_t(_now);
  auto lt = std::localtime(&tm);
  lt->tm_hour = 23;
  lt->tm_min = 59;
  lt->tm_sec = 59;

  std::tm tm_end = {/* .tm_sec  = */ lt->tm_sec,
                    /* .tm_min  = */ lt->tm_min,
                    /* .tm_hour = */ lt->tm_hour,
                    /* .tm_mday = */ lt->tm_mday,
                    /* .tm_mon  = */ lt->tm_mon,
                    /* .tm_year = */ lt->tm_year};
  return std::chrono::system_clock::from_time_t(std::mktime(&tm_end));
}

std::chrono::time_point<std::chrono::system_clock> get_hour_end() {
  return get_hour_end(get_now());
}

std::chrono::time_point<std::chrono::system_clock>
get_hour_end(std::chrono::time_point<std::chrono::system_clock> _now) {
  time_t tm = std::chrono::system_clock::to_time_t(_now);
  auto lt = std::localtime(&tm);
  lt->tm_min = 59;
  lt->tm_sec = 59;

  std::tm tm_end = {/* .tm_sec  = */ lt->tm_sec,
                    /* .tm_min  = */ lt->tm_min,
                    /* .tm_hour = */ lt->tm_hour,
                    /* .tm_mday = */ lt->tm_mday,
                    /* .tm_mon  = */ lt->tm_mon,
                    /* .tm_year = */ lt->tm_year};
  return std::chrono::system_clock::from_time_t(std::mktime(&tm_end));
}

int get_hours_till_midnight() { return get_hours_till_midnight(get_now()); }

int get_hours_till_midnight(
    std::chrono::time_point<std::chrono::system_clock> _now) {

  time_t tm = std::chrono::system_clock::to_time_t(_now);
  auto lt = std::localtime(&tm);
  return static_cast<int>(24 - lt->tm_hour) - 1;
}

std::string
to_readable(std::chrono::time_point<std::chrono::system_clock> _tp) {
  auto today = floor<date::days>(_tp);
  std::stringstream ss;
  ss << date::year_month_day{today} << ' ' << date::make_time(_tp - today);
  return ss.str();
}

std::string to_readable_dur(std::chrono::seconds input_seconds) {

  using namespace std::chrono;
  typedef duration<int, std::ratio<86400>> days;
  auto d = duration_cast<days>(input_seconds);
  input_seconds -= d;
  auto h = duration_cast<hours>(input_seconds);
  input_seconds -= h;
  auto m = duration_cast<minutes>(input_seconds);
  input_seconds -= m;
  auto s = duration_cast<seconds>(input_seconds);

  auto dc = d.count();
  auto hc = h.count();
  auto mc = m.count();
  auto sc = s.count();

  std::stringstream ss;
  ss.fill('0');
  if (dc) {
    ss << d.count() << "d";
  }
  if (dc || hc) {
    if (dc) {
      ss << std::setw(2);
    } // pad if second set of numbers
    ss << h.count() << "h";
  }
  if (dc || hc || mc) {
    if (dc || hc) {
      ss << std::setw(2);
    }
    ss << m.count() << "m";
  }
  if (dc || hc || mc || sc) {
    if (dc || hc || mc) {
      ss << std::setw(2);
    }
    ss << s.count() << 's';
  }

  return ss.str();
}

std::string
get_day_str(std::chrono::time_point<std::chrono::system_clock> _tp) {
  auto tm = std::chrono::system_clock::to_time_t(_tp);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&tm), "%Y.%m.%d");
  return ss.str();
}

std::string
get_day_time_str(std::chrono::time_point<std::chrono::system_clock> _tp) {
  auto tm = std::chrono::system_clock::to_time_t(_tp);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&tm), "%Y.%m.%d %H:%M:%S");
  return ss.str();
}

std::chrono::time_point<std::chrono::system_clock>
tp_from_day_str(std::string_view _day) {
  std::tm tm = {};
  std::stringstream ss{std::string{_day}};
  ss >> std::get_time(&tm, "%Y.%m.%d");
  return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

std::chrono::time_point<std::chrono::system_clock>
tp_from_day_time_str(std::string_view _day_time) {
  std::tm tm = {};
  std::stringstream ss{std::string{_day_time}};
  ss >> std::get_time(&tm, "%Y.%m.%d %H:%M:%S");
  return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

} // namespace misc
