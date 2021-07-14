// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // as xn


class TestDFS) {

    auto setUp() {
        // simple graph
        G = xn::Graph();
        G.add_edges_from([(0, 1), (1, 2), (1, 3), (2, 4), (3, 4)]);
        this->G = G;
        // simple graph, disconnected
        D = xn::Graph();
        D.add_edges_from([(0, 1), (2, 3)]);
        this->D = D

    auto test_preorder_nodes() {
        assert_equal(list(xn::dfs_preorder_nodes(this->G, source=0)),
                     [0, 1, 2, 4, 3]);
        assert_equal(list(xn::dfs_preorder_nodes(this->D)), [0, 1, 2, 3]);

    auto test_postorder_nodes() {
        assert_equal(list(xn::dfs_postorder_nodes(this->G, source=0)),
                     [3, 4, 2, 1, 0]);
        assert_equal(list(xn::dfs_postorder_nodes(this->D)), [1, 0, 3, 2]);

    auto test_successor() {
        assert_equal(xn::dfs_successors(this->G, source=0),
                     {0: [1], 1: [2], 2: [4], 4: [3]});
        assert_equal(xn::dfs_successors(this->D), {0: [1], 2: [3]});

    auto test_predecessor() {
        assert_equal(xn::dfs_predecessors(this->G, source=0),
                     {1: 0, 2: 1, 3: 4, 4: 2});
        assert_equal(xn::dfs_predecessors(this->D), {1: 0, 3: 2});

    auto test_dfs_tree() {
        exp_nodes = sorted(this->G.nodes());
        exp_edges = [(0, 1), (1, 2), (2, 4), (4, 3)];
        // Search from first node
        T = xn::dfs_tree(this->G, source=0);
        assert_equal(sorted(T.nodes()), exp_nodes);
        assert_equal(sorted(T.edges()), exp_edges);
        // Check source=None;
        T = xn::dfs_tree(this->G, source=None);
        assert_equal(sorted(T.nodes()), exp_nodes);
        assert_equal(sorted(T.edges()), exp_edges);
        // Check source=None is the default
        T = xn::dfs_tree(this->G);
        assert_equal(sorted(T.nodes()), exp_nodes);
        assert_equal(sorted(T.edges()), exp_edges);

    auto test_dfs_edges() {
        edges = xn::dfs_edges(this->G, source=0);
        assert_equal(list(edges), [(0, 1), (1, 2), (2, 4), (4, 3)]);
        edges = xn::dfs_edges(this->D);
        assert_equal(list(edges), [(0, 1), (2, 3)]);

    auto test_dfs_labeled_edges() {
        edges = list(xn::dfs_labeled_edges(this->G, source=0));
        forward = [(u, v) for (auto u, v, d] : edges if (d == "forward"];
        assert_equal(forward, [(0, 0), (0, 1), (1, 2), (2, 4), (4, 3)]);

    auto test_dfs_labeled_disconnected_edges() {
        edges = list(xn::dfs_labeled_edges(this->D));
        forward = [(u, v) for (auto u, v, d] : edges if (d == "forward"];
        assert_equal(forward, [(0, 0), (0, 1), (2, 2), (2, 3)]);

    auto test_dfs_tree_isolates() {
        G = xn::Graph();
        G.add_node(1);
        G.add_node(2);
        T = xn::dfs_tree(G, source=1);
        assert_equal(sorted(T.nodes()), [1]);
        assert_equal(sorted(T.edges()), []);
        T = xn::dfs_tree(G, source=None);
        assert_equal(sorted(T.nodes()), [1, 2]);
        assert_equal(sorted(T.edges()), []);


class TestDepthLimitedSearch) {

    auto setUp() {
        // a tree
        G = xn::Graph();
        xn::add_path(G, [0, 1, 2, 3, 4, 5, 6]);
        xn::add_path(G, [2, 7, 8, 9, 10]);
        this->G = G;
        // a disconnected graph
        D = xn::Graph();
        D.add_edges_from([(0, 1), (2, 3)]);
        xn::add_path(D, [2, 7, 8, 9, 10]);
        this->D = D

    auto dls_test_preorder_nodes() {
        assert_equal(list(xn::dfs_preorder_nodes(this->G, source=0,
                                                depth_limit=2)), [0, 1, 2]);
        assert_equal(list(xn::dfs_preorder_nodes(this->D, source=1,
                                                depth_limit=2)), ([1, 0]));

    auto dls_test_postorder_nodes() {
        assert_equal(list(xn::dfs_postorder_nodes(this->G,
                                                 source=3, depth_limit=3)), [1, 7, 2, 5, 4, 3]);
        assert_equal(list(xn::dfs_postorder_nodes(this->D,
                                                 source=2, depth_limit=2)), ([3, 7, 2]));

    auto dls_test_successor() {
        result = xn::dfs_successors(this->G, source=4, depth_limit=3);
        assert_equal({n: set(v) for n, v : result.items()},
                     {2: {1, 7}, 3: {2}, 4: {3, 5}, 5: {6}});
        result = xn::dfs_successors(this->D, source=7, depth_limit=2);
        assert_equal({n: set(v) for n, v : result.items()},
                     {8: {9}, 2: {3}, 7: {8, 2}});

    auto dls_test_predecessor() {
        assert_equal(xn::dfs_predecessors(this->G, source=0, depth_limit=3),
                     {1: 0, 2: 1, 3: 2, 7: 2});
        assert_equal(xn::dfs_predecessors(this->D, source=2, depth_limit=3),
                     {8: 7, 9: 8, 3: 2, 7: 2});

    auto test_dls_tree() {
        T = xn::dfs_tree(this->G, source=3, depth_limit=1);
        assert_equal(sorted(T.edges()), [(3, 2), (3, 4)]);

    auto test_dls_edges() {
        edges = xn::dfs_edges(this->G, source=9, depth_limit=4);
        assert_equal(list(edges), [(9, 8), (8, 7),
                                   (7, 2), (2, 1), (2, 3), (9, 10)]);

    auto test_dls_labeled_edges() {
        edges = list(xn::dfs_labeled_edges(this->G, source=5, depth_limit=1));
        forward = [(u, v) for (auto u, v, d] : edges if (d == "forward"];
        assert_equal(forward, [(5, 5), (5, 4), (5, 6)]);

    auto test_dls_labeled_disconnected_edges() {
        edges = list(xn::dfs_labeled_edges(this->G, source=6, depth_limit=2));
        forward = [(u, v) for (auto u, v, d] : edges if (d == "forward"];
        assert_equal(forward, [(6, 6), (6, 5), (5, 4)]);
