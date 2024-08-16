#include "misc/Bazel.h"

#include <cstdlib>
#include <sstream>
#include <string>

namespace misc {

std::string get_bazel_test_data_path(const std::string &_data_path) {
  std::stringstream ss;
  ss << std::getenv("TEST_SRCDIR");
  ss << "/" << std::getenv("TEST_WORKSPACE");
  ss << _data_path;
  return ss.str();
}

} // namespace misc
