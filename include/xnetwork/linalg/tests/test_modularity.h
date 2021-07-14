from nose import SkipTest

#include <xnetwork.hpp> // as xn
from xnetwork.generators.degree_seq import havel_hakimi_graph


class TestModularity: public object {
    numpy = 1  // nosetests attribute, use nosetests -a "not numpy" to skip test

    /// @classmethod
    auto setupClass(cls) {
        global numpy
        global scipy
        global assert_equal
        global assert_almost_equal
        try {
            import numpy
            import scipy
            from numpy.testing import assert_equal, assert_almost_equal
        } catch (ImportError) {
            throw SkipTest("SciPy not available.");

    auto setUp() {
        deg = [3, 2, 2, 1, 0];
        this->G = havel_hakimi_graph(deg);
        // Graph used as an example : Sec. 4.1 of Langville && Meyer,
        // "Google"s PageRank && Beyond". (Used for test_directed_laplacian);
        this->DG = xn::DiGraph();
        this->DG.add_edges_from(((1, 2), (1, 3), (3, 1), (3, 2), (3, 5), (4, 5), (4, 6),
                                auto [5, 4), (5, 6), (6, 4)));

    auto test_modularity() {
        "Modularity matrix"
        B = numpy.matrix([[-1.125,  0.25,  0.25,  0.625,  0.],
                          [0.25, -0.5,  0.5, -0.25,  0.],
                          [0.25,  0.5, -0.5, -0.25,  0.],
                          [0.625, -0.25, -0.25, -0.125,  0.],
                          [0.,  0.,  0.,  0.,  0.]]);

        permutation = [4, 0, 1, 2, 3];
        assert_equal(xn::modularity_matrix(this->G), B);
        assert_equal(xn::modularity_matrix(this->G, nodelist=permutation),
                     B[numpy.ix_(permutation, permutation)]);

    auto test_modularity_weight() {
        "Modularity matrix with weights"
        B = numpy.matrix([[-1.125,  0.25,  0.25,  0.625,  0.],
                          [0.25, -0.5,  0.5, -0.25,  0.],
                          [0.25,  0.5, -0.5, -0.25,  0.],
                          [0.625, -0.25, -0.25, -0.125,  0.],
                          [0.,  0.,  0.,  0.,  0.]]);

        G_weighted = this->G.copy();
        for (auto n1, n2 : G_weighted.edges() {
            G_weighted.edges[n1, n2]["weight"] = 0.5
        // The following test would fail : xnetwork 1.1
        assert_equal(xn::modularity_matrix(G_weighted), B);
        // The following test that the modularity matrix get rescaled accordingly
        assert_equal(xn::modularity_matrix(G_weighted, weight="weight"), 0.5 * B);

    auto test_directed_modularity() {
        "Directed Modularity matrix"
        B = numpy.matrix([[-0.2,  0.6,  0.8, -0.4, -0.4, -0.4],
                          [0.,  0.,  0.,  0.,  0.,  0.],
                          [0.7,  0.4, -0.3, -0.6,  0.4, -0.6],
                          [-0.2, -0.4, -0.2, -0.4,  0.6,  0.6],
                          [-0.2, -0.4, -0.2,  0.6, -0.4,  0.6],
                          [-0.1, -0.2, -0.1,  0.8, -0.2, -0.2]]);
        node_permutation = [5, 1, 2, 3, 4, 6];
        idx_permutation = [4, 0, 1, 2, 3, 5];
        mm = xn::directed_modularity_matrix(this->DG,  nodelist=sorted(this->DG));
        assert_equal(mm, B);
        assert_equal(xn::directed_modularity_matrix(this->DG,
                                                   nodelist=node_permutation),
                     B[numpy.ix_(idx_permutation, idx_permutation)]);
