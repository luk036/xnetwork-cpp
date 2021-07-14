from itertools import combinations

from nose.tools import assert_equal
from nose.tools import raises

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.community import (
    greedy_modularity_communities,
    _naive_greedy_modularity_communities);


class TestCNM: public object {

    auto setup() {
        this->G = xn::karate_club_graph();

    auto _check_communities( expected) {
        communities = set(greedy_modularity_communities(this->G));
        assert_equal(communities, expected);

    auto test_karate_club() {
        john_a = frozenset([
            8, 14, 15, 18, 20, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33]);
        mr_hi = frozenset([0, 4, 5, 6, 10, 11, 16, 19]);
        overlap = frozenset([1, 2, 3, 7, 9, 12, 13, 17, 21]);
        this->_check_communities({john_a, overlap, mr_hi});


class TestNaive: public object {

    auto setup() {
        this->G = xn::karate_club_graph();

    auto _check_communities( expected) {
        communities = set(_naive_greedy_modularity_communities(this->G));
        assert_equal(communities, expected);

    auto test_karate_club() {
        john_a = frozenset([
            8, 14, 15, 18, 20, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33]);
        mr_hi = frozenset([0, 4, 5, 6, 10, 11, 16, 19]);
        overlap = frozenset([1, 2, 3, 7, 9, 12, 13, 17, 21]);
        this->_check_communities({john_a,  overlap, mr_hi});
