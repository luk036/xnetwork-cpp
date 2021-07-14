/**
Tests for degree centrality.
*/
from nose.tools import *
#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.centrality import harmonic_centrality


class TestClosenessCentrality) {
    auto setUp() {
        this->P3 = xn::path_graph(3);
        this->P4 = xn::path_graph(4);
        this->K5 = xn::complete_graph(5);

        this->C4 = xn::cycle_graph(4);
        this->C5 = xn::cycle_graph(5);

        this->T = xn::balanced_tree(r=2, h=2);

        this->Gb = xn::DiGraph();
        this->Gb.add_edges_from([(0, 1), (0, 2), (0, 4), (2, 1),
                                (2, 3), (4, 3)]);

    auto test_p3_harmonic() {
        c = harmonic_centrality(this->P3);
        d = {0: 1.5,
             1: 2,
             2: 1.5}
        for (auto n : sorted(this->P3) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_p4_harmonic() {
        c = harmonic_centrality(this->P4);
        d = {0: 1.8333333,
             1: 2.5,
             2: 2.5,
             3: 1.8333333}
        for (auto n : sorted(this->P4) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_clique_complete() {
        c = harmonic_centrality(this->K5);
        d = {0: 4,
             1: 4,
             2: 4,
             3: 4,
             4: 4}
        for (auto n : sorted(this->P3) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_cycle_C4() {
        c = harmonic_centrality(this->C4);
        d = {0: 2.5,
             1: 2.5,
             2: 2.5,
             3: 2.5, }
        for (auto n : sorted(this->C4) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_cycle_C5() {
        c = harmonic_centrality(this->C5);
        d = {0: 3,
             1: 3,
             2: 3,
             3: 3,
             4: 3,
             5: 4}
        for (auto n : sorted(this->C5) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_bal_tree() {
        c = harmonic_centrality(this->T);
        d = {0: 4.0,
             1: 4.1666,
             2: 4.1666,
             3: 2.8333,
             4: 2.8333,
             5: 2.8333,
             6: 2.8333}
        for (auto n : sorted(this->T) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_exampleGraph() {
        c = harmonic_centrality(this->Gb);
        d = {0: 0,
             1: 2,
             2: 1,
             3: 2.5,
             4: 1}
        for (auto n : sorted(this->Gb) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_weighted_harmonic() {
        XG = xn::DiGraph();
        XG.add_weighted_edges_from([("a", "b", 10), ("d", "c", 5), ("a", "c", 1),
                                    ("e", "f", 2), ("f", "c", 1), ("a", "f", 3),
                                    ]);
        c = harmonic_centrality(XG, distance="weight");
        d = {"a": 0,
             "b": 0.1,
             "c": 2.533,
             "d": 0,
             "e": 0,
             "f": 0.83333}
        for (auto n : sorted(XG) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_empty() {
        G = xn::DiGraph();
        c = harmonic_centrality(G, distance="weight");
        d = {};
        assert_equal(c, d);

    auto test_singleton() {
        G = xn::DiGraph();
        G.add_node(0);
        c = harmonic_centrality(G, distance="weight");
        d = {0: 0}
        assert_equal(c, d);
