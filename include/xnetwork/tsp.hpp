#pragma once

/**
 * @file tsp.hpp
 * @brief Traveling Salesman Problem (TSP) approximation algorithms
 *
 * Implements the Christofides algorithm (3/2-approximation for Metric TSP)
 * and the 2-Opt local search heuristic for tour refinement.
 *
 * All functions accept a callable ``weight(u, v) -> double`` for edge
 * weights, making them agnostic to the underlying graph representation.
 *
 * Algorithm summary (Christofides, 1976):
 *   1. Compute a Minimum Spanning Tree (MST).
 *   2. Find odd-degree vertices in the MST.
 *   3. Compute a Minimum Weight Perfect Matching on odd vertices.
 *   4. Superimpose MST and matching -> an Eulerian multigraph.
 *   5. Find an Eulerian circuit (Hierholzer).
 *   6. Shortcut repeated vertices -> Hamiltonian cycle.
 *
 * Combining with 2-Opt refinement typically yields near-optimal tours
 * for moderate-size metric instances.
 */

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <limits>
#include <py2cpp/range.hpp>
#include <py2cpp/set.hpp>
#include <set>
#include <tuple>
#include <vector>
#include <xnetwork/classes/graph.hpp>

// ---------------------------------------------------------------------------
// Helper: calculate_total_distance
// ---------------------------------------------------------------------------

/**
 * @brief Compute the total length of a Hamiltonian path/cycle.
 *
 * @tparam Node     node type (must be usable as an index)
 * @tparam WeightFunc  callable ``double(Node, Node)``
 * @param path      sequence of nodes (last == first for a cycle)
 * @param weight    edge-weight function
 * @return double   sum of edge weights along the path
 */
template <typename Node, typename WeightFunc>
double calculate_total_distance(const std::vector<Node>& path, WeightFunc&& weight) {
    double dist = 0.0;
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        dist += std::forward<WeightFunc>(weight)(path[i], path[i + 1]);
    }
    return dist;
}

// ---------------------------------------------------------------------------
// Internal helpers for the Christofides algorithm
// ---------------------------------------------------------------------------

namespace detail {

    /**
     * @brief Prim's MST for dense (complete) graphs - O(n^2).
     *
     * @tparam Node     integral node type
     * @tparam WeightFunc  callable ``double(Node, Node)``
     * @param n        number of vertices (assumed 0 .. n-1)
     * @param weight   edge-weight function
     * @return vector of undirected edges forming the MST
     */
    template <typename Node, typename WeightFunc> auto prim_mst(size_t n, WeightFunc&& weight)
        -> std::vector<std::pair<Node, Node>> {
        constexpr double INF = std::numeric_limits<double>::max();

        std::vector<bool> in_mst(n, false);
        std::vector<double> key(n, INF);
        std::vector<Node> parent(n, Node{0});

        key[0] = 0.0;

        for (size_t count = 0; count < n; ++count) {
            // Find the vertex with smallest key not yet in MST
            double best = INF;
            Node u = Node{0};
            for (Node v = Node{0}; v < static_cast<Node>(n); ++v) {
                if (!in_mst[static_cast<size_t>(v)] && key[static_cast<size_t>(v)] < best) {
                    best = key[static_cast<size_t>(v)];
                    u = v;
                }
            }
            if (best == INF) break;  // isolated vertex (shouldn't happen for complete graphs)

            in_mst[static_cast<size_t>(u)] = true;

            // Relaxation
            for (Node v = Node{0}; v < static_cast<Node>(n); ++v) {
                if (!in_mst[static_cast<size_t>(v)]) {
                    const double w = std::forward<WeightFunc>(weight)(u, v);
                    auto& kv = key[static_cast<size_t>(v)];
                    if (w < kv) {
                        kv = w;
                        parent[static_cast<size_t>(v)] = u;
                    }
                }
            }
        }

        std::vector<std::pair<Node, Node>> edges;
        edges.reserve(n - 1);
        for (Node v = Node{1}; v < static_cast<Node>(n); ++v) {
            const auto p = parent[static_cast<size_t>(v)];
            if (p != v)  // safety
                edges.emplace_back(p, v);
        }
        return edges;
    }

    /**
     * @brief Collect vertices with odd degree in the MST.
     */
    template <typename Node>
    auto find_odd_degree_nodes(const std::vector<std::pair<Node, Node>>& mst_edges, size_t n)
        -> std::vector<Node> {
        std::vector<int> deg(n, 0);
        for (const auto& [u, v] : mst_edges) {
            ++deg[static_cast<size_t>(u)];
            ++deg[static_cast<size_t>(v)];
        }
        std::vector<Node> odd;
        for (Node v = Node{0}; v < static_cast<Node>(n); ++v) {
            if (deg[static_cast<size_t>(v)] % 2 != 0) odd.push_back(v);
        }
        return odd;
    }

    /**
     * @brief Greedy minimum-weight perfect matching - O(k^2 log k).
     *
     * Builds a sorted list of all candidate edges among ``odd_nodes`` and
     * greedily picks the cheapest non-adjacent pair.  This is a heuristic;
     * for a provable 3/2 bound a blossom algorithm would be required.
     */
    template <typename Node, typename WeightFunc>
    auto greedy_min_weight_matching(const std::vector<Node>& odd_nodes, WeightFunc&& weight)
        -> std::vector<std::pair<Node, Node>> {
        const size_t k = odd_nodes.size();
        if (k < 2) return {};

        // Build edge list sorted by weight
        std::vector<std::tuple<double, Node, Node>> edges;
        edges.reserve(k * (k - 1) / 2);
        for (size_t i = 0; i < k; ++i) {
            for (size_t j = i + 1; j < k; ++j) {
                edges.emplace_back(std::forward<WeightFunc>(weight)(odd_nodes[i], odd_nodes[j]),
                                   odd_nodes[i], odd_nodes[j]);
            }
        }
        std::sort(edges.begin(), edges.end(),
                  [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });

        std::vector<bool> used(k, false);
        std::vector<std::pair<Node, Node>> matching;
        matching.reserve(k / 2);

        for (const auto& [w, u, v] : edges) {
            // Map node -> index in odd_nodes (linear scan, k is small)
            size_t iu = 0;
            size_t iv = 0;
            for (size_t t = 0; t < k; ++t) {
                if (odd_nodes[t] == u) iu = t;
                if (odd_nodes[t] == v) iv = t;
            }
            if (!used[iu] && !used[iv]) {
                matching.emplace_back(u, v);
                used[iu] = true;
                used[iv] = true;
            }
        }
        return matching;
    }

    /**
     * @brief Build a multigraph adjacency list (MST + matching).
     *
     * Parallel edges are preserved via ``std::multiset`` so that the graph
     * is Eulerian (all vertices have even degree).
     */
    template <typename Node>
    auto build_multigraph(size_t n, const std::vector<std::pair<Node, Node>>& mst_edges,
                          const std::vector<std::pair<Node, Node>>& matching_edges)
        -> std::vector<std::multiset<Node>> {
        std::vector<std::multiset<Node>> adj(n);
        const auto add = [&](Node a, Node b) {
            adj[static_cast<size_t>(a)].insert(b);
            adj[static_cast<size_t>(b)].insert(a);
        };
        for (const auto& [u, v] : mst_edges) add(u, v);
        for (const auto& [u, v] : matching_edges) add(u, v);
        return adj;
    }

    /**
     * @brief Hierholzer's algorithm - find an Eulerian circuit.
     *
     * Assumes the graph is connected and every vertex has even degree.
     *
     * @param adj    adjacency (multiset per vertex) - consumed by the algorithm
     * @param start  start vertex
     * @return vector of directed edges ``(u, v)`` forming the circuit
     */
    template <typename Node> auto hierholzer(std::vector<std::multiset<Node>> adj, Node start)
        -> std::vector<std::pair<Node, Node>>;

    /**
     * @brief Convert an Eulerian circuit to a Hamiltonian cycle by skipping
     *        repeated vertices (shortcut).
     */
    template <typename Node>
    auto shortcut_eulerian(const std::vector<std::pair<Node, Node>>& eulerian_circuit)
        -> std::vector<Node> {
        std::vector<Node> path;
        py::set<Node> visited;
        for (const auto& [u, v] : eulerian_circuit) {
            if (!visited.contains(u)) {
                path.push_back(u);
                visited.insert(u);
            }
        }
        if (!path.empty()) path.push_back(path.front());  // close the loop
        return path;
    }

}  // namespace detail

// ---------------------------------------------------------------------------
// Christofides TSP - 3/2-approximation
// ---------------------------------------------------------------------------

/**
 * @brief Solve Metric TSP using the Christofides approximation algorithm.
 *
 * @tparam Graph       graph type with ``node_t`` and ``number_of_nodes()``
 * @tparam WeightFunc  callable ``double(node_t, node_t)``
 * @param G            input graph (only ``number_of_nodes()`` is used)
 * @param weight       edge-weight function (must satisfy triangle inequality)
 * @return Hamiltonian cycle ``[v0, v1, ..., vn, v0]``
 */
template <typename Graph, typename WeightFunc>
auto christofides_tsp(const Graph& G, WeightFunc&& weight) -> std::vector<typename Graph::node_t> {
    using Node = typename Graph::node_t;
    const size_t n = G.number_of_nodes();

    if (n == 0) return {};
    if (n == 1) return {Node{0}, Node{0}};

    // 1. Minimum Spanning Tree
    const auto mst_edges = detail::prim_mst<Node>(n, weight);

    // 2. Odd-degree vertices in the MST
    const auto odd_nodes = detail::find_odd_degree_nodes(mst_edges, n);

    // 3. Minimum-weight perfect matching on odd vertices
    const auto matching
        = detail::greedy_min_weight_matching(odd_nodes, std::forward<WeightFunc>(weight));

    // 4. Build the Eulerian multigraph (MST + matching)
    auto adj = detail::build_multigraph<Node>(n, mst_edges, matching);

    // 5. Eulerian circuit via Hierholzer
    const auto eulerian_circuit = detail::hierholzer<Node>(std::move(adj), Node{0});

    // 6. Shortcut -> Hamiltonian cycle
    return detail::shortcut_eulerian(eulerian_circuit);
}

// ---------------------------------------------------------------------------
// 2-Opt local search
// ---------------------------------------------------------------------------

/**
 * @brief Refine a TSP tour using 2-opt neighbourhood search.
 *
 * Replaces edge pairs ``(i-1,i)`` & ``(j,j+1)`` with ``(i-1,j)`` &
 * ``(i,j+1)`` by reversing segment ``[i, j]`` whenever this reduces
 * total length.  Repeats until a local optimum is reached.
 *
 * @tparam Graph       graph type
 * @tparam WeightFunc  callable ``double(node_t, node_t)``
 * @param path         initial tour (last == first)
 * @param G            graph (unused except for node type deduction)
 * @param weight       edge-weight function
 * @return locally optimal tour
 */
template <typename Graph, typename WeightFunc>
auto two_opt(std::vector<typename Graph::node_t> path, const Graph& G, WeightFunc&& weight)
    -> std::vector<typename Graph::node_t> {
    (void)G;  // only used for type deduction

    bool improved = true;
    while (improved) {
        improved = false;
        for (size_t i = 1; i + 2 < path.size(); ++i) {
            for (size_t j = i + 1; j + 1 < path.size(); ++j) {
                if (j - i == 1) continue;

                const double delta = -weight(path[i - 1], path[i]) - weight(path[j], path[j + 1])
                                     + weight(path[i - 1], path[j])
                                     + std::forward<WeightFunc>(weight)(path[i], path[j + 1]);

                if (delta < -1.0e-12) {
                    std::reverse(path.begin() + static_cast<ptrdiff_t>(i),
                                 path.begin() + static_cast<ptrdiff_t>(j + 1));
                    improved = true;
                }
            }
        }
    }
    return path;
}

// ---------------------------------------------------------------------------
// Combined solver: Christofides + 2-Opt
// ---------------------------------------------------------------------------

/**
 * @brief Solve Metric TSP using Christofides followed by 2-Opt refinement.
 *
 * This is the recommended entry point.  The 2-Opt post-processing typically
 * improves the Christofides tour significantly, often yielding results very
 * close to optimal for moderate-size metric instances.
 *
 * @tparam Graph       graph type
 * @tparam WeightFunc  callable ``double(node_t, node_t)``
 * @param G            input graph
 * @param weight       metric edge-weight function
 * @return refined Hamiltonian cycle ``[v0, v1, ..., vn, v0]``
 */
template <typename Graph, typename WeightFunc>
auto solve_christofides_2opt_tsp(const Graph& G, WeightFunc&& weight)
    -> std::vector<typename Graph::node_t> {
    auto path = christofides_tsp(G, std::forward<WeightFunc>(weight));
    if (path.size() <= 3) return path;
    return two_opt(std::move(path), G, weight);
}
