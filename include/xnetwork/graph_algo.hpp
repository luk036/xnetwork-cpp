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
    -> std::pair<CoverSet, typename WeightMap::mapped_type>;

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
                                 DepSet& dep) -> std::pair<IndSet, typename WeightMap::mapped_type>;

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
