#pragma once

/**
 * @file rand_cover.hpp
 * @brief Randomized Approximation Algorithm for Minimum Weighted Vertex Cover
 *
 * Implements Pitt's randomized algorithm (1985) for solving the minimum
 * weighted vertex cover problem on graphs. The algorithm achieves an expected
 * approximation ratio of 2.
 *
 * For each uncovered edge (u, v), selects one endpoint with probability
 * inversely proportional to its weight:
 *     P(pick u) = w(v) / (w(u) + w(v))
 *
 * Multi-threaded overloads run independent trials in parallel and return
 * the best (lowest-weight) cover.
 *
 * Reference:
 *     L. Pitt, "A Simple Probabilistic Approximation Algorithm for Vertex
 *     Cover," Technical Report, Yale University, 1985.
 */

#include <algorithm>
#include <cassert>
#include <future>
#include <optional>
#include <random>
#include <utility>
#include <vector>

#include <py2cpp/set.hpp>

#include <xnetwork/thread_pool.hpp>

namespace detail {

/**
 * @brief Reverse-delete post-processing step.
 *
 * Iterates through vertices in reverse addition order.  For each vertex,
 * temporarily removes it; if the cover remains valid, the removal is
 * kept (vertex was redundant).  Otherwise the vertex is restored.
 *
 * @tparam Node Vertex type
 * @tparam Validator Callable that returns true if current cover is valid
 * @param soln Mutable cover set (modified in place)
 * @param added_order Vertices in order they were added
 * @param is_valid Validation callable
 */
template <typename Node, typename Validator>
void reverse_delete_cover(py::set<Node>& soln,
                           const std::vector<Node>& added_order,
                           Validator&& is_valid) {
    for (auto it = added_order.rbegin(); it != added_order.rend(); ++it) {
        soln.erase(*it);
        if (!std::forward<Validator>(is_valid)()) {
            soln.insert(*it);
        }
    }
}

} // namespace detail

/**
 * @brief Single trial of Pitt's randomized vertex cover algorithm.
 *
 * @tparam Graph Graph type (requires node_t, edges())
 * @tparam WeightMap Weight map type (requires mapped_type, operator[])
 * @tparam RNG Random number generator type
 * @param ugraph Input undirected graph
 * @param weight Vertex weight mapping
 * @param coverset Initial vertex cover (preserved in the result)
 * @param rng Random number generator (seeded externally)
 * @return std::pair<py::set<typename Graph::node_t>, typename WeightMap::mapped_type>
 */
template <typename Graph, typename WeightMap, typename RNG>
auto rand_vertex_cover_trial(const Graph& ugraph,
                              const WeightMap& weight,
                              const py::set<typename Graph::node_t>& coverset,
                              RNG& rng)
    -> std::pair<py::set<typename Graph::node_t>, typename WeightMap::mapped_type> {
    using node_t = typename Graph::node_t;
    using CostType = typename WeightMap::mapped_type;

    py::set<node_t> soln = coverset.copy();
    std::vector<node_t> added_order;
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (const auto& edge : ugraph.edges()) {
        auto u = edge.first;
        auto v = edge.second;

        if (soln.contains(u) || soln.contains(v)) {
            continue;
        }

        // Pitt's rule: pick u with prob w(v) / (w(u) + w(v))
        auto w_u = weight[u];
        auto w_v = weight[v];
        const double threshold = static_cast<double>(w_v) /
            (static_cast<double>(w_u) + static_cast<double>(w_v));

        if (dist(rng) < threshold) {
            soln.insert(u);
            added_order.push_back(u);
        } else {
            soln.insert(v);
            added_order.push_back(v);
        }
    }

    // Phase 2: Reverse-Delete Post-Processing
    auto is_covered = [&]() -> bool {
        for (const auto& e : ugraph.edges()) {
            if (!soln.contains(e.first) && !soln.contains(e.second)) {
                return false;
            }
        }
        return true;
    };
    detail::reverse_delete_cover(soln, added_order, is_covered);

    CostType total_cost{};
    for (const auto& v : soln) {
        total_cost += weight[v];
    }
    return {std::move(soln), total_cost};
}

// -----------------------------------------------------------------------
// Convenience overload — single trial with seed
// -----------------------------------------------------------------------

/**
 * @brief Single-trial randomized vertex cover (seeded convenience wrapper).
 *
 * @copydetails rand_vertex_cover_trial
 * @param ugraph Input graph
 * @param weight Vertex weights
 * @param seed Random seed (default: 0). Use std::nullopt for non-deterministic.
 * @param coverset Optional initial cover set
 */
template <typename Graph, typename WeightMap>
auto rand_vertex_cover(const Graph& ugraph,
                        const WeightMap& weight,
                        std::optional<unsigned int> seed = std::optional<unsigned int>{0},
                        const py::set<typename Graph::node_t>& coverset = {})
    -> std::pair<py::set<typename Graph::node_t>, typename WeightMap::mapped_type> {
    // using node_t = typename Graph::node_t;

    if (seed.has_value()) {
        std::mt19937 rng{seed.value()};
        return rand_vertex_cover_trial(ugraph, weight, coverset, rng);
    }
    std::random_device rd;
    std::mt19937 rng{rd()};
    return rand_vertex_cover_trial(ugraph, weight, coverset, rng);
}

// -----------------------------------------------------------------------
// Multi-threaded: run num_trials independent trials, return best cover
// -----------------------------------------------------------------------

/**
 * @brief Multi-threaded randomized vertex cover.
 *
 * Runs @p num_trials independent Pitt trials in parallel using
 * std::async, then returns the cover with the lowest total weight.
 *
 * @tparam Graph Graph type
 * @tparam WeightMap Weight map type
 * @param ugraph Input undirected graph
 * @param weight Vertex weight mapping (read-concurrently from threads)
 * @param num_trials Number of independent Monte Carlo trials (default: 64)
 * @param seed Master random seed (default: 0). Trials use seed + index.
 * @param coverset Optional initial cover set (shared by all trials)
 * @return std::pair<py::set<typename Graph::node_t>, typename WeightMap::mapped_type>
 */
template <typename Graph, typename WeightMap>
auto rand_vertex_cover_mt(const Graph& ugraph,
                           const WeightMap& weight,
                           unsigned int num_trials = 64,
                           unsigned int seed = 0,
                           const py::set<typename Graph::node_t>& coverset = {})
    -> std::pair<py::set<typename Graph::node_t>, typename WeightMap::mapped_type> {
    using node_t = typename Graph::node_t;
    using CostType = typename WeightMap::mapped_type;
    using Result = std::pair<py::set<node_t>, CostType>;

    xnetwork::thread_pool pool;
    std::vector<std::future<Result>> futures;
    futures.reserve(num_trials);

    for (unsigned int t = 0; t < num_trials; ++t) {
        futures.push_back(pool.enqueue([&, t]() -> Result {
            std::mt19937 rng{seed + t};
            return rand_vertex_cover_trial(ugraph, weight, coverset, rng);
        }));
    }

    // Collect results and keep the best (lowest cost)
    auto best = futures[0].get();
    for (unsigned int t = 1; t < num_trials; ++t) {
        auto result = futures[t].get();
        if (result.second < best.second) {
            best = std::move(result);
        }
    }

    return best;
}
