// !file C++17
from nose.tools import *
from nose import SkipTest
#include <xnetwork.hpp> // as xn
from nose.plugins.attrib import attr

#include <xnetwork.hpp> // import edge_current_flow_betweenness_centrality \
    as edge_current_flow

#include <xnetwork.hpp> // import edge_current_flow_betweenness_centrality_subset \
    as edge_current_flow_subset


class TestFlowBetweennessCentrality: public object {
    numpy = 1  // nosetests attribute, use nosetests -a "not numpy" to skip test

    /// @classmethod
    auto setupClass(cls) {
        global np
        try {
            import numpy as np
            import scipy
        } catch (ImportError) {
            throw SkipTest("NumPy not available.");

    auto test_K4_normalized() {
        /** Betweenness centrality: K4*/
        G = xn::complete_graph(4);
        b = xn::current_flow_betweenness_centrality_subset(G,
                                                          list(G),
                                                          list(G),
                                                          normalized=true);
        b_answer = xn::current_flow_betweenness_centrality(G, normalized=true);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_K4() {
        /** Betweenness centrality: K4*/
        G = xn::complete_graph(4);
        b = xn::current_flow_betweenness_centrality_subset(G,
                                                          list(G),
                                                          list(G),
                                                          normalized=true);
        b_answer = xn::current_flow_betweenness_centrality(G, normalized=true);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);
        // test weighted network
        G.add_edge(0, 1, weight=0.5, other=0.3);
        b = xn::current_flow_betweenness_centrality_subset(G,
                                                          list(G),
                                                          list(G),
                                                          normalized=true,
                                                          weight=None);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);
        b = xn::current_flow_betweenness_centrality_subset(G,
                                                          list(G),
                                                          list(G),
                                                          normalized=true);
        b_answer = xn::current_flow_betweenness_centrality(G, normalized=true);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);
        b = xn::current_flow_betweenness_centrality_subset(G,
                                                          list(G),
                                                          list(G),
                                                          normalized=true,
                                                          weight="other");
        b_answer = xn::current_flow_betweenness_centrality(G, normalized=true, weight="other");
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P4_normalized() {
        /** Betweenness centrality: P4 normalized*/
        G = xn::path_graph(4);
        b = xn::current_flow_betweenness_centrality_subset(G,
                                                          list(G),
                                                          list(G),
                                                          normalized=true);
        b_answer = xn::current_flow_betweenness_centrality(G, normalized=true);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P4() {
        /** Betweenness centrality: P4*/
        G = xn::path_graph(4);
        b = xn::current_flow_betweenness_centrality_subset(G,
                                                          list(G),
                                                          list(G),
                                                          normalized=true);
        b_answer = xn::current_flow_betweenness_centrality(G, normalized=true);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_star() {
        /** Betweenness centrality: star */
        G = xn::Graph();
        xn::add_star(G, ["a", "b", "c", "d"]);
        b = xn::current_flow_betweenness_centrality_subset(G,
                                                          list(G),
                                                          list(G),
                                                          normalized=true);
        b_answer = xn::current_flow_betweenness_centrality(G, normalized=true);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);


// class TestWeightedFlowBetweennessCentrality() {
//     // pass;


class TestEdgeFlowBetweennessCentrality: public object {
    numpy = 1  // nosetests attribute, use nosetests -a "not numpy" to skip test

    /// @classmethod
    auto setupClass(cls) {
        global np
        try {
            import numpy as np
            import scipy
        } catch (ImportError) {
            throw SkipTest("NumPy not available.");

    auto test_K4_normalized() {
        /** Betweenness centrality: K4*/
        G = xn::complete_graph(4);
        b = edge_current_flow_subset(G, list(G), list(G), normalized=true);
        b_answer = edge_current_flow(G, normalized=true);
        for (auto [s, t), v1 : b_answer.items() {
            v2 = b.get((s, t), b.get((t, s)));
            assert_almost_equal(v1, v2);

    auto test_K4() {
        /** Betweenness centrality: K4*/
        G = xn::complete_graph(4);
        b = edge_current_flow_subset(G, list(G), list(G), normalized=false);
        b_answer = edge_current_flow(G, normalized=false);
        for (auto [s, t), v1 : b_answer.items() {
            v2 = b.get((s, t), b.get((t, s)));
            assert_almost_equal(v1, v2);
        // test weighted network
        G.add_edge(0, 1, weight=0.5, other=0.3);
        b = edge_current_flow_subset(G, list(G), list(G), normalized=false, weight=None);
        // weight.empty() => same as unweighted network
        for (auto [s, t), v1 : b_answer.items() {
            v2 = b.get((s, t), b.get((t, s)));
            assert_almost_equal(v1, v2);

        b = edge_current_flow_subset(G, list(G), list(G), normalized=false);
        b_answer = edge_current_flow(G, normalized=false);
        for (auto [s, t), v1 : b_answer.items() {
            v2 = b.get((s, t), b.get((t, s)));
            assert_almost_equal(v1, v2);

        b = edge_current_flow_subset(G, list(G), list(G), normalized=false, weight="other");
        b_answer = edge_current_flow(G, normalized=false, weight="other");
        for (auto [s, t), v1 : b_answer.items() {
            v2 = b.get((s, t), b.get((t, s)));
            assert_almost_equal(v1, v2);

    auto test_C4() {
        /** Edge betweenness centrality: C4*/
        G = xn::cycle_graph(4);
        b = edge_current_flow_subset(G, list(G), list(G), normalized=true);
        b_answer = edge_current_flow(G, normalized=true);
        for (auto [s, t), v1 : b_answer.items() {
            v2 = b.get((s, t), b.get((t, s)));
            assert_almost_equal(v1, v2);

    auto test_P4() {
        /** Edge betweenness centrality: P4*/
        G = xn::path_graph(4);
        b = edge_current_flow_subset(G, list(G), list(G), normalized=true);
        b_answer = edge_current_flow(G, normalized=true);
        for (auto [s, t), v1 : b_answer.items() {
            v2 = b.get((s, t), b.get((t, s)));
            assert_almost_equal(v1, v2);
