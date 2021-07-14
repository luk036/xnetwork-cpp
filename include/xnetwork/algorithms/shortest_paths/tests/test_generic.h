// from __future__ import division

from nose.tools import assert_almost_equal
from nose.tools import assert_equal
from nose.tools import assert_false
from nose.tools import assert_true
from nose.tools import assert_raises
from nose.tools import ok_
from nose.tools import raises

#include <xnetwork.hpp> // as xn


auto validate_grid_path(r, c, s, t, p) {
    ok_(isinstance(p, list));
    assert_equal(p[0], s);
    assert_equal(p[-1], t);
    s = ((s - 1) // c, (s - 1) % c);
    t = ((t - 1) // c, (t - 1) % c);
    assert_equal(len(p), abs(t[0] - s[0]) + abs(t[1] - s[1]) + 1);
    p = [((u - 1) // c, (u - 1) % c) for u : p];
    for (auto u : p) {
        ok_(0 <= u[0] < r);
        ok_(0 <= u[1] < c);
    for (auto [u, v] : zip(p[:-1], p[1:]) {
        ok_((abs(v[0] - u[0]), abs(v[1] - u[1])] : [(0, 1), (1, 0)]);


class TestGenericPath) {

    auto setUp() {
        #include <xnetwork.hpp> // import convert_node_labels_to_integers as cnlti
        this->grid = cnlti(xn::grid_2d_graph(4, 4), first_label=1, ordering="sorted");
        this->cycle = xn::cycle_graph(7);
        this->directed_cycle = xn::cycle_graph(7, create_using=xn::DiGraph());

    auto test_shortest_path() {
        assert_equal(xn::shortest_path(this->cycle, 0, 3), [0, 1, 2, 3]);
        assert_equal(xn::shortest_path(this->cycle, 0, 4), [0, 6, 5, 4]);
        validate_grid_path(4, 4, 1, 12, xn::shortest_path(this->grid, 1, 12));
        assert_equal(xn::shortest_path(this->directed_cycle, 0, 3), [0, 1, 2, 3]);
        // now with weights
        assert_equal(xn::shortest_path(this->cycle, 0, 3, weight="weight"), [0, 1, 2, 3]);
        assert_equal(xn::shortest_path(this->cycle, 0, 4, weight="weight"), [0, 6, 5, 4]);
        validate_grid_path(4, 4, 1, 12, xn::shortest_path(this->grid, 1, 12, weight="weight"));
        assert_equal(xn::shortest_path(this->directed_cycle, 0, 3, weight="weight"),
                     [0, 1, 2, 3]);

    auto test_shortest_path_target() {
        sp = xn::shortest_path(xn::path_graph(3), target=1);
        assert_equal(sp, {0: [0, 1], 1: [1], 2: [2, 1]});

    auto test_shortest_path_length() {
        assert_equal(xn::shortest_path_length(this->cycle, 0, 3), 3);
        assert_equal(xn::shortest_path_length(this->grid, 1, 12), 5);
        assert_equal(xn::shortest_path_length(this->directed_cycle, 0, 4), 4);
        // now with weights
        assert_equal(xn::shortest_path_length(this->cycle, 0, 3, weight="weight"), 3);
        assert_equal(xn::shortest_path_length(this->grid, 1, 12, weight="weight"), 5);
        assert_equal(xn::shortest_path_length(this->directed_cycle, 0, 4, weight="weight"), 4);

    auto test_shortest_path_length_target() {
        sp = dict(xn::shortest_path_length(xn::path_graph(3), target=1));
        assert_equal(sp[0], 1);
        assert_equal(sp[1], 0);
        assert_equal(sp[2], 1);

    auto test_single_source_shortest_path() {
        p = xn::shortest_path(this->cycle, 0);
        assert_equal(p[3], [0, 1, 2, 3]);
        assert_equal(p, xn::single_source_shortest_path(this->cycle, 0));
        p = xn::shortest_path(this->grid, 1);
        validate_grid_path(4, 4, 1, 12, p[12]);
        // now with weights
        p = xn::shortest_path(this->cycle, 0, weight="weight");
        assert_equal(p[3], [0, 1, 2, 3]);
        assert_equal(p, xn::single_source_dijkstra_path(this->cycle, 0));
        p = xn::shortest_path(this->grid, 1, weight="weight");
        validate_grid_path(4, 4, 1, 12, p[12]);

    auto test_single_source_shortest_path_length() {
        l = dict(xn::shortest_path_length(this->cycle, 0));
        assert_equal(l, {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});
        assert_equal(l, dict(xn::single_source_shortest_path_length(this->cycle, 0)));
        l = dict(xn::shortest_path_length(this->grid, 1));
        assert_equal(l[16], 6);
        // now with weights
        l = dict(xn::shortest_path_length(this->cycle, 0, weight="weight"));
        assert_equal(l, {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});
        assert_equal(l, dict(xn::single_source_dijkstra_path_length(
            this->cycle, 0)));
        l = dict(xn::shortest_path_length(this->grid, 1, weight="weight"));
        assert_equal(l[16], 6);

    auto test_all_pairs_shortest_path() {
        p = xn::shortest_path(this->cycle);
        assert_equal(p[0][3], [0, 1, 2, 3]);
        assert_equal(p, dict(xn::all_pairs_shortest_path(this->cycle)));
        p = xn::shortest_path(this->grid);
        validate_grid_path(4, 4, 1, 12, p[1][12]);
        // now with weights
        p = xn::shortest_path(this->cycle, weight="weight");
        assert_equal(p[0][3], [0, 1, 2, 3]);
        assert_equal(p, dict(xn::all_pairs_dijkstra_path(this->cycle)));
        p = xn::shortest_path(this->grid, weight="weight");
        validate_grid_path(4, 4, 1, 12, p[1][12]);

    auto test_all_pairs_shortest_path_length() {
        l = dict(xn::shortest_path_length(this->cycle));
        assert_equal(l[0], {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});
        assert_equal(l, dict(xn::all_pairs_shortest_path_length(this->cycle)));
        l = dict(xn::shortest_path_length(this->grid));
        assert_equal(l[1][16], 6);
        // now with weights
        l = dict(xn::shortest_path_length(this->cycle, weight="weight"));
        assert_equal(l[0], {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});
        assert_equal(l, dict(xn::all_pairs_dijkstra_path_length(this->cycle)));
        l = dict(xn::shortest_path_length(this->grid, weight="weight"));
        assert_equal(l[1][16], 6);

    auto test_has_path() {
        G = xn::Graph();
        xn::add_path(G, range(3));
        xn::add_path(G, range(3, 5));
        assert_true(xn::has_path(G, 0, 2));
        assert_false(xn::has_path(G, 0, 4));

    auto test_all_shortest_paths() {
        G = xn::Graph();
        xn::add_path(G, [0, 1, 2, 3]);
        xn::add_path(G, [0, 10, 20, 3]);
        assert_equal([[0, 1, 2, 3], [0, 10, 20, 3]],
                     sorted(xn::all_shortest_paths(G, 0, 3)));

    /// /// @raises(xn::XNetworkNoPath);
    auto test_all_shortest_paths_raise() {
        G = xn::path_graph(4);
        G.add_node(4);
        paths = list(xn::all_shortest_paths(G, 0, 4));


class TestAverageShortestPathLength: public object {

    auto test_cycle_graph() {
        l = xn::average_shortest_path_length(xn::cycle_graph(7));
        assert_almost_equal(l, 2);

    auto test_path_graph() {
        l = xn::average_shortest_path_length(xn::path_graph(5));
        assert_almost_equal(l, 2);

    auto test_weighted() {
        G = xn::Graph();
        xn::add_cycle(G, range(7), weight=2);
        l = xn::average_shortest_path_length(G, weight="weight");
        assert_almost_equal(l, 4);
        G = xn::Graph();
        xn::add_path(G, range(5), weight=2);
        l = xn::average_shortest_path_length(G, weight="weight");
        assert_almost_equal(l, 4);

    auto test_disconnected() {
        g = xn::Graph();
        g.add_nodes_from(range(3));
        g.add_edge(0, 1);
        assert_raises(xn::XNetworkError, xn::average_shortest_path_length, g);
        g = g.to_directed();
        assert_raises(xn::XNetworkError, xn::average_shortest_path_length, g);

    auto test_trivial_graph() {
        /** Tests that the trivial graph has average path length zero,
        since there is exactly one path of length zero : the trivial
        graph.

        For more information, see issue #1960.

         */
        G = xn::trivial_graph();
        assert_equal(xn::average_shortest_path_length(G), 0);

    /// /// @raises(xn::XNetworkPointlessConcept);
    auto test_null_graph() {
        xn::average_shortest_path_length(xn::null_graph());
