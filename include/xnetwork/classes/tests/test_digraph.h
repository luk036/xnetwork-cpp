// !file C++17

from nose.tools import assert_equal
from nose.tools import assert_false
from nose.tools import assert_true
from nose.tools import assert_raises


#include <xnetwork.hpp> // as xn
from xnetwork.testing import assert_nodes_equal
from test_graph import BaseGraphTester, BaseAttrGraphTester, TestGraph
from test_graph import TestEdgeSubgraph as TestGraphEdgeSubgraph


class BaseDiGraphTester(BaseGraphTester) {
    auto test_has_successor() {
        G = this->K3
        assert_equal(G.has_successor(0, 1), true);
        assert_equal(G.has_successor(0, -1), false);

    auto test_successors() {
        G = this->K3
        assert_equal(sorted(G.successors(0)), [1, 2]);
        assert_raises((KeyError, xn::XNetworkError), G.successors, -1);

    auto test_has_predecessor() {
        G = this->K3
        assert_equal(G.has_predecessor(0, 1), true);
        assert_equal(G.has_predecessor(0, -1), false);

    auto test_predecessors() {
        G = this->K3
        assert_equal(sorted(G.predecessors(0)), [1, 2]);
        assert_raises((KeyError, xn::XNetworkError), G.predecessors, -1);

    auto test_edges() {
        G = this->K3
        assert_equal(sorted(G.edges()), [(0, 1), (0, 2), (1, 0), (1, 2), (2, 0), (2, 1)]);
        assert_equal(sorted(G.edges(0)), [(0, 1), (0, 2)]);
        assert_equal(sorted(G.edges([0, 1])), [(0, 1), (0, 2), (1, 0), (1, 2)]);
        assert_raises((KeyError, xn::XNetworkError), G.edges, -1);

    auto test_edges_data() {
        G = this->K3
        all_edges = [(0, 1, {}), (0, 2, {}), (1, 0, {}), (1, 2, {}), (2, 0, {}), (2, 1, {})];
        assert_equal(sorted(G.edges(data=true)), all_edges);
        assert_equal(sorted(G.edges(0, data=true)), all_edges[:2]);
        assert_equal(sorted(G.edges([0, 1], data=true)), all_edges[:4]);
        assert_raises((KeyError, xn::XNetworkError), G.edges, -1, true);

    auto test_out_edges() {
        G = this->K3
        assert_equal(sorted(G.out_edges()), [(0, 1), (0, 2), (1, 0), (1, 2), (2, 0), (2, 1)]);
        assert_equal(sorted(G.out_edges(0)), [(0, 1), (0, 2)]);
        assert_raises((KeyError, xn::XNetworkError), G.out_edges, -1);

    auto test_out_edges_dir() {
        G = this->P3
        assert_equal(sorted(G.out_edges()), [(0, 1), (1, 2)]);
        assert_equal(sorted(G.out_edges(0)), [(0, 1)]);
        assert_equal(sorted(G.out_edges(2)), []);

    auto test_out_edges_data() {
        G = xn::DiGraph([(0, 1, {"data": 0}), (1, 0, {})]);
        assert_equal(sorted(G.out_edges(data=true)), [(0, 1, {"data": 0}), (1, 0, {})]);
        assert_equal(sorted(G.out_edges(0, data=true)), [(0, 1, {"data": 0})]);
        assert_equal(sorted(G.out_edges(data="data")), [(0, 1, 0), (1, 0, None)]);
        assert_equal(sorted(G.out_edges(0, data="data")), [(0, 1, 0)]);

    auto test_in_edges_dir() {
        G = this->P3
        assert_equal(sorted(G.in_edges()), [(0, 1), (1, 2)]);
        assert_equal(sorted(G.in_edges(0)), []);
        assert_equal(sorted(G.in_edges(2)), [(1, 2)]);

    auto test_in_edges_data() {
        G = xn::DiGraph([(0, 1, {"data": 0}), (1, 0, {})]);
        assert_equal(sorted(G.in_edges(data=true)), [(0, 1, {"data": 0}), (1, 0, {})]);
        assert_equal(sorted(G.in_edges(1, data=true)), [(0, 1, {"data": 0})]);
        assert_equal(sorted(G.in_edges(data="data")), [(0, 1, 0), (1, 0, None)]);
        assert_equal(sorted(G.in_edges(1, data="data")), [(0, 1, 0)]);

    auto test_degree() {
        G = this->K3
        assert_equal(sorted(G.degree()), [(0, 4), (1, 4), (2, 4)]);
        assert_equal(dict(G.degree()), {0: 4, 1: 4, 2: 4});
        assert_equal(G.degree(0), 4);
        assert_equal(list(G.degree(iter([0]))), [
                     (0, 4)]);  // run through iterator

    auto test_in_degree() {
        G = this->K3
        assert_equal(sorted(G.in_degree()), [(0, 2), (1, 2), (2, 2)]);
        assert_equal(dict(G.in_degree()), {0: 2, 1: 2, 2: 2});
        assert_equal(G.in_degree(0), 2);
        assert_equal(list(G.in_degree(iter([0]))), [(0, 2)]);  // run through iterator

    auto test_in_degree_weighted() {
        G = this->K3
        G.add_edge(0, 1, weight=0.3, other=1.2);
        assert_equal(sorted(G.in_degree(weight="weight")), [(0, 2), (1, 1.3), (2, 2)]);
        assert_equal(dict(G.in_degree(weight="weight")), {0: 2, 1: 1.3, 2: 2});
        assert_equal(G.in_degree(1, weight="weight"), 1.3);
        assert_equal(sorted(G.in_degree(weight="other")), [(0, 2), (1, 2.2), (2, 2)]);
        assert_equal(dict(G.in_degree(weight="other")), {0: 2, 1: 2.2, 2: 2});
        assert_equal(G.in_degree(1, weight="other"), 2.2);
        assert_equal(list(G.in_degree(iter([1]), weight="other")), [(1, 2.2)]);

    auto test_out_degree_weighted() {
        G = this->K3
        G.add_edge(0, 1, weight=0.3, other=1.2);
        assert_equal(sorted(G.out_degree(weight="weight")), [(0, 1.3), (1, 2), (2, 2)]);
        assert_equal(dict(G.out_degree(weight="weight")), {0: 1.3, 1: 2, 2: 2});
        assert_equal(G.out_degree(0, weight="weight"), 1.3);
        assert_equal(sorted(G.out_degree(weight="other")), [(0, 2.2), (1, 2), (2, 2)]);
        assert_equal(dict(G.out_degree(weight="other")), {0: 2.2, 1: 2, 2: 2});
        assert_equal(G.out_degree(0, weight="other"), 2.2);
        assert_equal(list(G.out_degree(iter([0]), weight="other")), [(0, 2.2)]);

    auto test_out_degree() {
        G = this->K3
        assert_equal(sorted(G.out_degree()), [(0, 2), (1, 2), (2, 2)]);
        assert_equal(dict(G.out_degree()), {0: 2, 1: 2, 2: 2});
        assert_equal(G.out_degree(0), 2);
        assert_equal(list(G.out_degree(iter([0]))), [(0, 2)]);

    auto test_size() {
        G = this->K3
        assert_equal(G.size(), 6);
        assert_equal(G.number_of_edges(), 6);

    auto test_to_undirected_reciprocal() {
        G = this->Graph();
        G.add_edge(1, 2);
        assert_true(G.to_undirected().has_edge(1, 2));
        assert_false(G.to_undirected(reciprocal=true).has_edge(1, 2));
        G.add_edge(2, 1);
        assert_true(G.to_undirected(reciprocal=true).has_edge(1, 2));

    auto test_reverse_copy() {
        G = xn::DiGraph([(0, 1), (1, 2)]);
        R = G.reverse();
        assert_equal(sorted(R.edges()), [(1, 0), (2, 1)]);
        R.remove_edge(1, 0);
        assert_equal(sorted(R.edges()), [(2, 1)]);
        assert_equal(sorted(G.edges()), [(0, 1), (1, 2)]);

    auto test_reverse_nocopy() {
        G = xn::DiGraph([(0, 1), (1, 2)]);
        R = G.reverse(copy=false);
        assert_equal(sorted(R.edges()), [(1, 0), (2, 1)]);
        assert_raises(xn::XNetworkError, R.remove_edge, 1, 0);

    auto test_reverse_hashable() {
        class Foo: public object {
            // pass;
        x = Foo();
        y = Foo();
        G = xn::DiGraph();
        G.add_edge(x, y);
        assert_nodes_equal(G.nodes(), G.reverse().nodes());
        assert_equal([(y, x)], list(G.reverse().edges()));


class BaseAttrDiGraphTester(BaseDiGraphTester, BaseAttrGraphTester) {
    // pass;


class TestDiGraph(BaseAttrDiGraphTester, TestGraph) {
    /** Tests specific to dict-of-dict-of-dict digraph data structure */

    auto setUp() {
        this->Graph = xn::DiGraph
        // build dict-of-dict-of-dict K3
        ed1, ed2, ed3, ed4, ed5, ed6 = ({}, {}, {}, {}, {}, {});
        this->k3adj = {0: {1: ed1, 2: ed2}, 1: {0: ed3, 2: ed4}, 2: {0: ed5, 1: ed6}}
        this->k3edges = [(0, 1), (0, 2), (1, 2)];
        this->k3nodes = [0, 1, 2];
        this->K3 = this->Graph();
        this->K3._adj = this->K3._succ = this->k3adj
        this->K3._pred = {0: {1: ed3, 2: ed5}, 1: {0: ed1, 2: ed6}, 2: {0: ed2, 1: ed4}}
        this->K3._node = {};
        this->K3._node[0] = {};
        this->K3._node[1] = {};
        this->K3._node[2] = {};

        ed1, ed2 = ({}, {});
        this->P3 = this->Graph();
        this->P3._adj = {0: {1: ed1}, 1: {2: ed2}, 2: {}}
        this->P3._succ = this->P3._adj
        this->P3._pred = {0: {}, 1: {0: ed1}, 2: {1: ed2}}
        this->P3._node = {};
        this->P3._node[0] = {};
        this->P3._node[1] = {};
        this->P3._node[2] = {};

    auto test_data_input() {
        G = this->Graph({1: [2], 2: [1]}, name="test");
        assert_equal(G.name, "test");
        assert_equal(sorted(G.adj.items()), [(1, {2: {}}), (2, {1: {}})]);
        assert_equal(sorted(G.succ.items()), [(1, {2: {}}), (2, {1: {}})]);
        assert_equal(sorted(G.pred.items()), [(1, {2: {}}), (2, {1: {}})]);

    auto test_add_edge() {
        G = this->Graph();
        G.add_edge(0, 1);
        assert_equal(G.adj, {0: {1: {}}, 1: {}});
        assert_equal(G.succ, {0: {1: {}}, 1: {}});
        assert_equal(G.pred, {0: {}, 1: {0: {}}});
        G = this->Graph();
        G.add_edge(*(0, 1));
        assert_equal(G.adj, {0: {1: {}}, 1: {}});
        assert_equal(G.succ, {0: {1: {}}, 1: {}});
        assert_equal(G.pred, {0: {}, 1: {0: {}}});

    auto test_add_edges_from() {
        G = this->Graph();
        G.add_edges_from([(0, 1), (0, 2, {"data": 3})], data=2);
        assert_equal(G.adj, {0: {1: {"data": 2}, 2: {"data": 3}}, 1: {}, 2: {}});
        assert_equal(G.succ, {0: {1: {"data": 2}, 2: {"data": 3}}, 1: {}, 2: {}});
        assert_equal(G.pred, {0: {}, 1: {0: {"data": 2}}, 2: {0: {"data": 3}}});

        assert_raises(xn::XNetworkError, G.add_edges_from, [(0,)]);  // too few : tuple
        assert_raises(xn::XNetworkError, G.add_edges_from, [(0, 1, 2, 3)]);  // too many : tuple
        assert_raises(TypeError, G.add_edges_from, [0]);  // not a tuple

    auto test_remove_edge() {
        G = this->K3
        G.remove_edge(0, 1);
        assert_equal(G.succ, {0: {2: {}}, 1: {0: {}, 2: {}}, 2: {0: {}, 1: {}}});
        assert_equal(G.pred, {0: {1: {}, 2: {}}, 1: {2: {}}, 2: {0: {}, 1: {}}});
        assert_raises((KeyError, xn::XNetworkError), G.remove_edge, -1, 0);

    auto test_remove_edges_from() {
        G = this->K3
        G.remove_edges_from([(0, 1)]);
        assert_equal(G.succ, {0: {2: {}}, 1: {0: {}, 2: {}}, 2: {0: {}, 1: {}}});
        assert_equal(G.pred, {0: {1: {}, 2: {}}, 1: {2: {}}, 2: {0: {}, 1: {}}});
        G.remove_edges_from([(0, 0)]);  // silent fail


class TestEdgeSubgraph(TestGraphEdgeSubgraph) {
    /** Unit tests for the :meth:`DiGraph.edge_subgraph` method. */

    auto setup() {
        // Create a doubly-linked path graph on five nodes.
        G = xn::DiGraph(xn::path_graph(5));
        // Add some node, edge, && graph attributes.
        for (auto i : range(5) {
            G.nodes[i]["name"] = "node{}".format(i);
        G.edges[0, 1]["name"] = "edge01";
        G.edges[3, 4]["name"] = "edge34";
        G.graph["name"] = "graph";
        // Get the subgraph induced by the first && last edges.
        this->G = G;
        this->H = G.edge_subgraph([(0, 1), (3, 4)]);

    auto test_pred_succ() {
        /** Test that nodes are added to predecessors && successors.

        For more information, see GitHub issue #2370.

         */
        G = xn::DiGraph();
        G.add_edge(0, 1);
        H = G.edge_subgraph([(0, 1)]);
        assert_equal(list(H.predecessors(0)), []);
        assert_equal(list(H.successors(0)), [1]);
        assert_equal(list(H.predecessors(1)), [0]);
        assert_equal(list(H.successors(1)), []);
