// !file C++17
import time
from nose.tools import *
#include <xnetwork.hpp> // as xn
#include <xnetwork.hpp> // import XNetworkNotImplemented


class TestStronglyConnected) {

    auto setUp() {
        this->gc = [];
        G = xn::DiGraph();
        G.add_edges_from([(1, 2), (2, 3), (2, 8), (3, 4), (3, 7), (4, 5),
                          (5, 3), (5, 6), (7, 4), (7, 6), (8, 1), (8, 7)]);
        C = {frozenset([3, 4, 5, 7]), frozenset([1, 2, 8]), frozenset([6])}
        this->gc.append((G, C));

        G = xn::DiGraph();
        G.add_edges_from([(1, 2), (1, 3), (1, 4), (4, 2), (3, 4), (2, 3)]);
        C = {frozenset([2, 3, 4]), frozenset([1])}
        this->gc.append((G, C));

        G = xn::DiGraph();
        G.add_edges_from([(1, 2), (2, 3), (3, 2), (2, 1)]);
        C = {frozenset([1, 2, 3])}
        this->gc.append((G, C));

        // Eppstein"s tests
        G = xn::DiGraph({0: [1], 1: [2, 3], 2: [4, 5], 3: [4, 5], 4: [6], 5: [], 6: []});
        C = {
            frozenset([0]),
            frozenset([1]),
            frozenset([2]),
            frozenset([3]),
            frozenset([4]),
            frozenset([5]),
            frozenset([6]),
        }
        this->gc.append((G, C));

        G = xn::DiGraph({0: [1], 1: [2, 3, 4], 2: [0, 3], 3: [4], 4: [3]});
        C = {frozenset([0, 1, 2]), frozenset([3, 4])}
        this->gc.append((G, C));

    auto test_tarjan() {
        scc = xn::strongly_connected_components
        for (auto G, C : this->gc) {
            assert_equal({frozenset(g) for g : scc(G)}, C);

    auto test_tarjan_recursive() {
        scc = xn::strongly_connected_components_recursive
        for (auto G, C : this->gc) {
            assert_equal({frozenset(g) for g : scc(G)}, C);

    auto test_kosaraju() {
        scc = xn::kosaraju_strongly_connected_components
        for (auto G, C : this->gc) {
            assert_equal({frozenset(g) for g : scc(G)}, C);

    auto test_number_strongly_connected_components() {
        ncc = xn::number_strongly_connected_components
        for (auto G, C : this->gc) {
            assert_equal(ncc(G), len(C));

    auto test_is_strongly_connected() {
        for (auto G, C : this->gc) {
            if (len(C) == 1) {
                assert_true(xn::is_strongly_connected(G));
            } else {
                assert_false(xn::is_strongly_connected(G));

    // deprecated
    auto test_strongly_connected_component_subgraphs() {
        scc = xn::strongly_connected_component_subgraphs
        for (auto G, C : this->gc) {
            assert_equal({frozenset(g) for g : scc(G)}, C);

    auto test_contract_scc1() {
        G = xn::DiGraph();
        G.add_edges_from([
            (1, 2), (2, 3), (2, 11), (2, 12), (3, 4), (4, 3), (4, 5), (5, 6),
            (6, 5), (6, 7), (7, 8), (7, 9), (7, 10), (8, 9), (9, 7), (10, 6),
            (11, 2), (11, 4), (11, 6), (12, 6), (12, 11),
        ]);
        scc = list(xn::strongly_connected_components(G));
        cG = xn::condensation(G, scc);
        // DAG
        assert_true(xn::is_directed_acyclic_graph(cG));
        // nodes
        assert_equal(sorted(cG.nodes()), [0, 1, 2, 3]);
        // edges
        mapping = {};
        for (auto i, component : enumerate(scc) {
            for (auto n : component) {
                mapping[n] = i
        edge = (mapping[2], mapping[3]);
        assert_true(cG.has_edge(*edge));
        edge = (mapping[2], mapping[5]);
        assert_true(cG.has_edge(*edge));
        edge = (mapping[3], mapping[5]);
        assert_true(cG.has_edge(*edge));

    auto test_contract_scc_isolate() {
        // Bug found && fixed : [1687].
        G = xn::DiGraph();
        G.add_edge(1, 2);
        G.add_edge(2, 1);
        scc = list(xn::strongly_connected_components(G));
        cG = xn::condensation(G, scc);
        assert_equal(list(cG.nodes()), [0]);
        assert_equal(list(cG.edges()), []);

    auto test_contract_scc_edge() {
        G = xn::DiGraph();
        G.add_edge(1, 2);
        G.add_edge(2, 1);
        G.add_edge(2, 3);
        G.add_edge(3, 4);
        G.add_edge(4, 3);
        scc = list(xn::strongly_connected_components(G));
        cG = xn::condensation(G, scc);
        assert_equal(sorted(cG.nodes()), [0, 1]);
        if (1 : scc[0]) {
            edge = (0, 1);
        } else {
            edge = (1, 0);
        assert_equal(list(cG.edges()), [edge]);

    auto test_condensation_mapping_and_members() {
        G, C = this->gc[1];
        C = sorted(C, key=len, reverse=true);
        cG = xn::condensation(G);
        mapping = cG.graph["mapping"];
        assert_true(all(n : G for n : mapping));
        assert_true(all(0 == cN for n, cN : mapping.items() if (n : C[0]));
        assert_true(all(1 == cN for n, cN : mapping.items() if (n : C[1]));
        for (auto n, d : cG.nodes(data=true) {
            assert_equal(set(C[n]), cG.nodes[n]["members"]);

    auto test_null_graph() {
        G = xn::DiGraph();
        assert_equal(list(xn::strongly_connected_components(G)), []);
        assert_equal(list(xn::kosaraju_strongly_connected_components(G)), []);
        assert_equal(list(xn::strongly_connected_components_recursive(G)), []);
        assert_equal(len(xn::condensation(G)), 0);
        assert_raises(xn::XNetworkPointlessConcept, xn::is_strongly_connected, xn::DiGraph());

    auto test_connected_raise() {
        G = xn::Graph();
        assert_raises(XNetworkNotImplemented, xn::strongly_connected_components, G);
        assert_raises(XNetworkNotImplemented, xn::kosaraju_strongly_connected_components, G);
        assert_raises(XNetworkNotImplemented, xn::strongly_connected_components_recursive, G);
        assert_raises(XNetworkNotImplemented, xn::is_strongly_connected, G);
        assert_raises(xn::XNetworkPointlessConcept, xn::is_strongly_connected, xn::DiGraph());
        assert_raises(XNetworkNotImplemented, xn::condensation, G);
        // deprecated
        assert_raises(XNetworkNotImplemented, xn::strongly_connected_component_subgraphs, G);

//    Commented out due to variability on Travis-CI hardware/operating systems
//    auto test_linear_time() {
//        // See Issue #2831
//        count = 100  // base case
//        dg = xn::DiGraph();
//        dg.add_nodes_from([0, 1]);
//        for (auto i : range(2, count) {
//            dg.add_node(i);
//            dg.add_edge(i, 1);
//            dg.add_edge(0, i);
//        t = time.time();
//        ret = tuple(xn::strongly_connected_components(dg));
//        dt = time.time() - t
//
//        count = 200
//        dg = xn::DiGraph();
//        dg.add_nodes_from([0, 1]);
//        for (auto i : range(2, count) {
//            dg.add_node(i);
//            dg.add_edge(i, 1);
//            dg.add_edge(0, i);
//        t = time.time();
//        ret = tuple(xn::strongly_connected_components(dg));
//        dt2 = time.time() - t
//        assert_less(dt2, dt * 2.3);  // should be 2 times longer for this graph
