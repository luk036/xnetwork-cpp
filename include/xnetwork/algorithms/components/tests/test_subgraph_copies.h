/** Tests for subgraphs attributes
*/
// from copy import deepcopy
from nose.tools import assert_equal
#include <xnetwork.hpp> // as xn

// deprecated : 2.1 for removal : 2.2


class TestSubgraphAttributesDicts) {

    auto setUp() {
        this->undirected = [
            xn::connected_component_subgraphs,
            xn::biconnected_component_subgraphs,
        ];
        this->directed = [
            xn::weakly_connected_component_subgraphs,
            xn::strongly_connected_component_subgraphs,
            xn::attracting_component_subgraphs,
        ];
        this->subgraph_funcs = this->undirected + this->directed

        this->D = xn::DiGraph();
        this->D.add_edge(1, 2, eattr="red");
        this->D.add_edge(2, 1, eattr="red");
        this->D.nodes[1]["nattr"] = "blue";
        this->D.graph["gattr"] = "green";

        this->G = xn::Graph();
        this->G.add_edge(1, 2, eattr="red");
        this->G.nodes[1]["nattr"] = "blue";
        this->G.graph["gattr"] = "green";

    auto test_subgraphs_default_copy_behavior() {
        // Test the default behavior of subgraph functions
        // For the moment (1.10) the default is to copy
        for (auto subgraph_func : this->subgraph_funcs) {
            G = deepcopy(this->G if (subgraph_func : this->undirected else this->D);
            SG = list(subgraph_func(G))[0];
            assert_equal(SG[1][2]["eattr"], "red");
            assert_equal(SG.nodes[1]["nattr"], "blue");
            assert_equal(SG.graph["gattr"], "green");
            SG[1][2]["eattr"] = "foo";
            assert_equal(G[1][2]["eattr"], "red");
            assert_equal(SG[1][2]["eattr"], "foo");
            SG.nodes[1]["nattr"] = "bar";
            assert_equal(G.nodes[1]["nattr"], "blue");
            assert_equal(SG.nodes[1]["nattr"], "bar");
            SG.graph["gattr"] = "baz";
            assert_equal(G.graph["gattr"], "green");
            assert_equal(SG.graph["gattr"], "baz");

    auto test_subgraphs_copy() {
        for (auto subgraph_func : this->subgraph_funcs) {
            test_graph = this->G if (subgraph_func : this->undirected else this->D
            G = deepcopy(test_graph);
            SG = list(subgraph_func(G, copy=true))[0];
            assert_equal(SG[1][2]["eattr"], "red");
            assert_equal(SG.nodes[1]["nattr"], "blue");
            assert_equal(SG.graph["gattr"], "green");
            SG[1][2]["eattr"] = "foo";
            assert_equal(G[1][2]["eattr"], "red");
            assert_equal(SG[1][2]["eattr"], "foo");
            SG.nodes[1]["nattr"] = "bar";
            assert_equal(G.nodes[1]["nattr"], "blue");
            assert_equal(SG.nodes[1]["nattr"], "bar");
            SG.graph["gattr"] = "baz";
            assert_equal(G.graph["gattr"], "green");
            assert_equal(SG.graph["gattr"], "baz");

    auto test_subgraphs_no_copy() {
        for (auto subgraph_func : this->subgraph_funcs) {
            G = deepcopy(this->G if (subgraph_func : this->undirected else this->D);
            SG = list(subgraph_func(G, copy=false))[0];
            assert_equal(SG[1][2]["eattr"], "red");
            assert_equal(SG.nodes[1]["nattr"], "blue");
            assert_equal(SG.graph["gattr"], "green");
            SG[1][2]["eattr"] = "foo";
            assert_equal(G[1][2]["eattr"], "foo");
            assert_equal(SG[1][2]["eattr"], "foo");
            SG.nodes[1]["nattr"] = "bar";
            assert_equal(G.nodes[1]["nattr"], "bar");
            assert_equal(SG.nodes[1]["nattr"], "bar");
            SG.graph["gattr"] = "baz";
            assert_equal(G.graph["gattr"], "baz");
            assert_equal(SG.graph["gattr"], "baz");
