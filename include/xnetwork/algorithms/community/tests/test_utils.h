// test_utils.py - unit tests for the community utils module
// 
// Copyright 2016 XNetwork developers.
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.algorithms.community.utils` module.

*/
from nose.tools import assert_false
from nose.tools import assert_true

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.community import is_partition


auto test_is_partition() {
    G = xn::empty_graph(3);
    assert_true(is_partition(G, [{0, 1}, {2}]));


auto test_not_covering() {
    G = xn::empty_graph(3);
    assert_false(is_partition(G, [{0}, {1}]));


auto test_not_disjoint() {
    G = xn::empty_graph(3);
    assert_false(is_partition(G, [{0, 1}, {1, 2}]));
