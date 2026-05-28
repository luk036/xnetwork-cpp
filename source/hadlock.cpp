#include <algorithm>
#include <cstdint>
#include <functional>
#include <limits>
#include <map>
#include <py2cpp/set.hpp>
#include <queue>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>
#include <xnetwork/classes/graph.hpp>
#include <xnetwork/hadlock.hpp>

namespace detail {

    template <typename Graph> auto biconnected_components(const Graph& G)
        -> std::vector<py::set<typename Graph::node_t>> {
        using node_t = typename Graph::node_t;

        std::vector<py::set<node_t>> components;
        std::map<node_t, int> disc;
        std::map<node_t, int> low;
        std::map<node_t, node_t> parent;
        std::vector<std::pair<node_t, node_t>> edge_stack;
        int time = 0;

        std::function<void(node_t)> dfs;
        dfs = [&](node_t u) {
            disc[u] = low[u] = ++time;
            int children = 0;

            for (const auto& v : G[u]) {
                if (!disc.count(v)) {
                    parent[v] = u;
                    ++children;
                    edge_stack.emplace_back(u, v);
                    dfs(v);

                    if (low[v] < low[u]) low[u] = low[v];

                    const bool is_art = (!parent.count(u) && children > 1)
                                        || (parent.count(u) && low[v] >= disc[u]);

                    if (is_art) {
                        py::set<node_t> comp;
                        while (!edge_stack.empty()) {
                            auto [x, y] = edge_stack.back();
                            edge_stack.pop_back();
                            comp.insert(x);
                            comp.insert(y);
                            if ((x == u && y == v) || (x == v && y == u)) break;
                        }
                        if (!comp.empty()) {
                            components.push_back(std::move(comp));
                        }
                    }
                } else if (disc[v] < disc[u]) {
                    auto parent_it = parent.find(u);
                    if (parent_it != parent.end() && parent_it->second == v) continue;
                    if (disc[v] < low[u]) low[u] = disc[v];
                    edge_stack.emplace_back(u, v);
                }
            }
        };

        for (const auto& node : G) {
            if (disc.count(node)) continue;
            dfs(node);
            if (!edge_stack.empty()) {
                py::set<node_t> comp;
                for (const auto& [x, y] : edge_stack) {
                    comp.insert(x);
                    comp.insert(y);
                }
                edge_stack.clear();
                components.push_back(std::move(comp));
            }
        }

        return components;
    }

    // -------------------------------------------------------------------
    // exact_mwpm: DP over subsets — Node template for type generality
    // -------------------------------------------------------------------

    template <typename Node>
    auto exact_mwpm(const std::vector<int>& odd_faces, const std::vector<std::vector<int>>& dist)
        -> std::vector<std::pair<int, int>> {
        const int n = static_cast<int>(odd_faces.size());
        const int size = 1 << n;

        // Precompute first_unset[mask]: smallest i where bit i is 0 in mask
        // Recurrence: first_unset[mask] = (mask & 1) ? first_unset[mask >> 1] + 1 : 0
        // Precompute first_set[mask]: smallest i where bit i is 1 in mask
        // Recurrence: first_set[mask] = (mask & 1) ? 0 : first_set[mask >> 1] + 1
        std::vector<int> first_unset(size, 0);
        std::vector<int> first_set(size, 0);
        for (int mask = 1; mask < size; ++mask) {
            first_unset[mask] = (mask & 1) ? first_unset[mask >> 1] + 1 : 0;
            first_set[mask] = (mask & 1) ? 0 : first_set[mask >> 1] + 1;
        }

        std::vector<int> dp(size, INF);
        dp[0] = 0;

        for (int mask = 0; mask < size; ++mask) {
            if (dp[mask] == INF) continue;
            const int first = first_unset[mask];
            if (first >= n) continue;

            for (int j = first + 1; j < n; ++j) {
                if (mask & (1 << j)) continue;
                const int new_mask = mask | (1 << first) | (1 << j);
                const int w = dist[odd_faces[first]][odd_faces[j]];
                dp[new_mask] = std::min(dp[mask] + w, dp[new_mask]);
            }
        }

        std::vector<std::pair<int, int>> matching;
        int mask = size - 1;
        while (mask) {
            const int first = first_set[mask];
            for (int j = first + 1; j < n; ++j) {
                if (!(mask & (1 << j))) continue;
                const int prev_mask = mask ^ (1 << first) ^ (1 << j);
                const int w = dist[odd_faces[first]][odd_faces[j]];
                if (dp[mask] == dp[prev_mask] + w) {
                    matching.emplace_back(odd_faces[first], odd_faces[j]);
                    mask = prev_mask;
                    break;
                }
            }
        }
        return matching;
    }

    // -------------------------------------------------------------------
    // dijkstra
    // -------------------------------------------------------------------

    template <typename Node>
    auto dijkstra(const std::vector<std::vector<DualEdge<Node>>>& dual, int src)
        -> std::pair<std::vector<int>, std::vector<int>> {
        const int n = static_cast<int>(dual.size());
        std::vector<int> dist(n, INF);
        std::vector<int> prev(n, -1);
        dist[src] = 0;

        using P = std::pair<int, int>;
        std::priority_queue<P, std::vector<P>, std::greater<>> pq;
        pq.emplace(0, src);

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d != dist[u]) continue;
            for (const auto& e : dual[u]) {
                if (dist[e.neighbor] > d + e.weight) {
                    dist[e.neighbor] = d + e.weight;
                    prev[e.neighbor] = u;
                    pq.push({dist[e.neighbor], e.neighbor});
                }
            }
        }
        return {dist, prev};
    }

    // -------------------------------------------------------------------
    // greedy_mwpm
    // -------------------------------------------------------------------

    template <typename Node>
    auto greedy_mwpm(const std::vector<int>& odd_faces, const std::vector<std::vector<int>>& dist)
        -> std::vector<std::pair<int, int>> {
        const int n = static_cast<int>(odd_faces.size());
        std::vector<std::tuple<int, int, int>> edges;
        edges.reserve(n * (n - 1) / 2);
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j)
                edges.emplace_back(dist[odd_faces[i]][odd_faces[j]], odd_faces[i], odd_faces[j]);

        std::sort(edges.begin(), edges.end());

        std::unordered_set<int> used;
        std::vector<std::pair<int, int>> matching;
        for (const auto& [w, u, v] : edges) {
            if (used.count(u) || used.count(v)) continue;
            used.insert(u);
            used.insert(v);
            matching.emplace_back(u, v);
            if (used.size() == static_cast<size_t>(n)) break;
        }
        return matching;
    }

    // Explicit instantiations
    template auto biconnected_components<xnetwork::SimpleGraph>(const xnetwork::SimpleGraph& G)
        -> std::vector<py::set<typename xnetwork::SimpleGraph::node_t>>;

    template auto exact_mwpm<uint32_t>(const std::vector<int>&,
                                       const std::vector<std::vector<int>>&)
        -> std::vector<std::pair<int, int>>;

    template auto dijkstra<uint32_t>(const std::vector<std::vector<DualEdge<uint32_t>>>&, int)
        -> std::pair<std::vector<int>, std::vector<int>>;

    template auto greedy_mwpm<uint32_t>(const std::vector<int>&,
                                        const std::vector<std::vector<int>>&)
        -> std::vector<std::pair<int, int>>;

}  // namespace detail
