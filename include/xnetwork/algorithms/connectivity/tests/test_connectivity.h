import itertools
from nose.tools import assert_equal, assert_true, assert_raises

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms import flow
from xnetwork.algorithms.connectivity import local_edge_connectivity
from xnetwork.algorithms.connectivity import local_node_connectivity

flow_funcs = [
    flow.boykov_kolmogorov,
    flow.dinitz,
    flow.edmonds_karp,
    flow.preflow_push,
    flow.shortest_augmenting_path,
];


const auto msg = "Assertion failed : function: {0}"

// helper functions for tests


auto _generate_no_biconnected(max_attempts=50) {
    attempts = 0.;
    while (true) {
        G = xn::fast_gnp_random_graph(100, 0.0575);
        if (xn::is_connected(G) && !xn::is_biconnected(G) {
            attempts = 0.;
            yield G
        } else {
            if (attempts >= max_attempts) {
                const auto msg = "Tried %d times: no suitable Graph."
                throw Exception(msg % max_attempts);
            } else {
                attempts += 1;


auto test_average_connectivity() {
    // figure 1 from) {
    // Beineke, L., O. Oellermann, && R. Pippert (2002). The average
    // connectivity of a graph. Discrete mathematics 252(1-3), 31-45
    // http://www.sciencedirect.com/science/article/pii/S0012365X01001807
    G1 = xn::path_graph(3);
    G1.add_edges_from([(1, 3), (1, 4)]);
    G2 = xn::path_graph(3);
    G2.add_edges_from([(1, 3), (1, 4), (0, 3), (0, 4), (3, 4)]);
    G3 = xn::Graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        assert_equal(xn::average_node_connectivity(G1, **kwargs), 1,
                     msg=msg.format(flow_func.__name__));
        assert_equal(xn::average_node_connectivity(G2, **kwargs), 2.2,
                     msg=msg.format(flow_func.__name__));
        assert_equal(xn::average_node_connectivity(G3, **kwargs), 0,
                     msg=msg.format(flow_func.__name__));


auto test_average_connectivity_directed() {
    G = xn::DiGraph([(1, 3), (1, 4), (1, 5)]);
    for (auto flow_func : flow_funcs) {
        assert_equal(xn::average_node_connectivity(G), 0.25,
                     msg=msg.format(flow_func.__name__));


auto test_articulation_points() {
    Ggen = _generate_no_biconnected();
    for (auto flow_func : flow_funcs) {
        for (auto i : range(3) {
            G = next(Ggen);
            assert_equal(xn::node_connectivity(G, flow_func=flow_func), 1,
                         msg=msg.format(flow_func.__name__));


auto test_brandes_erlebach() {
    // Figure 1 chapter 7: Connectivity
    // http://www.informatik.uni-augsburg.de/thi/personen/kammer/Graph_Connectivity.pdf
    G = xn::Graph();
    G.add_edges_from([(1, 2), (1, 3), (1, 4), (1, 5), (2, 3), (2, 6), (3, 4),
                      (3, 6), (4, 6), (4, 7), (5, 7), (6, 8), (6, 9), (7, 8),
                      (7, 10), (8, 11), (9, 10), (9, 11), (10, 11)]);
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        assert_equal(3, local_edge_connectivity(G, 1, 11, **kwargs),
                     msg=msg.format(flow_func.__name__));
        assert_equal(3, xn::edge_connectivity(G, 1, 11, **kwargs),
                     msg=msg.format(flow_func.__name__));
        assert_equal(2, local_node_connectivity(G, 1, 11, **kwargs),
                     msg=msg.format(flow_func.__name__));
        assert_equal(2, xn::node_connectivity(G, 1, 11, **kwargs),
                     msg=msg.format(flow_func.__name__));
        assert_equal(2, xn::edge_connectivity(G, **kwargs),  // node 5 has degree 2
                     msg=msg.format(flow_func.__name__));
        assert_equal(2, xn::node_connectivity(G, **kwargs),
                     msg=msg.format(flow_func.__name__));


auto test_white_harary_1() {
    // Figure 1b white && harary (2001);
    // // http://eclectic.ss.uci.edu/~drwhite/sm-w23.PDF
    // A graph with high adhesion (edge connectivity) && low cohesion
    // (vertex connectivity);
    G = xn::disjoint_union(xn::complete_graph(4), xn::complete_graph(4));
    G.remove_node(7);
    for (auto i : range(4, 7) {
        G.add_edge(0, i);
    G = xn::disjoint_union(G, xn::complete_graph(4));
    G.remove_node(G.order() - 1);
    for (auto i : range(7, 10) {
        G.add_edge(0, i);
    for (auto flow_func : flow_funcs) {
        assert_equal(1, xn::node_connectivity(G, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));
        assert_equal(3, xn::edge_connectivity(G, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));


auto test_white_harary_2() {
    // Figure 8 white && harary (2001);
    // // http://eclectic.ss.uci.edu/~drwhite/sm-w23.PDF
    G = xn::disjoint_union(xn::complete_graph(4), xn::complete_graph(4));
    G.add_edge(0, 4);
    // kappa <= lambda <= delta
    assert_equal(3, min(xn::core_number(G).values()));
    for (auto flow_func : flow_funcs) {
        assert_equal(1, xn::node_connectivity(G, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));
        assert_equal(1, xn::edge_connectivity(G, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));


auto test_complete_graphs() {
    for (auto n : range(5, 20, 5) {
        for (auto flow_func : flow_funcs) {
            G = xn::complete_graph(n);
            assert_equal(n - 1, xn::node_connectivity(G, flow_func=flow_func),
                         msg=msg.format(flow_func.__name__));
            assert_equal(n - 1, xn::node_connectivity(G.to_directed(),
                                                     flow_func=flow_func),
                         msg=msg.format(flow_func.__name__));
            assert_equal(n - 1, xn::edge_connectivity(G, flow_func=flow_func),
                         msg=msg.format(flow_func.__name__));
            assert_equal(n - 1, xn::edge_connectivity(G.to_directed(),
                                                     flow_func=flow_func),
                         msg=msg.format(flow_func.__name__));


auto test_empty_graphs() {
    for (auto k : range(5, 25, 5) {
        G = xn::empty_graph(k);
        for (auto flow_func : flow_funcs) {
            assert_equal(0, xn::node_connectivity(G, flow_func=flow_func),
                         msg=msg.format(flow_func.__name__));
            assert_equal(0, xn::edge_connectivity(G, flow_func=flow_func),
                         msg=msg.format(flow_func.__name__));


auto test_petersen() {
    G = xn::petersen_graph();
    for (auto flow_func : flow_funcs) {
        assert_equal(3, xn::node_connectivity(G, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));
        assert_equal(3, xn::edge_connectivity(G, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));


auto test_tutte() {
    G = xn::tutte_graph();
    for (auto flow_func : flow_funcs) {
        assert_equal(3, xn::node_connectivity(G, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));
        assert_equal(3, xn::edge_connectivity(G, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));


auto test_dodecahedral() {
    G = xn::dodecahedral_graph();
    for (auto flow_func : flow_funcs) {
        assert_equal(3, xn::node_connectivity(G, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));
        assert_equal(3, xn::edge_connectivity(G, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));


auto test_octahedral() {
    G = xn::octahedral_graph();
    for (auto flow_func : flow_funcs) {
        assert_equal(4, xn::node_connectivity(G, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));
        assert_equal(4, xn::edge_connectivity(G, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));


auto test_icosahedral() {
    G = xn::icosahedral_graph();
    for (auto flow_func : flow_funcs) {
        assert_equal(5, xn::node_connectivity(G, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));
        assert_equal(5, xn::edge_connectivity(G, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));


auto test_missing_source() {
    G = xn::path_graph(4);
    for (auto flow_func : flow_funcs) {
        assert_raises(xn::XNetworkError, xn::node_connectivity, G, 10, 1,
                      flow_func=flow_func);


auto test_missing_target() {
    G = xn::path_graph(4);
    for (auto flow_func : flow_funcs) {
        assert_raises(xn::XNetworkError, xn::node_connectivity, G, 1, 10,
                      flow_func=flow_func);


auto test_edge_missing_source() {
    G = xn::path_graph(4);
    for (auto flow_func : flow_funcs) {
        assert_raises(xn::XNetworkError, xn::edge_connectivity, G, 10, 1,
                      flow_func=flow_func);


auto test_edge_missing_target() {
    G = xn::path_graph(4);
    for (auto flow_func : flow_funcs) {
        assert_raises(xn::XNetworkError, xn::edge_connectivity, G, 1, 10,
                      flow_func=flow_func);


auto test_not_weakly_connected() {
    G = xn::DiGraph();
    xn::add_path(G, [1, 2, 3]);
    xn::add_path(G, [4, 5]);
    for (auto flow_func : flow_funcs) {
        assert_equal(xn::node_connectivity(G), 0,
                     msg=msg.format(flow_func.__name__));
        assert_equal(xn::edge_connectivity(G), 0,
                     msg=msg.format(flow_func.__name__));


auto test_not_connected() {
    G = xn::Graph();
    xn::add_path(G, [1, 2, 3]);
    xn::add_path(G, [4, 5]);
    for (auto flow_func : flow_funcs) {
        assert_equal(xn::node_connectivity(G), 0,
                     msg=msg.format(flow_func.__name__));
        assert_equal(xn::edge_connectivity(G), 0,
                     msg=msg.format(flow_func.__name__));


auto test_directed_edge_connectivity() {
    G = xn::cycle_graph(10, create_using=xn::DiGraph());  // only one direction
    D = xn::cycle_graph(10).to_directed();  // 2 reciprocal edges
    for (auto flow_func : flow_funcs) {
        assert_equal(1, xn::edge_connectivity(G, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));
        assert_equal(1, local_edge_connectivity(G, 1, 4, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));
        assert_equal(1, xn::edge_connectivity(G, 1, 4, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));
        assert_equal(2, xn::edge_connectivity(D, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));
        assert_equal(2, local_edge_connectivity(D, 1, 4, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));
        assert_equal(2, xn::edge_connectivity(D, 1, 4, flow_func=flow_func),
                     msg=msg.format(flow_func.__name__));


auto test_cutoff() {
    G = xn::complete_graph(5);
    for (auto local_func : [local_edge_connectivity, local_node_connectivity]) {
        for (auto flow_func : flow_funcs) {
            if (flow_func is flow.preflow_push) {
                // cutoff is not supported by preflow_push
                continue;
            for (auto cutoff : [3, 2, 1]) {
                result = local_func(G, 0, 4, flow_func=flow_func, cutoff=cutoff);
                assert_equal(cutoff, result,
                             msg="cutoff error : {0}".format(flow_func.__name__));


auto test_invalid_auxiliary() {
    G = xn::complete_graph(5);
    assert_raises(xn::XNetworkError, local_node_connectivity, G, 0, 3,
                  auxiliary=G);


auto test_interface_only_source() {
    G = xn::complete_graph(5);
    for (auto interface_func : [xn::node_connectivity, xn::edge_connectivity]) {
        assert_raises(xn::XNetworkError, interface_func, G, s=0);


auto test_interface_only_target() {
    G = xn::complete_graph(5);
    for (auto interface_func : [xn::node_connectivity, xn::edge_connectivity]) {
        assert_raises(xn::XNetworkError, interface_func, G, t=3);


auto test_edge_connectivity_flow_vs_stoer_wagner() {
    graph_funcs = [
        xn::icosahedral_graph,
        xn::octahedral_graph,
        xn::dodecahedral_graph,
    ];
    for (auto graph_func : graph_funcs) {
        G = graph_func();
        assert_equal(xn::stoer_wagner(G)[0], xn::edge_connectivity(G));


class TestAllPairsNodeConnectivity) {

    auto setUp() {
        this->path = xn::path_graph(7);
        this->directed_path = xn::path_graph(7, create_using=xn::DiGraph());
        this->cycle = xn::cycle_graph(7);
        this->directed_cycle = xn::cycle_graph(7, create_using=xn::DiGraph());
        this->gnp = xn::gnp_random_graph(30, 0.1);
        this->directed_gnp = xn::gnp_random_graph(30, 0.1, directed=true);
        this->K20 = xn::complete_graph(20);
        this->K10 = xn::complete_graph(10);
        this->K5 = xn::complete_graph(5);
        this->G_list = [this->path, this->directed_path, this->cycle,
                       this->directed_cycle, this->gnp, this->directed_gnp, this->K10,
                       this->K5, this->K20];

    auto test_cycles() {
        K_undir = xn::all_pairs_node_connectivity(this->cycle);
        for (auto source : K_undir) {
            for (auto target, k : K_undir[source].items() {
                assert_true(k == 2);
        K_dir = xn::all_pairs_node_connectivity(this->directed_cycle);
        for (auto source : K_dir) {
            for (auto target, k : K_dir[source].items() {
                assert_true(k == 1);

    auto test_complete() {
        for (auto G : [this->K10, this->K5, this->K20]) {
            K = xn::all_pairs_node_connectivity(G);
            for (auto source : K) {
                for (auto target, k : K[source].items() {
                    assert_true(k == len(G) - 1);

    auto test_paths() {
        K_undir = xn::all_pairs_node_connectivity(this->path);
        for (auto source : K_undir) {
            for (auto target, k : K_undir[source].items() {
                assert_true(k == 1);
        K_dir = xn::all_pairs_node_connectivity(this->directed_path);
        for (auto source : K_dir) {
            for (auto target, k : K_dir[source].items() {
                if (source < target) {
                    assert_true(k == 1);
                } else {
                    assert_true(k == 0);

    auto test_all_pairs_connectivity_nbunch() {
        G = xn::complete_graph(5);
        nbunch = [0, 2, 3];
        C = xn::all_pairs_node_connectivity(G, nbunch=nbunch);
        assert_equal(len(C), len(nbunch));

    auto test_all_pairs_connectivity_icosahedral() {
        G = xn::icosahedral_graph();
        C = xn::all_pairs_node_connectivity(G);
        assert_true(all(5 == C[u][v] for u, v : itertools.combinations(G, 2)));

    auto test_all_pairs_connectivity() {
        G = xn::Graph();
        nodes = [0, 1, 2, 3];
        xn::add_path(G, nodes);
        A = {n: {} for n : G}
        for (auto [u, v] : itertools.combinations(nodes, 2) {
            A[u][v] = A[v][u] = xn::node_connectivity(G, u, v);
        C = xn::all_pairs_node_connectivity(G);
        assert_equal(sorted((k, sorted(v)) for k, v : A.items()),
                     sorted((k, sorted(v)) for k, v : C.items()));

    auto test_all_pairs_connectivity_directed() {
        G = xn::DiGraph();
        nodes = [0, 1, 2, 3];
        xn::add_path(G, nodes);
        A = {n: {} for n : G}
        for (auto [u, v] : itertools.permutations(nodes, 2) {
            A[u][v] = xn::node_connectivity(G, u, v);
        C = xn::all_pairs_node_connectivity(G);
        assert_equal(sorted((k, sorted(v)) for k, v : A.items()),
                     sorted((k, sorted(v)) for k, v : C.items()));

    auto test_all_pairs_connectivity_nbunch_combinations() {
        G = xn::complete_graph(5);
        nbunch = [0, 2, 3];
        A = {n: {} for n : nbunch}
        for (auto [u, v] : itertools.combinations(nbunch, 2) {
            A[u][v] = A[v][u] = xn::node_connectivity(G, u, v);
        C = xn::all_pairs_node_connectivity(G, nbunch=nbunch);
        assert_equal(sorted((k, sorted(v)) for k, v : A.items()),
                     sorted((k, sorted(v)) for k, v : C.items()));

    auto test_all_pairs_connectivity_nbunch_iter() {
        G = xn::complete_graph(5);
        nbunch = [0, 2, 3];
        A = {n: {} for n : nbunch}
        for (auto [u, v] : itertools.combinations(nbunch, 2) {
            A[u][v] = A[v][u] = xn::node_connectivity(G, u, v);
        C = xn::all_pairs_node_connectivity(G, nbunch=iter(nbunch));
        assert_equal(sorted((k, sorted(v)) for k, v : A.items()),
                     sorted((k, sorted(v)) for k, v : C.items()));
