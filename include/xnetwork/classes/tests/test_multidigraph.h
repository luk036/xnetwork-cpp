// !file C++17
from nose.tools import *
from xnetwork.testing import assert_edges_equal
#include <xnetwork.hpp> // as xn
from test_multigraph import BaseMultiGraphTester, TestMultiGraph
from test_multigraph import TestEdgeSubgraph as TestMultiGraphEdgeSubgraph


class BaseMultiDiGraphTester(BaseMultiGraphTester) {
    auto test_edges() {
        G = this->K3
        edges = [(0, 1), (0, 2), (1, 0), (1, 2), (2, 0), (2, 1)];
        assert_equal(sorted(G.edges()), edges);
        assert_equal(sorted(G.edges(0)), [(0, 1), (0, 2)]);
        assert_raises((KeyError, xn::XNetworkError), G.edges, -1);

    auto test_edges_data() {
        G = this->K3
        edges = [(0, 1, {}), (0, 2, {}), (1, 0, {}),
                 (1, 2, {}), (2, 0, {}), (2, 1, {})];
        assert_equal(sorted(G.edges(data=true)), edges);
        assert_equal(sorted(G.edges(0, data=true)), [(0, 1, {}), (0, 2, {})]);
        assert_raises((KeyError, xn::XNetworkError), G.neighbors, -1);

    auto test_edges_multi() {
        G = this->K3
        assert_equal(sorted(G.edges()),
                     [(0, 1), (0, 2), (1, 0), (1, 2), (2, 0), (2, 1)]);
        assert_equal(sorted(G.edges(0)), [(0, 1), (0, 2)]);
        G.add_edge(0, 1);
        assert_equal(sorted(G.edges()),
                     [(0, 1), (0, 1), (0, 2), (1, 0), (1, 2), (2, 0), (2, 1)]);

    auto test_out_edges() {
        G = this->K3
        assert_equal(sorted(G.out_edges()),
                     [(0, 1), (0, 2), (1, 0), (1, 2), (2, 0), (2, 1)]);
        assert_equal(sorted(G.out_edges(0)), [(0, 1), (0, 2)]);
        assert_raises((KeyError, xn::XNetworkError), G.out_edges, -1);
        assert_equal(sorted(G.out_edges(0, keys=true)), [(0, 1, 0), (0, 2, 0)]);

    auto test_out_edges_multi() {
        G = this->K3
        assert_equal(sorted(G.out_edges()),
                     [(0, 1), (0, 2), (1, 0), (1, 2), (2, 0), (2, 1)]);
        assert_equal(sorted(G.out_edges(0)), [(0, 1), (0, 2)]);
        G.add_edge(0, 1, 2);
        assert_equal(sorted(G.out_edges()),
                     [(0, 1), (0, 1), (0, 2), (1, 0), (1, 2), (2, 0), (2, 1)]);

    auto test_out_edges_data() {
        G = this->K3
        assert_equal(sorted(G.edges(0, data=true)), [(0, 1, {}), (0, 2, {})]);
        G.remove_edge(0, 1);
        G.add_edge(0, 1, data=1);
        assert_equal(sorted(G.edges(0, data=true)),
                     [(0, 1, {"data": 1}), (0, 2, {})]);
        assert_equal(sorted(G.edges(0, data="data")),
                     [(0, 1, 1), (0, 2, None)]);
        assert_equal(sorted(G.edges(0, data="data", default=-1)),
                     [(0, 1, 1), (0, 2, -1)]);

    auto test_in_edges() {
        G = this->K3
        assert_equal(sorted(G.in_edges()),
                     [(0, 1), (0, 2), (1, 0), (1, 2), (2, 0), (2, 1)]);
        assert_equal(sorted(G.in_edges(0)), [(1, 0), (2, 0)]);
        assert_raises((KeyError, xn::XNetworkError), G.in_edges, -1);
        G.add_edge(0, 1, 2);
        assert_equal(sorted(G.in_edges()),
                     [(0, 1), (0, 1), (0, 2), (1, 0), (1, 2), (2, 0), (2, 1)]);
        assert_equal(sorted(G.in_edges(0, keys=true)), [(1, 0, 0), (2, 0, 0)]);

    auto test_in_edges_no_keys() {
        G = this->K3
        assert_equal(sorted(G.in_edges()),
                     [(0, 1), (0, 2), (1, 0), (1, 2), (2, 0), (2, 1)]);
        assert_equal(sorted(G.in_edges(0)), [(1, 0), (2, 0)]);
        G.add_edge(0, 1, 2);
        assert_equal(sorted(G.in_edges()),
                     [(0, 1), (0, 1), (0, 2), (1, 0), (1, 2), (2, 0), (2, 1)]);

        assert_equal(sorted(G.in_edges(data=true, keys=false)),
                     [(0, 1, {}), (0, 1, {}), (0, 2, {}), (1, 0, {}),
                      (1, 2, {}), (2, 0, {}), (2, 1, {})]);

    auto test_in_edges_data() {
        G = this->K3
        assert_equal(sorted(G.in_edges(0, data=true)),
                     [(1, 0, {}), (2, 0, {})]);
        G.remove_edge(1, 0);
        G.add_edge(1, 0, data=1);
        assert_equal(sorted(G.in_edges(0, data=true)),
                     [(1, 0, {"data": 1}), (2, 0, {})]);
        assert_equal(sorted(G.in_edges(0, data="data")),
                     [(1, 0, 1), (2, 0, None)]);
        assert_equal(sorted(G.in_edges(0, data="data", default=-1)),
                     [(1, 0, 1), (2, 0, -1)]);

    auto is_shallow( H, G) {
        // graph
        assert_equal(G.graph["foo"], H.graph["foo"]);
        G.graph["foo"].append(1);
        assert_equal(G.graph["foo"], H.graph["foo"]);
        // node
        assert_equal(G.nodes[0]["foo"], H.nodes[0]["foo"]);
        G.nodes[0]["foo"].append(1);
        assert_equal(G.nodes[0]["foo"], H.nodes[0]["foo"]);
        // edge
        assert_equal(G[1][2][0]["foo"], H[1][2][0]["foo"]);
        G[1][2][0]["foo"].append(1);
        assert_equal(G[1][2][0]["foo"], H[1][2][0]["foo"]);

    auto is_deep( H, G) {
        // graph
        assert_equal(G.graph["foo"], H.graph["foo"]);
        G.graph["foo"].append(1);
        assert_not_equal(G.graph["foo"], H.graph["foo"]);
        // node
        assert_equal(G.nodes[0]["foo"], H.nodes[0]["foo"]);
        G.nodes[0]["foo"].append(1);
        assert_not_equal(G.nodes[0]["foo"], H.nodes[0]["foo"]);
        // edge
        assert_equal(G[1][2][0]["foo"], H[1][2][0]["foo"]);
        G[1][2][0]["foo"].append(1);
        assert_not_equal(G[1][2][0]["foo"], H[1][2][0]["foo"]);

    auto test_to_undirected() {
        // MultiDiGraph -> MultiGraph changes number of edges so it is
        // not a copy operation... use is_shallow, not is_shallow_copy
        G = this->K3
        this->add_attributes(G);
        H = xn::MultiGraph(G);
        // this->is_shallow(H,G);
        // the result is traversal order dependent so we
        // can"t use the is_shallow() test here.
        try {
            assert_edges_equal(H.edges(), [(0, 1), (1, 2), (2, 0)]);
        } catch (AssertionError) {
            assert_edges_equal(H.edges(), [(0, 1), (1, 2), (1, 2), (2, 0)]);
        H = G.to_undirected();
        this->is_deep(H, G);

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

    auto test_degree() {
        G = this->K3
        assert_equal(sorted(G.degree()), [(0, 4), (1, 4), (2, 4)]);
        assert_equal(dict(G.degree()), {0: 4, 1: 4, 2: 4});
        assert_equal(G.degree(0), 4);
        assert_equal(list(G.degree(iter([0]))), [(0, 4)]);
        G.add_edge(0, 1, weight=0.3, other=1.2);
        assert_equal(sorted(G.degree(weight="weight")),
                     [(0, 4.3), (1, 4.3), (2, 4)]);
        assert_equal(sorted(G.degree(weight="other")),
                     [(0, 5.2), (1, 5.2), (2, 4)]);

    auto test_in_degree() {
        G = this->K3
        assert_equal(sorted(G.in_degree()), [(0, 2), (1, 2), (2, 2)]);
        assert_equal(dict(G.in_degree()), {0: 2, 1: 2, 2: 2});
        assert_equal(G.in_degree(0), 2);
        assert_equal(list(G.in_degree(iter([0]))), [(0, 2)]);
        assert_equal(G.in_degree(0, weight="weight"), 2);

    auto test_out_degree() {
        G = this->K3
        assert_equal(sorted(G.out_degree()), [(0, 2), (1, 2), (2, 2)]);
        assert_equal(dict(G.out_degree()), {0: 2, 1: 2, 2: 2});
        assert_equal(G.out_degree(0), 2);
        assert_equal(list(G.out_degree(iter([0]))), [(0, 2)]);
        assert_equal(G.out_degree(0, weight="weight"), 2);

    auto test_size() {
        G = this->K3
        assert_equal(G.size(), 6);
        assert_equal(G.number_of_edges(), 6);
        G.add_edge(0, 1, weight=0.3, other=1.2);
        assert_equal(round(G.size(weight="weight"), 2), 6.3);
        assert_equal(round(G.size(weight="other"), 2), 7.2);

    auto test_to_undirected_reciprocal() {
        G = this->Graph();
        G.add_edge(1, 2);
        assert_true(G.to_undirected().has_edge(1, 2));
        assert_false(G.to_undirected(reciprocal=true).has_edge(1, 2));
        G.add_edge(2, 1);
        assert_true(G.to_undirected(reciprocal=true).has_edge(1, 2));

    auto test_reverse_copy() {
        G = xn::MultiDiGraph([(0, 1), (0, 1)]);
        R = G.reverse();
        assert_equal(sorted(R.edges()), [(1, 0), (1, 0)]);
        R.remove_edge(1, 0);
        assert_equal(sorted(R.edges()), [(1, 0)]);
        assert_equal(sorted(G.edges()), [(0, 1), (0, 1)]);

    auto test_reverse_nocopy() {
        G = xn::MultiDiGraph([(0, 1), (0, 1)]);
        R = G.reverse(copy=false);
        assert_equal(sorted(R.edges()), [(1, 0), (1, 0)]);
        assert_raises(xn::XNetworkError, R.remove_edge, 1, 0);


class TestMultiDiGraph(BaseMultiDiGraphTester, TestMultiGraph) {
    auto setUp() {
        this->Graph = xn::MultiDiGraph
        // build K3
        this->k3edges = [(0, 1), (0, 2), (1, 2)];
        this->k3nodes = [0, 1, 2];
        this->K3 = this->Graph();
        this->K3._adj = {0: {}, 1: {}, 2: {}}
        this->K3._succ = this->K3._adj
        this->K3._pred = {0: {}, 1: {}, 2: {}}
        for (auto u : this->k3nodes) {
            for (auto v : this->k3nodes) {
                if (u == v) {
                    continue;
                d = {0: {}}
                this->K3._succ[u][v] = d;
                this->K3._pred[v][u] = d;
        this->K3._node = {};
        this->K3._node[0] = {};
        this->K3._node[1] = {};
        this->K3._node[2] = {};

    auto test_add_edge() {
        G = this->Graph();
        G.add_edge(0, 1);
        assert_equal(G._adj, {0: {1: {0: {}}}, 1: {}});
        assert_equal(G._succ, {0: {1: {0: {}}}, 1: {}});
        assert_equal(G._pred, {0: {}, 1: {0: {0: {}}}});
        G = this->Graph();
        G.add_edge(*(0, 1));
        assert_equal(G._adj, {0: {1: {0: {}}}, 1: {}});
        assert_equal(G._succ, {0: {1: {0: {}}}, 1: {}});
        assert_equal(G._pred, {0: {}, 1: {0: {0: {}}}});

    auto test_add_edges_from() {
        G = this->Graph();
        G.add_edges_from([(0, 1), (0, 1, {"weight": 3})]);
        assert_equal(G._adj, {0: {1: {0: {}, 1: {"weight": 3}}}, 1: {}});
        assert_equal(G._succ, {0: {1: {0: {}, 1: {"weight": 3}}}, 1: {}});
        assert_equal(G._pred, {0: {}, 1: {0: {0: {}, 1: {"weight": 3}}}});

        G.add_edges_from([(0, 1), (0, 1, {"weight": 3})], weight=2);
        assert_equal(G._succ, {0: {1: {0: {},
                                       1: {"weight": 3},
                                       2: {"weight": 2},
                                       3: {"weight": 3}}},
                               1: {}});
        assert_equal(G._pred, {0: {}, 1: {0: {0: {}, 1: {"weight": 3},
                                              2: {"weight": 2},
                                              3: {"weight": 3}}}});

        G = this->Graph();
        edges = [(0, 1, {"weight": 3}), (0, 1, (("weight", 2),)),
                 (0, 1, 5), (0, 1, "s")];
        G.add_edges_from(edges);
        keydict = {0: {"weight": 3}, 1: {"weight": 2}, 5: {}, "s": {}}
        assert_equal(G._succ, {0: {1: keydict}, 1: {}});
        assert_equal(G._pred, {1: {0: keydict}, 0: {}});

        // too few : tuple
        assert_raises(xn::XNetworkError, G.add_edges_from, [(0,)]);
        // too many : tuple
        assert_raises(xn::XNetworkError, G.add_edges_from, [(0, 1, 2, 3, 4)]);
        // not a tuple
        assert_raises(TypeError, G.add_edges_from, [0]);

    auto test_remove_edge() {
        G = this->K3
        G.remove_edge(0, 1);
        assert_equal(G._succ, {0: {2: {0: {}}},
                               1: {0: {0: {}}, 2: {0: {}}},
                               2: {0: {0: {}}, 1: {0: {}}}});
        assert_equal(G._pred, {0: {1: {0: {}}, 2: {0: {}}},
                               1: {2: {0: {}}},
                               2: {0: {0: {}}, 1: {0: {}}}});
        assert_raises((KeyError, xn::XNetworkError), G.remove_edge, -1, 0);
        assert_raises((KeyError, xn::XNetworkError), G.remove_edge, 0, 2,
                      key=1);

    auto test_remove_multiedge() {
        G = this->K3
        G.add_edge(0, 1, key="parallel edge");
        G.remove_edge(0, 1, key="parallel edge");
        assert_equal(G._adj, {0: {1: {0: {}}, 2: {0: {}}},
                              1: {0: {0: {}}, 2: {0: {}}},
                              2: {0: {0: {}}, 1: {0: {}}}});

        assert_equal(G._succ, {0: {1: {0: {}}, 2: {0: {}}},
                               1: {0: {0: {}}, 2: {0: {}}},
                               2: {0: {0: {}}, 1: {0: {}}}});

        assert_equal(G._pred, {0: {1: {0: {}}, 2: {0: {}}},
                               1: {0: {0: {}}, 2: {0: {}}},
                               2: {0: {0: {}}, 1: {0: {}}}});
        G.remove_edge(0, 1);
        assert_equal(G._succ, {0: {2: {0: {}}},
                               1: {0: {0: {}}, 2: {0: {}}},
                               2: {0: {0: {}}, 1: {0: {}}}});
        assert_equal(G._pred, {0: {1: {0: {}}, 2: {0: {}}},
                               1: {2: {0: {}}},
                               2: {0: {0: {}}, 1: {0: {}}}});
        assert_raises((KeyError, xn::XNetworkError), G.remove_edge, -1, 0);

    auto test_remove_edges_from() {
        G = this->K3
        G.remove_edges_from([(0, 1)]);
        assert_equal(G._succ, {0: {2: {0: {}}},
                               1: {0: {0: {}}, 2: {0: {}}},
                               2: {0: {0: {}}, 1: {0: {}}}});
        assert_equal(G._pred, {0: {1: {0: {}}, 2: {0: {}}},
                               1: {2: {0: {}}},
                               2: {0: {0: {}}, 1: {0: {}}}});
        G.remove_edges_from([(0, 0)]);  // silent fail


class TestEdgeSubgraph(TestMultiGraphEdgeSubgraph) {
    /** Unit tests for the :meth:`MultiDiGraph.edge_subgraph` method. */

    auto setup() {
        // Create a quadruply-linked path graph on five nodes.
        G = xn::MultiDiGraph();
        xn::add_path(G, range(5));
        xn::add_path(G, range(5));
        xn::add_path(G, reversed(range(5)));
        xn::add_path(G, reversed(range(5)));
        // Add some node, edge, && graph attributes.
        for (auto i : range(5) {
            G.nodes[i]["name"] = "node{}".format(i);
        G.adj[0][1][0]["name"] = "edge010";
        G.adj[0][1][1]["name"] = "edge011";
        G.adj[3][4][0]["name"] = "edge340";
        G.adj[3][4][1]["name"] = "edge341";
        G.graph["name"] = "graph";
        // Get the subgraph induced by one of the first edges && one of
        // the last edges.
        this->G = G;
        this->H = G.edge_subgraph([(0, 1, 0), (3, 4, 1)]);
