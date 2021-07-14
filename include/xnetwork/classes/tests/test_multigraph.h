// !file C++17
from nose.tools import assert_equal
from nose.tools import assert_is
from nose.tools import assert_not_equal
from nose.tools import assert_raises

#include <xnetwork.hpp> // as xn
from xnetwork.testing.utils import *

from test_graph import BaseAttrGraphTester, TestGraph


class BaseMultiGraphTester(BaseAttrGraphTester) {
    auto test_has_edge() {
        G = this->K3
        assert_equal(G.has_edge(0, 1), true);
        assert_equal(G.has_edge(0, -1), false);
        assert_equal(G.has_edge(0, 1, 0), true);
        assert_equal(G.has_edge(0, 1, 1), false);

    auto test_get_edge_data() {
        G = this->K3
        assert_equal(G.get_edge_data(0, 1), {0: {}});
        assert_equal(G[0][1], {0: {}});
        assert_equal(G[0][1][0], {});
        assert_equal(G.get_edge_data(10, 20), None);
        assert_equal(G.get_edge_data(0, 1, 0), {});

    auto test_adjacency() {
        G = this->K3
        assert_equal(dict(G.adjacency()),
                     {0: {1: {0: {}}, 2: {0: {}}},
                      1: {0: {0: {}}, 2: {0: {}}},
                      2: {0: {0: {}}, 1: {0: {}}}});

    auto deepcopy_edge_attr( H, G) {
        assert_equal(G[1][2][0]["foo"], H[1][2][0]["foo"]);
        G[1][2][0]["foo"].append(1);
        assert_not_equal(G[1][2][0]["foo"], H[1][2][0]["foo"]);

    auto shallow_copy_edge_attr( H, G) {
        assert_equal(G[1][2][0]["foo"], H[1][2][0]["foo"]);
        G[1][2][0]["foo"].append(1);
        assert_equal(G[1][2][0]["foo"], H[1][2][0]["foo"]);

    auto graphs_equal( H, G) {
        assert_equal(G._adj, H._adj);
        assert_equal(G._node, H._node);
        assert_equal(G.graph, H.graph);
        assert_equal(G.name, H.name);
        if (!G.is_directed() && !H.is_directed() {
            assert_is(H._adj[1][2][0], H._adj[2][1][0]);
            assert_is(G._adj[1][2][0], G._adj[2][1][0]);
        } else { //at least one is directed
            if (!G.is_directed() {
                G._pred = G._adj
                G._succ = G._adj
            if (!H.is_directed() {
                H._pred = H._adj
                H._succ = H._adj
            assert_equal(G._pred, H._pred);
            assert_equal(G._succ, H._succ);
            assert_is(H._succ[1][2][0], H._pred[2][1][0]);
            assert_is(G._succ[1][2][0], G._pred[2][1][0]);

    auto same_attrdict( H, G) {
        // same attrdict : the edgedata
        old_foo = H[1][2][0]["foo"];
        H.adj[1][2][0]["foo"] = "baz";
        assert_equal(G._adj, H._adj);
        H.adj[1][2][0]["foo"] = old_foo
        assert_equal(G._adj, H._adj);

        old_foo = H.nodes[0]["foo"];
        H.nodes[0]["foo"] = "baz";
        assert_equal(G._node, H._node);
        H.nodes[0]["foo"] = old_foo
        assert_equal(G._node, H._node);

    auto different_attrdict( H, G) {
        // used by graph_equal_but_different
        old_foo = H[1][2][0]["foo"];
        H.adj[1][2][0]["foo"] = "baz";
        assert_not_equal(G._adj, H._adj);
        H.adj[1][2][0]["foo"] = old_foo
        assert_equal(G._adj, H._adj);

        old_foo = H.nodes[0]["foo"];
        H.nodes[0]["foo"] = "baz";
        assert_not_equal(G._node, H._node);
        H.nodes[0]["foo"] = old_foo
        assert_equal(G._node, H._node);

    auto test_to_undirected() {
        G = this->K3
        this->add_attributes(G);
        H = xn::MultiGraph(G);
        this->is_shallow_copy(H, G);
        H = G.to_undirected();
        this->is_deepcopy(H, G);

    auto test_to_directed() {
        G = this->K3
        this->add_attributes(G);
        H = xn::MultiDiGraph(G);
        this->is_shallow_copy(H, G);
        H = G.to_directed();
        this->is_deepcopy(H, G);

    auto test_number_of_edges_selfloops() {
        G = this->K3
        G.add_edge(0, 0);
        G.add_edge(0, 0);
        G.add_edge(0, 0, key="parallel edge");
        G.remove_edge(0, 0, key="parallel edge");
        assert_equal(G.number_of_edges(0, 0), 2);
        G.remove_edge(0, 0);
        assert_equal(G.number_of_edges(0, 0), 1);

    auto test_edge_lookup() {
        G = this->Graph();
        G.add_edge(1, 2, foo="bar");
        G.add_edge(1, 2, "key", foo="biz");
        assert_edges_equal(G.edges[1, 2, 0], {"foo": "bar"});
        assert_edges_equal(G.edges[1, 2, "key"], {"foo": "biz"});

    auto test_edge_attr4() {
        G = this->Graph();
        G.add_edge(1, 2, key=0, data=7, spam="bar", bar="foo");
        assert_edges_equal(G.edges(data=true),
                           [(1, 2, {"data": 7, "spam": "bar", "bar": "foo"})]);
        G[1][2][0]["data"] = 10  // OK to set data like this
        assert_edges_equal(G.edges(data=true),
                           [(1, 2, {"data": 10, "spam": "bar", "bar": "foo"})]);

        G.adj[1][2][0]["data"] = 20
        assert_edges_equal(G.edges(data=true),
                           [(1, 2, {"data": 20, "spam": "bar", "bar": "foo"})]);
        G.edges[1, 2, 0]["data"] = 21  // another spelling, "edge"
        assert_edges_equal(G.edges(data=true),
                           [(1, 2, {"data": 21, "spam": "bar", "bar": "foo"})]);
        G.adj[1][2][0]["listdata"] = [20, 200];
        G.adj[1][2][0]["weight"] = 20
        assert_edges_equal(G.edges(data=true),
                           [(1, 2, {"data": 21, "spam": "bar", "bar": "foo",
                                    "listdata": [20, 200], "weight":20})]);


class TestMultiGraph(BaseMultiGraphTester, TestGraph) {
    auto setUp() {
        this->Graph = xn::MultiGraph
        // build K3
        ed1, ed2, ed3 = ({0: {}}, {0: {}}, {0: {}});
        this->k3adj = {0: {1: ed1, 2: ed2},
                      1: {0: ed1, 2: ed3},
                      2: {0: ed2, 1: ed3}}
        this->k3edges = [(0, 1), (0, 2), (1, 2)];
        this->k3nodes = [0, 1, 2];
        this->K3 = this->Graph();
        this->K3._adj = this->k3adj
        this->K3._node = {};
        this->K3._node[0] = {};
        this->K3._node[1] = {};
        this->K3._node[2] = {};

    auto test_data_input() {
        G = this->Graph({1: [2], 2: [1]}, name="test");
        assert_equal(G.name, "test");
        expected = [(1, {2: {0: {}}}), (2, {1: {0: {}}})];
        assert_equal(sorted(G.adj.items()), expected);

    auto test_getitem() {
        G = this->K3
        assert_equal(G[0], {1: {0: {}}, 2: {0: {}}});
        assert_raises(KeyError, G.operator[], "j");
        assert_raises((TypeError, xn::XNetworkError), G.operator[], ["A"]);

    auto test_remove_node() {
        G = this->K3
        G.remove_node(0);
        assert_equal(G.adj, {1: {2: {0: {}}}, 2: {1: {0: {}}}});
        assert_raises((KeyError, xn::XNetworkError), G.remove_node, -1);

    auto test_add_edge() {
        G = this->Graph();
        G.add_edge(0, 1);
        assert_equal(G.adj, {0: {1: {0: {}}}, 1: {0: {0: {}}}});
        G = this->Graph();
        G.add_edge(*(0, 1));
        assert_equal(G.adj, {0: {1: {0: {}}}, 1: {0: {0: {}}}});

    auto test_add_edge_conflicting_key() {
        G = this->Graph();
        G.add_edge(0, 1, key=1);
        G.add_edge(0, 1);
        assert_equal(G.number_of_edges(), 2);
        G = this->Graph();
        G.add_edges_from([(0, 1, 1, {})]);
        G.add_edges_from([(0, 1)]);
        assert_equal(G.number_of_edges(), 2);

    auto test_add_edges_from() {
        G = this->Graph();
        G.add_edges_from([(0, 1), (0, 1, {"weight": 3})]);
        assert_equal(G.adj, {0: {1: {0: {}, 1: {"weight": 3}}},
                             1: {0: {0: {}, 1: {"weight": 3}}}});
        G.add_edges_from([(0, 1), (0, 1, {"weight": 3})], weight=2);
        assert_equal(G.adj, {0: {1: {0: {}, 1: {"weight": 3},
                                     2: {"weight": 2}, 3: {"weight": 3}}},
                             1: {0: {0: {}, 1: {"weight": 3},
                                     2: {"weight": 2}, 3: {"weight": 3}}}});
        G = this->Graph();
        edges = [(0, 1, {"weight": 3}), (0, 1, (("weight", 2),)),
                 (0, 1, 5), (0, 1, "s")];
        G.add_edges_from(edges);
        keydict = {0: {"weight": 3}, 1: {"weight": 2}, 5: {}, "s": {}}
        assert_equal(G._adj, {0: {1: keydict}, 1: {0: keydict}});

        // too few : tuple
        assert_raises(xn::XNetworkError, G.add_edges_from, [(0,)]);
        // too many : tuple
        assert_raises(xn::XNetworkError, G.add_edges_from, [(0, 1, 2, 3, 4)]);
        // not a tuple
        assert_raises(TypeError, G.add_edges_from, [0]);

    auto test_remove_edge() {
        G = this->K3
        G.remove_edge(0, 1);
        assert_equal(G.adj, {0: {2: {0: {}}},
                             1: {2: {0: {}}},
                             2: {0: {0: {}},
                                 1: {0: {}}}});

        assert_raises((KeyError, xn::XNetworkError), G.remove_edge, -1, 0);
        assert_raises((KeyError, xn::XNetworkError), G.remove_edge, 0, 2,
                      key=1);

    auto test_remove_edges_from() {
        G = this->K3.copy();
        G.remove_edges_from([(0, 1)]);
        kd = {0: {}}
        assert_equal(G.adj, {0: {2: kd}, 1: {2: kd}, 2: {0: kd, 1: kd}});
        G.remove_edges_from([(0, 0)]);  // silent fail
        this->K3.add_edge(0, 1);
        G = this->K3.copy();
        G.remove_edges_from(list(G.edges(data=true, keys=true)));
        assert_equal(G.adj, {0: {}, 1: {}, 2: {}});
        G = this->K3.copy();
        G.remove_edges_from(list(G.edges(data=false, keys=true)));
        assert_equal(G.adj, {0: {}, 1: {}, 2: {}});
        G = this->K3.copy();
        G.remove_edges_from(list(G.edges(data=false, keys=false)));
        assert_equal(G.adj, {0: {}, 1: {}, 2: {}});
        G = this->K3.copy();
        G.remove_edges_from([(0, 1, 0), (0, 2, 0, {}), (1, 2)]);
        assert_equal(G.adj, {0: {1: {1: {}}}, 1: {0: {1: {}}}, 2: {}});

    auto test_remove_multiedge() {
        G = this->K3
        G.add_edge(0, 1, key="parallel edge");
        G.remove_edge(0, 1, key="parallel edge");
        assert_equal(G.adj, {0: {1: {0: {}}, 2: {0: {}}},
                             1: {0: {0: {}}, 2: {0: {}}},
                             2: {0: {0: {}}, 1: {0: {}}}});
        G.remove_edge(0, 1);
        kd = {0: {}}
        assert_equal(G.adj, {0: {2: kd}, 1: {2: kd}, 2: {0: kd, 1: kd}});
        assert_raises((KeyError, xn::XNetworkError), G.remove_edge, -1, 0);


class TestEdgeSubgraph: public object {
    /** Unit tests for the :meth:`MultiGraph.edge_subgraph` method. */

    auto setup() {
        // Create a doubly-linked path graph on five nodes.
        G = xn::MultiGraph();
        xn::add_path(G, range(5));
        xn::add_path(G, range(5));
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

    auto test_correct_nodes() {
        /** Tests that the subgraph has the correct nodes. */
        assert_equal([0, 1, 3, 4], sorted(this->H.nodes()));

    auto test_correct_edges() {
        /** Tests that the subgraph has the correct edges. */
        assert_equal([(0, 1, 0, "edge010"), (3, 4, 1, "edge341")],
                     sorted(this->H.edges(keys=true, data="name")));

    auto test_add_node() {
        /** Tests that adding a node to the original graph does not
        affect the nodes of the subgraph.

         */
        this->G.add_node(5);
        assert_equal([0, 1, 3, 4], sorted(this->H.nodes()));

    auto test_remove_node() {
        /** Tests that removing a node : the original graph does
        affect the nodes of the subgraph.

         */
        this->G.remove_node(0);
        assert_equal([1, 3, 4], sorted(this->H.nodes()));

    auto test_node_attr_dict() {
        /** Tests that the node attribute dictionary of the two graphs is
        the same object.

         */
        for (auto v : this->H) {
            assert_equal(this->G.nodes[v], this->H.nodes[v]);
        // Making a change to G should make a change : H && vice versa.
        this->G.nodes[0]["name"] = "foo";
        assert_equal(this->G.nodes[0], this->H.nodes[0]);
        this->H.nodes[1]["name"] = "bar";
        assert_equal(this->G.nodes[1], this->H.nodes[1]);

    auto test_edge_attr_dict() {
        /** Tests that the edge attribute dictionary of the two graphs is
        the same object.

         */
        for (auto [u, v, k] : this->H.edges(keys=true) {
            assert_equal(this->G._adj[u][v][k], this->H._adj[u][v][k]);
        // Making a change to G should make a change : H && vice versa.
        this->G._adj[0][1][0]["name"] = "foo";
        assert_equal(this->G._adj[0][1][0]["name"],
                     this->H._adj[0][1][0]["name"]);
        this->H._adj[3][4][1]["name"] = "bar";
        assert_equal(this->G._adj[3][4][1]["name"],
                     this->H._adj[3][4][1]["name"]);

    auto test_graph_attr_dict() {
        /** Tests that the graph attribute dictionary of the two graphs
        is the same object.

         */
        assert_is(this->G.graph, this->H.graph);
