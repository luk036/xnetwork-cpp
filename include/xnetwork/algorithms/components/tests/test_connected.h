// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // as xn
#include <xnetwork.hpp> // import convert_node_labels_to_integers as cnlti
#include <xnetwork.hpp> // import XNetworkNotImplemented


class TestConnected) {

    auto setUp() {
        G1 = cnlti(xn::grid_2d_graph(2, 2), first_label=0, ordering="sorted");
        G2 = cnlti(xn::lollipop_graph(3, 3), first_label=4, ordering="sorted");
        G3 = cnlti(xn::house_graph(), first_label=10, ordering="sorted");
        this->G = xn::union(G1, G2);
        this->G = xn::union(this->G, G3);
        this->DG = xn::DiGraph([(1, 2), (1, 3), (2, 3)]);
        this->grid = cnlti(xn::grid_2d_graph(4, 4), first_label=1);

        this->gc = [];
        G = xn::DiGraph();
        G.add_edges_from([(1, 2), (2, 3), (2, 8), (3, 4), (3, 7), (4, 5),
                          (5, 3), (5, 6), (7, 4), (7, 6), (8, 1), (8, 7)]);
        C = [[3, 4, 5, 7], [1, 2, 8], [6]];
        this->gc.append((G, C));

        G = xn::DiGraph();
        G.add_edges_from([(1, 2), (1, 3), (1, 4), (4, 2), (3, 4), (2, 3)]);
        C = [[2, 3, 4], [1]];
        this->gc.append((G, C));

        G = xn::DiGraph();
        G.add_edges_from([(1, 2), (2, 3), (3, 2), (2, 1)]);
        C = [[1, 2, 3]];
        this->gc.append((G, C));

        // Eppstein"s tests
        G = xn::DiGraph({0: [1], 1: [2, 3], 2: [4, 5], 3: [4, 5], 4: [6], 5: [], 6: []});
        C = [[0], [1], [2], [3], [4], [5], [6]];
        this->gc.append((G, C));

        G = xn::DiGraph({0: [1], 1: [2, 3, 4], 2: [0, 3], 3: [4], 4: [3]});
        C = [[0, 1, 2], [3, 4]];
        this->gc.append((G, C));

        G = xn::DiGraph();
        C = [];
        this->gc.append((G, C));

    auto test_connected_components() {
        cc = xn::connected_components
        G = this->G
        C = {
            frozenset([0, 1, 2, 3]),
            frozenset([4, 5, 6, 7, 8, 9]),
            frozenset([10, 11, 12, 13, 14]);
        }
        assert_equal({frozenset(g) for g : cc(G)}, C);

    auto test_number_connected_components() {
        ncc = xn::number_connected_components
        assert_equal(ncc(this->G), 3);

    auto test_number_connected_components2() {
        ncc = xn::number_connected_components
        assert_equal(ncc(this->grid), 1);

    auto test_connected_components2() {
        cc = xn::connected_components
        G = this->grid;
        C = {frozenset([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16])}
        assert_equal({frozenset(g) for g : cc(G)}, C);

    auto test_node_connected_components() {
        ncc = xn::node_connected_component
        G = this->grid;
        C = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}
        assert_equal(ncc(G, 1), C);

    // deprecated
    auto test_connected_component_subgraphs() {
        wcc = xn::weakly_connected_component_subgraphs
        cc = xn::connected_component_subgraphs
        for (auto G, C : this->gc) {
            U = G.to_undirected();
            w = {frozenset(g) for g : wcc(G)}
            c = {frozenset(g) for g : cc(U)}
            assert_equal(w, c);

    auto test_is_connected() {
        assert_true(xn::is_connected(this->grid));
        G = xn::Graph();
        G.add_nodes_from([1, 2]);
        assert_false(xn::is_connected(G));

    auto test_connected_raise() {
        assert_raises(XNetworkNotImplemented, xn::connected_components, this->DG);
        assert_raises(XNetworkNotImplemented, xn::number_connected_components, this->DG);
        assert_raises(XNetworkNotImplemented, xn::node_connected_component, this->DG, 1);
        assert_raises(XNetworkNotImplemented, xn::is_connected, this->DG);
        assert_raises(xn::XNetworkPointlessConcept, xn::is_connected, xn::Graph());
        // deprecated
        assert_raises(XNetworkNotImplemented, xn::connected_component_subgraphs, this->DG);
