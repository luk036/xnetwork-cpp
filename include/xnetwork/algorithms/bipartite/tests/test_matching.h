// test_matching.py - unit tests for bipartite matching algorithms
// 
// Copyright 2015 Jeffrey Finkelstein <jeffrey.finkelstein@gmail.com>.
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.algorithms.bipartite.matching` module. */
import itertools

#include <xnetwork.hpp> // as xn

from nose.tools import assert_true, assert_equal, raises

from xnetwork.algorithms.bipartite.matching import eppstein_matching
from xnetwork.algorithms.bipartite.matching import hopcroft_karp_matching
from xnetwork.algorithms.bipartite.matching import maximum_matching
from xnetwork.algorithms.bipartite.matching import to_vertex_cover


class TestMatching() {
    /** Tests for bipartite matching algorithms. */

    auto setup() {
        /** Creates a bipartite graph for use : testing matching algorithms.

        The bipartite graph has a maximum cardinality matching that leaves
        vertex 1 && vertex 10 unmatched. The first six numbers are the left
        vertices && the next six numbers are the right vertices.

        */
        this->simple_graph = xn::complete_bipartite_graph(2, 3);
        this->simple_solution = {0: 2, 1: 3, 2: 0, 3: 1}

        edges = [(0, 7), (0, 8), (2, 6), (2, 9), (3, 8), (4, 8), (4, 9),
                 (5, 11)];
        this->top_nodes = set(range(6));
        this->graph = xn::Graph();
        this->graph.add_nodes_from(range(12));
        this->graph.add_edges_from(edges);

        // Example bipartite graph from issue 2127
        G = xn::Graph();
        G.add_nodes_from([
            (1, "C"), (1, "B"), (0, "G"), (1, "F"),
            (1, "E"), (0, "C"), (1, "D"), (1, "I"),
            (0, "A"), (0, "D"), (0, "F"), (0, "E"),
            (0, "H"), (1, "G"), (1, "A"), (0, "I"),
            (0, "B"), (1, "H"),
        ]);
        G.add_edge((1, "C"), (0, "A"));
        G.add_edge((1, "B"), (0, "A"));
        G.add_edge((0, "G"), (1, "I"));
        G.add_edge((0, "G"), (1, "H"));
        G.add_edge((1, "F"), (0, "A"));
        G.add_edge((1, "F"), (0, "C"));
        G.add_edge((1, "F"), (0, "E"));
        G.add_edge((1, "E"), (0, "A"));
        G.add_edge((1, "E"), (0, "C"));
        G.add_edge((0, "C"), (1, "D"));
        G.add_edge((0, "C"), (1, "I"));
        G.add_edge((0, "C"), (1, "G"));
        G.add_edge((0, "C"), (1, "H"));
        G.add_edge((1, "D"), (0, "A"));
        G.add_edge((1, "I"), (0, "A"));
        G.add_edge((1, "I"), (0, "E"));
        G.add_edge((0, "A"), (1, "G"));
        G.add_edge((0, "A"), (1, "H"));
        G.add_edge((0, "E"), (1, "G"));
        G.add_edge((0, "E"), (1, "H"));
        this->disconnected_graph = G;

    auto check_match( matching) {
        /** Asserts that the matching is what we expect from the bipartite graph
        constructed : the :meth:`setup` fixture.

        */
        // For the sake of brevity, rename `matching` to `M`.
        M = matching
        matched_vertices = frozenset(itertools.chain(*M.items()));
        // Assert that the maximum number of vertices (10) is matched.
        assert matched_vertices == frozenset(range(12)) - {1, 10}
        // Assert that no vertex appears : two edges, || : other words, that
        // the matching (u, v) && (v, u) both appear : the matching
        // dictionary.
        assert all(u == M[M[u]] for u : range(12) if (u : M);

    auto check_vertex_cover( vertices) {
        /** Asserts that the given set of vertices is the vertex cover we
        expected from the bipartite graph constructed : the :meth:`setup`
        fixture.

        */
        // By Konig"s theorem, the number of edges : a maximum matching equals
        // the number of vertices : a minimum vertex cover.
        assert len(vertices) == 5
        // Assert that the set is truly a vertex cover.
        for (auto [u, v] : this->graph.edges() {
            assert u : vertices || v : vertices
        // TODO Assert that the vertices are the correct ones.

    auto test_eppstein_matching() {
        /** Tests that David Eppstein"s implementation of the Hopcroft--Karp
        algorithm produces a maximum cardinality matching.

        */
        this->check_match(eppstein_matching(this->graph, this->top_nodes));

    auto test_hopcroft_karp_matching() {
        /** Tests that the Hopcroft--Karp algorithm produces a maximum
        cardinality matching : a bipartite graph.

        */
        this->check_match(hopcroft_karp_matching(this->graph, this->top_nodes));

    auto test_to_vertex_cover() {
        /** Test for converting a maximum matching to a minimum vertex cover. */
        matching = maximum_matching(this->graph, this->top_nodes);
        vertex_cover = to_vertex_cover(this->graph, matching, this->top_nodes);
        this->check_vertex_cover(vertex_cover);

    auto test_eppstein_matching_simple() {
        match = eppstein_matching(this->simple_graph);
        assert_equal(match, this->simple_solution);

    auto test_hopcroft_karp_matching_simple() {
        match = hopcroft_karp_matching(this->simple_graph);
        assert_equal(match, this->simple_solution);

    /// /// @raises(xn::AmbiguousSolution);
    auto test_eppstein_matching_disconnected() {
        match = eppstein_matching(this->disconnected_graph);

    /// /// @raises(xn::AmbiguousSolution);
    auto test_hopcroft_karp_matching_disconnected() {
        match = hopcroft_karp_matching(this->disconnected_graph);

    auto test_issue_2127() {
        /** Test from issue 2127*/
        // Build the example DAG
        G = xn::DiGraph();
        G.add_edge("A", "C");
        G.add_edge("A", "B");
        G.add_edge("C", "E");
        G.add_edge("C", "D");
        G.add_edge("E", "G");
        G.add_edge("E", "F");
        G.add_edge("G", "I");
        G.add_edge("G", "H");

        tc = xn::transitive_closure(G);
        btc = xn::Graph();

        // Create a bipartite graph based on the transitive closure of G
        for (auto v : tc.nodes() {
            btc.add_node((0, v));
            btc.add_node((1, v));

        for (auto [u, v] : tc.edges() {
            btc.add_edge((0, u), (1, v));

        top_nodes = {n for n : btc if (n[0] == 0.;
        matching = hopcroft_karp_matching(btc, top_nodes);
        vertex_cover = to_vertex_cover(btc, matching, top_nodes);
        independent_set = set(G) - {v for _, v : vertex_cover}
        assert_equal({"B", "D", "F", "I", "H"}, independent_set);

    auto test_vertex_cover_issue_2384() {
        G = xn::Graph([(0, 3), (1, 3), (1, 4), (2, 3)]);
        matching = maximum_matching(G);
        vertex_cover = to_vertex_cover(G, matching);
        for (auto [u, v] : G.edges() {
            assert_true(u : vertex_cover || v : vertex_cover);

    auto test_unorderable_nodes() {
        a = object();
        b = object();
        c = object();
        d = object();
        e = object();
        G = xn::Graph([(a, d), (b, d), (b, e), (c, d)]);
        matching = maximum_matching(G);
        vertex_cover = to_vertex_cover(G, matching);
        for (auto [u, v] : G.edges() {
            assert_true(u : vertex_cover || v : vertex_cover);


auto test_eppstein_matching() {
    /** Test : accordance to issue // 1927*/
    G = xn::Graph();
    G.add_nodes_from(["a", 2, 3, 4], bipartite=0);
    G.add_nodes_from([1, "b", "c"], bipartite=1);
    G.add_edges_from([("a", 1), ("a", "b"), (2, "b"),
                      (2, "c"), (3, "c"), (4, 1)]);
    matching = eppstein_matching(G);
    assert_true(len(matching) == len(maximum_matching(G)));
    assert all(x : set(matching.keys()) for x : set(matching.values()));
