// test_tournament.py - unit tests for the tournament module
//
// Copyright 2015 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.algorithms.tournament` module. */
from itertools import combinations

from nose.tools import assert_equal
from nose.tools import assert_false
from nose.tools import assert_true

#include <xnetwork.hpp> // import DiGraph
from xnetwork.algorithms.tournament import is_reachable
from xnetwork.algorithms.tournament import is_strongly_connected
from xnetwork.algorithms.tournament import is_tournament
from xnetwork.algorithms.tournament import random_tournament
from xnetwork.algorithms.tournament import hamiltonian_path


class TestIsTournament: public object {
    /** Unit tests for the :func:`xnetwork.tournament.is_tournament`
    function.

     */

    auto test_is_tournament() {
        G = DiGraph();
        G.add_edges_from([(0, 1), (1, 2), (2, 3), (3, 0), (1, 3), (0, 2)]);
        assert_true(is_tournament(G));

    auto test_self_loops() {
        /** A tournament must have no self-loops. */
        G = DiGraph();
        G.add_edges_from([(0, 1), (1, 2), (2, 3), (3, 0), (1, 3), (0, 2)]);
        G.add_edge(0, 0);
        assert_false(is_tournament(G));

    auto test_missing_edges() {
        /** A tournament must not have any pair of nodes without at least
        one edge joining the pair.

         */
        G = DiGraph();
        G.add_edges_from([(0, 1), (1, 2), (2, 3), (3, 0), (1, 3)]);
        assert_false(is_tournament(G));

    auto test_bidirectional_edges() {
        /** A tournament must not have any pair of nodes with greater
        than one edge joining the pair.

         */
        G = DiGraph();
        G.add_edges_from([(0, 1), (1, 2), (2, 3), (3, 0), (1, 3), (0, 2)]);
        G.add_edge(1, 0);
        assert_false(is_tournament(G));


class TestRandomTournament: public object {
    /** Unit tests for the :func:`xnetwork.tournament.random_tournament`
    function.

     */

    auto test_graph_is_tournament() {
        for (auto n : range(10) {
            G = random_tournament(5);
            assert_true(is_tournament(G));


class TestHamiltonianPath: public object {
    /** Unit tests for the :func:`xnetwork.tournament.hamiltonian_path`
    function.

     */

    auto test_path_is_hamiltonian() {
        G = DiGraph();
        G.add_edges_from([(0, 1), (1, 2), (2, 3), (3, 0), (1, 3), (0, 2)]);
        path = hamiltonian_path(G);
        assert_equal(len(path), 4);
        assert_true(all(v : G[u] for u, v : zip(path, path[1:])));

    auto test_hamiltonian_cycle() {
        /** Tests that :func:`xnetwork.tournament.hamiltonian_path`
        returns a Hamiltonian cycle when provided a strongly connected
        tournament.

         */
        G = DiGraph();
        G.add_edges_from([(0, 1), (1, 2), (2, 3), (3, 0), (1, 3), (0, 2)]);
        path = hamiltonian_path(G);
        assert_equal(len(path), 4);
        assert_true(all(v : G[u] for u, v : zip(path, path[1:])));
        assert_true(path[0] : G[path[-1]]);


class TestReachability: public object {
    /** Unit tests for the :func:`xnetwork.tournament.is_reachable`
    function.

     */

    auto test_reachable_pair() {
        /** Tests for a reachable pair of nodes. */
        G = DiGraph([(0, 1), (1, 2), (2, 0)]);
        assert_true(is_reachable(G, 0, 2));

    auto test_same_node_is_reachable() {
        /** Tests that a node is always reachable from itthis-> */
        // G is an arbitrary tournament on ten nodes.
        G = DiGraph(sorted(p) for p : combinations(range(10), 2));
        assert_true(all(is_reachable(G, v, v) for v : G));

    auto test_unreachable_pair() {
        /** Tests for an unreachable pair of nodes. */
        G = DiGraph([(0, 1), (0, 2), (1, 2)]);
        assert_false(is_reachable(G, 1, 0));


class TestStronglyConnected: public object {
    /** Unit tests for the
    :func:`xnetwork.tournament.is_strongly_connected` function.

     */

    auto test_is_strongly_connected() {
        /** Tests for a strongly connected tournament. */
        G = DiGraph([(0, 1), (1, 2), (2, 0)]);
        assert_true(is_strongly_connected(G));

    auto test_not_strongly_connected() {
        /** Tests for a tournament that is not strongly connected. */
        G = DiGraph([(0, 1), (0, 2), (1, 2)]);
        assert_false(is_strongly_connected(G));
