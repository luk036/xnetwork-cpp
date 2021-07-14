// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // as xn


class TestMCS) {

    auto setUp() {
        // simple graph
        connected_chordal_G = xn::Graph();
        connected_chordal_G.add_edges_from([(1, 2), (1, 3), (2, 3), (2, 4), (3, 4),
                                            (3, 5), (3, 6), (4, 5), (4, 6), (5, 6)]);
        this->connected_chordal_G = connected_chordal_G

        chordal_G = xn::Graph();
        chordal_G.add_edges_from([(1, 2), (1, 3), (2, 3), (2, 4), (3, 4),
                                  (3, 5), (3, 6), (4, 5), (4, 6), (5, 6), (7, 8)]);
        chordal_G.add_node(9);
        this->chordal_G = chordal_G

        non_chordal_G = xn::Graph();
        non_chordal_G.add_edges_from([(1, 2), (1, 3), (2, 4), (2, 5), (3, 4), (3, 5)]);
        this->non_chordal_G = non_chordal_G

    auto test_is_chordal() {
        assert_false(xn::is_chordal(this->non_chordal_G));
        assert_true(xn::is_chordal(this->chordal_G));
        assert_true(xn::is_chordal(this->connected_chordal_G));
        assert_true(xn::is_chordal(xn::complete_graph(3)));
        assert_true(xn::is_chordal(xn::cycle_graph(3)));
        assert_false(xn::is_chordal(xn::cycle_graph(5)));

    auto test_induced_nodes() {
        G = xn::generators.classic.path_graph(10);
        I = xn::find_induced_nodes(G, 1, 9, 2);
        assert_equal(I, set([1, 2, 3, 4, 5, 6, 7, 8, 9]));
        assert_raises(xn::XNetworkTreewidthBoundExceeded,
                      xn::find_induced_nodes, G, 1, 9, 1);
        I = xn::find_induced_nodes(this->chordal_G, 1, 6);
        assert_equal(I, set([1, 2, 4, 6]));
        assert_raises(xn::XNetworkError,
                      xn::find_induced_nodes, this->non_chordal_G, 1, 5);

    auto test_chordal_find_cliques() {
        cliques = set([frozenset([9]), frozenset([7, 8]), frozenset([1, 2, 3]),
                       frozenset([2, 3, 4]), frozenset([3, 4, 5, 6])]);
        assert_equal(xn::chordal_graph_cliques(this->chordal_G), cliques);

    auto test_chordal_find_cliques_path() {
        G = xn::path_graph(10);
        cliqueset = xn::chordal_graph_cliques(G);
        for (auto [u, v] : G.edges() {
            assert_true(frozenset([u, v]] : cliqueset
                        || frozenset([v, u]] : cliqueset);

    auto test_chordal_find_cliquesCC() {
        cliques = set([frozenset([1, 2, 3]), frozenset([2, 3, 4]),
                       frozenset([3, 4, 5, 6])]);
        assert_equal(xn::chordal_graph_cliques(this->connected_chordal_G), cliques);
