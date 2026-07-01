#include <algorithm>
#include <functional>
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
    // exact_mwpm: DP over subsets - Node template for type generality
    // -------------------------------------------------------------------

    template <typename Node>
    auto exact_mwpm(const std::vector<int>& odd_faces, const std::vector<std::vector<int>>& dist)
        -> std::vector<std::pair<int, int>> {
        const auto n = odd_faces.size();
        const auto size = static_cast<size_t>(1) << n;

        // Precompute first_unset[mask]: smallest i where bit i is 0 in mask
        // Recurrence: first_unset[mask] = (mask & 1) ? first_unset[mask >> 1] + 1 : 0
        // Precompute first_set[mask]: smallest i where bit i is 1 in mask
        // Recurrence: first_set[mask] = (mask & 1) ? 0 : first_set[mask >> 1] + 1
        std::vector<unsigned> first_unset(size, 0u);
        std::vector<unsigned> first_set(size, 0u);
        for (size_t mask = 1; mask < size; ++mask) {
            first_unset[mask] = (mask & 1u) ? first_unset[mask >> 1] + 1u : 0u;
            first_set[mask] = (mask & 1u) ? 0u : first_set[mask >> 1] + 1u;
        }

        std::vector<int> dp(size, INF);
        dp[0] = 0;

        const auto* of = odd_faces.data();
        const auto* dist_raw = dist.data();

        for (size_t mask = 0; mask < size; ++mask) {
            if (dp[mask] == INF) continue;
            const auto first = first_unset[mask];
            if (first >= n) continue;

            for (auto j = first + 1; j < n; ++j) {
                if (mask & (size_t(1) << j)) continue;
                const auto new_mask = mask | (size_t(1) << first) | (size_t(1) << j);
                const int w = dist_raw[of[first]].data()[of[j]];
                dp[new_mask] = std::min(dp[mask] + w, dp[new_mask]);
            }
        }

        std::vector<std::pair<int, int>> matching;
        auto mask = size - 1;
        while (mask) {
            const auto first = first_set[mask];
            for (auto j = first + 1; j < n; ++j) {
                if (!(mask & (size_t(1) << j))) continue;
                const auto prev_mask = mask ^ (size_t(1) << first) ^ (size_t(1) << j);
                const int w = dist_raw[of[first]].data()[of[j]];
                if (dp[mask] == dp[prev_mask] + w) {
                    matching.emplace_back(of[first], of[j]);
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
        const auto n = dual.size();
        std::vector<int> dist(n, INF);
        std::vector<int> prev(n, -1);
        auto* dist_raw = dist.data();
        auto* prev_raw = prev.data();
        auto* dual_raw = dual.data();
        dist_raw[src] = 0;

        using P = std::pair<int, int>;
        std::priority_queue<P, std::vector<P>, std::greater<>> pq;
        pq.emplace(0, src);

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d != dist_raw[u]) continue;
            for (const auto& e : dual_raw[u]) {
                if (dist_raw[e.neighbor] > d + e.weight) {
                    dist_raw[e.neighbor] = d + e.weight;
                    prev_raw[e.neighbor] = u;
                    pq.push({dist_raw[e.neighbor], e.neighbor});
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
        const auto n = odd_faces.size();
        std::vector<std::tuple<int, int, int>> edges;
        edges.reserve(n * (n - 1) / 2);
        const auto* of = odd_faces.data();
        const auto* dist_raw = dist.data();
        for (size_t i = 0; i < n; ++i)
            for (size_t j = i + 1; j < n; ++j)
                edges.emplace_back(dist_raw[of[i]].data()[of[j]], of[i], of[j]);

        std::sort(edges.begin(), edges.end());

        std::unordered_set<int> used;
        std::vector<std::pair<int, int>> matching;
        for (const auto& [w, u, v] : edges) {
            if (used.count(u) || used.count(v)) continue;
            used.insert(u);
            used.insert(v);
            matching.emplace_back(u, v);
            if (used.size() == n) break;
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
