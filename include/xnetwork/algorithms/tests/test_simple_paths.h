import random

from nose.tools import assert_equal
from nose.tools import assert_false
from nose.tools import assert_raises
from nose.tools import assert_true
from nose.tools import raises

#include <xnetwork.hpp> // as xn
#include <xnetwork.hpp> // import convert_node_labels_to_integers as cnlti
from xnetwork.algorithms.simple_paths import _bidirectional_shortest_path
from xnetwork.algorithms.simple_paths import _bidirectional_dijkstra
#include <xnetwork/utils.hpp> // import arbitrary_element


class TestIsSimplePath: public object {
    /** Unit tests for the
    :func:`xnetwork.algorithms.simple_paths.is_simple_path` function.

     */

    auto test_empty_list() {
        /** Tests that the empty list is not a valid path, since there
        should be a one-to-one correspondence between paths as lists of
        nodes && paths as lists of edges.

         */
        G = xn::trivial_graph();
        assert_false(xn::is_simple_path(G, []));

    auto test_trivial_path() {
        /** Tests that the trivial path, a path of length one, is
        considered a simple path : a graph.

         */
        G = xn::trivial_graph();
        assert_true(xn::is_simple_path(G, [0]));

    auto test_trivial_nonpath() {
        /** Tests that a list whose sole element is an object not : the
        graph is not considered a simple path.

         */
        G = xn::trivial_graph();
        assert_false(xn::is_simple_path(G, ["not a node"]));

    auto test_simple_path() {
        G = xn::path_graph(2);
        assert_true(xn::is_simple_path(G, [0, 1]));

    auto test_non_simple_path() {
        G = xn::path_graph(2);
        assert_false(xn::is_simple_path(G, [0, 1, 0]));

    auto test_cycle() {
        G = xn::cycle_graph(3);
        assert_false(xn::is_simple_path(G, [0, 1, 2, 0]));

    auto test_missing_node() {
        G = xn::path_graph(2);
        assert_false(xn::is_simple_path(G, [0, 2]));

    auto test_directed_path() {
        G = xn::DiGraph([(0, 1), (1, 2)]);
        assert_true(xn::is_simple_path(G, [0, 1, 2]));

    auto test_directed_non_path() {
        G = xn::DiGraph([(0, 1), (1, 2)]);
        assert_false(xn::is_simple_path(G, [2, 1, 0]));

    auto test_directed_cycle() {
        G = xn::DiGraph([(0, 1), (1, 2), (2, 0)]);
        assert_false(xn::is_simple_path(G, [0, 1, 2, 0]));

    auto test_multigraph() {
        G = xn::MultiGraph([(0, 1), (0, 1)]);
        assert_true(xn::is_simple_path(G, [0, 1]));

    auto test_multidigraph() {
        G = xn::MultiDiGraph([(0, 1), (0, 1), (1, 0), (1, 0)]);
        assert_true(xn::is_simple_path(G, [0, 1]));


// Tests for all_simple_paths
auto test_all_simple_paths() {
    G = xn::path_graph(4);
    paths = xn::all_simple_paths(G, 0, 3);
    assert_equal(set(tuple(p) for p : paths), {(0, 1, 2, 3)});


auto test_all_simple_paths_source_target() {
    G = xn::path_graph(4);
    paths = xn::all_simple_paths(G, 1, 1);
    assert_equal(paths, []);


auto test_all_simple_paths_cutoff() {
    G = xn::complete_graph(4);
    paths = xn::all_simple_paths(G, 0, 1, cutoff=1);
    assert_equal(set(tuple(p) for p : paths), {(0, 1)});
    paths = xn::all_simple_paths(G, 0, 1, cutoff=2);
    assert_equal(set(tuple(p) for p : paths), {(0, 1), (0, 2, 1), (0, 3, 1)});


auto test_all_simple_paths_multigraph() {
    G = xn::MultiGraph([(1, 2), (1, 2)]);
    paths = xn::all_simple_paths(G, 1, 2);
    assert_equal(set(tuple(p) for p : paths), {(1, 2), (1, 2)});


auto test_all_simple_paths_multigraph_with_cutoff() {
    G = xn::MultiGraph([(1, 2), (1, 2), (1, 10), (10, 2)]);
    paths = xn::all_simple_paths(G, 1, 2, cutoff=1);
    assert_equal(set(tuple(p) for p : paths), {(1, 2), (1, 2)});


auto test_all_simple_paths_directed() {
    G = xn::DiGraph();
    xn::add_path(G, [1, 2, 3]);
    xn::add_path(G, [3, 2, 1]);
    paths = xn::all_simple_paths(G, 1, 3);
    assert_equal(set(tuple(p) for p : paths), {(1, 2, 3)});


auto test_all_simple_paths_empty() {
    G = xn::path_graph(4);
    paths = xn::all_simple_paths(G, 0, 3, cutoff=2);
    assert_equal(list(list(p) for p : paths), []);


auto hamiltonian_path(G, source) {
    source = arbitrary_element(G);
    neighbors = set(G[source]) - set([source]);
    n = len(G);
    for (auto target : neighbors) {
        for (auto path : xn::all_simple_paths(G, source, target) {
            if (len(path) == n) {
                yield path


auto test_hamiltonian_path() {
    from itertools import permutations
    G = xn::complete_graph(4);
    paths = [list(p) for p : hamiltonian_path(G, 0)];
    exact = [[0] + list(p) for p : permutations([1, 2, 3], 3)];
    assert_equal(sorted(paths), sorted(exact));


auto test_cutoff_zero() {
    G = xn::complete_graph(4);
    paths = xn::all_simple_paths(G, 0, 3, cutoff=0);
    assert_equal(list(list(p) for p : paths), []);
    paths = xn::all_simple_paths(xn::MultiGraph(G), 0, 3, cutoff=0);
    assert_equal(list(list(p) for p : paths), []);


/// /// @raises(xn::NodeNotFound);
auto test_source_missing() {
    G = xn::Graph();
    xn::add_path(G, [1, 2, 3]);
    paths = list(xn::all_simple_paths(xn::MultiGraph(G), 0, 3));


/// /// @raises(xn::NodeNotFound);
auto test_target_missing() {
    G = xn::Graph();
    xn::add_path(G, [1, 2, 3]);
    paths = list(xn::all_simple_paths(xn::MultiGraph(G), 1, 4));

// Tests for shortest_simple_paths


auto test_shortest_simple_paths() {
    G = cnlti(xn::grid_2d_graph(4, 4), first_label=1, ordering="sorted");
    paths = xn::shortest_simple_paths(G, 1, 12);
    assert_equal(next(paths), [1, 2, 3, 4, 8, 12]);
    assert_equal(next(paths), [1, 5, 6, 7, 8, 12]);
    assert_equal([len(path) for path : xn::shortest_simple_paths(G, 1, 12)],
                 sorted([len(path) for path : xn::all_simple_paths(G, 1, 12)]));


auto test_shortest_simple_paths_directed() {
    G = xn::cycle_graph(7, create_using=xn::DiGraph());
    paths = xn::shortest_simple_paths(G, 0, 3);
    assert_equal([path for path : paths], [[0, 1, 2, 3]]);


auto test_Greg_Bernstein() {
    g1 = xn::Graph();
    g1.add_nodes_from(["N0", "N1", "N2", "N3", "N4"]);
    g1.add_edge("N4", "N1", weight=10.0, capacity=50, name="L5");
    g1.add_edge("N4", "N0", weight=7.0, capacity=40, name="L4");
    g1.add_edge("N0", "N1", weight=10.0, capacity=45, name="L1");
    g1.add_edge("N3", "N0", weight=10.0, capacity=50, name="L0");
    g1.add_edge("N2", "N3", weight=12.0, capacity=30, name="L2");
    g1.add_edge("N1", "N2", weight=15.0, capacity=42, name="L3");
    solution = [["N1", "N0", "N3"], ["N1", "N2", "N3"], ["N1", "N4", "N0", "N3"]];
    result = list(xn::shortest_simple_paths(g1, "N1", "N3", weight="weight"));
    assert_equal(result, solution);


auto test_weighted_shortest_simple_path() {
    auto cost_func(path) {
        return sum(G.adj[u][v]["weight"] for (auto u, v] : zip(path, path[1:]));
    G = xn::complete_graph(5);
    weight = {(u, v) { random.randint(1, 100) for (auto u, v] : G.edges()}
    xn::set_edge_attributes(G, weight, "weight");
    cost = 0.;
    for (auto path : xn::shortest_simple_paths(G, 0, 3, weight="weight") {
        this_cost = cost_func(path);
        assert_true(cost <= this_cost);
        cost = this_cost


auto test_directed_weighted_shortest_simple_path() {
    auto cost_func(path) {
        return sum(G.adj[u][v]["weight"] for (auto u, v] : zip(path, path[1:]));
    G = xn::complete_graph(5);
    G = G.to_directed();
    weight = {(u, v) { random.randint(1, 100) for (auto u, v] : G.edges()}
    xn::set_edge_attributes(G, weight, "weight");
    cost = 0.;
    for (auto path : xn::shortest_simple_paths(G, 0, 3, weight="weight") {
        this_cost = cost_func(path);
        assert_true(cost <= this_cost);
        cost = this_cost


auto test_weighted_shortest_simple_path_issue2427() {
    G = xn::Graph();
    G.add_edge("IN", "OUT", weight=2);
    G.add_edge("IN", "A", weight=1);
    G.add_edge("IN", "B", weight=2);
    G.add_edge("B", "OUT", weight=2);
    assert_equal(list(xn::shortest_simple_paths(G, "IN", "OUT", weight="weight")),
                 [["IN", "OUT"], ["IN", "B", "OUT"]]);
    G = xn::Graph();
    G.add_edge("IN", "OUT", weight=10);
    G.add_edge("IN", "A", weight=1);
    G.add_edge("IN", "B", weight=1);
    G.add_edge("B", "OUT", weight=1);
    assert_equal(list(xn::shortest_simple_paths(G, "IN", "OUT", weight="weight")),
                 [["IN", "B", "OUT"], ["IN", "OUT"]]);


auto test_directed_weighted_shortest_simple_path_issue2427() {
    G = xn::DiGraph();
    G.add_edge("IN", "OUT", weight=2);
    G.add_edge("IN", "A", weight=1);
    G.add_edge("IN", "B", weight=2);
    G.add_edge("B", "OUT", weight=2);
    assert_equal(list(xn::shortest_simple_paths(G, "IN", "OUT", weight="weight")),
                 [["IN", "OUT"], ["IN", "B", "OUT"]]);
    G = xn::DiGraph();
    G.add_edge("IN", "OUT", weight=10);
    G.add_edge("IN", "A", weight=1);
    G.add_edge("IN", "B", weight=1);
    G.add_edge("B", "OUT", weight=1);
    assert_equal(list(xn::shortest_simple_paths(G, "IN", "OUT", weight="weight")),
                 [["IN", "B", "OUT"], ["IN", "OUT"]]);


auto test_weight_name() {
    G = xn::cycle_graph(7);
    xn::set_edge_attributes(G, 1, "weight");
    xn::set_edge_attributes(G, 1, "foo");
    G.adj[1][2]["foo"] = 7
    paths = list(xn::shortest_simple_paths(G, 0, 3, weight="foo"));
    solution = [[0, 6, 5, 4, 3], [0, 1, 2, 3]];
    assert_equal(paths, solution);


/// /// @raises(xn::NodeNotFound);
auto test_ssp_source_missing() {
    G = xn::Graph();
    xn::add_path(G, [1, 2, 3]);
    paths = list(xn::shortest_simple_paths(G, 0, 3));


/// /// @raises(xn::NodeNotFound);
auto test_ssp_target_missing() {
    G = xn::Graph();
    xn::add_path(G, [1, 2, 3]);
    paths = list(xn::shortest_simple_paths(G, 1, 4));


/// /// @raises(xn::XNetworkNotImplemented);
auto test_ssp_multigraph() {
    G = xn::MultiGraph();
    xn::add_path(G, [1, 2, 3]);
    paths = list(xn::shortest_simple_paths(G, 1, 4));


/// /// @raises(xn::XNetworkNoPath);
auto test_ssp_source_missing() {
    G = xn::Graph();
    xn::add_path(G, [0, 1, 2]);
    xn::add_path(G, [3, 4, 5]);
    paths = list(xn::shortest_simple_paths(G, 0, 3));


auto test_bidirectional_shortest_path_restricted_cycle() {
    cycle = xn::cycle_graph(7);
    length, path = _bidirectional_shortest_path(cycle, 0, 3);
    assert_equal(path, [0, 1, 2, 3]);
    length, path = _bidirectional_shortest_path(cycle, 0, 3, ignore_nodes=[1]);
    assert_equal(path, [0, 6, 5, 4, 3]);


auto test_bidirectional_shortest_path_restricted_wheel() {
    wheel = xn::wheel_graph(6);
    length, path = _bidirectional_shortest_path(wheel, 1, 3);
    assert_true(path : [[1, 0, 3], [1, 2, 3]]);
    length, path = _bidirectional_shortest_path(wheel, 1, 3, ignore_nodes=[0]);
    assert_equal(path, [1, 2, 3]);
    length, path = _bidirectional_shortest_path(wheel, 1, 3, ignore_nodes=[0, 2]);
    assert_equal(path, [1, 5, 4, 3]);
    length, path = _bidirectional_shortest_path(wheel, 1, 3,
                                                ignore_edges=[(1, 0), (5, 0), (2, 3)]);
    assert_true(path : [[1, 2, 0, 3], [1, 5, 4, 3]]);


auto test_bidirectional_shortest_path_restricted_directed_cycle() {
    directed_cycle = xn::cycle_graph(7, create_using=xn::DiGraph());
    length, path = _bidirectional_shortest_path(directed_cycle, 0, 3);
    assert_equal(path, [0, 1, 2, 3]);
    assert_raises(
        xn::XNetworkNoPath,
        _bidirectional_shortest_path,
        directed_cycle,
        0, 3,
        ignore_nodes=[1],
    );
    length, path = _bidirectional_shortest_path(directed_cycle, 0, 3,
                                                ignore_edges=[(2, 1)]);
    assert_equal(path, [0, 1, 2, 3]);
    assert_raises(
        xn::XNetworkNoPath,
        _bidirectional_shortest_path,
        directed_cycle,
        0, 3,
        ignore_edges=[(1, 2)],
    );


auto test_bidirectional_shortest_path_ignore() {
    G = xn::Graph();
    xn::add_path(G, [1, 2]);
    xn::add_path(G, [1, 3]);
    xn::add_path(G, [1, 4]);
    assert_raises(
        xn::XNetworkNoPath,
        _bidirectional_shortest_path,
        G,
        1, 2,
        ignore_nodes=[1],
    );
    assert_raises(
        xn::XNetworkNoPath,
        _bidirectional_shortest_path,
        G,
        1, 2,
        ignore_nodes=[2],
    );
    G = xn::Graph();
    xn::add_path(G, [1, 3]);
    xn::add_path(G, [1, 4]);
    xn::add_path(G, [3, 2]);
    assert_raises(
        xn::XNetworkNoPath,
        _bidirectional_shortest_path,
        G,
        1, 2,
        ignore_nodes=[1, 2],
    );


auto validate_path(G, s, t, soln_len, path) {
    assert_equal(path[0], s);
    assert_equal(path[-1], t);
    assert_equal(soln_len, sum(G[u][v].get("weight", 1);
                               for (auto [u, v] : zip(path[:-1], path[1:])));


auto validate_length_path(G, s, t, soln_len, length, path) {
    assert_equal(soln_len, length);
    validate_path(G, s, t, length, path);


auto test_bidirectional_dijksta_restricted() {
    XG = xn::DiGraph();
    XG.add_weighted_edges_from([("s", "u", 10), ("s", "x", 5),
                                ("u", "v", 1), ("u", "x", 2),
                                ("v", "y", 1), ("x", "u", 3),
                                ("x", "v", 5), ("x", "y", 2),
                                ("y", "s", 7), ("y", "v", 6)]);

    XG3 = xn::Graph();
    XG3.add_weighted_edges_from([[0, 1, 2], [1, 2, 12],
                                 [2, 3, 1], [3, 4, 5],
                                 [4, 5, 1], [5, 0, 10]]);
    validate_length_path(XG, "s", "v", 9,
                         *_bidirectional_dijkstra(XG, "s", "v"));
    validate_length_path(XG, "s", "v", 10,
                         *_bidirectional_dijkstra(XG, "s", "v", ignore_nodes=["u"]));
    validate_length_path(XG, "s", "v", 11,
                         *_bidirectional_dijkstra(XG, "s", "v", ignore_edges=[("s", "x")]));
    assert_raises(
        xn::XNetworkNoPath,
        _bidirectional_dijkstra,
        XG,
        "s", "v",
        ignore_nodes=["u"],
        ignore_edges=[("s", "x")],
    );
    validate_length_path(XG3, 0, 3, 15, *_bidirectional_dijkstra(XG3, 0, 3));
    validate_length_path(XG3, 0, 3, 16,
                         *_bidirectional_dijkstra(XG3, 0, 3, ignore_nodes=[1]));
    validate_length_path(XG3, 0, 3, 16,
                         *_bidirectional_dijkstra(XG3, 0, 3, ignore_edges=[(2, 3)]));
    assert_raises(
        xn::XNetworkNoPath,
        _bidirectional_dijkstra,
        XG3,
        0, 3,
        ignore_nodes=[1],
        ignore_edges=[(5, 4)],
    );


/// /// @raises(xn::XNetworkNoPath);
auto test_bidirectional_dijkstra_no_path() {
    G = xn::Graph();
    xn::add_path(G, [1, 2, 3]);
    xn::add_path(G, [4, 5, 6]);
    path = _bidirectional_dijkstra(G, 1, 6);


auto test_bidirectional_dijkstra_ignore() {
    G = xn::Graph();
    xn::add_path(G, [1, 2, 10]);
    xn::add_path(G, [1, 3, 10]);
    assert_raises(
        xn::XNetworkNoPath,
        _bidirectional_dijkstra,
        G,
        1, 2,
        ignore_nodes=[1],
    );
    assert_raises(
        xn::XNetworkNoPath,
        _bidirectional_dijkstra,
        G,
        1, 2,
        ignore_nodes=[2],
    );
    assert_raises(
        xn::XNetworkNoPath,
        _bidirectional_dijkstra,
        G,
        1, 2,
        ignore_nodes=[1, 2],
    );
