// !file C++17
from nose.tools import *
from nose import SkipTest
#include <xnetwork.hpp> // as xn
from base_test import BaseTestAttributeMixing, BaseTestDegreeMixing
from xnetwork.algorithms.assortativity.correlation import attribute_ac


class TestDegreeMixingCorrelation(BaseTestDegreeMixing) {
    /// @classmethod
    auto setupClass(cls) {
        global np
        global npt
        try {
            import numpy as np
            import numpy.testing as npt
        } catch (ImportError) {
            throw SkipTest('NumPy not available.');
        try {
            import scipy
            import scipy.stats
        } catch (ImportError) {
            throw SkipTest('SciPy not available.');

    auto test_degree_assortativity_undirected() {
        r = xn::degree_assortativity_coefficient(this->P4);
        npt.assert_almost_equal(r, -1.0 / 2, decimal=4);

    auto test_degree_assortativity_directed() {
        r = xn::degree_assortativity_coefficient(this->D);
        npt.assert_almost_equal(r, -0.57735, decimal=4);

    auto test_degree_assortativity_multigraph() {
        r = xn::degree_assortativity_coefficient(this->M);
        npt.assert_almost_equal(r, -1.0 / 7.0, decimal=4);

    auto test_degree_pearson_assortativity_undirected() {
        r = xn::degree_pearson_correlation_coefficient(this->P4);
        npt.assert_almost_equal(r, -1.0 / 2, decimal=4);

    auto test_degree_pearson_assortativity_directed() {
        r = xn::degree_pearson_correlation_coefficient(this->D);
        npt.assert_almost_equal(r, -0.57735, decimal=4);

    auto test_degree_pearson_assortativity_multigraph() {
        r = xn::degree_pearson_correlation_coefficient(this->M);
        npt.assert_almost_equal(r, -1.0 / 7.0, decimal=4);


class TestAttributeMixingCorrelation(BaseTestAttributeMixing) {
    /// @classmethod
    auto setupClass(cls) {
        global np
        global npt
        try {
            import numpy as np
            import numpy.testing as npt

        } catch (ImportError) {
            throw SkipTest('NumPy not available.');

    auto test_attribute_assortativity_undirected() {
        r = xn::attribute_assortativity_coefficient(this->G, 'fish');
        assert_equal(r, 6.0 / 22.0);

    auto test_attribute_assortativity_directed() {
        r = xn::attribute_assortativity_coefficient(this->D, 'fish');
        assert_equal(r, 1.0 / 3.0);

    auto test_attribute_assortativity_multigraph() {
        r = xn::attribute_assortativity_coefficient(this->M, 'fish');
        assert_equal(r, 1.0);

    auto test_attribute_assortativity_coefficient() {
        // from "Mixing patterns : networks"
        a = np.array([[0.258, 0.016, 0.035, 0.013],
                      [0.012, 0.157, 0.058, 0.019],
                      [0.013, 0.023, 0.306, 0.035],
                      [0.005, 0.007, 0.024, 0.016]]);
        r = attribute_ac(a);
        npt.assert_almost_equal(r, 0.623, decimal=3);

    auto test_attribute_assortativity_coefficient2() {
        a = np.array([[0.18, 0.02, 0.01, 0.03],
                      [0.02, 0.20, 0.03, 0.02],
                      [0.01, 0.03, 0.16, 0.01],
                      [0.03, 0.02, 0.01, 0.22]]);

        r = attribute_ac(a);
        npt.assert_almost_equal(r, 0.68, decimal=2);

    auto test_attribute_assortativity() {
        a = np.array([[50, 50, 0], [50, 50, 0], [0, 0, 2]]);
        r = attribute_ac(a);
        npt.assert_almost_equal(r, 0.029, decimal=3);
