#include <catch2/catch_test_macros.hpp>

TEST_CASE("Api design: nodiscard", "[nodiscard]") {

  //     [[nodiscard]] should be used in any non-mutating
  struct Vec {
    [[nodiscard]] bool is_empty() const noexcept { return true; }
  };

  Vec v;
  REQUIRE(v.is_empty());
}

void noexcept_func() noexcept { throw 42; }

TEST_CASE("Api design: noexcept", "[noexcept]") {

  try {
    // noexcept_func();
  } catch (...) {
    // terminate called earlier
  }
}

TEST_CASE("Api design: factory func", "[factory func]") {

  // Widget *make_widget(int widget_type)

  // [[nodiscard]] std::unique_ptr<Widget> make_widget(WidgetType type)
}