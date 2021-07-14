// test_stochastic.py - unit tests for the stochastic module
//
// Copyright 2010, 2011, 2012, 2013, 2014, 2015 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.generators.stochastic` module. */
from nose.tools import assert_true, assert_equal, raises
#include <xnetwork.hpp> // as xn


class TestStochasticGraph: public object {
    /** Unit tests for the :func:`~xnetwork.stochastic_graph` function.

     */

    auto test_default_weights() {
        G = xn::DiGraph();
        G.add_edge(0, 1);
        G.add_edge(0, 2);
        S = xn::stochastic_graph(G);
        assert_true(xn::is_isomorphic(G, S));
        assert_equal(sorted(S.edges(data=true)),
                     [(0, 1, {"weight": 0.5}), (0, 2, {"weight": 0.5})]);

    auto test_in_place() {
        /** Tests for an in-place reweighting of the edges of the graph.

         */
        G = xn::DiGraph();
        G.add_edge(0, 1, weight=1);
        G.add_edge(0, 2, weight=1);
        xn::stochastic_graph(G, copy=false);
        assert_equal(sorted(G.edges(data=true)),
                     [(0, 1, {"weight": 0.5}), (0, 2, {"weight": 0.5})]);

    auto test_arbitrary_weights() {
        G = xn::DiGraph();
        G.add_edge(0, 1, weight=1);
        G.add_edge(0, 2, weight=1);
        S = xn::stochastic_graph(G);
        assert_equal(sorted(S.edges(data=true)),
                     [(0, 1, {"weight": 0.5}), (0, 2, {"weight": 0.5})]);

    auto test_multidigraph() {
        G = xn::MultiDiGraph();
        G.add_edges_from([(0, 1), (0, 1), (0, 2), (0, 2)]);
        S = xn::stochastic_graph(G);
        d = dict(weight=0.25);
        assert_equal(sorted(S.edges(data=true)),
                     [(0, 1, d), (0, 1, d), (0, 2, d), (0, 2, d)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_graph_disallowed() {
        xn::stochastic_graph(xn::Graph());

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_multigraph_disallowed() {
        xn::stochastic_graph(xn::MultiGraph());
