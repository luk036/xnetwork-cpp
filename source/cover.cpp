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
    detail::scan_cycles<Graph, CoverSet>(ugraph, coverset,
                                         [&cycles](const auto& info, node_t parent, node_t child) {
                                             cycles.emplace_back(info, parent, child);
                                             return false;
                                         });
    return cycles;
}

template auto generic_bfs_cycle<xnetwork::SimpleGraph, py::set<uint32_t>>(
    const xnetwork::SimpleGraph&, const py::set<uint32_t>&)
    -> std::vector<std::tuple<py::dict<uint32_t, BFSInfo<uint32_t>>, uint32_t, uint32_t>>;

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

    // Removing a vertex can only expose edges incident to it, so redundancy is
    // an O(deg) neighbour check instead of an O(E) rescan of every edge.
    auto redundant = [&ugraph, &coverset](const node_t& v) -> bool {
        for (const auto& nb : ugraph[v]) {
            if (!coverset.contains(nb)) return false;
        }
        return true;
    };

    return pd_cover(make_violate_graph, weight, coverset, redundant);
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
            std::optional<std::vector<node_t>> result;
            detail::scan_cycles<Graph, CoverSet>(
                ugraph, coverset, [&result](const auto& info, node_t parent, node_t child) {
                    const auto& info_parent = info.at(parent);
                    const auto& info_child = info.at(child);
                    if ((info_parent.depth - info_child.depth) % 2 != 0) {
                        return false;  // keep scanning for an odd cycle
                    }
                    auto cycle_deque = construct_cycle<node_t>(info, parent, child);
                    result = std::vector<node_t>(cycle_deque.begin(), cycle_deque.end());
                    return true;
                });
            return result;
        };
    };

    return pd_cover(make_violate, weight, coverset);
}

template auto min_odd_cycle_cover<xnetwork::SimpleGraph, py::dict<uint32_t, int>,
                                  py::set<uint32_t>>(const xnetwork::SimpleGraph&,
                                                     py::dict<uint32_t, int>&, py::set<uint32_t>&)
    -> std::pair<py::set<uint32_t>, int>;
