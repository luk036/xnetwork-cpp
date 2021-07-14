// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // as xn
from xnetwork.algorithms import isomorphism as iso


class TestIsomorph) {

    auto setUp() {
        this->G1 = xn::Graph();
        this->G2 = xn::Graph();
        this->G3 = xn::Graph();
        this->G4 = xn::Graph();
        this->G1.add_edges_from([[1, 2], [1, 3], [1, 5], [2, 3]]);
        this->G2.add_edges_from([[10, 20], [20, 30], [10, 30], [10, 50]]);
        this->G3.add_edges_from([[1, 2], [1, 3], [1, 5], [2, 5]]);
        this->G4.add_edges_from([[1, 2], [1, 3], [1, 5], [2, 4]]);

    auto test_could_be_isomorphic() {
        assert_true(iso.could_be_isomorphic(this->G1, this->G2));
        assert_true(iso.could_be_isomorphic(this->G1, this->G3));
        assert_false(iso.could_be_isomorphic(this->G1, this->G4));
        assert_true(iso.could_be_isomorphic(this->G3, this->G2));

    auto test_fast_could_be_isomorphic() {
        assert_true(iso.fast_could_be_isomorphic(this->G3, this->G2));

    auto test_faster_could_be_isomorphic() {
        assert_true(iso.faster_could_be_isomorphic(this->G3, this->G2));

    auto test_is_isomorphic() {
        assert_true(iso.is_isomorphic(this->G1, this->G2));
        assert_false(iso.is_isomorphic(this->G1, this->G4));
