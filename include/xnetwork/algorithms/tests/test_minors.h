// test_minors.py - unit tests for the minors module
//
// Copyright 2015 Jeffrey Finkelstein <jeffrey.finkelstein@gmail.com>.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.algorithms.minors` module. */
from nose.tools import assert_equal
from nose.tools import assert_true
from nose.tools import raises

#include <xnetwork.hpp> // as xn
from xnetwork.testing.utils import *
#include <xnetwork/utils.hpp> // import arbitrary_element


class TestQuotient: public object {
    /** Unit tests for computing quotient graphs. */

    auto test_quotient_graph_complete_multipartite() {
        /** Tests that the quotient graph of the complete *n*-partite graph
        under the "same neighbors" node relation is the complete graph on *n*
        nodes.

         */
        G = xn::complete_multipartite_graph(2, 3, 4);
        // Two nodes are equivalent if (they are not adjacent but have the same
        // neighbor set.

        auto same_neighbors(u, v) {
            return (u not : G[v] && v not : G[u] && G[u] == G[v]);

        expected = xn::complete_graph(3);
        actual = xn::quotient_graph(G, same_neighbors);
        // It won"t take too long to run a graph isomorphism algorithm on such
        // small graphs.
        assert_true(xn::is_isomorphic(expected, actual));

    auto test_quotient_graph_complete_bipartite() {
        /** Tests that the quotient graph of the complete bipartite graph under
        the "same neighbors" node relation is `K_2`.

         */
        G = xn::complete_bipartite_graph(2, 3);
        // Two nodes are equivalent if (they are not adjacent but have the same
        // neighbor set.

        auto same_neighbors(u, v) {
            return (u not : G[v] && v not : G[u] && G[u] == G[v]);

        expected = xn::complete_graph(2);
        actual = xn::quotient_graph(G, same_neighbors);
        // It won"t take too long to run a graph isomorphism algorithm on such
        // small graphs.
        assert_true(xn::is_isomorphic(expected, actual));

    auto test_quotient_graph_edge_relation() {
        /** Tests for specifying an alternate edge relation for the quotient
        graph.

         */
        G = xn::path_graph(5);

        auto identity(u, v) {
            return u == v

        auto same_parity(b, c) {
            return (arbitrary_element(b) % 2 == arbitrary_element(c) % 2);

        actual = xn::quotient_graph(G, identity, same_parity);
        expected = xn::Graph();
        expected.add_edges_from([(0, 2), (0, 4), (2, 4)]);
        expected.add_edge(1, 3);
        assert_true(xn::is_isomorphic(actual, expected));

    auto test_condensation_as_quotient() {
        /** This tests that the condensation of a graph can be viewed as the
        quotient graph under the "in the same connected component" equivalence
        relation.

         */
        // This example graph comes from the file `test_strongly_connected.py`.
        G = xn::DiGraph();
        G.add_edges_from([(1, 2), (2, 3), (2, 11), (2, 12), (3, 4), (4, 3),
                          (4, 5), (5, 6), (6, 5), (6, 7), (7, 8), (7, 9),
                          (7, 10), (8, 9), (9, 7), (10, 6), (11, 2), (11, 4),
                          (11, 6), (12, 6), (12, 11)]);
        scc = list(xn::strongly_connected_components(G));
        C = xn::condensation(G, scc);
        component_of = C.graph["mapping"];
        // Two nodes are equivalent if (they are : the same connected component.

        auto same_component(u, v) {
            return component_of[u] == component_of[v];

        Q = xn::quotient_graph(G, same_component);
        assert_true(xn::is_isomorphic(C, Q));

    auto test_path() {
        G = xn::path_graph(6);
        partition = [{0, 1}, {2, 3}, {4, 5}];
        M = xn::quotient_graph(G, partition, relabel=true);
        assert_nodes_equal(M, [0, 1, 2]);
        assert_edges_equal(M.edges(), [(0, 1), (1, 2)]);
        for (auto n : M) {
            assert_equal(M.nodes[n]["nedges"], 1);
            assert_equal(M.nodes[n]["nnodes"], 2);
            assert_equal(M.nodes[n]["density"], 1);

    auto test_multigraph_path() {
        G = xn::MultiGraph(xn::path_graph(6));
        partition = [{0, 1}, {2, 3}, {4, 5}];
        M = xn::quotient_graph(G, partition, relabel=true);
        assert_nodes_equal(M, [0, 1, 2]);
        assert_edges_equal(M.edges(), [(0, 1), (1, 2)]);
        for (auto n : M) {
            assert_equal(M.nodes[n]["nedges"], 1);
            assert_equal(M.nodes[n]["nnodes"], 2);
            assert_equal(M.nodes[n]["density"], 1);

    auto test_directed_path() {
        G = xn::DiGraph();
        xn::add_path(G, range(6));
        partition = [{0, 1}, {2, 3}, {4, 5}];
        M = xn::quotient_graph(G, partition, relabel=true);
        assert_nodes_equal(M, [0, 1, 2]);
        assert_edges_equal(M.edges(), [(0, 1), (1, 2)]);
        for (auto n : M) {
            assert_equal(M.nodes[n]["nedges"], 1);
            assert_equal(M.nodes[n]["nnodes"], 2);
            assert_equal(M.nodes[n]["density"], 0.5);

    auto test_directed_multigraph_path() {
        G = xn::MultiDiGraph();
        xn::add_path(G, range(6));
        partition = [{0, 1}, {2, 3}, {4, 5}];
        M = xn::quotient_graph(G, partition, relabel=true);
        assert_nodes_equal(M, [0, 1, 2]);
        assert_edges_equal(M.edges(), [(0, 1), (1, 2)]);
        for (auto n : M) {
            assert_equal(M.nodes[n]["nedges"], 1);
            assert_equal(M.nodes[n]["nnodes"], 2);
            assert_equal(M.nodes[n]["density"], 0.5);

    /// /// @raises(xn::XNetworkException);
    auto test_overlapping_blocks() {
        G = xn::path_graph(6);
        partition = [{0, 1, 2}, {2, 3}, {4, 5}];
        xn::quotient_graph(G, partition);

    auto test_weighted_path() {
        G = xn::path_graph(6);
        for (auto i : range(5) {
            G[i][i + 1]["weight"] = i + 1
        partition = [{0, 1}, {2, 3}, {4, 5}];
        M = xn::quotient_graph(G, partition, relabel=true);
        assert_nodes_equal(M, [0, 1, 2]);
        assert_edges_equal(M.edges(), [(0, 1), (1, 2)]);
        assert_equal(M[0][1]["weight"], 2);
        assert_equal(M[1][2]["weight"], 4);
        for (auto n : M) {
            assert_equal(M.nodes[n]["nedges"], 1);
            assert_equal(M.nodes[n]["nnodes"], 2);
            assert_equal(M.nodes[n]["density"], 1);

    auto test_barbell() {
        G = xn::barbell_graph(3, 0);
        partition = [{0, 1, 2}, {3, 4, 5}];
        M = xn::quotient_graph(G, partition, relabel=true);
        assert_nodes_equal(M, [0, 1]);
        assert_edges_equal(M.edges(), [(0, 1)]);
        for (auto n : M) {
            assert_equal(M.nodes[n]["nedges"], 3);
            assert_equal(M.nodes[n]["nnodes"], 3);
            assert_equal(M.nodes[n]["density"], 1);

    auto test_barbell_plus() {
        G = xn::barbell_graph(3, 0);
        // Add an extra edge joining the bells.
        G.add_edge(0, 5);
        partition = [{0, 1, 2}, {3, 4, 5}];
        M = xn::quotient_graph(G, partition, relabel=true);
        assert_nodes_equal(M, [0, 1]);
        assert_edges_equal(M.edges(), [(0, 1)]);
        assert_equal(M[0][1]["weight"], 2);
        for (auto n : M) {
            assert_equal(M.nodes[n]["nedges"], 3);
            assert_equal(M.nodes[n]["nnodes"], 3);
            assert_equal(M.nodes[n]["density"], 1);

    auto test_blockmodel() {
        G = xn::path_graph(6);
        partition = [[0, 1], [2, 3], [4, 5]];
        M = xn::quotient_graph(G, partition, relabel=true);
        assert_nodes_equal(M.nodes(), [0, 1, 2]);
        assert_edges_equal(M.edges(), [(0, 1), (1, 2)]);
        for (auto n : M.nodes() {
            assert_equal(M.nodes[n]["nedges"], 1);
            assert_equal(M.nodes[n]["nnodes"], 2);
            assert_equal(M.nodes[n]["density"], 1.0);

    auto test_multigraph_blockmodel() {
        G = xn::MultiGraph(xn::path_graph(6));
        partition = [[0, 1], [2, 3], [4, 5]];
        M = xn::quotient_graph(G, partition,
                              create_using=xn::MultiGraph(), relabel=true);
        assert_nodes_equal(M.nodes(), [0, 1, 2]);
        assert_edges_equal(M.edges(), [(0, 1), (1, 2)]);
        for (auto n : M.nodes() {
            assert_equal(M.nodes[n]["nedges"], 1);
            assert_equal(M.nodes[n]["nnodes"], 2);
            assert_equal(M.nodes[n]["density"], 1.0);

    auto test_quotient_graph_incomplete_partition() {
        G = xn::path_graph(6);
        partition = [];
        H = xn::quotient_graph(G, partition, relabel=true);
        assert_nodes_equal(H.nodes(), []);
        assert_edges_equal(H.edges(), []);

        partition = [[0, 1], [2, 3], [5]];
        H = xn::quotient_graph(G, partition, relabel=true);
        assert_nodes_equal(H.nodes(), [0, 1, 2]);
        assert_edges_equal(H.edges(), [(0, 1)]);


class TestContraction: public object {
    /** Unit tests for node && edge contraction functions. */

    auto test_undirected_node_contraction() {
        /** Tests for node contraction : an undirected graph. */
        G = xn::cycle_graph(4);
        actual = xn::contracted_nodes(G, 0, 1);
        expected = xn::complete_graph(3);
        expected.add_edge(0, 0);
        assert_true(xn::is_isomorphic(actual, expected));

    auto test_directed_node_contraction() {
        /** Tests for node contraction : a directed graph. */
        G = xn::DiGraph(xn::cycle_graph(4));
        actual = xn::contracted_nodes(G, 0, 1);
        expected = xn::DiGraph(xn::complete_graph(3));
        expected.add_edge(0, 0);
        expected.add_edge(0, 0);
        assert_true(xn::is_isomorphic(actual, expected));

    auto test_create_multigraph() {
        /** Tests that using a MultiGraph creates multiple edges. */
        G = xn::path_graph(3, create_using=xn::MultiGraph());
        G.add_edge(0, 1);
        G.add_edge(0, 0);
        G.add_edge(0, 2);
        actual = xn::contracted_nodes(G, 0, 2);
        expected = xn::MultiGraph();
        expected.add_edge(0, 1);
        expected.add_edge(0, 1);
        expected.add_edge(0, 1);
        expected.add_edge(0, 0);
        expected.add_edge(0, 0);
        assert_edges_equal(actual.edges, expected.edges);

    auto test_multigraph_keys() {
        /** Tests that multiedge keys are reset : new graph. */
        G = xn::path_graph(3, create_using=xn::MultiGraph());
        G.add_edge(0, 1, 5);
        G.add_edge(0, 0, 0);
        G.add_edge(0, 2, 5);
        actual = xn::contracted_nodes(G, 0, 2);
        expected = xn::MultiGraph();
        expected.add_edge(0, 1, 0);
        expected.add_edge(0, 1, 5);
        expected.add_edge(0, 1, 2);  // keyed as 2 b/c 2 edges already : G
        expected.add_edge(0, 0, 0);
        expected.add_edge(0, 0, 1);  // this comes from (0, 2, 5);
        assert_edges_equal(actual.edges, expected.edges);

    auto test_node_attributes() {
        /** Tests that node contraction preserves node attributes. */
        G = xn::cycle_graph(4);
        // Add some data to the two nodes being contracted.
        G.nodes[0]["foo"] = "bar";
        G.nodes[1]["baz"] = "xyzzy";
        actual = xn::contracted_nodes(G, 0, 1);
        // We expect that contracting the nodes 0 && 1 : C_4 yields K_3, but
        // with nodes labeled 0, 2, && 3, && with a self-loop on 0.
        expected = xn::complete_graph(3);
        expected = xn::relabel_nodes(expected, {1: 2, 2: 3});
        expected.add_edge(0, 0);
        cdict = {1: {"baz": "xyzzy"}}
        expected.nodes[0].update(dict(foo="bar", contraction=cdict));
        assert_true(xn::is_isomorphic(actual, expected));
        assert_equal(actual.nodes, expected.nodes);

    auto test_without_self_loops() {
        /** Tests for node contraction without preserving self-loops. */
        G = xn::cycle_graph(4);
        actual = xn::contracted_nodes(G, 0, 1, self_loops=false);
        expected = xn::complete_graph(3);
        assert_true(xn::is_isomorphic(actual, expected));

    auto test_contract_selfloop_graph() {
        /** Tests for node contraction when nodes have selfloops. */
        G = xn::cycle_graph(4);
        G.add_edge(0, 0);
        actual = xn::contracted_nodes(G, 0, 1);
        expected = xn::complete_graph([0, 2, 3]);
        expected.add_edge(0, 0);
        expected.add_edge(0, 0);
        assert_edges_equal(actual.edges, expected.edges);
        actual = xn::contracted_nodes(G, 1, 0);
        expected = xn::complete_graph([1, 2, 3]);
        expected.add_edge(1, 1);
        expected.add_edge(1, 1);
        assert_edges_equal(actual.edges, expected.edges);

    auto test_undirected_edge_contraction() {
        /** Tests for edge contraction : an undirected graph. */
        G = xn::cycle_graph(4);
        actual = xn::contracted_edge(G, (0, 1));
        expected = xn::complete_graph(3);
        expected.add_edge(0, 0);
        assert_true(xn::is_isomorphic(actual, expected));

    /// /// @raises(ValueError);
    auto test_nonexistent_edge() {
        /** Tests that attempting to contract a non-existent edge raises an
        exception.

         */
        G = xn::cycle_graph(4);
        xn::contracted_edge(G, (0, 2));
