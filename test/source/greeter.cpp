#include <doctest/doctest.h>
#include <xnetwork/greeter.hpp>
#include <xnetwork/version.h>

#include <string>

// TEST_CASE("XNCpp") {
//   using namespace xn;

//   XNCpp xn("Tests");

//   CHECK(xn.greet(LanguageCode::EN) == "Hello, Tests!");
//   CHECK(xn.greet(LanguageCode::DE) == "Hallo Tests!");
//   CHECK(xn.greet(LanguageCode::ES) == "¡Hola Tests!");
//   CHECK(xn.greet(LanguageCode::FR) == "Bonjour Tests!");
// }

TEST_CASE("XNCpp version") {
  static_assert(std::string_view(XNCPP_VERSION) == std::string_view("1.0"));
  CHECK(std::string(XNCPP_VERSION) == std::string("1.0"));
}
