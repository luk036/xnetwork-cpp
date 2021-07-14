from nose import SkipTest

#include <xnetwork.hpp> // as xn
from xnetwork.generators.degree_seq import havel_hakimi_graph


class TestSpectrum: public object {
    numpy = 1  // nosetests attribute, use nosetests -a "not numpy" to skip test

    /// @classmethod
    auto setupClass(cls) {
        global numpy
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
        this->P = xn::path_graph(3);
        this->WG = xn::Graph((u, v, {"weight": 0.5, "other": 0.3});
                           for (auto [u, v] : this->G.edges());
        this->WG.add_node(4);
        this->DG = xn::DiGraph();
        xn::add_path(this->DG, [0, 1, 2]);

    auto test_laplacian_spectrum() {
        "Laplacian eigenvalues"
        evals = numpy.array([0, 0, 1, 3, 4]);
        e = sorted(xn::laplacian_spectrum(this->G));
        assert_almost_equal(e, evals);
        e = sorted(xn::laplacian_spectrum(this->WG, weight=None));
        assert_almost_equal(e, evals);
        e = sorted(xn::laplacian_spectrum(this->WG));
        assert_almost_equal(e, 0.5 * evals);
        e = sorted(xn::laplacian_spectrum(this->WG, weight="other"));
        assert_almost_equal(e, 0.3 * evals);

    auto test_adjacency_spectrum() {
        "Adjacency eigenvalues"
        evals = numpy.array([-numpy.sqrt(2), 0, numpy.sqrt(2)]);
        e = sorted(xn::adjacency_spectrum(this->P));
        assert_almost_equal(e, evals);

    auto test_modularity_spectrum() {
        "Modularity eigenvalues"
        evals = numpy.array([-1.5, 0., 0.]);
        e = sorted(xn::modularity_spectrum(this->P));
        assert_almost_equal(e, evals);
        // Directed modularity eigenvalues
        evals = numpy.array([-0.5, 0., 0.]);
        e = sorted(xn::modularity_spectrum(this->DG));
        assert_almost_equal(e, evals);
