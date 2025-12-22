// -*- coding: utf-8 -*-
#include <doctest/doctest.h>

#include <xnetwork/classes/graph.hpp>

TEST_CASE("Stress Test xnetwork::SimpleGraph") {
    constexpr auto num_nodes = 1000;
    auto gra = xnetwork::SimpleGraph{num_nodes};

    // Add a large number of edges
    for (auto idx = 0U; idx < num_nodes; ++idx) {
        for (auto jdx = idx + 1; jdx < num_nodes; ++jdx) {
            gra.add_edge(idx, jdx);
        }
    }

    // Check the degree of a node
    CHECK(gra.degree(0) == num_nodes - 1);
    CHECK(gra.degree(num_nodes - 1) == num_nodes - 1);

    // Clear the graph
    gra.clear();

    // Check that the graph is empty (@todo)
    // CHECK(gra.degree(0) == 0);
    // CHECK(gra.degree(num_nodes - 1) == 0);
}
