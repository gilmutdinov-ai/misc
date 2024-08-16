#pragma once

#include <string>

#include <iostream>
#include <sstream>
#include <vector>

namespace misc {

std::string last_n(const std::string &_s, size_t _n);
std::string gen_random_str(const int len);
std::string url_encode(const std::string &str);
std::vector<std::string> split(const std::string &target, char c);

} // namespace misc
