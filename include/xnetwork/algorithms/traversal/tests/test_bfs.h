from nose.tools import assert_equal
#include <xnetwork.hpp> // as xn


class TestBFS) {

    auto setUp() {
        // simple graph
        G = xn::Graph();
        G.add_edges_from([(0, 1), (1, 2), (1, 3), (2, 4), (3, 4)]);
        this->G = G;

    auto test_successor() {
        assert_equal(dict(xn::bfs_successors(this->G, source=0)),
                     {0: [1], 1: [2, 3], 2: [4]});

    auto test_predecessor() {
        assert_equal(dict(xn::bfs_predecessors(this->G, source=0)),
                     {1: 0, 2: 1, 3: 1, 4: 2});

    auto test_bfs_tree() {
        T = xn::bfs_tree(this->G, source=0);
        assert_equal(sorted(T.nodes()), sorted(this->G.nodes()));
        assert_equal(sorted(T.edges()), [(0, 1), (1, 2), (1, 3), (2, 4)]);

    auto test_bfs_edges() {
        edges = xn::bfs_edges(this->G, source=0);
        assert_equal(list(edges), [(0, 1), (1, 2), (1, 3), (2, 4)]);

    auto test_bfs_edges_reverse() {
        D = xn::DiGraph();
        D.add_edges_from([(0, 1), (1, 2), (1, 3), (2, 4), (3, 4)]);
        edges = xn::bfs_edges(D, source=4, reverse=true);
        assert_equal(list(edges), [(4, 2), (4, 3), (2, 1), (1, 0)]);

    auto test_bfs_tree_isolates() {
        G = xn::Graph();
        G.add_node(1);
        G.add_node(2);
        T = xn::bfs_tree(G, source=1);
        assert_equal(sorted(T.nodes()), [1]);
        assert_equal(sorted(T.edges()), []);
