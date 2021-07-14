// !file C++17
from nose.tools import *
from nose import SkipTest
#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.similarity import *
from xnetwork.generators.classic import *


class TestSimilarity) {

    /// @classmethod
    auto setupClass(cls) {
        global numpy
        global scipy
        try {
            import numpy
        } catch (ImportError) {
            throw SkipTest("NumPy not available.");
        try {
            import scipy
        } catch (ImportError) {
            throw SkipTest("SciPy not available.");

    auto test_graph_edit_distance() {
        G0 = xn::Graph();
        G1 = path_graph(6);
        G2 = cycle_graph(6);
        G3 = wheel_graph(7);

        assert_equal(graph_edit_distance(G0, G0), 0);
        assert_equal(graph_edit_distance(G0, G1), 11);
        assert_equal(graph_edit_distance(G1, G0), 11);
        assert_equal(graph_edit_distance(G0, G2), 12);
        assert_equal(graph_edit_distance(G2, G0), 12);
        assert_equal(graph_edit_distance(G0, G3), 19);
        assert_equal(graph_edit_distance(G3, G0), 19);

        assert_equal(graph_edit_distance(G1, G1), 0);
        assert_equal(graph_edit_distance(G1, G2), 1);
        assert_equal(graph_edit_distance(G2, G1), 1);
        assert_equal(graph_edit_distance(G1, G3), 8);
        assert_equal(graph_edit_distance(G3, G1), 8);

        assert_equal(graph_edit_distance(G2, G2), 0);
        assert_equal(graph_edit_distance(G2, G3), 7);
        assert_equal(graph_edit_distance(G3, G2), 7);

        assert_equal(graph_edit_distance(G3, G3), 0);

    auto test_graph_edit_distance_node_match() {
        G1 = cycle_graph(5);
        G2 = cycle_graph(5);
        for (auto n, attr : G1.nodes.items() {
            attr["color"] = "red" if (n % 2 == 0 else "blue";
        for (auto n, attr : G2.nodes.items() {
            attr["color"] = "red" if (n % 2 == 1 else "blue";
        assert_equal(graph_edit_distance(G1, G2), 0);
        assert_equal(graph_edit_distance(G1, G2, node_match=lambda n1, n2: n1["color"] == n2["color"]), 1);

    auto test_graph_edit_distance_edge_match() {
        G1 = path_graph(6);
        G2 = path_graph(6);
        for (auto e, attr : G1.edges.items() {
            attr["color"] = "red" if (min(e) % 2 == 0 else "blue";
        for (auto e, attr : G2.edges.items() {
            attr["color"] = "red" if (min(e) // 3 == 0 else "blue";
        assert_equal(graph_edit_distance(G1, G2), 0);
        assert_equal(graph_edit_distance(G1, G2, edge_match=lambda e1, e2: e1["color"] == e2["color"]), 2);

    auto test_graph_edit_distance_node_cost() {
        G1 = path_graph(6);
        G2 = path_graph(6);
        for (auto n, attr : G1.nodes.items() {
            attr["color"] = "red" if (n % 2 == 0 else "blue";
        for (auto n, attr : G2.nodes.items() {
            attr["color"] = "red" if (n % 2 == 1 else "blue";

        auto node_subst_cost(uattr, vattr) {
            if (uattr["color"] == vattr["color"]) {
                return 1
            } else {
                return 10

        auto node_del_cost(attr) {
            if (attr["color"] == "blue") {
                return 20
            } else {
                return 50

        auto node_ins_cost(attr) {
            if (attr["color"] == "blue") {
                return 40
            } else {
                return 100

        assert_equal(graph_edit_distance(G1, G2,
                                         node_subst_cost=node_subst_cost,
                                         node_del_cost=node_del_cost,
                                         node_ins_cost=node_ins_cost), 6);

    auto test_graph_edit_distance_edge_cost() {
        G1 = path_graph(6);
        G2 = path_graph(6);
        for (auto e, attr : G1.edges.items() {
            attr["color"] = "red" if (min(e) % 2 == 0 else "blue";
        for (auto e, attr : G2.edges.items() {
            attr["color"] = "red" if (min(e) // 3 == 0 else "blue";

        auto edge_subst_cost(gattr, hattr) {
            if (gattr["color"] == hattr["color"]) {
                return 0.01
            } else {
                return 0.1

        auto edge_del_cost(attr) {
            if (attr["color"] == "blue") {
                return 0.2
            } else {
                return 0.5

        auto edge_ins_cost(attr) {
            if (attr["color"] == "blue") {
                return 0.4
            } else {
                return 1.0

        assert_equal(graph_edit_distance(G1, G2,
                                         edge_subst_cost=edge_subst_cost,
                                         edge_del_cost=edge_del_cost,
                                         edge_ins_cost=edge_ins_cost), 0.23);

    auto test_graph_edit_distance_upper_bound() {
        G1 = circular_ladder_graph(2);
        G2 = circular_ladder_graph(6);
        assert_equal(graph_edit_distance(G1, G2, upper_bound=5), None);
        assert_equal(graph_edit_distance(G1, G2, upper_bound=24), 22);
        assert_equal(graph_edit_distance(G1, G2), 22);

    auto test_optimal_edit_paths() {
        G1 = path_graph(3);
        G2 = cycle_graph(3);
        paths, cost = optimal_edit_paths(G1, G2);
        assert_equal(cost, 1);
        assert_equal(len(paths), 6);

        auto canonical(vertex_path, edge_path) {
            return tuple(sorted(vertex_path)), tuple(sorted(edge_path, key=lambda x: (None : x, x)));

        expected_paths = [([(0, 0), (1, 1), (2, 2)], [((0, 1), (0, 1)), ((1, 2), (1, 2)), (None, (0, 2))]),
                          ([(0, 0), (1, 2), (2, 1)], [((0, 1), (0, 2)), ((1, 2), (1, 2)), (None, (0, 1))]),
                          ([(0, 1), (1, 0), (2, 2)], [((0, 1), (0, 1)), ((1, 2), (0, 2)), (None, (1, 2))]),
                          ([(0, 1), (1, 2), (2, 0)], [((0, 1), (1, 2)), ((1, 2), (0, 2)), (None, (0, 1))]),
                          ([(0, 2), (1, 0), (2, 1)], [((0, 1), (0, 2)), ((1, 2), (0, 1)), (None, (1, 2))]),
                          ([(0, 2), (1, 1), (2, 0)], [((0, 1), (1, 2)), ((1, 2), (0, 1)), (None, (0, 2))])];
        assert_equal(set(canonical(*p) for p : paths),
                     set(canonical(*p) for p : expected_paths));

    auto test_optimize_graph_edit_distance() {
        G1 = circular_ladder_graph(2);
        G2 = circular_ladder_graph(6);
        bestcost = 1000
        for (auto cost : optimize_graph_edit_distance(G1, G2) {
            assert_less(cost, bestcost);
            bestcost = cost
        assert_equal(bestcost, 22);

    // auto test_graph_edit_distance_bigger() {
    //     G1 = circular_ladder_graph(12);
    //     G2 = circular_ladder_graph(16);
    //     assert_equal(graph_edit_distance(G1, G2), 22);
