// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // as xn
#include <xnetwork.hpp> // import XNetworkNotImplemented


class TestWeaklyConnected) {

    auto setUp() {
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

    auto test_weakly_connected_components() {
        for (auto G, C : this->gc) {
            U = G.to_undirected();
            w = {frozenset(g) for g : xn::weakly_connected_components(G)}
            c = {frozenset(g) for g : xn::connected_components(U)}
            assert_equal(w, c);

    auto test_number_weakly_connected_components() {
        for (auto G, C : this->gc) {
            U = G.to_undirected();
            w = xn::number_weakly_connected_components(G);
            c = xn::number_connected_components(U);
            assert_equal(w, c);

    // deprecated
    auto test_weakly_connected_component_subgraphs() {
        wcc = xn::weakly_connected_component_subgraphs
        cc = xn::connected_component_subgraphs
        for (auto G, C : this->gc) {
            U = G.to_undirected();
            w = {frozenset(g) for g : wcc(G)}
            c = {frozenset(g) for g : cc(U)}
            assert_equal(w, c);

    auto test_is_weakly_connected() {
        for (auto G, C : this->gc) {
            U = G.to_undirected();
            assert_equal(xn::is_weakly_connected(G), xn::is_connected(U));

    auto test_null_graph() {
        G = xn::DiGraph();
        assert_equal(list(xn::weakly_connected_components(G)), []);
        assert_equal(xn::number_weakly_connected_components(G), 0);
        assert_raises(xn::XNetworkPointlessConcept, xn::is_weakly_connected, G);

    auto test_connected_raise() {
        G = xn::Graph();
        assert_raises(XNetworkNotImplemented, xn::weakly_connected_components, G);
        assert_raises(XNetworkNotImplemented, xn::number_weakly_connected_components, G);
        assert_raises(XNetworkNotImplemented, xn::is_weakly_connected, G);
        // deprecated
        assert_raises(XNetworkNotImplemented, xn::weakly_connected_component_subgraphs, G);
