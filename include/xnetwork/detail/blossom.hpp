#pragma once

/**
 * @file blossom.hpp
 * @brief Minimum-weight perfect matching via Edmonds' blossom algorithm.
 *
 * Thin adapter over the vendored `mwmatching` implementation (Joris van
 * Rantwijk, MIT). Replaces the earlier exact/greedy dispatch so that
 * Christofides TSP and Hadlock MAX-CUT keep their approximation guarantees for
 * large instances.
 */

#include <cstddef>
#include <utility>
#include <vector>

#if defined(_MSC_VER)
#    pragma warning(push, 0)
#elif defined(__GNUC__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wall"
#    pragma GCC diagnostic ignored "-Wextra"
#    pragma GCC diagnostic ignored "-Wconversion"
#endif

#include <xnetwork/detail/mwmatching/mwmatching.hpp>

namespace detail {

    /**
     * @brief Minimum-weight perfect matching on a complete graph.
     *
     * `weight(u, v)` returns the cost of pairing `u` with `v`. The graph is
     * assumed complete (every pair is pairable); `nodes` should have even size.
     *
     * Weights are shifted by `w = C - cost` (`C` greater than every cost) so the
     * maximum-weight solver maximizes `sum(C - cost)`, i.e. minimizes the total
     * cost, while every weight stays positive. Maximum cardinality then forces a
     * perfect matching.
     *
     * @tparam Node       vertex id type
     * @tparam WeightFunc callable `(Node, Node) -> numeric cost`
     * @param  nodes      vertices to match (even count)
     * @param  weight     pairwise cost function
     * @return vector of matched pairs
     */
    template <typename Node, typename WeightFunc> auto blossom_min_weight_perfect_matching(
        const std::vector<Node>& nodes, WeightFunc&& weight) -> std::vector<std::pair<Node, Node>> {
        const auto k = nodes.size();
        if (k < 2) return {};

        double max_cost = 0.0;
        for (std::size_t i = 0; i < k; ++i) {
            for (std::size_t j = i + 1; j < k; ++j) {
                const auto c = static_cast<double>(weight(nodes[i], nodes[j]));
                if (c > max_cost) max_cost = c;
            }
        }
        const auto c_const = max_cost + 1.0;

        std::vector<mwmatching::Edge<double>> edges;
        edges.reserve(k * (k - 1) / 2);
        for (std::size_t i = 0; i < k; ++i) {
            for (std::size_t j = i + 1; j < k; ++j) {
                const auto cost = static_cast<double>(weight(nodes[i], nodes[j]));
                edges.emplace_back(static_cast<mwmatching::VertexId>(i),
                                   static_cast<mwmatching::VertexId>(j), c_const - cost);
            }
        }

        const auto adjusted = mwmatching::adjust_weights_for_maximum_cardinality_matching(edges);
        const auto pairs = mwmatching::maximum_weight_matching(adjusted);

        std::vector<std::pair<Node, Node>> matching;
        matching.reserve(pairs.size());
        for (const auto& [a, b] : pairs) {
            matching.emplace_back(nodes[a], nodes[b]);
        }
        return matching;
    }

}  // namespace detail

#if defined(_MSC_VER)
#    pragma warning(pop)
#elif defined(__GNUC__)
#    pragma GCC diagnostic pop
#endif
