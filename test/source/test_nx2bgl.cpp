// -*- coding: utf-8 -*-
#include <doctest/doctest.h>  // for ResultBuilder, TestCase

#include <boost/graph/adjacency_list.hpp>      // for adjacency_list, listS ...
#include <boost/graph/graph_selectors.hpp>     // for directedS
#include <boost/graph/graph_traits.hpp>        // for graph_traits, graph_tr...
#include <boost/graph/properties.hpp>          // for edge_index_t, edge_wei...
#include <boost/iterator/iterator_facade.hpp>  // for operator!=, operator==
#include <boost/move/utility_core.hpp>         // for move
#include <boost/pending/property.hpp>          // for property, no_property
#include <py2cpp/nx2bgl.hpp>                   // for grAdaptor
#include <type_traits>                         // for move
#include <utility>                             // for pair

using graph_t = boost::adjacency_list<
    boost::listS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_weight_t, int, boost::property<boost::edge_index_t, int>>>;
using Vertex = boost::graph_traits<graph_t>::vertex_descriptor;
using Edge_it = boost::graph_traits<graph_t>::edge_iterator;

static py::grAdaptor<graph_t> create_test_case1() {
    using Edge = std::pair<int, int>;
    const auto num_nodes = 5;
    enum nodes { A, B, C, D, E };
    static Edge edge_array[] = {Edge{A, B}, Edge{B, C}, Edge{C, D}, Edge{D, E}, Edge{E, A}};
    int weights[] = {-5, 1, 1, 1, 1};
    int num_arcs = sizeof(edge_array) / sizeof(Edge);
    auto g = graph_t(edge_array, edge_array + num_arcs, weights, num_nodes);
    return py::grAdaptor<graph_t>{std::move(g)};
}

static py::grAdaptor<graph_t> create_test_case2() {
    using Edge = std::pair<int, int>;
    const auto num_nodes = 5;
    enum nodes { A, B, C, D, E };
    static Edge edge_array[] = {Edge{A, B}, Edge{B, C}, Edge{C, D}, Edge{D, E}, Edge{E, A}};
    int weights[] = {2, 1, 1, 1, 1};
    int num_arcs = sizeof(edge_array) / sizeof(Edge);
    auto g = graph_t(edge_array, edge_array + num_arcs, weights, num_nodes);
    return py::grAdaptor<graph_t>{std::move(g)};
}

static auto create_test_case_timing() -> py::grAdaptor<graph_t> {
    using Edge = std::pair<int, int>;
    constexpr auto num_nodes = 3;
    enum nodes { A, B, C };
    static Edge edge_array[] = {Edge{A, B}, Edge{B, A}, Edge{B, C}, Edge{C, B},
                                Edge{B, C}, Edge{C, B}, Edge{C, A}, Edge{A, C}};
    int weights[] = {7, 0, 3, 1, 6, 4, 2, 5};
    constexpr int num_arcs = sizeof(edge_array) / sizeof(Edge);
    auto g = graph_t(edge_array, edge_array + num_arcs, weights, num_nodes);
    return py::grAdaptor<graph_t>{std::move(g)};
}

static auto create_test_case_timing2() -> py::grAdaptor<graph_t> {
    using Edge = std::pair<int, int>;
    constexpr auto num_nodes = 3;
    enum nodes { A, B, C };
    static Edge edge_array[] = {Edge{A, B}, Edge{B, A}, Edge{B, C}, Edge{C, B},
                                Edge{B, C}, Edge{C, B}, Edge{C, A}, Edge{A, C}};
    int weights[] = {3, -4, -1, -3, 2, 0, -2, 1};
    constexpr int num_arcs = sizeof(edge_array) / sizeof(Edge);
    auto g = graph_t(edge_array, edge_array + num_arcs, weights, num_nodes);
    return py::grAdaptor<graph_t>{std::move(g)};
}

auto do_case(const py::grAdaptor<graph_t>&) -> bool { return true; }

auto do_case_float(const py::grAdaptor<graph_t>&) -> bool { return true; }

TEST_CASE("Test Negative Cycle (boost)") {
    const auto G = create_test_case1();
    const auto hasNeg = do_case(G);
    CHECK(hasNeg);
}

TEST_CASE("Test No Negative Cycle (boost)") {
    const auto G = create_test_case2();
    const auto hasNeg = do_case(G);
    CHECK(hasNeg);
}

TEST_CASE("Test Timing Graph (boost)") {
    const auto G = create_test_case_timing();
    const auto hasNeg = do_case(G);
    CHECK(hasNeg);
}

TEST_CASE("Test Timing Graph 2 (boost)") {
    const auto G = create_test_case_timing2();
    const auto hasNeg = do_case(G);
    CHECK(hasNeg);
}

TEST_CASE("Test Timing Graph float (boost)") {
    const auto G = create_test_case_timing();
    const auto hasNeg = do_case_float(G);
    CHECK(hasNeg);
}

TEST_CASE("Test Timing Graph 2 (boost)") {
    const auto G = create_test_case_timing2();
    const auto hasNeg = do_case_float(G);
    CHECK(hasNeg);
}
