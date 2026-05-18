#include <future>
#include <random>
#include <utility>
#include <vector>

#include <py2cpp/set.hpp>

#include <xnetwork/classes/graph.hpp>
#include <xnetwork/rand_cover.hpp>

template <typename Graph, typename WeightMap, typename RNG>
auto rand_vertex_cover_trial(const Graph& ugraph,
                              const WeightMap& weight,
                              const py::set<typename Graph::node_t>& coverset,
                              RNG& rng)
    -> std::pair<py::set<typename Graph::node_t>,
                 typename WeightMap::mapped_type>
{
    using node_t = typename Graph::node_t;
    using CostType = typename WeightMap::mapped_type;

    py::set<node_t> soln = coverset.copy();
    std::vector<node_t> added_order;
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (const auto& edge : ugraph.edges())
    {
        auto u = edge.first;
        auto v = edge.second;

        if (soln.contains(u) || soln.contains(v))
            continue;

        auto w_u = weight[u];
        auto w_v = weight[v];
        const double threshold = static_cast<double>(w_v)
            / (static_cast<double>(w_u) + static_cast<double>(w_v));

        if (dist(rng) < threshold)
        {
            soln.insert(u);
            added_order.push_back(u);
        }
        else
        {
            soln.insert(v);
            added_order.push_back(v);
        }
    }

    auto is_covered = [&]() -> bool {
        for (const auto& e : ugraph.edges())
        {
            if (!soln.contains(e.first) && !soln.contains(e.second))
                return false;
        }
        return true;
    };
    detail::reverse_delete_cover(soln, added_order, is_covered);

    CostType total_cost{};
    for (const auto& v : soln)
        total_cost += weight[v];

    return {std::move(soln), total_cost};
}

template auto rand_vertex_cover_trial<
    xnetwork::SimpleGraph, py::dict<uint32_t, int>, std::mt19937>(
    const xnetwork::SimpleGraph&, const py::dict<uint32_t, int>&,
    const py::set<uint32_t>&, std::mt19937&)
    -> std::pair<py::set<uint32_t>, int>;

// -----------------------------------------------------------------------
// rand_vertex_cover_mt
// -----------------------------------------------------------------------

template <typename Graph, typename WeightMap>
auto rand_vertex_cover_mt(const Graph& ugraph,
                           const WeightMap& weight,
                           unsigned int num_trials,
                           unsigned int seed,
                           const py::set<typename Graph::node_t>& coverset)
    -> std::pair<py::set<typename Graph::node_t>, typename WeightMap::mapped_type>
{
    using node_t = typename Graph::node_t;
    using CostType = typename WeightMap::mapped_type;
    using Result = std::pair<py::set<node_t>, CostType>;

    xnetwork::thread_pool pool;
    std::vector<std::future<Result>> futures;
    futures.reserve(num_trials);

    for (unsigned int t = 0; t < num_trials; ++t)
    {
        futures.push_back(pool.enqueue([&, t]() -> Result {
            std::mt19937 rng{seed + t};
            return rand_vertex_cover_trial(ugraph, weight, coverset, rng);
        }));
    }

    auto best = futures[0].get();
    for (unsigned int t = 1; t < num_trials; ++t)
    {
        auto result = futures[t].get();
        if (result.second < best.second)
            best = std::move(result);
    }

    return best;
}

template auto rand_vertex_cover_mt<
    xnetwork::SimpleGraph, py::dict<uint32_t, int>>(
    const xnetwork::SimpleGraph&, const py::dict<uint32_t, int>&,
    unsigned int, unsigned int, const py::set<uint32_t>&)
    -> std::pair<py::set<uint32_t>, int>;
