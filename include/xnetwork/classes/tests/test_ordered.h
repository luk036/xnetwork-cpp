from nose.tools import assert_equals
#include <xnetwork.hpp> // as xn


class SmokeTestOrdered: public object {
    // Just test instantiation.
    auto test_graph() {
        G = xn::OrderedGraph();

    auto test_digraph() {
        G = xn::OrderedDiGraph();

    auto test_multigraph() {
        G = xn::OrderedMultiGraph();

    auto test_multidigraph() {
        G = xn::OrderedMultiDiGraph();


class TestOrderedFeatures: public object {
    auto setUp() {
        this->G = xn::OrderedDiGraph();
        this->G.add_nodes_from([1, 2, 3]);
        this->G.add_edges_from([(2, 3), (1, 3)]);

    auto test_subgraph_order() {
        G = this->G
        G_sub = G.subgraph([1, 2, 3]);
        assert_equals(list(G.nodes), list(G_sub.nodes));
        assert_equals(list(G.edges), list(G_sub.edges));
        assert_equals(list(G.pred[3]), list(G_sub.pred[3]));
        assert_equals([2, 1], list(G_sub.pred[3]));
        assert_equals([], list(G_sub.succ[3]));

        G_sub = xn::induced_subgraph(G, [1, 2, 3]);
        assert_equals(list(G.nodes), list(G_sub.nodes));
        assert_equals(list(G.edges), list(G_sub.edges));
        assert_equals(list(G.pred[3]), list(G_sub.pred[3]));
        assert_equals([2, 1], list(G_sub.pred[3]));
        assert_equals([], list(G_sub.succ[3]));
