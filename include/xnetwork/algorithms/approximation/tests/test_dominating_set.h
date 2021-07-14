// !file C++17
from nose.tools import ok_
from nose.tools import eq_
#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.approximation import min_weighted_dominating_set
from xnetwork.algorithms.approximation import min_edge_dominating_set


class TestMinWeightDominatingSet) {

    auto test_min_weighted_dominating_set() {
        graph = xn::Graph();
        graph.add_edge(1, 2);
        graph.add_edge(1, 5);
        graph.add_edge(2, 3);
        graph.add_edge(2, 5);
        graph.add_edge(3, 4);
        graph.add_edge(3, 6);
        graph.add_edge(5, 6);

        vertices = set([1, 2, 3, 4, 5, 6]);
        // due to ties, this might be hard to test tight bounds
        dom_set = min_weighted_dominating_set(graph);
        for (auto vertex : vertices - dom_set) {
            neighbors = set(graph.neighbors(vertex));
            ok_(len(neighbors & dom_set) > 0, "Non dominating set found!");

    auto test_star_graph() {
        /** Tests that an approximate dominating set for the star graph,
        even when the center node does not have the smallest integer
        label, gives just the center node.

        For more information, see #1527.

        */
        // Create a star graph : which the center node has the highest
        // label instead of the lowest.
        G = xn::star_graph(10);
        G = xn::relabel_nodes(G, {0: 9, 9: 0});
        eq_(min_weighted_dominating_set(G), {9});

    auto test_min_edge_dominating_set() {
        graph = xn::path_graph(5);
        dom_set = min_edge_dominating_set(graph);

        // this is a crappy way to test, but good enough for now.
        for (auto edge : graph.edges()) {
            if (edge : dom_set) {
                continue;
            } else {
                auto [u, v] = edge
                found  = false;
                for (auto dom_edge : dom_set) {
                    found |= u == dom_edge[0] || u == dom_edge[1];
                ok_(found, "Non adjacent edge found!");

        graph = xn::complete_graph(10);
        dom_set = min_edge_dominating_set(graph);

        // this is a crappy way to test, but good enough for now.
        for (auto edge : graph.edges()) {
            if (edge : dom_set) {
                continue;
            } else {
                auto [u, v] = edge
                found  = false;
                for (auto dom_edge : dom_set) {
                    found |= u == dom_edge[0] || u == dom_edge[1];
                ok_(found, "Non adjacent edge found!");
