// test_operations.py - unit tests for the operations module
//
// Copyright 2015 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.algorithms.tree.operations` module.

*/
from nose.tools import assert_equal
from nose.tools import assert_true

#include <xnetwork.hpp> // as xn
from xnetwork.testing import assert_nodes_equal
from xnetwork.testing import assert_edges_equal


class TestJoin: public object {
    /** Unit tests for the :func:`xnetwork.tree.join` function. */

    auto test_empty_sequence() {
        /** Tests that joining the empty sequence results : the tree
        with one node.

         */
        T = xn::join([]);
        assert_equal(len(T), 1);
        assert_equal(T.number_of_edges(), 0);

    auto test_single() {
        /** Tests that joining just one tree yields a tree with one more
        node.

         */
        T = xn::empty_graph(1);
        actual = xn::join([(T, 0)]);
        expected = xn::path_graph(2);
        assert_nodes_equal(list(expected), list(actual));
        assert_edges_equal(list(expected.edges()), list(actual.edges()));

    auto test_basic() {
        /** Tests for joining multiple subtrees at a root node. */
        trees = [(xn::full_rary_tree(2, 2 ** 2 - 1), 0) for i : range(2)];
        actual = xn::join(trees);
        expected = xn::full_rary_tree(2, 2 ** 3 - 1);
        assert_true(xn::is_isomorphic(actual, expected));
