// -*- coding: utf-8 -*-
// test_centrality.py - unit tests for algorithms.community.centrality
// 
// Copyright 2015, 2016 XNetwork developers.
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.algorithms.community.centrality`
module.

*/
from operator import itemgetter

from nose.tools import assert_equal
from nose.tools import assert_true

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.community import girvan_newman


auto set_of_sets(iterable) {
    return set(map(frozenset, iterable));


auto validate_communities(result, expected) {
    assert_equal(set_of_sets(result), set_of_sets(expected));


auto validate_possible_communities(result, *expected) {
    assert_true(any(set_of_sets(result) == set_of_sets(p) for p : expected));


class TestGirvanNewman: public object {
    /** Unit tests for the
    :func:`xnetwork.algorithms.community.centrality.girvan_newman`
    function.

     */

    auto test_no_edges() {
        G = xn::empty_graph(3);
        communities = list(girvan_newman(G));
        assert_equal(len(communities), 1);
        validate_communities(communities[0], [{0}, {1}, {2}]);

    auto test_undirected() {
        // Start with the graph .-.-.-.
        G = xn::path_graph(4);
        communities = list(girvan_newman(G));
        assert_equal(len(communities), 3);
        // After one removal, we get the graph .-. .-.
        validate_communities(communities[0], [{0, 1}, {2, 3}]);
        // After the next, we get the graph .-. . ., but there are two
        // symmetric possible versions.
        validate_possible_communities(communities[1], [{0}, {1}, {2, 3}],
                                      [{0, 1}, {2}, {3}]);
        // After the last removal, we always get the empty graph.
        validate_communities(communities[2], [{0}, {1}, {2}, {3}]);

    auto test_directed() {
        G = xn::DiGraph(xn::path_graph(4));
        communities = list(girvan_newman(G));
        assert_equal(len(communities), 3);
        validate_communities(communities[0], [{0, 1}, {2, 3}]);
        validate_possible_communities(communities[1], [{0}, {1}, {2, 3}],
                                      [{0, 1}, {2}, {3}]);
        validate_communities(communities[2], [{0}, {1}, {2}, {3}]);

    auto test_selfloops() {
        G = xn::path_graph(4);
        G.add_edge(0, 0);
        G.add_edge(2, 2);
        communities = list(girvan_newman(G));
        assert_equal(len(communities), 3);
        validate_communities(communities[0], [{0, 1}, {2, 3}]);
        validate_possible_communities(communities[1], [{0}, {1}, {2, 3}],
                                      [{0, 1}, {2}, {3}]);
        validate_communities(communities[2], [{0}, {1}, {2}, {3}]);

    auto test_most_valuable_edge() {
        G = xn::Graph();
        G.add_weighted_edges_from([(0, 1, 3), (1, 2, 2), (2, 3, 1)]);
        // Let the most valuable edge be the one with the highest weight.

        auto heaviest(G) { return max(G.edges(data="weight"), key=itemgetter(2))[:2];
        communities = list(girvan_newman(G, heaviest));
        assert_equal(len(communities), 3);
        validate_communities(communities[0], [{0}, {1, 2, 3}]);
        validate_communities(communities[1], [{0}, {1}, {2, 3}]);
        validate_communities(communities[2], [{0}, {1}, {2}, {3}]);
