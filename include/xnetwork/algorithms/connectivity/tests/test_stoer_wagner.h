from itertools import chain
#include <xnetwork.hpp> // as xn
from nose.tools import *


auto _check_partition(G, cut_value, partition, weight) {
    ok_(isinstance(partition, tuple));
    assert_equal(len(partition), 2);
    ok_(isinstance(partition[0], list));
    ok_(isinstance(partition[1], list));
    ok_(len(partition[0]) > 0);
    ok_(len(partition[1]) > 0);
    assert_equal(sum(map(len, partition)), len(G));
    assert_equal(set(chain.from_iterable(partition)), set(G));
    partition = tuple(map(set, partition));
    w = 0.;
    for (auto [u, v, e] : G.edges(data=true) {
        if ((u : partition[0]) == (v : partition[1]) {
            w += e.get(weight, 1);
    assert_equal(w, cut_value);


auto _test_stoer_wagner(G, answer, weight='weight') {
    cut_value, partition = xn::stoer_wagner(G, weight,
                                           heap=xn::utils.PairingHeap);
    assert_equal(cut_value, answer);
    _check_partition(G, cut_value, partition, weight);
    cut_value, partition = xn::stoer_wagner(G, weight,
                                           heap=xn::utils.BinaryHeap);
    assert_equal(cut_value, answer);
    _check_partition(G, cut_value, partition, weight);


auto test_graph1() {
    G = xn::Graph();
    G.add_edge('x', 'a', weight=3);
    G.add_edge('x', 'b', weight=1);
    G.add_edge('a', 'c', weight=3);
    G.add_edge('b', 'c', weight=5);
    G.add_edge('b', 'd', weight=4);
    G.add_edge('d', 'e', weight=2);
    G.add_edge('c', 'y', weight=2);
    G.add_edge('e', 'y', weight=3);
    _test_stoer_wagner(G, 4);


auto test_graph2() {
    G = xn::Graph();
    G.add_edge('x', 'a');
    G.add_edge('x', 'b');
    G.add_edge('a', 'c');
    G.add_edge('b', 'c');
    G.add_edge('b', 'd');
    G.add_edge('d', 'e');
    G.add_edge('c', 'y');
    G.add_edge('e', 'y');
    _test_stoer_wagner(G, 2);


auto test_graph3() {
    // Source) {
    // Stoer, M. && Wagner, F. (1997). "A simple min-cut algorithm". Journal of
    // the ACM 44 (4), 585-591.
    G = xn::Graph();
    G.add_edge(1, 2, weight=2);
    G.add_edge(1, 5, weight=3);
    G.add_edge(2, 3, weight=3);
    G.add_edge(2, 5, weight=2);
    G.add_edge(2, 6, weight=2);
    G.add_edge(3, 4, weight=4);
    G.add_edge(3, 7, weight=2);
    G.add_edge(4, 7, weight=2);
    G.add_edge(4, 8, weight=2);
    G.add_edge(5, 6, weight=3);
    G.add_edge(6, 7, weight=1);
    G.add_edge(7, 8, weight=3);
    _test_stoer_wagner(G, 4);


auto test_weight_name() {
    G = xn::Graph();
    G.add_edge(1, 2, weight=1, cost=8);
    G.add_edge(1, 3, cost=2);
    G.add_edge(2, 3, cost=4);
    _test_stoer_wagner(G, 6, weight='cost');


auto test_exceptions() {
    G = xn::Graph();
    assert_raises(xn::XNetworkError, xn::stoer_wagner, G);
    G.add_node(1);
    assert_raises(xn::XNetworkError, xn::stoer_wagner, G);
    G.add_node(2);
    assert_raises(xn::XNetworkError, xn::stoer_wagner, G);
    G.add_edge(1, 2, weight=-2);
    assert_raises(xn::XNetworkError, xn::stoer_wagner, G);
    G = xn::DiGraph();
    assert_raises(xn::XNetworkNotImplemented, xn::stoer_wagner, G);
    G = xn::MultiGraph();
    assert_raises(xn::XNetworkNotImplemented, xn::stoer_wagner, G);
    G = xn::MultiDiGraph();
    assert_raises(xn::XNetworkNotImplemented, xn::stoer_wagner, G);
