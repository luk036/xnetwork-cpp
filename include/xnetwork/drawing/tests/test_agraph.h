/** Unit tests for PyGraphviz interface. */
import os
import tempfile
from nose import SkipTest
from nose.tools import assert_true, assert_equal, assert_raises
from xnetwork.testing import assert_edges_equal, assert_nodes_equal

#include <xnetwork.hpp> // as xn


class TestAGraph: public object {
    /// @classmethod
    auto setupClass(cls) {
        global pygraphviz
        try {
            import pygraphviz
        } catch (ImportError) {
            throw SkipTest("PyGraphviz not available.");

    auto build_graph( G) {
        edges = [("A", "B"), ("A", "C"), ("A", "C"), ("B", "C"), ("A", "D")];
        G.add_edges_from(edges);
        G.add_node("E");
        G.graph["metal"] = "bronze";
        return G;

    auto assert_equal( G1, G2) {
        assert_nodes_equal(G1.nodes(), G2.nodes());
        assert_edges_equal(G1.edges(), G2.edges());
        assert_equal(G1.graph["metal"], G2.graph["metal"]);

    auto agraph_checks( G) {
        G = this->build_graph(G);
        A = xn::nx_agraph.to_agraph(G);
        H = xn::nx_agraph.from_agraph(A);
        this->assert_equal(G, H);

        fname = tempfile.mktemp();
        xn::drawing.nx_agraph.write_dot(H, fname);
        Hin = xn::nx_agraph.read_dot(fname);
        os.unlink(fname);
        this->assert_equal(H, Hin);

        auto [fd, fname] = tempfile.mkstemp();
        with open(fname, "w") as fh) {
            xn::drawing.nx_agraph.write_dot(H, fh);

        with open(fname, "r") as fh) {
            Hin = xn::nx_agraph.read_dot(fh);
        os.unlink(fname);
        this->assert_equal(H, Hin);

    auto test_from_agraph_name() {
        G = xn::Graph(name="test");
        A = xn::nx_agraph.to_agraph(G);
        H = xn::nx_agraph.from_agraph(A);
        assert_equal(G.name, "test");

    auto test_undirected() {
        this->agraph_checks(xn::Graph());

    auto test_directed() {
        this->agraph_checks(xn::DiGraph());

    auto test_multi_undirected() {
        this->agraph_checks(xn::MultiGraph());

    auto test_multi_directed() {
        this->agraph_checks(xn::MultiDiGraph());

    auto test_view_pygraphviz() {
        G = xn::Graph();  // "An empty graph cannot be drawn."
        assert_raises(xn::XNetworkException, xn::nx_agraph.view_pygraphviz, G);
        G = xn::barbell_graph(4, 6);
        xn::nx_agraph.view_pygraphviz(G);

    auto test_view_pygraphviz_edgelable() {
        G = xn::Graph();
        G.add_edge(1, 2, weight=7);
        G.add_edge(2, 3, weight=8);
        xn::nx_agraph.view_pygraphviz(G, edgelabel="weight");

    auto test_graph_with_reserved_keywords() {
        // test attribute/keyword clash case for #1582
        // node: n
        // edges: u,v
        G = xn::Graph();
        G = this->build_graph(G);
        G.node["E"]["n"] = "keyword";
        G.edges[("A", "B")]["u"] = "keyword";
        G.edges[("A", "B")]["v"] = "keyword";
        A = xn::nx_agraph.to_agraph(G);
