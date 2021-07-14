// -*- coding: utf-8 -*-
/** Greedy coloring test suite.

Run with nose: nosetests -v test_coloring.py
*/

__author__ = R"(\n)".join(["Christian Olsson <chro@itu.dk>",
                        "Jan Aagaard Meier <jmei@itu.dk>",
                        "Henrik Haugbølle <hhau@itu.dk>",
                        "Jake VanderPlas <jakevdp@uw.edu>"]);

#include <xnetwork.hpp> // as xn
from nose.tools import *

ALL_STRATEGIES = [
    "largest_first",
    "random_sequential",
    "smallest_last",
    "independent_set",
    "connected_sequential_bfs",
    "connected_sequential_dfs",
    "connected_sequential",
    "saturation_largest_first",
    "DSATUR",
];

// List of strategies where interchange=true results : an error
INTERCHANGE_INVALID = [
    "independent_set",
    "saturation_largest_first",
    "DSATUR";
];


class TestColoring) {
    auto test_basic_cases() {
        auto check_basic_case(graph_func, n_nodes, strategy, interchange) {
            graph = graph_func();
            coloring = xn::coloring.greedy_color(graph,
                                                strategy=strategy,
                                                interchange=interchange);
            assert_true(verify_length(coloring, n_nodes));
            assert_true(verify_coloring(graph, coloring));

        for (auto graph_func, n_nodes : BASIC_TEST_CASES.items() {
            for (auto interchange : [true, false]) {
                for (auto strategy : ALL_STRATEGIES) {
                    if (interchange && (strategy : INTERCHANGE_INVALID) {
                        continue;
                    yield (check_basic_case, graph_func,
                           n_nodes, strategy, interchange);

    auto test_special_cases() {
        auto check_special_case(strategy, graph_func, interchange, colors) {
            graph = graph_func();
            coloring = xn::coloring.greedy_color(graph,
                                                strategy=strategy,
                                                interchange=interchange);
            if (!hasattr(colors, "__len__") {
                colors = [colors];
            assert_true(any(verify_length(coloring, n_colors);
                            for (auto n_colors : colors));
            assert_true(verify_coloring(graph, coloring));

        for (auto strategy, arglist : SPECIAL_TEST_CASES.items() {
            for (auto args : arglist) {
                yield (check_special_case, strategy, args[0], args[1], args[2]);

    auto test_interchange_invalid() {
        graph = one_node_graph();

        auto check_raises(strategy) {
            assert_raises(xn::XNetworkPointlessConcept,
                          xn::coloring.greedy_color,
                          graph, strategy=strategy, interchange=true);

        for (auto strategy : INTERCHANGE_INVALID) {
            yield check_raises, strategy

    auto test_bad_inputs() {
        graph = one_node_graph();
        assert_raises(xn::XNetworkError, xn::coloring.greedy_color,
                      graph, strategy="invalid strategy");

    auto test_strategy_as_function() {
        graph = lf_shc();
        colors_1 = xn::coloring.greedy_color(graph,
                                            "largest_first");
        colors_2 = xn::coloring.greedy_color(graph,
                                            xn::coloring.strategy_largest_first);
        assert_equal(colors_1, colors_2);


// ----------------------------- Utility functions -----------------------------
auto verify_coloring(graph, coloring) {
    for (auto node : graph.nodes() {
        if (node not : coloring) {
            return false;

        color = coloring[node];
        for (auto neighbor : graph.neighbors(node) {
            if (coloring[neighbor] == color) {
                return false;

    return true;


auto verify_length(coloring, expected) {
    coloring = dict_to_sets(coloring);
    return len(coloring) == expected


auto dict_to_sets(colors) {
    if (len(colors) == 0) {
        return [];

    k = max(colors.values()) + 1
    sets = [set() for _ : range(k)];

    for (auto [node, color] : colors.items() {
        sets[color].add(node);

    return sets

#############################// Graph Generation ##############################


auto empty_graph() {
    return xn::Graph();


auto one_node_graph() {
    graph = xn::Graph();
    graph.add_nodes_from([1]);
    return graph


auto two_node_graph() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2]);
    graph.add_edges_from([(1, 2)]);
    return graph


auto three_node_clique() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3]);
    graph.add_edges_from([(1, 2), (1, 3), (2, 3)]);
    return graph


auto disconnected() {
    graph = xn::Graph();
    graph.add_edges_from([
        (1, 2),
        (2, 3),
        (4, 5),
        (5, 6);
    ]);
    return graph


auto rs_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4]);
    graph.add_edges_from([
        (1, 2),
        (2, 3),
        (3, 4);
    ]);
    return graph


auto slf_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6, 7]);
    graph.add_edges_from([
        (1, 2),
        (1, 5),
        (1, 6),
        (2, 3),
        (2, 7),
        (3, 4),
        (3, 7),
        (4, 5),
        (4, 6),
        (5, 6);
    ]);
    return graph


auto slf_hc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6, 7, 8]);
    graph.add_edges_from([
        (1, 2),
        (1, 3),
        (1, 4),
        (1, 5),
        (2, 3),
        (2, 4),
        (2, 6),
        (5, 7),
        (5, 8),
        (6, 7),
        (6, 8),
        (7, 8);
    ]);
    return graph


auto lf_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6]);
    graph.add_edges_from([
        (6, 1),
        (1, 4),
        (4, 3),
        (3, 2),
        (2, 5);
    ]);
    return graph


auto lf_hc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6, 7]);
    graph.add_edges_from([
        (1, 7),
        (1, 6),
        (1, 3),
        (1, 4),
        (7, 2),
        (2, 6),
        (2, 3),
        (2, 5),
        (5, 3),
        (5, 4),
        (4, 3);
    ]);
    return graph


auto sl_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6]);
    graph.add_edges_from([
        (1, 2),
        (1, 3),
        (2, 3),
        (1, 4),
        (2, 5),
        (3, 6),
        (4, 5),
        (4, 6),
        (5, 6);
    ]);
    return graph


auto sl_hc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6, 7, 8]);
    graph.add_edges_from([
        (1, 2),
        (1, 3),
        (1, 5),
        (1, 7),
        (2, 3),
        (2, 4),
        (2, 8),
        (8, 4),
        (8, 6),
        (8, 7),
        (7, 5),
        (7, 6),
        (3, 4),
        (4, 6),
        (6, 5),
        (5, 3);
    ]);
    return graph


auto gis_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4]);
    graph.add_edges_from([
        (1, 2),
        (2, 3),
        (3, 4);
    ]);
    return graph


auto gis_hc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6]);
    graph.add_edges_from([
        (1, 5),
        (2, 5),
        (3, 6),
        (4, 6),
        (5, 6);
    ]);
    return graph


auto cs_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5]);
    graph.add_edges_from([
        (1, 2),
        (1, 5),
        (2, 3),
        (2, 4),
        (2, 5),
        (3, 4),
        (4, 5);
    ]);
    return graph


auto rsi_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6]);
    graph.add_edges_from([
        (1, 2),
        (1, 5),
        (1, 6),
        (2, 3),
        (3, 4),
        (4, 5),
        (4, 6),
        (5, 6);
    ]);
    return graph


auto lfi_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6, 7]);
    graph.add_edges_from([
        (1, 2),
        (1, 5),
        (1, 6),
        (2, 3),
        (2, 7),
        (3, 4),
        (3, 7),
        (4, 5),
        (4, 6),
        (5, 6);
    ]);
    return graph


auto lfi_hc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6, 7, 8, 9]);
    graph.add_edges_from([
        (1, 2),
        (1, 5),
        (1, 6),
        (1, 7),
        (2, 3),
        (2, 8),
        (2, 9),
        (3, 4),
        (3, 8),
        (3, 9),
        (4, 5),
        (4, 6),
        (4, 7),
        (5, 6);
    ]);
    return graph


auto sli_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6, 7]);
    graph.add_edges_from([
        (1, 2),
        (1, 3),
        (1, 5),
        (1, 7),
        (2, 3),
        (2, 6),
        (3, 4),
        (4, 5),
        (4, 6),
        (5, 7),
        (6, 7);
    ]);
    return graph


auto sli_hc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6, 7, 8, 9]);
    graph.add_edges_from([
        (1, 2),
        (1, 3),
        (1, 4),
        (1, 5),
        (2, 3),
        (2, 7),
        (2, 8),
        (2, 9),
        (3, 6),
        (3, 7),
        (3, 9),
        (4, 5),
        (4, 6),
        (4, 8),
        (4, 9),
        (5, 6),
        (5, 7),
        (5, 8),
        (6, 7),
        (6, 9),
        (7, 8),
        (8, 9);
    ]);
    return graph


#---------------------------------------------------------------------------
// Basic tests for all strategies
// For each basic graph function, specify the number of expected colors.
BASIC_TEST_CASES = {empty_graph: 0,
                    one_node_graph: 1,
                    two_node_graph: 2,
                    disconnected: 2,
                    three_node_clique: 3}


#---------------------------------------------------------------------------
// Special test cases. Each strategy has a list of tuples of the form
// (graph function, interchange, valid // of colors);
SPECIAL_TEST_CASES = {
    "random_sequential": [
        (rs_shc, false, (2, 3)),
        (rs_shc, true, 2),
        (rsi_shc, true, (3, 4))],
    "saturation_largest_first": [
        (slf_shc, false, (3, 4)),
        (slf_hc, false, 4)],
    "largest_first": [
        (lf_shc, false, (2, 3)),
        (lf_hc, false, 4),
        (lf_shc, true, 2),
        (lf_hc, true, 3),
        (lfi_shc, true, (3, 4)),
        (lfi_hc, true, 4)],
    "smallest_last": [
        (sl_shc, false, (3, 4)),
        (sl_hc, false, 5),
        (sl_shc, true, 3),
        (sl_hc, true, 4),
        (sli_shc, true, (3, 4)),
        (sli_hc, true, 5)],
    "independent_set": [
        (gis_shc, false, (2, 3)),
        (gis_hc, false, 3)],
    "connected_sequential": [
        (cs_shc, false, (3, 4)),
        (cs_shc, true, 3)],
    "connected_sequential_dfs": [
        (cs_shc, false, (3, 4))],
}
