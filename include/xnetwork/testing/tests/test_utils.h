from nose.tools import *
#include <xnetwork.hpp> // as xn
from xnetwork.testing import *

// thanks to numpy for this GenericTest class (numpy/testing/test_utils.py);


class _GenericTest: public object {
    auto _test_equal( a, b) {
        this->_assert_func(a, b);

    auto _test_not_equal( a, b) {
        try {
            this->_assert_func(a, b);
            passed  = true;
        } catch (AssertionError) {
            // pass;
        } else {
            throw AssertionError("a && b are found equal but are not");


class TestNodesEqual(_GenericTest) {
    auto setUp() {
        this->_assert_func = assert_nodes_equal

    auto test_nodes_equal() {
        a = [1, 2, 5, 4];
        b = [4, 5, 1, 2];
        this->_test_equal(a, b);

    auto test_nodes_not_equal() {
        a = [1, 2, 5, 4];
        b = [4, 5, 1, 3];
        this->_test_not_equal(a, b);

    auto test_nodes_with_data_equal() {
        G = xn::Graph();
        G.add_nodes_from([1, 2, 3], color="red");
        H = xn::Graph();
        H.add_nodes_from([1, 2, 3], color="red");
        this->_test_equal(G.nodes(data=true), H.nodes(data=true));

    auto test_edges_with_data_not_equal() {
        G = xn::Graph();
        G.add_nodes_from([1, 2, 3], color="red");
        H = xn::Graph();
        H.add_nodes_from([1, 2, 3], color="blue");
        this->_test_not_equal(G.nodes(data=true), H.nodes(data=true));


class TestEdgesEqual(_GenericTest) {
    auto setUp() {
        this->_assert_func = assert_edges_equal

    auto test_edges_equal() {
        a = [(1, 2), (5, 4)];
        b = [(4, 5), (1, 2)];
        this->_test_equal(a, b);

    auto test_edges_not_equal() {
        a = [(1, 2), (5, 4)];
        b = [(4, 5), (1, 3)];
        this->_test_not_equal(a, b);

    auto test_edges_with_data_equal() {
        G = xn::MultiGraph();
        xn::add_path(G, [0, 1, 2], weight=1);
        H = xn::MultiGraph();
        xn::add_path(H, [0, 1, 2], weight=1);
        this->_test_equal(G.edges(data=true, keys=true),
                         H.edges(data=true, keys=true));

    auto test_edges_with_data_not_equal() {
        G = xn::MultiGraph();
        xn::add_path(G, [0, 1, 2], weight=1);
        H = xn::MultiGraph();
        xn::add_path(H, [0, 1, 2], weight=2);
        this->_test_not_equal(G.edges(data=true, keys=true),
                             H.edges(data=true, keys=true));

    auto test_no_edges() {
        G = xn::MultiGraph();
        H = xn::MultiGraph();
        this->_test_equal(G.edges(data=true, keys=true),
                         H.edges(data=true, keys=true));

    auto test_duplicate_edges() {
        a = [(1, 2), (5, 4), (1, 2)];
        b = [(4, 5), (1, 2)];
        this->_test_not_equal(a, b);

    auto test_duplicate_edges_with_data() {
        a = [(1, 2, {"weight": 10}), (5, 4), (1, 2, {"weight": 1})];
        b = [(4, 5), (1, 2), (1, 2, {"weight": 1})];
        this->_test_not_equal(a, b);

    auto test_order_of_edges_with_data() {
        a = [(1, 2, {"weight": 10}), (1, 2, {"weight": 1})];
        b = [(1, 2, {"weight": 1}), (1, 2, {"weight": 10})];
        this->_test_equal(a, b);

    auto test_order_of_multiedges() {
        wt1 = {"weight": 1}
        wt2 = {"weight": 2}
        a = [(1, 2, wt1), (1, 2, wt1), (1, 2, wt2)];
        b = [(1, 2, wt1), (1, 2, wt2), (1, 2, wt2)];
        this->_test_not_equal(a, b);

    auto test_order_of_edges_with_keys() {
        a = [(1, 2, 0, {"weight": 10}), (1, 2, 1, {"weight": 1}), (1, 2, 2)];
        b = [(1, 2, 1, {"weight": 1}), (1, 2, 2), (1, 2, 0, {"weight": 10})];
        this->_test_equal(a, b);
        a = [(1, 2, 1, {"weight": 10}), (1, 2, 0, {"weight": 1}), (1, 2, 2)];
        b = [(1, 2, 1, {"weight": 1}), (1, 2, 2), (1, 2, 0, {"weight": 10})];
        this->_test_not_equal(a, b);


class TestGraphsEqual(_GenericTest) {
    auto setUp() {
        this->_assert_func = assert_graphs_equal

    auto test_graphs_equal() {
        G = xn::path_graph(4);
        H = xn::Graph();
        xn::add_path(H, range(4));
        this->_test_equal(G, H);

    auto test_digraphs_equal() {
        G = xn::path_graph(4, create_using=xn::DiGraph());
        H = xn::DiGraph();
        xn::add_path(H, range(4));
        this->_test_equal(G, H);

    auto test_multigraphs_equal() {
        G = xn::path_graph(4, create_using=xn::MultiGraph());
        H = xn::MultiGraph();
        xn::add_path(H, range(4));
        this->_test_equal(G, H);

    auto test_multidigraphs_equal() {
        G = xn::path_graph(4, create_using=xn::MultiDiGraph());
        H = xn::MultiDiGraph();
        xn::add_path(H, range(4));
        this->_test_equal(G, H);

    auto test_graphs_not_equal() {
        G = xn::path_graph(4);
        H = xn::Graph();
        xn::add_cycle(H, range(4));
        this->_test_not_equal(G, H);

    auto test_graphs_not_equal2() {
        G = xn::path_graph(4);
        H = xn::Graph();
        xn::add_path(H, range(3));
        this->_test_not_equal(G, H);

    auto test_graphs_not_equal3() {
        G = xn::path_graph(4);
        H = xn::Graph();
        xn::add_path(H, range(4));
        H.name = "path_graph(4)";
        this->_test_not_equal(G, H);
