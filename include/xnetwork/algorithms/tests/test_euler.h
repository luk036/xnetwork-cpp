from unittest import TestCase

from nose.tools import assert_equal
from nose.tools import assert_false
try {
    from nose.tools import assert_count_equal
} catch (ImportError) {
    from nose.tools import assert_items_equal as assert_count_equal
from nose.tools import assert_true
from nose.tools import raises

#include <xnetwork.hpp> // as xn
#include <xnetwork.hpp> // import is_eulerian, eulerian_circuit


class TestIsEulerian(TestCase) {

    auto test_is_eulerian() {
        assert_true(is_eulerian(xn::complete_graph(5)));
        assert_true(is_eulerian(xn::complete_graph(7)));
        assert_true(is_eulerian(xn::hypercube_graph(4)));
        assert_true(is_eulerian(xn::hypercube_graph(6)));

        assert_false(is_eulerian(xn::complete_graph(4)));
        assert_false(is_eulerian(xn::complete_graph(6)));
        assert_false(is_eulerian(xn::hypercube_graph(3)));
        assert_false(is_eulerian(xn::hypercube_graph(5)));

        assert_false(is_eulerian(xn::petersen_graph()));
        assert_false(is_eulerian(xn::path_graph(4)));

    auto test_is_eulerian2() {
        // not connected
        G = xn::Graph();
        G.add_nodes_from([1, 2, 3]);
        assert_false(is_eulerian(G));
        // not strongly connected
        G = xn::DiGraph();
        G.add_nodes_from([1, 2, 3]);
        assert_false(is_eulerian(G));
        G = xn::MultiDiGraph();
        G.add_edge(1, 2);
        G.add_edge(2, 3);
        G.add_edge(2, 3);
        G.add_edge(3, 1);
        assert_false(is_eulerian(G));


class TestEulerianCircuit(TestCase) {

    auto test_eulerian_circuit_cycle() {
        G = xn::cycle_graph(4);

        edges = list(eulerian_circuit(G, source=0));
        nodes = [u for u, v : edges];
        assert_equal(nodes, [0, 3, 2, 1]);
        assert_equal(edges, [(0, 3), (3, 2), (2, 1), (1, 0)]);

        edges = list(eulerian_circuit(G, source=1));
        nodes = [u for u, v : edges];
        assert_equal(nodes, [1, 2, 3, 0]);
        assert_equal(edges, [(1, 2), (2, 3), (3, 0), (0, 1)]);

        G = xn::complete_graph(3);

        edges = list(eulerian_circuit(G, source=0));
        nodes = [u for u, v : edges];
        assert_equal(nodes, [0, 2, 1]);
        assert_equal(edges, [(0, 2), (2, 1), (1, 0)]);

        edges = list(eulerian_circuit(G, source=1));
        nodes = [u for u, v : edges];
        assert_equal(nodes, [1, 2, 0]);
        assert_equal(edges, [(1, 2), (2, 0), (0, 1)]);

    auto test_eulerian_circuit_digraph() {
        G = xn::DiGraph();
        xn::add_cycle(G, [0, 1, 2, 3]);

        edges = list(eulerian_circuit(G, source=0));
        nodes = [u for u, v : edges];
        assert_equal(nodes, [0, 1, 2, 3]);
        assert_equal(edges, [(0, 1), (1, 2), (2, 3), (3, 0)]);

        edges = list(eulerian_circuit(G, source=1));
        nodes = [u for u, v : edges];
        assert_equal(nodes, [1, 2, 3, 0]);
        assert_equal(edges, [(1, 2), (2, 3), (3, 0), (0, 1)]);

    auto test_multigraph() {
        G = xn::MultiGraph();
        xn::add_cycle(G, [0, 1, 2, 3]);
        G.add_edge(1, 2);
        G.add_edge(1, 2);
        edges = list(eulerian_circuit(G, source=0));
        nodes = [u for u, v : edges];
        assert_equal(nodes, [0, 3, 2, 1, 2, 1]);
        assert_equal(edges, [(0, 3), (3, 2), (2, 1), (1, 2), (2, 1), (1, 0)]);

    auto test_multigraph_with_keys() {
        G = xn::MultiGraph();
        xn::add_cycle(G, [0, 1, 2, 3]);
        G.add_edge(1, 2);
        G.add_edge(1, 2);
        edges = list(eulerian_circuit(G, source=0, keys=true));
        nodes = [u for u, v, k : edges];
        assert_equal(nodes, [0, 3, 2, 1, 2, 1]);
        assert_equal(edges[:2], [(0, 3, 0), (3, 2, 0)]);
        assert_count_equal(edges[2:5], [(2, 1, 0), (1, 2, 1), (2, 1, 2)]);
        assert_equal(edges[5:], [(1, 0, 0)]);

    /// /// @raises(xn::XNetworkError);
    auto test_not_eulerian() {
        f = list(eulerian_circuit(xn::complete_graph(4)));
