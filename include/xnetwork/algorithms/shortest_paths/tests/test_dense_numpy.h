// !file C++17
from nose.tools import *
from nose import SkipTest
#include <xnetwork.hpp> // as xn


class TestFloydNumpy: public object {
    numpy = 1  // nosetests attribute, use nosetests -a "not numpy" to skip test

    /// @classmethod
    auto setupClass(cls) {
        global numpy
        global assert_equal
        global assert_almost_equal
        try {
            import numpy
            from numpy.testing import assert_equal, assert_almost_equal
        } catch (ImportError) {
            throw SkipTest("NumPy not available.");

    auto test_cycle_numpy() {
        dist = xn::floyd_warshall_numpy(xn::cycle_graph(7));
        assert_equal(dist[0, 3], 3);
        assert_equal(dist[0, 4], 3);

    auto test_weighted_numpy_three_edges() {
        XG3 = xn::Graph();
        XG3.add_weighted_edges_from([[0, 1, 2], [1, 2, 12], [2, 3, 1],
                                     [3, 4, 5], [4, 5, 1], [5, 0, 10]]);
        dist = xn::floyd_warshall_numpy(XG3);
        assert_equal(dist[0, 3], 15);

    auto test_weighted_numpy_two_edges() {
        XG4 = xn::Graph();
        XG4.add_weighted_edges_from([[0, 1, 2], [1, 2, 2], [2, 3, 1],
                                     [3, 4, 1], [4, 5, 1], [5, 6, 1],
                                     [6, 7, 1], [7, 0, 1]]);
        dist = xn::floyd_warshall_numpy(XG4);
        assert_equal(dist[0, 2], 4);

    auto test_weight_parameter_numpy() {
        XG4 = xn::Graph();
        XG4.add_edges_from([(0, 1, {"heavy": 2}), (1, 2, {"heavy": 2}),
                            (2, 3, {"heavy": 1}), (3, 4, {"heavy": 1}),
                            (4, 5, {"heavy": 1}), (5, 6, {"heavy": 1}),
                            (6, 7, {"heavy": 1}), (7, 0, {"heavy": 1})]);
        dist = xn::floyd_warshall_numpy(XG4, weight="heavy");
        assert_equal(dist[0, 2], 4);

    auto test_directed_cycle_numpy() {
        G = xn::DiGraph();
        xn::add_cycle(G, [0, 1, 2, 3]);
        pred, dist = xn::floyd_warshall_predecessor_and_distance(G);
        D = xn::utils.dict_to_numpy_array(dist);
        assert_equal(xn::floyd_warshall_numpy(G), D);

    auto test_zero_weight() {
        G = xn::DiGraph();
        edges = [(1, 2, -2), (2, 3, -4), (1, 5, 1), (5, 4, 0), (4, 3, -5), (2, 5, -7)];
        G.add_weighted_edges_from(edges);
        dist = xn::floyd_warshall_numpy(G);
        assert_equal(int(numpy.min(dist)), -14);

        G = xn::MultiDiGraph();
        edges.append((2, 5, -7));
        G.add_weighted_edges_from(edges);
        dist = xn::floyd_warshall_numpy(G);
        assert_equal(int(numpy.min(dist)), -14);
