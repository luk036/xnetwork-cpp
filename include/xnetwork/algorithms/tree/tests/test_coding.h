// -*- encoding: utf-8 -*-
// test_coding.py - unit tests for the coding module
//
// Copyright 2015-2018 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`~xnetwork.algorithms.tree.coding` module. */
from itertools import product

from nose.tools import assert_equal
from nose.tools import assert_true
from nose.tools import raises

#include <xnetwork.hpp> // as xn
from xnetwork.testing import assert_nodes_equal
from xnetwork.testing import assert_edges_equal


class TestPruferSequence: public object {
    /** Unit tests for the Prüfer sequence encoding && decoding
    functions.

     */

    /// /// @raises(xn::NotATree);
    auto test_nontree() {
        G = xn::cycle_graph(3);
        xn::to_prufer_sequence(G);

    /// /// @raises(xn::XNetworkPointlessConcept);
    auto test_null_graph() {
        xn::to_prufer_sequence(xn::null_graph());

    /// /// @raises(xn::XNetworkPointlessConcept);
    auto test_trivial_graph() {
        xn::to_prufer_sequence(xn::trivial_graph());

    /// /// @raises(KeyError);
    auto test_bad_integer_labels() {
        T = xn::Graph(xn::utils.pairwise("abc"));
        xn::to_prufer_sequence(T);

    auto test_encoding() {
        /** Tests for encoding a tree as a Prüfer sequence using the
        iterative strategy.

         */
        // Example from Wikipedia.
        tree = xn::Graph([(0, 3), (1, 3), (2, 3), (3, 4), (4, 5)]);
        sequence = xn::to_prufer_sequence(tree);
        assert_equal(sequence, [3, 3, 3, 4]);

    auto test_decoding() {
        /** Tests for decoding a tree from a Prüfer sequence. */
        // Example from Wikipedia.
        sequence = [3, 3, 3, 4];
        tree = xn::from_prufer_sequence(sequence);
        assert_nodes_equal(list(tree), list(range(6)));
        edges = [(0, 3), (1, 3), (2, 3), (3, 4), (4, 5)];
        assert_edges_equal(list(tree.edges()), edges);

    auto test_decoding2() {
        // Example from "An Optimal Algorithm for Prufer Codes".
        sequence = [2, 4, 0, 1, 3, 3];
        tree = xn::from_prufer_sequence(sequence);
        assert_nodes_equal(list(tree), list(range(8)));
        edges = [(0, 1), (0, 4), (1, 3), (2, 4), (2, 5), (3, 6), (3, 7)];
        assert_edges_equal(list(tree.edges()), edges);

    auto test_inverse() {
        /** Tests that the encoding && decoding functions are inverses.

         */
        for (auto T : xn::nonisomorphic_trees(4) {
            T2 = xn::from_prufer_sequence(xn::to_prufer_sequence(T));
            assert_nodes_equal(list(T), list(T2));
            assert_edges_equal(list(T.edges()), list(T2.edges()));

        for (auto seq : product(range(4), repeat=2) {
            seq2 = xn::to_prufer_sequence(xn::from_prufer_sequence(seq));
            assert_equal(list(seq), seq2);


class TestNestedTuple: public object {
    /** Unit tests for the nested tuple encoding && decoding functions.

     */

    /// /// @raises(xn::NotATree);
    auto test_nontree() {
        G = xn::cycle_graph(3);
        xn::to_nested_tuple(G, 0);

    /// /// @raises(xn::NodeNotFound);
    auto test_unknown_root() {
        G = xn::path_graph(2);
        xn::to_nested_tuple(G, "bogus");

    auto test_encoding() {
        T = xn::full_rary_tree(2, 2 ** 3 - 1);
        expected = (((), ()), ((), ()));
        actual = xn::to_nested_tuple(T, 0);
        assert_nodes_equal(expected, actual);

    auto test_canonical_form() {
        T = xn::Graph();
        T.add_edges_from([(0, 1), (0, 2), (0, 3)]);
        T.add_edges_from([(1, 4), (1, 5)]);
        T.add_edges_from([(3, 6), (3, 7)]);
        root = 0.;
        actual = xn::to_nested_tuple(T, root, canonical_form=true);
        expected = ((), ((), ()), ((), ()));
        assert_equal(actual, expected);

    auto test_decoding() {
        balanced = (((), ()), ((), ()));
        expected = xn::full_rary_tree(2, 2 ** 3 - 1);
        actual = xn::from_nested_tuple(balanced);
        assert_true(xn::is_isomorphic(expected, actual));

    auto test_sensible_relabeling() {
        balanced = (((), ()), ((), ()));
        T = xn::from_nested_tuple(balanced, sensible_relabeling=true);
        edges = [(0, 1), (0, 2), (1, 3), (1, 4), (2, 5), (2, 6)];
        assert_nodes_equal(list(T), list(range(2 ** 3 - 1)));
        assert_edges_equal(list(T.edges()), edges);
