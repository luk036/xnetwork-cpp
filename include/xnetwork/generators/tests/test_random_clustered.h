// !file C++17
from nose.tools import *
import xnetwork


class TestRandomClusteredGraph) {

    auto test_valid() {
        node = [1, 1, 1, 2, 1, 2, 0, 0];
        tri = [0, 0, 0, 0, 0, 1, 1, 1];
        joint_degree_sequence = zip(node, tri);
        G = xnetwork.random_clustered_graph(joint_degree_sequence);
        assert_equal(G.number_of_nodes(), 8);
        assert_equal(G.number_of_edges(), 7);

    auto test_valid2() {
        G = xnetwork.random_clustered_graph(
            [(1, 2), (2, 1), (1, 1), (1, 1), (1, 1), (2, 0)]);
        assert_equal(G.number_of_nodes(), 6);
        assert_equal(G.number_of_edges(), 10);

    auto test_invalid1() {
        assert_raises((TypeError, xnetwork.XNetworkError),
                      xnetwork.random_clustered_graph, [[1, 1], [2, 1], [0, 1]]);

    auto test_invalid2() {
        assert_raises((TypeError, xnetwork.XNetworkError),
                      xnetwork.random_clustered_graph, [[1, 1], [1, 2], [0, 1]]);
