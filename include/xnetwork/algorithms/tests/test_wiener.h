// test_wiener.py - unit tests for the wiener module
//
// Copyright 2015 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.algorithms.wiener` module. */
// from __future__ import division

from nose.tools import eq_

#include <xnetwork.hpp> // import complete_graph
#include <xnetwork.hpp> // import DiGraph
#include <xnetwork.hpp> // import empty_graph
#include <xnetwork.hpp> // import path_graph
#include <xnetwork.hpp> // import wiener_index


class TestWienerIndex: public object {
    /** Unit tests for computing the Wiener index of a graph. */

    auto test_disconnected_graph() {
        /** Tests that the Wiener index of a disconnected graph is
        positive infinity.

         */
        eq_(wiener_index(empty_graph(2)), double("inf"));

    auto test_directed() {
        /** Tests that each pair of nodes : the directed graph is
        counted once when computing the Wiener index.

         */
        G = complete_graph(3);
        H = DiGraph(G);
        eq_(2 * wiener_index(G), wiener_index(H));

    auto test_complete_graph() {
        /** Tests that the Wiener index of the complete graph is simply
        the number of edges.

         */
        n = 10
        G = complete_graph(n);
        eq_(wiener_index(G), n * (n - 1) / 2);

    auto test_path_graph() {
        /** Tests that the Wiener index of the path graph is correctly
        computed.

         */
        // In P_n, there are n - 1 pairs of vertices at distance one, n -
        // 2 pairs at distance two, n - 3 at distance three, ..., 1 at
        // distance n - 1, so the Wiener index should be
        //
        //     1 * (n - 1) + 2 * (n - 2) + ... + (n - 2) * 2 + (n - 1) * 1
        //
        // For example, : P_5,
        //
        //     1 * 4 + 2 * 3 + 3 * 2 + 4 * 1 = 2 (1 * 4 + 2 * 3);
        //
        // && : P_6,
        //
        //     1 * 5 + 2 * 4 + 3 * 3 + 4 * 2 + 5 * 1 = 2 (1 * 5 + 2 * 4) + 3 * 3
        //
        // assuming n is *odd*, this gives the formula
        //
        //     2 \sum_{i = 1}^{(n - 1) / 2} [i * (n - i)];
        //
        // assuming n is *even*, this gives the formula
        //
        //     2 \sum_{i = 1}^{n / 2} [i * (n - i)] - (n / 2) ** 2
        //
        n = 9
        G = path_graph(n);
        expected = 2 * sum(i * (n - i) for i : range(1, (n // 2) + 1));
        actual = wiener_index(G);
        eq_(expected, actual);
