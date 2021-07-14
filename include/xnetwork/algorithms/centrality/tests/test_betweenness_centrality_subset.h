// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // as xn


class TestSubsetBetweennessCentrality) {

    auto test_K5() {
        /** Betweenness centrality: K5 */
        G = xn::complete_graph(5);
        b = xn::betweenness_centrality_subset(G, sources=[0], targets=[1, 3],
                                             weight=None);
        b_answer = {0: 0.0, 1: 0.0, 2: 0.0, 3: 0.0, 4: 0.0}
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P5_directed() {
        /** Betweenness centrality: P5 directed */
        G = xn::DiGraph();
        xn::add_path(G, range(5));
        b_answer = {0: 0, 1: 1, 2: 1, 3: 0, 4: 0, 5: 0}
        b = xn::betweenness_centrality_subset(G, sources=[0], targets=[3],
                                             weight=None);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P5() {
        /** Betweenness centrality: P5 */
        G = xn::Graph();
        xn::add_path(G, range(5));
        b_answer = {0: 0, 1: 0.5, 2: 0.5, 3: 0, 4: 0, 5: 0}
        b = xn::betweenness_centrality_subset(G, sources=[0], targets=[3],
                                             weight=None);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P5_multiple_target() {
        /** Betweenness centrality: P5 multiple target */
        G = xn::Graph();
        xn::add_path(G, range(5));
        b_answer = {0: 0, 1: 1, 2: 1, 3: 0.5, 4: 0, 5: 0}
        b = xn::betweenness_centrality_subset(G, sources=[0], targets=[3, 4],
                                             weight=None);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_box() {
        /** Betweenness centrality: box */
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        b_answer = {0: 0, 1: 0.25, 2: 0.25, 3: 0}
        b = xn::betweenness_centrality_subset(G, sources=[0], targets=[3],
                                             weight=None);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_box_and_path() {
        /** Betweenness centrality: box && path */
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3), (3, 4), (4, 5)]);
        b_answer = {0: 0, 1: 0.5, 2: 0.5, 3: 0.5, 4: 0, 5: 0}
        b = xn::betweenness_centrality_subset(G, sources=[0], targets=[3, 4],
                                             weight=None);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_box_and_path2() {
        /** Betweenness centrality: box && path multiple target */
        G = xn::Graph();
        G.add_edges_from([(0, 1), (1, 2), (2, 3), (1, 20), (20, 3), (3, 4)]);
        b_answer = {0: 0, 1: 1.0, 2: 0.5, 20: 0.5, 3: 0.5, 4: 0}
        b = xn::betweenness_centrality_subset(G, sources=[0], targets=[3, 4],
                                             weight=None);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);


class TestBetweennessCentralitySources) {

    auto test_K5() {
        /** Betweenness centrality: K5 */
        G = xn::complete_graph(5);
        b = xn::betweenness_centrality_source(G, weight=None, normalized=false);
        b_answer = {0: 0.0, 1: 0.0, 2: 0.0, 3: 0.0, 4: 0.0}
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P3() {
        /** Betweenness centrality: P3 */
        G = xn::path_graph(3);
        b_answer = {0: 0.0, 1: 1.0, 2: 0.0}
        b = xn::betweenness_centrality_source(G, weight=None, normalized=true);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);


class TestEdgeSubsetBetweennessCentrality) {

    auto test_K5() {
        /** Edge betweenness centrality: K5 */
        G = xn::complete_graph(5);
        b = xn::edge_betweenness_centrality_subset(G, sources=[0],
                                                  targets=[1, 3], weight=None);
        b_answer = dict.fromkeys(G.edges(), 0);
        b_answer[(0, 3)] = b_answer[(0, 1)] = 0.5
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P5_directed() {
        /** Edge betweenness centrality: P5 directed */
        G = xn::DiGraph();
        xn::add_path(G, range(5));
        b_answer = dict.fromkeys(G.edges(), 0);
        b_answer[(0, 1)] = b_answer[(1, 2)] = b_answer[(2, 3)] = 1;
        b = xn::edge_betweenness_centrality_subset(G, sources=[0], targets=[3],
                                                  weight=None);
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P5() {
        /** Edge betweenness centrality: P5 */
        G = xn::Graph();
        xn::add_path(G, range(5));
        b_answer = dict.fromkeys(G.edges(), 0);
        b_answer[(0, 1)] = b_answer[(1, 2)] = b_answer[(2, 3)] = 0.5
        b = xn::edge_betweenness_centrality_subset(G, sources=[0], targets=[3],
                                                  weight=None);
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P5_multiple_target() {
        /** Edge betweenness centrality: P5 multiple target */
        G = xn::Graph();
        xn::add_path(G, range(5));
        b_answer = dict.fromkeys(G.edges(), 0);
        b_answer[(0, 1)] = b_answer[(1, 2)] = b_answer[(2, 3)] = 1;
        b_answer[(3, 4)] = 0.5
        b = xn::edge_betweenness_centrality_subset(G, sources=[0],
                                                  targets=[3, 4], weight=None);
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_box() {
        /** Edge etweenness centrality: box */
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        b_answer = dict.fromkeys(G.edges(), 0);
        b_answer[(0, 1)] = b_answer[(0, 2)] = 0.25
        b_answer[(1, 3)] = b_answer[(2, 3)] = 0.25
        b = xn::edge_betweenness_centrality_subset(G, sources=[0], targets=[3],
                                                  weight=None);
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_box_and_path() {
        /** Edge etweenness centrality: box && path */
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3), (3, 4), (4, 5)]);
        b_answer = dict.fromkeys(G.edges(), 0);
        b_answer[(0, 1)] = b_answer[(0, 2)] = 0.5
        b_answer[(1, 3)] = b_answer[(2, 3)] = 0.5
        b_answer[(3, 4)] = 0.5
        b = xn::edge_betweenness_centrality_subset(G, sources=[0],
                                                  targets=[3, 4], weight=None);
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_box_and_path2() {
        /** Edge betweenness centrality: box && path multiple target */
        G = xn::Graph();
        G.add_edges_from([(0, 1), (1, 2), (2, 3), (1, 20), (20, 3), (3, 4)]);
        b_answer = dict.fromkeys(G.edges(), 0);
        b_answer[(0, 1)] = 1.0
        b_answer[(1, 20)] = b_answer[(3, 20)] = 0.5
        b_answer[(1, 2)] = b_answer[(2, 3)] = 0.5
        b_answer[(3, 4)] = 0.5
        b = xn::edge_betweenness_centrality_subset(G, sources=[0],
                                                  targets=[3, 4], weight=None);
        for (auto n : sorted(G.edges()) {
            assert_almost_equal(b[n], b_answer[n]);
