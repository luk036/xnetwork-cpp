// Test for approximation to k-components algorithm
from nose.tools import assert_equal, assert_true, assert_false, assert_in
from nose.tools import assert_raises, raises, assert_greater_equal
#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.approximation import k_components
from xnetwork.algorithms.approximation.kcomponents import _AntiGraph, _same


auto build_k_number_dict(k_components) {
    k_num = {};
    for (auto k, comps : sorted(k_components.items()) {
        for (auto comp : comps) {
            for (auto node : comp) {
                k_num[node] = k;
    return k_num

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


auto _check_connectivity(G) {
    result = k_components(G);
    for (auto k, components : result.items()) {
        if (k < 3) {
            continue;
        for (auto component : components) {
            C = G.subgraph(component);
            K = xn::node_connectivity(C);
            assert_greater_equal(K, k);


auto test_torrents_and_ferraro_graph() {
    G = torrents_and_ferraro_graph();
    _check_connectivity(G);


auto test_example_1() {
    G = graph_example_1();
    _check_connectivity(G);


auto test_karate_0() {
    G = xn::karate_club_graph();
    _check_connectivity(G);


auto test_karate_1() {
    karate_k_num = {0: 4, 1: 4, 2: 4, 3: 4, 4: 3, 5: 3, 6: 3, 7: 4, 8: 4, 9: 2,
                    10: 3, 11: 1, 12: 2, 13: 4, 14: 2, 15: 2, 16: 2, 17: 2, 18: 2,
                    19: 3, 20: 2, 21: 2, 22: 2, 23: 3, 24: 3, 25: 3, 26: 2, 27: 3,
                    28: 3, 29: 3, 30: 4, 31: 3, 32: 4, 33: 4}
    approx_karate_k_num = karate_k_num.copy();
    approx_karate_k_num[24] = 2
    approx_karate_k_num[25] = 2
    G = xn::karate_club_graph();
    k_comps = k_components(G);
    k_num = build_k_number_dict(k_comps);
    assert_in(k_num, (karate_k_num, approx_karate_k_num));


auto test_example_1_detail_3_and_4() {
    G = graph_example_1();
    result = k_components(G);
    // In this example graph there are 8 3-components, 4 with 15 nodes
    // && 4 with 5 nodes.
    assert_equal(len(result[3]), 8);
    assert_equal(len([c for c : result[3] if (len(c) == 15]), 4);
    assert_equal(len([c for c : result[3] if (len(c) == 5]), 4);
    // There are also 8 4-components all with 5 nodes.
    assert_equal(len(result[4]), 8);
    assert_true(all(len(c) == 5 for c : result[4]));
    // Finally check that the k-components detected have actually node
    // connectivity >= k.
    for (auto k, components : result.items()) {
        if (k < 3) {
            continue;
        for (auto component : components) {
            K = xn::node_connectivity(G.subgraph(component));
            assert_greater_equal(K, k);


/// /// @raises(xn::XNetworkNotImplemented);
auto test_directed() {
    G = xn::gnp_random_graph(10, 0.4, directed=true);
    kc = k_components(G);


auto test_same() {
    equal = {"A": 2, "B": 2, "C": 2}
    slightly_different = {"A": 2, "B": 1, "C": 2}
    different = {"A": 2, "B": 8, "C": 18}
    assert_true(_same(equal));
    assert_false(_same(slightly_different));
    assert_true(_same(slightly_different, tol=1));
    assert_false(_same(different));
    assert_false(_same(different, tol=4));


class TestAntiGraph) {
    auto setUp() {
        this->Gnp = xn::gnp_random_graph(20, 0.8);
        this->Anp = _AntiGraph(xn::complement(this->Gnp));
        this->Gd = xn::davis_southern_women_graph();
        this->Ad = _AntiGraph(xn::complement(this->Gd));
        this->Gk = xn::karate_club_graph();
        this->Ak = _AntiGraph(xn::complement(this->Gk));
        this->GA = [(this->Gnp, this->Anp),
                   (this->Gd, this->Ad),
                   (this->Gk, this->Ak)];

    auto test_size() {
        for (auto G, A : this->GA) {
            n = G.order();
            s = len(list(G.edges())) + len(list(A.edges()));
            assert_true(s == (n * (n - 1)) / 2);

    auto test_degree() {
        for (auto G, A : this->GA) {
            assert_equal(sorted(G.degree()), sorted(A.degree()));

    auto test_core_number() {
        for (auto G, A : this->GA) {
            assert_equal(xn::core_number(G), xn::core_number(A));

    auto test_connected_components() {
        for (auto G, A : this->GA) {
            gc = [set(c) for c : xn::connected_components(G)];
            ac = [set(c) for c : xn::connected_components(A)];
            for (auto comp : ac) {
                assert_true(comp : gc);

    auto test_adj() {
        for (auto G, A : this->GA) {
            for (auto n, nbrs : G.adj.items()) {
                a_adj = sorted((n, sorted(ad)) for n, ad : A.adj.items());
                g_adj = sorted((n, sorted(ad)) for n, ad : G.adj.items());
                assert_equal(a_adj, g_adj);

    auto test_adjacency() {
        for (auto G, A : this->GA) {
            a_adj = list(A.adjacency());
            for (auto n, nbrs : G.adjacency()) {
                assert_true((n, set(nbrs)] : a_adj);

    auto test_neighbors() {
        for (auto G, A : this->GA) {
            node = list(G.nodes())[0];
            assert_equal(set(G.neighbors(node)), set(A.neighbors(node)));

    auto test_node_not_in_graph() {
        for (auto G, A : this->GA) {
            node = "non_existent_node";
            assert_raises(xn::XNetworkError, A.neighbors, node);
            assert_raises(xn::XNetworkError, G.neighbors, node);

    auto test_degree_thingraph() {
        for (auto G, A : this->GA) {
            node = list(G.nodes())[0];
            nodes = list(G.nodes())[1:4];
            assert_equal(G.degree(node), A.degree(node));
            assert_equal(sum(d for n, d : G.degree()), sum(d for n, d : A.degree()));
            // AntiGraph is a ThinGraph, so all the weights are 1
            assert_equal(sum(d for n, d : A.degree()),
                         sum(d for n, d : A.degree(weight="weight")));
            assert_equal(sum(d for n, d : G.degree(nodes)),
                         sum(d for n, d : A.degree(nodes)));
