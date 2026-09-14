/**
 * @file cover.hpp
 * @brief Primal-dual approximation algorithms for covering problems
 *
 * Implements a generic primal-dual cover algorithm (pd_cover) and specialized
 * functions for minimum vertex cover, minimum cycle cover, and minimum odd
 * cycle cover.
 */

#pragma once

#include <algorithm>
#include <cassert>
#include <deque>
#include <optional>
#include <py2cpp/dict.hpp>
#include <py2cpp/set.hpp>
#include <queue>
#include <utility>
#include <vector>

/**
 * @brief Implements a primal-dual approximation algorithm for covering problems.
 *
 * @dot
 *   digraph pd_flow {
 *     rankdir=LR; bgcolor="transparent";
 *     node [shape=box, style=filled, fillcolor="#d4e6f1"];
 *     init [label="Initialize gaps", fillcolor="#a9cce3"];
 *     pick [label="Pick min-gap\nvertex v in net"];
 *     cover [label="Add v to\ncover set"];
 *     update [label="Update gaps\ngap -= min_val"];
 *     check [label="More nets?", shape=diamond, fillcolor="#f9e79f"];
 *     done [label="Cover found!", fillcolor="#7fb3d8"];
 *     init -> pick -> cover -> update -> check;
 *     check -> pick [label="Yes", style=dashed, color="#e74c3c"];
 *     check -> done [label="No", color="#27ae60"];
 *   }
 * @enddot
 *
 * @tparam MakeViolator Factory callable: make_violator() returns a "violator".
 *   The violator is called repeatedly; each call returns
 *   std::optional<std::vector<NodeType>> - the next violation,
 *   or std::nullopt when exhausted.
 * @tparam WeightMap Weight mapping (mutable)
 * @tparam SolutionSet Set-like container for the solution
 * @tparam RedundantFunc Callable ``bool(node)``: true iff removing @p node keeps
 *   the solution valid. Lets callers supply a cheap local test instead of
 *   re-scanning every violation.
 * @param make_violator Factory that creates fresh violators
 * @param weight Weight function for vertices
 * @param soln Solution set (will be modified)
 * @param redundant Fast per-node redundancy predicate (see @p RedundantFunc)
 * @return std::pair<SolutionSet, typename WeightMap::mapped_type> Solution and total primal cost
 */
template <typename MakeViolator, typename WeightMap, typename SolutionSet, typename RedundantFunc>
auto pd_cover(MakeViolator make_violator, WeightMap& weight, SolutionSet& soln,
              RedundantFunc redundant) -> std::pair<SolutionSet, typename WeightMap::mapped_type> {
    using CostType = typename WeightMap::mapped_type;
    using NodeType = typename SolutionSet::value_type;

    CostType total_dual_cost = 0;
    auto gap = weight;  // copy weights
    std::vector<NodeType> added_order;

    // Phase 1: Primal-Dual Selection
    // Repeatedly call the violator for each violation, updating
    // coverset/gap between calls (lazy evaluation equivalent to the
    // original coroutine-based generator).
    {
        auto next = make_violator();
        while (auto opt = next()) {
            auto& violate_set = *opt;
            if (violate_set.empty()) continue;

            auto min_vtx = *std::min_element(
                violate_set.begin(), violate_set.end(),
                [&](const auto& v1, const auto& v2) { return gap[v1] < gap[v2]; });
            auto min_val = gap[min_vtx];

            if (!soln.contains(min_vtx)) {
                soln.insert(min_vtx);
                added_order.emplace_back(min_vtx);
            }

            total_dual_cost += min_val;

            for (const auto& vtx : violate_set) {
                gap[vtx] -= min_val;
            }
        }
    }

    // Phase 2: Reverse-Delete Post-Processing
    for (auto it = added_order.rbegin(); it != added_order.rend(); ++it) {
        soln.erase(*it);
        if (!redundant(*it)) {
            soln.insert(*it);
        }
    }

    CostType final_prml_cost = 0;
    for (const auto& vtx : soln) {
        final_prml_cost += weight[vtx];
    }

    assert(total_dual_cost <= final_prml_cost);
    return std::make_pair(soln, final_prml_cost);
}

/**
 * @brief Overload that validates each removal by re-running the violator.
 *
 * Slower than the explicit-predicate overload but works for any problem where
 * a cheap local redundancy test is not available.
 */
template <typename MakeViolator, typename WeightMap, typename SolutionSet>
auto pd_cover(MakeViolator make_violator, WeightMap& weight,
              SolutionSet& soln) -> std::pair<SolutionSet, typename WeightMap::mapped_type> {
    auto redundant = [&](const auto& /*vtx*/) -> bool {
        auto check = make_violator();
        while (auto opt = check()) {
            if (!opt->empty()) return false;
        }
        return true;
    };
    return pd_cover(make_violator, weight, soln, redundant);
}

/**
 * @brief Performs minimum weighted vertex cover using primal-dual approximation.
 *
 * @tparam Graph Graph type
 * @tparam WeightMap Weight map type
 * @tparam CoverSet Cover set type
 * @param ugraph Input graph
 * @param weight Weight function
 * @param coverset Cover set (will be modified)
 * @return std::pair<CoverSet, typename WeightMap::mapped_type> Cover set and total weight
 */
template <typename Graph, typename WeightMap, typename CoverSet>
auto min_vertex_cover(const Graph& ugraph, WeightMap& weight, CoverSet& coverset)
    -> std::pair<CoverSet, typename WeightMap::mapped_type>;

/**
 * @brief Overload without pre-existing coverset
 */
template <typename Graph, typename WeightMap>
auto min_vertex_cover(const Graph& ugraph, WeightMap& weight)
    -> std::pair<py::set<typename Graph::node_t>, typename WeightMap::mapped_type> {
    py::set<typename Graph::node_t> coverset{};
    return min_vertex_cover(ugraph, weight, coverset);
}

/**
 * @brief Information structure for BFS traversal
 */
template <typename Node> struct BFSInfo {
    Node parent;
    int depth;

    BFSInfo(Node p, int d) : parent(p), depth(d) {}
    BFSInfo(const BFSInfo&) = default;
    BFSInfo(BFSInfo&&) = default;
    BFSInfo& operator=(const BFSInfo&) = default;
    BFSInfo& operator=(BFSInfo&&) = default;
    ~BFSInfo() = default;
};

namespace detail {

    /**
     * @brief Visit every BFS back edge, one connected component at a time.
     *
     * Each component is traversed once (from its first uncovered vertex), so
     * the scan is O(V + E) instead of O(V * (V + E)). Calls @p accept with
     * (info, parent, child) for each back edge and stops as soon as @p accept
     * returns true.
     */
    template <typename Graph, typename CoverSet, typename Accept>
    void scan_cycles(const Graph& ugraph, const CoverSet& coverset, Accept&& accept) {
        using node_t = typename Graph::node_t;
        const int depth_limit = static_cast<int>(ugraph.number_of_nodes());
        py::set<node_t> visited;

        for (const auto& source : ugraph) {
            if (coverset.contains(source) || visited.contains(source)) continue;

            py::dict<node_t, BFSInfo<node_t>> info;
            info.insert_or_assign(source, BFSInfo<node_t>(source, depth_limit));
            visited.insert(source);

            std::queue<node_t> queue;
            queue.push(source);

            while (!queue.empty()) {
                node_t parent = queue.front();
                queue.pop();

                const auto& parent_info = info.at(parent);
                const node_t succ = parent_info.parent;
                const int depth_now = parent_info.depth;

                for (const auto& child : ugraph[parent]) {
                    if (coverset.contains(child)) continue;

                    if (!info.contains(child)) {
                        info.insert_or_assign(child, BFSInfo<node_t>(parent, depth_now - 1));
                        visited.insert(child);
                        queue.push(child);
                        continue;
                    }

                    if (succ == child) continue;

                    if (std::forward<Accept>(accept)(info, parent, child)) {
                        return;
                    }
                }
            }
        }
    }

}  // namespace detail

/**
 * @brief Constructs a cycle from BFS information
 *
 * @tparam Node Node type
 * @param info BFS information mapping
 * @param parent First node in cycle
 * @param child Second node in cycle
 * @return std::deque<Node> The constructed cycle
 */
template <typename Node> auto construct_cycle(const py::dict<Node, BFSInfo<Node>>& info,
                                              Node parent, Node child) -> std::deque<Node>;

/**
 * @brief Generic BFS cycle detection
 *
 * @tparam Graph Graph type
 * @tparam CoverSet Cover set type
 * @param ugraph Input graph
 * @param coverset Set of covered vertices (excluded from search)
 * @return std::vector<std::tuple<py::dict<typename Graph::node_t, BFSInfo<typename Graph::node_t>>,
 *                                typename Graph::node_t,
 *                                typename Graph::node_t>>
 *         Vector of (BFS info, parent, child) tuples for each cycle found
 */
template <typename Graph, typename CoverSet>
auto generic_bfs_cycle(const Graph& ugraph, const CoverSet& coverset)
    -> std::vector<std::tuple<py::dict<typename Graph::node_t, BFSInfo<typename Graph::node_t>>,
                              typename Graph::node_t, typename Graph::node_t>>;

/**
 * @brief Performs minimum cycle cover using primal-dual approximation.
 *
 * @tparam Graph Graph type
 * @tparam WeightMap Weight map type
 * @tparam CoverSet Cover set type
 * @param ugraph Input graph
 * @param weight Weight function
 * @param coverset Cover set (will be modified)
 * @return std::pair<CoverSet, typename WeightMap::mapped_type> Cover set and total weight
 */
template <typename Graph, typename WeightMap, typename CoverSet>
auto min_cycle_cover(const Graph& ugraph, WeightMap& weight, CoverSet& coverset)
    -> std::pair<CoverSet, typename WeightMap::mapped_type> {
    using node_t = typename Graph::node_t;

    // Factory: returns a violator that does a fresh BFS each call
    // and returns the first cycle found (or nullopt if none).
    auto make_violate = [&]() {
        return [&ugraph, &coverset]() -> std::optional<std::vector<node_t>> {
            std::optional<std::vector<node_t>> result;
            detail::scan_cycles<Graph, CoverSet>(
                ugraph, coverset, [&result](const auto& info, node_t parent, node_t child) {
                    auto cycle_deque = construct_cycle<node_t>(info, parent, child);
                    result = std::vector<node_t>(cycle_deque.begin(), cycle_deque.end());
                    return true;  // stop at the first cycle
                });
            return result;
        };
    };

    return pd_cover(make_violate, weight, coverset);
}

/**
 * @brief Overload without pre-existing coverset
 */
template <typename Graph, typename WeightMap>
auto min_cycle_cover(const Graph& ugraph, WeightMap& weight)
    -> std::pair<py::set<typename Graph::node_t>, typename WeightMap::mapped_type> {
    py::set<typename Graph::node_t> coverset{};
    return min_cycle_cover(ugraph, weight, coverset);
}

/**
 * @brief Performs minimum odd cycle cover using primal-dual approximation.
 *
 * @tparam Graph Graph type
 * @tparam WeightMap Weight map type
 * @tparam CoverSet Cover set type
 * @param ugraph Input graph
 * @param weight Weight function
 * @param coverset Cover set (will be modified)
 * @return std::pair<CoverSet, typename WeightMap::mapped_type> Cover set and total weight
 */
template <typename Graph, typename WeightMap, typename CoverSet>
auto min_odd_cycle_cover(const Graph& ugraph, WeightMap& weight, CoverSet& coverset)
    -> std::pair<CoverSet, typename WeightMap::mapped_type>;

/**
 * @brief Overload without pre-existing coverset
 */
template <typename Graph, typename WeightMap>
auto min_odd_cycle_cover(const Graph& ugraph, WeightMap& weight)
    -> std::pair<py::set<typename Graph::node_t>, typename WeightMap::mapped_type> {
    py::set<typename Graph::node_t> coverset{};
    return min_odd_cycle_cover(ugraph, weight, coverset);
}
