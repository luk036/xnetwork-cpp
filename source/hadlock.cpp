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
    // dijkstra
    // -------------------------------------------------------------------

    template <typename Node>
    auto dijkstra(const std::vector<std::vector<DualEdge<Node>>>& dual, int src)
        -> std::pair<std::vector<int>, std::vector<int>> {
        const auto n = dual.size();
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

    // Explicit instantiations
    template auto biconnected_components<xnetwork::SimpleGraph>(const xnetwork::SimpleGraph& G)
        -> std::vector<py::set<typename xnetwork::SimpleGraph::node_t>>;

    template auto dijkstra<uint32_t>(const std::vector<std::vector<DualEdge<uint32_t>>>&, int)
        -> std::pair<std::vector<int>, std::vector<int>>;

}  // namespace detail
