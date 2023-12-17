// -*- coding: utf-8 -*-
#include <doctest/doctest.h>  // for ResultBuilder, TestCase, CHECK

#include <array>                          // for array
#include <iosfwd>                         // for string
#include <string>                         // for basic_string, operator==
#include <utility>                        // for pair
#include <vector>                         // for vector
#include <xnetwork/classes/digraphs.hpp>  // for DiGraphS

template <typename Container> inline auto create_test_case4(const Container &weights) {
    using Edge = std::pair<std::string, std::string>;
    std::vector<std::string> nodes = {"A", "B", "C", "D", "E"};
    const auto edges = std::array<Edge, 5>{Edge{"A", "B"}, Edge{"B", "C"}, Edge{"C", "D"},
                                           Edge{"D", "E"}, Edge{"E", "A"}};
    // constexpr auto weights = std::array<int, 5> {-5, 1, 1, 1, 1};

    auto gra = xnetwork::DiGraphS<std::vector<std::string>>{nodes};
    gra.add_edges_from(edges, weights);
    return gra;
}

/**
 * @brief
 *
 * @tparam Graph
 * @param gra
 */
template <typename Graph> static void do_case(const Graph &gra) {
    auto count = 0U;
    for (auto _ : gra) {
        static_assert(sizeof _ >= 0, "unused");
        ++count;
    }

    CHECK(gra.number_of_nodes() == count);

    // auto count2 = 0U;
    // for ([[maybe_unused]] auto _ : gra.edges())
    // {
    //     ++count2;
    // }
    // CHECK(gra.number_of_edges() == count2);

    auto deg = 0U;
    for (auto _ : gra["B"]) {
        static_assert(sizeof _ >= 0, "unused");
        ++deg;
    }
    CHECK_EQ(gra.degree("B"), deg);
}

/**
 * @brief
 *
 */
TEST_CASE("Test xnetwork Negative Cycle") {
    auto weights = std::array<int, 5>{-5, 1, 1, 1, 1};
    auto gra = create_test_case4(weights);
    do_case(gra);
    // CHECK(hasNeg);
}
