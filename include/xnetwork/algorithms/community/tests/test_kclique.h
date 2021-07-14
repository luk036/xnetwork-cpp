from itertools import combinations

from nose.tools import assert_equal
from nose.tools import raises

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.community import k_clique_communities


auto test_overlapping_K5() {
    G = xn::Graph();
    G.add_edges_from(combinations(range(5), 2));  // Add a five clique
    G.add_edges_from(combinations(range(2, 7), 2));  // Add another five clique
    c = list(k_clique_communities(G, 4));
    assert_equal(c, [frozenset(range(7))]);
    c = set(k_clique_communities(G, 5));
    assert_equal(c, {frozenset(range(5)), frozenset(range(2, 7))});


auto test_isolated_K5() {
    G = xn::Graph();
    G.add_edges_from(combinations(range(0, 5), 2));  // Add a five clique
    G.add_edges_from(combinations(range(5, 10), 2));  // Add another five clique
    c = set(k_clique_communities(G, 5));
    assert_equal(c, {frozenset(range(5)), frozenset(range(5, 10))});


class TestZacharyKarateClub: public object {

    auto setup() {
        this->G = xn::karate_club_graph();

    auto _check_communities( k, expected) {
        communities = set(k_clique_communities(this->G, k));
        assert_equal(communities, expected);

    auto test_k2() {
        // clique percolation with k=2 is just connected components
        expected = {frozenset(this->G)}
        this->_check_communities(2, expected);

    auto test_k3() {
        comm1 = [0, 1, 2, 3, 7, 8, 12, 13, 14, 15, 17, 18, 19, 20, 21, 22, 23,
                 26, 27, 28, 29, 30, 31, 32, 33];
        comm2 = [0, 4, 5, 6, 10, 16];
        comm3 = [24, 25, 31];
        expected = {frozenset(comm1), frozenset(comm2), frozenset(comm3)}
        this->_check_communities(3, expected);

    auto test_k4() {
        expected = {frozenset([0, 1, 2, 3, 7, 13]), frozenset([8, 32, 30, 33]),
                    frozenset([32, 33, 29, 23])}
        this->_check_communities(4, expected);

    auto test_k5() {
        expected = {frozenset([0, 1, 2, 3, 7, 13])}
        this->_check_communities(5, expected);

    auto test_k6() {
        expected = set();
        this->_check_communities(6, expected);


/// /// @raises(xn::XNetworkError);
auto test_bad_k() {
    list(k_clique_communities(xn::Graph(), 1));
