#include <doctest/doctest.h>
#include <xnetwork/version.h>

#include <cppcoro/generator.hpp>
#include <string>
#include <xnetwork/greeter.hpp>

using namespace cppcoro;
using ret_t = std::tuple<int, int>;

/**
 * @brief Generate all permutations by adjacent transposition
 *
 * @param n
 * @return cppcoro::generator<int>
 */
auto SJT_gen(int n) -> cppcoro::generator<int> {
    /** Generate the swaps for the Steinhaus-Johnson-Trotter algorithm.*/
    if (n == 2) {
        co_yield 0;
        co_yield 0;  // tricky part: return to the original list
        co_return;
    }

    auto&& gen = SJT_gen(n - 1);
    for (auto it = gen.begin(); it != gen.end(); ++it) {
        for (auto i = n - 2; i != -1; --i) {  // downward
            co_yield i;
        }
        co_yield 1 + *it;
        for (auto i = 0; i != n - 1; ++i) {  // upward
            co_yield i;
        }
        co_yield *(++it);  // tricky part
    }
}

TEST_CASE("Coroutine") {
    int cnt = 0;  // start from 0
    for ([[maybe_unused]] auto i : SJT_gen(6)) {
        ++cnt;
    }
    CHECK(cnt == 720);
}

// TEST_CASE("XNetwork") {
//   using namespace xnetwork;

//   XNetwork xn("Tests");

//   CHECK(xn.greet(LanguageCode::EN) == "Hello, Tests!");
//   CHECK(xn.greet(LanguageCode::DE) == "Hallo Tests!");
//   CHECK(xn.greet(LanguageCode::ES) == "¡Hola Tests!");
//   CHECK(xn.greet(LanguageCode::FR) == "Bonjour Tests!");
// }

TEST_CASE("XNetwork version") {
    static_assert(std::string_view(XNETWORK_VERSION) == std::string_view("1.0"));
    CHECK(std::string(XNETWORK_VERSION) == std::string("1.0"));
}
