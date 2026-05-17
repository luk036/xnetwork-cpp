#pragma once

#include <cassert>
#include <py2cpp/dict.hpp>
#include <py2cpp/set.hpp>
#include <utility>  // for std::pair

/**
 * @brief Performs minimum weighted vertex cover using a primal-dual approximation algorithm
 *
 * This function implements a primal-dual algorithm for minimum weighted vertex cover on a graph.
 *
 * @tparam Graph The graph type
 * @tparam WeightMap The weight map type (mutable mapping from node to weight)
 * @tparam CoverSet The cover set type
 * @param ugraph The input graph
 * @param weight The weight function for vertices
 * @param coverset The vertex cover set (will be modified)
 * @return std::pair<CoverSet, typename WeightMap::mapped_type> The cover set and total weight
 */
template <typename Graph, typename WeightMap, typename CoverSet>
auto min_vertex_cover_fast(const Graph& ugraph, WeightMap& weight, CoverSet& coverset)
    -> std::pair<CoverSet, typename WeightMap::mapped_type> {
    using CostType = typename WeightMap::mapped_type;

    // Create a copy of weights for the gap values
    auto gap = weight;

    CostType total_dual_cost = 0;
    CostType total_prml_cost = 0;

    // Iterate through all edges in the graph
    for (const auto& edge : ugraph.edges()) {
        auto utx = edge.first;
        auto vtx = edge.second;

        if (coverset.contains(utx) || coverset.contains(vtx)) {
            continue;
        }

        if (gap[utx] < gap[vtx]) {
            std::swap(utx, vtx);
        }

        coverset.insert(vtx);
        total_dual_cost += gap[vtx];
        total_prml_cost += weight[vtx];
        gap[utx] -= gap[vtx];
        gap[vtx] = 0;
    }

    assert(total_dual_cost <= total_prml_cost);
    return std::make_pair(coverset, total_prml_cost);
}

/**
 * @brief Overload that creates an empty coverset
 */
template <typename Graph, typename WeightMap>
auto min_vertex_cover_fast(const Graph& ugraph, WeightMap& weight)
    -> std::pair<py::set<typename Graph::node_t>, typename WeightMap::mapped_type> {
    py::set<typename Graph::node_t> coverset{};
    return min_vertex_cover_fast(ugraph, weight, coverset);
}

/**
 * @brief Performs minimum weighted maximal independent set using primal-dual algorithm
 *
 * This function implements a primal-dual algorithm for minimum weighted maximal independent set.
 *
 * @tparam Graph The graph type
 * @tparam WeightMap The weight map type
 * @tparam IndSet The independent set type
 * @tparam DepSet The dependent set type
 * @param ugraph The input graph
 * @param weight The weight function for vertices
 * @param indset The independent set (will be modified)
 * @param dep The dependent set (will be modified)
 * @return std::pair<IndSet, typename WeightMap::mapped_type> The independent set and total weight
 */
template <typename Graph, typename WeightMap, typename IndSet, typename DepSet>
auto min_maximal_independant_set(const Graph& ugraph, WeightMap& weight, IndSet& indset,
                                 DepSet& dep)
    -> std::pair<IndSet, typename WeightMap::mapped_type> {
    using node_t = typename Graph::node_t;
    using CostType = typename WeightMap::mapped_type;

    // Helper function to mark a vertex and its neighbors as dependent
    auto coverset = [&](node_t utx) {
        dep.insert(utx);
        for (const auto& vtx : ugraph[utx]) {
            dep.insert(vtx);
        }
    };

    auto gap = weight;
    CostType total_prml_cost = 0;
    CostType total_dual_cost = 0;

    // Iterate through all vertices in the graph
    for (const auto& utx : ugraph) {
        if (dep.contains(utx)) {
            continue;
        }
        if (indset.contains(utx)) {  // pre-defined independent
            continue;
        }

        CostType min_val = gap[utx];
        node_t min_vtx = utx;

        // Find the minimum gap vertex among neighbors
        for (const auto& vtx : ugraph[utx]) {
            if (dep.contains(vtx)) {
                continue;
            }
            if (min_val > gap[vtx]) {
                min_val = gap[vtx];
                min_vtx = vtx;
            }
        }

        indset.insert(min_vtx);
        coverset(min_vtx);
        total_prml_cost += weight[min_vtx];
        total_dual_cost += min_val;

        if (min_vtx == utx) {
            continue;
        }

        // Update gaps for neighbors
        for (const auto& vtx : ugraph[utx]) {
            gap[vtx] -= min_val;
        }
    }

    assert(total_dual_cost <= total_prml_cost);
    return std::make_pair(indset, total_prml_cost);
}

/**
 * @brief Overload that creates empty indset and dep sets
 */
template <typename Graph, typename WeightMap>
auto min_maximal_independant_set(const Graph& ugraph, WeightMap& weight)
    -> std::pair<py::set<typename Graph::node_t>, typename WeightMap::mapped_type> {
    py::set<typename Graph::node_t> indset{};
    py::set<typename Graph::node_t> dep{};
    return min_maximal_independant_set(ugraph, weight, indset, dep);
}
