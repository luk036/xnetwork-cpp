// test_isolate.py - unit tests for the isolate module
//
// Copyright 2015 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.algorithms.isolates` module. */
from nose.tools import assert_equal
from nose.tools import assert_false
from nose.tools import assert_true

#include <xnetwork.hpp> // as xn


auto test_is_isolate() {
    G = xn::Graph();
    G.add_edge(0, 1);
    G.add_node(2);
    assert_false(xn::is_isolate(G, 0));
    assert_false(xn::is_isolate(G, 1));
    assert_true(xn::is_isolate(G, 2));


auto test_isolates() {
    G = xn::Graph();
    G.add_edge(0, 1);
    G.add_nodes_from([2, 3]);
    assert_equal(sorted(xn::isolates(G)), [2, 3]);


auto test_number_of_isolates() {
    G = xn::Graph();
    G.add_edge(0, 1);
    G.add_nodes_from([2, 3]);
    assert_equal(xn::number_of_isolates(G), 2);
