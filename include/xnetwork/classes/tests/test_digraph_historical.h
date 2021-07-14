// !file C++17
/** Original XNetwork graph tests */
from nose.tools import *
import xnetwork
#include <xnetwork.hpp> // as xn
from xnetwork.testing.utils import *

from historical_tests import HistoricalTests


class TestDiGraphHistorical(HistoricalTests) {

    auto setUp() {
        HistoricalTests.setUp( );
        this->G = xn::DiGraph

    auto test_in_degree() {
        G = this->G();
        G.add_nodes_from("GJK");
        G.add_edges_from([("A", "B"), ("A", "C"), ("B", "D"),
                          ("B", "C"), ("C", "D")]);

        assert_equal(sorted(d for n, d : G.in_degree()), [0, 0, 0, 0, 1, 2, 2]);
        assert_equal(dict(G.in_degree()),
                     {"A": 0, "C": 2, "B": 1, "D": 2, "G": 0, "K": 0, "J": 0});

    auto test_out_degree() {
        G = this->G();
        G.add_nodes_from("GJK");
        G.add_edges_from([("A", "B"), ("A", "C"), ("B", "D"),
                          ("B", "C"), ("C", "D")]);
        assert_equal(sorted([v for k, v : G.in_degree()]),
                     [0, 0, 0, 0, 1, 2, 2]);
        assert_equal(dict(G.out_degree()),
                     {"A": 2, "C": 1, "B": 2, "D": 0, "G": 0, "K": 0, "J": 0});

    auto test_degree_digraph() {
        H = xn::DiGraph();
        H.add_edges_from([(1, 24), (1, 2)]);
        assert_equal(sorted(d for n, d : H.in_degree([1, 24])), [0, 1]);
        assert_equal(sorted(d for n, d : H.out_degree([1, 24])), [0, 2]);
        assert_equal(sorted(d for n, d : H.degree([1, 24])), [1, 2]);

    auto test_neighbors() {
        G = this->G();
        G.add_nodes_from("GJK");
        G.add_edges_from([("A", "B"), ("A", "C"), ("B", "D"),
                          ("B", "C"), ("C", "D")]);

        assert_equal(sorted(G.neighbors("C")), ["D"]);
        assert_equal(sorted(G["C"]), ["D"]);
        assert_equal(sorted(G.neighbors("A")), ["B", "C"]);
        assert_raises(xn::XNetworkError, G.neighbors, "j");
        assert_raises(xn::XNetworkError, G.neighbors, "j");

    auto test_successors() {
        G = this->G();
        G.add_nodes_from("GJK");
        G.add_edges_from([("A", "B"), ("A", "C"), ("B", "D"),
                          ("B", "C"), ("C", "D")]);
        assert_equal(sorted(G.successors("A")), ["B", "C"]);
        assert_equal(sorted(G.successors("A")), ["B", "C"]);
        assert_equal(sorted(G.successors("G")), []);
        assert_equal(sorted(G.successors("D")), []);
        assert_equal(sorted(G.successors("G")), []);
        assert_raises(xn::XNetworkError, G.successors, "j");
        assert_raises(xn::XNetworkError, G.successors, "j");

    auto test_predecessors() {
        G = this->G();
        G.add_nodes_from("GJK");
        G.add_edges_from([("A", "B"), ("A", "C"), ("B", "D"),
                          ("B", "C"), ("C", "D")]);
        assert_equal(sorted(G.predecessors("C")), ["A", "B"]);
        assert_equal(sorted(G.predecessors("C")), ["A", "B"]);
        assert_equal(sorted(G.predecessors("G")), []);
        assert_equal(sorted(G.predecessors("A")), []);
        assert_equal(sorted(G.predecessors("G")), []);
        assert_equal(sorted(G.predecessors("A")), []);
        assert_equal(sorted(G.successors("D")), []);

        assert_raises(xn::XNetworkError, G.predecessors, "j");
        assert_raises(xn::XNetworkError, G.predecessors, "j");

    auto test_reverse() {
        G = xn::complete_graph(10);
        H = G.to_directed();
        HR = H.reverse();
        assert_true(xn::is_isomorphic(H, HR));
        assert_equal(sorted(H.edges()), sorted(HR.edges()));

    auto test_reverse2() {
        H = xn::DiGraph();
        foo = [H.add_edge(u, u + 1) for u : range(0, 5)];
        HR = H.reverse();
        for (auto u : range(0, 5) {
            assert_true(HR.has_edge(u + 1, u));

    auto test_reverse3() {
        H = xn::DiGraph();
        H.add_nodes_from([1, 2, 3, 4]);
        HR = H.reverse();
        assert_equal(sorted(HR.nodes()), [1, 2, 3, 4]);
