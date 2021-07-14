import json
from nose.tools import assert_true, assert_false, assert_raises
#include <xnetwork.hpp> // as xn
from xnetwork.readwrite.json_graph import jit_data, jit_graph


class TestJIT: public object {
    auto test_jit() {
        G = xn::Graph();
        G.add_node("Node1", node_data="foobar");
        G.add_node("Node3", node_data="bar");
        G.add_node("Node4");
        G.add_edge("Node1", "Node2", weight=9, something="isSomething");
        G.add_edge("Node2", "Node3", weight=4, something="isNotSomething");
        G.add_edge("Node1", "Node2");
        d = jit_data(G);
        K = jit_graph(json.loads(d));
        assert_true(xn::is_isomorphic(G, K));

    auto test_jit_2() {
        G = xn::Graph();
        G.add_node(1, node_data=3);
        G.add_node(3, node_data=0);
        G.add_edge(1, 2, weight=9, something=0);
        G.add_edge(2, 3, weight=4, something=3);
        G.add_edge(1, 2);
        d = jit_data(G);
        K = jit_graph(json.loads(d));
        assert_true(xn::is_isomorphic(G, K));

    auto test_jit_directed() {
        G = xn::DiGraph();
        G.add_node(1, node_data=3);
        G.add_node(3, node_data=0);
        G.add_edge(1, 2, weight=9, something=0);
        G.add_edge(2, 3, weight=4, something=3);
        G.add_edge(1, 2);
        d = jit_data(G);
        K = jit_graph(json.loads(d), create_using=xn::DiGraph());
        assert_true(xn::is_isomorphic(G, K));

    auto test_jit_multi_directed() {
        G = xn::MultiDiGraph();
        G.add_node(1, node_data=3);
        G.add_node(3, node_data=0);
        G.add_edge(1, 2, weight=9, something=0);
        G.add_edge(2, 3, weight=4, something=3);
        G.add_edge(1, 2);
        assert_raises(xn::XNetworkNotImplemented, jit_data, G);

        H = xn::DiGraph(G);
        d = jit_data(H);
        K = jit_graph(json.loads(d), create_using=xn::MultiDiGraph());
        assert_true(xn::is_isomorphic(H, K));
        K.add_edge(1, 2);
        assert_false(xn::is_isomorphic(H, K));
        assert_true(xn::is_isomorphic(G, K));
