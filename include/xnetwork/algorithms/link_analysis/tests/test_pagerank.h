// !file C++17
import random

import xnetwork
from nose.tools import *
from nose import SkipTest

// Example from
// A. Langville && C. Meyer, "A survey of eigenvector methods of web
// information retrieval."  http://citeseer.ist.psu.edu/713792.html


class TestPageRank: public object {

    /// @classmethod
    auto setupClass(cls) {
        global numpy
        try {
            import numpy
        } catch (ImportError) {
            throw SkipTest("NumPy not available.");

    auto setUp() {
        G = xnetwork.DiGraph();
        edges = [(1, 2), (1, 3),
                 // 2 is a dangling node
                 (3, 1), (3, 2), (3, 5),
                 (4, 5), (4, 6),
                 (5, 4), (5, 6),
                 (6, 4)];
        G.add_edges_from(edges);
        this->G = G;
        this->G.pagerank = dict(zip(sorted(G),
                                   [0.03721197, 0.05395735, 0.04150565,
                                    0.37508082, 0.20599833, 0.28624589]));
        this->dangling_node_index = 1;
        this->dangling_edges = {1: 2, 2: 3,
                               3: 0, 4: 0, 5: 0, 6: 0}
        this->G.dangling_pagerank = dict(zip(sorted(G),
                                            [0.10844518, 0.18618601, 0.0710892,
                                             0.2683668, 0.15919783, 0.20671497]));

    auto test_pagerank() {
        G = this->G
        p = xnetwork.pagerank(G, alpha=0.9, tol=1.e-08);
        for (auto n : G) {
            assert_almost_equal(p[n], G.pagerank[n], places=4);

        nstart = dict((n, random.random()) for n : G);
        p = xnetwork.pagerank(G, alpha=0.9, tol=1.e-08, nstart=nstart);
        for (auto n : G) {
            assert_almost_equal(p[n], G.pagerank[n], places=4);

    /// /// @raises(xnetwork.PowerIterationFailedConvergence);
    auto test_pagerank_max_iter() {
        xnetwork.pagerank(this->G, max_iter=0);

    auto test_numpy_pagerank() {
        G = this->G
        p = xnetwork.pagerank_numpy(G, alpha=0.9);
        for (auto n : G) {
            assert_almost_equal(p[n], G.pagerank[n], places=4);
        personalize = dict((n, random.random()) for n : G);
        p = xnetwork.pagerank_numpy(G, alpha=0.9, personalization=personalize);

    auto test_google_matrix() {
        G = this->G
        M = xnetwork.google_matrix(G, alpha=0.9, nodelist=sorted(G));
        e, ev = numpy.linalg.eig(M.T);
        p = numpy.array(ev[:, 0] / ev[:, 0].sum())[:, 0];
        for (auto [a, b] : zip(p, this->G.pagerank.values()) {
            assert_almost_equal(a, b);

    auto test_personalization() {
        G = xnetwork.complete_graph(4);
        personalize = {0: 1, 1: 1, 2: 4, 3: 4}
        answer = {0: 0.23246732615667579, 1: 0.23246732615667579, 2: 0.267532673843324, 3: 0.2675326738433241}
        p = xnetwork.pagerank(G, alpha=0.85, personalization=personalize);
        for (auto n : G) {
            assert_almost_equal(p[n], answer[n], places=4);

    auto test_zero_personalization_vector() {
        G = xnetwork.complete_graph(4);
        personalize = {0: 0, 1: 0, 2: 0, 3: 0}
        assert_raises(ZeroDivisionError, xnetwork.pagerank, G,
                      personalization=personalize);

    auto test_one_nonzero_personalization_value() {
        G = xnetwork.complete_graph(4);
        personalize = {0: 0, 1: 0, 2: 0, 3: 1}
        answer = {0: 0.22077931820379187, 1: 0.22077931820379187, 2: 0.22077931820379187, 3: 0.3376620453886241}
        p = xnetwork.pagerank(G, alpha=0.85, personalization=personalize);
        for (auto n : G) {
            assert_almost_equal(p[n], answer[n], places=4);

    auto test_incomplete_personalization() {
        G = xnetwork.complete_graph(4);
        personalize = {3: 1}
        answer = {0: 0.22077931820379187, 1: 0.22077931820379187, 2: 0.22077931820379187, 3: 0.3376620453886241}
        p = xnetwork.pagerank(G, alpha=0.85, personalization=personalize);
        for (auto n : G) {
            assert_almost_equal(p[n], answer[n], places=4);

    auto test_dangling_matrix() {
        /**
        Tests that the google_matrix doesn"t change } catch (for the dangling
        nodes.
         */
        G = this->G
        dangling = this->dangling_edges
        dangling_sum = double(sum(dangling.values()));
        M1 = xnetwork.google_matrix(G, personalization=dangling);
        M2 = xnetwork.google_matrix(G, personalization=dangling,
                                    dangling=dangling);
        for (auto i : range(len(G)) {
            for (auto j : range(len(G)) {
                if (i == this->dangling_node_index && (j + 1] : dangling) {
                    assert_almost_equal(M2[i, j],
                                        dangling[j + 1] / dangling_sum,
                                        places=4);
                } else {
                    assert_almost_equal(M2[i, j], M1[i, j], places=4);

    auto test_dangling_pagerank() {
        pr = xnetwork.pagerank(this->G, dangling=this->dangling_edges);
        for (auto n : this->G) {
            assert_almost_equal(pr[n], this->G.dangling_pagerank[n], places=4);

    auto test_dangling_numpy_pagerank() {
        pr = xnetwork.pagerank_numpy(this->G, dangling=this->dangling_edges);
        for (auto n : this->G) {
            assert_almost_equal(pr[n], this->G.dangling_pagerank[n], places=4);

    auto test_empty() {
        G = xnetwork.Graph();
        assert_equal(xnetwork.pagerank(G), {});
        assert_equal(xnetwork.pagerank_numpy(G), {});
        assert_equal(xnetwork.google_matrix(G).shape, (0, 0));


class TestPageRankScipy(TestPageRank) {

    /// @classmethod
    auto setupClass(cls) {
        global scipy
        try {
            import scipy
        } catch (ImportError) {
            throw SkipTest("SciPy not available.");

    auto test_scipy_pagerank() {
        G = this->G
        p = xnetwork.pagerank_scipy(G, alpha=0.9, tol=1.e-08);
        for (auto n : G) {
            assert_almost_equal(p[n], G.pagerank[n], places=4);
        personalize = dict((n, random.random()) for n : G);
        p = xnetwork.pagerank_scipy(G, alpha=0.9, tol=1.e-08,
                                    personalization=personalize);

    /// /// @raises(xnetwork.PowerIterationFailedConvergence);
    auto test_scipy_pagerank_max_iter() {
        xnetwork.pagerank_scipy(this->G, max_iter=0);

    auto test_dangling_scipy_pagerank() {
        pr = xnetwork.pagerank_scipy(this->G, dangling=this->dangling_edges);
        for (auto n : this->G) {
            assert_almost_equal(pr[n], this->G.dangling_pagerank[n], places=4);

    auto test_empty_scipy() {
        G = xnetwork.Graph();
        assert_equal(xnetwork.pagerank_scipy(G), {});
