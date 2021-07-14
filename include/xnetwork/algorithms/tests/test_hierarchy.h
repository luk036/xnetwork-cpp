// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // as xn


auto test_hierarchy_exception() {
    G = xn::cycle_graph(5);
    assert_raises(xn::XNetworkError, xn::flow_hierarchy, G);


auto test_hierarchy_cycle() {
    G = xn::cycle_graph(5, create_using=xn::DiGraph());
    assert_equal(xn::flow_hierarchy(G), 0.0);


auto test_hierarchy_tree() {
    G = xn::full_rary_tree(2, 16, create_using=xn::DiGraph());
    assert_equal(xn::flow_hierarchy(G), 1.0);


auto test_hierarchy_1() {
    G = xn::DiGraph();
    G.add_edges_from([(0, 1), (1, 2), (2, 3), (3, 1), (3, 4), (0, 4)]);
    assert_equal(xn::flow_hierarchy(G), 0.5);


auto test_hierarchy_weight() {
    G = xn::DiGraph();
    G.add_edges_from([(0, 1, {"weight": .3}),
                      (1, 2, {"weight": .1}),
                      (2, 3, {"weight": .1}),
                      (3, 1, {"weight": .1}),
                      (3, 4, {"weight": .3}),
                      (0, 4, {"weight": .3})]);
    assert_equal(xn::flow_hierarchy(G, weight="weight"), .75);
