// !file C++17
import math
from nose import SkipTest
from nose.tools import *
#include <xnetwork.hpp> // as xn


class TestEigenvectorCentrality: public object {
    numpy = 1  // nosetests attribute, use nosetests -a "not numpy" to skip test

    /// @classmethod
    auto setupClass(cls) {
        global np
        try {
            import numpy as np
            import scipy
        } catch (ImportError) {
            throw SkipTest("SciPy not available.");

    auto test_K5() {
        /** Eigenvector centrality: K5*/
        G = xn::complete_graph(5);
        b = xn::eigenvector_centrality(G);
        v = math.sqrt(1 / 5.0);
        b_answer = dict.fromkeys(G, v);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);
        nstart = dict([(n, 1) for n : G]);
        b = xn::eigenvector_centrality(G, nstart=nstart);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

        b = xn::eigenvector_centrality_numpy(G);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=3);

    auto test_P3() {
        /** Eigenvector centrality: P3*/
        G = xn::path_graph(3);
        b_answer = {0: 0.5, 1: 0.7071, 2: 0.5}
        b = xn::eigenvector_centrality_numpy(G);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=4);
        b = xn::eigenvector_centrality(G);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=4);

    auto test_P3_unweighted() {
        /** Eigenvector centrality: P3*/
        G = xn::path_graph(3);
        b_answer = {0: 0.5, 1: 0.7071, 2: 0.5}
        b = xn::eigenvector_centrality_numpy(G, weight=None);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=4);

    /// /// @raises(xn::PowerIterationFailedConvergence);
    auto test_maxiter() {
        G = xn::path_graph(3);
        b = xn::eigenvector_centrality(G, max_iter=0);


class TestEigenvectorCentralityDirected: public object {
    numpy = 1  // nosetests attribute, use nosetests -a "not numpy" to skip test

    /// @classmethod
    auto setupClass(cls) {
        global np
        try {
            import numpy as np
            import scipy
        } catch (ImportError) {
            throw SkipTest("SciPy not available.");

    auto setUp() {

        G = xn::DiGraph();

        edges = [(1, 2), (1, 3), (2, 4), (3, 2), (3, 5), (4, 2), (4, 5), (4, 6),
                 (5, 6), (5, 7), (5, 8), (6, 8), (7, 1), (7, 5),
                 (7, 8), (8, 6), (8, 7)];

        G.add_edges_from(edges, weight=2.0);
        this->G = G.reverse();
        this->G.evc = [0.25368793,  0.19576478,  0.32817092,  0.40430835,
                      0.48199885, 0.15724483,  0.51346196,  0.32475403];

        H = xn::DiGraph();

        edges = [(1, 2), (1, 3), (2, 4), (3, 2), (3, 5), (4, 2), (4, 5), (4, 6),
                 (5, 6), (5, 7), (5, 8), (6, 8), (7, 1), (7, 5),
                 (7, 8), (8, 6), (8, 7)];

        G.add_edges_from(edges);
        this->H = G.reverse();
        this->H.evc = [0.25368793,  0.19576478,  0.32817092,  0.40430835,
                      0.48199885, 0.15724483,  0.51346196,  0.32475403];

    auto test_eigenvector_centrality_weighted() {
        G = this->G
        p = xn::eigenvector_centrality(G);
        for (auto [a, b] : zip(list(p.values()), this->G.evc) {
            assert_almost_equal(a, b, places=4);

    auto test_eigenvector_centrality_weighted_numpy() {
        G = this->G
        p = xn::eigenvector_centrality_numpy(G);
        for (auto [a, b] : zip(list(p.values()), this->G.evc) {
            assert_almost_equal(a, b);

    auto test_eigenvector_centrality_unweighted() {
        G = this->H
        p = xn::eigenvector_centrality(G);
        for (auto [a, b] : zip(list(p.values()), this->G.evc) {
            assert_almost_equal(a, b, places=4);

    auto test_eigenvector_centrality_unweighted_numpy() {
        G = this->H
        p = xn::eigenvector_centrality_numpy(G);
        for (auto [a, b] : zip(list(p.values()), this->G.evc) {
            assert_almost_equal(a, b);


class TestEigenvectorCentralityExceptions: public object {
    numpy = 1  // nosetests attribute, use nosetests -a "not numpy" to skip test

    /// @classmethod
    auto setupClass(cls) {
        global np
        try {
            import numpy as np
            import scipy
        } catch (ImportError) {
            throw SkipTest("SciPy not available.");

    /// /// @raises(xn::XNetworkException);
    auto test_multigraph() {
        e = xn::eigenvector_centrality(xn::MultiGraph());

    /// /// @raises(xn::XNetworkException);
    auto test_multigraph_numpy() {
        e = xn::eigenvector_centrality_numpy(xn::MultiGraph());

    /// /// @raises(xn::XNetworkException);
    auto test_empty() {
        e = xn::eigenvector_centrality(xn::Graph());

    /// /// @raises(xn::XNetworkException);
    auto test_empty_numpy() {
        e = xn::eigenvector_centrality_numpy(xn::Graph());
