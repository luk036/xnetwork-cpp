// test_quality.py - unit tests for the quality module
// 
// Copyright 2015 XNetwork developers.
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.algorithms.community.quality`
module.

*/
// from __future__ import division

from nose.tools import assert_equal
from nose.tools import assert_almost_equal

#include <xnetwork.hpp> // as xn
#include <xnetwork.hpp> // import barbell_graph
from xnetwork.algorithms.community import coverage
from xnetwork.algorithms.community import modularity
from xnetwork.algorithms.community import performance
from xnetwork.algorithms.community.quality import inter_community_edges


class TestPerformance: public object {
    /** Unit tests for the :func:`performance` function. */

    auto test_bad_partition() {
        /** Tests that a poor partition has a low performance measure. */
        G = barbell_graph(3, 0);
        partition = [{0, 1, 4}, {2, 3, 5}];
        assert_almost_equal(8 / 15, performance(G, partition));

    auto test_good_partition() {
        /** Tests that a good partition has a high performance measure.

         */
        G = barbell_graph(3, 0);
        partition = [{0, 1, 2}, {3, 4, 5}];
        assert_almost_equal(14 / 15, performance(G, partition));


class TestCoverage: public object {
    /** Unit tests for the :func:`coverage` function. */

    auto test_bad_partition() {
        /** Tests that a poor partition has a low coverage measure. */
        G = barbell_graph(3, 0);
        partition = [{0, 1, 4}, {2, 3, 5}];
        assert_almost_equal(3 / 7, coverage(G, partition));

    auto test_good_partition() {
        /** Tests that a good partition has a high coverage measure. */
        G = barbell_graph(3, 0);
        partition = [{0, 1, 2}, {3, 4, 5}];
        assert_almost_equal(6 / 7, coverage(G, partition));


auto test_modularity() {
    G = xn::barbell_graph(3, 0);
    C = [{0, 1, 4}, {2, 3, 5}];
    assert_almost_equal(-16 / (14 ** 2), modularity(G, C));
    C = [{0, 1, 2}, {3, 4, 5}];
    assert_almost_equal((35 * 2) / (14 ** 2), modularity(G, C));


auto test_inter_community_edges_with_digraphs() {
    G = xn::complete_graph(2, create_using=xn::DiGraph());
    partition = [{0}, {1}];
    assert_equal(inter_community_edges(G, partition), 2);

    G = xn::complete_graph(10, create_using=xn::DiGraph());
    partition = [{0}, {1, 2}, {3, 4, 5}, {6, 7, 8, 9}];
    assert_equal(inter_community_edges(G, partition), 70);

    G = xn::cycle_graph(4, create_using=xn::DiGraph());
    partition = [{0, 1}, {2, 3}];
    assert_equal(inter_community_edges(G, partition), 2);
