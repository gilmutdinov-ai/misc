#pragma once
#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

namespace misc {

// expect sorted
template <typename T>
size_t push_vector_minus(const std::vector<T> &try_urls,
                         const std::vector<T> &allowed_urls,
                         std::vector<T> &disallowed_urls) {
  std::set_difference(try_urls.begin(), try_urls.end(), allowed_urls.begin(),
                      allowed_urls.end(),
                      std::inserter(disallowed_urls, disallowed_urls.begin()));
  return disallowed_urls.size();
}

template <typename T>
static void limit_vector(const std::vector<T> &_a, std::vector<T> &_b,
                         size_t _l) {
  for (size_t i = 0; i < std::min(_l, _a.size()); ++i)
    _b.push_back(_a[i]);
}

template <typename T> std::vector<T> leave_unique(const std::vector<T> &_v) {
  std::set<T> s{_v.begin(), _v.end()};
  return std::vector<T>{s.begin(), s.end()};
}

template <typename T> void print(const std::vector<T> &_v) {
  for (size_t i = 0; i < _v.size(); ++i)
    std::cout << "|" << _v[i] << "|" << std::endl;
}

} // namespace misc
