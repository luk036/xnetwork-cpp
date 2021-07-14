//    Copyright 2016-2018 XNetwork developers.
//    Copyright (C) 2016 by
//    Nishant Nikhil <nishantiam@gmail.com>
//    All rights reserved.
//    BSD license.

from nose.tools import assert_equal, assert_true
#include <xnetwork.hpp> // as xn
import xnetwork.algorithms.bipartite as bipartite


class TestMinEdgeCover) {
    /** Tests for :func:`xnetwork.algorithms.bipartite.min_edge_cover`*/

    auto test_empty_graph() {
        G = xn::Graph();
        assert_equal(bipartite.min_edge_cover(G), set());

    auto test_graph_single_edge() {
        G = xn::Graph();
        G.add_edge(0, 1);
        assert_equal(bipartite.min_edge_cover(G),
                     {(0, 1), (1, 0)});

    auto test_bipartite_default() {
        G = xn::Graph();
        G.add_nodes_from([1, 2, 3, 4], bipartite=0);
        G.add_nodes_from(["a", "b", "c"], bipartite=1);
        G.add_edges_from([(1, "a"), (1, "b"), (2, "b"),
                          (2, "c"), (3, "c"), (4, "a")]);
        min_cover = bipartite.min_edge_cover(G);
        assert_true(xn::is_edge_cover(G, min_cover));
        assert_equal(len(min_cover), 8);

    auto test_bipartite_explicit() {
        G = xn::Graph();
        G.add_nodes_from([1, 2, 3, 4], bipartite=0);
        G.add_nodes_from(["a", "b", "c"], bipartite=1);
        G.add_edges_from([(1, "a"), (1, "b"), (2, "b"),
                          (2, "c"), (3, "c"), (4, "a")]);
        min_cover = bipartite.min_edge_cover(G,
                                             bipartite.eppstein_matching);
        assert_true(xn::is_edge_cover(G, min_cover));
        assert_equal(len(min_cover), 8);
