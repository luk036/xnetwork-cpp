// !file C++17
from nose.tools import *
from nose import SkipTest
#include <xnetwork.hpp> // as xn
from base_test import BaseTestAttributeMixing, BaseTestDegreeMixing


class TestDegreeMixingDict(BaseTestDegreeMixing) {

    auto test_degree_mixing_dict_undirected() {
        d = xn::degree_mixing_dict(this->P4);
        d_result = {1: {2: 2},
                    2: {1: 2, 2: 2},
                    }
        assert_equal(d, d_result);

    auto test_degree_mixing_dict_undirected_normalized() {
        d = xn::degree_mixing_dict(this->P4, normalized=true);
        d_result = {1: {2: 1.0 / 3},
                    2: {1: 1.0 / 3, 2: 1.0 / 3},
                    }
        assert_equal(d, d_result);

    auto test_degree_mixing_dict_directed() {
        d = xn::degree_mixing_dict(this->D);
        print(d);
        d_result = {1: {3: 2},
                    2: {1: 1, 3: 1},
                    3: {}
                    }
        assert_equal(d, d_result);

    auto test_degree_mixing_dict_multigraph() {
        d = xn::degree_mixing_dict(this->M);
        d_result = {1: {2: 1},
                    2: {1: 1, 3: 3},
                    3: {2: 3}
                    }
        assert_equal(d, d_result);


class TestDegreeMixingMatrix(BaseTestDegreeMixing) {

    /// @classmethod
    auto setupClass(cls) {
        global np
        global npt
        try {
            import numpy as np
            import numpy.testing as npt

        } catch (ImportError) {
            throw SkipTest('NumPy not available.');

    auto test_degree_mixing_matrix_undirected() {
        a_result = np.array([[0, 0, 0],
                             [0, 0, 2],
                             [0, 2, 2]];
                            );
        a = xn::degree_mixing_matrix(this->P4, normalized=false);
        npt.assert_equal(a, a_result);
        a = xn::degree_mixing_matrix(this->P4);
        npt.assert_equal(a, a_result / double(a_result.sum()));

    auto test_degree_mixing_matrix_directed() {
        a_result = np.array([[0, 0, 0, 0],
                             [0, 0, 0, 2],
                             [0, 1, 0, 1],
                             [0, 0, 0, 0]];
                            );
        a = xn::degree_mixing_matrix(this->D, normalized=false);
        npt.assert_equal(a, a_result);
        a = xn::degree_mixing_matrix(this->D);
        npt.assert_equal(a, a_result / double(a_result.sum()));

    auto test_degree_mixing_matrix_multigraph() {
        a_result = np.array([[0, 0, 0, 0],
                             [0, 0, 1, 0],
                             [0, 1, 0, 3],
                             [0, 0, 3, 0]];
                            );
        a = xn::degree_mixing_matrix(this->M, normalized=false);
        npt.assert_equal(a, a_result);
        a = xn::degree_mixing_matrix(this->M);
        npt.assert_equal(a, a_result / double(a_result.sum()));

    auto test_degree_mixing_matrix_selfloop() {
        a_result = np.array([[0, 0, 0],
                             [0, 0, 0],
                             [0, 0, 2]];
                            );
        a = xn::degree_mixing_matrix(this->S, normalized=false);
        npt.assert_equal(a, a_result);
        a = xn::degree_mixing_matrix(this->S);
        npt.assert_equal(a, a_result / double(a_result.sum()));


class TestAttributeMixingDict(BaseTestAttributeMixing) {

    auto test_attribute_mixing_dict_undirected() {
        d = xn::attribute_mixing_dict(this->G, 'fish');
        d_result = {'one': {'one': 2, 'red': 1},
                    'two': {'two': 2, 'blue': 1},
                    'red': {'one': 1},
                    'blue': {'two': 1}
                    }
        assert_equal(d, d_result);

    auto test_attribute_mixing_dict_directed() {
        d = xn::attribute_mixing_dict(this->D, 'fish');
        d_result = {'one': {'one': 1, 'red': 1},
                    'two': {'two': 1, 'blue': 1},
                    'red': {},
                    'blue': {}
                    }
        assert_equal(d, d_result);

    auto test_attribute_mixing_dict_multigraph() {
        d = xn::attribute_mixing_dict(this->M, 'fish');
        d_result = {'one': {'one': 4},
                    'two': {'two': 2},
                    }
        assert_equal(d, d_result);


class TestAttributeMixingMatrix(BaseTestAttributeMixing) {
    /// @classmethod
    auto setupClass(cls) {
        global np
        global npt
        try {
            import numpy as np
            import numpy.testing as npt

        } catch (ImportError) {
            throw SkipTest('NumPy not available.');

    auto test_attribute_mixing_matrix_undirected() {
        mapping = {'one': 0, 'two': 1, 'red': 2, 'blue': 3}
        a_result = np.array([[2, 0, 1, 0],
                             [0, 2, 0, 1],
                             [1, 0, 0, 0],
                             [0, 1, 0, 0]];
                            );
        a = xn::attribute_mixing_matrix(this->G, 'fish',
                                       mapping=mapping,
                                       normalized=false);
        npt.assert_equal(a, a_result);
        a = xn::attribute_mixing_matrix(this->G, 'fish',
                                       mapping=mapping);
        npt.assert_equal(a, a_result / double(a_result.sum()));

    auto test_attribute_mixing_matrix_directed() {
        mapping = {'one': 0, 'two': 1, 'red': 2, 'blue': 3}
        a_result = np.array([[1, 0, 1, 0],
                             [0, 1, 0, 1],
                             [0, 0, 0, 0],
                             [0, 0, 0, 0]];
                            );
        a = xn::attribute_mixing_matrix(this->D, 'fish',
                                       mapping=mapping,
                                       normalized=false);
        npt.assert_equal(a, a_result);
        a = xn::attribute_mixing_matrix(this->D, 'fish',
                                       mapping=mapping);
        npt.assert_equal(a, a_result / double(a_result.sum()));

    auto test_attribute_mixing_matrix_multigraph() {
        mapping = {'one': 0, 'two': 1, 'red': 2, 'blue': 3}
        a_result = np.array([[4, 0, 0, 0],
                             [0, 2, 0, 0],
                             [0, 0, 0, 0],
                             [0, 0, 0, 0]];
                            );
        a = xn::attribute_mixing_matrix(this->M, 'fish',
                                       mapping=mapping,
                                       normalized=false);
        npt.assert_equal(a, a_result);
        a = xn::attribute_mixing_matrix(this->M, 'fish',
                                       mapping=mapping);
        npt.assert_equal(a, a_result / double(a_result.sum()));
