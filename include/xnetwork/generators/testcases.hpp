/**
 * @file testcases.hpp
 * @brief Test case generators for XNetwork algorithms
 *
 * This file contains functions to generate standard test cases
 * for testing graph algorithms.
 */

#pragma once
#include <array>
#include <xnetwork/classes/digraphs.hpp>

/**
 * @brief Create a test case with 5 nodes and 5 edges forming a cycle
 *
 * This function creates a directed graph with 5 nodes (A, B, C, D, E)
 * connected in a cycle: A->B->C->D->E->A. Edge weights are assigned
 * from the provided container.
 *
 * @tparam Container The type of container for weights (should support iteration)
 * @param weights A container of weights for the edges
 * @return xnetwork::SimpleDiGraphS A directed graph with the test structure
 *
 * @note The graph has the structure: A->B->C->D->E->A
 */
template <typename Container> inline auto create_test_case1(const Container& weights) {
    using Edge = std::pair<uint32_t, uint32_t>;
    constexpr uint32_t num_nodes = 5;
    enum nodes { A, B, C, D, E };
    const auto edges
        = std::array<Edge, 5>{Edge{A, B}, Edge{B, C}, Edge{C, D}, Edge{D, E}, Edge{E, A}};
    // constexpr auto weights = std::array<int, 5> {-5, 1, 1, 1, 1};
    auto gra = xnetwork::SimpleDiGraphS{num_nodes};
    gra.add_edges_from(edges, weights);
    return gra;
}

/**
 * @brief Create a test case with 3 nodes and bidirectional connections
 *
 * This function creates a directed graph with 3 nodes (A, B, C)
 * where each pair of nodes is connected in both directions.
 *
 * @tparam Container The type of container for weights (should support iteration)
 * @param weights A container of weights for the edges
 * @return xnetwork::SimpleDiGraphS A directed graph with the test structure
 *
 * @note The graph has the structure: A<->B<->C<->A (all bidirectional)
 */
template <typename Container> inline auto create_test_case2(const Container& weights) {
    using Edge = std::pair<uint32_t, uint32_t>;
    constexpr uint32_t num_nodes = 3;
    enum nodes { A, B, C };
    const auto edges = std::array<Edge, 6>{Edge{A, B}, Edge{B, A}, Edge{B, C},
                                           Edge{C, B}, Edge{C, A}, Edge{A, C}};
    auto gra = xnetwork::SimpleDiGraphS{num_nodes};
    gra.add_edges_from(edges, weights);
    return gra;
}

/**
 * @brief Create a timing test case with 3 nodes and bidirectional connections
 *
 * This function creates a directed graph with 3 nodes (A, B, C)
 * intended for timing tests, where each pair of nodes is connected
 * in both directions.
 *
 * @tparam Container The type of container for weights (should support iteration)
 * @param weights A container of weights for the edges
 * @return xnetwork::SimpleDiGraphS A directed graph with the test structure
 *
 * @note The graph has 6 edges: A<->B, B<->C, C<->A (all bidirectional)
 */
template <typename Container> inline auto create_test_case_timing(const Container& weights) {
    using Edge = std::pair<uint32_t, uint32_t>;
    constexpr uint32_t num_nodes = 3;
    enum nodes { A, B, C };
    const auto edges = std::array<Edge, 6>{Edge{A, B}, Edge{B, A}, Edge{B, C}, Edge{C, B},
                                           Edge{C, A}, Edge{A, C}};  // no multiple edges
    auto gra = xnetwork::SimpleDiGraphS{num_nodes};
    gra.add_edges_from(edges, weights);
    return gra;
}
