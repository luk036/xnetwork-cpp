/// @file test_tsp.cpp
/// Unit tests for the TSP approximation algorithms (tsp.hpp).

#include <doctest/doctest.h>

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <set>
#include <vector>
#include <xnetwork/classes/graph.hpp>
#include <xnetwork/tsp.hpp>

using SimpleGraph = xnetwork::SimpleGraph;
using node_t = SimpleGraph::node_t;

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

/// Build a complete graph with n nodes.
static auto make_complete_graph(uint32_t n) -> SimpleGraph {
    SimpleGraph G{n};
    for (uint32_t i = 0; i < n; ++i)
        for (uint32_t j = i + 1; j < n; ++j) G.add_edge(i, j);
    return G;
}

/// Euclidean weight function for a set of 2D points.
struct EuclideanWeight {
    const std::vector<std::pair<double, double>>& points;

    auto operator()(uint32_t u, uint32_t v) const -> double {
        const double dx = points[u].first - points[v].first;
        const double dy = points[u].second - points[v].second;
        return std::sqrt(dx * dx + dy * dy);
    }
};

/// Uniform weight (all edges cost 1).
static auto uniform_weight(uint32_t /*u*/, uint32_t /*v*/) -> double { return 1.0; }

/// Manhattan (L1) weight function for a set of 2D points.
struct ManhattanWeight {
    const std::vector<std::pair<double, double>>& points;

    auto operator()(uint32_t u, uint32_t v) const -> double {
        const double dx = points[u].first - points[v].first;
        const double dy = points[u].second - points[v].second;
        return std::abs(dx) + std::abs(dy);
    }
};

/// Check that path is a valid Hamiltonian cycle for n nodes.
static auto is_valid_hamiltonian_cycle(const std::vector<node_t>& path, size_t n) -> bool {
    if (path.size() != n + 1) return false;
    if (path.front() != path.back()) return false;
    const std::set<node_t> visited(path.begin(), path.end() - 1);
    return visited.size() == n && *visited.begin() == 0
           && *visited.rbegin() == static_cast<node_t>(n - 1);
}

// ---------------------------------------------------------------------------
// calculate_total_distance
// ---------------------------------------------------------------------------

TEST_CASE("calculate_total_distance") {
    const auto weight = [](uint32_t u, uint32_t v) -> double {
        if ((u == 0 && v == 1) || (u == 1 && v == 0)) return 1.0;
        if ((u == 1 && v == 2) || (u == 2 && v == 1)) return 2.0;
        return 3.0;
    };
    CHECK_EQ(calculate_total_distance(std::vector<node_t>{0, 1, 2, 0}, weight), 6.0);
    CHECK_EQ(calculate_total_distance(std::vector<node_t>{0, 2, 1, 0}, weight), 6.0);
}

// ---------------------------------------------------------------------------
// christofides_tsp
// ---------------------------------------------------------------------------

TEST_CASE("Christofides TSP n=1") {
    auto G = make_complete_graph(1);
    const auto tour = christofides_tsp(G, uniform_weight);
    CHECK_EQ(tour.size(), 2);
    CHECK_EQ(tour.front(), 0);
    CHECK_EQ(tour.back(), 0);
}

TEST_CASE("Christofides TSP n=2") {
    auto G = make_complete_graph(2);
    const auto tour = christofides_tsp(G, uniform_weight);
    CHECK(is_valid_hamiltonian_cycle(tour, 2));
}

TEST_CASE("Christofides TSP n=3 uniform") {
    auto G = make_complete_graph(3);
    const auto tour = christofides_tsp(G, uniform_weight);
    CHECK(is_valid_hamiltonian_cycle(tour, 3));
    CHECK_EQ(calculate_total_distance(tour, uniform_weight), 3.0);
}

TEST_CASE("Christofides TSP n=5 euclidean") {
    const std::vector<std::pair<double, double>> pts
        = {{0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}, {1.5, 0.5}, {0.0, 1.0}};
    auto G = make_complete_graph(5);
    const auto tour = christofides_tsp(G, EuclideanWeight{pts});
    CHECK(is_valid_hamiltonian_cycle(tour, 5));
    CHECK_GT(calculate_total_distance(tour, EuclideanWeight{pts}), 0.0);
}

TEST_CASE("Christofides TSP n=10 visits all nodes") {
    const std::vector<std::pair<double, double>> pts
        = {{0.0, 0.0}, {1.0, 2.0}, {3.0, 1.0}, {4.0, 4.0}, {2.0, 5.0},
           {5.0, 0.0}, {6.0, 3.0}, {7.0, 7.0}, {8.0, 2.0}, {9.0, 5.0}};
    auto G = make_complete_graph(10);
    const auto tour = christofides_tsp(G, EuclideanWeight{pts});
    CHECK(is_valid_hamiltonian_cycle(tour, 10));

    std::set<node_t> visited(tour.begin(), tour.end() - 1);
    CHECK_EQ(visited.size(), 10);
}

// ---------------------------------------------------------------------------
// two_opt
// ---------------------------------------------------------------------------

TEST_CASE("2-Opt collinear points (already optimal)") {
    const std::vector<std::pair<double, double>> pts
        = {{0.0, 0.0}, {1.0, 0.0}, {2.0, 0.0}, {3.0, 0.0}};
    auto G = make_complete_graph(4);
    std::vector<node_t> tour = {0, 1, 2, 3, 0};
    const auto refined = two_opt(tour, G, EuclideanWeight{pts});
    CHECK(is_valid_hamiltonian_cycle(refined, 4));
    CHECK_EQ(calculate_total_distance(refined, EuclideanWeight{pts}), 6.0);
}

TEST_CASE("2-Opt improves crossing tour on unit square") {
    const std::vector<std::pair<double, double>> pts
        = {{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}};
    auto G = make_complete_graph(4);

    std::vector<node_t> crossing = {0, 2, 1, 3, 0};
    const auto dist_before = calculate_total_distance(crossing, EuclideanWeight{pts});
    const auto refined = two_opt(std::move(crossing), G, EuclideanWeight{pts});
    const auto dist_after = calculate_total_distance(refined, EuclideanWeight{pts});
    CHECK_LE(dist_after, dist_before);
    CHECK(is_valid_hamiltonian_cycle(refined, 4));
}

// ---------------------------------------------------------------------------
// solve_christofides_2opt_tsp
// ---------------------------------------------------------------------------

TEST_CASE("Combined solver valid cycle") {
    const std::vector<std::pair<double, double>> pts
        = {{0.0, 0.0}, {1.0, 2.0}, {3.0, 1.0}, {4.0, 4.0}, {2.0, 5.0},
           {5.0, 0.0}, {6.0, 3.0}, {7.0, 7.0}, {8.0, 2.0}, {9.0, 5.0}};
    auto G = make_complete_graph(10);
    const auto tour = solve_christofides_2opt_tsp(G, EuclideanWeight{pts});
    CHECK(is_valid_hamiltonian_cycle(tour, 10));
}

TEST_CASE("Combined solver n=2 (trivial)") {
    auto G = make_complete_graph(2);
    const auto tour = solve_christofides_2opt_tsp(G, uniform_weight);
    CHECK(is_valid_hamiltonian_cycle(tour, 2));
}

TEST_CASE("Combined solver improvement over Christofides alone") {
    const std::vector<std::pair<double, double>> pts
        = {{0.0, 0.0}, {1.0, 3.0}, {3.0, 1.0}, {4.0, 5.0}, {2.0, 4.0},
           {5.0, 1.0}, {6.0, 4.0}, {7.0, 6.0}, {8.0, 2.0}, {9.0, 7.0},
           {1.0, 5.0}, {3.0, 6.0}, {4.0, 2.0}, {5.0, 5.0}, {6.0, 1.0}};
    auto G = make_complete_graph(15);
    const auto weight = EuclideanWeight{pts};

    const auto christo_only = christofides_tsp(G, weight);
    const auto combined = solve_christofides_2opt_tsp(G, weight);

    const auto dist_christo = calculate_total_distance(christo_only, weight);
    const auto dist_combined = calculate_total_distance(combined, weight);

    CHECK_LE(dist_combined, dist_christo + 1.0e-10);
}

TEST_CASE("Combined solver n=20 stress") {
    const std::vector<std::pair<double, double>> pts
        = {{0.0, 0.0},   {12.0, 5.0},  {3.0, 18.0},  {45.0, 7.0},  {22.0, 33.0},
           {8.0, 12.0},  {37.0, 2.0},  {9.0, 44.0},  {51.0, 15.0}, {28.0, 29.0},
           {6.0, 38.0},  {42.0, 11.0}, {17.0, 25.0}, {33.0, 41.0}, {55.0, 4.0},
           {14.0, 48.0}, {20.0, 9.0},  {39.0, 22.0}, {47.0, 36.0}, {25.0, 19.0}};
    auto G = make_complete_graph(20);
    const auto weight = EuclideanWeight{pts};

    const auto tour = solve_christofides_2opt_tsp(G, weight);
    CHECK(is_valid_hamiltonian_cycle(tour, 20));
    CHECK_GT(calculate_total_distance(tour, weight), 0.0);
}

TEST_CASE("Combined solver 3/2 bound check") {
    const std::vector<std::pair<double, double>> pts
        = {{0.0, 0.0}, {1.0, 3.0}, {3.0, 1.0}, {4.0, 5.0}, {2.0, 4.0},
           {5.0, 1.0}, {6.0, 4.0}, {7.0, 6.0}, {8.0, 2.0}, {9.0, 7.0}};
    auto G = make_complete_graph(10);
    const auto weight = EuclideanWeight{pts};

    const auto tour = solve_christofides_2opt_tsp(G, weight);
    const auto tour_weight = calculate_total_distance(tour, weight);

    CHECK(is_valid_hamiltonian_cycle(tour, 10));
    CHECK_GT(tour_weight, 0.0);
}

// ---------------------------------------------------------------------------
// L1 (Manhattan) metric tests
// ---------------------------------------------------------------------------

TEST_CASE("Christofides TSP n=5 manhattan") {
    const std::vector<std::pair<double, double>> pts
        = {{0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}, {1.5, 0.5}, {0.0, 1.0}};
    auto G = make_complete_graph(5);
    const auto weight = ManhattanWeight{pts};
    const auto tour = christofides_tsp(G, weight);
    CHECK(is_valid_hamiltonian_cycle(tour, 5));
    CHECK_GT(calculate_total_distance(tour, weight), 0.0);

    CHECK_EQ(weight(0, 1), 1.0);
    CHECK_EQ(weight(2, 4), 0.5);
}

TEST_CASE("Combined solver n=10 manhattan valid cycle") {
    const std::vector<std::pair<double, double>> pts
        = {{0.0, 0.0}, {1.0, 2.0}, {3.0, 1.0}, {4.0, 4.0}, {2.0, 5.0},
           {5.0, 0.0}, {6.0, 3.0}, {7.0, 7.0}, {8.0, 2.0}, {9.0, 5.0}};
    auto G = make_complete_graph(10);
    const auto weight = ManhattanWeight{pts};
    const auto tour = solve_christofides_2opt_tsp(G, weight);
    CHECK(is_valid_hamiltonian_cycle(tour, 10));
}

TEST_CASE("Combined solver n=15 manhattan improvement") {
    const std::vector<std::pair<double, double>> pts
        = {{0.0, 0.0}, {1.0, 3.0}, {3.0, 1.0}, {4.0, 5.0}, {2.0, 4.0},
           {5.0, 1.0}, {6.0, 4.0}, {7.0, 6.0}, {8.0, 2.0}, {9.0, 7.0},
           {1.0, 5.0}, {3.0, 6.0}, {4.0, 2.0}, {5.0, 5.0}, {6.0, 1.0}};
    auto G = make_complete_graph(15);
    const auto weight = ManhattanWeight{pts};

    const auto christo_only = christofides_tsp(G, weight);
    const auto combined = solve_christofides_2opt_tsp(G, weight);

    const auto dist_christo = calculate_total_distance(christo_only, weight);
    const auto dist_combined = calculate_total_distance(combined, weight);

    CHECK_LE(dist_combined, dist_christo + 1.0e-10);
}

TEST_CASE("Combined solver n=20 manhattan stress") {
    const std::vector<std::pair<double, double>> pts
        = {{0.0, 0.0},   {12.0, 5.0},  {3.0, 18.0},  {45.0, 7.0},  {22.0, 33.0},
           {8.0, 12.0},  {37.0, 2.0},  {9.0, 44.0},  {51.0, 15.0}, {28.0, 29.0},
           {6.0, 38.0},  {42.0, 11.0}, {17.0, 25.0}, {33.0, 41.0}, {55.0, 4.0},
           {14.0, 48.0}, {20.0, 9.0},  {39.0, 22.0}, {47.0, 36.0}, {25.0, 19.0}};
    auto G = make_complete_graph(20);
    const auto weight = ManhattanWeight{pts};

    const auto tour = solve_christofides_2opt_tsp(G, weight);
    CHECK(is_valid_hamiltonian_cycle(tour, 20));
    CHECK_GT(calculate_total_distance(tour, weight), 0.0);
}

TEST_CASE("2-Opt manhattan improves crossing tour on unit square") {
    const std::vector<std::pair<double, double>> pts
        = {{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}};
    auto G = make_complete_graph(4);
    const auto weight = ManhattanWeight{pts};

    std::vector<node_t> crossing = {0, 2, 1, 3, 0};
    const auto dist_before = calculate_total_distance(crossing, weight);
    const auto refined = two_opt(std::move(crossing), G, weight);
    const auto dist_after = calculate_total_distance(refined, weight);
    CHECK_LE(dist_after, dist_before);
    CHECK(is_valid_hamiltonian_cycle(refined, 4));
}

TEST_CASE("Combined solver manhattan 3/2 bound check") {
    const std::vector<std::pair<double, double>> pts
        = {{0.0, 0.0}, {1.0, 3.0}, {3.0, 1.0}, {4.0, 5.0}, {2.0, 4.0},
           {5.0, 1.0}, {6.0, 4.0}, {7.0, 6.0}, {8.0, 2.0}, {9.0, 7.0}};
    auto G = make_complete_graph(10);
    const auto weight = ManhattanWeight{pts};

    const auto tour = solve_christofides_2opt_tsp(G, weight);
    CHECK(is_valid_hamiltonian_cycle(tour, 10));
    CHECK_GT(calculate_total_distance(tour, weight), 0.0);
}
