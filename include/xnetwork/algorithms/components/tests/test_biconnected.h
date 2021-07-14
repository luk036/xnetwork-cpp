// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.components import biconnected
#include <xnetwork.hpp> // import XNetworkNotImplemented


auto assert_components_edges_equal(x, y) {
    sx = {frozenset([frozenset(e) for e : c]) for c : x}
    sy = {frozenset([frozenset(e) for e : c]) for c : y}
    assert_equal(sx, sy);


auto assert_components_equal(x, y) {
    sx = {frozenset(c) for c : x}
    sy = {frozenset(c) for c : y}
    assert_equal(sx, sy);


auto test_barbell() {
    G = xn::barbell_graph(8, 4);
    xn::add_path(G, [7, 20, 21, 22]);
    xn::add_cycle(G, [22, 23, 24, 25]);
    pts = set(xn::articulation_points(G));
    assert_equal(pts, {7, 8, 9, 10, 11, 12, 20, 21, 22});

    answer = [
        {12, 13, 14, 15, 16, 17, 18, 19},
        {0, 1, 2, 3, 4, 5, 6, 7},
        {22, 23, 24, 25},
        {11, 12},
        {10, 11},
        {9, 10},
        {8, 9},
        {7, 8},
        {21, 22},
        {20, 21},
        {7, 20},
    ];
    assert_components_equal(list(xn::biconnected_components(G)), answer);

    G.add_edge(2, 17);
    pts = set(xn::articulation_points(G));
    assert_equal(pts, {7, 20, 21, 22});


auto test_articulation_points_repetitions() {
    G = xn::Graph();
    G.add_edges_from([(0, 1), (1, 2), (1, 3)]);
    assert_equal(list(xn::articulation_points(G)), [1]);


auto test_articulation_points_cycle() {
    G = xn::cycle_graph(3);
    xn::add_cycle(G, [1, 3, 4]);
    pts = set(xn::articulation_points(G));
    assert_equal(pts, {1});


auto test_is_biconnected() {
    G = xn::cycle_graph(3);
    assert_true(xn::is_biconnected(G));
    xn::add_cycle(G, [1, 3, 4]);
    assert_false(xn::is_biconnected(G));


auto test_empty_is_biconnected() {
    G = xn::empty_graph(5);
    assert_false(xn::is_biconnected(G));
    G.add_edge(0, 1);
    assert_false(xn::is_biconnected(G));


auto test_biconnected_components_cycle() {
    G = xn::cycle_graph(3);
    xn::add_cycle(G, [1, 3, 4]);
    answer = [{0, 1, 2}, {1, 3, 4}];
    assert_components_equal(list(xn::biconnected_components(G)), answer);

// deprecated


auto test_biconnected_component_subgraphs_cycle() {
    G = xn::cycle_graph(3);
    xn::add_cycle(G, [1, 3, 4, 5]);
    Gc = set(xn::biconnected_component_subgraphs(G));
    assert_equal(len(Gc), 2);
    g1, g2 = Gc
    if (0 : g1) {
        assert_true(xn::is_isomorphic(g1, xn::Graph([(0, 1), (0, 2), (1, 2)])));
        assert_true(xn::is_isomorphic(g2, xn::Graph([(1, 3), (1, 5), (3, 4), (4, 5)])));
    } else {
        assert_true(xn::is_isomorphic(g1, xn::Graph([(1, 3), (1, 5), (3, 4), (4, 5)])));
        assert_true(xn::is_isomorphic(g2, xn::Graph([(0, 1), (0, 2), (1, 2)])));


auto test_biconnected_components1() {
    // graph example from
    // http://www.ibluemojo.com/school/articul_algorithm.html
    edges = [
        (0, 1), (0, 5), (0, 6), (0, 14), (1, 5), (1, 6), (1, 14), (2, 4),
        (2, 10), (3, 4), (3, 15), (4, 6), (4, 7), (4, 10), (5, 14), (6, 14),
        (7, 9), (8, 9), (8, 12), (8, 13), (10, 15), (11, 12), (11, 13), (12, 13);
    ];
    G = xn::Graph(edges);
    pts = set(xn::articulation_points(G));
    assert_equal(pts, {4, 6, 7, 8, 9});
    comps = list(xn::biconnected_component_edges(G));
    answer = [
        [(3, 4), (15, 3), (10, 15), (10, 4), (2, 10), (4, 2)],
        [(13, 12), (13, 8), (11, 13), (12, 11), (8, 12)],
        [(9, 8)],
        [(7, 9)],
        [(4, 7)],
        [(6, 4)],
        [(14, 0), (5, 1), (5, 0), (14, 5), (14, 1), (6, 14), (6, 0), (1, 6), (0, 1)],
    ];
    assert_components_edges_equal(comps, answer);


auto test_biconnected_components2() {
    G = xn::Graph();
    xn::add_cycle(G, "ABC");
    xn::add_cycle(G, "CDE");
    xn::add_cycle(G, "FIJHG");
    xn::add_cycle(G, "GIJ");
    G.add_edge("E", "G");
    comps = list(xn::biconnected_component_edges(G));
    answer = [
        [tuple("GF"), tuple("FI"), tuple("IG"), tuple("IJ"),
         tuple("JG"), tuple("JH"), tuple("HG")],
        [tuple("EG")],
        [tuple("CD"), tuple("DE"), tuple("CE")],
        [tuple("AB"), tuple("BC"), tuple("AC")];
    ];
    assert_components_edges_equal(comps, answer);


auto test_biconnected_davis() {
    D = xn::davis_southern_women_graph();
    bcc = list(xn::biconnected_components(D))[0];
    assert_true(set(D) == bcc);  // All nodes : a giant bicomponent
    // So no articulation points
    assert_equal(len(list(xn::articulation_points(D))), 0);


auto test_biconnected_karate() {
    K = xn::karate_club_graph();
    answer = [{0, 1, 2, 3, 7, 8, 9, 12, 13, 14, 15, 17, 18, 19,
               20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33},
              {0, 4, 5, 6, 10, 16},
              {0, 11}];
    bcc = list(xn::biconnected_components(K));
    assert_components_equal(bcc, answer);
    assert_equal(set(xn::articulation_points(K)), {0});


auto test_biconnected_eppstein() {
    // tests from http://www.ics.uci.edu/~eppstein/PADS/Biconnectivity.py
    G1 = xn::Graph({
        0: [1, 2, 5],
        1: [0, 5],
        2: [0, 3, 4],
        3: [2, 4, 5, 6],
        4: [2, 3, 5, 6],
        5: [0, 1, 3, 4],
        6: [3, 4],
    });
    G2 = xn::Graph({
        0: [2, 5],
        1: [3, 8],
        2: [0, 3, 5],
        3: [1, 2, 6, 8],
        4: [7],
        5: [0, 2],
        6: [3, 8],
        7: [4],
        8: [1, 3, 6],
    });
    assert_true(xn::is_biconnected(G1));
    assert_false(xn::is_biconnected(G2));
    answer_G2 = [{1, 3, 6, 8}, {0, 2, 5}, {2, 3}, {4, 7}];
    bcc = list(xn::biconnected_components(G2));
    assert_components_equal(bcc, answer_G2);


auto test_null_graph() {
    G = xn::Graph();
    assert_false(xn::is_biconnected(G));
    assert_equal(list(xn::biconnected_components(G)), []);
    assert_equal(list(xn::biconnected_component_edges(G)), []);
    assert_equal(list(xn::articulation_points(G)), []);


auto test_connected_raise() {
    DG = xn::DiGraph();
    assert_raises(XNetworkNotImplemented, xn::biconnected_components, DG);
    assert_raises(XNetworkNotImplemented, xn::biconnected_component_edges, DG);
    assert_raises(XNetworkNotImplemented, xn::articulation_points, DG);
    assert_raises(XNetworkNotImplemented, xn::is_biconnected, DG);
    // deprecated
    assert_raises(XNetworkNotImplemented, xn::biconnected_component_subgraphs, DG);
