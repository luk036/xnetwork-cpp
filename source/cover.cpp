#include <cassert>
#include <deque>
#include <optional>
#include <py2cpp/dict.hpp>
#include <py2cpp/set.hpp>
#include <queue>
#include <tuple>
#include <utility>
#include <vector>
#include <xnetwork/classes/graph.hpp>
#include <xnetwork/cover.hpp>

// -----------------------------------------------------------------------
// _construct_cycle
// -----------------------------------------------------------------------

template <typename Node> auto construct_cycle(const py::dict<Node, BFSInfo<Node>>& info,
                                              Node parent, Node child) -> std::deque<Node> {
    const auto& info_parent = info.at(parent);
    const auto& info_child = info.at(child);

    Node node_a;
    Node node_b;
    int depth_a = 0;
    int depth_b = 0;

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

    while (depth_a < depth_b) {
        path.emplace_back(node_a);
        const auto& next_info = info.at(node_a);
        node_a = next_info.parent;
        depth_a = next_info.depth;
    }

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

template auto construct_cycle<uint32_t>(const py::dict<uint32_t, BFSInfo<uint32_t>>&, uint32_t,
                                        uint32_t) -> std::deque<uint32_t>;

// -----------------------------------------------------------------------
// _generic_bfs_cycle
// -----------------------------------------------------------------------

template <typename Graph, typename CoverSet>
auto generic_bfs_cycle(const Graph& ugraph, const CoverSet& coverset)
    -> std::vector<std::tuple<py::dict<typename Graph::node_t, BFSInfo<typename Graph::node_t>>,
                              typename Graph::node_t, typename Graph::node_t>> {
    using node_t = typename Graph::node_t;
    std::vector<std::tuple<py::dict<node_t, BFSInfo<node_t>>, node_t, node_t>> cycles;

    int depth_limit = static_cast<int>(ugraph.number_of_nodes());

    for (const auto& source : ugraph) {
        if (coverset.contains(source)) continue;

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
                if (coverset.contains(child)) continue;

                if (!info.contains(child)) {
                    info.insert_or_assign(child, BFSInfo<node_t>(parent, depth_now - 1));
                    queue.push(child);
                    continue;
                }

                if (succ == child) continue;

                cycles.emplace_back(info, parent, child);
            }
        }
    }

    return cycles;
}

template auto generic_bfs_cycle<xnetwork::SimpleGraph, py::set<uint32_t>>(
    const xnetwork::SimpleGraph&, const py::set<uint32_t>&)
    -> std::vector<std::tuple<py::dict<uint32_t, BFSInfo<uint32_t>>, uint32_t, uint32_t>>;

// -----------------------------------------------------------------------
// _extract_odd_cycle
// -----------------------------------------------------------------------

template <typename Node> auto extract_odd_cycle(const py::dict<Node, Node>& parent, Node utx,
                                                Node vtx) -> std::vector<Node> {
    py::set<Node> ancestors;
    Node node = utx;
    while (true) {
        ancestors.insert(node);
        const Node par = parent.at(node);
        if (par == node) break;
        node = par;
    }

    node = vtx;
    while (!ancestors.contains(node)) {
        node = parent.at(node);
    }
    const Node lca = node;

    std::vector<Node> branch_u;
    node = utx;
    while (node != lca) {
        branch_u.emplace_back(node);
        node = parent.at(node);
    }

    std::vector<Node> branch_v;
    node = vtx;
    while (node != lca) {
        branch_v.emplace_back(node);
        node = parent.at(node);
    }

    std::vector<Node> cycle;
    cycle.reserve(branch_u.size() + branch_v.size() + 1);
    cycle.emplace_back(lca);
    for (auto it = branch_u.rbegin(); it != branch_u.rend(); ++it) {
        cycle.emplace_back(*it);
    }
    for (const auto& vtx_in_branch : branch_v) {
        cycle.emplace_back(vtx_in_branch);
    }
    return cycle;
}

template auto extract_odd_cycle<uint32_t>(const py::dict<uint32_t, uint32_t>&, uint32_t,
                                          uint32_t) -> std::vector<uint32_t>;

// -----------------------------------------------------------------------
// _find_odd_cycle
// -----------------------------------------------------------------------

template <typename Graph, typename CoverSet>
auto find_odd_cycle(const Graph& ugraph, const CoverSet& coverset)
    -> std::optional<std::vector<typename Graph::node_t>> {
    using node_t = typename Graph::node_t;

    py::dict<node_t, int> color;
    py::dict<node_t, node_t> parent;

    for (const auto& source : ugraph) {
        if (coverset.contains(source) || color.contains(source)) continue;

        color.insert_or_assign(source, 0);
        parent.insert_or_assign(source, source);

        std::queue<node_t> queue;
        queue.push(source);

        while (!queue.empty()) {
            const node_t utx = queue.front();
            queue.pop();
            const int utx_color = color.at(utx);

            for (const auto& vtx : ugraph[utx]) {
                if (coverset.contains(vtx)) continue;

                if (!color.contains(vtx)) {
                    color.insert_or_assign(vtx, utx_color ^ 1);
                    parent.insert_or_assign(vtx, utx);
                    queue.push(vtx);
                } else if (color.at(vtx) == utx_color) {
                    return extract_odd_cycle<node_t>(parent, utx, vtx);
                }
            }
        }
    }

    return std::nullopt;
}

template auto find_odd_cycle<xnetwork::SimpleGraph, py::set<uint32_t>>(
    const xnetwork::SimpleGraph&, const py::set<uint32_t>&) -> std::optional<std::vector<uint32_t>>;

// -----------------------------------------------------------------------
// min_vertex_cover
// -----------------------------------------------------------------------

template <typename Graph, typename WeightMap, typename CoverSet>
auto min_vertex_cover(const Graph& ugraph, WeightMap& weight, CoverSet& coverset)
    -> std::pair<CoverSet, typename WeightMap::mapped_type> {
    using node_t = typename Graph::node_t;

    auto make_violate_graph = [&]() {
        auto edge_list = ugraph.edges();
        return [&coverset, edge_list = std::move(edge_list),
                idx = std::size_t{0}]() mutable -> std::optional<std::vector<node_t>> {
            while (idx < edge_list.size()) {
                const auto& [utx, vtx] = edge_list[idx];
                ++idx;
                if (!coverset.contains(utx) && !coverset.contains(vtx))
                    return std::vector<node_t>{utx, vtx};
            }
            return std::nullopt;
        };
    };

    return pd_cover(make_violate_graph, weight, coverset);
}

template auto min_vertex_cover<xnetwork::SimpleGraph, py::dict<uint32_t, int>, py::set<uint32_t>>(
    const xnetwork::SimpleGraph&, py::dict<uint32_t, int>&, py::set<uint32_t>&)
    -> std::pair<py::set<uint32_t>, int>;

// -----------------------------------------------------------------------
// min_odd_cycle_cover
// -----------------------------------------------------------------------

template <typename Graph, typename WeightMap, typename CoverSet>
auto min_odd_cycle_cover(const Graph& ugraph, WeightMap& weight, CoverSet& coverset)
    -> std::pair<CoverSet, typename WeightMap::mapped_type> {
    using node_t = typename Graph::node_t;

    auto make_violate = [&]() {
        return [&ugraph, &coverset]() -> std::optional<std::vector<node_t>> {
            return find_odd_cycle<Graph, CoverSet>(ugraph, coverset);
        };
    };

    return pd_cover(make_violate, weight, coverset);
}

template auto min_odd_cycle_cover<xnetwork::SimpleGraph, py::dict<uint32_t, int>,
                                  py::set<uint32_t>>(const xnetwork::SimpleGraph&,
                                                     py::dict<uint32_t, int>&, py::set<uint32_t>&)
    -> std::pair<py::set<uint32_t>, int>;
