#pragma once
#include <string>
#include <string_view>

namespace misc {

std::string base64_encode(std::string_view _s);

std::string do_base64_encode(unsigned char const *bytes_to_encode,
                             unsigned int in_len);

std::string base64_decode(std::string_view encoded_string);

} // namespace misc