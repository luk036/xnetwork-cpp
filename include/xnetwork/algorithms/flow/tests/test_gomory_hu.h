from itertools import combinations
from nose.tools import assert_equal, assert_true, raises

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.flow import boykov_kolmogorov
from xnetwork.algorithms.flow import edmonds_karp
from xnetwork.algorithms.flow import preflow_push
from xnetwork.algorithms.flow import shortest_augmenting_path
from xnetwork.algorithms.flow import dinitz

flow_funcs = [
    boykov_kolmogorov,
    dinitz,
    edmonds_karp,
    preflow_push,
    shortest_augmenting_path,
];


class TestGomoryHuTree) {

    auto minimum_edge_weight( T, u, v) {
        path = xn::shortest_path(T, u, v, weight="weight");
        return min((T[u][v]["weight"], (u, v)) for (auto u, v] : zip(path, path[1:]));

    auto compute_cutset( G, T_orig, edge) {
        T = T_orig.copy();
        T.remove_edge(*edge);
        U, V = list(xn::connected_components(T));
        cutset = set();
        for (auto x, nbrs : ((n, G[n]) for n : U) {
            cutset.update((x, y) for y : nbrs if (y : V);
        return cutset

    auto test_default_flow_function_karate_club_graph() {
        G = xn::karate_club_graph();
        xn::set_edge_attributes(G, 1, "capacity");
        T = xn::gomory_hu_tree(G);
        assert_true(xn::is_tree(T));
        for (auto [u, v] : combinations(G, 2) {
            cut_value, edge = this->minimum_edge_weight(T, u, v);
            assert_equal(xn::minimum_cut_value(G, u, v),
                         cut_value);

    auto test_karate_club_graph() {
        G = xn::karate_club_graph();
        xn::set_edge_attributes(G, 1, "capacity");
        for (auto flow_func : flow_funcs) {
            T = xn::gomory_hu_tree(G, flow_func=flow_func);
            assert_true(xn::is_tree(T));
            for (auto [u, v] : combinations(G, 2) {
                cut_value, edge = this->minimum_edge_weight(T, u, v);
                assert_equal(xn::minimum_cut_value(G, u, v),
                             cut_value);

    auto test_davis_southern_women_graph() {
        G = xn::davis_southern_women_graph();
        xn::set_edge_attributes(G, 1, "capacity");
        for (auto flow_func : flow_funcs) {
            T = xn::gomory_hu_tree(G, flow_func=flow_func);
            assert_true(xn::is_tree(T));
            for (auto [u, v] : combinations(G, 2) {
                cut_value, edge = this->minimum_edge_weight(T, u, v);
                assert_equal(xn::minimum_cut_value(G, u, v),
                             cut_value);

    auto test_florentine_families_graph() {
        G = xn::florentine_families_graph();
        xn::set_edge_attributes(G, 1, "capacity");
        for (auto flow_func : flow_funcs) {
            T = xn::gomory_hu_tree(G, flow_func=flow_func);
            assert_true(xn::is_tree(T));
            for (auto [u, v] : combinations(G, 2) {
                cut_value, edge = this->minimum_edge_weight(T, u, v);
                assert_equal(xn::minimum_cut_value(G, u, v),
                             cut_value);

    auto test_karate_club_graph_cutset() {
        G = xn::karate_club_graph();
        xn::set_edge_attributes(G, 1, "capacity");
        T = xn::gomory_hu_tree(G);
        assert_true(xn::is_tree(T));
        auto [u, v] = 0, 33
        cut_value, edge = this->minimum_edge_weight(T, u, v);
        cutset = this->compute_cutset(G, T, edge);
        assert_equal(cut_value, len(cutset));

    auto test_wikipedia_example() {
        // Example from https://en.wikipedia.org/wiki/Gomory%E2%80%93Hu_tree
        G = xn::Graph();
        G.add_weighted_edges_from((
            (0, 1, 1), (0, 2, 7), (1, 2, 1),
            (1, 3, 3), (1, 4, 2), (2, 4, 4),
            (3, 4, 1), (3, 5, 6), (4, 5, 2),
        ));
        for (auto flow_func : flow_funcs) {
            T = xn::gomory_hu_tree(G, capacity="weight", flow_func=flow_func);
            assert_true(xn::is_tree(T));
            for (auto [u, v] : combinations(G, 2) {
                cut_value, edge = this->minimum_edge_weight(T, u, v);
                assert_equal(xn::minimum_cut_value(G, u, v, capacity="weight"),
                             cut_value);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_directed_raises() {
        G = xn::DiGraph();
        T = xn::gomory_hu_tree(G);

    /// /// @raises(xn::XNetworkError);
    auto test_empty_raises() {
        G = xn::empty_graph();
        T = xn::gomory_hu_tree(G);
