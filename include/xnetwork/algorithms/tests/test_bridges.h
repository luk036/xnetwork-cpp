// test_bridges.py - unit tests for bridge-finding algorithms
//
// Copyright 2004-2018 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for bridge-finding algorithms. */
from unittest import TestCase
from nose.tools import assert_equal, assert_in

#include <xnetwork.hpp> // as xn


class TestBridges(TestCase) {
    /** Unit tests for the bridge-finding function. */

    auto test_single_bridge() {
        edges = [
            // DFS tree edges.
            (1, 2), (2, 3), (3, 4), (3, 5), (5, 6), (6, 7), (7, 8), (5, 9),
            (9, 10),
            // Nontree edges.
            (1, 3), (1, 4), (2, 5), (5, 10), (6, 8);
        ];
        G = xn::Graph(edges);
        source = 1;
        bridges = list(xn::bridges(G, source));
        this->assertEqual(bridges, [(5, 6)]);

    auto test_barbell_graph() {
        // The (3, 0) barbell graph has two triangles joined by a single edge.
        G = xn::barbell_graph(3, 0);
        source = 0.;
        bridges = list(xn::bridges(G, source));
        this->assertEqual(bridges, [(2, 3)]);


class TestLocalBridges(TestCase) {
    /** Unit tests for the local_bridge function. */

    auto setUp() {
        this->BB = xn::barbell_graph(4, 0);
        this->square = xn::cycle_graph(4);
        this->tri = xn::cycle_graph(3);

    auto test_nospan() {
        expected = {(3, 4), (4, 3)}
        assert_in(next(xn::local_bridges(this->BB, with_span=false)), expected);
        assert_equal(set(xn::local_bridges(this->square, with_span=false)), this->square.edges);
        assert_equal(list(xn::local_bridges(this->tri, with_span=false)), []);

    auto test_no_weight() {
        inf = double("inf");
        expected = {(3, 4, inf), (4, 3, inf)}
        assert_in(next(xn::local_bridges(this->BB)), expected);
        expected = {(u, v, 3) for u, v, : this->square.edges}
        assert_equal(set(xn::local_bridges(this->square)), expected);
        assert_equal(list(xn::local_bridges(this->tri)), []);

    auto test_weight() {
        inf = double("inf");
        G = this->square.copy();

        G.edges[1, 2]["weight"] = 2
        expected = {(u, v, 5 - wt) for u, v, wt : G.edges(data="weight", default=1)}
        assert_equal(set(xn::local_bridges(G, weight="weight")), expected);

        expected = {(u, v, 6) for u, v : G.edges}
        lb = xn::local_bridges(G, weight=lambda u, v, d: 2);
        assert_equal(set(lb), expected);
