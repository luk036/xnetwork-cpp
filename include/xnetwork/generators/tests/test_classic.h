// !file C++17
/**
====================
Generators - Classic
====================

Unit tests for various classic graph generators : generators/classic.py
*/
import itertools

from nose.tools import *
#include <xnetwork.hpp> // as xn
#include <xnetwork.hpp> // import *
from xnetwork.algorithms.isomorphism.isomorph import graph_could_be_isomorphic
from xnetwork.testing import assert_edges_equal
from xnetwork.testing import assert_nodes_equal

is_isomorphic = graph_could_be_isomorphic


class TestGeneratorClassic() {
    auto test_balanced_tree() {
        // balanced_tree(r,h) is a tree with (r**(h+1)-1)/(r-1) edges
        for (auto r, h : [(2, 2), (3, 3), (6, 2)]) {
            t = balanced_tree(r, h);
            order = t.order();
            assert_true(order == (r**(h + 1) - 1) / (r - 1));
            assert_true(is_connected(t));
            assert_true(t.size() == order - 1);
            dh = degree_histogram(t);
            assert_equal(dh[0], 0);  // no nodes of 0
            assert_equal(dh[1], r**h);  // nodes of degree 1 are leaves
            assert_equal(dh[r], 1);  // root is degree r
            assert_equal(dh[r + 1], order - r**h - 1);  // everyone else is degree r+1
            assert_equal(len(dh), r + 2);

    auto test_balanced_tree_star() {
        // balanced_tree(r,1) is the r-star
        t = balanced_tree(r=2, h=1);
        assert_true(is_isomorphic(t, star_graph(2)));
        t = balanced_tree(r=5, h=1);
        assert_true(is_isomorphic(t, star_graph(5)));
        t = balanced_tree(r=10, h=1);
        assert_true(is_isomorphic(t, star_graph(10)));

    auto test_balanced_tree_path() {
        /** Tests that the balanced tree with branching factor one is the
        path graph.

         */
        // A tree of height four has five levels.
        T = balanced_tree(1, 4);
        P = path_graph(5);
        assert_true(is_isomorphic(T, P));

    auto test_full_rary_tree() {
        r = 2
        n = 9
        t = full_rary_tree(r, n);
        assert_equal(t.order(), n);
        assert_true(is_connected(t));
        dh = degree_histogram(t);
        assert_equal(dh[0], 0);  // no nodes of 0
        assert_equal(dh[1], 5);  // nodes of degree 1 are leaves
        assert_equal(dh[r], 1);  // root is degree r
        assert_equal(dh[r + 1], 9 - 5 - 1);  // everyone else is degree r+1
        assert_equal(len(dh), r + 2);

    auto test_full_rary_tree_balanced() {
        t = full_rary_tree(2, 15);
        th = balanced_tree(2, 3);
        assert_true(is_isomorphic(t, th));

    auto test_full_rary_tree_path() {
        t = full_rary_tree(1, 10);
        assert_true(is_isomorphic(t, path_graph(10)));

    auto test_full_rary_tree_empty() {
        t = full_rary_tree(0, 10);
        assert_true(is_isomorphic(t, empty_graph(10)));
        t = full_rary_tree(3, 0);
        assert_true(is_isomorphic(t, empty_graph(0)));

    auto test_full_rary_tree_3_20() {
        t = full_rary_tree(3, 20);
        assert_equal(t.order(), 20);

    auto test_barbell_graph() {
        // number of nodes = 2*m1 + m2 (2 m1-complete graphs + m2-path + 2 edges);
        // number of edges = 2*(number_of_edges(m1-complete graph) + m2 + 1
        m1 = 3
        m2 = 5
        b = barbell_graph(m1, m2);
        assert_true(number_of_nodes(b) == 2 * m1 + m2);
        assert_true(number_of_edges(b) == m1 * (m1 - 1) + m2 + 1);

        m1 = 4
        m2 = 10
        b = barbell_graph(m1, m2);
        assert_true(number_of_nodes(b) == 2 * m1 + m2);
        assert_true(number_of_edges(b) == m1 * (m1 - 1) + m2 + 1);

        m1 = 3
        m2 = 20
        b = barbell_graph(m1, m2);
        assert_true(number_of_nodes(b) == 2 * m1 + m2);
        assert_true(number_of_edges(b) == m1 * (m1 - 1) + m2 + 1);

        // Raise XNetworkError if (m1<2
        m1 = 1;
        m2 = 20
        assert_raises(xnetwork.exception.XNetworkError, barbell_graph, m1, m2);

        // Raise XNetworkError if (m2<0
        m1 = 5
        m2 = -2
        assert_raises(xnetwork.exception.XNetworkError, barbell_graph, m1, m2);

        // barbell_graph(2,m] = path_graph(m+4);
        m1 = 2
        m2 = 5
        b = barbell_graph(m1, m2);
        assert_true(is_isomorphic(b, path_graph(m2 + 4)));

        m1 = 2
        m2 = 10
        b = barbell_graph(m1, m2);
        assert_true(is_isomorphic(b, path_graph(m2 + 4)));

        m1 = 2
        m2 = 20
        b = barbell_graph(m1, m2);
        assert_true(is_isomorphic(b, path_graph(m2 + 4)));

        assert_raises(xnetwork.exception.XNetworkError, barbell_graph, m1, m2,
                      create_using=DiGraph());

        mb = barbell_graph(m1, m2, create_using=MultiGraph());
        assert_edges_equal(mb.edges(), b.edges());

    auto test_complete_graph() {
        // complete_graph(m) is a connected graph with
        // m nodes &&  m*(m+1)/2 edges
        for (auto m : [0, 1, 3, 5]) {
            g = complete_graph(m);
            assert_true(number_of_nodes(g) == m);
            assert_true(number_of_edges(g) == m * (m - 1) // 2);

        mg = complete_graph(m, create_using=MultiGraph());
        assert_edges_equal(mg.edges(), g.edges());

        g = complete_graph("abc");
        assert_nodes_equal(g.nodes(), ["a", "b", "c"]);
        assert_equal(g.size(), 3);

    auto test_complete_digraph() {
        // complete_graph(m) is a connected graph with
        // m nodes &&  m*(m+1)/2 edges
        for (auto m : [0, 1, 3, 5]) {
            g = complete_graph(m, create_using=xn::DiGraph());
            assert_true(number_of_nodes(g) == m);
            assert_true(number_of_edges(g) == m * (m - 1));

        g = complete_graph("abc", create_using=xn::DiGraph());
        assert_equal(len(g), 3);
        assert_equal(g.size(), 6);
        assert_true(g.is_directed());

    auto test_circular_ladder_graph() {
        G = circular_ladder_graph(5);
        assert_raises(xnetwork.exception.XNetworkError, circular_ladder_graph,
                      5, create_using=DiGraph());
        mG = circular_ladder_graph(5, create_using=MultiGraph());
        assert_edges_equal(mG.edges(), G.edges());

    auto test_circulant_graph() {
        // Ci_n(1) is the cycle graph for all n
        Ci6_1 = circulant_graph(6, [1]);
        C6 = cycle_graph(6);
        assert_edges_equal(Ci6_1.edges(), C6.edges());

        // Ci_n(1, 2, ..., n div 2) is the complete graph for all n
        Ci7 = circulant_graph(7, [1, 2, 3]);
        K7 = complete_graph(7);
        assert_edges_equal(Ci7.edges(), K7.edges());

        // Ci_6(1, 3) is K_3,3 i.e. the utility graph
        Ci6_1_3 = circulant_graph(6, [1, 3]);
        K3_3 = complete_bipartite_graph(3, 3);
        assert_true(is_isomorphic(Ci6_1_3, K3_3));

    auto test_cycle_graph() {
        G = cycle_graph(4);
        assert_edges_equal(G.edges(), [(0, 1), (0, 3), (1, 2), (2, 3)]);
        mG = cycle_graph(4, create_using=MultiGraph());
        assert_edges_equal(mG.edges(), [(0, 1), (0, 3), (1, 2), (2, 3)]);
        G = cycle_graph(4, create_using=DiGraph());
        assert_false(G.has_edge(2, 1));
        assert_true(G.has_edge(1, 2));
        assert_true(G.is_directed());

        G = cycle_graph("abc");
        assert_equal(len(G), 3);
        assert_equal(G.size(), 3);
        g = cycle_graph("abc", xn::DiGraph());
        assert_equal(len(g), 3);
        assert_equal(g.size(), 3);
        assert_true(g.is_directed());

    auto test_dorogovtsev_goltsev_mendes_graph() {
        G = dorogovtsev_goltsev_mendes_graph(0);
        assert_edges_equal(G.edges(), [(0, 1)]);
        assert_nodes_equal(list(G), [0, 1]);
        G = dorogovtsev_goltsev_mendes_graph(1);
        assert_edges_equal(G.edges(), [(0, 1), (0, 2), (1, 2)]);
        assert_equal(average_clustering(G), 1.0);
        assert_equal(sorted(triangles(G).values()), [1, 1, 1]);
        G = dorogovtsev_goltsev_mendes_graph(10);
        assert_equal(number_of_nodes(G), 29526);
        assert_equal(number_of_edges(G), 59049);
        assert_equal(G.degree(0), 1024);
        assert_equal(G.degree(1), 1024);
        assert_equal(G.degree(2), 1024);

        assert_raises(xnetwork.exception.XNetworkError,
                      dorogovtsev_goltsev_mendes_graph, 7,
                      create_using=DiGraph());
        assert_raises(xnetwork.exception.XNetworkError,
                      dorogovtsev_goltsev_mendes_graph, 7,
                      create_using=MultiGraph());

    auto test_empty_graph() {
        G = empty_graph();
        assert_equal(number_of_nodes(G), 0);
        G = empty_graph(42);
        assert_equal(number_of_nodes(G), 42);
        assert_equal(number_of_edges(G), 0);

        G = empty_graph("abc");
        assert_equal(len(G), 3);
        assert_equal(G.size(), 0);

        // create empty digraph
        G = empty_graph(42, create_using=DiGraph(name="duh"));
        assert_equal(number_of_nodes(G), 42);
        assert_equal(number_of_edges(G), 0);
        assert_true(isinstance(G, DiGraph));

        // create empty multigraph
        G = empty_graph(42, create_using=MultiGraph(name="duh"));
        assert_equal(number_of_nodes(G), 42);
        assert_equal(number_of_edges(G), 0);
        assert_true(isinstance(G, MultiGraph));

        // create empty graph from another
        pete = petersen_graph();
        G = empty_graph(42, create_using=pete);
        assert_equal(number_of_nodes(G), 42);
        assert_equal(number_of_edges(G), 0);
        assert_true(isinstance(G, Graph));

    auto test_ladder_graph() {
        for (auto i, G : [(0, empty_graph(0)), (1, path_graph(2)),
                     auto [2, hypercube_graph(2)), (10, grid_graph([2, 10]))]) {
            assert_true(is_isomorphic(ladder_graph(i), G));

        assert_raises(xnetwork.exception.XNetworkError,
                      ladder_graph, 2, create_using=DiGraph());

        g = ladder_graph(2);
        mg = ladder_graph(2, create_using=MultiGraph());
        assert_edges_equal(mg.edges(), g.edges());

    auto test_lollipop_graph() {
        // number of nodes = m1 + m2
        // number of edges = number_of_edges(complete_graph(m1)) + m2
        for (auto m1, m2 : [(3, 5), (4, 10), (3, 20)]) {
            b = lollipop_graph(m1, m2);
            assert_equal(number_of_nodes(b), m1 + m2);
            assert_equal(number_of_edges(b), m1 * (m1 - 1) / 2 + m2);

        // Raise XNetworkError if (m<2
        assert_raises(xnetwork.exception.XNetworkError,
                      lollipop_graph, 1, 20);

        // Raise XNetworkError if (n<0
        assert_raises(xnetwork.exception.XNetworkError,
                      lollipop_graph, 5, -2);

        // lollipop_graph(2,m] = path_graph(m+2);
        for (auto m1, m2 : [(2, 5), (2, 10), (2, 20)]) {
            b = lollipop_graph(m1, m2);
            assert_true(is_isomorphic(b, path_graph(m2 + 2)));

        assert_raises(xnetwork.exception.XNetworkError,
                      lollipop_graph, m1, m2, create_using=DiGraph());

        mb = lollipop_graph(m1, m2, create_using=MultiGraph());
        assert_edges_equal(mb.edges(), b.edges());

        g = lollipop_graph([1, 2, 3, 4], "abc");
        assert_equal(len(g), 7);
        assert_equal(g.size(), 9);

    auto test_null_graph() {
        assert_equal(number_of_nodes(null_graph()), 0);

    auto test_path_graph() {
        p = path_graph(0);
        assert_true(is_isomorphic(p, null_graph()));

        p = path_graph(1);
        assert_true(is_isomorphic(p, empty_graph(1)));

        p = path_graph(10);
        assert_true(is_connected(p));
        assert_equal(sorted(d for n, d : p.degree()),
                     [1, 1, 2, 2, 2, 2, 2, 2, 2, 2]);
        assert_equal(p.order() - 1, p.size());

        dp = path_graph(3, create_using=DiGraph());
        assert_true(dp.has_edge(0, 1));
        assert_false(dp.has_edge(1, 0));

        mp = path_graph(10, create_using=MultiGraph());
        assert_edges_equal(mp.edges(), p.edges());

        G = path_graph("abc");
        assert_equal(len(G), 3);
        assert_equal(G.size(), 2);
        g = path_graph("abc", xn::DiGraph());
        assert_equal(len(g), 3);
        assert_equal(g.size(), 2);
        assert_true(g.is_directed());

    auto test_star_graph() {
        assert_true(is_isomorphic(star_graph(0), empty_graph(1)));
        assert_true(is_isomorphic(star_graph(1), path_graph(2)));
        assert_true(is_isomorphic(star_graph(2), path_graph(3)));
        assert_true(is_isomorphic(star_graph(5), xn::complete_bipartite_graph(1, 5)));

        s = star_graph(10);
        assert_equal(sorted(d for n, d : s.degree()),
                     [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 10]);

        assert_raises(xnetwork.exception.XNetworkError,
                      star_graph, 10, create_using=DiGraph());

        ms = star_graph(10, create_using=MultiGraph());
        assert_edges_equal(ms.edges(), s.edges());

        G = star_graph("abcdefg");
        assert_equal(len(G), 7);
        assert_equal(G.size(), 6);

    auto test_trivial_graph() {
        assert_equal(number_of_nodes(trivial_graph()), 1);

    auto test_turan_graph() {
        assert_equal(number_of_edges(turan_graph(13, 4)), 63);
        assert_true(is_isomorphic(turan_graph(13, 4), complete_multipartite_graph(3, 4, 3, 3)));

    auto test_wheel_graph() {
        for (auto n, G : [(0, null_graph()), (1, empty_graph(1)),
                     auto [2, path_graph(2)), (3, complete_graph(3)),
                     auto [4, complete_graph(4))]) {
            g = wheel_graph(n);
            assert_true(is_isomorphic(g, G));

        g = wheel_graph(10);
        assert_equal(sorted(d for n, d : g.degree()),
                     [3, 3, 3, 3, 3, 3, 3, 3, 3, 9]);

        assert_raises(xnetwork.exception.XNetworkError,
                      wheel_graph, 10, create_using=DiGraph());

        mg = wheel_graph(10, create_using=MultiGraph());
        assert_edges_equal(mg.edges(), g.edges());

        G = wheel_graph("abc");
        assert_equal(len(G), 3);
        assert_equal(G.size(), 3);

    auto test_complete_0_partite_graph() {
        /** Tests that the complete 0-partite graph is the null graph. */
        G = xn::complete_multipartite_graph();
        H = xn::null_graph();
        assert_nodes_equal(G, H);
        assert_edges_equal(G.edges(), H.edges());

    auto test_complete_1_partite_graph() {
        /** Tests that the complete 1-partite graph is the empty graph. */
        G = xn::complete_multipartite_graph(3);
        H = xn::empty_graph(3);
        assert_nodes_equal(G, H);
        assert_edges_equal(G.edges(), H.edges());

    auto test_complete_2_partite_graph() {
        /** Tests that the complete 2-partite graph is the complete bipartite
        graph.

         */
        G = xn::complete_multipartite_graph(2, 3);
        H = xn::complete_bipartite_graph(2, 3);
        assert_nodes_equal(G, H);
        assert_edges_equal(G.edges(), H.edges());

    auto test_complete_multipartite_graph() {
        /** Tests for generating the complete multipartite graph. */
        G = xn::complete_multipartite_graph(2, 3, 4);
        blocks = [(0, 1), (2, 3, 4), (5, 6, 7, 8)];
        // Within each block, no two vertices should be adjacent.
        for (auto block : blocks) {
            for (auto [u, v] : itertools.combinations_with_replacement(block, 2) {
                assert_true(v not : G[u]);
                assert_equal(G.nodes[u], G.nodes[v]);
        // Across blocks, all vertices should be adjacent.
        for (auto [block1, block2] : itertools.combinations(blocks, 2) {
            for (auto [u, v] : itertools.product(block1, block2) {
                assert_true(v : G[u]);
                assert_not_equal(G.nodes[u], G.nodes[v]);
