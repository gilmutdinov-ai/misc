#include "junk/cpp/common/stream_mock.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

// std::move - I no longer need this value here
//   just mark, equivalent to
//   static_cast<T&&>(x)
//
using namespace std;

string getData() { return string{"123"}; }

TEST_CASE("Move semantics w/names: multiple times", "[move]") {
  string str{getData()};
  vector<string> v1;
  vector<string> v2;
  v1.push_back(str);            // COPY
  v2.push_back(std::move(str)); // MOVE, no longer need the value
  REQUIRE(v1[0] == v2[0]);
}

TEST_CASE("Move semantics w/names: parameters", "[move]") {
  vector<string> history;
  auto reinit = [&](std::string &s) {
    history.push_back(std::move(s)); // MOVE, no longer need the value
    s = "default value";
  };
  string s{"init value"};
  reinit(s);
  REQUIRE(history[0] == "init value");
  REQUIRE(s == "default value");
}

TEST_CASE("Move semantics w/names: read stream", "[move]") {
  char buffer[] = "abc\ndef\nghi";
  size_t buffer_size = 11;

  vector<string> rows;
  string row;
  misc::mock_stream in(buffer, 0, buffer_size);
  while (getline(in, row)) {        // reuse moved object
    rows.push_back(std::move(row)); // MOVE
  }
  REQUIRE(rows[0] == "abc");
  REQUIRE(rows[1] == "def");
  REQUIRE(rows[2] == "ghi");
}

TEST_CASE("Move semantics w/names: valid but unspecified state", "[move]") {
  vector<string> v;
  v.reserve(3);
  string s = getData();
  v.push_back(s);
  v.push_back(std::move(s)); // MOVE

  // now s - valid but unspecified
  s += '.';
  size_t i = s.size();
  REQUIRE(i >= 1);

  // char c = s[0]; // some value, dont know
  // char c2 = s[5]; // ERROR: UB

  s = "hello";
  REQUIRE(s == "hello");
}

TEST_CASE("Move semantics w/names: swap", "[move]") {
  auto swap = [](string &a, string &b) {
    string tmp{std::move(a)};
    a = std::move(b);
    b = std::move(tmp);
  };
  string a{"_a"};
  string b{"_b"};
  swap(a, b);
  REQUIRE(a == "_b");
  REQUIRE(b == "_a");
}

TEST_CASE("Move semantics: vectors", "[move]") {

  vector<string> v;
  string s = getData();

  // push_back(const T &elem)
  // push_back(T&& elem)

  v.push_back(s);            // COPY
  v.push_back(getData());    // MOVE tmp to v
  v.push_back(s + s);        // MOVE tmp to v
  v.push_back(std::move(s)); // MOVE, no longer need

  string expect = getData();
  REQUIRE(v[0] == expect);
  REQUIRE(v[1] == expect);
  REQUIRE(v[2] == expect + expect);
  REQUIRE(v[3] == expect);
}

TEST_CASE("Move semantics: strings", "[move]") {
  class Str {
    size_t len;
    char *data;

  public:
    ~Str() {
      delete[] data;
      cout << "Str dtor\n";
    }

    Str(const char *s) : len{strlen(s)}, data{nullptr} {
      if (len == 0)
        return;
      data = new char[len + 1];
      memcpy(data, s, len + 1);
      cout << "Str from-c ctor\n";
    }

    // copy constructor uses lvalue ref
    Str(const Str &s) : len{s.len}, data{nullptr} {
      if (len == 0)
        return;
      data = new char[len + 1];
      memcpy(data, s.data, len + 1);
      cout << "Str copy ctor\n";
    }

    // move contructor uses rvalue ref
    Str(Str &&s) : len{s.len}, data{s.data} {
      s.len = 0;
      s.data = nullptr;
      cout << "Str move ctor\n";
    }

    bool operator==(const Str &s) const {
      if (len != s.len)
        return false;
      for (size_t i = 0; i < len; ++i)
        if (data[i] != s.data[i])
          return false;
      return true;
    }
  };

  Str expect{"123"};
  Str s{"123"};
  vector<Str> v;
  v.push_back(s);            // copy
  v.push_back(std::move(s)); // move

  REQUIRE(v[0] == expect);
}

TEST_CASE("Move semantics: overloading on refs", "[move]") {

  // void foo(const T &) - ro-access, can bind to everyth
  // void foo(T &) - w-access, only non-const named obj (lvalue)
  // void foo(T &&) - move-access, only obj w/o name o w/move (rvalue)

  // void foo(const T&&) - possible, but semantics contradiction, usually
  // converted to "const T&"

  const string s = getData(); // don't use const if you later move
  vector<string> v;
  v.push_back(std::move(s)); // just copies, not an error

  // const string getValue(); - don't return const by value
}

TEST_CASE("Move semantics for classes", "[move]") {
  // Guarantee for library objects:
  //   unless otherwise specified, after move objects
  //   in valid but unspecified state
  //
  // Copy as fallback
  //   may be disabled: thread, streams, unique_ptr
  //
  // Default move ops are generated automatically
  //   move ctor, move assignment operator
  //     move members
  //   but if only this can't be a problem, no user defined:
  //     copy ctor, assignment op, destructor
  //
  class Customer {
    string first;
    string last;
    int val;

  public:
    // move sematics auto enabled
    //  unless it not implementable (members movable)
    Customer(const string &f, const string &l, int v)
        : first{f}, last{l}, val{v} {}
    int getVal() const { return val; }

    // Customer(const Customer &cust) = default;
    //  - disables move semantics
    //    std::move fallbacks to copy
    //    maybe there is a reason why you don't
    //    use default-generated copying, maybe
    //    you need something special in move too.
    //
  };

  vector<Customer> v;
  Customer cust{"f", "l", 77};
  v.push_back(std::move(cust)); // moves
  // cust valid unspecified state
  REQUIRE(v[0].getVal() == 77);
}