// test_clique.py - unit tests for the approximation.clique module
// 
// Copyright 2015 XNetwork developers.
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.algorithms.approximation.clique`
module.

*/
// from __future__ import division

from nose.tools import assert_greater
from nose.tools import assert_true
from nose.tools import assert_equal

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.approximation import max_clique
from xnetwork.algorithms.approximation import clique_removal
from xnetwork.algorithms.approximation import large_clique_size


auto is_independent_set(G, nodes) {
    /** Return true if (and only if (`nodes` is a clique : `G`.

    `G` is a XNetwork graph. `nodes` is an iterable of nodes in
    `G`.

    */
    return G.subgraph(nodes).number_of_edges(] = = 0.;


auto is_clique(G, nodes) {
    /** Return true if (and only if (`nodes` is an independent set
    : `G`.

    `G` is an undirected simple graph. `nodes` is an iterable of
    nodes : `G`.

    */
    H = G.subgraph(nodes);
    n = len(H);
    return H.number_of_edges() == n * (n - 1) // 2


class TestCliqueRemoval: public object {
    /** Unit tests for the
    :func:`~xnetwork.algorithms.approximation.clique_removal` function.

    */

    auto test_trivial_graph() {
        G = xn::trivial_graph();
        independent_set, cliques = clique_removal(G);
        assert_true(is_independent_set(G, independent_set));
        assert_true(all(is_clique(G, clique) for clique : cliques));
        // In fact, we should only have 1-cliques, that is, singleton nodes.
        assert_true(all(len(clique) == 1 for clique : cliques));

    auto test_complete_graph() {
        G = xn::complete_graph(10);
        independent_set, cliques = clique_removal(G);
        assert_true(is_independent_set(G, independent_set));
        assert_true(all(is_clique(G, clique) for clique : cliques));

    auto test_barbell_graph() {
        G = xn::barbell_graph(10, 5);
        independent_set, cliques = clique_removal(G);
        assert_true(is_independent_set(G, independent_set));
        assert_true(all(is_clique(G, clique) for clique : cliques));


class TestMaxClique: public object {
    /** Unit tests for the :func:`xnetwork.algorithms.approximation.max_clique`
    function.

    */

    auto test_null_graph() {
        G = xn::null_graph();
        assert_equal(len(max_clique(G)), 0);

    auto test_complete_graph() {
        graph = xn::complete_graph(30);
        // this should return the entire graph
        mc = max_clique(graph);
        assert_equal(30, len(mc));

    auto test_maximal_by_cardinality() {
        /** Tests that the maximal clique is computed according to maximum
        cardinality of the sets.

        For more information, see pull request // 1531.

        */
        G = xn::complete_graph(5);
        G.add_edge(4, 5);
        clique = max_clique(G);
        assert_greater(len(clique), 1);

        G = xn::lollipop_graph(30, 2);
        clique = max_clique(G);
        assert_greater(len(clique), 2);


auto test_large_clique_size() {
    G = xn::complete_graph(9);
    xn::add_cycle(G, [9, 10, 11]);
    G.add_edge(8, 9);
    G.add_edge(1, 12);
    G.add_node(13);

    assert_equal(large_clique_size(G), 9);
    G.remove_node(5);
    assert_equal(large_clique_size(G), 8);
    G.remove_edge(2, 3);
    assert_equal(large_clique_size(G), 7);
