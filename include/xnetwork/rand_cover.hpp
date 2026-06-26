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

#include <cassert>
#include <optional>
#include <py2cpp/set.hpp>
#include <random>
#include <utility>
#include <vector>
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
    void reverse_delete_cover(py::set<Node>& soln, const std::vector<Node>& added_order,
                              Validator&& is_valid) {
        for (auto it = added_order.rbegin(); it != added_order.rend(); ++it) {
            soln.erase(*it);
            if (!std::forward<Validator>(is_valid)()) {
                soln.insert(*it);
            }
        }
    }

}  // namespace detail

/**
 * @brief Single trial of Pitt's randomized vertex cover algorithm.
 *
 * For each uncovered edge \f$(u, v)\f$, selects one endpoint with probability
 * inversely proportional to its weight:
 * @f[
 *     P(\text{pick } u) = \frac{w(v)}{w(u) + w(v)}
 * @f]
 *
 * @dot
 *   digraph random_trial {
 *     rankdir=TB; bgcolor="transparent";
 *     node [shape=box, style=filled, fillcolor="#d4e6f1"];
 *     start [label="Start with\ninitial coverset", fillcolor="#a9cce3"];
 *     pick [label="Pick uncovered\nedge (u,v)"];
 *     prob [label="Choose endpoint\nP(u) = w(v)/(w(u)+w(v))", shape=diamond, fillcolor="#f9e79f"];
 *     add [label="Add chosen\nvertex to cover"];
 *     check [label="All edges\ncovered?", shape=diamond, fillcolor="#f9e79f"];
 *     reverse [label="Reverse-delete\npost-processing"];
 *     done [label="Minimal\ncover", fillcolor="#7fb3d8"];
 *     start -> pick;
 *     pick -> prob;
 *     prob -> add;
 *     add -> check;
 *     check -> pick [label="No", style=dashed, color="#e74c3c"];
 *     check -> reverse [label="Yes", color="#27ae60"];
 *     reverse -> done;
 *   }
 * @enddot
 *
 * @tparam Graph Graph type (requires node_t, edges())
 * @tparam WeightMap Weight map type (requires mapped_type, operator[])
 * @tparam RNG Random number generator type
 * @param ugraph Input undirected graph
 * @param weight Vertex weight mapping
 * @param coverset Initial vertex cover (preserved in the result)
 * @return std::pair<py::set<typename Graph::node_t>, typename WeightMap::mapped_type>
 */
template <typename Graph, typename WeightMap, typename RNG>
auto rand_vertex_cover_trial(const Graph& ugraph, const WeightMap& weight,
                             const py::set<typename Graph::node_t>& coverset, RNG& rng)
    -> std::pair<py::set<typename Graph::node_t>, typename WeightMap::mapped_type>;

// -----------------------------------------------------------------------
// Convenience overload - single trial with seed
// -----------------------------------------------------------------------

/**
 * @brief Single-trial randomized vertex cover (seeded convenience wrapper).
 *
 * @param ugraph Input graph
 * @param weight Vertex weights
 * @param seed Random seed (default: 0). Use std::nullopt for non-deterministic.
 * @param coverset Optional initial cover set
 * @return A pair of (cover set, total weight)
 */
template <typename Graph, typename WeightMap>
auto rand_vertex_cover(const Graph& ugraph, const WeightMap& weight,
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
auto rand_vertex_cover_mt(const Graph& ugraph, const WeightMap& weight,
                          unsigned int num_trials = 64, unsigned int seed = 0,
                          const py::set<typename Graph::node_t>& coverset = {})
    -> std::pair<py::set<typename Graph::node_t>, typename WeightMap::mapped_type>;
