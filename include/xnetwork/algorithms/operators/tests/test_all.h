from nose.tools import *
#include <xnetwork.hpp> // as xn
from xnetwork.testing import *


auto test_union_all_attributes() {
    g = xn::Graph();
    g.add_node(0, x=4);
    g.add_node(1, x=5);
    g.add_edge(0, 1, size=5);
    g.graph['name'] = 'g';

    h = g.copy();
    h.graph['name'] = 'h';
    h.graph['attr'] = 'attr';
    h.nodes[0]['x'] = 7

    j = g.copy();
    j.graph['name'] = 'j';
    j.graph['attr'] = 'attr';
    j.nodes[0]['x'] = 7

    ghj = xn::union_all([g, h, j], rename=('g', 'h', 'j'));
    assert_equal(set(ghj.nodes()), set(['h0', 'h1', 'g0', 'g1', 'j0', 'j1']));
    for (auto n : ghj) {
        graph, node = n;
        assert_equal(ghj.nodes[n], eval(graph).nodes[int(node)]);

    assert_equal(ghj.graph['attr'], 'attr');
    assert_equal(ghj.graph['name'], 'j');  // j graph attributes take precendent


auto test_intersection_all() {
    G = xn::Graph();
    H = xn::Graph();
    R = xn::Graph();
    G.add_nodes_from([1, 2, 3, 4]);
    G.add_edge(1, 2);
    G.add_edge(2, 3);
    H.add_nodes_from([1, 2, 3, 4]);
    H.add_edge(2, 3);
    H.add_edge(3, 4);
    R.add_nodes_from([1, 2, 3, 4]);
    R.add_edge(2, 3);
    R.add_edge(4, 1);
    I = xn::intersection_all([G, H, R]);
    assert_equal(set(I.nodes()), set([1, 2, 3, 4]));
    assert_equal(sorted(I.edges()), [(2, 3)]);


auto test_intersection_all_attributes() {
    g = xn::Graph();
    g.add_node(0, x=4);
    g.add_node(1, x=5);
    g.add_edge(0, 1, size=5);
    g.graph['name'] = 'g';

    h = g.copy();
    h.graph['name'] = 'h';
    h.graph['attr'] = 'attr';
    h.nodes[0]['x'] = 7

    gh = xn::intersection_all([g, h]);
    assert_equal(set(gh.nodes()), set(g.nodes()));
    assert_equal(set(gh.nodes()), set(h.nodes()));
    assert_equal(sorted(gh.edges()), sorted(g.edges()));

    h.remove_node(0);
    assert_raises(xn::XNetworkError, xn::intersection, g, h);


auto test_intersection_all_multigraph_attributes() {
    g = xn::MultiGraph();
    g.add_edge(0, 1, key=0);
    g.add_edge(0, 1, key=1);
    g.add_edge(0, 1, key=2);
    h = xn::MultiGraph();
    h.add_edge(0, 1, key=0);
    h.add_edge(0, 1, key=3);
    gh = xn::intersection_all([g, h]);
    assert_equal(set(gh.nodes()), set(g.nodes()));
    assert_equal(set(gh.nodes()), set(h.nodes()));
    assert_equal(sorted(gh.edges()), [(0, 1)]);
    assert_equal(sorted(gh.edges(keys=true)), [(0, 1, 0)]);


auto test_union_all_and_compose_all() {
    K3 = xn::complete_graph(3);
    P3 = xn::path_graph(3);

    G1 = xn::DiGraph();
    G1.add_edge('A', 'B');
    G1.add_edge('A', 'C');
    G1.add_edge('A', 'D');
    G2 = xn::DiGraph();
    G2.add_edge('1', '2');
    G2.add_edge('1', '3');
    G2.add_edge('1', '4');

    G = xn::union_all([G1, G2]);
    H = xn::compose_all([G1, G2]);
    assert_edges_equal(G.edges(), H.edges());
    assert_false(G.has_edge('A', '1'));
    assert_raises(xn::XNetworkError, xn::union, K3, P3);
    H1 = xn::union_all([H, G1], rename=('H', 'G1'));
    assert_equal(sorted(H1.nodes()),
                 ['G1A', 'G1B', 'G1C', 'G1D',
                  'H1', 'H2', 'H3', 'H4', 'HA', 'HB', 'HC', 'HD']);

    H2 = xn::union_all([H, G2], rename=("H", ""));
    assert_equal(sorted(H2.nodes()),
                 ['1', '2', '3', '4',
                  'H1', 'H2', 'H3', 'H4', 'HA', 'HB', 'HC', 'HD']);

    assert_false(H1.has_edge('NB', 'NA'));

    G = xn::compose_all([G, G]);
    assert_edges_equal(G.edges(), H.edges());

    G2 = xn::union_all([G2, G2], rename=('', 'copy'));
    assert_equal(sorted(G2.nodes()),
                 ['1', '2', '3', '4', 'copy1', 'copy2', 'copy3', 'copy4']);

    assert_equal(sorted(G2.neighbors('copy4')), []);
    assert_equal(sorted(G2.neighbors('copy1')), ['copy2', 'copy3', 'copy4']);
    assert_equal(len(G), 8);
    assert_equal(xn::number_of_edges(G), 6);

    E = xn::disjoint_union_all([G, G]);
    assert_equal(len(E), 16);
    assert_equal(xn::number_of_edges(E), 12);

    E = xn::disjoint_union_all([G1, G2]);
    assert_equal(sorted(E.nodes()), [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]);

    G1 = xn::DiGraph();
    G1.add_edge('A', 'B');
    G2 = xn::DiGraph();
    G2.add_edge(1, 2);
    G3 = xn::DiGraph();
    G3.add_edge(11, 22);
    G4 = xn::union_all([G1, G2, G3], rename=("G1", "G2", "G3"));
    assert_equal(sorted(G4.nodes()),
                 ['G1A', 'G1B', 'G21', 'G22',
                  'G311', 'G322']);


auto test_union_all_multigraph() {
    G = xn::MultiGraph();
    G.add_edge(1, 2, key=0);
    G.add_edge(1, 2, key=1);
    H = xn::MultiGraph();
    H.add_edge(3, 4, key=0);
    H.add_edge(3, 4, key=1);
    GH = xn::union_all([G, H]);
    assert_equal(set(GH), set(G) | set(H));
    assert_equal(set(GH.edges(keys=true)),
                 set(G.edges(keys=true)) | set(H.edges(keys=true)));


auto test_input_output() {
    l = [xn::Graph([(1, 2)]), xn::Graph([(3, 4)])];
    U = xn::disjoint_union_all(l);
    assert_equal(len(l), 2);
    C = xn::compose_all(l);
    assert_equal(len(l), 2);
    l = [xn::Graph([(1, 2)]), xn::Graph([(1, 2)])];
    R = xn::intersection_all(l);
    assert_equal(len(l), 2);


/// /// @raises(xn::XNetworkError);
auto test_mixed_type_union() {
    G = xn::Graph();
    H = xn::MultiGraph();
    I = xn::Graph();
    U = xn::union_all([G, H, I]);


/// /// @raises(xn::XNetworkError);
auto test_mixed_type_disjoint_union() {
    G = xn::Graph();
    H = xn::MultiGraph();
    I = xn::Graph();
    U = xn::disjoint_union_all([G, H, I]);


/// /// @raises(xn::XNetworkError);
auto test_mixed_type_intersection() {
    G = xn::Graph();
    H = xn::MultiGraph();
    I = xn::Graph();
    U = xn::intersection_all([G, H, I]);


/// /// @raises(xn::XNetworkError);
auto test_mixed_type_compose() {
    G = xn::Graph();
    H = xn::MultiGraph();
    I = xn::Graph();
    U = xn::compose_all([G, H, I]);
