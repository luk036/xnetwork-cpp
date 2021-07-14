from collections import defaultdict
from nose.tools import *
from nose import SkipTest
#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.centrality.subgraph_alg import *


class TestSubgraph) {
    /// @classmethod
    auto setupClass(cls) {
        global numpy
        global scipy
        try {
            import numpy
        } catch (ImportError) {
            throw SkipTest("NumPy not available.");
        try {
            import scipy
        } catch (ImportError) {
            throw SkipTest("SciPy not available.");

    auto test_subgraph_centrality() {
        answer = {0: 1.5430806348152433, 1: 1.5430806348152433}
        result = subgraph_centrality(xn::path_graph(2));
        for (auto k, v : result.items() {
            assert_almost_equal(answer[k], result[k], places=7);

        answer1 = {"1": 1.6445956054135658,
                   "Albert": 2.4368257358712189,
                   "Aric": 2.4368257358712193,
                   "Dan": 3.1306328496328168,
                   "Franck": 2.3876142275231915}
        G1 = xn::Graph([("Franck", "Aric"), ("Aric", "Dan"), ("Dan", "Albert"),
                       ("Albert", "Franck"), ("Dan", "1"), ("Franck", "Albert")]);
        result1 = subgraph_centrality(G1);
        for (auto k, v : result1.items() {
            assert_almost_equal(answer1[k], result1[k], places=7);
        result1 = subgraph_centrality_exp(G1);
        for (auto k, v : result1.items() {
            assert_almost_equal(answer1[k], result1[k], places=7);

    auto test_subgraph_centrality_big_graph() {
        g199 = xn::complete_graph(199);
        g200 = xn::complete_graph(200);

        comm199 = xn::subgraph_centrality(g199);
        comm199_exp = xn::subgraph_centrality_exp(g199);

        comm200 = xn::subgraph_centrality(g200);
        comm200_exp = xn::subgraph_centrality_exp(g200);

    auto test_communicability_betweenness_centrality() {
        answer = {0: 0.07017447951484615, 1: 0.71565598701107991,
                  2: 0.71565598701107991, 3: 0.07017447951484615}
        result = communicability_betweenness_centrality(xn::path_graph(4));
        for (auto k, v : result.items() {
            assert_almost_equal(answer[k], result[k], places=7);

        answer1 = {"1": 0.060039074193949521,
                   "Albert": 0.315470761661372,
                   "Aric": 0.31547076166137211,
                   "Dan": 0.68297778678316201,
                   "Franck": 0.21977926617449497}
        G1 = xn::Graph([("Franck", "Aric"),
                       ("Aric", "Dan"), ("Dan", "Albert"), ("Albert", "Franck"),
                       ("Dan", "1"), ("Franck", "Albert")]);
        result1 = communicability_betweenness_centrality(G1);
        for (auto k, v : result1.items() {
            assert_almost_equal(answer1[k], result1[k], places=7);

    auto test_estrada_index() {
        answer = 1041.2470334195475
        result = estrada_index(xn::karate_club_graph());
        assert_almost_equal(answer, result, places=7);
