// -*- coding: utf-8 -*-
#include <array>
#include <doctest/doctest.h>
#include <string>
#include <vector>
#include <xnetwork/classes/digraphs.hpp>

template <typename Container>
inline auto create_test_case4(const Container& weights)
{
    using Edge = std::pair<std::string, std::string>;
    std::vector<std::string> nodes = {"A", "B", "C", "D", "E"};
    const auto edges = std::array<Edge, 5> {Edge {"A", "B"}, Edge {"B", "C"},
        Edge {"C", "D"}, Edge {"D", "E"}, Edge {"E", "A"}};
    // constexpr auto weights = std::array<int, 5> {-5, 1, 1, 1, 1};

    auto G = xn::DiGraphS {nodes};
    G.add_edges_from(edges, weights);
    return G;
}

/*!
 * @brief
 *
 * @tparam Graph
 * @param G
 * @return true
 * @return false
 */
template <typename Graph>
void do_case(const Graph& )
{
    // const auto get_weight = [&](const auto& edge) -> int {
    //     const auto [u, v] = G.end_points(edge);
    //     return G[u][v];
    // };

    // auto dist = py::dict<std::string, int> {};
    // for (auto&& v : G)
    // {
    //     dist[v] = 0;
    // }
    // auto N = negCycleFinder<Graph>(G);
    // const auto cycle = N.find_neg_cycle(std::move(dist), get_weight);
    // return !cycle.empty();
}


/*!
 * @brief
 *
 */
TEST_CASE("Test xnetwork Negative Cycle")
{
    auto weights = std::array<int, 5> {-5, 1, 1, 1, 1};
    auto G = create_test_case4(weights);
    do_case(G);
    // CHECK(hasNeg);
}

/*!
 * @brief
 *
 */
TEST_CASE("Test No Negative Cycle")
{
    auto weights = std::array<int, 5> {2, 1, 1, 1, 1};
    auto G = create_test_case4(weights);
    do_case(G);
    // CHECK(!hasNeg);
}

// /*!
//  * @brief
//  *
//  */
// TEST_CASE("Test Timing Graph")
// {
//     auto weights = std::array<int, 8> {7, 0, 3, 1, 6, 4, 2, 5};
//     auto G = create_test_case_timing(weights);
//     const auto hasNeg = do_case(G);
//     CHECK(!hasNeg);
// }

// /*!
//  * @brief
//  *
//  */
// TEST_CASE("Test Timing Graph (2)")
// {
//     auto weights = std::array<int, 8> {3, -4, -1, -1, 2, 0, -2, 1};
//     auto G = create_test_case_timing(weights);
//     const auto hasNeg = do_case(G);
//     CHECK(hasNeg);
// }
