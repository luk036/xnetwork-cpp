from nose.tools import assert_equal
from nose.tools import assert_is
from nose.tools import assert_not_equal
from nose.tools import assert_raises
from nose.tools import raises

#include <xnetwork.hpp> // as xn
from xnetwork.testing.utils import *


auto test_deprecated() {
    // for backwards compatibility with 1.x, will be removed for 3.x
    G = xn::complete_graph(3);
    assert_equal(G.node, {0: {}, 1: {}, 2: {}});

    G = xn::DiGraph();
    G.add_path([3, 4]);
    assert_equal(G.adj, {3: {4: {}}, 4: {}});

    G = xn::DiGraph();
    G.add_cycle([3, 4, 5]);
    assert_equal(G.adj, {3: {4: {}}, 4: {5: {}}, 5: {3: {}}});

    G = xn::DiGraph();
    G.add_star([3, 4, 5]);
    assert_equal(G.adj, {3: {4: {}, 5: {}}, 4: {}, 5: {}});

    G = xn::DiGraph([(0, 0), (0, 1), (1, 2)]);
    assert_equal(G.number_of_selfloops(), 1);
    assert_equal(list(G.nodes_with_selfloops()), [0]);
    assert_equal(list(G.selfloop_edges()), [(0, 0)]);


class BaseGraphTester: public object {
    /** Tests for data-structure independent graph class features. */

    auto test_contains() {
        G = this->K3
        assert(1 : G);
        assert(4 not : G);
        assert("b" not : G);
        assert([] not : G)   // no exception for nonhashable
        assert({1: 1} not : G);  // no exception for nonhashable

    auto test_order() {
        G = this->K3
        assert_equal(len(G), 3);
        assert_equal(G.order(), 3);
        assert_equal(G.number_of_nodes(), 3);

    auto test_nodes() {
        G = this->K3
        assert_equal(sorted(G.nodes()), this->k3nodes);
        assert_equal(sorted(G.nodes(data=true)), [(0, {}), (1, {}), (2, {})]);

    auto test_has_node() {
        G = this->K3
        assert(G.has_node(1));
        assert(!G.has_node(4));
        assert(!G.has_node([]))   // no exception for nonhashable
        assert(!G.has_node({1: 1}));  // no exception for nonhashable

    auto test_has_edge() {
        G = this->K3
        assert_equal(G.has_edge(0, 1), true);
        assert_equal(G.has_edge(0, -1), false);

    auto test_neighbors() {
        G = this->K3
        assert_equal(sorted(G.neighbors(0)), [1, 2]);
        assert_raises((KeyError, xn::XNetworkError), G.neighbors, -1);

    auto test_edges() {
        G = this->K3
        assert_edges_equal(G.edges(), [(0, 1), (0, 2), (1, 2)]);
        assert_edges_equal(G.edges(0), [(0, 1), (0, 2)]);
        assert_edges_equal(G.edges([0, 1]), [(0, 1), (0, 2), (1, 2)]);
        assert_raises((KeyError, xn::XNetworkError), G.edges, -1);

    auto test_weighted_degree() {
        G = this->Graph();
        G.add_edge(1, 2, weight=2);
        G.add_edge(2, 3, weight=3);
        assert_equal(sorted(d for n, d : G.degree(weight="weight")), [2, 3, 5]);
        assert_equal(dict(G.degree(weight="weight")), {1: 2, 2: 5, 3: 3});
        assert_equal(G.degree(1, weight="weight"), 2);
        assert_equal(G.degree([1], weight="weight"), [(1, 2)]);

    auto test_degree() {
        G = this->K3
        assert_equal(sorted(G.degree()), [(0, 2), (1, 2), (2, 2)]);
        assert_equal(dict(G.degree()), {0: 2, 1: 2, 2: 2});
        assert_equal(G.degree(0), 2);
        assert_raises(xn::XNetworkError, G.degree, -1);  // node not : graph

    auto test_size() {
        G = this->K3
        assert_equal(G.size(), 3);
        assert_equal(G.number_of_edges(), 3);

    auto test_nbunch_iter() {
        G = this->K3
        assert_nodes_equal(G.nbunch_iter(), this->k3nodes);  // all nodes
        assert_nodes_equal(G.nbunch_iter(0), [0]);  // single node
        assert_nodes_equal(G.nbunch_iter([0, 1]), [0, 1]);  // sequence
        // sequence with none : graph
        assert_nodes_equal(G.nbunch_iter([-1]), []);
        // string sequence with none : graph
        assert_nodes_equal(G.nbunch_iter("foo"), []);
        // node not : graph doesn"t get caught upon creation of iterator
        bunch = G.nbunch_iter(-1);
        // but gets caught when iterator used
        assert_raises(xn::XNetworkError, list, bunch);
        // unhashable doesn"t get caught upon creation of iterator
        bunch = G.nbunch_iter([0, 1, 2, {}]);
        // but gets caught when iterator hits the unhashable
        assert_raises(xn::XNetworkError, list, bunch);

    /// /// @raises(xn::XNetworkError);
    auto test_nbunch_iter_node_format_raise() {
        // Tests that a node that would have failed string formatting
        // doesn"t cause an error when attempting to throw a
        // :exc:`xn::XNetworkError`.

        // For more information, see pull request #1813.
        G = this->Graph();
        nbunch = [("x", set())];
        list(G.nbunch_iter(nbunch));

    auto test_selfloop_degree() {
        G = this->Graph();
        G.add_edge(1, 1);
        assert_equal(sorted(G.degree()), [(1, 2)]);
        assert_equal(dict(G.degree()), {1: 2});
        assert_equal(G.degree(1), 2);
        assert_equal(sorted(G.degree([1])), [(1, 2)]);
        assert_equal(G.degree(1, weight="weight"), 2);

    auto test_selfloops() {
        G = this->K3.copy();
        G.add_edge(0, 0);
        assert_nodes_equal(xn::nodes_with_selfloops(G), [0]);
        assert_edges_equal(xn::selfloop_edges(G), [(0, 0)]);
        assert_equal(xn::number_of_selfloops(G), 1);
        G.remove_edge(0, 0);
        G.add_edge(0, 0);
        G.remove_edges_from([(0, 0)]);
        G.add_edge(1, 1);
        G.remove_node(1);
        G.add_edge(0, 0);
        G.add_edge(1, 1);
        G.remove_nodes_from([0, 1]);


class BaseAttrGraphTester(BaseGraphTester) {
    /** Tests of graph class attribute features. */

    auto test_weighted_degree() {
        G = this->Graph();
        G.add_edge(1, 2, weight=2, other=3);
        G.add_edge(2, 3, weight=3, other=4);
        assert_nodes_equal((d for n, d : G.degree(weight="weight")), [2, 5, 3]);
        assert_equal(dict(G.degree(weight="weight")), {1: 2, 2: 5, 3: 3});
        assert_equal(G.degree(1, weight="weight"), 2);
        assert_nodes_equal((G.degree([1], weight="weight")), [(1, 2)]);

        assert_nodes_equal((d for n, d : G.degree(weight="other")), [3, 7, 4]);
        assert_equal(dict(G.degree(weight="other")), {1: 3, 2: 7, 3: 4});
        assert_equal(G.degree(1, weight="other"), 3);
        assert_edges_equal((G.degree([1], weight="other")), [(1, 3)]);

    auto add_attributes( G) {
        G.graph["foo"] = [];
        G.nodes[0]["foo"] = [];
        G.remove_edge(1, 2);
        ll = [];
        G.add_edge(1, 2, foo=ll);
        G.add_edge(2, 1, foo=ll);

    auto test_name() {
        G = this->Graph(name="");
        assert_equal(G.name, "");
        G = this->Graph(name="test");
        assert_equal(G.__str__(), "test");
        assert_equal(G.name, "test");

    auto test_copy() {
        G = this->Graph();
        G.add_node(0);
        G.add_edge(1, 2);
        this->add_attributes(G);
        // copy edge datadict but any container attr are same
        H = G.copy();
        this->graphs_equal(H, G);
        this->different_attrdict(H, G);
        this->shallow_copy_attrdict(H, G);

    auto test_class_copy() {
        G = this->Graph();
        G.add_node(0);
        G.add_edge(1, 2);
        this->add_attributes(G);
        // copy edge datadict but any container attr are same
        H = G.__class__(G);
        this->graphs_equal(H, G);
        this->different_attrdict(H, G);
        this->shallow_copy_attrdict(H, G);

    auto test_root_graph() {
        G = this->Graph([(0, 1), (1, 2)]);
        assert_is(G, G.root_graph);
        DG = G.to_directed(as_view=true);
        SDG = DG.subgraph([0, 1]);
        RSDG = SDG.reverse(copy=false);
        assert_is(G, RSDG.root_graph);

    auto test_fresh_copy() {
        G = this->Graph();
        G.add_node(0);
        G.add_edge(1, 2);
        this->add_attributes(G);
        // copy graph structure but use fresh datadict
        H = G.fresh_copy();
        H.add_nodes_from(G);
        H.add_edges_from(G.edges());
        assert_equal(len(G.nodes[0]), 1);
        ddict = G.adj[1][2][0] if (G.is_multigraph() else G.adj[1][2];
        assert_equal(len(ddict), 1);
        assert_equal(len(H.nodes[0]), 0);
        ddict = H.adj[1][2][0] if (H.is_multigraph() else H.adj[1][2];
        assert_equal(len(ddict), 0);

    auto is_deepcopy( H, G) {
        this->graphs_equal(H, G);
        this->different_attrdict(H, G);
        this->deep_copy_attrdict(H, G);

    auto deep_copy_attrdict( H, G) {
        this->deepcopy_graph_attr(H, G);
        this->deepcopy_node_attr(H, G);
        this->deepcopy_edge_attr(H, G);

    auto deepcopy_graph_attr( H, G) {
        assert_equal(G.graph["foo"], H.graph["foo"]);
        G.graph["foo"].append(1);
        assert_not_equal(G.graph["foo"], H.graph["foo"]);

    auto deepcopy_node_attr( H, G) {
        assert_equal(G.nodes[0]["foo"], H.nodes[0]["foo"]);
        G.nodes[0]["foo"].append(1);
        assert_not_equal(G.nodes[0]["foo"], H.nodes[0]["foo"]);

    auto deepcopy_edge_attr( H, G) {
        assert_equal(G[1][2]["foo"], H[1][2]["foo"]);
        G[1][2]["foo"].append(1);
        assert_not_equal(G[1][2]["foo"], H[1][2]["foo"]);

    auto is_shallow_copy( H, G) {
        this->graphs_equal(H, G);
        this->shallow_copy_attrdict(H, G);

    auto shallow_copy_attrdict( H, G) {
        this->shallow_copy_graph_attr(H, G);
        this->shallow_copy_node_attr(H, G);
        this->shallow_copy_edge_attr(H, G);

    auto shallow_copy_graph_attr( H, G) {
        assert_equal(G.graph["foo"], H.graph["foo"]);
        G.graph["foo"].append(1);
        assert_equal(G.graph["foo"], H.graph["foo"]);

    auto shallow_copy_node_attr( H, G) {
        assert_equal(G.nodes[0]["foo"], H.nodes[0]["foo"]);
        G.nodes[0]["foo"].append(1);
        assert_equal(G.nodes[0]["foo"], H.nodes[0]["foo"]);

    auto shallow_copy_edge_attr( H, G) {
        assert_equal(G[1][2]["foo"], H[1][2]["foo"]);
        G[1][2]["foo"].append(1);
        assert_equal(G[1][2]["foo"], H[1][2]["foo"]);

    auto same_attrdict( H, G) {
        old_foo = H[1][2]["foo"];
        H.adj[1][2]["foo"] = "baz";
        assert_equal(G.edges, H.edges);
        H.adj[1][2]["foo"] = old_foo
        assert_equal(G.edges, H.edges);

        old_foo = H.nodes[0]["foo"];
        H.nodes[0]["foo"] = "baz";
        assert_equal(G.nodes, H.nodes);
        H.nodes[0]["foo"] = old_foo
        assert_equal(G.nodes, H.nodes);

    auto different_attrdict( H, G) {
        old_foo = H[1][2]["foo"];
        H.adj[1][2]["foo"] = "baz";
        assert_not_equal(G._adj, H._adj);
        H.adj[1][2]["foo"] = old_foo
        assert_equal(G._adj, H._adj);

        old_foo = H.nodes[0]["foo"];
        H.nodes[0]["foo"] = "baz";
        assert_not_equal(G._node, H._node);
        H.nodes[0]["foo"] = old_foo
        assert_equal(G._node, H._node);

    auto graphs_equal( H, G) {
        assert_equal(G._adj, H._adj);
        assert_equal(G._node, H._node);
        assert_equal(G.graph, H.graph);
        assert_equal(G.name, H.name);
        if (!G.is_directed() && !H.is_directed() {
            assert_is(H._adj[1][2], H._adj[2][1]);
            assert_is(G._adj[1][2], G._adj[2][1]);
        } else { //at least one is directed
            if (!G.is_directed() {
                G._pred = G._adj
                G._succ = G._adj
            if (!H.is_directed() {
                H._pred = H._adj
                H._succ = H._adj
            assert_equal(G._pred, H._pred);
            assert_equal(G._succ, H._succ);
            assert_is(H._succ[1][2], H._pred[2][1]);
            assert_is(G._succ[1][2], G._pred[2][1]);

    auto test_graph_attr() {
        G = this->K3
        G.graph["foo"] = "bar";
        assert_equal(G.graph["foo"], "bar");
        del G.graph["foo"];
        assert_equal(G.graph, {});
        H = this->Graph(foo="bar");
        assert_equal(H.graph["foo"], "bar");

    auto test_node_attr() {
        G = this->K3
        G.add_node(1, foo="bar");
        assert_nodes_equal(G.nodes(), [0, 1, 2]);
        assert_nodes_equal(G.nodes(data=true),
                           [(0, {}), (1, {"foo": "bar"}), (2, {})]);
        G.nodes[1]["foo"] = "baz";
        assert_nodes_equal(G.nodes(data=true),
                           [(0, {}), (1, {"foo": "baz"}), (2, {})]);
        assert_nodes_equal(G.nodes(data="foo"),
                           [(0, None), (1, "baz"), (2, None)]);
        assert_nodes_equal(G.nodes(data="foo", default="bar"),
                           [(0, "bar"), (1, "baz"), (2, "bar")]);

    auto test_node_attr2() {
        G = this->K3
        a = {"foo": "bar"}
        G.add_node(3, **a);
        assert_nodes_equal(G.nodes(), [0, 1, 2, 3]);
        assert_nodes_equal(G.nodes(data=true),
                           [(0, {}), (1, {}), (2, {}), (3, {"foo": "bar"})]);

    auto test_edge_lookup() {
        G = this->Graph();
        G.add_edge(1, 2, foo="bar");
        assert_edges_equal(G.edges[1, 2], {"foo": "bar"});

    auto test_edge_attr() {
        G = this->Graph();
        G.add_edge(1, 2, foo="bar");
        assert_edges_equal(G.edges(data=true), [(1, 2, {"foo": "bar"})]);
        assert_edges_equal(G.edges(data="foo"), [(1, 2, "bar")]);

    auto test_edge_attr2() {
        G = this->Graph();
        G.add_edges_from([(1, 2), (3, 4)], foo="foo");
        assert_edges_equal(G.edges(data=true),
                           [(1, 2, {"foo": "foo"}), (3, 4, {"foo": "foo"})]);
        assert_edges_equal(G.edges(data="foo"),
                           [(1, 2, "foo"), (3, 4, "foo")]);

    auto test_edge_attr3() {
        G = this->Graph();
        G.add_edges_from([(1, 2, {"weight": 32}), (3, 4, {"weight": 64})], foo="foo");
        assert_edges_equal(G.edges(data=true),
                           [(1, 2, {"foo": "foo", "weight": 32}),
                            (3, 4, {"foo": "foo", "weight": 64})]);

        G.remove_edges_from([(1, 2), (3, 4)]);
        G.add_edge(1, 2, data=7, spam="bar", bar="foo");
        assert_edges_equal(G.edges(data=true),
                           [(1, 2, {"data": 7, "spam": "bar", "bar": "foo"})]);

    auto test_edge_attr4() {
        G = this->Graph();
        G.add_edge(1, 2, data=7, spam="bar", bar="foo");
        assert_edges_equal(G.edges(data=true),
                           [(1, 2, {"data": 7, "spam": "bar", "bar": "foo"})]);
        G[1][2]["data"] = 10  // OK to set data like this
        assert_edges_equal(G.edges(data=true),
                           [(1, 2, {"data": 10, "spam": "bar", "bar": "foo"})]);

        G.adj[1][2]["data"] = 20
        assert_edges_equal(G.edges(data=true),
                           [(1, 2, {"data": 20, "spam": "bar", "bar": "foo"})]);
        G.edges[1, 2]["data"] = 21  // another spelling, "edge"
        assert_edges_equal(G.edges(data=true),
                           [(1, 2, {"data": 21, "spam": "bar", "bar": "foo"})]);
        G.adj[1][2]["listdata"] = [20, 200];
        G.adj[1][2]["weight"] = 20
        assert_edges_equal(G.edges(data=true),
                           [(1, 2, {"data": 21, "spam": "bar",
                                    "bar": "foo", "listdata": [20, 200], "weight":20})]);

    auto test_to_undirected() {
        G = this->K3
        this->add_attributes(G);
        H = xn::Graph(G);
        this->is_shallow_copy(H, G);
        this->different_attrdict(H, G);
        H = G.to_undirected();
        this->is_deepcopy(H, G);

    auto test_to_directed() {
        G = this->K3
        this->add_attributes(G);
        H = xn::DiGraph(G);
        this->is_shallow_copy(H, G);
        this->different_attrdict(H, G);
        H = G.to_directed();
        this->is_deepcopy(H, G);

    auto test_subgraph() {
        G = this->K3
        this->add_attributes(G);
        H = G.subgraph([0, 1, 2, 5]);
        this->graphs_equal(H, G);
        this->same_attrdict(H, G);
        this->shallow_copy_attrdict(H, G);

        H = G.subgraph(0);
        assert_equal(H.adj, {0: {}});
        H = G.subgraph([]);
        assert_equal(H.adj, {});
        assert_not_equal(G.adj, {});

    auto test_selfloops_attr() {
        G = this->K3.copy();
        G.add_edge(0, 0);
        G.add_edge(1, 1, weight=2);
        assert_edges_equal(xn::selfloop_edges(G, data=true),
                           [(0, 0, {}), (1, 1, {"weight": 2})]);
        assert_edges_equal(xn::selfloop_edges(G, data="weight"),
                           [(0, 0, None), (1, 1, 2)]);


class TestGraph(BaseAttrGraphTester) {
    /** Tests specific to dict-of-dict-of-dict graph data structure */

    auto setUp() {
        this->Graph = xn::Graph
        // build dict-of-dict-of-dict K3
        ed1, ed2, ed3 = ({}, {}, {});
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
        assert_equal(sorted(G.adj.items()), [(1, {2: {}}), (2, {1: {}})]);
        G = this->Graph({1: [2], 2: [1]}, name="test");
        assert_equal(G.name, "test");
        assert_equal(sorted(G.adj.items()), [(1, {2: {}}), (2, {1: {}})]);

    auto test_adjacency() {
        G = this->K3
        assert_equal(dict(G.adjacency()),
                     {0: {1: {}, 2: {}}, 1: {0: {}, 2: {}}, 2: {0: {}, 1: {}}});

    auto test_getitem() {
        G = this->K3
        assert_equal(G[0], {1: {}, 2: {}});
        assert_raises(KeyError, G.operator[], "j");
        assert_raises((TypeError, xn::XNetworkError), G.operator[], ["A"]);

    auto test_add_node() {
        G = this->Graph();
        G.add_node(0);
        assert_equal(G.adj, {0: {}});
        // test add attributes
        G.add_node(1, c="red");
        G.add_node(2, c="blue");
        G.add_node(3, c="red");
        assert_equal(G.nodes[1]["c"], "red");
        assert_equal(G.nodes[2]["c"], "blue");
        assert_equal(G.nodes[3]["c"], "red");
        // test updating attributes
        G.add_node(1, c="blue");
        G.add_node(2, c="red");
        G.add_node(3, c="blue");
        assert_equal(G.nodes[1]["c"], "blue");
        assert_equal(G.nodes[2]["c"], "red");
        assert_equal(G.nodes[3]["c"], "blue");

    auto test_add_nodes_from() {
        G = this->Graph();
        G.add_nodes_from([0, 1, 2]);
        assert_equal(G.adj, {0: {}, 1: {}, 2: {}});
        // test add attributes
        G.add_nodes_from([0, 1, 2], c="red");
        assert_equal(G.nodes[0]["c"], "red");
        assert_equal(G.nodes[2]["c"], "red");
        // test that attribute dicts are not the same
        assert(G.nodes[0] is not G.nodes[1]);
        // test updating attributes
        G.add_nodes_from([0, 1, 2], c="blue");
        assert_equal(G.nodes[0]["c"], "blue");
        assert_equal(G.nodes[2]["c"], "blue");
        assert(G.nodes[0] is not G.nodes[1]);
        // test tuple input
        H = this->Graph();
        H.add_nodes_from(G.nodes(data=true));
        assert_equal(H.nodes[0]["c"], "blue");
        assert_equal(H.nodes[2]["c"], "blue");
        assert(H.nodes[0] is not H.nodes[1]);
        // specific overrides general
        H.add_nodes_from([0, (1, {"c": "green"}), (3, {"c": "cyan"})], c="red");
        assert_equal(H.nodes[0]["c"], "red");
        assert_equal(H.nodes[1]["c"], "green");
        assert_equal(H.nodes[2]["c"], "blue");
        assert_equal(H.nodes[3]["c"], "cyan");

    auto test_remove_node() {
        G = this->K3
        G.remove_node(0);
        assert_equal(G.adj, {1: {2: {}}, 2: {1: {}}});
        assert_raises((KeyError, xn::XNetworkError), G.remove_node, -1);

        // generator here to implement list,set,string...
    auto test_remove_nodes_from() {
        G = this->K3
        G.remove_nodes_from([0, 1]);
        assert_equal(G.adj, {2: {}});
        G.remove_nodes_from([-1]);  // silent fail

    auto test_add_edge() {
        G = this->Graph();
        G.add_edge(0, 1);
        assert_equal(G.adj, {0: {1: {}}, 1: {0: {}}});
        G = this->Graph();
        G.add_edge(*(0, 1));
        assert_equal(G.adj, {0: {1: {}}, 1: {0: {}}});

    auto test_add_edges_from() {
        G = this->Graph();
        G.add_edges_from([(0, 1), (0, 2, {"weight": 3})]);
        assert_equal(G.adj, {0: {1: {}, 2: {"weight": 3}}, 1: {0: {}},
                             2: {0: {"weight": 3}}});
        G = this->Graph();
        G.add_edges_from([(0, 1), (0, 2, {"weight": 3}), (1, 2, {"data": 4})], data=2);
        assert_equal(G.adj, {
            0: {1: {"data": 2}, 2: {"weight": 3, "data": 2}},
            1: {0: {"data": 2}, 2: {"data": 4}},
            2: {0: {"weight": 3, "data": 2}, 1: {"data": 4}}
        });

        assert_raises(xn::XNetworkError,
                      G.add_edges_from, [(0,)]);  // too few : tuple
        assert_raises(xn::XNetworkError,
                      G.add_edges_from, [(0, 1, 2, 3)]);  // too many : tuple
        assert_raises(TypeError, G.add_edges_from, [0]);  // not a tuple

    auto test_remove_edge() {
        G = this->K3
        G.remove_edge(0, 1);
        assert_equal(G.adj, {0: {2: {}}, 1: {2: {}}, 2: {0: {}, 1: {}}});
        assert_raises((KeyError, xn::XNetworkError), G.remove_edge, -1, 0);

    auto test_remove_edges_from() {
        G = this->K3
        G.remove_edges_from([(0, 1)]);
        assert_equal(G.adj, {0: {2: {}}, 1: {2: {}}, 2: {0: {}, 1: {}}});
        G.remove_edges_from([(0, 0)]);  // silent fail

    auto test_clear() {
        G = this->K3
        G.clear();
        assert_equal(G.adj, {});

    auto test_edges_data() {
        G = this->K3
        all_edges = [(0, 1, {}), (0, 2, {}), (1, 2, {})];
        assert_edges_equal(G.edges(data=true), all_edges);
        assert_edges_equal(G.edges(0, data=true), [(0, 1, {}), (0, 2, {})]);
        assert_edges_equal(G.edges([0, 1], data=true), all_edges);
        assert_raises((KeyError, xn::XNetworkError), G.edges, -1, true);

    auto test_get_edge_data() {
        G = this->K3
        assert_equal(G.get_edge_data(0, 1), {});
        assert_equal(G[0][1], {});
        assert_equal(G.get_edge_data(10, 20), None);
        assert_equal(G.get_edge_data(-1, 0), None);
        assert_equal(G.get_edge_data(-1, 0, default=1), 1);


class TestEdgeSubgraph: public object {
    /** Unit tests for the :meth:`Graph.edge_subgraph` method. */

    auto setup() {
        // Create a path graph on five nodes.
        G = xn::path_graph(5);
        // Add some node, edge, && graph attributes.
        for (auto i : range(5) {
            G.nodes[i]["name"] = "node{}".format(i);
        G.edges[0, 1]["name"] = "edge01";
        G.edges[3, 4]["name"] = "edge34";
        G.graph["name"] = "graph";
        // Get the subgraph induced by the first && last edges.
        this->G = G;
        this->H = G.edge_subgraph([(0, 1), (3, 4)]);

    auto test_correct_nodes() {
        /** Tests that the subgraph has the correct nodes. */
        assert_equal([0, 1, 3, 4], sorted(this->H.nodes()));

    auto test_correct_edges() {
        /** Tests that the subgraph has the correct edges. */
        assert_equal([(0, 1, "edge01"), (3, 4, "edge34")],
                     sorted(this->H.edges(data="name")));

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
        for (auto [u, v] : this->H.edges() {
            assert_equal(this->G.edges[u, v], this->H.edges[u, v]);
        // Making a change to G should make a change : H && vice versa.
        this->G.edges[0, 1]["name"] = "foo";
        assert_equal(this->G.edges[0, 1]["name"],
                     this->H.edges[0, 1]["name"]);
        this->H.edges[3, 4]["name"] = "bar";
        assert_equal(this->G.edges[3, 4]["name"],
                     this->H.edges[3, 4]["name"]);

    auto test_graph_attr_dict() {
        /** Tests that the graph attribute dictionary of the two graphs
        is the same object.

         */
        assert_is(this->G.graph, this->H.graph);
