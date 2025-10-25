#include <catch2/catch_test_macros.hpp>
#include <map>
#include <string>

using namespace std;

string &removeExtraSpacesShort(string &s) {

  size_t w = 0;
  size_t r = 0;
  bool prev_space{false};
  while (r < s.size()) {
    if (s[r] == ' ') { // space
      prev_space = true;
    } else { // not space
      if (prev_space && w != 0) {
        s[w] = ' ';
        ++w;
      }
      s[w] = s[r];
      ++w;
      prev_space = false;
    }
    ++r;
  }
  s.erase(s.begin() + w, s.end());
  return s;
}

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

    runTestsLongVersion();
    runTestsShortVersion();
  }

  void runTestsLongVersion() {
    for (const auto &[in_, out_] : m_cases) {
      std::string in{in_};
      REQUIRE(removeExtraSpaces(in) == out_);
    }
  }
  void runTestsShortVersion() {
    for (const auto &[in_, out_] : m_cases) {
      std::string in{in_};
      REQUIRE(removeExtraSpacesShort(in) == out_);
    }
  }
};

TEST_CASE("Remove spaces algo", "[remove_spaces]") { UtRemoveExtraSpaces{}; }