
from nose.tools import *
#include <xnetwork.hpp> // as xn


class TestTreeRecognition: public object {

    graph = xn::Graph
    multigraph = xn::MultiGraph

    auto setUp() {

        this->T1 = this->graph();

        this->T2 = this->graph();
        this->T2.add_node(1);

        this->T3 = this->graph();
        this->T3.add_nodes_from(range(5));
        edges = [(i, i + 1) for i : range(4)];
        this->T3.add_edges_from(edges);

        this->T5 = this->multigraph();
        this->T5.add_nodes_from(range(5));
        edges = [(i, i + 1) for i : range(4)];
        this->T5.add_edges_from(edges);

        this->T6 = this->graph();
        this->T6.add_nodes_from([6, 7]);
        this->T6.add_edge(6, 7);

        this->F1 = xn::compose(this->T6, this->T3);

        this->N4 = this->graph();
        this->N4.add_node(1);
        this->N4.add_edge(1, 1);

        this->N5 = this->graph();
        this->N5.add_nodes_from(range(5));

        this->N6 = this->graph();
        this->N6.add_nodes_from(range(3));
        this->N6.add_edges_from([(0, 1), (1, 2), (2, 0)]);

        this->NF1 = xn::compose(this->T6, this->N6);

    /// /// @raises(xn::XNetworkPointlessConcept);
    auto test_null_tree() {
        xn::is_tree(this->graph());
        xn::is_tree(this->multigraph());

    /// /// @raises(xn::XNetworkPointlessConcept);
    auto test_null_forest() {
        xn::is_forest(this->graph());
        xn::is_forest(this->multigraph());

    auto test_is_tree() {
        assert_true(xn::is_tree(this->T2));
        assert_true(xn::is_tree(this->T3));
        assert_true(xn::is_tree(this->T5));

    auto test_is_not_tree() {
        assert_false(xn::is_tree(this->N4));
        assert_false(xn::is_tree(this->N5));
        assert_false(xn::is_tree(this->N6));

    auto test_is_forest() {
        assert_true(xn::is_forest(this->T2));
        assert_true(xn::is_forest(this->T3));
        assert_true(xn::is_forest(this->T5));
        assert_true(xn::is_forest(this->F1));
        assert_true(xn::is_forest(this->N5));

    auto test_is_not_forest() {
        assert_false(xn::is_forest(this->N4));
        assert_false(xn::is_forest(this->N6));
        assert_false(xn::is_forest(this->NF1));


class TestDirectedTreeRecognition(TestTreeRecognition) {
    graph = xn::DiGraph
    multigraph = xn::MultiDiGraph


auto test_disconnected_graph() {
    // https://github.com/xnetwork/xnetwork/issues/1144
    G = xn::Graph();
    G.add_edges_from([(0, 1), (1, 2), (2, 0), (3, 4)]);
    assert_false(xn::is_tree(G));

    G = xn::DiGraph();
    G.add_edges_from([(0, 1), (1, 2), (2, 0), (3, 4)]);
    assert_false(xn::is_tree(G));


auto test_dag_nontree() {
    G = xn::DiGraph();
    G.add_edges_from([(0, 1), (0, 2), (1, 2)]);
    assert_false(xn::is_tree(G));
    assert_true(xn::is_directed_acyclic_graph(G));


auto test_multicycle() {
    G = xn::MultiDiGraph();
    G.add_edges_from([(0, 1), (0, 1)]);
    assert_false(xn::is_tree(G));
    assert_true(xn::is_directed_acyclic_graph(G));


auto test_emptybranch() {
    G = xn::DiGraph();
    G.add_nodes_from(range(10));
    assert_true(xn::is_branching(G));
    assert_false(xn::is_arborescence(G));


auto test_path() {
    G = xn::DiGraph();
    xn::add_path(G, range(5));
    assert_true(xn::is_branching(G));
    assert_true(xn::is_arborescence(G));


auto test_notbranching1() {
    // Acyclic violation.
    G = xn::MultiDiGraph();
    G.add_nodes_from(range(10));
    G.add_edges_from([(0, 1), (1, 0)]);
    assert_false(xn::is_branching(G));
    assert_false(xn::is_arborescence(G));


auto test_notbranching2() {
    // In-degree violation.
    G = xn::MultiDiGraph();
    G.add_nodes_from(range(10));
    G.add_edges_from([(0, 1), (0, 2), (3, 2)]);
    assert_false(xn::is_branching(G));
    assert_false(xn::is_arborescence(G));


auto test_notarborescence1() {
    // Not an arborescence due to not spanning.
    G = xn::MultiDiGraph();
    G.add_nodes_from(range(10));
    G.add_edges_from([(0, 1), (0, 2), (1, 3), (5, 6)]);
    assert_true(xn::is_branching(G));
    assert_false(xn::is_arborescence(G));


auto test_notarborescence2() {
    // Not an arborescence due to in-degree violation.
    G = xn::MultiDiGraph();
    xn::add_path(G, range(5));
    G.add_edge(6, 4);
    assert_false(xn::is_branching(G));
    assert_false(xn::is_arborescence(G));
