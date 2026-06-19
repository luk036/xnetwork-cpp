#include <doctest/doctest.h>

#include <cstdint>
#include <optional>
#include <py2cpp/dict.hpp>
#include <py2cpp/set.hpp>
#include <utility>
#include <vector>
#include <xnetwork/classes/graph.hpp>  // for SimpleGraph
#include <xnetwork/cover.hpp>

TEST_CASE("Test pd_cover basic") {
    // A violator that returns three pre-defined edges one at a time.
    struct Violator {
        std::vector<std::vector<uint32_t>> edges{{0, 1}, {0, 2}, {1, 2}};
        std::size_t idx = 0;
        std::optional<std::vector<uint32_t>> operator()() {
            if (idx >= edges.size()) return std::nullopt;
            return edges[idx++];
        }
    };
    auto make_violate = []() { return Violator{}; };

    py::dict<uint32_t, int> weight;
    weight[0] = 1;
    weight[1] = 2;
    weight[2] = 3;

    py::set<uint32_t> soln;
    auto [covered, cost] = pd_cover(make_violate, weight, soln);

    CHECK(covered.contains(0));
    CHECK(covered.contains(1));
    CHECK_FALSE(covered.contains(2));
    CHECK_EQ(cost, 3);
}

TEST_CASE("Test min_vertex_cover simple") {
    xnetwork::SimpleGraph ugraph(3);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    py::dict<uint32_t, int> weight;
    weight[0] = 1;
    weight[1] = 1;
    weight[2] = 1;

    py::set<uint32_t> soln;
    auto [covered, cost] = min_vertex_cover(ugraph, weight, soln);

    // Verify all edges are covered
    for (const auto& edge : ugraph.edges()) {
        auto expression = covered.contains(edge.first) || covered.contains(edge.second);
        CHECK(expression);
    }
}

TEST_CASE("Test min_cycle_cover triangle") {
    xnetwork::SimpleGraph ugraph(3);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    ugraph.add_edge(2, 0);
    py::dict<uint32_t, int> weight{{0, 1}, {1, 1}, {2, 1}};

    py::set<uint32_t> soln;
    auto [covered, cost] = min_cycle_cover(ugraph, weight, soln);

    // Should cover at least one vertex to break the cycle
    CHECK_GE(covered.size(), 1);
    CHECK_GE(cost, 1);
}

TEST_CASE("Test min_odd_cycle_cover triangle") {
    xnetwork::SimpleGraph ugraph(3);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    ugraph.add_edge(2, 0);
    py::dict<uint32_t, int> weight{{0, 1}, {1, 1}, {2, 1}};

    py::set<uint32_t> soln;
    auto [covered, cost] = min_odd_cycle_cover(ugraph, weight, soln);

    // Should cover at least one vertex to break the odd cycle
    CHECK_GE(covered.size(), 1);
    CHECK_GE(cost, 1);
}

TEST_CASE("Test construct_cycle") {
    py::dict<uint32_t, BFSInfo<uint32_t>> info;

    info.insert_or_assign(static_cast<uint32_t>(0), BFSInfo<uint32_t>(static_cast<uint32_t>(0), 3));
    info.insert_or_assign(static_cast<uint32_t>(1), BFSInfo<uint32_t>(static_cast<uint32_t>(0), 2));
    info.insert_or_assign(static_cast<uint32_t>(2), BFSInfo<uint32_t>(static_cast<uint32_t>(1), 1));
    info.insert_or_assign(static_cast<uint32_t>(3), BFSInfo<uint32_t>(static_cast<uint32_t>(2), 0));

    auto cycle = construct_cycle<uint32_t>(info, 1, 3);

    // Cycle should contain the path
    CHECK_GE(cycle.size(), 2);
}

TEST_CASE("Test empty graph") {
    xnetwork::SimpleGraph ugraph(0);
    py::dict<uint32_t, int> weight;

    auto [covered, cost] = min_vertex_cover(ugraph, weight);
    CHECK(covered.empty());
    CHECK_EQ(cost, 0);

    auto [covered2, cost2] = min_cycle_cover(ugraph, weight);
    CHECK(covered2.empty());
    CHECK_EQ(cost2, 0);
}

TEST_CASE("Test single vertex") {
    xnetwork::SimpleGraph ugraph(1);
    py::dict<uint32_t, int> weight{{0, 5}};

    auto [covered, cost] = min_vertex_cover(ugraph, weight);
    CHECK(covered.empty());  // No edges to cover
    CHECK_EQ(cost, 0);
}

TEST_CASE("Test minimal vertex cover triangle") {
    // A triangle needs 2 vertices to cover all edges
    xnetwork::SimpleGraph ugraph(3);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    ugraph.add_edge(2, 0);
    py::dict<uint32_t, int> weight{{0, 1}, {1, 1}, {2, 1}};

    py::set<uint32_t> soln;
    auto [covered, cost] = min_vertex_cover(ugraph, weight, soln);

    // Any 2 nodes cover all edges in a triangle.
    // Post-processing ensures it doesn't accidentally pick all 3.
    CHECK_EQ(covered.size(), 2);
    CHECK_EQ(cost, 2);
}

TEST_CASE("Test cycle cover tree") {
    // A tree has no cycles → cycle cover should be empty
    xnetwork::SimpleGraph ugraph(4);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    ugraph.add_edge(2, 3);
    py::dict<uint32_t, int> weight{{0, 1}, {1, 1}, {2, 1}, {3, 1}};

    py::set<uint32_t> soln;
    auto [covered, cost] = min_cycle_cover(ugraph, weight, soln);

    CHECK(covered.empty());
    CHECK_EQ(cost, 0);
}

TEST_CASE("Test odd cycle cover detection") {
    // Square (even) + Triangle (odd) in one graph
    xnetwork::SimpleGraph ugraph(7);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(1, 2);
    ugraph.add_edge(2, 3);
    ugraph.add_edge(3, 0);  // even cycle (square)
    ugraph.add_edge(4, 5);
    ugraph.add_edge(5, 6);
    ugraph.add_edge(6, 4);  // odd cycle (triangle)
    py::dict<uint32_t, int> weight{{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}};

    py::set<uint32_t> soln;
    auto [covered, cost] = min_odd_cycle_cover(ugraph, weight, soln);

    // Should only pick a vertex from the triangle (nodes 4, 5, or 6)
    CHECK_MESSAGE((covered.contains(4) || covered.contains(5) || covered.contains(6)),
                  "Should cover at least one odd cycle vertex");
    CHECK_MESSAGE((!covered.contains(0) && !covered.contains(1) && !covered.contains(2)
                   && !covered.contains(3)),
                  "Should not cover any even cycle vertex");
}

TEST_CASE("Test post-processing minimality") {
    // K5: complete graph with 5 vertices, all weights = 1
    xnetwork::SimpleGraph ugraph(5);
    ugraph.add_edge(0, 1);
    ugraph.add_edge(0, 2);
    ugraph.add_edge(0, 3);
    ugraph.add_edge(0, 4);
    ugraph.add_edge(1, 2);
    ugraph.add_edge(1, 3);
    ugraph.add_edge(1, 4);
    ugraph.add_edge(2, 3);
    ugraph.add_edge(2, 4);
    ugraph.add_edge(3, 4);
    py::dict<uint32_t, int> weight{{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}};

    py::set<uint32_t> soln;
    auto [covered, cost] = min_vertex_cover(ugraph, weight, soln);

    // For K5, a minimal vertex cover has 4 nodes.
    // Verify: removing any node from the solution breaks the cover.
    for (const auto& node : covered) {
        py::set<uint32_t> test_soln = covered.copy();
        test_soln.erase(node);
        bool found_uncovered = false;
        for (const auto& edge : ugraph.edges()) {
            if (!test_soln.contains(edge.first) && !test_soln.contains(edge.second)) {
                found_uncovered = true;
                break;
            }
        }
        CHECK_MESSAGE(found_uncovered, "Node " << node << " was redundant in the cover");
    }
}
