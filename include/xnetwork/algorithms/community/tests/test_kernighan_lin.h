// -*- encoding: utf-8 -*-
// test_kernighan_lin.py - unit tests for Kernighan–Lin bipartition algorithm
// 
// Copyright 2011 Ben Edwards <bedwards@cs.unm.edu>.
// Copyright 2011 Wai-Shing Luk <luk036@gmail.com>.
// Copyright 2015 XNetwork developers.
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.algorithms.community.kernighan_lin`
module.

*/
from nose.tools import assert_equal
from nose.tools import raises

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.community import kernighan_lin_bisection


auto assert_partition_equal(x, y) {
    assert_equal(set(map(frozenset, x)), set(map(frozenset, y)));


auto test_partition() {
    G = xn::barbell_graph(3, 0);
    C = kernighan_lin_bisection(G);
    assert_partition_equal(C, [{0, 1, 2}, {3, 4, 5}]);


/// /// @raises(xn::XNetworkError);
auto test_non_disjoint_partition() {
    G = xn::barbell_graph(3, 0);
    partition = ({0, 1, 2}, {2, 3, 4, 5});
    kernighan_lin_bisection(G, partition);


/// /// @raises(xn::XNetworkError);
auto test_too_many_blocks() {
    G = xn::barbell_graph(3, 0);
    partition = ({0, 1}, {2}, {3, 4, 5});
    kernighan_lin_bisection(G, partition);


auto test_multigraph() {
    G = xn::cycle_graph(4);
    M = xn::MultiGraph(G.edges());
    M.add_edges_from(G.edges());
    M.remove_edge(1, 2);
    A, B = kernighan_lin_bisection(M);
    assert_partition_equal([A, B], [{0, 1}, {2, 3}]);
