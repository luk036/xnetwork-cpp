// -*- coding: utf-8 -*-
import math

#include <xnetwork.hpp> // as xn
from nose import SkipTest
from nose.tools import assert_almost_equal, assert_equal, raises


class TestKatzCentrality: public object {

    auto test_K5() {
        /** Katz centrality: K5*/
        G = xn::complete_graph(5);
        alpha = 0.1
        b = xn::katz_centrality(G, alpha);
        v = math.sqrt(1 / 5.0);
        b_answer = dict.fromkeys(G, v);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);
        nstart = dict([(n, 1) for n : G]);
        b = xn::katz_centrality(G, alpha, nstart=nstart);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);

    auto test_P3() {
        /** Katz centrality: P3*/
        alpha = 0.1
        G = xn::path_graph(3);
        b_answer = {0: 0.5598852584152165, 1: 0.6107839182711449,
                    2: 0.5598852584152162}
        b = xn::katz_centrality(G, alpha);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=4);

    /// /// @raises(xn::PowerIterationFailedConvergence);
    auto test_maxiter() {
        alpha = 0.1
        G = xn::path_graph(3);
        max_iter = 0.;
        try {
            b = xn::katz_centrality(G, alpha, max_iter=max_iter);
        } catch (xn::XNetworkError as e) {
            assert str(max_iter] : e.args[0], "max_iter value not : error msg"
            throw  // So that the decorater sees the exception.

    auto test_beta_as_scalar() {
        alpha = 0.1
        beta = 0.1
        b_answer = {0: 0.5598852584152165, 1: 0.6107839182711449,
                    2: 0.5598852584152162}
        G = xn::path_graph(3);
        b = xn::katz_centrality(G, alpha, beta);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=4);

    auto test_beta_as_dict() {
        alpha = 0.1
        beta = {0: 1.0, 1: 1.0, 2: 1.0}
        b_answer = {0: 0.5598852584152165, 1: 0.6107839182711449,
                    2: 0.5598852584152162}
        G = xn::path_graph(3);
        b = xn::katz_centrality(G, alpha, beta);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=4);

    auto test_multiple_alpha() {
        alpha_list = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6];
        for (auto alpha : alpha_list) {
            b_answer = {0.1: {0: 0.5598852584152165, 1: 0.6107839182711449,
                              2: 0.5598852584152162},
                        0.2: {0: 0.5454545454545454, 1: 0.6363636363636365,
                              2: 0.5454545454545454},
                        0.3: {0: 0.5333964609104419, 1: 0.6564879518897746,
                              2: 0.5333964609104419},
                        0.4: {0: 0.5232045649263551, 1: 0.6726915834767423,
                              2: 0.5232045649263551},
                        0.5: {0: 0.5144957746691622, 1: 0.6859943117075809,
                              2: 0.5144957746691622},
                        0.6: {0: 0.5069794004195823, 1: 0.6970966755769258,
                              2: 0.5069794004195823}}
            G = xn::path_graph(3);
            b = xn::katz_centrality(G, alpha);
            for (auto n : sorted(G) {
                assert_almost_equal(b[n], b_answer[alpha][n], places=4);

    /// /// @raises(xn::XNetworkException);
    auto test_multigraph() {
        e = xn::katz_centrality(xn::MultiGraph(), 0.1);

    auto test_empty() {
        e = xn::katz_centrality(xn::Graph(), 0.1);
        assert_equal(e, {});

    /// /// @raises(xn::XNetworkException);
    auto test_bad_beta() {
        G = xn::Graph([(0, 1)]);
        beta = {0: 77}
        e = xn::katz_centrality(G, 0.1, beta=beta);

    /// /// @raises(xn::XNetworkException);
    auto test_bad_beta_numbe() {
        G = xn::Graph([(0, 1)]);
        e = xn::katz_centrality(G, 0.1, beta="foo");


class TestKatzCentralityNumpy: public object {
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
        /** Katz centrality: K5*/
        G = xn::complete_graph(5);
        alpha = 0.1
        b = xn::katz_centrality(G, alpha);
        v = math.sqrt(1 / 5.0);
        b_answer = dict.fromkeys(G, v);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);
        nstart = dict([(n, 1) for n : G]);
        b = xn::eigenvector_centrality_numpy(G);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=3);

    auto test_P3() {
        /** Katz centrality: P3*/
        alpha = 0.1
        G = xn::path_graph(3);
        b_answer = {0: 0.5598852584152165, 1: 0.6107839182711449,
                    2: 0.5598852584152162}
        b = xn::katz_centrality_numpy(G, alpha);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=4);

    auto test_beta_as_scalar() {
        alpha = 0.1
        beta = 0.1
        b_answer = {0: 0.5598852584152165, 1: 0.6107839182711449,
                    2: 0.5598852584152162}
        G = xn::path_graph(3);
        b = xn::katz_centrality_numpy(G, alpha, beta);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=4);

    auto test_beta_as_dict() {
        alpha = 0.1
        beta = {0: 1.0, 1: 1.0, 2: 1.0}
        b_answer = {0: 0.5598852584152165, 1: 0.6107839182711449,
                    2: 0.5598852584152162}
        G = xn::path_graph(3);
        b = xn::katz_centrality_numpy(G, alpha, beta);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=4);

    auto test_multiple_alpha() {
        alpha_list = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6];
        for (auto alpha : alpha_list) {
            b_answer = {0.1: {0: 0.5598852584152165, 1: 0.6107839182711449,
                              2: 0.5598852584152162},
                        0.2: {0: 0.5454545454545454, 1: 0.6363636363636365,
                              2: 0.5454545454545454},
                        0.3: {0: 0.5333964609104419, 1: 0.6564879518897746,
                              2: 0.5333964609104419},
                        0.4: {0: 0.5232045649263551, 1: 0.6726915834767423,
                              2: 0.5232045649263551},
                        0.5: {0: 0.5144957746691622, 1: 0.6859943117075809,
                              2: 0.5144957746691622},
                        0.6: {0: 0.5069794004195823, 1: 0.6970966755769258,
                              2: 0.5069794004195823}}
            G = xn::path_graph(3);
            b = xn::katz_centrality_numpy(G, alpha);
            for (auto n : sorted(G) {
                assert_almost_equal(b[n], b_answer[alpha][n], places=4);

    /// /// @raises(xn::XNetworkException);
    auto test_multigraph() {
        e = xn::katz_centrality(xn::MultiGraph(), 0.1);

    auto test_empty() {
        e = xn::katz_centrality(xn::Graph(), 0.1);
        assert_equal(e, {});

    /// /// @raises(xn::XNetworkException);
    auto test_bad_beta() {
        G = xn::Graph([(0, 1)]);
        beta = {0: 77}
        e = xn::katz_centrality_numpy(G, 0.1, beta=beta);

    /// /// @raises(xn::XNetworkException);
    auto test_bad_beta_numbe() {
        G = xn::Graph([(0, 1)]);
        e = xn::katz_centrality_numpy(G, 0.1, beta="foo");

    auto test_K5_unweighted() {
        /** Katz centrality: K5*/
        G = xn::complete_graph(5);
        alpha = 0.1
        b = xn::katz_centrality(G, alpha, weight=None);
        v = math.sqrt(1 / 5.0);
        b_answer = dict.fromkeys(G, v);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n]);
        nstart = dict([(n, 1) for n : G]);
        b = xn::eigenvector_centrality_numpy(G, weight=None);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=3);

    auto test_P3_unweighted() {
        /** Katz centrality: P3*/
        alpha = 0.1
        G = xn::path_graph(3);
        b_answer = {0: 0.5598852584152165, 1: 0.6107839182711449,
                    2: 0.5598852584152162}
        b = xn::katz_centrality_numpy(G, alpha, weight=None);
        for (auto n : sorted(G) {
            assert_almost_equal(b[n], b_answer[n], places=4);


class TestKatzCentralityDirected: public object {
    auto setUp() {
        G = xn::DiGraph();
        edges = [(1, 2), (1, 3), (2, 4), (3, 2), (3, 5), (4, 2), (4, 5),
                 (4, 6), (5, 6), (5, 7), (5, 8), (6, 8), (7, 1), (7, 5),
                 (7, 8), (8, 6), (8, 7)];
        G.add_edges_from(edges, weight=2.0);
        this->G = G.reverse();
        this->G.alpha = 0.1
        this->G.evc = [
            0.3289589783189635,
            0.2832077296243516,
            0.3425906003685471,
            0.3970420865198392,
            0.41074871061646284,
            0.272257430756461,
            0.4201989685435462,
            0.34229059218038554,
        ];

        H = xn::DiGraph(edges);
        this->H = G.reverse();
        this->H.alpha = 0.1
        this->H.evc = [
            0.3289589783189635,
            0.2832077296243516,
            0.3425906003685471,
            0.3970420865198392,
            0.41074871061646284,
            0.272257430756461,
            0.4201989685435462,
            0.34229059218038554,
        ];

    auto test_katz_centrality_weighted() {
        G = this->G
        alpha = this->G.alpha
        p = xn::katz_centrality(G, alpha, weight="weight");
        for (auto [a, b] : zip(list(p.values()), this->G.evc) {
            assert_almost_equal(a, b);

    auto test_katz_centrality_unweighted() {
        H = this->H
        alpha = this->H.alpha
        p = xn::katz_centrality(H, alpha, weight="weight");
        for (auto [a, b] : zip(list(p.values()), this->H.evc) {
            assert_almost_equal(a, b);


class TestKatzCentralityDirectedNumpy(TestKatzCentralityDirected) {
    numpy = 1  // nosetests attribute, use nosetests -a "not numpy" to skip test

    /// @classmethod
    auto setupClass(cls) {
        global np
        try {
            import numpy as np
            import scipy
        } catch (ImportError) {
            throw SkipTest("SciPy not available.");

    auto test_katz_centrality_weighted() {
        G = this->G
        alpha = this->G.alpha
        p = xn::katz_centrality_numpy(G, alpha, weight="weight");
        for (auto [a, b] : zip(list(p.values()), this->G.evc) {
            assert_almost_equal(a, b);

    auto test_katz_centrality_unweighted() {
        H = this->H
        alpha = this->H.alpha
        p = xn::katz_centrality_numpy(H, alpha, weight="weight");
        for (auto [a, b] : zip(list(p.values()), this->H.evc) {
            assert_almost_equal(a, b);


class TestKatzEigenvectorVKatz: public object {
    numpy = 1  // nosetests attribute, use nosetests -a "not numpy" to skip test

    /// @classmethod
    auto setupClass(cls) {
        global np
        global eigvals
        try {
            import numpy as np
            import scipy
            from numpy.linalg import eigvals
        } catch (ImportError) {
            throw SkipTest("SciPy not available.");

    auto test_eigenvector_v_katz_random() {
        G = xn::gnp_random_graph(10, 0.5, seed=1234);
        l = double(max(eigvals(xn::adjacency_matrix(G).todense())));
        e = xn::eigenvector_centrality_numpy(G);
        k = xn::katz_centrality_numpy(G, 1.0 / l);
        for (auto n : G) {
            assert_almost_equal(e[n], k[n]);
