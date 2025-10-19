#include <catch2/catch_test_macros.hpp>
#include <map>
#include <string>

std::string &removeExtraSpaces(std::string &s) {
  // remove leading
  int w = 0;
  int r = 0;
  bool prev_space{false};
  while (r < s.size() && s[r] == ' ') { // leading spaces
    ++r;
    prev_space = true;
  }
  // r points to word
  while (r < s.size()) {

    if (s[r] != ' ') { // legit symbols
      if (prev_space && w != 0) {
        s[w] = ' ';
        ++w;
      }
      prev_space = false;
      while (r < s.size() && s[r] != ' ') {
        s[w] = s[r];
        ++w;
        ++r;
      }

    } else { // space
      prev_space = true;
      while (r < s.size() && s[r] == ' ')
        ++r;
    }
  }
  s.erase(s.begin() + w, s.end());
  return s;
}

class UtRemoveExtraSpaces {
public:
  std::map<std::string, std::string> m_cases;

  UtRemoveExtraSpaces() {
    m_cases = {{"  xx x   xxx     ", "xx x xxx"},
               {"          x    xx", "x xx"},
               {"xxxxx", "xxxxx"},
               {"  xxxx xxxx ", "xxxx xxxx"}};

    runTests();
  }

  void runTests() {
    for (const auto &[in_, out_] : m_cases) {
      std::string in{in_};
      REQUIRE(removeExtraSpaces(in) == out_);
    }
  }
};

TEST_CASE("Remove spaces algo", "[remove_spaces]") { UtRemoveExtraSpaces{}; }