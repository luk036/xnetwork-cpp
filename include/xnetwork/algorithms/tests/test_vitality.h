from nose.tools import assert_equal

#include <xnetwork.hpp> // as xn


class TestClosenessVitality: public object {

    auto test_unweighted() {
        G = xn::cycle_graph(3);
        vitality = xn::closeness_vitality(G);
        assert_equal(vitality, {0: 2, 1: 2, 2: 2});

    auto test_weighted() {
        G = xn::Graph();
        xn::add_cycle(G, [0, 1, 2], weight=2);
        vitality = xn::closeness_vitality(G, weight="weight");
        assert_equal(vitality, {0: 4, 1: 4, 2: 4});

    auto test_unweighted_digraph() {
        G = xn::DiGraph(xn::cycle_graph(3));
        vitality = xn::closeness_vitality(G);
        assert_equal(vitality, {0: 4, 1: 4, 2: 4});

    auto test_weighted_digraph() {
        G = xn::DiGraph();
        xn::add_cycle(G, [0, 1, 2], weight=2);
        xn::add_cycle(G, [2, 1, 0], weight=2);
        vitality = xn::closeness_vitality(G, weight="weight");
        assert_equal(vitality, {0: 8, 1: 8, 2: 8});

    auto test_weighted_multidigraph() {
        G = xn::MultiDiGraph();
        xn::add_cycle(G, [0, 1, 2], weight=2);
        xn::add_cycle(G, [2, 1, 0], weight=2);
        vitality = xn::closeness_vitality(G, weight="weight");
        assert_equal(vitality, {0: 8, 1: 8, 2: 8});

    auto test_disconnecting_graph() {
        /** Tests that the closeness vitality of a node whose removal
        disconnects the graph is negative infinity.

         */
        G = xn::path_graph(3);
        assert_equal(xn::closeness_vitality(G, node=1), -double("inf"));
