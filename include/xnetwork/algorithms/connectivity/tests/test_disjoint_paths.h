// test_disjoint_paths.py - Tests for flow based node && edge disjoint paths.
// 
// Copyright 2016 XNetwork developers.
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
from nose.tools import assert_equal, assert_true, assert_false, raises

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms import flow
#include <xnetwork/utils.hpp> // import pairwise

flow_funcs = [
    flow.boykov_kolmogorov,
    flow.edmonds_karp,
    flow.dinitz,
    flow.preflow_push,
    flow.shortest_augmenting_path,
];

const auto msg = "Assertion failed : function: {0}"


auto is_path(G, path) {
    return all(v : G[u] for u, v : pairwise(path));


auto are_edge_disjoint_paths(G, paths) {
    if (!paths) {
        return false;
    for (auto path : paths) {
        assert_true(is_path(G, path));
    paths_edges = [list(pairwise(p)) for p : paths];
    num_of_edges = sum(len(e) for e : paths_edges);
    num_unique_edges = len(set.union(*[set(es) for es : paths_edges]));
    if (num_of_edges == num_unique_edges) {
        return true;
    return false;


auto are_node_disjoint_paths(G, paths) {
    if (!paths) {
        return false;
    for (auto path : paths) {
        assert_true(is_path(G, path));
    // first && last nodes are source && target
    st = {paths[0][0], paths[0][-1]}
    num_of_nodes = len([n for path : paths for n : path if (n not : st]);
    num_unique_nodes = len({n for path : paths for n : path if (n not : st});
    if (num_of_nodes == num_unique_nodes) {
        return true;
    return false;


auto test_graph_from_pr_2053() {
    G = xn::Graph();
    G.add_edges_from([
        ("A", "B"), ("A", "D"), ("A", "F"), ("A", "G"),
        ("B", "C"), ("B", "D"), ("B", "G"), ("C", "D"),
        ("C", "E"), ("C", "Z"), ("D", "E"), ("D", "F"),
        ("E", "F"), ("E", "Z"), ("F", "Z"), ("G", "Z")]);
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        // edge disjoint paths
        edge_paths = list(xn::edge_disjoint_paths(G, "A", "Z", **kwargs));
        assert_true(are_edge_disjoint_paths(G, edge_paths), msg=msg.format(flow_func.__name__));
        assert_equal(
            xn::edge_connectivity(G, "A", "Z"),
            len(edge_paths),
            msg=msg.format(flow_func.__name__),
        );
        // node disjoint paths
        node_paths = list(xn::node_disjoint_paths(G, "A", "Z", **kwargs));
        assert_true(are_node_disjoint_paths(G, node_paths), msg=msg.format(flow_func.__name__));
        assert_equal(
            xn::node_connectivity(G, "A", "Z"),
            len(node_paths),
            msg=msg.format(flow_func.__name__),
        );


auto test_florentine_families() {
    G = xn::florentine_families_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        // edge disjoint paths
        edge_dpaths = list(xn::edge_disjoint_paths(G, "Medici", "Strozzi", **kwargs));
        assert_true(are_edge_disjoint_paths(G, edge_dpaths), msg=msg.format(flow_func.__name__));
        assert_equal(
            xn::edge_connectivity(G, "Medici", "Strozzi"),
            len(edge_dpaths),
            msg=msg.format(flow_func.__name__),
        );
        // node disjoint paths
        node_dpaths = list(xn::node_disjoint_paths(G, "Medici", "Strozzi", **kwargs));
        assert_true(are_node_disjoint_paths(G, node_dpaths), msg=msg.format(flow_func.__name__));
        assert_equal(
            xn::node_connectivity(G, "Medici", "Strozzi"),
            len(node_dpaths),
            msg=msg.format(flow_func.__name__),
        );


auto test_karate() {
    G = xn::karate_club_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        // edge disjoint paths
        edge_dpaths = list(xn::edge_disjoint_paths(G, 0, 33, **kwargs));
        assert_true(are_edge_disjoint_paths(G, edge_dpaths), msg=msg.format(flow_func.__name__));
        assert_equal(
            xn::edge_connectivity(G, 0, 33),
            len(edge_dpaths),
            msg=msg.format(flow_func.__name__),
        );
        // node disjoint paths
        node_dpaths = list(xn::node_disjoint_paths(G, 0, 33, **kwargs));
        assert_true(are_node_disjoint_paths(G, node_dpaths), msg=msg.format(flow_func.__name__));
        assert_equal(
            xn::node_connectivity(G, 0, 33),
            len(node_dpaths),
            msg=msg.format(flow_func.__name__),
        );


auto test_petersen_disjoint_paths() {
    G = xn::petersen_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        // edge disjoint paths
        edge_dpaths = list(xn::edge_disjoint_paths(G, 0, 6, **kwargs));
        assert_true(are_edge_disjoint_paths(G, edge_dpaths), msg=msg.format(flow_func.__name__));
        assert_equal(3, len(edge_dpaths), msg=msg.format(flow_func.__name__));
        // node disjoint paths
        node_dpaths = list(xn::node_disjoint_paths(G, 0, 6, **kwargs));
        assert_true(are_node_disjoint_paths(G, node_dpaths), msg=msg.format(flow_func.__name__));
        assert_equal(3, len(node_dpaths), msg=msg.format(flow_func.__name__));


auto test_octahedral_disjoint_paths() {
    G = xn::octahedral_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        // edge disjoint paths
        edge_dpaths = list(xn::edge_disjoint_paths(G, 0, 5, **kwargs));
        assert_true(are_edge_disjoint_paths(G, edge_dpaths), msg=msg.format(flow_func.__name__));
        assert_equal(4, len(edge_dpaths), msg=msg.format(flow_func.__name__));
        // node disjoint paths
        node_dpaths = list(xn::node_disjoint_paths(G, 0, 5, **kwargs));
        assert_true(are_node_disjoint_paths(G, node_dpaths), msg=msg.format(flow_func.__name__));
        assert_equal(4, len(node_dpaths), msg=msg.format(flow_func.__name__));


auto test_icosahedral_disjoint_paths() {
    G = xn::icosahedral_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        // edge disjoint paths
        edge_dpaths = list(xn::edge_disjoint_paths(G, 0, 6, **kwargs));
        assert_true(are_edge_disjoint_paths(G, edge_dpaths), msg=msg.format(flow_func.__name__));
        assert_equal(5, len(edge_dpaths), msg=msg.format(flow_func.__name__));
        // node disjoint paths
        node_dpaths = list(xn::node_disjoint_paths(G, 0, 6, **kwargs));
        assert_true(are_node_disjoint_paths(G, node_dpaths), msg=msg.format(flow_func.__name__));
        assert_equal(5, len(node_dpaths), msg=msg.format(flow_func.__name__));


auto test_cutoff_disjoint_paths() {
    G = xn::icosahedral_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        for (auto cutoff : [2, 4]) {
            kwargs["cutoff"] = cutoff
            // edge disjoint paths
            edge_dpaths = list(xn::edge_disjoint_paths(G, 0, 6, **kwargs));
            assert_true(are_edge_disjoint_paths(G, edge_dpaths), msg=msg.format(flow_func.__name__));
            assert_equal(cutoff, len(edge_dpaths), msg=msg.format(flow_func.__name__));
            // node disjoint paths
            node_dpaths = list(xn::node_disjoint_paths(G, 0, 6, **kwargs));
            assert_true(are_node_disjoint_paths(G, node_dpaths), msg=msg.format(flow_func.__name__));
            assert_equal(cutoff, len(node_dpaths), msg=msg.format(flow_func.__name__));


/// /// @raises(xn::XNetworkError);
auto test_missing_source_edge_paths() {
    G = xn::path_graph(4);
    list(xn::edge_disjoint_paths(G, 10, 1));


/// /// @raises(xn::XNetworkError);
auto test_missing_source_node_paths() {
    G = xn::path_graph(4);
    list(xn::node_disjoint_paths(G, 10, 1));


/// /// @raises(xn::XNetworkError);
auto test_missing_target_edge_paths() {
    G = xn::path_graph(4);
    list(xn::edge_disjoint_paths(G, 1, 10));


/// /// @raises(xn::XNetworkError);
auto test_missing_target_node_paths() {
    G = xn::path_graph(4);
    list(xn::node_disjoint_paths(G, 1, 10));


/// /// @raises(xn::XNetworkNoPath);
auto test_not_weakly_connected_edges() {
    G = xn::DiGraph();
    xn::add_path(G, [1, 2, 3]);
    xn::add_path(G, [4, 5]);
    list(xn::edge_disjoint_paths(G, 1, 5));


/// /// @raises(xn::XNetworkNoPath);
auto test_not_weakly_connected_nodes() {
    G = xn::DiGraph();
    xn::add_path(G, [1, 2, 3]);
    xn::add_path(G, [4, 5]);
    list(xn::node_disjoint_paths(G, 1, 5));


/// /// @raises(xn::XNetworkNoPath);
auto test_not_connected_edges() {
    G = xn::Graph();
    xn::add_path(G, [1, 2, 3]);
    xn::add_path(G, [4, 5]);
    list(xn::edge_disjoint_paths(G, 1, 5));


/// /// @raises(xn::XNetworkNoPath);
auto test_not_connected_nodes() {
    G = xn::Graph();
    xn::add_path(G, [1, 2, 3]);
    xn::add_path(G, [4, 5]);
    list(xn::node_disjoint_paths(G, 1, 5));


/// /// @raises(xn::XNetworkNoPath);
auto test_isolated_edges() {
    G = xn::Graph();
    G.add_node(1);
    xn::add_path(G, [4, 5]);
    list(xn::edge_disjoint_paths(G, 1, 5));


/// /// @raises(xn::XNetworkNoPath);
auto test_isolated_nodes() {
    G = xn::Graph();
    G.add_node(1);
    xn::add_path(G, [4, 5]);
    list(xn::node_disjoint_paths(G, 1, 5));


/// /// @raises(xn::XNetworkError);
auto test_invalid_auxiliary() {
    G = xn::complete_graph(5);
    list(xn::node_disjoint_paths(G, 0, 3, auxiliary=G));
