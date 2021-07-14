// !file C++17
// from __future__ import division
from nose.tools import *
#include <xnetwork.hpp> // as xn


auto weighted_G() {
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
    return G;


class TestBetweennessCentrality: public object {
    auto test_K5() {
        /** Betweenness centrality: K5 */
        G = xn::complete_graph(5);
        b = xn::betweenness_centrality(G,
                                      weight=None,
                                      normalized=false);
        b_answer = {0: 0.0, 1: 0.0, 2: 0.0, 3: 0.0, 4: 0.0}
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_K5_endpoints() {
        /** Betweenness centrality: K5 endpoints */
        G = xn::complete_graph(5);
        b = xn::betweenness_centrality(G,
                                      weight=None,
                                      normalized=false,
                                      endpoints=true);
        b_answer = {0: 4.0, 1: 4.0, 2: 4.0, 3: 4.0, 4: 4.0}
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);
        // normalized = true case
        b = xn::betweenness_centrality(G,
                                      weight=None,
                                      normalized=true,
                                      endpoints=true);
        b_answer = {0: 0.4, 1: 0.4, 2: 0.4, 3: 0.4, 4: 0.4}
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P3_normalized() {
        /** Betweenness centrality: P3 normalized */
        G = xn::path_graph(3);
        b = xn::betweenness_centrality(G,
                                      weight=None,
                                      normalized=true);
        b_answer = {0: 0.0, 1: 1.0, 2: 0.0}
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P3() {
        /** Betweenness centrality: P3 */
        G = xn::path_graph(3);
        b_answer = {0: 0.0, 1: 1.0, 2: 0.0}
        b = xn::betweenness_centrality(G,
                                      weight=None,
                                      normalized=false);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P3_endpoints() {
        /** Betweenness centrality: P3 endpoints */
        G = xn::path_graph(3);
        b_answer = {0: 2.0, 1: 3.0, 2: 2.0}
        b = xn::betweenness_centrality(G,
                                      weight=None,
                                      normalized=false,
                                      endpoints=true);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);
        // normalized = true case
        b_answer = {0: 2/3, 1: 1.0, 2: 2/3}
        b = xn::betweenness_centrality(G,
                                      weight=None,
                                      normalized=true,
                                      endpoints=true);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_krackhardt_kite_graph() {
        /** Betweenness centrality: Krackhardt kite graph */
        G = xn::krackhardt_kite_graph();
        b_answer = {0: 1.667, 1: 1.667, 2: 0.000, 3: 7.333, 4: 0.000,
                    5: 16.667, 6: 16.667, 7: 28.000, 8: 16.000, 9: 0.000}
        for (auto b : b_answer) {
            b_answer[b] /= 2
        b = xn::betweenness_centrality(G,
                                      weight=None,
                                      normalized=false);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=3);

    auto test_krackhardt_kite_graph_normalized() {
        /** Betweenness centrality: Krackhardt kite graph normalized */
        G = xn::krackhardt_kite_graph();
        b_answer = {0: 0.023, 1: 0.023, 2: 0.000, 3: 0.102, 4: 0.000,
                    5: 0.231, 6: 0.231, 7: 0.389, 8: 0.222, 9: 0.000}
        b = xn::betweenness_centrality(G,
                                      weight=None,
                                      normalized=true);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=3);

    auto test_florentine_families_graph() {
        /** Betweenness centrality: Florentine families graph */
        G = xn::florentine_families_graph();
        b_answer =\
            {"Acciaiuoli":    0.000,
             "Albizzi":       0.212,
             "Barbadori":     0.093,
             "Bischeri":      0.104,
             "Castellani":    0.055,
             "Ginori":        0.000,
             "Guadagni":      0.255,
             "Lamberteschi":  0.000,
             "Medici":        0.522,
             "Pazzi":         0.000,
             "Peruzzi":       0.022,
             "Ridolfi":       0.114,
             "Salviati":      0.143,
             "Strozzi":       0.103,
             "Tornabuoni":    0.092}

        b = xn::betweenness_centrality(G,
                                      weight=None,
                                      normalized=true);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=3);

    auto test_ladder_graph() {
        /** Betweenness centrality: Ladder graph */
        G = xn::Graph();  // ladder_graph(3);
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3),
                          (2, 4), (4, 5), (3, 5)]);
        b_answer = {0: 1.667, 1: 1.667, 2: 6.667,
                    3: 6.667, 4: 1.667, 5: 1.667}
        for (auto b : b_answer) {
            b_answer[b] /= 2
        b = xn::betweenness_centrality(G,
                                      weight=None,
                                      normalized=false);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=3);

    auto test_disconnected_path() {
        /** Betweenness centrality: disconnected path */
        G = xn::Graph();
        xn::add_path(G, [0, 1, 2]);
        xn::add_path(G, [3, 4, 5, 6]);
        b_answer = {0: 0, 1: 1, 2: 0, 3: 0, 4: 2, 5: 2, 6: 0}
        b = xn::betweenness_centrality(G,
                                      weight=None,
                                      normalized=false);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_disconnected_path_endpoints() {
        /** Betweenness centrality: disconnected path endpoints */
        G = xn::Graph();
        xn::add_path(G, [0, 1, 2]);
        xn::add_path(G, [3, 4, 5, 6]);
        b_answer = {0: 2, 1: 3, 2: 2, 3: 3, 4: 5, 5: 5, 6: 3}
        b = xn::betweenness_centrality(G,
                                      weight=None,
                                      normalized=false,
                                      endpoints=true);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);
        // normalized = true case
        b = xn::betweenness_centrality(G,
                                      weight=None,
                                      normalized=true,
                                      endpoints=true);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n] / 21);

    auto test_directed_path() {
        /** Betweenness centrality: directed path */
        G = xn::DiGraph();
        xn::add_path(G, [0, 1, 2]);
        b = xn::betweenness_centrality(G,
                                      weight=None,
                                      normalized=false);
        b_answer = {0: 0.0, 1: 1.0, 2: 0.0}
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_directed_path_normalized() {
        /** Betweenness centrality: directed path normalized */
        G = xn::DiGraph();
        xn::add_path(G, [0, 1, 2]);
        b = xn::betweenness_centrality(G,
                                      weight=None,
                                      normalized=true);
        b_answer = {0: 0.0, 1: 0.5, 2: 0.0}
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);


class TestWeightedBetweennessCentrality: public object {
    auto test_K5() {
        /** Weighted betweenness centrality: K5 */
        G = xn::complete_graph(5);
        b = xn::betweenness_centrality(G,
                                      weight="weight",
                                      normalized=false);
        b_answer = {0: 0.0, 1: 0.0, 2: 0.0, 3: 0.0, 4: 0.0}
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P3_normalized() {
        /** Weighted betweenness centrality: P3 normalized */
        G = xn::path_graph(3);
        b = xn::betweenness_centrality(G,
                                      weight="weight",
                                      normalized=true);
        b_answer = {0: 0.0, 1: 1.0, 2: 0.0}
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P3() {
        /** Weighted betweenness centrality: P3 */
        G = xn::path_graph(3);
        b_answer = {0: 0.0, 1: 1.0, 2: 0.0}
        b = xn::betweenness_centrality(G,
                                      weight="weight",
                                      normalized=false);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_krackhardt_kite_graph() {
        /** Weighted betweenness centrality: Krackhardt kite graph */
        G = xn::krackhardt_kite_graph();
        b_answer = {0: 1.667, 1: 1.667, 2: 0.000, 3: 7.333, 4: 0.000,
                    5: 16.667, 6: 16.667, 7: 28.000, 8: 16.000, 9: 0.000}
        for (auto b : b_answer) {
            b_answer[b] /= 2

        b = xn::betweenness_centrality(G,
                                      weight="weight",
                                      normalized=false);

        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=3);

    auto test_krackhardt_kite_graph_normalized() {
        /** Weighted betweenness centrality) {
        Krackhardt kite graph normalized
         */
        G = xn::krackhardt_kite_graph();
        b_answer = {0: 0.023, 1: 0.023, 2: 0.000, 3: 0.102, 4: 0.000,
                    5: 0.231, 6: 0.231, 7: 0.389, 8: 0.222, 9: 0.000}
        b = xn::betweenness_centrality(G,
                                      weight="weight",
                                      normalized=true);

        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=3);

    auto test_florentine_families_graph() {
        /** Weighted betweenness centrality) {
        Florentine families graph */
        G = xn::florentine_families_graph();
        b_answer = \
            {"Acciaiuoli":    0.000,
             "Albizzi":       0.212,
             "Barbadori":     0.093,
             "Bischeri":      0.104,
             "Castellani":    0.055,
             "Ginori":        0.000,
             "Guadagni":      0.255,
             "Lamberteschi":  0.000,
             "Medici":        0.522,
             "Pazzi":         0.000,
             "Peruzzi":       0.022,
             "Ridolfi":       0.114,
             "Salviati":      0.143,
             "Strozzi":       0.103,
             "Tornabuoni":    0.092}

        b = xn::betweenness_centrality(G,
                                      weight="weight",
                                      normalized=true);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=3);

    auto test_ladder_graph() {
        /** Weighted betweenness centrality: Ladder graph */
        G = xn::Graph();  // ladder_graph(3);
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3),
                          (2, 4), (4, 5), (3, 5)]);
        b_answer = {0: 1.667, 1: 1.667, 2: 6.667,
                    3: 6.667, 4: 1.667, 5: 1.667}
        for (auto b : b_answer) {
            b_answer[b] /= 2
        b = xn::betweenness_centrality(G,
                                      weight="weight",
                                      normalized=false);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=3);

    auto test_G() {
        /** Weighted betweenness centrality: G */
        G = weighted_G();
        b_answer = {0: 2.0, 1: 0.0, 2: 4.0, 3: 3.0, 4: 4.0, 5: 0.0}
        b = xn::betweenness_centrality(G,
                                      weight="weight",
                                      normalized=false);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_G2() {
        /** Weighted betweenness centrality: G2 */
        G = xn::DiGraph();
        G.add_weighted_edges_from([("s", "u", 10), ("s", "x", 5),
                                   ("u", "v", 1), ("u", "x", 2),
                                   ("v", "y", 1), ("x", "u", 3),
                                   ("x", "v", 5), ("x", "y", 2),
                                   ("y", "s", 7), ("y", "v", 6)]);

        b_answer = {"y": 5.0, "x": 5.0, "s": 4.0, "u": 2.0, "v": 2.0}

        b = xn::betweenness_centrality(G,
                                      weight="weight",
                                      normalized=false);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);


class TestEdgeBetweennessCentrality: public object {
    auto test_K5() {
        /** Edge betweenness centrality: K5 */
        G = xn::complete_graph(5);
        b = xn::edge_betweenness_centrality(G, weight=None, normalized=false);
        b_answer = dict.fromkeys(G.edges(), 1);
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_normalized_K5() {
        /** Edge betweenness centrality: K5 */
        G = xn::complete_graph(5);
        b = xn::edge_betweenness_centrality(G, weight=None, normalized=true);
        b_answer = dict.fromkeys(G.edges(), 1 / 10);
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_C4() {
        /** Edge betweenness centrality: C4 */
        G = xn::cycle_graph(4);
        b = xn::edge_betweenness_centrality(G, weight=None, normalized=true);
        b_answer = {(0, 1) { 2, (0, 3) { 2, (1, 2) { 2, (2, 3) { 2}
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n] / 6);

    auto test_P4() {
        /** Edge betweenness centrality: P4 */
        G = xn::path_graph(4);
        b = xn::edge_betweenness_centrality(G, weight=None, normalized=false);
        b_answer = {(0, 1) { 3, (1, 2) { 4, (2, 3) { 3}
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_normalized_P4() {
        /** Edge betweenness centrality: P4 */
        G = xn::path_graph(4);
        b = xn::edge_betweenness_centrality(G, weight=None, normalized=true);
        b_answer = {(0, 1) { 3, (1, 2) { 4, (2, 3) { 3}
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n] / 6);

    auto test_balanced_tree() {
        /** Edge betweenness centrality: balanced tree */
        G = xn::balanced_tree(r=2, h=2);
        b = xn::edge_betweenness_centrality(G, weight=None, normalized=false);
        b_answer = {(0, 1) { 12, (0, 2) { 12,
                    (1, 3) { 6, (1, 4) { 6, (2, 5) { 6, (2, 6) { 6}
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n]);


class TestWeightedEdgeBetweennessCentrality: public object {
    auto test_K5() {
        /** Edge betweenness centrality: K5 */
        G = xn::complete_graph(5);
        b = xn::edge_betweenness_centrality(G, weight="weight", normalized=false);
        b_answer = dict.fromkeys(G.edges(), 1);
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_C4() {
        /** Edge betweenness centrality: C4 */
        G = xn::cycle_graph(4);
        b = xn::edge_betweenness_centrality(G, weight="weight", normalized=false);
        b_answer = {(0, 1) { 2, (0, 3) { 2, (1, 2) { 2, (2, 3) { 2}
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P4() {
        /** Edge betweenness centrality: P4 */
        G = xn::path_graph(4);
        b = xn::edge_betweenness_centrality(G, weight="weight", normalized=false);
        b_answer = {(0, 1) { 3, (1, 2) { 4, (2, 3) { 3}
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_balanced_tree() {
        /** Edge betweenness centrality: balanced tree */
        G = xn::balanced_tree(r=2, h=2);
        b = xn::edge_betweenness_centrality(G, weight="weight", normalized=false);
        b_answer = {(0, 1) { 12, (0, 2) { 12,
                    (1, 3) { 6, (1, 4) { 6, (2, 5) { 6, (2, 6) { 6}
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_weighted_graph() {
        eList = [(0, 1, 5), (0, 2, 4), (0, 3, 3),
                 (0, 4, 2), (1, 2, 4), (1, 3, 1),
                 (1, 4, 3), (2, 4, 5), (3, 4, 4)];
        G = xn::Graph();
        G.add_weighted_edges_from(eList);
        b = xn::edge_betweenness_centrality(G, weight="weight", normalized=false);
        b_answer = {(0, 1) { 0.0,
                    (0, 2) { 1.0,
                    (0, 3) { 2.0,
                    (0, 4) { 1.0,
                    (1, 2) { 2.0,
                    (1, 3) { 3.5,
                    (1, 4) { 1.5,
                    (2, 4) { 1.0,
                    (3, 4) { 0.5}
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_normalized_weighted_graph() {
        eList = [(0, 1, 5), (0, 2, 4), (0, 3, 3),
                 (0, 4, 2), (1, 2, 4), (1, 3, 1),
                 (1, 4, 3), (2, 4, 5), (3, 4, 4)];
        G = xn::Graph();
        G.add_weighted_edges_from(eList);
        b = xn::edge_betweenness_centrality(G, weight="weight", normalized=true);
        b_answer = {(0, 1) { 0.0,
                    (0, 2) { 1.0,
                    (0, 3) { 2.0,
                    (0, 4) { 1.0,
                    (1, 2) { 2.0,
                    (1, 3) { 3.5,
                    (1, 4) { 1.5,
                    (2, 4) { 1.0,
                    (3, 4) { 0.5}
        norm = len(G) * (len(G) - 1) / 2
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n] / norm);
