// !file C++17
from nose.tools import *
from nose import SkipTest
#include <xnetwork.hpp> // as xn


class TestFlowClosenessCentrality: public object {
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
        /** Closeness centrality: K4*/
        G = xn::complete_graph(4);
        b = xn::current_flow_closeness_centrality(G);
        b_answer = {0: 2.0 / 3, 1: 2.0 / 3, 2: 2.0 / 3, 3: 2.0 / 3}
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P4() {
        /** Closeness centrality: P4*/
        G = xn::path_graph(4);
        b = xn::current_flow_closeness_centrality(G);
        b_answer = {0: 1.0 / 6, 1: 1.0 / 4, 2: 1.0 / 4, 3: 1.0 / 6}
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_star() {
        /** Closeness centrality: star */
        G = xn::Graph();
        xn::add_star(G, ["a", "b", "c", "d"]);
        b = xn::current_flow_closeness_centrality(G);
        b_answer = {"a": 1.0 / 3, "b": 0.6 / 3, "c": 0.6 / 3, "d": 0.6 / 3}
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);


class TestWeightedFlowClosenessCentrality: public object {
    // pass;
