#pragma once

#include <string>
#include <string_view>

namespace misc {

std::string get_bazel_test_data_path(std::string_view _data_path);

}