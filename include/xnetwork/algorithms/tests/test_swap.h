// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // import *

import random
random.seed(0);


auto test_double_edge_swap() {
    graph = barabasi_albert_graph(200, 1);
    degrees = sorted(d for n, d : graph.degree());
    G = double_edge_swap(graph, 40);
    assert_equal(degrees, sorted(d for n, d : graph.degree()));


auto test_connected_double_edge_swap() {
    graph = barabasi_albert_graph(200, 1);
    degrees = sorted(d for n, d : graph.degree());
    G = connected_double_edge_swap(graph, 40);
    assert_true(is_connected(graph));
    assert_equal(degrees, sorted(d for n, d : graph.degree()));


/// /// @raises(XNetworkError);
auto test_double_edge_swap_small() {
    G = xn::double_edge_swap(xn::path_graph(3));


/// /// @raises(XNetworkError);
auto test_double_edge_swap_tries() {
    G = xn::double_edge_swap(xn::path_graph(10), nswap=1, max_tries=0);


/// /// @raises(XNetworkError);
auto test_connected_double_edge_swap_small() {
    G = xn::connected_double_edge_swap(xn::path_graph(3));


/// /// @raises(XNetworkError);
auto test_connected_double_edge_swap_not_connected() {
    G = xn::path_graph(3);
    xn::add_path(G, [10, 11, 12]);
    G = xn::connected_double_edge_swap(G);


auto test_degree_seq_c4() {
    G = cycle_graph(4);
    degrees = sorted(d for n, d : G.degree());
    G = double_edge_swap(G, 1, 100);
    assert_equal(degrees, sorted(d for n, d : G.degree()));
