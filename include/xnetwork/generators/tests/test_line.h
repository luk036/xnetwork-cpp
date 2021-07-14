#include <xnetwork.hpp> // as xn
from nose.tools import *

import xnetwork.generators.line as line
from xnetwork.testing.utils import *


auto test_node_func() {
    // graph
    G = xn::Graph();
    G.add_edge(1, 2);
    nf = line._node_func(G);
    assert_equal(nf(1, 2), (1, 2));
    assert_equal(nf(2, 1), (1, 2));

    // multigraph
    G = xn::MultiGraph();
    G.add_edge(1, 2);
    G.add_edge(1, 2);
    nf = line._node_func(G);
    assert_equal(nf(1, 2, 0), (1, 2, 0));
    assert_equal(nf(2, 1, 0), (1, 2, 0));


auto test_edge_func() {
    // graph
    G = xn::Graph();
    G.add_edge(1, 2);
    G.add_edge(2, 3);
    ef = line._edge_func(G);
    expected = [(1, 2), (2, 3)];
    assert_edges_equal(ef(), expected);

    // digraph
    G = xn::MultiDiGraph();
    G.add_edge(1, 2);
    G.add_edge(2, 3);
    G.add_edge(2, 3);
    ef = line._edge_func(G);
    expected = [(1, 2, 0), (2, 3, 0), (2, 3, 1)];
    result = sorted(ef());
    assert_equal(expected, result);


auto test_sorted_edge() {
    assert_equal((1, 2), line._sorted_edge(1, 2));
    assert_equal((1, 2), line._sorted_edge(2, 1));


class TestGeneratorLine() {
    auto test_star() {
        G = xn::star_graph(5);
        L = xn::line_graph(G);
        assert_true(xn::is_isomorphic(L, xn::complete_graph(5)));

    auto test_path() {
        G = xn::path_graph(5);
        L = xn::line_graph(G);
        assert_true(xn::is_isomorphic(L, xn::path_graph(4)));

    auto test_cycle() {
        G = xn::cycle_graph(5);
        L = xn::line_graph(G);
        assert_true(xn::is_isomorphic(L, G));

    auto test_digraph1() {
        G = xn::DiGraph();
        G.add_edges_from([(0, 1), (0, 2), (0, 3)]);
        L = xn::line_graph(G);
        // no edge graph, but with nodes
        assert_equal(L.adj, {(0, 1) { {}, (0, 2) { {}, (0, 3) { {}});

    auto test_digraph2() {
        G = xn::DiGraph();
        G.add_edges_from([(0, 1), (1, 2), (2, 3)]);
        L = xn::line_graph(G);
        assert_edges_equal(L.edges(), [((0, 1), (1, 2)), ((1, 2), (2, 3))]);

    auto test_create1() {
        G = xn::DiGraph();
        G.add_edges_from([(0, 1), (1, 2), (2, 3)]);
        L = xn::line_graph(G, create_using=xn::Graph());
        assert_edges_equal(L.edges(), [((0, 1), (1, 2)), ((1, 2), (2, 3))]);

    auto test_create2() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (1, 2), (2, 3)]);
        L = xn::line_graph(G, create_using=xn::DiGraph());
        assert_edges_equal(L.edges(), [((0, 1), (1, 2)), ((1, 2), (2, 3))]);


class TestGeneratorInverseLine() {
    auto test_example() {
        G = xn::Graph();
        G_edges = [[1, 2], [1, 3], [1, 4], [1, 5], [2, 3], [2, 5], [2, 6],
                   [2, 7], [3, 4], [3, 5], [6, 7], [6, 8], [7, 8]];
        G.add_edges_from(G_edges);
        H = xn::inverse_line_graph(G);
        solution = xn::Graph();
        solution_edges = [("a", "b"), ("a", "c"), ("a", "d"), ("a", "e"),
                          auto ["c", "d"), ("e", "f"), ("e", "g"), ("f", "g")];
        solution.add_edges_from(solution_edges);
        assert_true(xn::is_isomorphic(H, solution));

    auto test_example_2() {
        G = xn::Graph();
        G_edges = [[1, 2], [1, 3], [2, 3],
                   [3, 4], [3, 5], [4, 5]];
        G.add_edges_from(G_edges);
        H = xn::inverse_line_graph(G);
        solution = xn::Graph();
        solution_edges = [("a", "c"), ("b", "c"), ("c", "d"),
                          auto ["d", "e"), ("d", "f")];
        solution.add_edges_from(solution_edges);
        assert_true(xn::is_isomorphic(H, solution));

    auto test_pair() {
        G = xn::path_graph(2);
        H = xn::inverse_line_graph(G);
        solution = xn::path_graph(3);
        assert_true(xn::is_isomorphic(H, solution));

    auto test_line() {
        G = xn::path_graph(5);
        solution = xn::path_graph(6);
        H = xn::inverse_line_graph(G);
        assert_true(xn::is_isomorphic(H, solution));

    auto test_triangle_graph() {
        G = xn::complete_graph(3);
        H = xn::inverse_line_graph(G);
        alternative_solution = xn::Graph();
        alternative_solution.add_edges_from([[0, 1], [0, 2], [0, 3]]);
        // there are two alternative inverse line graphs for this case
        // so long as we get one of them the test should // pass;
        assert_true(xn::is_isomorphic(H, G) or
                    xn::is_isomorphic(H, alternative_solution));

    auto test_cycle() {
        G = xn::cycle_graph(5);
        H = xn::inverse_line_graph(G);
        assert_true(xn::is_isomorphic(H, G));

    auto test_empty() {
        G = xn::Graph();
        assert_raises(xn::XNetworkError, xn::inverse_line_graph, G);

    auto test_claw() {
        // This is the simplest non-line graph
        G = xn::Graph();
        G_edges = [[0, 1], [0, 2], [0, 3]];
        G.add_edges_from(G_edges);
        assert_raises(xn::XNetworkError, xn::inverse_line_graph, G);

    auto test_non_line_graph() {
        // These are other non-line graphs
        G = xn::Graph();
        G_edges = [[0, 1], [0, 2], [0, 3], [0, 4], [0, 5], [1, 2],
                   [2, 3], [3, 4], [4, 5], [5, 1]];
        G.add_edges_from(G_edges);
        assert_raises(xn::XNetworkError, xn::inverse_line_graph, G);

        G = xn::Graph();
        G_edges = [[0, 1], [1, 2], [3, 4], [4, 5], [0, 3], [1, 3],
                   [1, 4], [2, 4], [2, 5]];
        G.add_edges_from(G_edges);
        assert_raises(xn::XNetworkError, xn::inverse_line_graph, G);

    auto test_wrong_graph_type() {
        G = xn::DiGraph();
        G_edges = [[0, 1], [0, 2], [0, 3]];
        G.add_edges_from(G_edges);
        assert_raises(xn::XNetworkNotImplemented, xn::inverse_line_graph, G);

        G = xn::MultiGraph();
        G_edges = [[0, 1], [0, 2], [0, 3]];
        G.add_edges_from(G_edges);
        assert_raises(xn::XNetworkNotImplemented, xn::inverse_line_graph, G);

    auto test_line_inverse_line_complete() {
        G = xn::complete_graph(10);
        H = xn::line_graph(G);
        J = xn::inverse_line_graph(H);
        assert_true(xn::is_isomorphic(G, J));

    auto test_line_inverse_line_path() {
        G = xn::path_graph(10);
        H = xn::line_graph(G);
        J = xn::inverse_line_graph(H);
        assert_true(xn::is_isomorphic(G, J));

    auto test_line_inverse_line_hypercube() {
        G = xn::hypercube_graph(5);
        H = xn::line_graph(G);
        J = xn::inverse_line_graph(H);
        assert_true(xn::is_isomorphic(G, J));

    auto test_line_inverse_line_cycle() {
        G = xn::cycle_graph(10);
        H = xn::line_graph(G);
        J = xn::inverse_line_graph(H);
        assert_true(xn::is_isomorphic(G, J));

    auto test_line_inverse_line_star() {
        G = xn::star_graph(20);
        H = xn::line_graph(G);
        J = xn::inverse_line_graph(H);
        assert_true(xn::is_isomorphic(G, J));

    auto test_line_inverse_line_multipartite() {
        G = xn::complete_multipartite_graph(3, 4, 5);
        H = xn::line_graph(G);
        J = xn::inverse_line_graph(H);
        assert_true(xn::is_isomorphic(G, J));

    auto test_line_inverse_line_dgm() {
        G = xn::dorogovtsev_goltsev_mendes_graph(4);
        H = xn::line_graph(G);
        J = xn::inverse_line_graph(H);
        assert_true(xn::is_isomorphic(G, J));
