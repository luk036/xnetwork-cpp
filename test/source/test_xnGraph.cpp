// -*- coding: utf-8 -*-
#include <cinttypes>
#include <doctest/doctest.h>
#include <py2cpp/py2cpp.hpp>
#include <xnetwork/classes/graph.hpp>

/*!
 * @brief
 *
 * @tparam Graph
 * @param G
 */
template <typename Graph>
static void do_case(const Graph& G)
{
    auto count = 0U;
    for ([[maybe_unused]] auto _ : G)
    {
        ++count;
    }
    CHECK(G.number_of_nodes() == count);

    auto count2 = 0U;
    for ([[maybe_unused]] auto _ : G.edges())
    {
        ++count2;
    }
    // CHECK(G.number_of_edges() == count2);

    auto deg = 0U;
    for ([[maybe_unused]] auto _ : G[1U])
    {
        ++deg;
    }
    CHECK(G.degree(1U) == deg);
}

/**
 * @brief Create a test netlist object
 *
 * @return Netlist
 */
TEST_CASE("Test xn::Graph")
{
    constexpr auto num_nodes = 6;
    enum nodes
    {
        a1,
        a2,
        a3,
        n1,
        n2,
        n3
    };
    // const auto R = py::range<std::uint8_t>(0, num_nodes);
    auto G = xn::SimpleGraph {num_nodes};
    G.add_edge(a1, n1);
    G.add_edge(a1, n1);
    G.add_edge(a1, n2);
    G.add_edge(a2, n2);

    do_case(G);
}


/**
 * @brief Create a test netlist object
 *
 * @return Netlist
 */
TEST_CASE("Test xn::Graph (not simple graph)")
{
    // constexpr auto num_nodes = 6;
    enum nodes_name
    {
        a1,
        a2,
        a3,
        n1,
        n2,
        n3
    };

    std::vector<uint8_t> nodes{a2, a3, n1, n3, n2};

    // const auto R = py::range<std::uint8_t>(0, num_nodes);
    auto G = xn::Graph {nodes};
    G.add_edge(a2, n1);
    G.add_edge(a3, n2);
    G.add_edge(a3, n3);
    G.add_edge(a2, n2);

    do_case(G);
}
