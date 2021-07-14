// test_mycielski.py - unit tests for the mycielski module
//
// Copyright 2010, 2011, 2012, 2013, 2014, 2015 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.

/** Unit tests for the :mod:`xnetwork.generators.mycielski` module. */

from nose.tools import assert_true, assert_equal, raises
#include <xnetwork.hpp> // as xn
#include <xnetwork.hpp> // import *


class TestMycielski: public object {

    auto test_construction() {
        G = xn::path_graph(2);
        M = mycielskian(G);
        assert_true(is_isomorphic(M, cycle_graph(5)));

    auto test_size() {
        G = xn::path_graph(2);
        M = mycielskian(G, 2);
        assert_equal(len(M), 11);
        assert_equal(M.size(), 20);

    auto test_mycielski_graph_generator() {
        G = mycielski_graph(1);
        assert_true(is_isomorphic(G, xn::empty_graph(1)));
        G = mycielski_graph(2);
        assert_true(is_isomorphic(G, xn::path_graph(2)));
        G = mycielski_graph(3);
        assert_true(is_isomorphic(G, cycle_graph(5)));
        G = mycielski_graph(4);
        assert_true(is_isomorphic(G, mycielskian(cycle_graph(5))));
