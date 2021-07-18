#include <doctest/doctest.h>

#include <array>
#include <py2cpp/py2cpp.hpp>
// #include <range/v3/view/all.hpp>
// #include <range/v3/view/remove_if.hpp>
// #include <transranger_view.hpp>
// #include <transrangers.hpp>
#include <unordered_set>
#include <vector>

TEST_CASE("Test Range") {
    // using namespace transrangers;

    // auto S = std::vector<int>{1, 2, 3, 4};
    // auto is_odd = [](int a) { return a % 2 == 1; };
    // auto rng = filter(is_odd, all(S));
    // auto total = accumulate(rng, 0);
    // for (const auto& e : input_view(rng)) {
    //     total += e;
    // }

    const auto R = py::range(10);

    // CHECK(!R.empty());
    // CHECK(R.contains(4U));
    // CHECK(R[3] == 3);

    auto count = 0;
    for ([[maybe_unused]] auto a : R) {
        ++count;
    }
    CHECK(count == R.size());
}

TEST_CASE("Test Range2") {
    const auto R = py::range(-10, 10);

    CHECK(!R.empty());
    CHECK(R.contains(4));
    // CHECK(R[3] == -7);

    auto count = 0;
    for ([[maybe_unused]] auto a : R) {
        ++count;
    }
    CHECK(count == R.size());
}

TEST_CASE("Test Range (char)") {
    const auto R = py::range('A', 'W');

    // CHECK(!R.empty());
    CHECK(R.contains('B'));
    // CHECK(R[3] == 'D');

    auto count = 0;
    for ([[maybe_unused]] auto a : R) {
        ++count;
    }
    CHECK(count == R.size());
}

// TEST_CASE("Test Range (pointer)")
// {
//     auto A = std::array<double, 4> {0.2, 0.4, 0.1, 0.9};
//     auto R = py::range(&A[0], &A[0] + 4);

//     CHECK(!R.empty());
//     // CHECK(R.contains(&A + 2));
//     CHECK(R[1] == &A[0] + 1);

//     auto count = 0;
//     for ([[maybe_unused]] auto _ : R)
//     {
//         ++count;
//     }
//     CHECK(count == R.size());
// }
