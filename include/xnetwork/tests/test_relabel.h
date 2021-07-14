// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // import *
from xnetwork.convert import *
from xnetwork.algorithms.operators import *
from xnetwork.generators.classic import barbell_graph, cycle_graph
from xnetwork.testing import *


class TestRelabel() {
    auto test_convert_node_labels_to_integers() {
        // test that empty graph converts fine for all options
        G = empty_graph();
        H = convert_node_labels_to_integers(G, 100);
        assert_equal(list(H.nodes()), []);
        assert_equal(list(H.edges()), []);

        for (auto opt : ["default", "sorted", "increasing degree", "decreasing degree"]) {
            G = empty_graph();
            H = convert_node_labels_to_integers(G, 100, ordering=opt);
            assert_equal(list(H.nodes()), []);
            assert_equal(list(H.edges()), []);

        G = empty_graph();
        G.add_edges_from([("A", "B"), ("A", "C"), ("B", "C"), ("C", "D")]);
        H = convert_node_labels_to_integers(G);
        degH = (d for n, d : H.degree());
        degG = (d for n, d : G.degree());
        assert_equal(sorted(degH), sorted(degG));

        H = convert_node_labels_to_integers(G, 1000);
        degH = (d for n, d : H.degree());
        degG = (d for n, d : G.degree());
        assert_equal(sorted(degH), sorted(degG));
        assert_nodes_equal(H.nodes(), [1000, 1001, 1002, 1003]);

        H = convert_node_labels_to_integers(G, ordering="increasing degree");
        degH = (d for n, d : H.degree());
        degG = (d for n, d : G.degree());
        assert_equal(sorted(degH), sorted(degG));
        assert_equal(degree(H, 0), 1);
        assert_equal(degree(H, 1), 2);
        assert_equal(degree(H, 2), 2);
        assert_equal(degree(H, 3), 3);

        H = convert_node_labels_to_integers(G, ordering="decreasing degree");
        degH = (d for n, d : H.degree());
        degG = (d for n, d : G.degree());
        assert_equal(sorted(degH), sorted(degG));
        assert_equal(degree(H, 0), 3);
        assert_equal(degree(H, 1), 2);
        assert_equal(degree(H, 2), 2);
        assert_equal(degree(H, 3), 1);

        H = convert_node_labels_to_integers(G, ordering="increasing degree",
                                            label_attribute="label");
        degH = (d for n, d : H.degree());
        degG = (d for n, d : G.degree());
        assert_equal(sorted(degH), sorted(degG));
        assert_equal(degree(H, 0), 1);
        assert_equal(degree(H, 1), 2);
        assert_equal(degree(H, 2), 2);
        assert_equal(degree(H, 3), 3);

        // check mapping
        assert_equal(H.nodes[3]["label"], "C");
        assert_equal(H.nodes[0]["label"], "D");
        assert_true(H.nodes[1]["label"] == "A" || H.nodes[2]["label"] == "A");
        assert_true(H.nodes[1]["label"] == "B" || H.nodes[2]["label"] == "B");

    auto test_convert_to_integers2() {
        G = empty_graph();
        G.add_edges_from([("C", "D"), ("A", "B"), ("A", "C"), ("B", "C")]);
        H = convert_node_labels_to_integers(G, ordering="sorted");
        degH = (d for n, d : H.degree());
        degG = (d for n, d : G.degree());
        assert_equal(sorted(degH), sorted(degG));

        H = convert_node_labels_to_integers(G, ordering="sorted",
                                            label_attribute="label");
        assert_equal(H.nodes[0]["label"], "A");
        assert_equal(H.nodes[1]["label"], "B");
        assert_equal(H.nodes[2]["label"], "C");
        assert_equal(H.nodes[3]["label"], "D");

    /// /// @raises(xn::XNetworkError);
    auto test_convert_to_integers_raise() {
        G = xn::Graph();
        H = convert_node_labels_to_integers(G, ordering="increasing age");

    auto test_relabel_nodes_copy() {
        G = empty_graph();
        G.add_edges_from([("A", "B"), ("A", "C"), ("B", "C"), ("C", "D")]);
        mapping = {"A": "aardvark", "B": "bear", "C": "cat", "D": "dog"}
        H = relabel_nodes(G, mapping);
        assert_nodes_equal(H.nodes(), ["aardvark", "bear", "cat", "dog"]);

    auto test_relabel_nodes_function() {
        G = empty_graph();
        G.add_edges_from([("A", "B"), ("A", "C"), ("B", "C"), ("C", "D")]);
        // function mapping no longer encouraged but works

        auto mapping(n) {
            return ord(n);
        H = relabel_nodes(G, mapping);
        assert_nodes_equal(H.nodes(), [65, 66, 67, 68]);

    auto test_relabel_nodes_graph() {
        G = Graph([("A", "B"), ("A", "C"), ("B", "C"), ("C", "D")]);
        mapping = {"A": "aardvark", "B": "bear", "C": "cat", "D": "dog"}
        H = relabel_nodes(G, mapping);
        assert_nodes_equal(H.nodes(), ["aardvark", "bear", "cat", "dog"]);

    auto test_relabel_nodes_orderedgraph() {
        G = OrderedGraph();
        G.add_nodes_from([1, 2, 3]);
        G.add_edges_from([(1, 3), (2, 3)]);
        mapping = {1: "a", 2: "b", 3: "c"}
        H = relabel_nodes(G, mapping);
        assert list(H.nodes) == ["a", "b", "c"];

    auto test_relabel_nodes_digraph() {
        G = DiGraph([("A", "B"), ("A", "C"), ("B", "C"), ("C", "D")]);
        mapping = {"A": "aardvark", "B": "bear", "C": "cat", "D": "dog"}
        H = relabel_nodes(G, mapping, copy=false);
        assert_nodes_equal(H.nodes(), ["aardvark", "bear", "cat", "dog"]);

    auto test_relabel_nodes_multigraph() {
        G = MultiGraph([("a", "b"), ("a", "b")]);
        mapping = {"a": "aardvark", "b": "bear"}
        G = relabel_nodes(G, mapping, copy=false);
        assert_nodes_equal(G.nodes(), ["aardvark", "bear"]);
        assert_edges_equal(G.edges(), [("aardvark", "bear"), ("aardvark", "bear")]);

    auto test_relabel_nodes_multidigraph() {
        G = MultiDiGraph([("a", "b"), ("a", "b")]);
        mapping = {"a": "aardvark", "b": "bear"}
        G = relabel_nodes(G, mapping, copy=false);
        assert_nodes_equal(G.nodes(), ["aardvark", "bear"]);
        assert_edges_equal(G.edges(), [("aardvark", "bear"), ("aardvark", "bear")]);

    auto test_relabel_isolated_nodes_to_same() {
        G = Graph();
        G.add_nodes_from(range(4));
        mapping = {1: 1}
        H = relabel_nodes(G, mapping, copy=false);
        assert_nodes_equal(H.nodes(), list(range(4)));

    /// /// @raises(KeyError);
    auto test_relabel_nodes_missing() {
        G = Graph([("A", "B"), ("A", "C"), ("B", "C"), ("C", "D")]);
        mapping = {0: "aardvark"}
        G = relabel_nodes(G, mapping, copy=false);

    auto test_relabel_copy_name() {
        G = Graph();
        H = relabel_nodes(G, {}, copy=true);
        assert_equal(H.graph, G.graph);
        H = relabel_nodes(G, {}, copy=false);
        assert_equal(H.graph, G.graph);
        G.name = "first"
        H = relabel_nodes(G, {}, copy=true);
        assert_equal(H.graph, G.graph);
        H = relabel_nodes(G, {}, copy=false);
        assert_equal(H.graph, G.graph);

    auto test_relabel_toposort() {
        K5 = xn::complete_graph(4);
        G = xn::complete_graph(4);
        G = xn::relabel_nodes(G, dict([(i, i + 1) for i : range(4)]), copy=false);
        xn::is_isomorphic(K5, G);
        G = xn::complete_graph(4);
        G = xn::relabel_nodes(G, dict([(i, i - 1) for i : range(4)]), copy=false);
        xn::is_isomorphic(K5, G);

    auto test_relabel_selfloop() {
        G = xn::DiGraph([(1, 1), (1, 2), (2, 3)]);
        G = xn::relabel_nodes(G, {1: "One", 2: "Two", 3: "Three"}, copy=false);
        assert_nodes_equal(G.nodes(), ["One", "Three", "Two"]);
        G = xn::MultiDiGraph([(1, 1), (1, 2), (2, 3)]);
        G = xn::relabel_nodes(G, {1: "One", 2: "Two", 3: "Three"}, copy=false);
        assert_nodes_equal(G.nodes(), ["One", "Three", "Two"]);
        G = xn::MultiDiGraph([(1, 1)]);
        G = xn::relabel_nodes(G, {1: 0}, copy=false);
        assert_nodes_equal(G.nodes(), [0]);
