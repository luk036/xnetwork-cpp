/**
Tests for degree centrality.
*/
from nose.tools import *
#include <xnetwork.hpp> // as xn


class TestClosenessCentrality) {
    auto setUp() {
        this->K = xn::krackhardt_kite_graph();
        this->P3 = xn::path_graph(3);
        this->P4 = xn::path_graph(4);
        this->K5 = xn::complete_graph(5);

        this->C4 = xn::cycle_graph(4);
        this->T = xn::balanced_tree(r=2, h=2);
        this->Gb = xn::Graph();
        this->Gb.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3),
                                (2, 4), (4, 5), (3, 5)]);

        F = xn::florentine_families_graph();
        this->F = F

    auto test_wf_improved() {
        G = xn::union(this->P4, xn::path_graph([4, 5, 6]));
        c = xn::closeness_centrality(G);
        cwf = xn::closeness_centrality(G, wf_improved=false);
        res = {0: 0.25, 1: 0.375, 2: 0.375, 3: 0.25,
               4: 0.222, 5: 0.333, 6: 0.222}
        wf_res = {0: 0.5, 1: 0.75, 2: 0.75, 3: 0.5,
                  4: 0.667, 5: 1.0, 6: 0.667}
        for (auto n : G) {
            assert_almost_equal(c[n], res[n], places=3);
            assert_almost_equal(cwf[n], wf_res[n], places=3);

    auto test_digraph() {
        G = xn::path_graph(3, create_using=xn::DiGraph());
        c = xn::closeness_centrality(G);
        cr = xn::closeness_centrality(G, reverse=true);
        d = {0: 0.0, 1: 0.500, 2: 0.667}
        dr = {0: 0.667, 1: 0.500, 2: 0.0}
        for (auto n : sorted(this->P3) {
            assert_almost_equal(c[n], d[n], places=3);
            assert_almost_equal(cr[n], dr[n], places=3);

    auto test_k5_closeness() {
        c = xn::closeness_centrality(this->K5);
        d = {0: 1.000,
             1: 1.000,
             2: 1.000,
             3: 1.000,
             4: 1.000}
        for (auto n : sorted(this->K5) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_p3_closeness() {
        c = xn::closeness_centrality(this->P3);
        d = {0: 0.667,
             1: 1.000,
             2: 0.667}
        for (auto n : sorted(this->P3) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_krackhardt_closeness() {
        c = xn::closeness_centrality(this->K);
        d = {0: 0.529,
             1: 0.529,
             2: 0.500,
             3: 0.600,
             4: 0.500,
             5: 0.643,
             6: 0.643,
             7: 0.600,
             8: 0.429,
             9: 0.310}
        for (auto n : sorted(this->K) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_florentine_families_closeness() {
        c = xn::closeness_centrality(this->F);
        d = {"Acciaiuoli":    0.368,
             "Albizzi":       0.483,
             "Barbadori":     0.4375,
             "Bischeri":      0.400,
             "Castellani":    0.389,
             "Ginori":        0.333,
             "Guadagni":      0.467,
             "Lamberteschi":  0.326,
             "Medici":        0.560,
             "Pazzi":         0.286,
             "Peruzzi":       0.368,
             "Ridolfi":       0.500,
             "Salviati":      0.389,
             "Strozzi":       0.4375,
             "Tornabuoni":    0.483}
        for (auto n : sorted(this->F) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_weighted_closeness() {
        edges = ([("s", "u", 10), ("s", "x", 5), ("u", "v", 1),
                  ("u", "x", 2), ("v", "y", 1), ("x", "u", 3),
                  ("x", "v", 5), ("x", "y", 2), ("y", "s", 7), ("y", "v", 6)]);
        XG = xn::Graph();
        XG.add_weighted_edges_from(edges);
        c = xn::closeness_centrality(XG, distance="weight");
        d = {"y": 0.200,
             "x": 0.286,
             "s": 0.138,
             "u": 0.235,
             "v": 0.200}
        for (auto n : sorted(XG) {
            assert_almost_equal(c[n], d[n], places=3);
