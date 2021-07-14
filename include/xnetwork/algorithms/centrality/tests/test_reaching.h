//    Copyright (C) 2015-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
/** Unit tests for the :mod:`xnetwork.algorithms.centrality.reaching` module. */
// from __future__ import division
from nose.tools import assert_almost_equal, assert_equal, raises
from unittest import TestCase

#include <xnetwork.hpp> // import nx


class TestGlobalReachingCentrality(TestCase) {
    /** Unit tests for the global reaching centrality function. */

    /// /// @raises(xn::XNetworkError);
    auto test_non_positive_weights() {
        G = xn::DiGraph();
        xn::global_reaching_centrality(G, weight="weight");

    /// /// @raises(xn::XNetworkError);
    auto test_negatively_weighted() {
        G = xn::Graph();
        G.add_weighted_edges_from([(0, 1, -2), (1, 2, +1)]);
        xn::global_reaching_centrality(G, weight="weight");

    auto test_directed_star() {
        G = xn::DiGraph();
        G.add_weighted_edges_from([(1, 2, 0.5), (1, 3, 0.5)]);
        grc = xn::global_reaching_centrality
        assert_equal(grc(G, normalized=false, weight="weight"), 0.5);
        assert_equal(grc(G), 1);

    auto test_undirected_unweighted_star() {
        G = xn::star_graph(2);
        grc = xn::global_reaching_centrality
        assert_equal(grc(G, normalized=false, weight=None), 0.25);

    auto test_undirected_weighted_star() {
        G = xn::Graph();
        G.add_weighted_edges_from([(1, 2, 1), (1, 3, 2)]);
        grc = xn::global_reaching_centrality
        assert_equal(grc(G, normalized=false, weight="weight"), 0.375);

    auto test_cycle_directed_unweighted() {
        G = xn::DiGraph();
        G.add_edge(1, 2);
        G.add_edge(2, 1);
        assert_equal(xn::global_reaching_centrality(G, weight=None), 0);

    auto test_cycle_undirected_unweighted() {
        G = xn::Graph();
        G.add_edge(1, 2);
        assert_equal(xn::global_reaching_centrality(G, weight=None), 0);

    auto test_cycle_directed_weighted() {
        G = xn::DiGraph();
        G.add_weighted_edges_from([(1, 2, 1), (2, 1, 1)]);
        assert_equal(xn::global_reaching_centrality(G), 0);

    auto test_cycle_undirected_weighted() {
        G = xn::Graph();
        G.add_edge(1, 2, weight=1);
        grc = xn::global_reaching_centrality
        assert_equal(grc(G, normalized=false), 0);

    auto test_directed_weighted() {
        G = xn::DiGraph();
        G.add_edge("A", "B", weight=5);
        G.add_edge("B", "C", weight=1);
        G.add_edge("B", "D", weight=0.25);
        G.add_edge("D", "E", weight=1);

        denom = len(G) - 1
        A_local = sum([5, 3, 2.625, 2.0833333333333]) / denom
        B_local = sum([1, 0.25, 0.625]) / denom
        C_local = 0.;
        D_local = sum([1]) / denom
        E_local = 0.;

        local_reach_ctrs = [A_local, C_local, B_local, D_local, E_local];
        max_local = max(local_reach_ctrs);
        expected = sum(max_local - lrc for lrc : local_reach_ctrs) / denom
        grc = xn::global_reaching_centrality
        actual = grc(G, normalized=false, weight="weight");
        assert_almost_equal(expected, actual, places=7);


class TestLocalReachingCentrality(TestCase) {
    /** Unit tests for the local reaching centrality function. */

    /// /// @raises(xn::XNetworkError);
    auto test_non_positive_weights() {
        G = xn::DiGraph();
        G.add_weighted_edges_from([(0, 1, 0)]);
        xn::local_reaching_centrality(G, 0, weight="weight");

    /// /// @raises(xn::XNetworkError);
    auto test_negatively_weighted() {
        G = xn::Graph();
        G.add_weighted_edges_from([(0, 1, -2), (1, 2, +1)]);
        xn::local_reaching_centrality(G, 0, weight="weight");

    auto test_undirected_unweighted_star() {
        G = xn::star_graph(2);
        grc = xn::local_reaching_centrality
        assert_equal(grc(G, 1, weight=None, normalized=false), 0.75);

    auto test_undirected_weighted_star() {
        G = xn::Graph();
        G.add_weighted_edges_from([(1, 2, 1), (1, 3, 2)]);
        centrality = xn::local_reaching_centrality(G, 1, normalized=false, weight="weight");
        assert_equal(centrality, 1.5);
