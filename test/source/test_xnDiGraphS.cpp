// -*- coding: utf-8 -*-
#include <doctest/doctest.h>  // for ResultBuilder, TestCase, CHECK

#include <array>                          // for array
#include <cstdint>                        // for uint32_t
#include <string>                         // for basic_string, operator==
#include <utility>                        // for pair
#include <vector>                         // for vector
#include <xnetwork/classes/digraphs.hpp>  // for DiGraphS

template <typename Container> inline auto create_test_case4(const Container& weights) {
    using Edge = std::pair<std::string, std::string>;
    std::vector<std::string> nodes = {"A", "B", "C", "D", "E"};
    const auto edges = std::array<Edge, 5>{Edge{"A", "B"}, Edge{"B", "C"}, Edge{"C", "D"},
                                           Edge{"D", "E"}, Edge{"E", "A"}};

    auto gra = xnetwork::DiGraphS<std::vector<std::string>>{nodes};
    gra.add_edges_from(edges, weights);
    return gra;
}

/**
 * @brief
 *
 * @tparam Graph
 * @param[in] gra
 */
template <typename Graph> static void do_case(const Graph& gra) {
    auto count = 0U;
    for ([[maybe_unused]] const auto& itr : gra) {
        ++count;
    }

    CHECK_EQ(gra.number_of_nodes(), count);

    auto deg = 0U;
    for ([[maybe_unused]] const auto& itr : gra["B"]) {
        ++deg;
    }
    CHECK_EQ(gra.degree("B"), deg);
}

/**
 * @brief
 *
 */
TEST_CASE("Test xnetwork::DiGraphS iteration") {
    auto weights = std::array<int, 5>{-5, 1, 1, 1, 1};
    auto gra = create_test_case4(weights);
    do_case(gra);
}

TEST_CASE("Test xnetwork::DiGraphS directed basics") {
    std::vector<uint32_t> nodes{0, 1, 2};
    xnetwork::DiGraphS<std::vector<uint32_t>> gra{nodes};
    gra.add_edge(0, 1);
    gra.add_edge(1, 0);
    gra.add_edge(1, 2);

    CHECK(gra.is_directed());
    CHECK(gra.has_successor(0, 1));
    CHECK(gra.has_successor(1, 0));
    CHECK_FALSE(gra.has_successor(0, 2));
    CHECK_EQ(gra.number_of_edges(), 3);

    gra.clear();
    CHECK_EQ(gra.number_of_edges(), 0);
    CHECK_EQ(gra.number_of_nodes(), 3);
}
