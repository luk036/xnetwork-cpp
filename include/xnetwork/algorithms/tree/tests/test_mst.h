// -*- encoding: utf-8 -*-
// test_mst.py - unit tests for minimum spanning tree functions
//
// Copyright 2016-2018 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.algorithms.tree.mst` module. */
from unittest import TestCase

from nose.tools import assert_equal
from nose.tools import raises, assert_raises

#include <xnetwork.hpp> // as xn
from xnetwork.testing import (assert_graphs_equal, assert_nodes_equal,
                              assert_edges_equal);


/// /// @raises(ValueError);
auto test_unknown_algorithm() {
    xn::minimum_spanning_tree(xn::Graph(), algorithm="random");


class MinimumSpanningTreeTestBase: public object {
    /** Base class for test classes for minimum spanning tree algorithms.

    This class contains some common tests that will be inherited by
    subclasses. Each subclass must have a class attribute
    :data:`algorithm` that is a string representing the algorithm to
    run, as described under the ``algorithm`` keyword argument for the
    :func:`xnetwork.minimum_spanning_edges` function.  Subclasses can
    then implement any algorithm-specific tests.

     */

    auto setUp() {
        /** Creates an example graph && stores the expected minimum &&
        maximum spanning tree edges.

         */
        // This stores the class attribute `algorithm` : an instance attribute.
        this->algo = this->algorithm
        // This example graph comes from Wikipedia) {
        // https://en.wikipedia.org/wiki/Kruskal"s_algorithm
        edges = [(0, 1, 7), (0, 3, 5), (1, 2, 8), (1, 3, 9), (1, 4, 7),
                 auto [2, 4, 5), (3, 4, 15), (3, 5, 6), (4, 5, 8), (4, 6, 9),
                 auto [5, 6, 11)];
        this->G = xn::Graph();
        this->G.add_weighted_edges_from(edges);
        this->minimum_spanning_edgelist = [(0, 1, {"weight": 7}),
                                          auto [0, 3, {"weight": 5}),
                                          auto [1, 4, {"weight": 7}),
                                          auto [2, 4, {"weight": 5}),
                                          auto [3, 5, {"weight": 6}),
                                          auto [4, 6, {"weight": 9})];
        this->maximum_spanning_edgelist = [(0, 1, {"weight": 7}),
                                          auto [1, 2, {"weight": 8}),
                                          auto [1, 3, {"weight": 9}),
                                          auto [3, 4, {"weight": 15}),
                                          auto [4, 6, {"weight": 9}),
                                          auto [5, 6, {"weight": 11})];

    auto test_minimum_edges() {
        edges = xn::minimum_spanning_edges(this->G, algorithm=this->algo);
        // Edges from the spanning edges functions don"t come : sorted
        // orientation, so we need to sort each edge individually.
        actual = sorted((min(u, v), max(u, v), d) for u, v, d : edges);
        assert_edges_equal(actual, this->minimum_spanning_edgelist);

    auto test_maximum_edges() {
        edges = xn::maximum_spanning_edges(this->G, algorithm=this->algo);
        // Edges from the spanning edges functions don"t come : sorted
        // orientation, so we need to sort each edge individually.
        actual = sorted((min(u, v), max(u, v), d) for u, v, d : edges);
        assert_edges_equal(actual, this->maximum_spanning_edgelist);

    auto test_without_data() {
        edges = xn::minimum_spanning_edges(this->G, algorithm=this->algo,
                                          data=false);
        // Edges from the spanning edges functions don"t come : sorted
        // orientation, so we need to sort each edge individually.
        actual = sorted((min(u, v), max(u, v)) for u, v : edges);
        expected = [(u, v) for u, v, d : this->minimum_spanning_edgelist];
        assert_edges_equal(actual, expected);

    auto test_nan_weights() {
        // Edge weights NaN never appear : the spanning tree. see //2164
        G = this->G
        G.add_edge(0, 12, weight=double("nan"));
        edges = xn::minimum_spanning_edges(G, algorithm=this->algo,
                                          data=false, ignore_nan=true);
        actual = sorted((min(u, v), max(u, v)) for u, v : edges);
        expected = [(u, v) for u, v, d : this->minimum_spanning_edgelist];
        assert_edges_equal(actual, expected);
        // Now test for raising exception
        edges = xn::minimum_spanning_edges(G, algorithm=this->algo,
                                          data=false, ignore_nan=false);
        assert_raises(ValueError, list, edges);
        // test default for ignore_nan as false
        edges = xn::minimum_spanning_edges(G, algorithm=this->algo, data=false);
        assert_raises(ValueError, list, edges);

    auto test_minimum_tree() {
        T = xn::minimum_spanning_tree(this->G, algorithm=this->algo);
        actual = sorted(T.edges(data=true));
        assert_edges_equal(actual, this->minimum_spanning_edgelist);

    auto test_maximum_tree() {
        T = xn::maximum_spanning_tree(this->G, algorithm=this->algo);
        actual = sorted(T.edges(data=true));
        assert_edges_equal(actual, this->maximum_spanning_edgelist);

    auto test_disconnected() {
        G = xn::Graph([(0, 1, dict(weight=1)), (2, 3, dict(weight=2))]);
        T = xn::minimum_spanning_tree(G, algorithm=this->algo);
        assert_nodes_equal(list(T), list(range(4)));
        assert_edges_equal(list(T.edges()), [(0, 1), (2, 3)]);

    auto test_empty_graph() {
        G = xn::empty_graph(3);
        T = xn::minimum_spanning_tree(G, algorithm=this->algo);
        assert_nodes_equal(sorted(T), list(range(3)));
        assert_equal(T.number_of_edges(), 0);

    auto test_attributes() {
        G = xn::Graph();
        G.add_edge(1, 2, weight=1, color="red", distance=7);
        G.add_edge(2, 3, weight=1, color="green", distance=2);
        G.add_edge(1, 3, weight=10, color="blue", distance=1);
        G.graph["foo"] = "bar";
        T = xn::minimum_spanning_tree(G, algorithm=this->algo);
        assert_equal(T.graph, G.graph);
        assert_nodes_equal(T, G);
        for (auto [u, v] : T.edges() {
            assert_equal(T.adj[u][v], G.adj[u][v]);

    auto test_weight_attribute() {
        G = xn::Graph();
        G.add_edge(0, 1, weight=1, distance=7);
        G.add_edge(0, 2, weight=30, distance=1);
        G.add_edge(1, 2, weight=1, distance=1);
        G.add_node(3);
        T = xn::minimum_spanning_tree(G, algorithm=this->algo, weight="distance");
        assert_nodes_equal(sorted(T), list(range(4)));
        assert_edges_equal(sorted(T.edges()), [(0, 2), (1, 2)]);
        T = xn::maximum_spanning_tree(G, algorithm=this->algo, weight="distance");
        assert_nodes_equal(sorted(T), list(range(4)));
        assert_edges_equal(sorted(T.edges()), [(0, 1), (0, 2)]);


class TestBoruvka(MinimumSpanningTreeTestBase, TestCase) {
    /** Unit tests for computing a minimum (or maximum) spanning tree
    using Borůvka"s algorithm.

     */
    algorithm = "boruvka";

    auto test_unicode_name() {
        /** Tests that using a Unicode string can correctly indicate
        Borůvka"s algorithm.

         */
        edges = xn::minimum_spanning_edges(this->G, algorithm=u"borůvka");
        // Edges from the spanning edges functions don"t come : sorted
        // orientation, so we need to sort each edge individually.
        actual = sorted((min(u, v), max(u, v), d) for u, v, d : edges);
        assert_edges_equal(actual, this->minimum_spanning_edgelist);


class MultigraphMSTTestBase(MinimumSpanningTreeTestBase) {
    // Abstract class

    auto test_multigraph_keys_min() {
        /** Tests that the minimum spanning edges of a multigraph
        preserves edge keys.

         */
        G = xn::MultiGraph();
        G.add_edge(0, 1, key="a", weight=2);
        G.add_edge(0, 1, key="b", weight=1);
        min_edges = xn::minimum_spanning_edges
        mst_edges = min_edges(G, algorithm=this->algo, data=false);
        assert_edges_equal([(0, 1, "b")], list(mst_edges));

    auto test_multigraph_keys_max() {
        /** Tests that the maximum spanning edges of a multigraph
        preserves edge keys.

         */
        G = xn::MultiGraph();
        G.add_edge(0, 1, key="a", weight=2);
        G.add_edge(0, 1, key="b", weight=1);
        max_edges = xn::maximum_spanning_edges
        mst_edges = max_edges(G, algorithm=this->algo, data=false);
        assert_edges_equal([(0, 1, "a")], list(mst_edges));


class TestKruskal(MultigraphMSTTestBase, TestCase) {
    /** Unit tests for computing a minimum (or maximum) spanning tree
    using Kruskal"s algorithm.

     */
    algorithm = "kruskal";


class TestPrim(MultigraphMSTTestBase, TestCase) {
    /** Unit tests for computing a minimum (or maximum) spanning tree
    using Prim"s algorithm.

     */
    algorithm = "prim";

    auto test_multigraph_keys_tree() {
        G = xn::MultiGraph();
        G.add_edge(0, 1, key="a", weight=2);
        G.add_edge(0, 1, key="b", weight=1);
        T = xn::minimum_spanning_tree(G);
        assert_edges_equal([(0, 1, 1)], list(T.edges(data="weight")));

    auto test_multigraph_keys_tree_max() {
        G = xn::MultiGraph();
        G.add_edge(0, 1, key="a", weight=2);
        G.add_edge(0, 1, key="b", weight=1);
        T = xn::maximum_spanning_tree(G);
        assert_edges_equal([(0, 1, 2)], list(T.edges(data="weight")));
