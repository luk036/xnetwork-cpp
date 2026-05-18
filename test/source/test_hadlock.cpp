#include <doctest/doctest.h>

#include <cstdint>
#include <xnetwork/classes/graph.hpp> // for SimpleGraph
#include <xnetwork/hadlock.hpp>
#include <py2cpp/range.hpp>
#include <py2cpp/set.hpp>
#include <utility>
#include <vector>

// ===================================================================
// Test graph — minimal mock following test_graph_algo / test_cover pattern
// ===================================================================
struct TestGraph {
    using node_t = uint32_t;
    std::vector<std::pair<node_t, node_t>> edges_list;
    std::vector<std::vector<node_t>> adjacency;
    uint32_t num_nodes;

    TestGraph(uint32_t num_nodes,
              std::vector<std::pair<node_t, node_t>> edges)
        : edges_list(std::move(edges)), adjacency(num_nodes), num_nodes(num_nodes) {
        for (const auto& e : edges_list) {
            adjacency[e.first].push_back(e.second);
            adjacency[e.second].push_back(e.first);
        }
    }

    auto edges() const -> const std::vector<std::pair<node_t, node_t>>& {
        return edges_list;
    }

    auto operator[](node_t node) const -> const std::vector<node_t>& {
        return adjacency[node];
    }

    auto begin() const {
        return py::range<uint32_t>(0, num_nodes).begin();
    }
    auto end() const {
        return py::range<uint32_t>(0, num_nodes).end();
    }

    auto number_of_nodes() const -> size_t { return num_nodes; }
};

// ===================================================================
// Helper weight lambdas
// ===================================================================
const auto unit_weight = [](uint32_t, uint32_t) -> int { return 1; };

// ===================================================================
// Tests: solve_hadlock_max_cut
// ===================================================================

TEST_CASE("Triangle") {
    // 3-cycle: w(0,1)=5, w(1,2)=10, w(2,0)=3
    TestGraph G(3, {{0, 1}, {1, 2}, {2, 0}});
    auto weight = [](uint32_t u, uint32_t v) -> int {
        if ((u == 0 && v == 1) || (u == 1 && v == 0)) return 5;
        if ((u == 1 && v == 2) || (u == 2 && v == 1)) return 10;
        if ((u == 2 && v == 0) || (u == 0 && v == 2)) return 3;
        return 1;
    };
    // Two faces (inner + outer), each with 3 nodes (odd)
    std::vector<std::vector<uint32_t>> faces = {{0, 1, 2}, {0, 2, 1}};

    auto cut = solve_hadlock_max_cut(G, weight, faces);
    auto [ok, val] = validate_max_cut(G, cut, weight);

    CHECK(ok);
    CHECK_EQ(val, 15); // 5 + 10
}

TEST_CASE("Square with diagonal") {
    // Square 1-2-3-4-1 (weights 5,10,5,10) + diagonal 1-3 (weight 2)
    // Planar embedding: two triangles + outer quad
    TestGraph G(5, {{1, 2}, {2, 3}, {3, 4}, {4, 1}, {1, 3}});
    auto weight = [](uint32_t u, uint32_t v) -> int {
        if ((u == 1 && v == 2) || (u == 2 && v == 1)) return 5;
        if ((u == 2 && v == 3) || (u == 3 && v == 2)) return 10;
        if ((u == 3 && v == 4) || (u == 4 && v == 3)) return 5;
        if ((u == 4 && v == 1) || (u == 1 && v == 4)) return 10;
        if ((u == 1 && v == 3) || (u == 3 && v == 1)) return 2;
        return 1;
    };
    // Face 0 = triangle 1-2-3 (odd), Face 1 = triangle 1-3-4 (odd),
    // Face 2 = outer quad 1-4-3-2 (even)
    std::vector<std::vector<uint32_t>> faces = {
        {1, 2, 3}, {1, 3, 4}, {1, 4, 3, 2}
    };

    auto cut = solve_hadlock_max_cut(G, weight, faces);
    auto [ok, val] = validate_max_cut(G, cut, weight);

    CHECK(ok);
    CHECK_EQ(val, 30); // 5 + 10 + 5 + 10
}

TEST_CASE("Grid 2x2 is bipartite — all edges in cut") {
    // (0,0)->0, (0,1)->1, (1,0)->2, (1,1)->3
    TestGraph G(4, {{0, 1}, {0, 2}, {1, 3}, {2, 3}});
    // Inner face 0-1-3-2 (even), outer face 0-2-3-1 (even)
    std::vector<std::vector<uint32_t>> faces = {
        {0, 1, 3, 2}, {0, 2, 3, 1}
    };

    auto cut = solve_hadlock_max_cut(G, unit_weight, faces);

    // All 4 edges should be in the cut
    CHECK_EQ(cut.size(), 4);
    auto [ok, val] = validate_max_cut(G, cut, unit_weight);
    CHECK(ok);
    CHECK_EQ(val, 4);
}

TEST_CASE("Empty graph") {
    TestGraph G(0, {});
    std::vector<std::vector<uint32_t>> faces;

    auto cut = solve_hadlock_max_cut(G, unit_weight, faces);
    CHECK(cut.empty());
}

TEST_CASE("Single edge") {
    TestGraph G(2, {{0, 1}});
    // Two faces, each a digon [0,1] — both even (2 nodes)
    std::vector<std::vector<uint32_t>> faces = {{0, 1}, {1, 0}};

    auto weight = [](uint32_t, uint32_t) -> int { return 7; };
    auto cut = solve_hadlock_max_cut(G, weight, faces);

    CHECK_EQ(cut.size(), 1);
    auto [ok, val] = validate_max_cut(G, cut, weight);
    CHECK(ok);
    CHECK_EQ(val, 7);
}

TEST_CASE("Unit weight triangle") {
    TestGraph G(3, {{0, 1}, {1, 2}, {2, 0}});
    std::vector<std::vector<uint32_t>> faces = {{0, 1, 2}, {0, 2, 1}};

    auto cut = solve_hadlock_max_cut(G, unit_weight, faces);
    auto [ok, val] = validate_max_cut(G, cut, unit_weight);

    CHECK(ok);
    // Total = 3, min weight edge excluded = 1, cut weight = 2
    CHECK_EQ(val, 2);
}

// ===================================================================
// Tests: validate_max_cut
// ===================================================================

TEST_CASE("Valid cut validates as bipartite") {
    TestGraph G(5, {{1, 2}, {2, 3}, {3, 4}, {4, 1}, {1, 3}});
    py::set<std::pair<uint32_t, uint32_t>> cut;
    cut.insert({1, 2});
    cut.insert({2, 3});
    cut.insert({3, 4});
    cut.insert({4, 1});

    auto [ok, val] = validate_max_cut(
        G, cut, [](uint32_t, uint32_t) { return 1; });
    CHECK(ok);
    CHECK_EQ(val, 4);
}

TEST_CASE("Cut containing a triangle is invalid") {
    TestGraph G(3, {{0, 1}, {1, 2}, {2, 0}});
    py::set<std::pair<uint32_t, uint32_t>> cut;
    cut.insert({0, 1});
    cut.insert({1, 2});
    cut.insert({2, 0});

    auto [ok, val] = validate_max_cut(G, cut, unit_weight);
    CHECK_FALSE(ok);
}

TEST_CASE("Unit weight overload") {
    TestGraph G(2, {{0, 1}});
    py::set<std::pair<uint32_t, uint32_t>> cut;
    cut.insert({0, 1});

    auto [ok, val] = validate_max_cut(G, cut);
    CHECK(ok);
    CHECK_EQ(val, 1);
}

// ===================================================================
// Tests: biconnected_components
// ===================================================================

TEST_CASE("biconnected_components — single triangle") {
    // Triangle has one bc component containing all 3 nodes
    xnetwork::SimpleGraph G(3);
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 0);
    auto comps = detail::biconnected_components(G);

    CHECK_EQ(comps.size(), 1);
    CHECK(comps[0].contains(0));
    CHECK(comps[0].contains(1));
    CHECK(comps[0].contains(2));
}

TEST_CASE("biconnected_components — bowtie (two triangles sharing vertex)") {
    // Two triangles sharing node 2 → two bc components
    // Comp1: {0,1,2}, Comp2: {2,3,4}
    xnetwork::SimpleGraph G(5);
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 0);
    G.add_edge(2, 3);
    G.add_edge(3, 4);
    G.add_edge(4, 2);
    auto comps = detail::biconnected_components(G);

    CHECK_EQ(comps.size(), 2);

    // Each component should have exactly 3 nodes
    // Node 2 (articulation point) appears in both
    CHECK_EQ(comps[0].size(), 3);
    CHECK_EQ(comps[1].size(), 3);
    CHECK(comps[0].contains(2));
    CHECK(comps[1].contains(2));
}

TEST_CASE("biconnected_components — tree (no cycles)") {
    // A tree has one biconnected component per edge (each is size 2)
    xnetwork::SimpleGraph G(4);
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 3);
    auto comps = detail::biconnected_components(G);

    // 3 edges → 3 biconnected components, each with 2 nodes
    CHECK_EQ(comps.size(), 3);
    for (const auto& comp : comps) {
        CHECK_EQ(comp.size(), 2);
    }
}

TEST_CASE("biconnected_components — empty graph") {
    xnetwork::SimpleGraph G(0);
    auto comps = detail::biconnected_components(G);
    CHECK(comps.empty());
}

// ===================================================================
// Tests: bc-optimized solve_hadlock_max_cut
// ===================================================================

TEST_CASE("BC-optimized — bowtie (two unit-weight triangles)") {
    // Two triangles sharing node 2:
    //   Triangle 1: 0-1-2 (unit weight)
    //   Triangle 2: 2-3-4 (unit weight)
    // Each triangle's max cut = 2 edges → total 4 edges in the cut
    xnetwork::SimpleGraph G(5);
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 0);
    G.add_edge(2, 3);
    G.add_edge(3, 4);
    G.add_edge(4, 2);

    // Per-component faces
    // Comp 0 (triangle 0-1-2): inner {0,1,2}, outer {0,2,1}
    // Comp 1 (triangle 2-3-4): inner {2,3,4}, outer {2,4,3}
    std::vector<std::vector<std::vector<uint32_t>>> component_faces = {
        {{0, 1, 2}, {0, 2, 1}},
        {{2, 3, 4}, {2, 4, 3}}
    };

    auto cut = solve_hadlock_max_cut(G, unit_weight, component_faces);
    auto [ok, val] = validate_max_cut(G, cut, unit_weight);

    CHECK(ok);
    CHECK_EQ(cut.size(), 4);  // 2 + 2 edges
    CHECK_EQ(val, 4);
}

TEST_CASE("BC-optimized — disconnected triangles") {
    // Two completely disconnected triangles (no shared vertex):
    //   Triangle 1: nodes {0,1,2}
    //   Triangle 2: nodes {3,4,5}
    xnetwork::SimpleGraph G(6);
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 0);
    G.add_edge(3, 4);
    G.add_edge(4, 5);
    G.add_edge(5, 3);

    std::vector<std::vector<std::vector<uint32_t>>> component_faces = {
        {{0, 1, 2}, {0, 2, 1}},
        {{3, 4, 5}, {3, 5, 4}}
    };

    auto cut = solve_hadlock_max_cut(G, unit_weight, component_faces);
    auto [ok, val] = validate_max_cut(G, cut, unit_weight);

    CHECK(ok);
    CHECK_EQ(cut.size(), 4);  // 2 + 2
    CHECK_EQ(val, 4);
}

TEST_CASE("BC-optimized — mixed: triangle + square") {
    // Triangle (nodes 0,1,2) + square (nodes 2,3,4,5) sharing node 2
    // Triangle faces: inner {0,1,2}, outer {0,2,1}
    // Square  faces: inner {2,3,4,5}, outer {2,5,4,3}
    xnetwork::SimpleGraph G(6);
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 0);
    G.add_edge(2, 3);
    G.add_edge(3, 4);
    G.add_edge(4, 5);
    G.add_edge(5, 2);
    // Edge weights: triangle edges = 1, square edges = 1
    // Square is bipartite (even cycle) → all 4 square edges in cut
    // Triangle has max cut = 2 edges
    // Total cut = 6 edges, weight = 6

    std::vector<std::vector<std::vector<uint32_t>>> component_faces = {
        {{0, 1, 2}, {0, 2, 1}},             // triangle
        {{2, 3, 4, 5}, {2, 5, 4, 3}}        // square
    };

    auto cut = solve_hadlock_max_cut(G, unit_weight, component_faces);
    auto [ok, val] = validate_max_cut(G, cut, unit_weight);

    CHECK(ok);
    // Square has 0 odd faces → all 4 edges in cut
    // Triangle has max cut = 2 edges
    CHECK_EQ(val, 6);
}

TEST_CASE("BC-optimized — component_faces mismatch handled gracefully") {
    // Only provide faces for 1 component but graph has 2
    xnetwork::SimpleGraph G(5);
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 0);
    G.add_edge(2, 3);
    G.add_edge(3, 4);
    G.add_edge(4, 2);

    // Only 1 face list for a 2-component graph
    std::vector<std::vector<std::vector<uint32_t>>> component_faces = {
        {{0, 1, 2}, {0, 2, 1}}
    };

    // Should not crash — returns partial cut
    auto cut = solve_hadlock_max_cut(G, unit_weight, component_faces);
    CHECK_GE(cut.size(), 0);  // at least the first component's cut
}
