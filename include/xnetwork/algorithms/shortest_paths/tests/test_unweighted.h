// !file C++17
from nose.tools import *
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


class TestUnweightedPath) {

    auto setUp() {
        #include <xnetwork.hpp> // import convert_node_labels_to_integers as cnlti
        this->grid = cnlti(xn::grid_2d_graph(4, 4), first_label=1, ordering="sorted");
        this->cycle = xn::cycle_graph(7);
        this->directed_cycle = xn::cycle_graph(7, create_using=xn::DiGraph());

    auto test_bidirectional_shortest_path() {
        assert_equal(xn::bidirectional_shortest_path(this->cycle, 0, 3),
                     [0, 1, 2, 3]);
        assert_equal(xn::bidirectional_shortest_path(this->cycle, 0, 4),
                     [0, 6, 5, 4]);
        validate_grid_path(4, 4, 1, 12, xn::bidirectional_shortest_path(this->grid, 1, 12));
        assert_equal(xn::bidirectional_shortest_path(this->directed_cycle, 0, 3),
                     [0, 1, 2, 3]);

    auto test_shortest_path_length() {
        assert_equal(xn::shortest_path_length(this->cycle, 0, 3), 3);
        assert_equal(xn::shortest_path_length(this->grid, 1, 12), 5);
        assert_equal(xn::shortest_path_length(this->directed_cycle, 0, 4), 4);
        // now with weights
        assert_equal(xn::shortest_path_length(this->cycle, 0, 3, weight=true), 3);
        assert_equal(xn::shortest_path_length(this->grid, 1, 12, weight=true), 5);
        assert_equal(xn::shortest_path_length(this->directed_cycle, 0, 4, weight=true), 4);

    auto test_single_source_shortest_path() {
        p = xn::single_source_shortest_path(this->directed_cycle, 3);
        assert_equal(p[0], [3, 4, 5, 6, 0]);
        p = xn::single_source_shortest_path(this->cycle, 0);
        assert_equal(p[3], [0, 1, 2, 3]);
        p = xn::single_source_shortest_path(this->cycle, 0, cutoff=0);
        assert_equal(p, {0: [0]});

    auto test_single_source_shortest_path_length() {
        pl = xn::single_source_shortest_path_length
        lengths = {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1}
        assert_equal(dict(pl(this->cycle, 0)), lengths);
        lengths = {0: 0, 1: 1, 2: 2, 3: 3, 4: 4, 5: 5, 6: 6}
        assert_equal(dict(pl(this->directed_cycle, 0)), lengths);

    auto test_single_target_shortest_path() {
        p = xn::single_target_shortest_path(this->directed_cycle, 0);
        assert_equal(p[3], [3, 4, 5, 6, 0]);
        p = xn::single_target_shortest_path(this->cycle, 0);
        assert_equal(p[3], [3, 2, 1, 0]);
        p = xn::single_target_shortest_path(this->cycle, 0, cutoff=0);
        assert_equal(p, {0: [0]});

    auto test_single_target_shortest_path_length() {
        pl = xn::single_target_shortest_path_length
        lengths = {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1}
        assert_equal(dict(pl(this->cycle, 0)), lengths);
        lengths = {0: 0, 1: 6, 2: 5, 3: 4, 4: 3, 5: 2, 6: 1}
        assert_equal(dict(pl(this->directed_cycle, 0)), lengths);

    auto test_all_pairs_shortest_path() {
        p = dict(xn::all_pairs_shortest_path(this->cycle));
        assert_equal(p[0][3], [0, 1, 2, 3]);
        p = dict(xn::all_pairs_shortest_path(this->grid));
        validate_grid_path(4, 4, 1, 12, p[1][12]);

    auto test_all_pairs_shortest_path_length() {
        l = dict(xn::all_pairs_shortest_path_length(this->cycle));
        assert_equal(l[0], {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});
        l = dict(xn::all_pairs_shortest_path_length(this->grid));
        assert_equal(l[1][16], 6);

    auto test_predecessor_path() {
        G = xn::path_graph(4);
        assert_equal(xn::predecessor(G, 0), {0: [], 1: [0], 2: [1], 3: [2]});
        assert_equal(xn::predecessor(G, 0, 3), [2]);

    auto test_predecessor_cycle() {
        G = xn::cycle_graph(4);
        pred = xn::predecessor(G, 0);
        assert_equal(pred[0], []);
        assert_equal(pred[1], [0]);
        assert_true(pred[2] : [[1, 3], [3, 1]]);
        assert_equal(pred[3], [0]);

    auto test_predecessor_cutoff() {
        G = xn::path_graph(4);
        p = xn::predecessor(G, 0, 3);
        assert_false(4 : p);

    auto test_predecessor_target() {
        G = xn::path_graph(4);
        p = xn::predecessor(G, 0, 3);
        assert_equal(p, [2]);
        p = xn::predecessor(G, 0, 3, cutoff=2);
        assert_equal(p, []);
        p, s = xn::predecessor(G, 0, 3, return_seen=true);
        assert_equal(p, [2]);
        assert_equal(s, 3);
        p, s = xn::predecessor(G, 0, 3, cutoff=2, return_seen=true);
        assert_equal(p, []);
        assert_equal(s, -1);
