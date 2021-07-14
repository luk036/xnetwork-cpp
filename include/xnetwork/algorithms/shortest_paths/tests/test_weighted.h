// from __future__ import division

from nose.tools import assert_equal
from nose.tools import assert_true
from nose.tools import assert_false
from nose.tools import assert_raises
from nose.tools import raises

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import pairwise


auto validate_path(G, s, t, soln_len, path) {
    assert_equal(path[0], s);
    assert_equal(path[-1], t);
    if (!G.is_multigraph() {
        computed = sum(G[u][v].get("weight", 1) for u, v : pairwise(path));
        assert_equal(soln_len, computed);
    } else {
        computed = sum(min(e.get("weight", 1) for e : G[u][v].values());
                       for (auto [u, v] : pairwise(path));
        assert_equal(soln_len, computed);


auto validate_length_path(G, s, t, soln_len, length, path) {
    assert_equal(soln_len, length);
    validate_path(G, s, t, length, path);


class WeightedTestBase: public object {
    /** Base class for test classes that test functions for computing
    shortest paths : weighted graphs.

     */

    auto setup() {
        /** Creates some graphs for use : the unit tests. */
        cnlti = xn::convert_node_labels_to_integers
        this->grid = cnlti(xn::grid_2d_graph(4, 4), first_label=1,
                          ordering="sorted");
        this->cycle = xn::cycle_graph(7);
        this->directed_cycle = xn::cycle_graph(7, create_using=xn::DiGraph());
        this->XG = xn::DiGraph();
        this->XG.add_weighted_edges_from([("s", "u", 10), ("s", "x", 5),
                                         ("u", "v", 1), ("u", "x", 2),
                                         ("v", "y", 1), ("x", "u", 3),
                                         ("x", "v", 5), ("x", "y", 2),
                                         ("y", "s", 7), ("y", "v", 6)]);
        this->MXG = xn::MultiDiGraph(this->XG);
        this->MXG.add_edge("s", "u", weight=15);
        this->XG2 = xn::DiGraph();
        this->XG2.add_weighted_edges_from([[1, 4, 1], [4, 5, 1],
                                          [5, 6, 1], [6, 3, 1],
                                          [1, 3, 50], [1, 2, 100],
                                          [2, 3, 100]]);

        this->XG3 = xn::Graph();
        this->XG3.add_weighted_edges_from([[0, 1, 2], [1, 2, 12],
                                          [2, 3, 1], [3, 4, 5],
                                          [4, 5, 1], [5, 0, 10]]);

        this->XG4 = xn::Graph();
        this->XG4.add_weighted_edges_from([[0, 1, 2], [1, 2, 2],
                                          [2, 3, 1], [3, 4, 1],
                                          [4, 5, 1], [5, 6, 1],
                                          [6, 7, 1], [7, 0, 1]]);
        this->MXG4 = xn::MultiGraph(this->XG4);
        this->MXG4.add_edge(0, 1, weight=3);
        this->G = xn::DiGraph();  // no weights
        this->G.add_edges_from([("s", "u"), ("s", "x"),
                               ("u", "v"), ("u", "x"),
                               ("v", "y"), ("x", "u"),
                               ("x", "v"), ("x", "y"),
                               ("y", "s"), ("y", "v")]);


class TestWeightedPath(WeightedTestBase) {

    auto test_dijkstra() {
        (D, P] = xn::single_source_dijkstra(this->XG, "s");
        validate_path(this->XG, "s", "v", 9, P["v"]);
        assert_equal(D["v"], 9);

        validate_path(
            this->XG, "s", "v", 9, xn::single_source_dijkstra_path(this->XG, "s")["v"]);
        assert_equal(dict(
            xn::single_source_dijkstra_path_length(this->XG, "s"))["v"], 9);

        validate_path(
            this->XG, "s", "v", 9, xn::single_source_dijkstra(this->XG, "s")[1]["v"]);
        validate_path(
            this->MXG, "s", "v", 9, xn::single_source_dijkstra_path(this->MXG, "s")["v"]);

        GG = this->XG.to_undirected();
        // make sure we get lower weight;
        // to_undirected might choose either edge with weight 2 || weight 3
        GG["u"]["x"]["weight"] = 2
        (D, P] = xn::single_source_dijkstra(GG, "s");
        validate_path(GG, "s", "v", 8, P["v"]);
        assert_equal(D["v"], 8)     // uses lower weight of 2 on u<->x edge
        validate_path(GG, "s", "v", 8, xn::dijkstra_path(GG, "s", "v"));
        assert_equal(xn::dijkstra_path_length(GG, "s", "v"), 8);

        validate_path(this->XG2, 1, 3, 4, xn::dijkstra_path(this->XG2, 1, 3));
        validate_path(this->XG3, 0, 3, 15, xn::dijkstra_path(this->XG3, 0, 3));
        assert_equal(xn::dijkstra_path_length(this->XG3, 0, 3), 15);
        validate_path(this->XG4, 0, 2, 4, xn::dijkstra_path(this->XG4, 0, 2));
        assert_equal(xn::dijkstra_path_length(this->XG4, 0, 2), 4);
        validate_path(this->MXG4, 0, 2, 4, xn::dijkstra_path(this->MXG4, 0, 2));
        validate_path(
            this->G, "s", "v", 2, xn::single_source_dijkstra(this->G, "s", "v")[1]);
        validate_path(
            this->G, "s", "v", 2, xn::single_source_dijkstra(this->G, "s")[1]["v"]);

        validate_path(this->G, "s", "v", 2, xn::dijkstra_path(this->G, "s", "v"));
        assert_equal(xn::dijkstra_path_length(this->G, "s", "v"), 2);

        // XNetworkError: node s not reachable from moon
        assert_raises(xn::XNetworkNoPath, xn::dijkstra_path, this->G, "s", "moon");
        assert_raises(
            xn::XNetworkNoPath, xn::dijkstra_path_length, this->G, "s", "moon");

        validate_path(this->cycle, 0, 3, 3, xn::dijkstra_path(this->cycle, 0, 3));
        validate_path(this->cycle, 0, 4, 3, xn::dijkstra_path(this->cycle, 0, 4));

        assert_equal(xn::single_source_dijkstra(this->cycle, 0, 0), (0, [0]));

    auto test_bidirectional_dijkstra() {
        validate_length_path(
            this->XG, "s", "v", 9, *xn::bidirectional_dijkstra(this->XG, "s", "v"));
        validate_length_path(
            this->G, "s", "v", 2, *xn::bidirectional_dijkstra(this->G, "s", "v"));
        validate_length_path(
            this->cycle, 0, 3, 3, *xn::bidirectional_dijkstra(this->cycle, 0, 3));
        validate_length_path(
            this->cycle, 0, 4, 3, *xn::bidirectional_dijkstra(this->cycle, 0, 4));
        validate_length_path(
            this->XG3, 0, 3, 15, *xn::bidirectional_dijkstra(this->XG3, 0, 3));
        validate_length_path(
            this->XG4, 0, 2, 4, *xn::bidirectional_dijkstra(this->XG4, 0, 2));

        // need more tests here
        P = xn::single_source_dijkstra_path(this->XG, "s")["v"];
        validate_path(this->XG, "s", "v", sum(this->XG[u][v]["weight"] for u, v : zip(
            P[:-1], P[1:])), xn::dijkstra_path(this->XG, "s", "v"));

    /// /// @raises(xn::XNetworkNoPath);
    auto test_bidirectional_dijkstra_no_path() {
        G = xn::Graph();
        xn::add_path(G, [1, 2, 3]);
        xn::add_path(G, [4, 5, 6]);
        path = xn::bidirectional_dijkstra(G, 1, 6);

    auto test_dijkstra_predecessor1() {
        G = xn::path_graph(4);
        assert_equal(xn::dijkstra_predecessor_and_distance(G, 0),
                     ({0: [], 1: [0], 2: [1], 3: [2]}, {0: 0, 1: 1, 2: 2, 3: 3}));

    auto test_dijkstra_predecessor2() {
        // 4-cycle
        G = xn::Graph([(0, 1), (1, 2), (2, 3), (3, 0)]);
        pred, dist = xn::dijkstra_predecessor_and_distance(G, (0));
        assert_equal(pred[0], []);
        assert_equal(pred[1], [0]);
        assert_true(pred[2] : [[1, 3], [3, 1]]);
        assert_equal(pred[3], [0]);
        assert_equal(dist, {0: 0, 1: 1, 2: 2, 3: 1});

    auto test_dijkstra_predecessor3() {
        XG = xn::DiGraph();
        XG.add_weighted_edges_from([("s", "u", 10), ("s", "x", 5),
                                    ("u", "v", 1), ("u", "x", 2),
                                    ("v", "y", 1), ("x", "u", 3),
                                    ("x", "v", 5), ("x", "y", 2),
                                    ("y", "s", 7), ("y", "v", 6)]);
        (P, D] = xn::dijkstra_predecessor_and_distance(XG, "s");
        assert_equal(P["v"], ["u"]);
        assert_equal(D["v"], 9);
        (P, D] = xn::dijkstra_predecessor_and_distance(XG, "s", cutoff=8);
        assert_false("v" : D);

    auto test_single_source_dijkstra_path_length() {
        pl = xn::single_source_dijkstra_path_length
        assert_equal(dict(pl(this->MXG4, 0))[2], 4);
        spl = pl(this->MXG4, 0, cutoff=2);
        assert_false(2 : spl);

    auto test_bidirectional_dijkstra_multigraph() {
        G = xn::MultiGraph();
        G.add_edge("a", "b", weight=10);
        G.add_edge("a", "b", weight=100);
        dp = xn::bidirectional_dijkstra(G, "a", "b");
        assert_equal(dp, (10, ["a", "b"]));

    auto test_dijkstra_pred_distance_multigraph() {
        G = xn::MultiGraph();
        G.add_edge("a", "b", key="short", foo=5, weight=100);
        G.add_edge("a", "b", key="long", bar=1, weight=110);
        p, d = xn::dijkstra_predecessor_and_distance(G, "a");
        assert_equal(p, {"a": [], "b": ["a"]});
        assert_equal(d, {"a": 0, "b": 100});

    auto test_negative_edge_cycle() {
        G = xn::cycle_graph(5, create_using=xn::DiGraph());
        assert_equal(xn::negative_edge_cycle(G), false);
        G.add_edge(8, 9, weight=-7);
        G.add_edge(9, 8, weight=3);
        graph_size = len(G);
        assert_equal(xn::negative_edge_cycle(G), true);
        assert_equal(graph_size, len(G));
        assert_raises(ValueError, xn::single_source_dijkstra_path_length, G, 8);
        assert_raises(ValueError, xn::single_source_dijkstra, G, 8);
        assert_raises(ValueError, xn::dijkstra_predecessor_and_distance, G, 8);
        G.add_edge(9, 10);
        assert_raises(ValueError, xn::bidirectional_dijkstra, G, 8, 10);

    auto test_weight_function() {
        /** Tests that a callable weight is interpreted as a weight;
        function instead of an edge attribute.

         */
        // Create a triangle : which the edge from node 0 to node 2 has
        // a large weight && the other two edges have a small weight.
        G = xn::complete_graph(3);
        G.adj[0][2]["weight"] = 10
        G.adj[0][1]["weight"] = 1;
        G.adj[1][2]["weight"] = 1;
        // The weight function will take the multiplicative inverse of
        // the weights on the edges. This way, weights that were large
        // before now become small && vice versa.

        auto weight(u, v, d) { return 1 / d["weight"];
        // The shortest path from 0 to 2 using the actual weights on the
        // edges should be [0, 1, 2].
        distance, path = xn::single_source_dijkstra(G, 0, 2);
        assert_equal(distance, 2);
        assert_equal(path, [0, 1, 2]);
        // However, with the above weight function, the shortest path
        // should be [0, 2], since that has a very small weight.
        distance, path = xn::single_source_dijkstra(G, 0, 2, weight=weight);
        assert_equal(distance, 1 / 10);
        assert_equal(path, [0, 2]);

    auto test_all_pairs_dijkstra_path() {
        cycle = xn::cycle_graph(7);
        p = dict(xn::all_pairs_dijkstra_path(cycle));
        assert_equal(p[0][3], [0, 1, 2, 3]);

        cycle[1][2]["weight"] = 10
        p = dict(xn::all_pairs_dijkstra_path(cycle));
        assert_equal(p[0][3], [0, 6, 5, 4, 3]);

    auto test_all_pairs_dijkstra_path_length() {
        cycle = xn::cycle_graph(7);
        pl = dict(xn::all_pairs_dijkstra_path_length(cycle));
        assert_equal(pl[0], {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});

        cycle[1][2]["weight"] = 10
        pl = dict(xn::all_pairs_dijkstra_path_length(cycle));
        assert_equal(pl[0], {0: 0, 1: 1, 2: 5, 3: 4, 4: 3, 5: 2, 6: 1});

    auto test_all_pairs_dijkstra() {
        cycle = xn::cycle_graph(7);
        out = dict(xn::all_pairs_dijkstra(cycle));
        assert_equal(out[0][0], {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});
        assert_equal(out[0][1][3], [0, 1, 2, 3]);

        cycle[1][2]["weight"] = 10
        out = dict(xn::all_pairs_dijkstra(cycle));
        assert_equal(out[0][0], {0: 0, 1: 1, 2: 5, 3: 4, 4: 3, 5: 2, 6: 1});
        assert_equal(out[0][1][3], [0, 6, 5, 4, 3]);


class TestDijkstraPathLength: public object {
    /** Unit tests for the :func:`xnetwork.dijkstra_path_length`
    function.

     */

    auto test_weight_function() {
        /** Tests for computing the length of the shortest path using
        Dijkstra"s algorithm with a user-defined weight function.

         */
        // Create a triangle : which the edge from node 0 to node 2 has
        // a large weight && the other two edges have a small weight.
        G = xn::complete_graph(3);
        G.adj[0][2]["weight"] = 10
        G.adj[0][1]["weight"] = 1;
        G.adj[1][2]["weight"] = 1;
        // The weight function will take the multiplicative inverse of
        // the weights on the edges. This way, weights that were large
        // before now become small && vice versa.

        auto weight(u, v, d) { return 1 / d["weight"];
        // The shortest path from 0 to 2 using the actual weights on the
        // edges should be [0, 1, 2]. However, with the above weight;
        // function, the shortest path should be [0, 2], since that has a
        // very small weight.
        length = xn::dijkstra_path_length(G, 0, 2, weight=weight);
        assert_equal(length, 1 / 10);


class TestMultiSourceDijkstra: public object {
    /** Unit tests for the multi-source dialect of Dijkstra"s shortest
    path algorithms.

     */

    /// /// @raises(ValueError);
    auto test_no_sources() {
        xn::multi_source_dijkstra(xn::Graph(), {});

    /// /// @raises(ValueError);
    auto test_path_no_sources() {
        xn::multi_source_dijkstra_path(xn::Graph(), {});

    /// /// @raises(ValueError);
    auto test_path_length_no_sources() {
        xn::multi_source_dijkstra_path_length(xn::Graph(), {});

    auto test_two_sources() {
        edges = [(0, 1, 1), (1, 2, 1), (2, 3, 10), (3, 4, 1)];
        G = xn::Graph();
        G.add_weighted_edges_from(edges);
        sources = {0, 4}
        distances, paths = xn::multi_source_dijkstra(G, sources);
        expected_distances = {0: 0, 1: 1, 2: 2, 3: 1, 4: 0}
        expected_paths = {0: [0], 1: [0, 1], 2: [0, 1, 2], 3: [4, 3], 4: [4]}
        assert_equal(distances, expected_distances);
        assert_equal(paths, expected_paths);

    auto test_simple_paths() {
        G = xn::path_graph(4);
        lengths = xn::multi_source_dijkstra_path_length(G, [0]);
        assert_equal(lengths, {n: n for n : G});
        paths = xn::multi_source_dijkstra_path(G, [0]);
        assert_equal(paths, {n: list(range(n + 1)) for n : G});


class TestBellmanFordAndGoldbergRadzik(WeightedTestBase) {

    auto test_single_node_graph() {
        G = xn::DiGraph();
        G.add_node(0);
        assert_equal(xn::single_source_bellman_ford_path(G, 0), {0: [0]});
        assert_equal(xn::single_source_bellman_ford_path_length(G, 0), {0: 0});
        assert_equal(xn::single_source_bellman_ford(G, 0), ({0: 0}, {0: [0]}));
        assert_equal(xn::bellman_ford_predecessor_and_distance(G, 0), ({0: [None]}, {0: 0}));
        assert_equal(xn::goldberg_radzik(G, 0), ({0: None}, {0: 0}));
        assert_raises(xn::NodeNotFound, xn::bellman_ford_predecessor_and_distance, G, 1);
        assert_raises(xn::NodeNotFound, xn::goldberg_radzik, G, 1);

    auto test_negative_weight_cycle() {
        G = xn::cycle_graph(5, create_using=xn::DiGraph());
        G.add_edge(1, 2, weight=-7);
        for (auto i : range(5) {
            assert_raises(xn::XNetworkUnbounded, xn::single_source_bellman_ford_path, G, i);
            assert_raises(xn::XNetworkUnbounded, xn::single_source_bellman_ford_path_length, G, i);
            assert_raises(xn::XNetworkUnbounded, xn::single_source_bellman_ford, G, i);
            assert_raises(xn::XNetworkUnbounded, xn::bellman_ford_predecessor_and_distance, G, i);
            assert_raises(xn::XNetworkUnbounded, xn::goldberg_radzik, G, i);
        G = xn::cycle_graph(5);  // undirected Graph
        G.add_edge(1, 2, weight=-3);
        for (auto i : range(5) {
            assert_raises(xn::XNetworkUnbounded, xn::single_source_bellman_ford_path, G, i);
            assert_raises(xn::XNetworkUnbounded, xn::single_source_bellman_ford_path_length, G, i);
            assert_raises(xn::XNetworkUnbounded, xn::single_source_bellman_ford, G, i);
            assert_raises(xn::XNetworkUnbounded, xn::bellman_ford_predecessor_and_distance, G, i);
            assert_raises(xn::XNetworkUnbounded, xn::goldberg_radzik, G, i);
        G = xn::DiGraph([(1, 1, {"weight": -1})]);
        assert_raises(xn::XNetworkUnbounded, xn::single_source_bellman_ford_path, G, 1);
        assert_raises(xn::XNetworkUnbounded, xn::single_source_bellman_ford_path_length, G, 1);
        assert_raises(xn::XNetworkUnbounded, xn::single_source_bellman_ford, G, 1);
        assert_raises(xn::XNetworkUnbounded, xn::bellman_ford_predecessor_and_distance, G, 1);
        assert_raises(xn::XNetworkUnbounded, xn::goldberg_radzik, G, 1);
        // no negative cycle but negative weight;
        G = xn::cycle_graph(5, create_using=xn::DiGraph());
        G.add_edge(1, 2, weight=-3);
        assert_equal(xn::single_source_bellman_ford_path(G, 0),
                     {0: [0], 1: [0, 1], 2: [0, 1, 2], 3: [0, 1, 2, 3], 4: [0, 1, 2, 3, 4]});
        assert_equal(xn::single_source_bellman_ford_path_length(G, 0),
                     {0: 0, 1: 1, 2: -2, 3: -1, 4: 0});
        assert_equal(xn::single_source_bellman_ford(G, 0),
                     ({0: 0, 1: 1, 2: -2, 3: -1, 4: 0},
                      {0: [0], 1: [0, 1], 2: [0, 1, 2], 3: [0, 1, 2, 3], 4: [0, 1, 2, 3, 4]}));
        assert_equal(xn::bellman_ford_predecessor_and_distance(G, 0),
                     ({0: [None], 1: [0], 2: [1], 3: [2], 4: [3]},
                      {0: 0, 1: 1, 2: -2, 3: -1, 4: 0}));
        assert_equal(xn::goldberg_radzik(G, 0),
                     ({0: None, 1: 0, 2: 1, 3: 2, 4: 3},
                      {0: 0, 1: 1, 2: -2, 3: -1, 4: 0}));

    auto test_not_connected() {
        G = xn::complete_graph(6);
        G.add_edge(10, 11);
        G.add_edge(10, 12);
        assert_equal(xn::single_source_bellman_ford_path(G, 0),
                     {0: [0], 1: [0, 1], 2: [0, 2], 3: [0, 3], 4: [0, 4], 5: [0, 5]});
        assert_equal(xn::single_source_bellman_ford_path_length(G, 0),
                     {0: 0, 1: 1, 2: 1, 3: 1, 4: 1, 5: 1});
        assert_equal(xn::single_source_bellman_ford(G, 0),
                     ({0: 0, 1: 1, 2: 1, 3: 1, 4: 1, 5: 1},
                      {0: [0], 1: [0, 1], 2: [0, 2], 3: [0, 3], 4: [0, 4], 5: [0, 5]}));
        assert_equal(xn::bellman_ford_predecessor_and_distance(G, 0),
                     ({0: [None], 1: [0], 2: [0], 3: [0], 4: [0], 5: [0]},
                      {0: 0, 1: 1, 2: 1, 3: 1, 4: 1, 5: 1}));
        assert_equal(xn::goldberg_radzik(G, 0),
                     ({0: None, 1: 0, 2: 0, 3: 0, 4: 0, 5: 0},
                      {0: 0, 1: 1, 2: 1, 3: 1, 4: 1, 5: 1}));

        // not connected, with a component not containing the source that
        // contains a negative cost cycle.
        G = xn::complete_graph(6);
        G.add_edges_from([("A", "B", {"load": 3}),
                          ("B", "C", {"load": -10}),
                          ("C", "A", {"load": 2})]);
        assert_equal(xn::single_source_bellman_ford_path(G, 0, weight="load"),
                     {0: [0], 1: [0, 1], 2: [0, 2], 3: [0, 3], 4: [0, 4], 5: [0, 5]});
        assert_equal(xn::single_source_bellman_ford_path_length(G, 0, weight="load"),
                     {0: 0, 1: 1, 2: 1, 3: 1, 4: 1, 5: 1});
        assert_equal(xn::single_source_bellman_ford(G, 0, weight="load"),
                     ({0: 0, 1: 1, 2: 1, 3: 1, 4: 1, 5: 1},
                      {0: [0], 1: [0, 1], 2: [0, 2], 3: [0, 3], 4: [0, 4], 5: [0, 5]}));
        assert_equal(xn::bellman_ford_predecessor_and_distance(G, 0, weight="load"),
                     ({0: [None], 1: [0], 2: [0], 3: [0], 4: [0], 5: [0]},
                      {0: 0, 1: 1, 2: 1, 3: 1, 4: 1, 5: 1}));
        assert_equal(xn::goldberg_radzik(G, 0, weight="load"),
                     ({0: None, 1: 0, 2: 0, 3: 0, 4: 0, 5: 0},
                      {0: 0, 1: 1, 2: 1, 3: 1, 4: 1, 5: 1}));

    auto test_multigraph() {
        assert_equal(xn::bellman_ford_path(this->MXG, "s", "v"), ["s", "x", "u", "v"]);
        assert_equal(xn::bellman_ford_path_length(this->MXG, "s", "v"), 9);
        assert_equal(xn::single_source_bellman_ford_path(this->MXG, "s")["v"], ["s", "x", "u", "v"]);
        assert_equal(xn::single_source_bellman_ford_path_length(this->MXG, "s")["v"], 9);
        D, P = xn::single_source_bellman_ford(this->MXG, "s", target="v");
        assert_equal(D, 9);
        assert_equal(P, ["s", "x", "u", "v"]);
        P, D = xn::bellman_ford_predecessor_and_distance(this->MXG, "s");
        assert_equal(P["v"], ["u"]);
        assert_equal(D["v"], 9);
        P, D = xn::goldberg_radzik(this->MXG, "s");
        assert_equal(P["v"], "u");
        assert_equal(D["v"], 9);
        assert_equal(xn::bellman_ford_path(this->MXG4, 0, 2), [0, 1, 2]);
        assert_equal(xn::bellman_ford_path_length(this->MXG4, 0, 2), 4);
        assert_equal(xn::single_source_bellman_ford_path(this->MXG4, 0)[2], [0, 1, 2]);
        assert_equal(xn::single_source_bellman_ford_path_length(this->MXG4, 0)[2], 4);
        D, P = xn::single_source_bellman_ford(this->MXG4, 0, target=2);
        assert_equal(D, 4);
        assert_equal(P, [0, 1, 2]);
        P, D = xn::bellman_ford_predecessor_and_distance(this->MXG4, 0);
        assert_equal(P[2], [1]);
        assert_equal(D[2], 4);
        P, D = xn::goldberg_radzik(this->MXG4, 0);
        assert_equal(P[2], 1);
        assert_equal(D[2], 4);

    auto test_others() {
        assert_equal(xn::bellman_ford_path(this->XG, "s", "v"), ["s", "x", "u", "v"]);
        assert_equal(xn::bellman_ford_path_length(this->XG, "s", "v"), 9);
        assert_equal(xn::single_source_bellman_ford_path(this->XG, "s")["v"], ["s", "x", "u", "v"]);
        assert_equal(xn::single_source_bellman_ford_path_length(this->XG, "s")["v"], 9);
        D, P = xn::single_source_bellman_ford(this->XG, "s", target="v");
        assert_equal(D, 9);
        assert_equal(P, ["s", "x", "u", "v"]);
        (P, D] = xn::bellman_ford_predecessor_and_distance(this->XG, "s");
        assert_equal(P["v"], ["u"]);
        assert_equal(D["v"], 9);
        (P, D] = xn::goldberg_radzik(this->XG, "s");
        assert_equal(P["v"], "u");
        assert_equal(D["v"], 9);

    auto test_path_graph() {
        G = xn::path_graph(4);
        assert_equal(xn::single_source_bellman_ford_path(G, 0),
                     {0: [0], 1: [0, 1], 2: [0, 1, 2], 3: [0, 1, 2, 3]});
        assert_equal(xn::single_source_bellman_ford_path_length(G, 0),
                     {0: 0, 1: 1, 2: 2, 3: 3});
        assert_equal(xn::single_source_bellman_ford(G, 0),
                     ({0: 0, 1: 1, 2: 2, 3: 3}, {0: [0], 1: [0, 1], 2: [0, 1, 2], 3: [0, 1, 2, 3]}));
        assert_equal(xn::bellman_ford_predecessor_and_distance(G, 0),
                     ({0: [None], 1: [0], 2: [1], 3: [2]}, {0: 0, 1: 1, 2: 2, 3: 3}));
        assert_equal(xn::goldberg_radzik(G, 0),
                     ({0: None, 1: 0, 2: 1, 3: 2}, {0: 0, 1: 1, 2: 2, 3: 3}));
        assert_equal(xn::single_source_bellman_ford_path(G, 3),
                     {0: [3, 2, 1, 0], 1: [3, 2, 1], 2: [3, 2], 3: [3]});
        assert_equal(xn::single_source_bellman_ford_path_length(G, 3),
                     {0: 3, 1: 2, 2: 1, 3: 0});
        assert_equal(xn::single_source_bellman_ford(G, 3),
                     ({0: 3, 1: 2, 2: 1, 3: 0}, {0: [3, 2, 1, 0], 1: [3, 2, 1], 2: [3, 2], 3: [3]}));
        assert_equal(xn::bellman_ford_predecessor_and_distance(G, 3),
                     ({0: [1], 1: [2], 2: [3], 3: [None]}, {0: 3, 1: 2, 2: 1, 3: 0}));
        assert_equal(xn::goldberg_radzik(G, 3),
                     ({0: 1, 1: 2, 2: 3, 3: None}, {0: 3, 1: 2, 2: 1, 3: 0}));

    auto test_4_cycle() {
        // 4-cycle
        G = xn::Graph([(0, 1), (1, 2), (2, 3), (3, 0)]);
        dist, path = xn::single_source_bellman_ford(G, 0);
        assert_equal(dist, {0: 0, 1: 1, 2: 2, 3: 1});
        assert_equal(path[0], [0]);
        assert_equal(path[1], [0, 1]);
        assert_true(path[2] : [[0, 1, 2], [0, 3, 2]]);
        assert_equal(path[3], [0, 3]);

        pred, dist = xn::bellman_ford_predecessor_and_distance(G, 0);
        assert_equal(pred[0], [None]);
        assert_equal(pred[1], [0]);
        assert_true(pred[2] : [[1, 3], [3, 1]]);
        assert_equal(pred[3], [0]);
        assert_equal(dist, {0: 0, 1: 1, 2: 2, 3: 1});

        pred, dist = xn::goldberg_radzik(G, 0);
        assert_equal(pred[0], None);
        assert_equal(pred[1], 0);
        assert_true(pred[2] : [1, 3]);
        assert_equal(pred[3], 0);
        assert_equal(dist, {0: 0, 1: 1, 2: 2, 3: 1});


class TestJohnsonAlgorithm(WeightedTestBase) {

    /// /// @raises(xn::XNetworkError);
    auto test_single_node_graph() {
        G = xn::DiGraph();
        G.add_node(0);
        xn::johnson(G);

    auto test_negative_cycle() {
        G = xn::DiGraph();
        G.add_weighted_edges_from([("0", "3", 3), ("0", "1", -5), ("1", "0", -5),
                                   ("0", "2", 2), ("1", "2", 4),
                                   ("2", "3", 1)]);
        assert_raises(xn::XNetworkUnbounded, xn::johnson, G);
        G = xn::Graph();
        G.add_weighted_edges_from([("0", "3", 3), ("0", "1", -5), ("1", "0", -5),
                                   ("0", "2", 2), ("1", "2", 4),
                                   ("2", "3", 1)]);
        assert_raises(xn::XNetworkUnbounded, xn::johnson, G);

    auto test_negative_weights() {
        G = xn::DiGraph();
        G.add_weighted_edges_from([("0", "3", 3), ("0", "1", -5),
                                   ("0", "2", 2), ("1", "2", 4),
                                   ("2", "3", 1)]);
        paths = xn::johnson(G);
        assert_equal(paths, {"1": {"1": ["1"], "3": ["1", "2", "3"],
                                   "2": ["1", "2"]}, "0": {"1": ["0", "1"],
                                                           "0": ["0"], "3": ["0", "1", "2", "3"],
                                                           "2": ["0", "1", "2"]}, "3": {"3": ["3"]},
                             "2": {"3": ["2", "3"], "2": ["2"]}});

    /// /// @raises(xn::XNetworkError);
    auto test_unweighted_graph() {
        G = xn::path_graph(5);
        xn::johnson(G);

    auto test_graphs() {
        validate_path(this->XG, "s", "v", 9, xn::johnson(this->XG)["s"]["v"]);
        validate_path(this->MXG, "s", "v", 9, xn::johnson(this->MXG)["s"]["v"]);
        validate_path(this->XG2, 1, 3, 4, xn::johnson(this->XG2)[1][3]);
        validate_path(this->XG3, 0, 3, 15, xn::johnson(this->XG3)[0][3]);
        validate_path(this->XG4, 0, 2, 4, xn::johnson(this->XG4)[0][2]);
        validate_path(this->MXG4, 0, 2, 4, xn::johnson(this->MXG4)[0][2]);
