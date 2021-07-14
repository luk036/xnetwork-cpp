from nose.tools import assert_raises
#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.approximation.steinertree import metric_closure
from xnetwork.algorithms.approximation.steinertree import steiner_tree
from xnetwork.testing.utils import assert_edges_equal


class TestSteinerTree) {
    auto setUp() {
        G = xn::Graph();
        G.add_edge(1, 2, weight=10);
        G.add_edge(2, 3, weight=10);
        G.add_edge(3, 4, weight=10);
        G.add_edge(4, 5, weight=10);
        G.add_edge(5, 6, weight=10);
        G.add_edge(2, 7, weight=1);
        G.add_edge(7, 5, weight=1);
        this->G = G;
        this->term_nodes = [1, 2, 3, 4, 5];

    auto test_connected_metric_closure() {
        G = this->G.copy();
        G.add_node(100);
        assert_raises(xn::XNetworkError, metric_closure, G);

    auto test_metric_closure() {
        M = metric_closure(this->G);
        mc = [(1, 2, {"distance": 10, "path": [1, 2]}),
              (1, 3, {"distance": 20, "path": [1, 2, 3]}),
              (1, 4, {"distance": 22, "path": [1, 2, 7, 5, 4]}),
              (1, 5, {"distance": 12, "path": [1, 2, 7, 5]}),
              (1, 6, {"distance": 22, "path": [1, 2, 7, 5, 6]}),
              (1, 7, {"distance": 11, "path": [1, 2, 7]}),
              (2, 3, {"distance": 10, "path": [2, 3]}),
              (2, 4, {"distance": 12, "path": [2, 7, 5, 4]}),
              (2, 5, {"distance": 2, "path": [2, 7, 5]}),
              (2, 6, {"distance": 12, "path": [2, 7, 5, 6]}),
              (2, 7, {"distance": 1, "path": [2, 7]}),
              (3, 4, {"distance": 10, "path": [3, 4]}),
              (3, 5, {"distance": 12, "path": [3, 2, 7, 5]}),
              (3, 6, {"distance": 22, "path": [3, 2, 7, 5, 6]}),
              (3, 7, {"distance": 11, "path": [3, 2, 7]}),
              (4, 5, {"distance": 10, "path": [4, 5]}),
              (4, 6, {"distance": 20, "path": [4, 5, 6]}),
              (4, 7, {"distance": 11, "path": [4, 5, 7]}),
              (5, 6, {"distance": 10, "path": [5, 6]}),
              (5, 7, {"distance": 1, "path": [5, 7]}),
              (6, 7, {"distance": 11, "path": [6, 5, 7]})];
        assert_edges_equal(list(M.edges(data=true)), mc);

    auto test_steiner_tree() {
        S = steiner_tree(this->G, this->term_nodes);
        expected_steiner_tree = [(1, 2, {"weight": 10}),
                                 (2, 3, {"weight": 10}),
                                 (2, 7, {"weight": 1}),
                                 (3, 4, {"weight": 10}),
                                 (5, 7, {"weight": 1})];
        assert_edges_equal(list(S.edges(data=true)), expected_steiner_tree);
