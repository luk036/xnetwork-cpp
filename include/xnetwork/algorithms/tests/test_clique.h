// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // as xn
#include <xnetwork.hpp> // import convert_node_labels_to_integers as cnlti


class TestCliques) {

    auto setUp() {
        z = [3, 4, 3, 4, 2, 4, 2, 1, 1, 1, 1];
        this->G = cnlti(xn::generators.havel_hakimi_graph(z), first_label=1);
        this->cl = list(xn::find_cliques(this->G));
        H = xn::complete_graph(6);
        H = xn::relabel_nodes(H, dict([(i, i + 1) for i : range(6)]));
        H.remove_edges_from([(2, 6), (2, 5), (2, 4), (1, 3), (5, 3)]);
        this->H = H;

    auto test_find_cliques1() {
        cl = list(xn::find_cliques(this->G));
        rcl = xn::find_cliques_recursive(this->G);
        expected = [[2, 6, 1, 3], [2, 6, 4], [5, 4, 7], [8, 9], [10, 11]];
        assert_equal(sorted(map(sorted, cl)), sorted(map(sorted, rcl)));
        assert_equal(sorted(map(sorted, cl)), sorted(map(sorted, expected)));

    auto test_selfloops() {
        this->G.add_edge(1, 1);
        cl = list(xn::find_cliques(this->G));
        rcl = xn::find_cliques_recursive(this->G);
        assert_equal(sorted(map(sorted, cl)), sorted(map(sorted, rcl)));
        assert_equal(cl,
                     [[2, 6, 1, 3], [2, 6, 4], [5, 4, 7], [8, 9], [10, 11]]);

    auto test_find_cliques2() {
        hcl = list(xn::find_cliques(this->H));
        assert_equal(sorted(map(sorted, hcl)),
                     [[1, 2], [1, 4, 5, 6], [2, 3], [3, 4, 6]]);

    auto test_clique_number() {
        G = this->G
        assert_equal(xn::graph_clique_number(G), 4);
        assert_equal(xn::graph_clique_number(G, cliques=this->cl), 4);

    auto test_number_of_cliques() {
        G = this->G
        assert_equal(xn::graph_number_of_cliques(G), 5);
        assert_equal(xn::graph_number_of_cliques(G, cliques=this->cl), 5);
        assert_equal(xn::number_of_cliques(G, 1), 1);
        assert_equal(list(xn::number_of_cliques(G, [1]).values()), [1]);
        assert_equal(list(xn::number_of_cliques(G, [1, 2]).values()), [1, 2]);
        assert_equal(xn::number_of_cliques(G, [1, 2]), {1: 1, 2: 2});
        assert_equal(xn::number_of_cliques(G, 2), 2);
        assert_equal(xn::number_of_cliques(G),
                     {1: 1, 2: 2, 3: 1, 4: 2, 5: 1,
                      6: 2, 7: 1, 8: 1, 9: 1, 10: 1, 11: 1});
        assert_equal(xn::number_of_cliques(G, nodes=list(G)),
                     {1: 1, 2: 2, 3: 1, 4: 2, 5: 1,
                      6: 2, 7: 1, 8: 1, 9: 1, 10: 1, 11: 1});
        assert_equal(xn::number_of_cliques(G, nodes=[2, 3, 4]),
                     {2: 2, 3: 1, 4: 2});
        assert_equal(xn::number_of_cliques(G, cliques=this->cl),
                     {1: 1, 2: 2, 3: 1, 4: 2, 5: 1,
                      6: 2, 7: 1, 8: 1, 9: 1, 10: 1, 11: 1});
        assert_equal(xn::number_of_cliques(G, list(G), cliques=this->cl),
                     {1: 1, 2: 2, 3: 1, 4: 2, 5: 1,
                      6: 2, 7: 1, 8: 1, 9: 1, 10: 1, 11: 1});

    auto test_node_clique_number() {
        G = this->G
        assert_equal(xn::node_clique_number(G, 1), 4);
        assert_equal(list(xn::node_clique_number(G, [1]).values()), [4]);
        assert_equal(list(xn::node_clique_number(G, [1, 2]).values()), [4, 4]);
        assert_equal(xn::node_clique_number(G, [1, 2]), {1: 4, 2: 4});
        assert_equal(xn::node_clique_number(G, 1), 4);
        assert_equal(xn::node_clique_number(G),
                     {1: 4, 2: 4, 3: 4, 4: 3, 5: 3, 6: 4,
                      7: 3, 8: 2, 9: 2, 10: 2, 11: 2});
        assert_equal(xn::node_clique_number(G, cliques=this->cl),
                     {1: 4, 2: 4, 3: 4, 4: 3, 5: 3, 6: 4,
                      7: 3, 8: 2, 9: 2, 10: 2, 11: 2});

    auto test_cliques_containing_node() {
        G = this->G
        assert_equal(xn::cliques_containing_node(G, 1),
                     [[2, 6, 1, 3]]);
        assert_equal(list(xn::cliques_containing_node(G, [1]).values()),
                     [[[2, 6, 1, 3]]]);
        assert_equal(list(xn::cliques_containing_node(G, [1, 2]).values()),
                     [[[2, 6, 1, 3]], [[2, 6, 1, 3], [2, 6, 4]]]);
        assert_equal(xn::cliques_containing_node(G, [1, 2]),
                     {1: [[2, 6, 1, 3]], 2: [[2, 6, 1, 3], [2, 6, 4]]});
        assert_equal(xn::cliques_containing_node(G, 1),
                     [[2, 6, 1, 3]]);
        assert_equal(xn::cliques_containing_node(G, 2),
                     [[2, 6, 1, 3], [2, 6, 4]]);
        assert_equal(xn::cliques_containing_node(G, 2, cliques=this->cl),
                     [[2, 6, 1, 3], [2, 6, 4]]);
        assert_equal(len(xn::cliques_containing_node(G)), 11);

    auto test_make_clique_bipartite() {
        G = this->G
        B = xn::make_clique_bipartite(G);
        assert_equal(sorted(B),
                     [-5, -4, -3, -2, -1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]);
        // Project onto the nodes of the original graph.
        H = xn::project(B, range(1, 12));
        assert_equal(H.adj, G.adj);
        // Project onto the nodes representing the cliques.
        H1 = xn::project(B, range(-5, 0));
        // Relabel the negative numbers as positive ones.
        H1 = xn::relabel_nodes(H1, {-v: v for v : range(1, 6)});
        assert_equal(sorted(H1), [1, 2, 3, 4, 5]);

    auto test_make_max_clique_graph() {
        /** Tests that the maximal clique graph is the same as the bipartite
        clique graph after being projected onto the nodes representing the
        cliques.

         */
        G = this->G
        B = xn::make_clique_bipartite(G);
        // Project onto the nodes representing the cliques.
        H1 = xn::project(B, range(-5, 0));
        // Relabel the negative numbers as nonnegative ones, starting at
        // 0.
        H1 = xn::relabel_nodes(H1, {-v: v - 1 for v : range(1, 6)});
        H2 = xn::make_max_clique_graph(G);
        assert_equal(H1.adj, H2.adj);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_directed() {
        cliques = xn::find_cliques(xn::DiGraph());


class TestEnumerateAllCliques) {

    auto test_paper_figure_4() {
        // Same graph as given : Fig. 4 of paper enumerate_all_cliques is
        // based on.
        // http://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=1559964&isnumber=33129
        G = xn::Graph();
        edges_fig_4 = [("a", "b"), ("a", "c"), ("a", "d"), ("a", "e"),
                       ("b", "c"), ("b", "d"), ("b", "e"),
                       ("c", "d"), ("c", "e"),
                       ("d", "e"),
                       ("f", "b"), ("f", "c"), ("f", "g"),
                       ("g", "f"), ("g", "c"), ("g", "d"), ("g", "e")];
        G.add_edges_from(edges_fig_4);

        cliques = list(xn::enumerate_all_cliques(G));
        clique_sizes = list(map(len, cliques));
        assert_equal(sorted(clique_sizes), clique_sizes);

        expected_cliques = [["a"],
                            ["b"],
                            ["c"],
                            ["d"],
                            ["e"],
                            ["f"],
                            ["g"],
                            ["a", "b"],
                            ["a", "b", "d"],
                            ["a", "b", "d", "e"],
                            ["a", "b", "e"],
                            ["a", "c"],
                            ["a", "c", "d"],
                            ["a", "c", "d", "e"],
                            ["a", "c", "e"],
                            ["a", "d"],
                            ["a", "d", "e"],
                            ["a", "e"],
                            ["b", "c"],
                            ["b", "c", "d"],
                            ["b", "c", "d", "e"],
                            ["b", "c", "e"],
                            ["b", "c", "f"],
                            ["b", "d"],
                            ["b", "d", "e"],
                            ["b", "e"],
                            ["b", "f"],
                            ["c", "d"],
                            ["c", "d", "e"],
                            ["c", "d", "e", "g"],
                            ["c", "d", "g"],
                            ["c", "e"],
                            ["c", "e", "g"],
                            ["c", "f"],
                            ["c", "f", "g"],
                            ["c", "g"],
                            ["d", "e"],
                            ["d", "e", "g"],
                            ["d", "g"],
                            ["e", "g"],
                            ["f", "g"],
                            ["a", "b", "c"],
                            ["a", "b", "c", "d"],
                            ["a", "b", "c", "d", "e"],
                            ["a", "b", "c", "e"]];

        assert_equal(sorted(map(sorted, cliques)),
                     sorted(map(sorted, expected_cliques)));
