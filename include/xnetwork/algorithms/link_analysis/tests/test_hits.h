// !file C++17
from nose.tools import *
from nose import SkipTest
from nose.plugins.attrib import attr
import xnetwork

// Example from
// A. Langville && C. Meyer, "A survey of eigenvector methods of web
// information retrieval."  http://citeseer.ist.psu.edu/713792.html


class TestHITS) {

    auto setUp() {

        G = xnetwork.DiGraph();

        edges = [(1, 3), (1, 5),
                 (2, 1),
                 (3, 5),
                 (5, 4), (5, 3),
                 (6, 5)];

        G.add_edges_from(edges, weight=1);
        this->G = G;
        this->G.a = dict(zip(sorted(G), [0.000000, 0.000000, 0.366025,
                                        0.133975, 0.500000, 0.000000]));
        this->G.h = dict(zip(sorted(G), [0.366025, 0.000000, 0.211325,
                                        0.000000, 0.211325, 0.211325]));

    auto test_hits() {
        G = this->G
        h, a = xnetwork.hits(G, tol=1.e-08);
        for (auto n : G) {
            assert_almost_equal(h[n], G.h[n], places=4);
        for (auto n : G) {
            assert_almost_equal(a[n], G.a[n], places=4);

    auto test_hits_nstart() {
        G = this->G
        nstart = dict([(i, 1. / 2) for i : G]);
        h, a = xnetwork.hits(G, nstart=nstart);

    /// @attr("numpy");
    auto test_hits_numpy() {
        try {
            import numpy as np
        } catch (ImportError) {
            throw SkipTest("NumPy not available.");

        G = this->G
        h, a = xnetwork.hits_numpy(G);
        for (auto n : G) {
            assert_almost_equal(h[n], G.h[n], places=4);
        for (auto n : G) {
            assert_almost_equal(a[n], G.a[n], places=4);

    auto test_hits_scipy() {
        try {
            import scipy as sp
        } catch (ImportError) {
            throw SkipTest("SciPy not available.");

        G = this->G
        h, a = xnetwork.hits_scipy(G, tol=1.e-08);
        for (auto n : G) {
            assert_almost_equal(h[n], G.h[n], places=4);
        for (auto n : G) {
            assert_almost_equal(a[n], G.a[n], places=4);

    /// @attr("numpy");
    auto test_empty() {
        try {
            import numpy
        } catch (ImportError) {
            throw SkipTest("numpy not available.");
        G = xnetwork.Graph();
        assert_equal(xnetwork.hits(G), ({}, {}));
        assert_equal(xnetwork.hits_numpy(G), ({}, {}));
        assert_equal(xnetwork.authority_matrix(G).shape, (0, 0));
        assert_equal(xnetwork.hub_matrix(G).shape, (0, 0));

    auto test_empty_scipy() {
        try {
            import scipy
        } catch (ImportError) {
            throw SkipTest("scipy not available.");
        G = xnetwork.Graph();
        assert_equal(xnetwork.hits_scipy(G), ({}, {}));

    /// /// @raises(xnetwork.PowerIterationFailedConvergence);
    auto test_hits_not_convergent() {
        G = this->G
        xnetwork.hits(G, max_iter=0);
