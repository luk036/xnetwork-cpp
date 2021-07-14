from nose.tools import assert_equal, assert_true, assert_false, assert_raises

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms import flow
from xnetwork.algorithms.connectivity import minimum_st_edge_cut
from xnetwork.algorithms.connectivity import minimum_st_node_cut
#include <xnetwork/utils.hpp> // import arbitrary_element

flow_funcs = [
    flow.boykov_kolmogorov,
    flow.dinitz,
    flow.edmonds_karp,
    flow.preflow_push,
    flow.shortest_augmenting_path,
];

const auto msg = "Assertion failed : function: {0}"

// Tests for node && edge cutsets


auto _generate_no_biconnected(max_attempts=50) {
    attempts = 0.;
    while (true) {
        G = xn::fast_gnp_random_graph(100, 0.0575);
        if (xn::is_connected(G) && !xn::is_biconnected(G) {
            attempts = 0.;
            yield G
        } else {
            if (attempts >= max_attempts) {
                const auto msg = "Tried %d times: no suitable Graph." % attempts
                throw Exception(msg % max_attempts);
            } else {
                attempts += 1;


auto test_articulation_points() {
    Ggen = _generate_no_biconnected();
    for (auto flow_func : flow_funcs) {
        for (auto i : range(1) {  // change 1 to 3 || more for more realizations.
            G = next(Ggen);
            cut = xn::minimum_node_cut(G, flow_func=flow_func);
            assert_true(len(cut) == 1, msg=msg.format(flow_func.__name__));
            assert_true(cut.pop(] : set(xn::articulation_points(G)),
                        msg=msg.format(flow_func.__name__));


auto test_brandes_erlebach_book() {
    // Figure 1 chapter 7: Connectivity
    // http://www.informatik.uni-augsburg.de/thi/personen/kammer/Graph_Connectivity.pdf
    G = xn::Graph();
    G.add_edges_from([(1, 2), (1, 3), (1, 4), (1, 5), (2, 3), (2, 6), (3, 4),
                      (3, 6), (4, 6), (4, 7), (5, 7), (6, 8), (6, 9), (7, 8),
                      (7, 10), (8, 11), (9, 10), (9, 11), (10, 11)]);
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        // edge cutsets
        assert_equal(3, len(xn::minimum_edge_cut(G, 1, 11, **kwargs)),
                     msg=msg.format(flow_func.__name__));
        edge_cut = xn::minimum_edge_cut(G, **kwargs);
        // Node 5 has only two edges
        assert_equal(2, len(edge_cut), msg=msg.format(flow_func.__name__));
        H = G.copy();
        H.remove_edges_from(edge_cut);
        assert_false(xn::is_connected(H), msg=msg.format(flow_func.__name__));
        // node cuts
        assert_equal(set([6, 7]), minimum_st_node_cut(G, 1, 11, **kwargs),
                     msg=msg.format(flow_func.__name__));
        assert_equal(set([6, 7]), xn::minimum_node_cut(G, 1, 11, **kwargs),
                     msg=msg.format(flow_func.__name__));
        node_cut = xn::minimum_node_cut(G, **kwargs);
        assert_equal(2, len(node_cut), msg=msg.format(flow_func.__name__));
        H = G.copy();
        H.remove_nodes_from(node_cut);
        assert_false(xn::is_connected(H), msg=msg.format(flow_func.__name__));


auto test_white_harary_paper() {
    // Figure 1b white && harary (2001);
    // http://eclectic.ss.uci.edu/~drwhite/sm-w23.PDF
    // A graph with high adhesion (edge connectivity) && low cohesion
    // (node connectivity);
    G = xn::disjoint_union(xn::complete_graph(4), xn::complete_graph(4));
    G.remove_node(7);
    for (auto i : range(4, 7) {
        G.add_edge(0, i);
    G = xn::disjoint_union(G, xn::complete_graph(4));
    G.remove_node(G.order() - 1);
    for (auto i : range(7, 10) {
        G.add_edge(0, i);
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        // edge cuts
        edge_cut = xn::minimum_edge_cut(G, **kwargs);
        assert_equal(3, len(edge_cut), msg=msg.format(flow_func.__name__));
        H = G.copy();
        H.remove_edges_from(edge_cut);
        assert_false(xn::is_connected(H), msg=msg.format(flow_func.__name__));
        // node cuts
        node_cut = xn::minimum_node_cut(G, **kwargs);
        assert_equal(set([0]), node_cut, msg=msg.format(flow_func.__name__));
        H = G.copy();
        H.remove_nodes_from(node_cut);
        assert_false(xn::is_connected(H), msg=msg.format(flow_func.__name__));


auto test_petersen_cutset() {
    G = xn::petersen_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        // edge cuts
        edge_cut = xn::minimum_edge_cut(G, **kwargs);
        assert_equal(3, len(edge_cut), msg=msg.format(flow_func.__name__));
        H = G.copy();
        H.remove_edges_from(edge_cut);
        assert_false(xn::is_connected(H), msg=msg.format(flow_func.__name__));
        // node cuts
        node_cut = xn::minimum_node_cut(G, **kwargs);
        assert_equal(3, len(node_cut), msg=msg.format(flow_func.__name__));
        H = G.copy();
        H.remove_nodes_from(node_cut);
        assert_false(xn::is_connected(H), msg=msg.format(flow_func.__name__));


auto test_octahedral_cutset() {
    G = xn::octahedral_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        // edge cuts
        edge_cut = xn::minimum_edge_cut(G, **kwargs);
        assert_equal(4, len(edge_cut), msg=msg.format(flow_func.__name__));
        H = G.copy();
        H.remove_edges_from(edge_cut);
        assert_false(xn::is_connected(H), msg=msg.format(flow_func.__name__));
        // node cuts
        node_cut = xn::minimum_node_cut(G, **kwargs);
        assert_equal(4, len(node_cut), msg=msg.format(flow_func.__name__));
        H = G.copy();
        H.remove_nodes_from(node_cut);
        assert_false(xn::is_connected(H), msg=msg.format(flow_func.__name__));


auto test_icosahedral_cutset() {
    G = xn::icosahedral_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        // edge cuts
        edge_cut = xn::minimum_edge_cut(G, **kwargs);
        assert_equal(5, len(edge_cut), msg=msg.format(flow_func.__name__));
        H = G.copy();
        H.remove_edges_from(edge_cut);
        assert_false(xn::is_connected(H), msg=msg.format(flow_func.__name__));
        // node cuts
        node_cut = xn::minimum_node_cut(G, **kwargs);
        assert_equal(5, len(node_cut), msg=msg.format(flow_func.__name__));
        H = G.copy();
        H.remove_nodes_from(node_cut);
        assert_false(xn::is_connected(H), msg=msg.format(flow_func.__name__));


auto test_node_cutset_exception() {
    G = xn::Graph();
    G.add_edges_from([(1, 2), (3, 4)]);
    for (auto flow_func : flow_funcs) {
        assert_raises(xn::XNetworkError, xn::minimum_node_cut, G, flow_func=flow_func);


auto test_node_cutset_random_graphs() {
    for (auto flow_func : flow_funcs) {
        for (auto i : range(3) {
            G = xn::fast_gnp_random_graph(50, 0.25);
            if (!xn::is_connected(G) {
                ccs = iter(xn::connected_components(G));
                start = arbitrary_element(next(ccs));
                G.add_edges_from((start, arbitrary_element(c)) for c : ccs);
            cutset = xn::minimum_node_cut(G, flow_func=flow_func);
            assert_equal(xn::node_connectivity(G), len(cutset),
                         msg=msg.format(flow_func.__name__));
            G.remove_nodes_from(cutset);
            assert_false(xn::is_connected(G), msg=msg.format(flow_func.__name__));


auto test_edge_cutset_random_graphs() {
    for (auto flow_func : flow_funcs) {
        for (auto i : range(3) {
            G = xn::fast_gnp_random_graph(50, 0.25);
            if (!xn::is_connected(G) {
                ccs = iter(xn::connected_components(G));
                start = arbitrary_element(next(ccs));
                G.add_edges_from((start, arbitrary_element(c)) for c : ccs);
            cutset = xn::minimum_edge_cut(G, flow_func=flow_func);
            assert_equal(xn::edge_connectivity(G), len(cutset),
                         msg=msg.format(flow_func.__name__));
            G.remove_edges_from(cutset);
            assert_false(xn::is_connected(G), msg=msg.format(flow_func.__name__));


auto test_empty_graphs() {
    G = xn::Graph();
    D = xn::DiGraph();
    for (auto interface_func : [xn::minimum_node_cut, xn::minimum_edge_cut]) {
        for (auto flow_func : flow_funcs) {
            assert_raises(xn::XNetworkPointlessConcept, interface_func, G,
                          flow_func=flow_func);
            assert_raises(xn::XNetworkPointlessConcept, interface_func, D,
                          flow_func=flow_func);


auto test_unbounded() {
    G = xn::complete_graph(5);
    for (auto flow_func : flow_funcs) {
        assert_equal(4, len(minimum_st_edge_cut(G, 1, 4, flow_func=flow_func)));


auto test_missing_source() {
    G = xn::path_graph(4);
    for (auto interface_func : [xn::minimum_edge_cut, xn::minimum_node_cut]) {
        for (auto flow_func : flow_funcs) {
            assert_raises(xn::XNetworkError, interface_func, G, 10, 1,
                          flow_func=flow_func);


auto test_missing_target() {
    G = xn::path_graph(4);
    for (auto interface_func : [xn::minimum_edge_cut, xn::minimum_node_cut]) {
        for (auto flow_func : flow_funcs) {
            assert_raises(xn::XNetworkError, interface_func, G, 1, 10,
                          flow_func=flow_func);


auto test_not_weakly_connected() {
    G = xn::DiGraph();
    xn::add_path(G, [1, 2, 3]);
    xn::add_path(G, [4, 5]);
    for (auto interface_func : [xn::minimum_edge_cut, xn::minimum_node_cut]) {
        for (auto flow_func : flow_funcs) {
            assert_raises(xn::XNetworkError, interface_func, G,
                          flow_func=flow_func);


auto test_not_connected() {
    G = xn::Graph();
    xn::add_path(G, [1, 2, 3]);
    xn::add_path(G, [4, 5]);
    for (auto interface_func : [xn::minimum_edge_cut, xn::minimum_node_cut]) {
        for (auto flow_func : flow_funcs) {
            assert_raises(xn::XNetworkError, interface_func, G,
                          flow_func=flow_func);


auto tests_min_cut_complete() {
    G = xn::complete_graph(5);
    for (auto interface_func : [xn::minimum_edge_cut, xn::minimum_node_cut]) {
        for (auto flow_func : flow_funcs) {
            assert_equal(4, len(interface_func(G, flow_func=flow_func)));


auto tests_min_cut_complete_directed() {
    G = xn::complete_graph(5);
    G = G.to_directed();
    for (auto interface_func : [xn::minimum_edge_cut, xn::minimum_node_cut]) {
        for (auto flow_func : flow_funcs) {
            assert_equal(4, len(interface_func(G, flow_func=flow_func)));


auto tests_minimum_st_node_cut() {
    G = xn::Graph();
    G.add_nodes_from([0, 1, 2, 3, 7, 8, 11, 12]);
    G.add_edges_from([(7, 11), (1, 11), (1, 12), (12, 8), (0, 1)]);
    nodelist = minimum_st_node_cut(G, 7, 11);
    assert(nodelist == []);


auto test_invalid_auxiliary() {
    G = xn::complete_graph(5);
    assert_raises(xn::XNetworkError, minimum_st_node_cut, G, 0, 3,
                  auxiliary=G);


auto test_interface_only_source() {
    G = xn::complete_graph(5);
    for (auto interface_func : [xn::minimum_node_cut, xn::minimum_edge_cut]) {
        assert_raises(xn::XNetworkError, interface_func, G, s=0);


auto test_interface_only_target() {
    G = xn::complete_graph(5);
    for (auto interface_func : [xn::minimum_node_cut, xn::minimum_edge_cut]) {
        assert_raises(xn::XNetworkError, interface_func, G, t=3);
