// -*- coding: utf-8 -*-
#include <doctest/doctest.h>  // for ResultBuilder, TestCase, CHECK

#include <cstdint>                     // for uint8_t
#include <py2cpp/dict.hpp>             // for dict<>::Base
#include <py2cpp/set.hpp>              // for set
#include <tuple>                       // for tuple_element<>::type
#include <unordered_map>               // for operator!=
#include <vector>                      // for vector
#include <xnetwork/classes/graph.hpp>  // for Graph, SimpleGraph

// #if __cplusplus > 201703L
// #include <cppcoro/generator.hpp> // for operator!=, generator
// #endif

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

    // #if __cplusplus > 201703L
    //     auto count2 = 0;
    //     for ([[maybe_unused]] auto _ : gra.edges()) {
    //         ++count2;
    //     }
    //     CHECK(count2 > 0);
    // #endif
    //
    auto deg = 0U;
    for (auto _ : gra[1U]) {
        static_assert(sizeof _ >= 0, "unused");
        ++deg;
    }
    CHECK(gra.degree(1U) == deg);
}

/**
 * @brief Create a test netlist object
 *
 * @return Netlist
 */
TEST_CASE("Test xnetwork::Graph") {
    constexpr auto num_nodes = 6;
    enum nodes { a1, a2, a3, n1, n2, n3 };
    // const auto R = py::range<std::uint8_t>(0, num_nodes);
    auto gra = xnetwork::SimpleGraph{num_nodes};
    gra.add_edge(a1, n1);
    gra.add_edge(a1, n1);
    gra.add_edge(a1, n2);
    gra.add_edge(a2, n2);

    do_case(gra);
}

/**
 * @brief Create a test netlist object
 *
 * @return Netlist
 */
TEST_CASE("Test xnetwork::Graph (not simple graph)") {
    // constexpr auto num_nodes = 6;
    enum nodes_name { a1, a2, a3, n1, n2, n3 };

    std::vector<uint8_t> nodes{a2, a3, n1, n3, n2};

    // const auto R = py::range<std::uint8_t>(0, num_nodes);
    auto gra = xnetwork::Graph<std::vector<uint8_t>>{nodes};
    gra.add_edge(a2, n1);
    gra.add_edge(a3, n2);
    gra.add_edge(a3, n3);
    gra.add_edge(a2, n2);

    do_case(gra);
}
