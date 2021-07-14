from nose.tools import *
#include <xnetwork.hpp> // as xn


auto test_2d_grid_graph() {
    // FC article claims 2d grid graph of size n is (3,3)-connected
    // && (5,9)-connected, but I don"t think it is (5,9)-connected
    G = xn::grid_2d_graph(8, 8, periodic=true);
    assert_true(xn::is_kl_connected(G, 3, 3));
    assert_false(xn::is_kl_connected(G, 5, 9));
    auto [H, graphOK] = xn::kl_connected_subgraph(G, 5, 9, same_as_graph=true);
    assert_false(graphOK);


auto test_small_graph() {
    G = xn::Graph();
    G.add_edge(1, 2);
    G.add_edge(1, 3);
    G.add_edge(2, 3);
    assert_true(xn::is_kl_connected(G, 2, 2));
    H = xn::kl_connected_subgraph(G, 2, 2);
    auto [H, graphOK] = xn::kl_connected_subgraph(G, 2, 2,
                                            low_memory=true,
                                            same_as_graph=true);
    assert_true(graphOK);
