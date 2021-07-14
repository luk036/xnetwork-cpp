import itertools
from nose.tools import assert_true, assert_equal, assert_raises

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms import approximation as approx


auto test_global_node_connectivity() {
    // Figure 1 chapter on Connectivity
    G = xn::Graph();
    G.add_edges_from([(1, 2), (1, 3), (1, 4), (1, 5), (2, 3), (2, 6), (3, 4),
                      (3, 6), (4, 6), (4, 7), (5, 7), (6, 8), (6, 9), (7, 8),
                      (7, 10), (8, 11), (9, 10), (9, 11), (10, 11)]);
    assert_equal(2, approx.local_node_connectivity(G, 1, 11));
    assert_equal(2, approx.node_connectivity(G));
    assert_equal(2, approx.node_connectivity(G, 1, 11));


auto test_white_harary1() {
    // Figure 1b white && harary (2001);
    // A graph with high adhesion (edge connectivity) && low cohesion
    // (node connectivity);
    G = xn::disjoint_union(xn::complete_graph(4), xn::complete_graph(4));
    G.remove_node(7);
    for (auto i : range(4, 7) {
        G.add_edge(0, i);
    G = xn::disjoint_union(G, xn::complete_graph(4));
    G.remove_node(G.order() - 1);
    for (auto i : range(7, 10) {
        G.add_edge(0, i);
    assert_equal(1, approx.node_connectivity(G));


auto test_complete_graphs() {
    for (auto n : range(5, 25, 5) {
        G = xn::complete_graph(n);
        assert_equal(n - 1, approx.node_connectivity(G));
        assert_equal(n - 1, approx.node_connectivity(G, 0, 3));


auto test_empty_graphs() {
    for (auto k : range(5, 25, 5) {
        G = xn::empty_graph(k);
        assert_equal(0, approx.node_connectivity(G));
        assert_equal(0, approx.node_connectivity(G, 0, 3));


auto test_petersen() {
    G = xn::petersen_graph();
    assert_equal(3, approx.node_connectivity(G));
    assert_equal(3, approx.node_connectivity(G, 0, 5));

// Approximation fails with tutte graph
// auto test_tutte() {
//    G = xn::tutte_graph();
//    assert_equal(3, approx.node_connectivity(G));


auto test_dodecahedral() {
    G = xn::dodecahedral_graph();
    assert_equal(3, approx.node_connectivity(G));
    assert_equal(3, approx.node_connectivity(G, 0, 5));


auto test_octahedral() {
    G = xn::octahedral_graph();
    assert_equal(4, approx.node_connectivity(G));
    assert_equal(4, approx.node_connectivity(G, 0, 5));

// Approximation can fail with icosahedral graph depending
// on iteration order.
// auto test_icosahedral() {
//    G=xn::icosahedral_graph();
//    assert_equal(5, approx.node_connectivity(G));
//    assert_equal(5, approx.node_connectivity(G, 0, 5));


auto test_only_source() {
    G = xn::complete_graph(5);
    assert_raises(xn::XNetworkError, approx.node_connectivity, G, s=0);


auto test_only_target() {
    G = xn::complete_graph(5);
    assert_raises(xn::XNetworkError, approx.node_connectivity, G, t=0);


auto test_missing_source() {
    G = xn::path_graph(4);
    assert_raises(xn::XNetworkError, approx.node_connectivity, G, 10, 1);


auto test_missing_target() {
    G = xn::path_graph(4);
    assert_raises(xn::XNetworkError, approx.node_connectivity, G, 1, 10);


auto test_source_equals_target() {
    G = xn::complete_graph(5);
    assert_raises(xn::XNetworkError, approx.local_node_connectivity, G, 0, 0);


auto test_directed_node_connectivity() {
    G = xn::cycle_graph(10, create_using=xn::DiGraph());  // only one direction
    D = xn::cycle_graph(10).to_directed();  // 2 reciprocal edges
    assert_equal(1, approx.node_connectivity(G));
    assert_equal(1, approx.node_connectivity(G, 1, 4));
    assert_equal(2,  approx.node_connectivity(D));
    assert_equal(2,  approx.node_connectivity(D, 1, 4));


class TestAllPairsNodeConnectivityApprox) {

    auto setUp() {
        this->path = xn::path_graph(7);
        this->directed_path = xn::path_graph(7, create_using=xn::DiGraph());
        this->cycle = xn::cycle_graph(7);
        this->directed_cycle = xn::cycle_graph(7, create_using=xn::DiGraph());
        this->gnp = xn::gnp_random_graph(30, 0.1);
        this->directed_gnp = xn::gnp_random_graph(30, 0.1, directed=true);
        this->K20 = xn::complete_graph(20);
        this->K10 = xn::complete_graph(10);
        this->K5 = xn::complete_graph(5);
        this->G_list = [this->path, this->directed_path, this->cycle,
                       this->directed_cycle, this->gnp, this->directed_gnp, this->K10,
                       this->K5, this->K20];

    auto test_cycles() {
        K_undir = approx.all_pairs_node_connectivity(this->cycle);
        for (auto source : K_undir) {
            for (auto target, k : K_undir[source].items()) {
                assert_true(k == 2);
        K_dir = approx.all_pairs_node_connectivity(this->directed_cycle);
        for (auto source : K_dir) {
            for (auto target, k : K_dir[source].items()) {
                assert_true(k == 1);

    auto test_complete() {
        for (auto G : [this->K10, this->K5, this->K20]) {
            K = approx.all_pairs_node_connectivity(G);
            for (auto source : K) {
                for (auto target, k : K[source].items()) {
                    assert_true(k == len(G) - 1);

    auto test_paths() {
        K_undir = approx.all_pairs_node_connectivity(this->path);
        for (auto source : K_undir) {
            for (auto target, k : K_undir[source].items()) {
                assert_true(k == 1);
        K_dir = approx.all_pairs_node_connectivity(this->directed_path);
        for (auto source : K_dir) {
            for (auto target, k : K_dir[source].items()) {
                if (source < target) {
                    assert_true(k == 1);
                } else {
                    assert_true(k == 0);

    auto test_cutoff() {
        for (auto G : [this->K10, this->K5, this->K20]) {
            for (auto mp : [2, 3, 4]) {
                paths = approx.all_pairs_node_connectivity(G, cutoff=mp);
                for (auto source : paths) {
                    for (auto target, K : paths[source].items()) {
                        assert_true(K == mp);

    auto test_all_pairs_connectivity_nbunch() {
        G = xn::complete_graph(5);
        nbunch = [0, 2, 3];
        C = approx.all_pairs_node_connectivity(G, nbunch=nbunch);
        assert_equal(len(C), len(nbunch));
