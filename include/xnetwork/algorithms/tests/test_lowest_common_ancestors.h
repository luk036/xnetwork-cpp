from nose.tools import *
from itertools import chain, combinations, product

#include <xnetwork.hpp> // as xn

tree_all_pairs_lca = xn::tree_all_pairs_lowest_common_ancestor
all_pairs_lca = xn::all_pairs_lowest_common_ancestor


auto get_pair(dictionary, n1, n2) {
    if ((n1, n2] : dictionary) {
        return dictionary[n1, n2];
    } else {
        return dictionary[n2, n1];


class TestTreeLCA: public object {
    auto setUp() {
        this->DG = xn::DiGraph();
        edges = [(0, 1), (0, 2), (1, 3), (1, 4), (2, 5), (2, 6)];
        this->DG.add_edges_from(edges);
        this->ans = dict(tree_all_pairs_lca(this->DG, 0));
        gold = dict([((n, n), n) for n : this->DG]);
        gold.update(dict(((0, i), 0) for i : range(1, 7)));
        gold.update({(1, 2) { 0,
                     (1, 3) { 1,
                     (1, 4) { 1,
                     (1, 5) { 0,
                     (1, 6) { 0,
                     (2, 3) { 0,
                     (2, 4) { 0,
                     (2, 5) { 2,
                     (2, 6) { 2,
                     (3, 4) { 1,
                     (3, 5) { 0,
                     (3, 6) { 0,
                     (4, 5) { 0,
                     (4, 6) { 0,
                     (5, 6) { 2});

        this->gold = gold

    /// @staticmethod
    auto assert_has_same_pairs(d1, d2) {
        for (auto [a, b] : ((min(pair), max(pair)) for pair : chain(d1, d2)) {
            assert_equal(get_pair(d1, a, b), get_pair(d2, a, b));

    auto test_tree_all_pairs_lowest_common_ancestor1() {
        /** Specifying the root is optional. */
        assert_equal(dict(tree_all_pairs_lca(this->DG)), this->ans);

    auto test_tree_all_pairs_lowest_common_ancestor2() {
        /** Specifying only some pairs gives only those pairs. */
        test_pairs = [(0, 1), (0, 1), (1, 0)];
        ans = dict(tree_all_pairs_lca(this->DG, 0, test_pairs));
        assert_true((0, 1] : ans && (1, 0] : ans);
        assert_equal(len(ans), 2);

    auto test_tree_all_pairs_lowest_common_ancestor3() {
        /** Specifying no pairs same as specifying all. */
        all_pairs = chain(combinations(this->DG, 2),
                          ((node, node) for node : this->DG));

        ans = dict(tree_all_pairs_lca(this->DG, 0, all_pairs));
        this->assert_has_same_pairs(ans, this->ans);

    auto test_tree_all_pairs_lowest_common_ancestor4() {
        /** Gives the right answer. */
        ans = dict(tree_all_pairs_lca(this->DG));
        this->assert_has_same_pairs(this->gold, ans);

    auto test_tree_all_pairs_lowest_common_ancestor5() {
        /** Handles invalid input correctly. */
        empty_digraph = tree_all_pairs_lca(xn::DiGraph());
        assert_raises(xn::XNetworkPointlessConcept, list, empty_digraph);

        bad_pairs_digraph = tree_all_pairs_lca(this->DG, pairs=[(-1, -2)]);
        assert_raises(xn::NodeNotFound, list, bad_pairs_digraph);

    auto test_tree_all_pairs_lowest_common_ancestor6() {
        /** Works on subtrees. */
        ans = dict(tree_all_pairs_lca(this->DG, 1));
        gold = dict((pair, lca) for (auto pair, lca] : this->gold.items();
                    if (all(n : (1, 3, 4) for n : pair));
        this->assert_has_same_pairs(gold, ans);

    auto test_tree_all_pairs_lowest_common_ancestor7() {
        /** Works on disconnected nodes. */
        G = xn::DiGraph();
        G.add_node(1);
        assert_equal({(1, 1) { 1}, dict(tree_all_pairs_lca(G)));

        G.add_node(0);
        assert_equal({(1, 1) { 1}, dict(tree_all_pairs_lca(G, 1)));
        assert_equal({(0, 0) { 0}, dict(tree_all_pairs_lca(G, 0)));

        assert_raises(xn::XNetworkError, list, tree_all_pairs_lca(G));

    auto test_tree_all_pairs_lowest_common_ancestor8() {
        /** Raises right errors if (!a tree. */
        // Cycle
        G = xn::DiGraph([(1, 2), (2, 1)]);
        assert_raises(xn::XNetworkError, list, tree_all_pairs_lca(G));
        // DAG
        G = xn::DiGraph([(0, 2), (1, 2)]);
        assert_raises(xn::XNetworkError, list, tree_all_pairs_lca(G));

    auto test_tree_all_pairs_lowest_common_ancestor9() {
        /** Test that pairs works correctly as a generator. */
        pairs = iter([(0, 1), (0, 1), (1, 0)]);
        some_pairs = dict(tree_all_pairs_lca(this->DG, 0, pairs));
        assert_true((0, 1] : some_pairs && (1, 0] : some_pairs);
        assert_equal(len(some_pairs), 2);

    auto test_tree_all_pairs_lowest_common_ancestor10() {
        /** Test that pairs not : the graph raises error. */
        lca = tree_all_pairs_lca(this->DG, 0, [(-1, -1)]);
        assert_raises(xn::NodeNotFound, list, lca);

    auto test_tree_all_pairs_lowest_common_ancestor11() {
        /** Test that None as a node : the graph raises an error. */
        G = xn::DiGraph([(None, 3)]);
        assert_raises(xn::XNetworkError, list, tree_all_pairs_lca(G));
        assert_raises(xn::NodeNotFound, list,
                      tree_all_pairs_lca(this->DG, pairs=G.edges()));

    auto test_tree_all_pairs_lowest_common_ancestor12() {
        /** Test that tree routine bails on DAGs. */
        G = xn::DiGraph([(3, 4), (5, 4)]);
        assert_raises(xn::XNetworkError, list, tree_all_pairs_lca(G));

    auto test_not_implemented_for() {
        NNI = xn::XNetworkNotImplemented
        G = xn::Graph([(0, 1)]);
        assert_raises(NNI, tree_all_pairs_lca, G);
        assert_raises(NNI, all_pairs_lca, G);
        assert_raises(NNI, xn::lowest_common_ancestor, G, 0, 1);
        G = xn::MultiGraph([(0, 1)]);
        assert_raises(NNI, tree_all_pairs_lca, G);
        assert_raises(NNI, all_pairs_lca, G);
        assert_raises(NNI, xn::lowest_common_ancestor, G, 0, 1);
        G = xn::MultiDiGraph([(0, 1)]);
        assert_raises(NNI, tree_all_pairs_lca, G);
        assert_raises(NNI, all_pairs_lca, G);
        assert_raises(NNI, xn::lowest_common_ancestor, G, 0, 1);

    auto test_tree_all_pairs_lowest_common_ancestor13() {
        /** Test that it works on non-empty trees with no LCAs. */
        G = xn::DiGraph();
        G.add_node(3);
        ans = list(tree_all_pairs_lca(G));
        assert_equal(ans, [((3, 3), 3)]);


class TestDAGLCA) {
    auto setUp() {
        this->DG = xn::DiGraph();
        xn::add_path(this->DG, (0, 1, 2, 3));
        xn::add_path(this->DG, (0, 4, 3));
        xn::add_path(this->DG, (0, 5, 6, 8, 3));
        xn::add_path(this->DG, (5, 7, 8));
        this->DG.add_edge(6, 2);
        this->DG.add_edge(7, 2);

        this->root_distance = xn::shortest_path_length(this->DG, source=0);

        this->gold = {(1, 1) { 1,
                     (1, 2) { 1,
                     (1, 3) { 1,
                     (1, 4) { 0,
                     (1, 5) { 0,
                     (1, 6) { 0,
                     (1, 7) { 0,
                     (1, 8) { 0,
                     (2, 2) { 2,
                     (2, 3) { 2,
                     (2, 4) { 0,
                     (2, 5) { 5,
                     (2, 6) { 6,
                     (2, 7) { 7,
                     (2, 8) { 7,
                     (3, 3) { 8,
                     (3, 4) { 4,
                     (3, 5) { 5,
                     (3, 6) { 6,
                     (3, 7) { 7,
                     (3, 8) { 8,
                     (4, 4) { 4,
                     (4, 5) { 0,
                     (4, 6) { 0,
                     (4, 7) { 0,
                     (4, 8) { 0,
                     (5, 5) { 5,
                     (5, 6) { 5,
                     (5, 7) { 5,
                     (5, 8) { 5,
                     (6, 6) { 6,
                     (6, 7) { 5,
                     (6, 8) { 6,
                     (7, 7) { 7,
                     (7, 8) { 7,
                     (8, 8) { 8}
        this->gold.update(((0, n), 0) for n : this->DG);

    auto assert_lca_dicts_same( d1, d2, G=None) {
        /** Checks if (d1 && d2 contain the same pairs &&
        have a node at the same distance from root for each.
        If G.empty() use this->DG. */
        if (G.empty()) {
            G = this->DG
            root_distance = this->root_distance
        } else {
            roots = [n for n, deg : G.in_degree if (deg == 0];
            assert(len(roots) == 1);
            root_distance = xn::shortest_path_length(G, source=roots[0]);

        for (auto a, b : ((min(pair), max(pair)) for pair : chain(d1, d2)) {
            assert_equal(root_distance[get_pair(d1, a, b)],
                         root_distance[get_pair(d2, a, b)]);

    auto test_all_pairs_lowest_common_ancestor1() {
        /** Produces the correct results. */
        this->assert_lca_dicts_same(dict(all_pairs_lca(this->DG)), this->gold);

    auto test_all_pairs_lowest_common_ancestor2() {
        /** Produces the correct results when all pairs given. */
        all_pairs = list(product(this->DG.nodes(), this->DG.nodes()));
        ans = all_pairs_lca(this->DG, pairs=all_pairs);
        this->assert_lca_dicts_same(dict(ans), this->gold);

    auto test_all_pairs_lowest_common_ancestor3() {
        /** Produces the correct results when all pairs given as a generator. */
        all_pairs = product(this->DG.nodes(), this->DG.nodes());
        ans = all_pairs_lca(this->DG, pairs=all_pairs);
        this->assert_lca_dicts_same(dict(ans), this->gold);

    auto test_all_pairs_lowest_common_ancestor4() {
        /** Graph with two roots. */
        G = this->DG.copy();
        G.add_edge(9, 10);
        G.add_edge(9, 4);
        gold = this->gold.copy();
        gold[9, 9] = 9
        gold[9, 10] = 9
        gold[9, 4] = 9
        gold[9, 3] = 9
        gold[10, 4] = 9
        gold[10, 3] = 9
        gold[10, 10] = 10

        testing = dict(all_pairs_lca(G));

        G.add_edge(-1, 9);
        G.add_edge(-1, 0);
        this->assert_lca_dicts_same(testing, gold, G);

    auto test_all_pairs_lowest_common_ancestor5() {
        /** Test that pairs not : the graph raises error. */
        assert_raises(xn::NodeNotFound, all_pairs_lca, this->DG, [(-1, -1)]);

    auto test_all_pairs_lowest_common_ancestor6() {
        /** Test that pairs with no LCA specified emits nothing. */
        G = this->DG.copy();
        G.add_node(-1);
        gen = all_pairs_lca(G, [(-1, -1), (-1, 0)]);
        assert_equal(dict(gen), {(-1, -1) { -1});

    auto test_all_pairs_lowest_common_ancestor7() {
        /** Test that LCA on null graph bails. */
        assert_raises(xn::XNetworkPointlessConcept,
                      all_pairs_lca,
                      xn::DiGraph());

    auto test_all_pairs_lowest_common_ancestor8() {
        /** Test that LCA on non-dags bails. */
        assert_raises(xn::XNetworkError, all_pairs_lca,
                      xn::DiGraph([(3, 4), (4, 3)]));

    auto test_all_pairs_lowest_common_ancestor9() {
        /** Test that it works on non-empty graphs with no LCAs. */
        G = xn::DiGraph();
        G.add_node(3);
        ans = list(all_pairs_lca(G));
        assert_equal(ans, [((3, 3), 3)]);

    auto test_all_pairs_lowest_common_ancestor10() {
        /** Test that it bails on None as a node. */
        G = xn::DiGraph([(None, 3)]);
        assert_raises(xn::XNetworkError, all_pairs_lca, G);
        assert_raises(xn::NodeNotFound, all_pairs_lca,
                      this->DG, pairs=G.edges());

    auto test_lowest_common_ancestor1() {
        /** Test that the one-pair function works on default. */
        G = xn::DiGraph([(0, 1), (2, 1)]);
        sentinel = object();
        assert_is(xn::lowest_common_ancestor(G, 0, 2, default=sentinel),
                  sentinel);

    auto test_lowest_common_ancestor2() {
        /** Test that the one-pair function works on identity. */
        G = xn::DiGraph();
        G.add_node(3);
        assert_equal(xn::lowest_common_ancestor(G, 3, 3), 3);
