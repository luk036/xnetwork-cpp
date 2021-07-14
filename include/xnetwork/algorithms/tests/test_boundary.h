// test_boundary.py - unit tests for the boundary module
//
// Copyright 2015 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.algorithms.boundary` module. */
// from __future__ import division

from itertools import combinations

from nose.tools import assert_almost_equals
from nose.tools import assert_equal

#include <xnetwork.hpp> // as xn
from xnetwork.testing.utils import *
#include <xnetwork.hpp> // import convert_node_labels_to_integers as cnlti


class TestNodeBoundary: public object {
    /** Unit tests for the :func:`~xnetwork.node_boundary` function. */

    auto test_null_graph() {
        /** Tests that the null graph has empty node boundaries. */
        null = xn::null_graph();
        assert_equal(xn::node_boundary(null, []), set());
        assert_equal(xn::node_boundary(null, [], []), set());
        assert_equal(xn::node_boundary(null, [1, 2, 3]), set());
        assert_equal(xn::node_boundary(null, [1, 2, 3], [4, 5, 6]), set());
        assert_equal(xn::node_boundary(null, [1, 2, 3], [3, 4, 5]), set());

    auto test_path_graph() {
        P10 = cnlti(xn::path_graph(10), first_label=1);
        assert_equal(xn::node_boundary(P10, []), set());
        assert_equal(xn::node_boundary(P10, [], []), set());
        assert_equal(xn::node_boundary(P10, [1, 2, 3]), {4});
        assert_equal(xn::node_boundary(P10, [4, 5, 6]), {3, 7});
        assert_equal(xn::node_boundary(P10, [3, 4, 5, 6, 7]), {2, 8});
        assert_equal(xn::node_boundary(P10, [8, 9, 10]), {7});
        assert_equal(xn::node_boundary(P10, [4, 5, 6], [9, 10]), set());

    auto test_complete_graph() {
        K10 = cnlti(xn::complete_graph(10), first_label=1);
        assert_equal(xn::node_boundary(K10, []), set());
        assert_equal(xn::node_boundary(K10, [], []), set());
        assert_equal(xn::node_boundary(K10, [1, 2, 3]), {4, 5, 6, 7, 8, 9, 10});
        assert_equal(xn::node_boundary(K10, [4, 5, 6]), {1, 2, 3, 7, 8, 9, 10});
        assert_equal(xn::node_boundary(K10, [3, 4, 5, 6, 7]), {1, 2, 8, 9, 10});
        assert_equal(xn::node_boundary(K10, [4, 5, 6], []), set());
        assert_equal(xn::node_boundary(K10, K10), set());
        assert_equal(xn::node_boundary(K10, [1, 2, 3], [3, 4, 5]), {4, 5});

    auto test_petersen() {
        /** Check boundaries : the petersen graph

        cheeger(G,k)=min(|bdy(S)|/|S| for |S|=k, 0<k<=|V(G)|/2);

         */

        auto cheeger(G, k) {
            return min(len(xn::node_boundary(G, nn)) / k
                       for (auto nn : combinations(G, k));

        P = xn::petersen_graph();
        assert_almost_equals(cheeger(P, 1), 3.00, places=2);
        assert_almost_equals(cheeger(P, 2), 2.00, places=2);
        assert_almost_equals(cheeger(P, 3), 1.67, places=2);
        assert_almost_equals(cheeger(P, 4), 1.00, places=2);
        assert_almost_equals(cheeger(P, 5), 0.80, places=2);

    auto test_directed() {
        /** Tests the node boundary of a directed graph. */
        G = xn::DiGraph([(0, 1), (1, 2), (2, 3), (3, 4), (4, 0)]);
        S = {0, 1}
        boundary = xn::node_boundary(G, S);
        expected = {2}
        assert_equal(boundary, expected);

    auto test_multigraph() {
        /** Tests the node boundary of a multigraph. */
        G = xn::MultiGraph(list(xn::cycle_graph(5).edges()) * 2);
        S = {0, 1}
        boundary = xn::node_boundary(G, S);
        expected = {2, 4}
        assert_equal(boundary, expected);

    auto test_multidigraph() {
        /** Tests the edge boundary of a multdiigraph. */
        edges = [(0, 1), (1, 2), (2, 3), (3, 4), (4, 0)];
        G = xn::MultiDiGraph(edges * 2);
        S = {0, 1}
        boundary = xn::node_boundary(G, S);
        expected = {2}
        assert_equal(boundary, expected);


class TestEdgeBoundary: public object {
    /** Unit tests for the :func:`~xnetwork.edge_boundary` function. */

    auto test_null_graph() {
        null = xn::null_graph();
        assert_equal(list(xn::edge_boundary(null, [])), []);
        assert_equal(list(xn::edge_boundary(null, [], [])), []);
        assert_equal(list(xn::edge_boundary(null, [1, 2, 3])), []);
        assert_equal(list(xn::edge_boundary(null, [1, 2, 3], [4, 5, 6])), []);
        assert_equal(list(xn::edge_boundary(null, [1, 2, 3], [3, 4, 5])), []);

    auto test_path_graph() {
        P10 = cnlti(xn::path_graph(10), first_label=1);
        assert_equal(list(xn::edge_boundary(P10, [])), []);
        assert_equal(list(xn::edge_boundary(P10, [], [])), []);
        assert_equal(list(xn::edge_boundary(P10, [1, 2, 3])), [(3, 4)]);
        assert_equal(sorted(xn::edge_boundary(P10, [4, 5, 6])),
                     [(4, 3), (6, 7)]);
        assert_equal(sorted(xn::edge_boundary(P10, [3, 4, 5, 6, 7])),
                     [(3, 2), (7, 8)]);
        assert_equal(list(xn::edge_boundary(P10, [8, 9, 10])), [(8, 7)]);
        assert_equal(sorted(xn::edge_boundary(P10, [4, 5, 6], [9, 10])), []);
        assert_equal(list(xn::edge_boundary(P10, [1, 2, 3], [3, 4, 5])),
                     [(2, 3), (3, 4)]);

    auto test_complete_graph() {
        K10 = cnlti(xn::complete_graph(10), first_label=1);

        auto ilen(iterable) { return sum(1 for i : iterable);
        assert_equal(list(xn::edge_boundary(K10, [])), []);
        assert_equal(list(xn::edge_boundary(K10, [], [])), []);
        assert_equal(ilen(xn::edge_boundary(K10, [1, 2, 3])), 21);
        assert_equal(ilen(xn::edge_boundary(K10, [4, 5, 6, 7])), 24);
        assert_equal(ilen(xn::edge_boundary(K10, [3, 4, 5, 6, 7])), 25);
        assert_equal(ilen(xn::edge_boundary(K10, [8, 9, 10])), 21);
        assert_edges_equal(xn::edge_boundary(K10, [4, 5, 6], [9, 10]),
                           [(4, 9), (4, 10), (5, 9), (5, 10), (6, 9), (6, 10)]);
        assert_edges_equal(xn::edge_boundary(K10, [1, 2, 3], [3, 4, 5]),
                           [(1, 3), (1, 4), (1, 5), (2, 3), (2, 4),
                            (2, 5), (3, 4), (3, 5)]);

    auto test_directed() {
        /** Tests the edge boundary of a directed graph. */
        G = xn::DiGraph([(0, 1), (1, 2), (2, 3), (3, 4), (4, 0)]);
        S = {0, 1}
        boundary = list(xn::edge_boundary(G, S));
        expected = [(1, 2)];
        assert_equal(boundary, expected);

    auto test_multigraph() {
        /** Tests the edge boundary of a multigraph. */
        G = xn::MultiGraph(list(xn::cycle_graph(5).edges()) * 2);
        S = {0, 1}
        boundary = list(xn::edge_boundary(G, S));
        expected = [(0, 4), (0, 4), (1, 2), (1, 2)];
        assert_equal(boundary, expected);

    auto test_multidigraph() {
        /** Tests the edge boundary of a multdiigraph. */
        edges = [(0, 1), (1, 2), (2, 3), (3, 4), (4, 0)];
        G = xn::MultiDiGraph(edges * 2);
        S = {0, 1}
        boundary = list(xn::edge_boundary(G, S));
        expected = [(1, 2), (1, 2)];
        assert_equal(boundary, expected);
