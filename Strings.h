#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace misc {

std::string last_n(const std::string &_s, size_t _n);
std::string gen_random_str(const int len);
std::string url_encode(const std::string &str);
std::vector<std::string> split(std::string_view target, char c);

template <typename T> std::string vec_to_csv_str(const std::vector<T> &_v) {
  std::stringstream ss;
  for (size_t i = 0; i < _v.size(); ++i) {
    ss << _v[i];
    if (i != _v.size() - 1)
      ss << ",";
  }
  return ss.str();
}

} // namespace misc
