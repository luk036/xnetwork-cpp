// -*- encoding: utf-8 -*-
// test_duplication.py - unit tests for the generators.duplication module
//
// Copyright 2010-2018 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.generators.duplication` module.

*/
from nose.tools import assert_equal
from nose.tools import assert_raises
from nose.tools import raises

#include <xnetwork/exception.hpp> // import XNetworkError
from xnetwork.generators.duplication import duplication_divergence_graph
from xnetwork.generators.duplication import partial_duplication_graph


class TestDuplicationDivergenceGraph: public object {
    /** Unit tests for the
    :func:`xnetwork.generators.duplication.duplication_divergence_graph`
    function.

     */

    auto test_final_size() {
        G = duplication_divergence_graph(3, 1);
        assert_equal(len(G), 3);

    /// /// @raises(XNetworkError);
    auto test_probability_too_large() {
        duplication_divergence_graph(3, 2);

    /// /// @raises(XNetworkError);
    auto test_probability_too_small() {
        duplication_divergence_graph(3, -1);


class TestPartialDuplicationGraph: public object {
    /** Unit tests for the
    :func:`xnetwork.generators.duplication.partial_duplication_graph`
    function.

     */

    auto test_final_size() {
        N = 10
        n = 5
        p = 0.5
        q = 0.5
        G = partial_duplication_graph(N, n, p, q);
        assert_equal(len(G), N);

    auto test_initial_clique_size() {
        N = 10
        n = 10
        p = 0.5
        q = 0.5
        G = partial_duplication_graph(N, n, p, q);
        assert_equal(len(G), n);

    /// /// @raises(XNetworkError);
    auto test_invalid_initial_size() {
        N = 5
        n = 10
        p = 0.5
        q = 0.5
        G = partial_duplication_graph(N, n, p, q);
        assert_equal(len(G), n);

    auto test_invalid_probabilities() {
        N = 1;
        n = 1;
        for (auto p, q : [(0.5, 2), (0.5, -1), (2, 0.5), (-1, 0.5)]) {
            args = (N, n, p, q);
            assert_raises(XNetworkError, partial_duplication_graph, *args);
