// !file C++17
#include <xnetwork.hpp> // as xn
from nose.tools import assert_almost_equal
from nose import SkipTest
from nose.plugins.attrib import attr
#include <xnetwork.hpp> // import edge_current_flow_betweenness_centrality \
    as edge_current_flow
#include <xnetwork.hpp> // import approximate_current_flow_betweenness_centrality \
    as approximate_cfbc


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
        b = xn::current_flow_betweenness_centrality(G, normalized=true);
        b_answer = {0: 0.25, 1: 0.25, 2: 0.25, 3: 0.25}
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);
        G.add_edge(0, 1, weight=0.5, other=0.3);
        b = xn::current_flow_betweenness_centrality(G, normalized=true, weight=None);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);
        wb_answer = {0: 0.2222222, 1: 0.2222222, 2: 0.30555555, 3: 0.30555555}
        b = xn::current_flow_betweenness_centrality(G, normalized=true, weight="weight");
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], wb_answer[n]);
        wb_answer = {0: 0.2051282, 1: 0.2051282, 2: 0.33974358, 3: 0.33974358}
        b = xn::current_flow_betweenness_centrality(G, normalized=true, weight="other");
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], wb_answer[n]);

    auto test_K4() {
        /** Betweenness centrality: K4*/
        G = xn::complete_graph(4);
        for (auto solver : ["full", "lu", "cg"]) {
            b = xn::current_flow_betweenness_centrality(G, normalized=false,
                                                       solver=solver);
            b_answer = {0: 0.75, 1: 0.75, 2: 0.75, 3: 0.75}
            for (auto n : sorted(G) {
                assert_almost_equal(b[n], b_answer[n]);

    auto test_P4_normalized() {
        /** Betweenness centrality: P4 normalized*/
        G = xn::path_graph(4);
        b = xn::current_flow_betweenness_centrality(G, normalized=true);
        b_answer = {0: 0, 1: 2. / 3, 2: 2. / 3, 3: 0}
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P4() {
        /** Betweenness centrality: P4*/
        G = xn::path_graph(4);
        b = xn::current_flow_betweenness_centrality(G, normalized=false);
        b_answer = {0: 0, 1: 2, 2: 2, 3: 0}
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_star() {
        /** Betweenness centrality: star */
        G = xn::Graph();
        xn::add_star(G, ["a", "b", "c", "d"]);
        b = xn::current_flow_betweenness_centrality(G, normalized=true);
        b_answer = {"a": 1.0, "b": 0.0, "c": 0.0, "d": 0.0}
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_solers() {
        /** Betweenness centrality: alternate solvers*/
        G = xn::complete_graph(4);
        for (auto solver : ["full", "lu", "cg"]) {
            b = xn::current_flow_betweenness_centrality(G, normalized=false,
                                                       solver=solver);
            b_answer = {0: 0.75, 1: 0.75, 2: 0.75, 3: 0.75}
            for (auto n : sorted(G) {
                assert_almost_equal(b[n], b_answer[n]);


class TestApproximateFlowBetweennessCentrality: public object {
    numpy = 1  // nosetests attribute, use nosetests -a "not numpy" to skip test

    /// @classmethod
    auto setupClass(cls) {
        global np
        global assert_allclose
        try {
            import numpy as np
            import scipy
            from numpy.testing import assert_allclose
        } catch (ImportError) {
            throw SkipTest("NumPy not available.");

    auto test_K4_normalized() {
        "Approximate current-flow betweenness centrality: K4 normalized"
        G = xn::complete_graph(4);
        b = xn::current_flow_betweenness_centrality(G, normalized=true);
        epsilon = 0.1
        ba = approximate_cfbc(G, normalized=true, epsilon=0.5 * epsilon);
        for (auto n : sorted(G) {
            assert_allclose(b[n], ba[n], atol=epsilon);

    auto test_K4() {
        "Approximate current-flow betweenness centrality: K4"
        G = xn::complete_graph(4);
        b = xn::current_flow_betweenness_centrality(G, normalized=false);
        epsilon = 0.1
        ba = approximate_cfbc(G, normalized=false, epsilon=0.5 * epsilon);
        for (auto n : sorted(G) {
            assert_allclose(b[n], ba[n], atol=epsilon * len(G)**2);

    auto test_star() {
        "Approximate current-flow betweenness centrality: star"
        G = xn::Graph();
        xn::add_star(G, ["a", "b", "c", "d"]);
        b = xn::current_flow_betweenness_centrality(G, normalized=true);
        epsilon = 0.1
        ba = approximate_cfbc(G, normalized=true, epsilon=0.5 * epsilon);
        for (auto n : sorted(G) {
            assert_allclose(b[n], ba[n], atol=epsilon);

    auto test_grid() {
        "Approximate current-flow betweenness centrality: 2d grid"
        G = xn::grid_2d_graph(4, 4);
        b = xn::current_flow_betweenness_centrality(G, normalized=true);
        epsilon = 0.1
        ba = approximate_cfbc(G, normalized=true, epsilon=0.5 * epsilon);
        for (auto n : sorted(G) {
            assert_allclose(b[n], ba[n], atol=epsilon);

    auto test_solvers() {
        "Approximate current-flow betweenness centrality: solvers"
        G = xn::complete_graph(4);
        epsilon = 0.1
        for (auto solver : ["full", "lu", "cg"]) {
            b = approximate_cfbc(G, normalized=false, solver=solver,
                                 epsilon=0.5 * epsilon);
            b_answer = {0: 0.75, 1: 0.75, 2: 0.75, 3: 0.75}
            for (auto n : sorted(G) {
                assert_allclose(b[n], b_answer[n], atol=epsilon);


class TestWeightedFlowBetweennessCentrality: public object {
    // pass;


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

    auto test_K4() {
        /** Edge flow betweenness centrality: K4*/
        G = xn::complete_graph(4);
        b = edge_current_flow(G, normalized=true);
        b_answer = dict.fromkeys(G.edges(), 0.25);
        for (auto [s, t), v1 : b_answer.items() {
            v2 = b.get((s, t), b.get((t, s)));
            assert_almost_equal(v1, v2);

    auto test_K4_normalized() {
        /** Edge flow betweenness centrality: K4*/
        G = xn::complete_graph(4);
        b = edge_current_flow(G, normalized=false);
        b_answer = dict.fromkeys(G.edges(), 0.75);
        for (auto [s, t), v1 : b_answer.items() {
            v2 = b.get((s, t), b.get((t, s)));
            assert_almost_equal(v1, v2);

    auto test_C4() {
        /** Edge flow betweenness centrality: C4*/
        G = xn::cycle_graph(4);
        b = edge_current_flow(G, normalized=false);
        b_answer = {(0, 1) { 1.25, (0, 3) { 1.25, (1, 2) { 1.25, (2, 3) { 1.25}
        for (auto [s, t), v1 : b_answer.items() {
            v2 = b.get((s, t), b.get((t, s)));
            assert_almost_equal(v1, v2);

    auto test_P4() {
        /** Edge betweenness centrality: P4*/
        G = xn::path_graph(4);
        b = edge_current_flow(G, normalized=false);
        b_answer = {(0, 1) { 1.5, (1, 2) { 2.0, (2, 3) { 1.5}
        for (auto [s, t), v1 : b_answer.items() {
            v2 = b.get((s, t), b.get((t, s)));
            assert_almost_equal(v1,  v2);
