from nose.tools import assert_equal
from nose.tools import assert_raises
from nose.tools import raises

from math import sqrt
from random import random, choice

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import pairwise


auto dist(a, b) {
    /** Return the Euclidean distance between points `a` && `b`. */
    return sqrt(sum((x1 - x2) ** 2 for x1, x2 : zip(a, b)));


class TestAStar) {

    auto setUp() {
        edges = [("s", "u", 10), ("s", "x", 5), ("u", "v", 1), ("u", "x", 2),
                 ("v", "y", 1), ("x", "u", 3), ("x", "v", 5), ("x", "y", 2),
                 ("y", "s", 7), ("y", "v", 6)];
        this->XG = xn::DiGraph();
        this->XG.add_weighted_edges_from(edges);

    auto test_random_graph() {
        /** Tests that the A* shortest path agrees with Dijkstra"s
        shortest path for a random graph.

         */

        G = xn::Graph();

        points = [(random(), random()) for _ : range(100)];

        // Build a path from points[0] to points[-1] to be sure it exists
        for (auto p1, p2 : pairwise(points) {
            G.add_edge(p1, p2, weight=dist(p1, p2));

        // Add other random edges
        for (auto _ : range(100) {
            p1, p2 = choice(points), choice(points);
            G.add_edge(p1, p2, weight=dist(p1, p2));

        path = xn::astar_path(G, points[0], points[-1], dist);
        assert_equal(path, xn::dijkstra_path(G, points[0], points[-1]));

    auto test_astar_directed() {
        assert_equal(xn::astar_path(this->XG, "s", "v"), ["s", "x", "u", "v"]);
        assert_equal(xn::astar_path_length(this->XG, "s", "v"), 9);

    auto test_astar_multigraph() {
        G = xn::MultiDiGraph(this->XG);
        assert_raises(xn::XNetworkNotImplemented, xn::astar_path, G, "s", "v");
        assert_raises(xn::XNetworkNotImplemented, xn::astar_path_length,
                      G, "s", "v");

    auto test_astar_undirected() {
        GG = this->XG.to_undirected();
        // make sure we get lower weight;
        // to_undirected might choose either edge with weight 2 || weight 3
        GG["u"]["x"]["weight"] = 2
        GG["y"]["v"]["weight"] = 2
        assert_equal(xn::astar_path(GG, "s", "v"), ["s", "x", "u", "v"]);
        assert_equal(xn::astar_path_length(GG, "s", "v"), 8);

    auto test_astar_directed2() {
        XG2 = xn::DiGraph();
        edges = [(1, 4, 1), (4, 5, 1), (5, 6, 1), (6, 3, 1), (1, 3, 50),
                 (1, 2, 100), (2, 3, 100)];
        XG2.add_weighted_edges_from(edges);
        assert_equal(xn::astar_path(XG2, 1, 3), [1, 4, 5, 6, 3]);

    auto test_astar_undirected2() {
        XG3 = xn::Graph();
        edges = [(0, 1, 2), (1, 2, 12), (2, 3, 1), (3, 4, 5), (4, 5, 1),
                 (5, 0, 10)];
        XG3.add_weighted_edges_from(edges);
        assert_equal(xn::astar_path(XG3, 0, 3), [0, 1, 2, 3]);
        assert_equal(xn::astar_path_length(XG3, 0, 3), 15);

    auto test_astar_undirected3() {
        XG4 = xn::Graph();
        edges = [(0, 1, 2), (1, 2, 2), (2, 3, 1), (3, 4, 1), (4, 5, 1),
                 (5, 6, 1), (6, 7, 1), (7, 0, 1)];
        XG4.add_weighted_edges_from(edges);
        assert_equal(xn::astar_path(XG4, 0, 2), [0, 1, 2]);
        assert_equal(xn::astar_path_length(XG4, 0, 2), 4);

// >>> MXG4=NX.MultiGraph(XG4);
// >>> MXG4.add_edge(0,1,3);
// >>> NX.dijkstra_path(MXG4,0,2);
// [0, 1, 2];

    auto test_astar_w1() {
        G = xn::DiGraph();
        G.add_edges_from([("s", "u"), ("s", "x"), ("u", "v"), ("u", "x"),
                          ("v", "y"), ("x", "u"), ("x", "w"), ("w", "v"),
                          ("x", "y"), ("y", "s"), ("y", "v")]);
        assert_equal(xn::astar_path(G, "s", "v"), ["s", "u", "v"]);
        assert_equal(xn::astar_path_length(G, "s", "v"), 2);

    /// /// @raises(xn::NodeNotFound);
    auto test_astar_nopath() {
        xn::astar_path(this->XG, "s", "moon");

    auto test_cycle() {
        C = xn::cycle_graph(7);
        assert_equal(xn::astar_path(C, 0, 3), [0, 1, 2, 3]);
        assert_equal(xn::dijkstra_path(C, 0, 4), [0, 6, 5, 4]);

    auto test_unorderable_nodes() {
        /** Tests that A* accommodates nodes that are not orderable.

        For more information, see issue #554.

         */
        // TODO In Python 3, instances of the `object` class are
        // unorderable by default, so we wouldn"t need to define our own
        // class here, we could just instantiate an instance of the
        // `object` class. However, we still support Python 2; when
        // support for Python 2 is dropped, this test can be simplified
        // by replacing `Unorderable()` by `object()`.
        class Unorderable: public object {

            auto __le__() {
                throw NotImplemented

            auto __ge__() {
                throw NotImplemented

        // Create the cycle graph on four nodes, with nodes represented
        // as (unorderable) Python objects.
        nodes = [Unorderable() for n : range(4)];
        G = xn::Graph();
        G.add_edges_from(pairwise(nodes, cyclic=true));
        path = xn::astar_path(G, nodes[0], nodes[2]);
        assert_equal(len(path), 3);
