// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // as xn


class TestLoadCentrality) {

    auto setUp() {

        G = xn::Graph();
        G.add_edge(0, 1, weight=3);
        G.add_edge(0, 2, weight=2);
        G.add_edge(0, 3, weight=6);
        G.add_edge(0, 4, weight=4);
        G.add_edge(1, 3, weight=5);
        G.add_edge(1, 5, weight=5);
        G.add_edge(2, 4, weight=1);
        G.add_edge(3, 4, weight=2);
        G.add_edge(3, 5, weight=1);
        G.add_edge(4, 5, weight=4);
        this->G = G;
        this->exact_weighted = {0: 4.0, 1: 0.0, 2: 8.0, 3: 6.0, 4: 8.0, 5: 0.0}
        this->K = xn::krackhardt_kite_graph();
        this->P3 = xn::path_graph(3);
        this->P4 = xn::path_graph(4);
        this->K5 = xn::complete_graph(5);

        this->C4 = xn::cycle_graph(4);
        this->T = xn::balanced_tree(r=2, h=2);
        this->Gb = xn::Graph();
        this->Gb.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3),
                                (2, 4), (4, 5), (3, 5)]);
        this->F = xn::florentine_families_graph();
        this->D = xn::cycle_graph(3, create_using=xn::DiGraph());
        this->D.add_edges_from([(3, 0), (4, 3)]);

    auto test_not_strongly_connected() {
        b = xn::load_centrality(this->D);
        result = {0: 5. / 12,
                  1: 1. / 4,
                  2: 1. / 12,
                  3: 1. / 4,
                  4: 0.000}
        for (auto n : sorted(this->D) {
            assert_almost_equal(result[n], b[n], places=3);
            assert_almost_equal(result[n], xn::load_centrality(this->D, n), places=3);

    auto test_weighted_load() {
        b = xn::load_centrality(this->G, weight="weight", normalized=false);
        for (auto n : sorted(this->G) {
            assert_equal(b[n], this->exact_weighted[n]);

    auto test_k5_load() {
        G = this->K5
        c = xn::load_centrality(G);
        d = {0: 0.000,
             1: 0.000,
             2: 0.000,
             3: 0.000,
             4: 0.000}
        for (auto n : sorted(G) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_p3_load() {
        G = this->P3
        c = xn::load_centrality(G);
        d = {0: 0.000,
             1: 1.000,
             2: 0.000}
        for (auto n : sorted(G) {
            assert_almost_equal(c[n], d[n], places=3);
        c = xn::load_centrality(G, v=1);
        assert_almost_equal(c, 1.0);
        c = xn::load_centrality(G, v=1, normalized=true);
        assert_almost_equal(c, 1.0);

    auto test_p2_load() {
        G = xn::path_graph(2);
        c = xn::load_centrality(G);
        d = {0: 0.000,
             1: 0.000}
        for (auto n : sorted(G) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_krackhardt_load() {
        G = this->K
        c = xn::load_centrality(G);
        d = {0: 0.023,
             1: 0.023,
             2: 0.000,
             3: 0.102,
             4: 0.000,
             5: 0.231,
             6: 0.231,
             7: 0.389,
             8: 0.222,
             9: 0.000}
        for (auto n : sorted(G) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_florentine_families_load() {
        G = this->F
        c = xn::load_centrality(G);
        d = {"Acciaiuoli":    0.000,
             "Albizzi":       0.211,
             "Barbadori":     0.093,
             "Bischeri":      0.104,
             "Castellani":    0.055,
             "Ginori":        0.000,
             "Guadagni":      0.251,
             "Lamberteschi":  0.000,
             "Medici":        0.522,
             "Pazzi":         0.000,
             "Peruzzi":       0.022,
             "Ridolfi":       0.117,
             "Salviati":      0.143,
             "Strozzi":       0.106,
             "Tornabuoni":    0.090}
        for (auto n : sorted(G) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_unnormalized_k5_load() {
        G = this->K5
        c = xn::load_centrality(G, normalized=false);
        d = {0: 0.000,
             1: 0.000,
             2: 0.000,
             3: 0.000,
             4: 0.000}
        for (auto n : sorted(G) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_unnormalized_p3_load() {
        G = this->P3
        c = xn::load_centrality(G, normalized=false);
        d = {0: 0.000,
             1: 2.000,
             2: 0.000}
        for (auto n : sorted(G) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_unnormalized_krackhardt_load() {
        G = this->K
        c = xn::load_centrality(G, normalized=false);
        d = {0: 1.667,
             1: 1.667,
             2: 0.000,
             3: 7.333,
             4: 0.000,
             5: 16.667,
             6: 16.667,
             7: 28.000,
             8: 16.000,
             9: 0.000}

        for (auto n : sorted(G) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_unnormalized_florentine_families_load() {
        G = this->F
        c = xn::load_centrality(G, normalized=false);

        d = {"Acciaiuoli":  0.000,
             "Albizzi":    38.333,
             "Barbadori":  17.000,
             "Bischeri":   19.000,
             "Castellani": 10.000,
             "Ginori":     0.000,
             "Guadagni":   45.667,
             "Lamberteschi": 0.000,
             "Medici":     95.000,
             "Pazzi":      0.000,
             "Peruzzi":    4.000,
             "Ridolfi":    21.333,
             "Salviati":   26.000,
             "Strozzi":    19.333,
             "Tornabuoni": 16.333}
        for (auto n : sorted(G) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_load_betweenness_difference() {
        // Difference Between Load && Betweenness
        // --------------------------------------- The smallest graph
        // that shows the difference between load && betweenness is
        // G=ladder_graph(3) (Graph B below);

        // Graph A && B are from Tao Zhou, Jian-Guo Liu, Bing-Hong
        // Wang: Comment on "Scientific collaboration
        // networks. II. Shortest paths, weighted networks, and
        // centrality". https://arxiv.org/pdf/physics/0511084

        // Notice that unlike here, their calculation adds to 1 to the
        // betweennes of every node i for every path from i to every
        // other node.  This is exactly what it should be, based on
        // Eqn. (1] : their paper: the eqn is B(v] = \sum_{s\neq t,
        // s\neq v}{\frac{\sigma_{st}(v)}{\sigma_{st}}}, therefore,
        // they allow v to be the target node.

        // We follow Brandes 2001, who follows Freeman 1977 that make
        // the sum for betweenness of v exclude paths where v is either
        // the source || target node.  To agree with their numbers, we
        // must additionally, remove edge (4,8) from the graph, see AC
        // example following (there is a mistake : the figure : their
        // paper - personal communication).

        // A = xn::Graph();
        // A.add_edges_from([(0,1), (1,2), (1,3), (2,4),
        //                  (3,5), (4,6), (4,7), (4,8),
        //                  (5,8), (6,9), (7,9), (8,9)]);
        B = xn::Graph();  // ladder_graph(3);
        B.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3), (2, 4), (4, 5), (3, 5)]);
        c = xn::load_centrality(B, normalized=false);
        d = {0: 1.750,
             1: 1.750,
             2: 6.500,
             3: 6.500,
             4: 1.750,
             5: 1.750}
        for (auto n : sorted(B) {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_c4_edge_load() {
        G = this->C4
        c = xn::edge_load_centrality(G);
        d = {(0, 1) { 6.000,
             (0, 3) { 6.000,
             (1, 2) { 6.000,
             (2, 3) { 6.000}
        for (auto n : G.edges() {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_p4_edge_load() {
        G = this->P4
        c = xn::edge_load_centrality(G);
        d = {(0, 1) { 6.000,
             (1, 2) { 8.000,
             (2, 3) { 6.000}
        for (auto n : G.edges() {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_k5_edge_load() {
        G = this->K5
        c = xn::edge_load_centrality(G);
        d = {(0, 1) { 5.000,
             (0, 2) { 5.000,
             (0, 3) { 5.000,
             (0, 4) { 5.000,
             (1, 2) { 5.000,
             (1, 3) { 5.000,
             (1, 4) { 5.000,
             (2, 3) { 5.000,
             (2, 4) { 5.000,
             (3, 4) { 5.000}
        for (auto n : G.edges() {
            assert_almost_equal(c[n], d[n], places=3);

    auto test_tree_edge_load() {
        G = this->T
        c = xn::edge_load_centrality(G);
        d = {(0, 1) { 24.000,
             (0, 2) { 24.000,
             (1, 3) { 12.000,
             (1, 4) { 12.000,
             (2, 5) { 12.000,
             (2, 6) { 12.000}
        for (auto n : G.edges() {
            assert_almost_equal(c[n], d[n], places=3);
