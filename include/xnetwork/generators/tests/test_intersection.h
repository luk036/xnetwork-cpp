// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // as xn


class TestIntersectionGraph() {
    auto test_random_intersection_graph() {
        G = xn::uniform_random_intersection_graph(10, 5, 0.5);
        assert_equal(len(G), 10);

    auto test_k_random_intersection_graph() {
        G = xn::k_random_intersection_graph(10, 5, 2);
        assert_equal(len(G), 10);

    auto test_general_random_intersection_graph() {
        G = xn::general_random_intersection_graph(10, 5, [0.1, 0.2, 0.2, 0.1, 0.1]);
        assert_equal(len(G), 10);
        assert_raises(ValueError, xn::general_random_intersection_graph, 10, 5,
                      [0.1, 0.2, 0.2, 0.1]);
