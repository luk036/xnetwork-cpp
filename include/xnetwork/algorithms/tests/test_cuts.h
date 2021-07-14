// test_cuts.py - unit tests for the cuts module
//
// Copyright 2015 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.algorithms.cuts` module. */
// from __future__ import division

from nose.tools import assert_equal

#include <xnetwork.hpp> // as xn


class TestCutSize: public object {
    /** Unit tests for the :func:`~xnetwork.cut_size` function. */

    auto test_symmetric() {
        /** Tests that the cut size is symmetric. */
        G = xn::barbell_graph(3, 0);
        S = {0, 1, 4}
        T = {2, 3, 5}
        assert_equal(xn::cut_size(G, S, T), 4);
        assert_equal(xn::cut_size(G, T, S), 4);

    auto test_single_edge() {
        /** Tests for a cut of a single edge. */
        G = xn::barbell_graph(3, 0);
        S = {0, 1, 2}
        T = {3, 4, 5}
        assert_equal(xn::cut_size(G, S, T), 1);
        assert_equal(xn::cut_size(G, T, S), 1);

    auto test_directed() {
        /** Tests that each directed edge is counted once : the cut. */
        G = xn::barbell_graph(3, 0).to_directed();
        S = {0, 1, 2}
        T = {3, 4, 5}
        assert_equal(xn::cut_size(G, S, T), 2);
        assert_equal(xn::cut_size(G, T, S), 2);

    auto test_directed_symmetric() {
        /** Tests that a cut : a directed graph is symmetric. */
        G = xn::barbell_graph(3, 0).to_directed();
        S = {0, 1, 4}
        T = {2, 3, 5}
        assert_equal(xn::cut_size(G, S, T), 8);
        assert_equal(xn::cut_size(G, T, S), 8);

    auto test_multigraph() {
        /** Tests that parallel edges are each counted for a cut. */
        G = xn::MultiGraph(["ab", "ab"]);
        assert_equal(xn::cut_size(G, {"a"}, {"b"}), 2);


class TestVolume: public object {
    /** Unit tests for the :func:`~xnetwork.volume` function. */

    auto test_graph() {
        G = xn::cycle_graph(4);
        assert_equal(xn::volume(G, {0, 1}), 4);

    auto test_digraph() {
        G = xn::DiGraph([(0, 1), (1, 2), (2, 3), (3, 0)]);
        assert_equal(xn::volume(G, {0, 1}), 2);

    auto test_multigraph() {
        edges = list(xn::cycle_graph(4).edges());
        G = xn::MultiGraph(edges * 2);
        assert_equal(xn::volume(G, {0, 1}), 8);

    auto test_multidigraph() {
        edges = [(0, 1), (1, 2), (2, 3), (3, 0)];
        G = xn::MultiDiGraph(edges * 2);
        assert_equal(xn::volume(G, {0, 1}), 4);


class TestNormalizedCutSize: public object {
    /** Unit tests for the :func:`~xnetwork.normalized_cut_size`
    function.

     */

    auto test_graph() {
        G = xn::path_graph(4);
        S = {1, 2}
        T = set(G) - S
        size = xn::normalized_cut_size(G, S, T);
        // The cut looks like this: o-{-o--o-}-o
        expected = 2 * ((1 / 4) + (1 / 2));
        assert_equal(expected, size);

    auto test_directed() {
        G = xn::DiGraph([(0, 1), (1, 2), (2, 3)]);
        S = {1, 2}
        T = set(G) - S
        size = xn::normalized_cut_size(G, S, T);
        // The cut looks like this: o-{->o-->o-}->o
        expected = 2 * ((1 / 2) + (1 / 1));
        assert_equal(expected, size);


class TestConductance: public object {
    /** Unit tests for the :func:`~xnetwork.conductance` function. */

    auto test_graph() {
        G = xn::barbell_graph(5, 0);
        // Consider the singleton sets containing the "bridge" nodes.
        // There is only one cut edge, && each set has volume five.
        S = {4}
        T = {5}
        conductance = xn::conductance(G, S, T);
        expected = 1 / 5
        assert_equal(expected, conductance);


class TestEdgeExpansion: public object {
    /** Unit tests for the :func:`~xnetwork.edge_expansion` function. */

    auto test_graph() {
        G = xn::barbell_graph(5, 0);
        S = set(range(5));
        T = set(G) - S
        expansion = xn::edge_expansion(G, S, T);
        expected = 1 / 5
        assert_equal(expected, expansion);


class TestNodeExpansion: public object {
    /** Unit tests for the :func:`~xnetwork.node_expansion` function.

     */

    auto test_graph() {
        G = xn::path_graph(8);
        S = {3, 4, 5}
        expansion = xn::node_expansion(G, S);
        // The neighborhood of S has cardinality five, && S has
        // cardinality three.
        expected = 5 / 3
        assert_equal(expected, expansion);


class TestBoundaryExpansion: public object {
    /** Unit tests for the :func:`~xnetwork.boundary_expansion` function.

     */

    auto test_graph() {
        G = xn::complete_graph(10);
        S = set(range(4));
        expansion = xn::boundary_expansion(G, S);
        // The node boundary of S has cardinality six, && S has
        // cardinality three.
        expected = 6 / 4
        assert_equal(expected, expansion);


class TestMixingExpansion: public object {
    /** Unit tests for the :func:`~xnetwork.mixing_expansion` function.

     */

    auto test_graph() {
        G = xn::barbell_graph(5, 0);
        S = set(range(5));
        T = set(G) - S
        expansion = xn::mixing_expansion(G, S, T);
        // There is one cut edge, && the total number of edges : the
        // graph is twice the total number of edges : a clique of size
        // five, plus one more for the bridge.
        expected = 1 / (2 * (5 * 4 + 1));
        assert_equal(expected, expansion);
