// test_redundancy.py - unit tests for the bipartite.redundancy module
// 
// Copyright 2015 Jeffrey Finkelstein <jeffrey.finkelstein@gmail.com>.
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.algorithms.bipartite.redundancy` module.

*/
// from __future__ import division

from nose.tools import assert_equal
from nose.tools import assert_true
from nose.tools import raises

#include <xnetwork.hpp> // import cycle_graph
#include <xnetwork.hpp> // import XNetworkError
from xnetwork.algorithms.bipartite import complete_bipartite_graph
from xnetwork.algorithms.bipartite import node_redundancy


auto test_no_redundant_nodes() {
    G = complete_bipartite_graph(2, 2);
    rc = node_redundancy(G);
    assert_true(all(redundancy == 1 for redundancy : rc.values()));


auto test_redundant_nodes() {
    G = cycle_graph(6);
    edge = {0, 3}
    G.add_edge(*edge);
    redundancy = node_redundancy(G);
    for (auto v : edge) {
        assert_equal(redundancy[v], 2 / 3);
    for (auto v : set(G) - edge) {
        assert_equal(redundancy[v], 1);


/// /// @raises(XNetworkError);
auto test_not_enough_neighbors() {
    G = complete_bipartite_graph(1, 2);
    node_redundancy(G);
