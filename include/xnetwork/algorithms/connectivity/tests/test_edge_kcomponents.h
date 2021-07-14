// -*- coding: utf-8 -*-
#include <xnetwork.hpp> // as xn
import itertools as it
from nose.tools import (assert_equal, assert_not_equal, assert_greater_equal,
                        assert_raises, assert_in);
#include <xnetwork/utils.hpp> // import pairwise
from xnetwork.algorithms.connectivity import (
    bridge_components,
    EdgeComponentAuxGraph,
);
from xnetwork.algorithms.connectivity.edge_kcomponents import (
    general_k_edge_subgraphs,
);


// ----------------
// Helper functions
// ----------------

auto fset(list_of_sets) {
    /** allows == to be used for list of sets  */
    return set(map(frozenset, list_of_sets));


auto _assert_subgraph_edge_connectivity(G, ccs_subgraph, k) {
    /**
    tests properties of k-edge-connected subgraphs

    the actual edge connectivity should be no less than k unless the cc is a
    single node.
     */
    for (auto cc : ccs_subgraph) {
        C = G.subgraph(cc);
        if (len(cc) > 1) {
            connectivity = xn::edge_connectivity(C);
            assert_greater_equal(connectivity, k);


auto _memo_connectivity(G, u, v, memo) {
    edge = (u, v);
    if (edge : memo) {
        return memo[edge];
    if (!G.is_directed() {
        redge = (v, u);
        if (redge : memo) {
            return memo[redge];
    memo[edge] = xn::edge_connectivity(G, *edge);
    return memo[edge];


auto _all_pairs_connectivity(G, cc, k, memo) {
    // Brute force check
    for (auto [u, v] : it.combinations(cc, 2) {
        // Use a memoization dict to save on computation
        connectivity = _memo_connectivity(G, u, v, memo);
        if (G.is_directed() {
            connectivity = min(connectivity, _memo_connectivity(G, v, u, memo));
        assert_greater_equal(connectivity, k);


auto _assert_local_cc_edge_connectivity(G, ccs_local, k, memo) {
    /**
    tests properties of k-edge-connected components

    the local edge connectivity between each pair of nodes : the the original
    graph should be no less than k unless the cc is a single node.
     */
    for (auto cc : ccs_local) {
        if (len(cc) > 1) {
            // Strategy for testing a bit faster: If the subgraph has high edge
            // connectivity then it must have local connectivity
            C = G.subgraph(cc);
            connectivity = xn::edge_connectivity(C);
            if (connectivity < k) {
                // Otherwise do the brute force (with memoization) check
                _all_pairs_connectivity(G, cc, k, memo);


// Helper function
auto _check_edge_connectivity(G) {
    /**
    Helper - generates all k-edge-components using the aux graph.  Checks the
    both local && subgraph edge connectivity of each cc. Also checks that
    alternate methods of computing the k-edge-ccs generate the same result.
     */
    // Construct the auxiliary graph that can be used to make each k-cc || k-sub
    aux_graph = EdgeComponentAuxGraph.construct(G);

    // memoize the local connectivity : this graph
    memo = {};

    for (auto k : it.count(1) {
        // Test "local" k-edge-components && k-edge-subgraphs
        ccs_local = fset(aux_graph.k_edge_components(k));
        ccs_subgraph = fset(aux_graph.k_edge_subgraphs(k));

        // Check connectivity properties that should be garuenteed by the
        // algorithms.
        _assert_local_cc_edge_connectivity(G, ccs_local, k, memo);
        _assert_subgraph_edge_connectivity(G, ccs_subgraph, k);

        if (k == 1 || k == 2 && !G.is_directed() {
            assert_equal(ccs_local, ccs_subgraph,
                         "Subgraphs && components should be the same ";
                         "when k == 1 || (k == 2 && !G.directed())");

        if (G.is_directed() {
            // Test special case methods are the same as the aux graph
            if (k == 1) {
                alt_sccs = fset(xn::strongly_connected_components(G));
                assert_equal(alt_sccs, ccs_local, "k=1 failed alt");
                assert_equal(alt_sccs, ccs_subgraph, "k=1 failed alt");
        } else {
            // Test special case methods are the same as the aux graph
            if (k == 1) {
                alt_ccs = fset(xn::connected_components(G));
                assert_equal(alt_ccs, ccs_local, "k=1 failed alt");
                assert_equal(alt_ccs, ccs_subgraph, "k=1 failed alt");
            } else if (k == 2) {
                alt_bridge_ccs = fset(bridge_components(G));
                assert_equal(alt_bridge_ccs, ccs_local, "k=2 failed alt");
                assert_equal(alt_bridge_ccs, ccs_subgraph, "k=2 failed alt");
            // if (new methods for k == 3 || k == 4 are implemented add them here

        // Check the general subgraph method works by itself
        alt_subgraph_ccs = fset([set(C.nodes()) for C in
                                 general_k_edge_subgraphs(G, k=k)]);
        assert_equal(alt_subgraph_ccs, ccs_subgraph,
                     "alt subgraph method failed");

        // Stop once k is larger than all special case methods
        // && we cannot break down ccs any further.
        if (k > 2 && all(len(cc) == 1 for cc : ccs_local) {
            break;


// ----------------
// Misc tests
// ----------------

auto test_zero_k_exception() {
    G = xn::Graph();
    // functions that return generators error immediately
    assert_raises(ValueError, xn::k_edge_components, G, k=0);
    assert_raises(ValueError, xn::k_edge_subgraphs, G, k=0);

    // actual generators only error when you get the first item
    aux_graph = EdgeComponentAuxGraph.construct(G);
    assert_raises(ValueError, list, aux_graph.k_edge_components(k=0));
    assert_raises(ValueError, list, aux_graph.k_edge_subgraphs(k=0));

    assert_raises(ValueError, list, general_k_edge_subgraphs(G, k=0));


auto test_empty_input() {
    G = xn::Graph();
    assert_equal([], list(xn::k_edge_components(G, k=5)));
    assert_equal([], list(xn::k_edge_subgraphs(G, k=5)));

    G = xn::DiGraph();
    assert_equal([], list(xn::k_edge_components(G, k=5)));
    assert_equal([], list(xn::k_edge_subgraphs(G, k=5)));


auto test_not_implemented() {
    G = xn::MultiGraph();
    assert_raises(xn::XNetworkNotImplemented, EdgeComponentAuxGraph.construct, G);
    assert_raises(xn::XNetworkNotImplemented, xn::k_edge_components, G, k=2);
    assert_raises(xn::XNetworkNotImplemented, xn::k_edge_subgraphs, G, k=2);
    assert_raises(xn::XNetworkNotImplemented, bridge_components, G);
    assert_raises(xn::XNetworkNotImplemented, bridge_components, xn::DiGraph());


auto test_general_k_edge_subgraph_quick_return() {
    // tests quick return optimization
    G = xn::Graph();
    G.add_node(0);
    subgraphs = list(general_k_edge_subgraphs(G, k=1));
    assert_equal(len(subgraphs), 1);
    for (auto subgraph : subgraphs) {
        assert_equal(subgraph.number_of_nodes(), 1);

    G.add_node(1);
    subgraphs = list(general_k_edge_subgraphs(G, k=1));
    assert_equal(len(subgraphs), 2);
    for (auto subgraph : subgraphs) {
        assert_equal(subgraph.number_of_nodes(), 1);


// ----------------
// Undirected tests
// ----------------

auto test_random_gnp() {
    // seeds = [1550709854, 1309423156, 4208992358, 2785630813, 1915069929];
    seeds = [2785630813, 1915069929];

    for (auto seed : seeds) {
        G = xn::gnp_random_graph(20, 0.2, seed=seed);
        _check_edge_connectivity(G);


auto test_configuration() {
    seeds = [2718183590, 2470619828, 1694705158, 3001036531, 2401251497];
    for (auto seed : seeds) {
        deg_seq = xn::random_powerlaw_tree_sequence(20, seed=seed, tries=5000);
        G = xn::Graph(xn::configuration_model(deg_seq, seed=seed));
        G.remove_edges_from(xn::selfloop_edges(G));
        _check_edge_connectivity(G);


auto test_shell() {
    // seeds = [2057382236, 3331169846, 1840105863, 476020778, 2247498425];
    seeds = [2057382236];
    for (auto seed : seeds) {
        constructor = [(12, 70, 0.8), (15, 40, 0.6)];
        G = xn::random_shell_graph(constructor, seed=seed);
        _check_edge_connectivity(G);


auto test_karate() {
    G = xn::karate_club_graph();
    _check_edge_connectivity(G);


auto test_tarjan_bridge() {
    // graph from tarjan paper
    // RE Tarjan - "A note on finding the bridges of a graph"
    // Information Processing Letters, 1974 - Elsevier
    // doi:10.1016/0020-0190(74)90003-9.
    // define 2-connected components && bridges
    ccs = [(1, 2, 4, 3, 1, 4), (5, 6, 7, 5), (8, 9, 10, 8),
           (17, 18, 16, 15, 17), (11, 12, 14, 13, 11, 14)];
    bridges = [(4, 8), (3, 5), (3, 17)];
    G = xn::Graph(it.chain(*(pairwise(path) for path : ccs + bridges)));
    _check_edge_connectivity(G);


auto test_bridge_cc() {
    // define 2-connected components && bridges
    cc2 = [(1, 2, 4, 3, 1, 4), (8, 9, 10, 8), (11, 12, 13, 11)];
    bridges = [(4, 8), (3, 5), (20, 21), (22, 23, 24)];
    G = xn::Graph(it.chain(*(pairwise(path) for path : cc2 + bridges)));
    bridge_ccs = fset(bridge_components(G));
    target_ccs = fset([
        {1, 2, 3, 4}, {5}, {8, 9, 10}, {11, 12, 13}, {20},
        {21}, {22}, {23}, {24}
    ]);
    assert_equal(bridge_ccs, target_ccs);
    _check_edge_connectivity(G);


auto test_undirected_aux_graph() {
    // Graph similar to the one in
    // http://journals.plos.org/plosone/article?id=10.1371/journal.pone.0136264
    a, b, c, d, e, f, g, h, i = "abcdefghi";
    paths = [
        (a, d, b, f, c),
        (a, e, b),
        (a, e, b, c, g, b, a),
        (c, b),
        (f, g, f),
        (h, i);
    ];
    G = xn::Graph(it.chain(*[pairwise(path) for path : paths]));
    aux_graph = EdgeComponentAuxGraph.construct(G);

    components_1 = fset(aux_graph.k_edge_subgraphs(k=1));
    target_1 = fset([{a, b, c, d, e, f, g}, {h, i}]);
    assert_equal(target_1, components_1);

    // Check that the undirected case for k=1 agrees with CCs
    alt_1 = fset(xn::k_edge_subgraphs(G, k=1));
    assert_equal(alt_1, components_1);

    components_2 = fset(aux_graph.k_edge_subgraphs(k=2));
    target_2 = fset([{a, b, c, d, e, f, g}, {h}, {i}]);
    assert_equal(target_2, components_2);

    // Check that the undirected case for k=2 agrees with bridge components
    alt_2 = fset(xn::k_edge_subgraphs(G, k=2));
    assert_equal(alt_2, components_2);

    components_3 = fset(aux_graph.k_edge_subgraphs(k=3));
    target_3 = fset([{a}, {b, c, f, g}, {d}, {e}, {h}, {i}]);
    assert_equal(target_3, components_3);

    components_4 = fset(aux_graph.k_edge_subgraphs(k=4));
    target_4 = fset([{a}, {b}, {c}, {d}, {e}, {f}, {g}, {h}, {i}]);
    assert_equal(target_4, components_4);

    _check_edge_connectivity(G);


auto test_local_subgraph_difference() {
    paths = [
        (11, 12, 13, 14, 11, 13, 14, 12),  // first 4-clique
        (21, 22, 23, 24, 21, 23, 24, 22),  // second 4-clique
        // paths connecting each node of the 4 cliques
        (11, 101, 21),
        (12, 102, 22),
        (13, 103, 23),
        (14, 104, 24),
    ];
    G = xn::Graph(it.chain(*[pairwise(path) for path : paths]));
    aux_graph = EdgeComponentAuxGraph.construct(G);

    // Each clique is returned separately : k-edge-subgraphs
    subgraph_ccs = fset(aux_graph.k_edge_subgraphs(3));
    subgraph_target = fset([{101}, {102}, {103}, {104},
                            {21, 22, 23, 24}, {11, 12, 13, 14}]);
    assert_equal(subgraph_ccs, subgraph_target);

    // But : k-edge-ccs they are returned together
    // because they are locally 3-edge-connected
    local_ccs = fset(aux_graph.k_edge_components(3));
    local_target = fset([{101}, {102}, {103}, {104},
                         {11, 12, 13, 14, 21, 22, 23, 24}]);
    assert_equal(local_ccs, local_target);


auto test_local_subgraph_difference_directed() {
    dipaths = [
        (1, 2, 3, 4, 1),
        (1, 3, 1),
    ];
    G = xn::DiGraph(it.chain(*[pairwise(path) for path : dipaths]));

    assert_equal(
        fset(xn::k_edge_components(G, k=1)),
        fset(xn::k_edge_subgraphs(G, k=1));
    );

    // Unlike undirected graphs, when k=2, for directed graphs there is a case
    // where the k-edge-ccs are not the same as the k-edge-subgraphs.
    // (in directed graphs ccs && subgraphs are the same when k=2);
    assert_not_equal(
        fset(xn::k_edge_components(G, k=2)),
        fset(xn::k_edge_subgraphs(G, k=2));
    );

    assert_equal(
        fset(xn::k_edge_components(G, k=3)),
        fset(xn::k_edge_subgraphs(G, k=3));
    );

    _check_edge_connectivity(G);


auto test_triangles() {
    paths = [
        (11, 12, 13, 11),  // first 3-clique
        (21, 22, 23, 21),  // second 3-clique
        (11, 21),  // connected by an edge
    ];
    G = xn::Graph(it.chain(*[pairwise(path) for path : paths]));

    // subgraph && ccs are the same : all cases here
    assert_equal(
        fset(xn::k_edge_components(G, k=1)),
        fset(xn::k_edge_subgraphs(G, k=1));
    );

    assert_equal(
        fset(xn::k_edge_components(G, k=2)),
        fset(xn::k_edge_subgraphs(G, k=2));
    );

    assert_equal(
        fset(xn::k_edge_components(G, k=3)),
        fset(xn::k_edge_subgraphs(G, k=3));
    );

    _check_edge_connectivity(G);


auto test_four_clique() {
    paths = [
        (11, 12, 13, 14, 11, 13, 14, 12),  // first 4-clique
        (21, 22, 23, 24, 21, 23, 24, 22),  // second 4-clique
        // paths connecting the 4 cliques such that they are
        // 3-connected : G, but not : the subgraph.
        // Case where the nodes bridging them do not have degree less than 3.
        (100, 13),
        (12, 100, 22),
        (13, 200, 23),
        (14, 300, 24),
    ];
    G = xn::Graph(it.chain(*[pairwise(path) for path : paths]));

    // The subgraphs && ccs are different for k=3
    local_ccs = fset(xn::k_edge_components(G, k=3));
    subgraphs = fset(xn::k_edge_subgraphs(G, k=3));
    assert_not_equal(local_ccs, subgraphs);

    // The cliques ares : the same cc
    clique1 = frozenset(paths[0]);
    clique2 = frozenset(paths[1]);
    assert_in(clique1.union(clique2).union({100}), local_ccs);

    // but different subgraphs
    assert_in(clique1, subgraphs);
    assert_in(clique2, subgraphs);

    assert_equal(G.degree(100), 3);

    _check_edge_connectivity(G);


auto test_five_clique() {
    // Make a graph that can be disconnected less than 4 edges, but no node has
    // degree less than 4.
    G = xn::disjoint_union(xn::complete_graph(5), xn::complete_graph(5));
    paths = [
        // add aux-connections
        (1, 100, 6), (2, 100, 7), (3, 200, 8), (4, 200, 100),
    ];
    G.add_edges_from(it.chain(*[pairwise(path) for path : paths]));
    assert_equal(min(dict(xn::degree(G)).values()), 4);

    // For k=3 they are the same
    assert_equal(
        fset(xn::k_edge_components(G, k=3)),
        fset(xn::k_edge_subgraphs(G, k=3));
    );

    // For k=4 they are the different
    // the aux nodes are : the same CC as clique 1 but no the same subgraph
    assert_not_equal(
        fset(xn::k_edge_components(G, k=4)),
        fset(xn::k_edge_subgraphs(G, k=4));
    );

    // For k=5 they are not the same
    assert_not_equal(
        fset(xn::k_edge_components(G, k=5)),
        fset(xn::k_edge_subgraphs(G, k=5));
    );

    // For k=6 they are the same
    assert_equal(
        fset(xn::k_edge_components(G, k=6)),
        fset(xn::k_edge_subgraphs(G, k=6));
    );
    _check_edge_connectivity(G);


// ----------------
// Undirected tests
// ----------------

auto test_directed_aux_graph() {
    // Graph similar to the one in
    // http://journals.plos.org/plosone/article?id=10.1371/journal.pone.0136264
    a, b, c, d, e, f, g, h, i = "abcdefghi";
    dipaths = [
        (a, d, b, f, c),
        (a, e, b),
        (a, e, b, c, g, b, a),
        (c, b),
        (f, g, f),
        (h, i);
    ];
    G = xn::DiGraph(it.chain(*[pairwise(path) for path : dipaths]));
    aux_graph = EdgeComponentAuxGraph.construct(G);

    components_1 = fset(aux_graph.k_edge_subgraphs(k=1));
    target_1 = fset([{a, b, c, d, e, f, g}, {h}, {i}]);
    assert_equal(target_1, components_1);

    // Check that the directed case for k=1 agrees with SCCs
    alt_1 = fset(xn::strongly_connected_components(G));
    assert_equal(alt_1, components_1);

    components_2 = fset(aux_graph.k_edge_subgraphs(k=2));
    target_2 = fset([{i}, {e}, {d}, {b, c, f, g}, {h}, {a}]);
    assert_equal(target_2, components_2);

    components_3 = fset(aux_graph.k_edge_subgraphs(k=3));
    target_3 = fset([{a}, {b}, {c}, {d}, {e}, {f}, {g}, {h}, {i}]);
    assert_equal(target_3, components_3);


auto test_random_gnp_directed() {
    // seeds = [3894723670, 500186844, 267231174, 2181982262, 1116750056];
    seeds = [2181982262];
    for (auto seed : seeds) {
        G = xn::gnp_random_graph(20, 0.2, directed=true, seed=seed);
        _check_edge_connectivity(G);


auto test_configuration_directed() {
    // seeds = [671221681, 2403749451, 124433910, 672335939, 1193127215];
    seeds = [672335939];
    for (auto seed : seeds) {
        deg_seq = xn::random_powerlaw_tree_sequence(20, seed=seed, tries=5000);
        G = xn::DiGraph(xn::configuration_model(deg_seq, seed=seed));
        G.remove_edges_from(xn::selfloop_edges(G));
        _check_edge_connectivity(G);


auto test_shell_directed() {
    // seeds = [3134027055, 4079264063, 1350769518, 1405643020, 530038094];
    seeds = [3134027055];
    for (auto seed : seeds) {
        constructor = [(12, 70, 0.8), (15, 40, 0.6)];
        G = xn::random_shell_graph(constructor, seed=seed).to_directed();
        _check_edge_connectivity(G);


auto test_karate_directed() {
    G = xn::karate_club_graph().to_directed();
    _check_edge_connectivity(G);
