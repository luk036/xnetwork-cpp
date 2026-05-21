#include <doctest/doctest.h>

#include <cstdint>
#include <py2cpp/dict.hpp>
#include <py2cpp/range.hpp>
#include <py2cpp/set.hpp>
#include <utility>
#include <xnetwork/classes/graph.hpp>  // for SimpleGraph
#include <xnetwork/rand_cover.hpp>

// Helper: verify every edge is covered
template <typename Graph, typename CoverSet>
bool is_valid_vertex_cover(const Graph& ugraph, const CoverSet& soln) {
    for (const auto& edge : ugraph.edges()) {
        if (!soln.contains(edge.first) && !soln.contains(edge.second)) {
            return false;
        }
    }
    return true;
}

// ============================================================================
// rand_vertex_cover (single trial)
// ============================================================================

TEST_CASE("rand_vertex_cover simple triangle") {
    xnetwork::SimpleGraph ugraph(3);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    ugraph.add_edge(2, 0);
    py::dict<uint32_t, int> weight{{0, 1}, {1, 1}, {2, 1}};

    auto [soln, cost] = rand_vertex_cover(ugraph, weight, 42);
    CHECK_EQ(soln.size(), 2);  // optimum for triangle is 2
    CHECK_EQ(cost, 2);
    CHECK(is_valid_vertex_cover(ugraph, soln));
}

TEST_CASE("rand_vertex_cover line") {
    xnetwork::SimpleGraph ugraph(3);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    py::dict<uint32_t, int> weight{{0, 1}, {1, 1}, {2, 1}};

    auto [soln, cost] = rand_vertex_cover(ugraph, weight, 1);
    CHECK(1 <= cost);
    CHECK(cost <= 2);
    CHECK(is_valid_vertex_cover(ugraph, soln));
}

TEST_CASE("rand_vertex_cover star") {
    xnetwork::SimpleGraph ugraph(4);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(0, 2);
    ugraph.add_edge(0, 3);
    py::dict<uint32_t, int> weight{{0, 1}, {1, 1}, {2, 1}, {3, 1}};

    auto [soln, cost] = rand_vertex_cover(ugraph, weight, 2);
    CHECK(1 <= cost);
    CHECK(cost <= 3);
    CHECK(is_valid_vertex_cover(ugraph, soln));
}

TEST_CASE("rand_vertex_cover weighted") {
    // Lighter vertex should be selected
    xnetwork::SimpleGraph ugraph(2);
    ugraph.add_edge(0, 1);
    py::dict<uint32_t, int> weight{{0, 100}, {1, 1}};

    auto [soln, cost] = rand_vertex_cover(ugraph, weight, 42);
    CHECK_EQ(soln.size(), 1);
    CHECK(soln.contains(1));  // lighter vertex chosen by Pitt's rule
    CHECK_EQ(cost, 1);
}

TEST_CASE("rand_vertex_cover empty graph") {
    xnetwork::SimpleGraph ugraph(0);
    py::dict<uint32_t, int> weight;

    auto [soln, cost] = rand_vertex_cover(ugraph, weight, 0);
    CHECK(soln.empty());
    CHECK_EQ(cost, 0);
}

TEST_CASE("rand_vertex_cover single edge") {
    xnetwork::SimpleGraph ugraph(2);
    ugraph.add_edge(0, 1);
    py::dict<uint32_t, int> weight{{0, 1}, {1, 1}};

    auto [soln, cost] = rand_vertex_cover(ugraph, weight, 7);
    CHECK_EQ(soln.size(), 1);
    CHECK_EQ(cost, 1);
    CHECK(is_valid_vertex_cover(ugraph, soln));
}

TEST_CASE("rand_vertex_cover deterministic seed") {
    xnetwork::SimpleGraph ugraph(4);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    ugraph.add_edge(2, 3);
    ugraph.add_edge(3, 0);
    py::dict<uint32_t, int> weight{{0, 2}, {1, 3}, {2, 1}, {3, 4}};

    auto [soln1, cost1] = rand_vertex_cover(ugraph, weight, 123);
    auto [soln2, cost2] = rand_vertex_cover(ugraph, weight, 123);

    CHECK_EQ(soln1, soln2);
    CHECK_EQ(cost1, cost2);
}

TEST_CASE("rand_vertex_cover with initial coverset") {
    xnetwork::SimpleGraph ugraph(3);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    ugraph.add_edge(2, 0);
    py::dict<uint32_t, int> weight{{0, 1}, {1, 1}, {2, 1}};

    py::set<uint32_t> initial{0};
    auto [soln, cost] = rand_vertex_cover(ugraph, weight, 42, initial);

    CHECK(soln.contains(0));  // pre-seeded vertex preserved
    CHECK(is_valid_vertex_cover(ugraph, soln));
}

// ============================================================================
// rand_vertex_cover_mt (multi-threaded)
// ============================================================================

TEST_CASE("rand_vertex_cover_mt simple triangle") {
    xnetwork::SimpleGraph ugraph(3);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    ugraph.add_edge(2, 0);
    py::dict<uint32_t, int> weight{{0, 1}, {1, 1}, {2, 1}};

    // With many trials, should reliably find cover of size 2
    auto [soln, cost] = rand_vertex_cover_mt(ugraph, weight, 128, 42);
    CHECK_EQ(cost, 2);
    CHECK(is_valid_vertex_cover(ugraph, soln));
}

TEST_CASE("rand_vertex_cover_mt weighted") {
    xnetwork::SimpleGraph ugraph(2);
    ugraph.add_edge(0, 1);
    py::dict<uint32_t, int> weight{{0, 100}, {1, 1}};

    // Many trials: lighter vertex should always win
    auto [soln, cost] = rand_vertex_cover_mt(ugraph, weight, 256, 42);
    CHECK_EQ(soln.size(), 1);
    CHECK(soln.contains(1));
    CHECK_EQ(cost, 1);
}

TEST_CASE("rand_vertex_cover_mt empty graph") {
    xnetwork::SimpleGraph ugraph(0);
    py::dict<uint32_t, int> weight;

    auto [soln, cost] = rand_vertex_cover_mt(ugraph, weight, 64, 0);
    CHECK(soln.empty());
    CHECK_EQ(cost, 0);
}

TEST_CASE("rand_vertex_cover_mt deterministic seed") {
    xnetwork::SimpleGraph ugraph(4);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    ugraph.add_edge(2, 3);
    ugraph.add_edge(3, 0);
    py::dict<uint32_t, int> weight{{0, 2}, {1, 3}, {2, 1}, {3, 4}};

    auto [soln1, cost1] = rand_vertex_cover_mt(ugraph, weight, 64, 123);
    auto [soln2, cost2] = rand_vertex_cover_mt(ugraph, weight, 64, 123);

    CHECK_EQ(soln1, soln2);
    CHECK_EQ(cost1, cost2);
}

TEST_CASE("rand_vertex_cover_mt with initial coverset") {
    xnetwork::SimpleGraph ugraph(3);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    ugraph.add_edge(2, 0);
    py::dict<uint32_t, int> weight{{0, 1}, {1, 1}, {2, 1}};

    py::set<uint32_t> initial{0};
    auto [soln, cost] = rand_vertex_cover_mt(ugraph, weight, 64, 42, initial);

    CHECK(soln.contains(0));
    CHECK(is_valid_vertex_cover(ugraph, soln));
}

TEST_CASE("rand_vertex_cover_mt larger graph") {
    // Manually construct a graph with 8 nodes and ~20 edges
    xnetwork::SimpleGraph ugraph(8);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(0, 2);
    ugraph.add_edge(0, 3);
    ugraph.add_edge(1, 2);
    ugraph.add_edge(1, 4);
    ugraph.add_edge(2, 3);
    ugraph.add_edge(2, 5);
    ugraph.add_edge(3, 6);
    ugraph.add_edge(4, 5);
    ugraph.add_edge(4, 7);
    ugraph.add_edge(5, 6);
    ugraph.add_edge(5, 7);
    ugraph.add_edge(6, 7);
    py::dict<uint32_t, int> weight{{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}};

    auto [soln, cost] = rand_vertex_cover_mt(ugraph, weight, 128, 1);
    CHECK(is_valid_vertex_cover(ugraph, soln));
    CHECK_LE(cost, 8);  // can't exceed all vertices
    CHECK_GE(cost, 1);  // must have at least one vertex
}
