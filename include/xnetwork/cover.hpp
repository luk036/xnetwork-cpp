#pragma once

#include <algorithm>
#include <cassert>
#include <deque>
#include <optional>
#include <queue>
#include <utility>
#include <vector>

#include <py2cpp/dict.hpp>
#include <py2cpp/set.hpp>

/**
 * @brief Implements a primal-dual approximation algorithm for covering problems.
 *
 * @tparam MakeViolator Factory callable: make_violator() returns a "violator".
 *   The violator is called repeatedly; each call returns
 *   std::optional<std::vector<NodeType>> — the next violation,
 *   or std::nullopt when exhausted.
 * @tparam WeightMap Weight mapping (mutable)
 * @tparam SolutionSet Set-like container for the solution
 * @param make_violator Factory that creates fresh violators
 * @param weight Weight function for vertices
 * @param soln Solution set (will be modified)
 * @return std::pair<SolutionSet, typename WeightMap::mapped_type> Solution and total primal cost
 */
template <typename MakeViolator, typename WeightMap, typename SolutionSet>
auto pd_cover(MakeViolator make_violator, WeightMap& weight, SolutionSet& soln)
    -> std::pair<SolutionSet, typename WeightMap::mapped_type> {
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

            auto min_vtx
                = *std::min_element(violate_set.begin(), violate_set.end(),
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
        bool is_redundant = true;
        {
            auto check = make_violator();
            while (auto opt = check()) {
                if (!opt->empty()) {
                    is_redundant = false;
                    break;
                }
            }
        }
        if (!is_redundant) {
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
    -> std::pair<CoverSet, typename WeightMap::mapped_type> {
    using node_t = typename Graph::node_t;

    // Factory: returns a violator that lazily scans edges one at a time.
    // Each call yields the next uncovered edge.  The coverset is checked
    // at each step so that vertices added between calls are respected.
    auto make_violate_graph = [&]() {
        auto edge_list = ugraph.edges();
        return [&coverset, edge_list, idx = std::size_t{0}]() mutable
            -> std::optional<std::vector<node_t>> {
            while (idx < edge_list.size()) {
                const auto& [utx, vtx] = edge_list[idx];
                ++idx;
                if (!coverset.contains(utx) && !coverset.contains(vtx)) {
                    return std::vector<node_t>{utx, vtx};
                }
            }
            return std::nullopt;
        };
    };

    return pd_cover(make_violate_graph, weight, coverset);
}

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
    BFSInfo& operator=(const BFSInfo&) = default;
};

/**
 * @brief Constructs a cycle from BFS information
 *
 * @tparam Node Node type
 * @param info BFS information mapping
 * @param parent First node in cycle
 * @param child Second node in cycle
 * @return std::deque<Node> The constructed cycle
 */
template <typename Node> auto _construct_cycle(const py::dict<Node, BFSInfo<Node>>& info,
                                                Node parent, Node child) -> std::deque<Node> {
    const auto& info_parent = info.at(parent);
    const auto& info_child = info.at(child);

    Node node_a, node_b;
    int depth_a, depth_b;

    if (info_parent.depth < info_child.depth) {
        node_a = parent;
        depth_a = info_parent.depth;
        node_b = child;
        depth_b = info_child.depth;
    } else {
        node_a = child;
        depth_a = info_child.depth;
        node_b = parent;
        depth_b = info_parent.depth;
    }

    std::deque<Node> path;

    // Build path from node_a up to same depth as node_b
    while (depth_a < depth_b) {
        path.emplace_back(node_a);
        const auto& next_info = info.at(node_a);
        node_a = next_info.parent;
        depth_a = next_info.depth;
    }

    // Move both nodes up until they meet
    while (node_a != node_b) {
        path.emplace_back(node_a);
        path.push_front(node_b);

        const auto& info_a = info.at(node_a);
        const auto& info_b = info.at(node_b);

        node_a = info_a.parent;
        node_b = info_b.parent;
    }

    path.push_front(node_b);
    return path;
}

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
auto _generic_bfs_cycle(const Graph& ugraph, const CoverSet& coverset)
    -> std::vector<std::tuple<py::dict<typename Graph::node_t, BFSInfo<typename Graph::node_t>>,
                               typename Graph::node_t, typename Graph::node_t>> {
    using node_t = typename Graph::node_t;
    std::vector<std::tuple<py::dict<node_t, BFSInfo<node_t>>, node_t, node_t>> cycles;

    int depth_limit = static_cast<int>(ugraph.number_of_nodes());

    for (const auto& source : ugraph) {
        if (coverset.contains(source)) {
            continue;
        }

        py::dict<node_t, BFSInfo<node_t>> info;
        info.insert_or_assign(source, BFSInfo<node_t>(source, depth_limit));

        std::queue<node_t> queue;
        queue.push(source);

        while (!queue.empty()) {
            node_t parent = queue.front();
            queue.pop();

            const auto& parent_info = info.at(parent);
            node_t succ = parent_info.parent;
            int depth_now = parent_info.depth;

            for (const auto& child : ugraph[parent]) {
                if (coverset.contains(child)) {
                    continue;
                }

                if (!info.contains(child)) {
                    info.insert_or_assign(child, BFSInfo<node_t>(parent, depth_now - 1));
                    queue.push(child);
                    continue;
                }

                if (succ == child) {
                    continue;
                }

                // Cycle found
                cycles.emplace_back(info, parent, child);
            }
        }
    }

    return cycles;  // Empty if no cycles found
}

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
            auto cycles = _generic_bfs_cycle<Graph, CoverSet>(ugraph, coverset);
            if (cycles.empty()) return std::nullopt;
            const auto& [info, parent, child] = cycles[0];
            auto cycle_deque = _construct_cycle<node_t>(info, parent, child);
            return std::vector<node_t>(cycle_deque.begin(), cycle_deque.end());
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
    -> std::pair<CoverSet, typename WeightMap::mapped_type> {
    using node_t = typename Graph::node_t;

    // Factory: returns a violator that does a fresh BFS each call
    // and returns the first odd cycle found (or nullopt if none).
    auto make_violate = [&]() {
        return [&ugraph, &coverset]() -> std::optional<std::vector<node_t>> {
            auto cycles = _generic_bfs_cycle<Graph, CoverSet>(ugraph, coverset);
            if (cycles.empty()) return std::nullopt;
            for (const auto& [info, parent, child] : cycles) {
                const auto& info_parent = info.at(parent);
                const auto& info_child = info.at(child);
                if ((info_parent.depth - info_child.depth) % 2 == 0) {
                    auto cycle_deque = _construct_cycle<node_t>(info, parent, child);
                    return std::vector<node_t>(cycle_deque.begin(), cycle_deque.end());
                }
            }
            return std::nullopt;
        };
    };

    return pd_cover(make_violate, weight, coverset);
}

/**
 * @brief Overload without pre-existing coverset
 */
template <typename Graph, typename WeightMap>
auto min_odd_cycle_cover(const Graph& ugraph, WeightMap& weight)
    -> std::pair<py::set<typename Graph::node_t>, typename WeightMap::mapped_type> {
    py::set<typename Graph::node_t> coverset{};
    return min_odd_cycle_cover(ugraph, weight, coverset);
}
