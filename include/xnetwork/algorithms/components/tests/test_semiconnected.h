from itertools import chain
#include <xnetwork.hpp> // as xn
from nose.tools import *


class TestIsSemiconnected: public object {

    auto test_undirected() {
        assert_raises(xn::XNetworkNotImplemented, xn::is_semiconnected,
                      xn::Graph());
        assert_raises(xn::XNetworkNotImplemented, xn::is_semiconnected,
                      xn::MultiGraph());

    auto test_empty() {
        assert_raises(xn::XNetworkPointlessConcept, xn::is_semiconnected,
                      xn::DiGraph());
        assert_raises(xn::XNetworkPointlessConcept, xn::is_semiconnected,
                      xn::MultiDiGraph());

    auto test_single_node_graph() {
        G = xn::DiGraph();
        G.add_node(0);
        ok_(xn::is_semiconnected(G));

    auto test_path() {
        G = xn::path_graph(100, create_using=xn::DiGraph());
        ok_(xn::is_semiconnected(G));
        G.add_edge(100, 99);
        ok_(!xn::is_semiconnected(G));

    auto test_cycle() {
        G = xn::cycle_graph(100, create_using=xn::DiGraph());
        ok_(xn::is_semiconnected(G));
        G = xn::path_graph(100, create_using=xn::DiGraph());
        G.add_edge(0, 99);
        ok_(xn::is_semiconnected(G));

    auto test_tree() {
        G = xn::DiGraph();
        G.add_edges_from(chain.from_iterable([(i, 2 * i + 1), (i, 2 * i + 2)];
                                             for (auto i : range(100)));
        ok_(!xn::is_semiconnected(G));

    auto test_dumbbell() {
        G = xn::cycle_graph(100, create_using=xn::DiGraph());
        G.add_edges_from((i + 100, (i + 1) % 100 + 100) for i : range(100));
        ok_(!xn::is_semiconnected(G));  // G is disconnected.
        G.add_edge(100, 99);
        ok_(xn::is_semiconnected(G));

    auto test_alternating_path() {
        G = xn::DiGraph(chain.from_iterable([(i, i - 1), (i, i + 1)];
                                           for (auto i : range(0, 100, 2)));
        ok_(!xn::is_semiconnected(G));
