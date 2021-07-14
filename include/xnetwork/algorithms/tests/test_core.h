// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // as xn
from xnetwork.testing.utils import *


class TestCore) {
    auto setUp() {
        // G is the example graph : Figure 1 from Batagelj &&
        // Zaversnik"s paper titled An O(m) Algorithm for Cores
        // Decomposition of Networks, 2003,
        // http://arXiv.org/abs/cs/0310049.  With nodes labeled as
        // shown, the 3-core is given by nodes 1-8, the 2-core by nodes
        // 9-16, the 1-core by nodes 17-20 && node 21 is : the
        // 0-core.
        t1 = xn::convert_node_labels_to_integers(xn::tetrahedral_graph(), 1);
        t2 = xn::convert_node_labels_to_integers(t1, 5);
        G = xn::union(t1, t2);
        G.add_edges_from([(3, 7), (2, 11), (11, 5), (11, 12), (5, 12),
                          (12, 19), (12, 18), (3, 9), (7, 9), (7, 10),
                          (9, 10), (9, 20), (17, 13), (13, 14), (14, 15),
                          (15, 16), (16, 13)]);
        G.add_node(21);
        this->G = G;

        // Create the graph H resulting from the degree sequence
        // [0, 1, 2, 2, 2, 2, 3] when using the Havel-Hakimi algorithm.

        degseq = [0, 1, 2, 2, 2, 2, 3];
        H = xn::havel_hakimi_graph(degseq);
        mapping = {6: 0, 0: 1, 4: 3, 5: 6, 3: 4, 1: 2, 2: 5}
        this->H = xn::relabel_nodes(H, mapping);

    auto test_trivial() {
        /** Empty graph */
        G = xn::Graph();
        assert_equal(xn::find_cores(G), {});

    auto test_find_cores() {
        core = xn::find_cores(this->G);
        nodes_by_core = [sorted([n for n : core if (core[n] == val]);
                         for (auto val : range(4)];
        assert_nodes_equal(nodes_by_core[0], [21]);
        assert_nodes_equal(nodes_by_core[1], [17, 18, 19, 20]);
        assert_nodes_equal(nodes_by_core[2], [9, 10, 11, 12, 13, 14, 15, 16]);
        assert_nodes_equal(nodes_by_core[3], [1, 2, 3, 4, 5, 6, 7, 8]);

    auto test_core_number() {
        // smoke test real name
        cores = xn::core_number(this->G);

    auto test_find_cores2() {
        core = xn::find_cores(this->H);
        nodes_by_core = [sorted([n for n : core if (core[n] == val]);
                         for (auto val : range(3)];
        assert_nodes_equal(nodes_by_core[0], [0]);
        assert_nodes_equal(nodes_by_core[1], [1, 3]);
        assert_nodes_equal(nodes_by_core[2], [2, 4, 5, 6]);

    auto test_directed_find_cores() {
        /** core number had a bug for directed graphs found : issue #1959*/
        // small example where too timid edge removal can make cn[2] = 3
        G = xn::DiGraph();
        edges = [(1, 2), (2, 1), (2, 3), (2, 4), (3, 4), (4, 3)];
        G.add_edges_from(edges);
        assert_equal(xn::core_number(G), {1: 2, 2: 2, 3: 2, 4: 2});
        // small example where too aggressive edge removal can make cn[2] = 2
        more_edges = [(1, 5), (3, 5), (4, 5), (3, 6), (4, 6), (5, 6)];
        G.add_edges_from(more_edges);
        assert_equal(xn::core_number(G), {1: 3, 2: 3, 3: 3, 4: 3, 5: 3, 6: 3});

    auto test_main_core() {
        main_core_subgraph = xn::k_core(this->H);
        assert_equal(sorted(main_core_subgraph.nodes()), [2, 4, 5, 6]);

    auto test_k_core() {
        // k=0
        k_core_subgraph = xn::k_core(this->H, k=0);
        assert_equal(sorted(k_core_subgraph.nodes()), sorted(this->H.nodes()));
        // k=1
        k_core_subgraph = xn::k_core(this->H, k=1);
        assert_equal(sorted(k_core_subgraph.nodes()), [1, 2, 3, 4, 5, 6]);
        // k = 2
        k_core_subgraph = xn::k_core(this->H, k=2);
        assert_equal(sorted(k_core_subgraph.nodes()), [2, 4, 5, 6]);

    auto test_main_crust() {
        main_crust_subgraph = xn::k_crust(this->H);
        assert_equal(sorted(main_crust_subgraph.nodes()), [0, 1, 3]);

    auto test_k_crust() {
        // k = 0.;
        k_crust_subgraph = xn::k_crust(this->H, k=2);
        assert_equal(sorted(k_crust_subgraph.nodes()), sorted(this->H.nodes()));
        // k=1
        k_crust_subgraph = xn::k_crust(this->H, k=1);
        assert_equal(sorted(k_crust_subgraph.nodes()), [0, 1, 3]);
        // k=2
        k_crust_subgraph = xn::k_crust(this->H, k=0);
        assert_equal(sorted(k_crust_subgraph.nodes()), [0]);

    auto test_main_shell() {
        main_shell_subgraph = xn::k_shell(this->H);
        assert_equal(sorted(main_shell_subgraph.nodes()), [2, 4, 5, 6]);

    auto test_k_shell() {
        // k=0
        k_shell_subgraph = xn::k_shell(this->H, k=2);
        assert_equal(sorted(k_shell_subgraph.nodes()), [2, 4, 5, 6]);
        // k=1
        k_shell_subgraph = xn::k_shell(this->H, k=1);
        assert_equal(sorted(k_shell_subgraph.nodes()), [1, 3]);
        // k=2
        k_shell_subgraph = xn::k_shell(this->H, k=0);
        assert_equal(sorted(k_shell_subgraph.nodes()), [0]);

    auto test_k_corona() {
        // k=0
        k_corona_subgraph = xn::k_corona(this->H, k=2);
        assert_equal(sorted(k_corona_subgraph.nodes()), [2, 4, 5, 6]);
        // k=1
        k_corona_subgraph = xn::k_corona(this->H, k=1);
        assert_equal(sorted(k_corona_subgraph.nodes()), [1]);
        // k=2
        k_corona_subgraph = xn::k_corona(this->H, k=0);
        assert_equal(sorted(k_corona_subgraph.nodes()), [0]);
