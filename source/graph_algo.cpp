#include <cassert>
#include <cstdint>
#include <py2cpp/dict.hpp>
#include <py2cpp/set.hpp>
#include <utility>
#include <xnetwork/classes/graph.hpp>
#include <xnetwork/graph_algo.hpp>

template <typename Graph, typename WeightMap, typename IndSet, typename DepSet>
auto min_maximal_independant_set(const Graph& ugraph, WeightMap& weight, IndSet& indset,
                                 DepSet& dep)
    -> std::pair<IndSet, typename WeightMap::mapped_type> {
    using node_t = typename Graph::node_t;
    using CostType = typename WeightMap::mapped_type;

    auto coverset = [&](node_t utx) {
        dep.insert(utx);
        for (const auto& vtx : ugraph[utx]) dep.insert(vtx);
    };

    auto gap = weight;
    CostType total_prml_cost = 0;
    CostType total_dual_cost = 0;

    for (const auto& utx : ugraph) {
        if (dep.contains(utx)) continue;
        if (indset.contains(utx)) continue;

        CostType min_val = gap[utx];
        node_t min_vtx = utx;

        for (const auto& vtx : ugraph[utx]) {
            if (dep.contains(vtx)) continue;
            if (min_val > gap[vtx]) {
                min_val = gap[vtx];
                min_vtx = vtx;
            }
        }

        indset.insert(min_vtx);
        coverset(min_vtx);
        total_prml_cost += weight[min_vtx];
        total_dual_cost += min_val;

        if (min_vtx == utx) continue;

        for (const auto& vtx : ugraph[utx]) gap[vtx] -= min_val;
    }

    assert(total_dual_cost <= total_prml_cost);
    return std::make_pair(indset, total_prml_cost);
}

template auto min_maximal_independant_set<xnetwork::SimpleGraph, py::dict<uint32_t, int>,
                                          py::set<uint32_t>, py::set<uint32_t>>(
    const xnetwork::SimpleGraph&, py::dict<uint32_t, int>&, py::set<uint32_t>&, py::set<uint32_t>&)
    -> std::pair<py::set<uint32_t>, int>;

// -----------------------------------------------------------------------
// min_vertex_cover_fast
// -----------------------------------------------------------------------

template <typename Graph, typename WeightMap, typename CoverSet>
auto min_vertex_cover_fast(const Graph& ugraph, WeightMap& weight, CoverSet& coverset)
    -> std::pair<CoverSet, typename WeightMap::mapped_type> {
    using CostType = typename WeightMap::mapped_type;

    auto gap = weight;
    CostType total_dual_cost = 0;
    CostType total_prml_cost = 0;

    ugraph.for_each_edge([&](auto utx, auto vtx) {
        if (coverset.contains(utx) || coverset.contains(vtx)) return;

        if (gap[utx] < gap[vtx]) std::swap(utx, vtx);

        coverset.insert(vtx);
        total_dual_cost += gap[vtx];
        total_prml_cost += weight[vtx];
        gap[utx] -= gap[vtx];
        gap[vtx] = 0;
    });

    assert(total_dual_cost <= total_prml_cost);
    return std::make_pair(coverset, total_prml_cost);
}

template auto min_vertex_cover_fast<xnetwork::SimpleGraph, py::dict<uint32_t, int>,
                                    py::set<uint32_t>>(const xnetwork::SimpleGraph&,
                                                       py::dict<uint32_t, int>&, py::set<uint32_t>&)
    -> std::pair<py::set<uint32_t>, int>;
