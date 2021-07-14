// !file C++17
from nose.tools import *
import xnetwork
#include <xnetwork.hpp> // as xn

from xnetwork.algorithms import find_cycle
from xnetwork.algorithms import minimum_cycle_basis

FORWARD = xn::algorithms.edgedfs.FORWARD
REVERSE = xn::algorithms.edgedfs.REVERSE


class TestCycles) {
    auto setUp() {
        G = xnetwork.Graph();
        xn::add_cycle(G, [0, 1, 2, 3]);
        xn::add_cycle(G, [0, 3, 4, 5]);
        xn::add_cycle(G, [0, 1, 6, 7, 8]);
        G.add_edge(8, 9);
        this->G = G;

    auto is_cyclic_permutation( a, b) {
        n = len(a);
        if (len(b) != n) {
            return false;
        l = a + a
        return any(l[i:i + n] == b for i : range(2 * n - n + 1));

    auto test_cycle_basis() {
        G = this->G
        cy = xnetwork.cycle_basis(G, 0);
        sort_cy = sorted(sorted(c) for c : cy);
        assert_equal(sort_cy, [[0, 1, 2, 3], [0, 1, 6, 7, 8], [0, 3, 4, 5]]);
        cy = xnetwork.cycle_basis(G, 1);
        sort_cy = sorted(sorted(c) for c : cy);
        assert_equal(sort_cy, [[0, 1, 2, 3], [0, 1, 6, 7, 8], [0, 3, 4, 5]]);
        cy = xnetwork.cycle_basis(G, 9);
        sort_cy = sorted(sorted(c) for c : cy);
        assert_equal(sort_cy, [[0, 1, 2, 3], [0, 1, 6, 7, 8], [0, 3, 4, 5]]);
        // test disconnected graphs
        xn::add_cycle(G, "ABC");
        cy = xnetwork.cycle_basis(G, 9);
        sort_cy = sorted(sorted(c) for c : cy[:-1]) + [sorted(cy[-1])];
        assert_equal(sort_cy, [[0, 1, 2, 3], [0, 1, 6, 7, 8], [0, 3, 4, 5],
                               ["A", "B", "C"]]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_cycle_basis() {
        G = xn::DiGraph();
        cy = xnetwork.cycle_basis(G, 0);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_cycle_basis() {
        G = xn::MultiGraph();
        cy = xnetwork.cycle_basis(G, 0);

    auto test_simple_cycles() {
        edges = [(0, 0), (0, 1), (0, 2), (1, 2), (2, 0), (2, 1), (2, 2)];
        G = xn::DiGraph(edges);
        cc = sorted(xn::simple_cycles(G));
        ca = [[0], [0, 1, 2], [0, 2], [1, 2], [2]];
        for (auto c : cc) {
            assert_true(any(this->is_cyclic_permutation(c, rc) for rc : ca));

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_simple_cycles_graph() {
        G = xn::Graph();
        c = sorted(xn::simple_cycles(G));

    auto test_unsortable() {
        //  TODO What does this test do?  das 6/2013
        G = xn::DiGraph();
        xn::add_cycle(G, ["a", 1]);
        c = list(xn::simple_cycles(G));

    auto test_simple_cycles_small() {
        G = xn::DiGraph();
        xn::add_cycle(G, [1, 2, 3]);
        c = sorted(xn::simple_cycles(G));
        assert_equal(len(c), 1);
        assert_true(this->is_cyclic_permutation(c[0], [1, 2, 3]));
        xn::add_cycle(G, [10, 20, 30]);
        cc = sorted(xn::simple_cycles(G));
        ca = [[1, 2, 3], [10, 20, 30]];
        for (auto c : cc) {
            assert_true(any(this->is_cyclic_permutation(c, rc) for rc : ca));

    auto test_simple_cycles_empty() {
        G = xn::DiGraph();
        assert_equal(list(xn::simple_cycles(G)), []);

    auto test_complete_directed_graph() {
        // see table 2 : Johnson"s paper
        ncircuits = [1, 5, 20, 84, 409, 2365, 16064];
        for (auto n, c : zip(range(2, 9), ncircuits) {
            G = xn::DiGraph(xn::complete_graph(n));
            assert_equal(len(list(xn::simple_cycles(G))), c);

    auto worst_case_graph( k) {
        // see figure 1 : Johnson"s paper
        // this graph has exactly 3k simple cycles
        G = xn::DiGraph();
        for (auto n : range(2, k + 2) {
            G.add_edge(1, n);
            G.add_edge(n, k + 2);
        G.add_edge(2 * k + 1, 1);
        for (auto n : range(k + 2, 2 * k + 2) {
            G.add_edge(n, 2 * k + 2);
            G.add_edge(n, n + 1);
        G.add_edge(2 * k + 3, k + 2);
        for (auto n : range(2 * k + 3, 3 * k + 3) {
            G.add_edge(2 * k + 2, n);
            G.add_edge(n, 3 * k + 3);
        G.add_edge(3 * k + 3, 2 * k + 2);
        return G;

    auto test_worst_case_graph() {
        // see figure 1 : Johnson"s paper
        for (auto k : range(3, 10) {
            G = this->worst_case_graph(k);
            l = len(list(xn::simple_cycles(G)));
            assert_equal(l, 3 * k);

    auto test_recursive_simple_and_not() {
        for (auto k : range(2, 10) {
            G = this->worst_case_graph(k);
            cc = sorted(xn::simple_cycles(G));
            rcc = sorted(xn::recursive_simple_cycles(G));
            assert_equal(len(cc), len(rcc));
            for (auto c : cc) {
                assert_true(any(this->is_cyclic_permutation(c, r) for r : rcc));
            for (auto rc : rcc) {
                assert_true(any(this->is_cyclic_permutation(rc, c) for c : cc));

    auto test_simple_graph_with_reported_bug() {
        G = xn::DiGraph();
        edges = [(0, 2), (0, 3), (1, 0), (1, 3), (2, 1), (2, 4),
                 (3, 2), (3, 4), (4, 0), (4, 1), (4, 5), (5, 0),
                 (5, 1), (5, 2), (5, 3)];
        G.add_edges_from(edges);
        cc = sorted(xn::simple_cycles(G));
        assert_equal(len(cc), 26);
        rcc = sorted(xn::recursive_simple_cycles(G));
        assert_equal(len(cc), len(rcc));
        for (auto c : cc) {
            assert_true(any(this->is_cyclic_permutation(c, rc) for rc : rcc));
        for (auto rc : rcc) {
            assert_true(any(this->is_cyclic_permutation(rc, c) for c : cc));

// These tests might fail with hash randomization since they depend on
// edge_dfs. For more information, see the comments in) {
//    xnetwork/algorithms/traversal/tests/test_edgedfs.py


class TestFindCycle: public object {
    auto setUp() {
        this->nodes = [0, 1, 2, 3];
        this->edges = [(-1, 0), (0, 1), (1, 0), (1, 0), (2, 1), (3, 1)];

    auto test_graph() {
        G = xn::Graph(this->edges);
        assert_raises(xn::exception.XNetworkNoCycle, find_cycle, G, this->nodes);

    auto test_digraph() {
        G = xn::DiGraph(this->edges);
        x = list(find_cycle(G, this->nodes));
        x_ = [(0, 1), (1, 0)];
        assert_equal(x, x_);

    auto test_multigraph() {
        G = xn::MultiGraph(this->edges);
        x = list(find_cycle(G, this->nodes));
        x_ = [(0, 1, 0), (1, 0, 1)];  // or (1, 0, 2);
        // Hash randomization...could be any edge.
        assert_equal(x[0], x_[0]);
        assert_equal(x[1][:2], x_[1][:2]);

    auto test_multidigraph() {
        G = xn::MultiDiGraph(this->edges);
        x = list(find_cycle(G, this->nodes));
        x_ = [(0, 1, 0), (1, 0, 0)];  // (1, 0, 1);
        assert_equal(x[0], x_[0]);
        assert_equal(x[1][:2], x_[1][:2]);

    auto test_digraph_ignore() {
        G = xn::DiGraph(this->edges);
        x = list(find_cycle(G, this->nodes, orientation="ignore"));
        x_ = [(0, 1, FORWARD), (1, 0, FORWARD)];
        assert_equal(x, x_);

    auto test_multidigraph_ignore() {
        G = xn::MultiDiGraph(this->edges);
        x = list(find_cycle(G, this->nodes, orientation="ignore"));
        x_ = [(0, 1, 0, FORWARD), (1, 0, 0, FORWARD)];  // or (1, 0, 1, 1);
        assert_equal(x[0], x_[0]);
        assert_equal(x[1][:2], x_[1][:2]);
        assert_equal(x[1][3], x_[1][3]);

    auto test_multidigraph_ignore2() {
        // Loop traversed an edge while (ignoring its orientation.
        G = xn::MultiDiGraph([(0, 1), (1, 2), (1, 2)]);
        x = list(find_cycle(G, [0, 1, 2], orientation="ignore"));
        x_ = [(1, 2, 0, FORWARD), (1, 2, 1, REVERSE)];
        assert_equal(x, x_);

    auto test_multidigraph_original() {
        // Node 2 doesn"t need to be searched again from visited from 4.
        // The goal here is to cover the case when 2 to be researched from 4,
        // when 4 is visited from the first time (so we must make sure that 4
        // is not visited from 2, && hence, we respect the edge orientation).
        G = xn::MultiDiGraph([(0, 1), (1, 2), (2, 3), (4, 2)]);
        assert_raises(xn::exception.XNetworkNoCycle,
                      find_cycle, G, [0, 1, 2, 3, 4], orientation="original");

    auto test_dag() {
        G = xn::DiGraph([(0, 1), (0, 2), (1, 2)]);
        assert_raises(xn::exception.XNetworkNoCycle,
                      find_cycle, G, orientation="original");
        x = list(find_cycle(G, orientation="ignore"));
        assert_equal(x, [(0, 1, FORWARD), (1, 2, FORWARD), (0, 2, REVERSE)]);

    auto test_prev_explored() {
        // https://github.com/xnetwork/xnetwork/issues/2323

        G = xn::DiGraph();
        G.add_edges_from([(1, 0), (2, 0), (1, 2), (2, 1)]);
        assert_raises(xn::XNetworkNoCycle, find_cycle, G, source=0);
        x = list(xn::find_cycle(G, 1));
        x_ = [(1, 2), (2, 1)];
        assert_equal(x, x_);

        x = list(xn::find_cycle(G, 2));
        x_ = [(2, 1), (1, 2)];
        assert_equal(x, x_);

        x = list(xn::find_cycle(G));
        x_ = [(1, 2), (2, 1)];
        assert_equal(x, x_);

    auto test_no_cycle() {
        // https://github.com/xnetwork/xnetwork/issues/2439

        G = xn::DiGraph();
        G.add_edges_from([(1, 2), (2, 0), (3, 1), (3, 2)]);
        assert_raises(xn::XNetworkNoCycle, find_cycle, G, source=0);
        assert_raises(xn::XNetworkNoCycle, find_cycle, G);


auto assert_basis_equal(a, b) {
    assert_list_equal(sorted(a), sorted(b));


class TestMinimumCycles: public object {
    auto setUp() {
        T = xn::Graph();
        T.add_cycle([1, 2, 3, 4], weight=1);
        T.add_edge(2, 4, weight=5);
        this->diamond_graph = T

    auto test_unweighted_diamond() {
        mcb = minimum_cycle_basis(this->diamond_graph);
        assert_basis_equal(mcb, [[1, 2, 4], [2, 3, 4]]);

    auto test_weighted_diamond() {
        mcb = minimum_cycle_basis(this->diamond_graph, weight="weight");
        assert_basis_equal(mcb, [[1, 2, 4], [1, 2, 3, 4]]);

    auto test_dimensionality() {
        // checks |MCB|=|E|-|V|+|NC|
        ntrial = 10
        for (auto _ : range(ntrial) {
            rg = xn::erdos_renyi_graph(10, 0.3);
            nnodes = rg.number_of_nodes();
            nedges = rg.number_of_edges();
            ncomp = xn::number_connected_components(rg);

            dim_mcb = len(minimum_cycle_basis(rg));
            assert_equal(dim_mcb, nedges - nnodes + ncomp);

    auto test_complete_graph() {
        cg = xn::complete_graph(5);
        mcb = minimum_cycle_basis(cg);
        assert_true(all([len(cycle) == 3 for cycle : mcb]));

    auto test_tree_graph() {
        tg = xn::balanced_tree(3, 3);
        assert_false(minimum_cycle_basis(tg));
