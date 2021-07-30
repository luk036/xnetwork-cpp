/*
 *  Distributed under the MIT License (See accompanying file /LICENSE )
 */
#include <doctest/doctest.h>

#include <boost/multiprecision/cpp_int.hpp>
#include <ostream>
#include <py2cpp/fractions.hpp>

using namespace fun;
using boost::multiprecision::cpp_int;

// auto operator<=>(const cpp_int& a, const cpp_int& b) -> std::strong_ordering {
//     if (operator<(a, b)) return std::strong_ordering::less;
//     if (operator>(a, b)) return std::strong_ordering::greater;
//     return std::strong_ordering::equivalent;
// }

TEST_CASE("Fraction") {
    const auto a = cpp_int{3};
    const auto b = cpp_int{4};
    const auto c = cpp_int{5};
    const auto d = cpp_int{6};

    const auto p = Fraction{a, b};
    const auto q = Fraction{c, d};

    CHECK(p == Fraction<cpp_int>(30, 40));
    CHECK(p + q == Fraction<cpp_int>(19, 12));
    CHECK(p - q == Fraction<cpp_int>(-1, 12));
    CHECK(p != 0);
}

TEST_CASE("Fraction Special Cases") {
    const auto p = Fraction{3, 4};
    const auto inf = Fraction{1, 0};
    const auto nan = Fraction{0, 0};
    const auto zero = Fraction{0, 1};

    CHECK(-inf < zero);
    CHECK(zero < inf);
    CHECK(-inf < p);
    CHECK(p < inf);
    CHECK(inf == inf);
    CHECK(-inf < inf);
    CHECK(inf == inf * p);
    CHECK(inf == inf * inf);
    CHECK(inf == p / zero);
    CHECK(inf == inf / zero);
    CHECK(nan == nan);
    CHECK(nan == inf * zero);
    CHECK(nan == -inf * zero);
    CHECK(nan == inf / inf);
    CHECK(nan == nan * zero);
    CHECK(nan == nan * nan);
    CHECK(inf == inf + inf);
    CHECK(nan == inf - inf);
    // CHECK( inf + p == nan ); // ???
    // CHECK( -inf + p == nan ); // ???
}
