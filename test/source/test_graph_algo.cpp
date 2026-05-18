#include <doctest/doctest.h>

#include <cstdint>
#include <py2cpp/dict.hpp>
#include <py2cpp/set.hpp>
#include <utility>  // for std::pair

#include <xnetwork/graph_algo.hpp>
#include <xnetwork/classes/graph.hpp>  // for SimpleGraph

TEST_CASE("Test min_vertex_cover_fast - Basic Example 1") {
    // Create graph: 0-1-2 with all weights 1
    xnetwork::SimpleGraph ugraph(3);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    py::dict<uint32_t, int> weight;
    weight[0] = 1;
    weight[1] = 2;
    weight[2] = 1;

    auto [coverset, total_weight] = min_vertex_cover_fast(ugraph, weight);

    // Expected: {0, 2} with total weight 2
    CHECK_EQ(total_weight, 2);
    CHECK(coverset.contains(0));
    CHECK(coverset.contains(2));
    CHECK_FALSE(coverset.contains(1));
}

TEST_CASE("Test min_vertex_cover_fast - Basic Example 2") {
    // Create a small graph: triangle
    xnetwork::SimpleGraph ugraph(3);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    ugraph.add_edge(2, 0);
    py::dict<uint32_t, int> weight;
    weight[0] = 1;
    weight[1] = 1;
    weight[2] = 1;

    auto [coverset, total_weight] = min_vertex_cover_fast(ugraph, weight);

    // Should cover at least 2 vertices in a triangle
    CHECK_GE(coverset.size(), 2);
    CHECK_GE(total_weight, 2);
}

TEST_CASE("Test min_vertex_cover_fast - With Pre-existing Coverset") {
    xnetwork::SimpleGraph ugraph(4);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    ugraph.add_edge(2, 3);
    py::dict<uint32_t, int> weight;
    weight[0] = 1;
    weight[1] = 1;
    weight[2] = 1;
    weight[3] = 1;

    py::set<uint32_t> coverset;
    coverset.insert(1);  // Pre-cover vertex 1

    auto [result_set, total_weight] = min_vertex_cover_fast(ugraph, weight, coverset);

    // Vertex 1 should still be in the cover set
    CHECK(result_set.contains(1));
    CHECK_GE(total_weight, 1);
}

TEST_CASE("Test min_maximal_independant_set - Basic Example 1") {
    // Create graph: 0-1-2
    xnetwork::SimpleGraph ugraph(3);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    py::dict<uint32_t, int> weight;
    weight[0] = 1;
    weight[1] = 2;
    weight[2] = 1;

    auto [indset, total_weight] = min_maximal_independant_set(ugraph, weight);

    // Expected: {0, 2} with total weight 2
    CHECK_EQ(total_weight, 2);
    CHECK(indset.contains(0));
    CHECK(indset.contains(2));
    CHECK_FALSE(indset.contains(1));
}

TEST_CASE("Test min_maximal_independant_set - Basic Example 2") {
    // Create a small graph
    xnetwork::SimpleGraph ugraph(5);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(0, 2);
    ugraph.add_edge(1, 2);
    ugraph.add_edge(1, 3);
    ugraph.add_edge(2, 3);
    ugraph.add_edge(2, 4);
    ugraph.add_edge(3, 4);
    py::dict<uint32_t, int> weight;
    for (uint32_t i = 0; i < 5; ++i) {
        weight[i] = 1;
    }

    auto [indset, total_weight] = min_maximal_independant_set(ugraph, weight);

    // Should be an independent set (no adjacent vertices)
    for (const auto& u : indset) {
        for (const auto& v : indset) {
            if (u != v) {
                bool adjacent = false;
                for (const auto& neighbor : ugraph[u]) {
                    if (neighbor == v) {
                        adjacent = true;
                        break;
                    }
                }
                CHECK_FALSE(adjacent);
            }
        }
    }

    // Should be maximal (no vertex can be added without violating independence)
    for (uint32_t i = 0; i < 5; ++i) {
        if (!indset.contains(i)) {
            bool can_add = true;
            for (const auto& neighbor : ugraph[i]) {
                if (indset.contains(neighbor)) {
                    can_add = false;
                    break;
                }
            }
            CHECK_FALSE(can_add);
        }
    }
}

TEST_CASE("Test min_maximal_independant_set - With Pre-existing Sets") {
    xnetwork::SimpleGraph ugraph(4);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    ugraph.add_edge(2, 3);
    py::dict<uint32_t, int> weight;
    weight[0] = 1;
    weight[1] = 1;
    weight[2] = 1;
    weight[3] = 1;

    py::set<uint32_t> indset;
    indset.insert(0);  // Pre-defined independent vertex
    py::set<uint32_t> dep;

    auto [result_set, total_weight] = min_maximal_independant_set(ugraph, weight, indset, dep);

    // Vertex 0 should still be in the independent set
    CHECK(result_set.contains(0));
    // Vertex 1 should be in dep (adjacent to 0)
    CHECK(dep.contains(1));
}

TEST_CASE("Test min_vertex_cover_fast - Weighted Example") {
    // Test with different weights
    xnetwork::SimpleGraph ugraph(3);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    py::dict<uint32_t, int> weight;
    weight[0] = 3;
    weight[1] = 1;
    weight[2] = 2;

    auto [coverset, total_weight] = min_vertex_cover_fast(ugraph, weight);

    // Algorithm should prefer cheaper vertices
    // Expected: cover vertex 1 (weight 1) to cover both edges
    CHECK_EQ(total_weight, 1);
    CHECK(coverset.contains(1));
    CHECK_FALSE(coverset.contains(0));
    CHECK_FALSE(coverset.contains(2));
}

TEST_CASE("Test min_maximal_independant_set - Weighted Example") {
    // Test with different weights
    xnetwork::SimpleGraph ugraph(3);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    py::dict<uint32_t, int> weight;
    weight[0] = 1;
    weight[1] = 3;
    weight[2] = 2;

    auto [indset, total_weight] = min_maximal_independant_set(ugraph, weight);

    // Algorithm should prefer cheaper vertices for independent set
    // Expected: {0, 2} with total weight 3
    CHECK_EQ(total_weight, 3);
    CHECK(indset.contains(0));
    CHECK(indset.contains(2));
    CHECK_FALSE(indset.contains(1));
}

// TEST_CASE("Test Both Algorithms - Small Graph") {
//     // Test both algorithms on the same small graph
//     xnetwork::SimpleGraph ugraph(4);
//     ugraph.add_edge(0, 1);
//     ugraph.add_edge(1, 2);
//     ugraph.add_edge(2, 3);
//     ugraph.add_edge(3, 0);
//     py::dict<uint32_t, int> weight;
//     for (uint32_t i = 0; i < 4; ++i) {
//         weight[i] = 1;
//     }

//     auto [cover_set, cover_weight] = min_vertex_cover_fast(ugraph, weight);
//     auto [ind_set, ind_weight] = min_maximal_independant_set(ugraph, weight);

//     // In any graph, vertex cover + independent set should cover all vertices
//     for (uint32_t i = 0; i < 4; ++i) {
//         auto expression = cover_set.contains(i) || ind_set.contains(i);
//         CHECK(expression);
//     }
// }

TEST_CASE("Test Empty Graph") {
    xnetwork::SimpleGraph ugraph(0);
    py::dict<uint32_t, int> weight;

    auto [cover_set, cover_weight] = min_vertex_cover_fast(ugraph, weight);
    auto [ind_set, ind_weight] = min_maximal_independant_set(ugraph, weight);

    CHECK_EQ(cover_weight, 0);
    CHECK_EQ(ind_weight, 0);
    CHECK(cover_set.empty());
    CHECK(ind_set.empty());
}

TEST_CASE("Test Single Vertex Graph") {
    xnetwork::SimpleGraph ugraph(1);
    py::dict<uint32_t, int> weight;
    weight[0] = 5;

    auto [cover_set, cover_weight] = min_vertex_cover_fast(ugraph, weight);
    auto [ind_set, ind_weight] = min_maximal_independant_set(ugraph, weight);

    // For single vertex with no edges, vertex cover should be empty
    // Independent set should contain the vertex
    CHECK_EQ(cover_weight, 0);
    CHECK(cover_set.empty());
    CHECK_EQ(ind_weight, 5);
    CHECK(ind_set.contains(0));
}
