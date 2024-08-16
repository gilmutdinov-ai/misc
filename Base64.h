#pragma once
#include <string>

namespace misc {

std::string base64_encode(const std::string &_s);

std::string do_base64_encode(unsigned char const *bytes_to_encode,
                             unsigned int in_len);

std::string base64_decode(std::string const &encoded_string);

} // namespace misc