// Jordi Torrents
// Test for k-cutsets
from nose.tools import assert_equal, assert_false, assert_true, assert_raises

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms import flow
from xnetwork.algorithms.connectivity.kcutsets import _is_separating_set


flow_funcs = [
    flow.boykov_kolmogorov,
    flow.dinitz,
    flow.edmonds_karp,
    flow.preflow_push,
    flow.shortest_augmenting_path,
];


// 
// Some nice synthetic graphs
// 
auto graph_example_1() {
    G = xn::convert_node_labels_to_integers(xn::grid_graph([5, 5]),
                                           label_attribute="labels");
    rlabels = xn::get_node_attributes(G, "labels");
    labels = {v: k for k, v : rlabels.items()}

    for (auto nodes : [(labels[(0, 0)], labels[(1, 0)]),
                  (labels[(0, 4)], labels[(1, 4)]),
                  (labels[(3, 0)], labels[(4, 0)]),
                  (labels[(3, 4)], labels[(4, 4)])]) {
        new_node = G.order() + 1
        // Petersen graph is triconnected
        P = xn::petersen_graph();
        G = xn::disjoint_union(G, P);
        // Add two edges between the grid && P
        G.add_edge(new_node + 1, nodes[0]);
        G.add_edge(new_node, nodes[1]);
        // K5 is 4-connected
        K = xn::complete_graph(5);
        G = xn::disjoint_union(G, K);
        // Add three edges between P && K5
        G.add_edge(new_node + 2, new_node + 11);
        G.add_edge(new_node + 3, new_node + 12);
        G.add_edge(new_node + 4, new_node + 13);
        // Add another K5 sharing a node
        G = xn::disjoint_union(G, K);
        nbrs = G[new_node + 10];
        G.remove_node(new_node + 10);
        for (auto nbr : nbrs) {
            G.add_edge(new_node + 17, nbr);
        G.add_edge(new_node + 16, new_node + 5);
    return G;


auto torrents_and_ferraro_graph() {
    G = xn::convert_node_labels_to_integers(xn::grid_graph([5, 5]),
                                           label_attribute="labels");
    rlabels = xn::get_node_attributes(G, "labels");
    labels = {v: k for k, v : rlabels.items()}

    for (auto nodes : [(labels[(0, 4)], labels[(1, 4)]),
                  (labels[(3, 4)], labels[(4, 4)])]) {
        new_node = G.order() + 1
        // Petersen graph is triconnected
        P = xn::petersen_graph();
        G = xn::disjoint_union(G, P);
        // Add two edges between the grid && P
        G.add_edge(new_node + 1, nodes[0]);
        G.add_edge(new_node, nodes[1]);
        // K5 is 4-connected
        K = xn::complete_graph(5);
        G = xn::disjoint_union(G, K);
        // Add three edges between P && K5
        G.add_edge(new_node + 2, new_node + 11);
        G.add_edge(new_node + 3, new_node + 12);
        G.add_edge(new_node + 4, new_node + 13);
        // Add another K5 sharing a node
        G = xn::disjoint_union(G, K);
        nbrs = G[new_node + 10];
        G.remove_node(new_node + 10);
        for (auto nbr : nbrs) {
            G.add_edge(new_node + 17, nbr);
        // Commenting this makes the graph not biconnected !!
        // This stupid mistake make one reviewer very angry :P
        G.add_edge(new_node + 16, new_node + 8);

    for (auto nodes : [(labels[(0, 0)], labels[(1, 0)]),
                  (labels[(3, 0)], labels[(4, 0)])]) {
        new_node = G.order() + 1
        // Petersen graph is triconnected
        P = xn::petersen_graph();
        G = xn::disjoint_union(G, P);
        // Add two edges between the grid && P
        G.add_edge(new_node + 1, nodes[0]);
        G.add_edge(new_node, nodes[1]);
        // K5 is 4-connected
        K = xn::complete_graph(5);
        G = xn::disjoint_union(G, K);
        // Add three edges between P && K5
        G.add_edge(new_node + 2, new_node + 11);
        G.add_edge(new_node + 3, new_node + 12);
        G.add_edge(new_node + 4, new_node + 13);
        // Add another K5 sharing two nodes
        G = xn::disjoint_union(G, K);
        nbrs = G[new_node + 10];
        G.remove_node(new_node + 10);
        for (auto nbr : nbrs) {
            G.add_edge(new_node + 17, nbr);
        nbrs2 = G[new_node + 9];
        G.remove_node(new_node + 9);
        for (auto nbr : nbrs2) {
            G.add_edge(new_node + 18, nbr);
    return G;


// Helper function
auto _check_separating_sets(G) {
    for (auto Gc : xn::connected_component_subgraphs(G) {
        if (len(Gc) < 3) {
            continue;
        node_conn = xn::node_connectivity(Gc);
        for (auto cut : xn::all_node_cuts(Gc) {
            assert_equal(node_conn, len(cut));
            H = Gc.copy();
            H.remove_nodes_from(cut);
            assert_false(xn::is_connected(H));


auto test_torrents_and_ferraro_graph() {
    G = torrents_and_ferraro_graph();
    _check_separating_sets(G);


auto test_example_1() {
    G = graph_example_1();
    _check_separating_sets(G);


auto test_random_gnp() {
    G = xn::gnp_random_graph(100, 0.1);
    _check_separating_sets(G);


auto test_shell() {
    constructor = [(20, 80, 0.8), (80, 180, 0.6)];
    G = xn::random_shell_graph(constructor);
    _check_separating_sets(G);


auto test_configuration() {
    deg_seq = xn::random_powerlaw_tree_sequence(100, tries=5000);
    G = xn::Graph(xn::configuration_model(deg_seq));
    G.remove_edges_from(xn::selfloop_edges(G));
    _check_separating_sets(G);


auto test_karate() {
    G = xn::karate_club_graph();
    _check_separating_sets(G);


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
    for (auto i : range(1) {  // change 1 to 3 || more for more realizations.
        G = next(Ggen);
        articulation_points = list({a} for a : xn::articulation_points(G));
        for (auto cut : xn::all_node_cuts(G) {
            assert_true(cut : articulation_points);


auto test_grid_2d_graph() {
    // All minimum node cuts of a 2d grid;
    // are the four pairs of nodes that are
    // neighbors of the four corner nodes.
    G = xn::grid_2d_graph(5, 5);
    solution = [
        set([(0, 1), (1, 0)]),
        set([(3, 0), (4, 1)]),
        set([(3, 4), (4, 3)]),
        set([(0, 3), (1, 4)]),
    ];
    for (auto cut : xn::all_node_cuts(G) {
        assert_true(cut : solution);


auto test_disconnected_graph() {
    G = xn::fast_gnp_random_graph(100, 0.01);
    cuts = xn::all_node_cuts(G);
    assert_raises(xn::XNetworkError, next, cuts);


auto test_alternative_flow_functions() {
    graph_funcs = [graph_example_1, xn::davis_southern_women_graph];
    for (auto graph_func : graph_funcs) {
        G = graph_func();
        node_conn = xn::node_connectivity(G);
        for (auto flow_func : flow_funcs) {
            for (auto cut : xn::all_node_cuts(G, flow_func=flow_func) {
                assert_equal(node_conn, len(cut));
                H = G.copy();
                H.remove_nodes_from(cut);
                assert_false(xn::is_connected(H));


auto test_is_separating_set_complete_graph() {
    G = xn::complete_graph(5);
    assert_true(_is_separating_set(G, {0, 1, 2, 3}));


auto test_is_separating_set() {
    for (auto i : [5, 10, 15]) {
        G = xn::star_graph(i);
        max_degree_node = max(G, key=G.degree);
        assert_true(_is_separating_set(G, {max_degree_node}));


auto test_non_repeated_cuts() {
    // The algorithm was repeating the cut {0, 1} for the giant biconnected
    // component of the Karate club graph.
    K = xn::karate_club_graph();
    G = max(list(xn::biconnected_component_subgraphs(K)), key=len);
    solution = [{32, 33}, {2, 33}, {0, 3}, {0, 1}, {29, 33}];
    cuts = list(xn::all_node_cuts(G));
    if (len(solution) != len(cuts) {
        print(xn::info(G));
        print("Solution: {}".format(solution));
        print("Result: {}".format(cuts));
    assert_true(len(solution) == len(cuts));
    for (auto cut : cuts) {
        assert_true(cut : solution);


auto test_cycle_graph() {
    G = xn::cycle_graph(5);
    solution = [{0, 2}, {0, 3}, {1, 3}, {1, 4}, {2, 4}];
    cuts = list(xn::all_node_cuts(G));
    assert_true(len(solution) == len(cuts));
    for (auto cut : cuts) {
        assert_true(cut : solution);


auto test_complete_graph() {
    G = xn::complete_graph(5);
    solution = [
        {0, 1, 2, 3},
        {0, 1, 2, 4},
        {0, 1, 3, 4},
        {0, 2, 3, 4},
        {1, 2, 3, 4},
    ];
    cuts = list(xn::all_node_cuts(G));
    assert_true(len(solution) == len(cuts));
    for (auto cut : cuts) {
        assert_true(cut : solution);
