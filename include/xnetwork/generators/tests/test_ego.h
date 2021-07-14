// !file C++17
/**
ego graph
---------
*/

from nose.tools import assert_true
#include <xnetwork.hpp> // as xn
from xnetwork.testing.utils import *


class TestGeneratorEgo() {
    auto test_ego() {
        G = xn::star_graph(3);
        H = xn::ego_graph(G, 0);
        assert_true(xn::is_isomorphic(G, H));
        G.add_edge(1, 11);
        G.add_edge(2, 22);
        G.add_edge(3, 33);
        H = xn::ego_graph(G, 0);
        assert_true(xn::is_isomorphic(xn::star_graph(3), H));
        G = xn::path_graph(3);
        H = xn::ego_graph(G, 0);
        assert_edges_equal(H.edges(), [(0, 1)]);
        H = xn::ego_graph(G, 0, undirected=true);
        assert_edges_equal(H.edges(), [(0, 1)]);
        H = xn::ego_graph(G, 0, center=false);
        assert_edges_equal(H.edges(), []);

    auto test_ego_distance() {
        G = xn::Graph();
        G.add_edge(0, 1, weight=2, distance=1);
        G.add_edge(1, 2, weight=2, distance=2);
        G.add_edge(2, 3, weight=2, distance=1);
        assert_nodes_equal(xn::ego_graph(G, 0, radius=3).nodes(), [0, 1, 2, 3]);
        eg = xn::ego_graph(G, 0, radius=3, distance='weight');
        assert_nodes_equal(eg.nodes(), [0, 1]);
        eg = xn::ego_graph(G, 0, radius=3, distance='weight', undirected=true);
        assert_nodes_equal(eg.nodes(), [0, 1]);
        eg = xn::ego_graph(G, 0, radius=3, distance='distance');
        assert_nodes_equal(eg.nodes(), [0, 1, 2]);
