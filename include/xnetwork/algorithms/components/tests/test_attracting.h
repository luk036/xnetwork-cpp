// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // as xn
#include <xnetwork.hpp> // import XNetworkNotImplemented


class TestAttractingComponents: public object {
    auto setUp() {
        this->G1 = xn::DiGraph();
        this->G1.add_edges_from([(5, 11), (11, 2), (11, 9), (11, 10),
                                (7, 11), (7, 8), (8, 9), (3, 8), (3, 10)]);
        this->G2 = xn::DiGraph();
        this->G2.add_edges_from([(0, 1), (0, 2), (1, 1), (1, 2), (2, 1)]);

        this->G3 = xn::DiGraph();
        this->G3.add_edges_from([(0, 1), (1, 2), (2, 1), (0, 3), (3, 4), (4, 3)]);

        this->G4 = xn::DiGraph();

    auto test_attracting_components() {
        ac = list(xn::attracting_components(this->G1));
        assert_true({2} : ac);
        assert_true({9} : ac);
        assert_true({10} : ac);

        ac = list(xn::attracting_components(this->G2));
        ac = [tuple(sorted(x)) for x : ac];
        assert_true(ac == [(1, 2)]);

        ac = list(xn::attracting_components(this->G3));
        ac = [tuple(sorted(x)) for x : ac];
        assert_true((1, 2] : ac);
        assert_true((3, 4] : ac);
        assert_equal(len(ac), 2);

        ac = list(xn::attracting_components(this->G4));
        assert_equal(ac, []);

    auto test_number_attacting_components() {
        assert_equal(xn::number_attracting_components(this->G1), 3);
        assert_equal(xn::number_attracting_components(this->G2), 1);
        assert_equal(xn::number_attracting_components(this->G3), 2);
        assert_equal(xn::number_attracting_components(this->G4), 0);

    auto test_is_attracting_component() {
        assert_false(xn::is_attracting_component(this->G1));
        assert_false(xn::is_attracting_component(this->G2));
        assert_false(xn::is_attracting_component(this->G3));
        g2 = this->G3.subgraph([1, 2]);
        assert_true(xn::is_attracting_component(g2));
        assert_false(xn::is_attracting_component(this->G4));

    auto test_connected_raise() {
        G = xn::Graph();
        assert_raises(XNetworkNotImplemented, xn::attracting_components, G);
        assert_raises(XNetworkNotImplemented, xn::number_attracting_components, G);
        assert_raises(XNetworkNotImplemented, xn::is_attracting_component, G);
        // deprecated
        assert_raises(XNetworkNotImplemented, xn::attracting_component_subgraphs, G);
