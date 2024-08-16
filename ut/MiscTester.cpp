#include "misc/ut/MiscTester.h"
#include "misc/Vectors.h"
#include <catch2/catch_test_macros.hpp>

namespace misc {

MiscTester::MiscTester() { _testVectors(); }

void MiscTester::_testVectors() {
  {
    std::vector<std::string> a{"1", "2", "3", "4"};
    std::vector<std::string> b{"2", "4"};
    std::vector<std::string> expect_c{"1", "3"};
    std::vector<std::string> c;

    push_vector_minus(a, b, c);
    REQUIRE(c == expect_c);
  }
  {
    std::vector<std::string> a{"1", "2", "2", "4"};
    std::vector<std::string> b_expect{"1", "2", "4"};
    auto b = leave_unique(a);
    REQUIRE(b == b_expect);
  }
}

} // namespace misc