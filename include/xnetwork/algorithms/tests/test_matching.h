from itertools import permutations
import math

from nose.tools import assert_equal
from nose.tools import assert_false
from nose.tools import assert_true
#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.matching import matching_dict_to_set
from xnetwork.testing import assert_edges_equal


class TestMaxWeightMatching: public object {
    /** Unit tests for the
    :func:`~xnetwork.algorithms.matching.max_weight_matching` function.

     */

    auto test_trivial1() {
        /** Empty graph */
        G = xn::Graph();
        assert_equal(xn::max_weight_matching(G), set());

    auto test_trivial2() {
        /** Self loop */
        G = xn::Graph();
        G.add_edge(0, 0, weight=100);
        assert_equal(xn::max_weight_matching(G), set());

    auto test_trivial3() {
        /** Single edge */
        G = xn::Graph();
        G.add_edge(0, 1);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({0: 1, 1: 0}));

    auto test_trivial4() {
        /** Small graph */
        G = xn::Graph();
        G.add_edge("one", "two", weight=10);
        G.add_edge("two", "three", weight=11);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({"three": "two", "two": "three"}));

    auto test_trivial5() {
        /** Path */
        G = xn::Graph();
        G.add_edge(1, 2, weight=5);
        G.add_edge(2, 3, weight=11);
        G.add_edge(3, 4, weight=5);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({2: 3, 3: 2}));
        assert_edges_equal(xn::max_weight_matching(G, 1),
                           matching_dict_to_set({1: 2, 2: 1, 3: 4, 4: 3}));

    auto test_trivial6() {
        /** Small graph with arbitrary weight attribute */
        G = xn::Graph();
        G.add_edge("one", "two", weight=10, abcd=11);
        G.add_edge("two", "three", weight=11, abcd=10);
        assert_edges_equal(xn::max_weight_matching(G, weight="abcd"),
                           matching_dict_to_set({"one": "two", "two": "one"}));

    auto test_doubleing_point_weights() {
        /** Floating point weights */
        G = xn::Graph();
        G.add_edge(1, 2, weight=math.pi);
        G.add_edge(2, 3, weight=math.exp(1));
        G.add_edge(1, 3, weight=3.0);
        G.add_edge(1, 4, weight=math.sqrt(2.0));
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({1: 4, 2: 3, 3: 2, 4: 1}));

    auto test_negative_weights() {
        /** Negative weights */
        G = xn::Graph();
        G.add_edge(1, 2, weight=2);
        G.add_edge(1, 3, weight=-2);
        G.add_edge(2, 3, weight=1);
        G.add_edge(2, 4, weight=-1);
        G.add_edge(3, 4, weight=-6);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({1: 2, 2: 1}));
        assert_edges_equal(xn::max_weight_matching(G, 1),
                           matching_dict_to_set({1: 3, 2: 4, 3: 1, 4: 2}));

    auto test_s_blossom() {
        /** Create S-blossom && use it for augmentation: */
        G = xn::Graph();
        G.add_weighted_edges_from([(1, 2, 8), (1, 3, 9),
                                   (2, 3, 10), (3, 4, 7)]);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({1: 2, 2: 1, 3: 4, 4: 3}));

        G.add_weighted_edges_from([(1, 6, 5), (4, 5, 6)]);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({1: 6, 2: 3, 3: 2, 4: 5, 5: 4, 6: 1}));

    auto test_s_t_blossom() {
        /** Create S-blossom, relabel as T-blossom, use for augmentation: */
        G = xn::Graph();
        G.add_weighted_edges_from([(1, 2, 9), (1, 3, 8), (2, 3, 10),
                                   (1, 4, 5), (4, 5, 4), (1, 6, 3)]);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({1: 6, 2: 3, 3: 2, 4: 5, 5: 4, 6: 1}));
        G.add_edge(4, 5, weight=3);
        G.add_edge(1, 6, weight=4);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({1: 6, 2: 3, 3: 2, 4: 5, 5: 4, 6: 1}));
        G.remove_edge(1, 6);
        G.add_edge(3, 6, weight=4);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({1: 2, 2: 1, 3: 6, 4: 5, 5: 4, 6: 3}));

    auto test_nested_s_blossom() {
        /** Create nested S-blossom, use for augmentation: */

        G = xn::Graph();
        G.add_weighted_edges_from([(1, 2, 9), (1, 3, 9), (2, 3, 10),
                                   (2, 4, 8), (3, 5, 8), (4, 5, 10),
                                   (5, 6, 6)]);
        assert_equal(xn::max_weight_matching(G),
                     matching_dict_to_set({1: 3, 2: 4, 3: 1, 4: 2, 5: 6, 6: 5}));

    auto test_nested_s_blossom_relabel() {
        /** Create S-blossom, relabel as S, include : nested S-blossom: */
        G = xn::Graph();
        G.add_weighted_edges_from([(1, 2, 10), (1, 7, 10), (2, 3, 12),
                                   (3, 4, 20), (3, 5, 20), (4, 5, 25),
                                   (5, 6, 10), (6, 7, 10), (7, 8, 8)]);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({1: 2, 2: 1, 3: 4, 4: 3, 5: 6, 6: 5, 7: 8, 8: 7}));

    auto test_nested_s_blossom_expand() {
        /** Create nested S-blossom, augment, expand recursively: */
        G = xn::Graph();
        G.add_weighted_edges_from([(1, 2, 8), (1, 3, 8), (2, 3, 10),
                                   (2, 4, 12), (3, 5, 12), (4, 5, 14),
                                   (4, 6, 12), (5, 7, 12), (6, 7, 14),
                                   (7, 8, 12)]);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({1: 2, 2: 1, 3: 5, 4: 6, 5: 3, 6: 4, 7: 8, 8: 7}));

    auto test_s_blossom_relabel_expand() {
        /** Create S-blossom, relabel as T, expand: */
        G = xn::Graph();
        G.add_weighted_edges_from([(1, 2, 23), (1, 5, 22), (1, 6, 15),
                                   (2, 3, 25), (3, 4, 22), (4, 5, 25),
                                   (4, 8, 14), (5, 7, 13)]);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({1: 6, 2: 3, 3: 2, 4: 8, 5: 7, 6: 1, 7: 5, 8: 4}));

    auto test_nested_s_blossom_relabel_expand() {
        /** Create nested S-blossom, relabel as T, expand: */
        G = xn::Graph();
        G.add_weighted_edges_from([(1, 2, 19), (1, 3, 20), (1, 8, 8),
                                   (2, 3, 25), (2, 4, 18), (3, 5, 18),
                                   (4, 5, 13), (4, 7, 7), (5, 6, 7)]);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({1: 8, 2: 3, 3: 2, 4: 7, 5: 6, 6: 5, 7: 4, 8: 1}));

    auto test_nasty_blossom1() {
        /** Create blossom, relabel as T : more than one way, expand,
        augment) {
         */
        G = xn::Graph();
        G.add_weighted_edges_from([(1, 2, 45), (1, 5, 45), (2, 3, 50),
                                   (3, 4, 45), (4, 5, 50), (1, 6, 30),
                                   (3, 9, 35), (4, 8, 35), (5, 7, 26),
                                   (9, 10, 5)]);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({1: 6, 2: 3, 3: 2, 4: 8, 5: 7,
                                                 6: 1, 7: 5, 8: 4, 9: 10, 10: 9}));

    auto test_nasty_blossom2() {
        /** Again but slightly different: */
        G = xn::Graph();
        G.add_weighted_edges_from([(1, 2, 45), (1, 5, 45), (2, 3, 50),
                                   (3, 4, 45), (4, 5, 50), (1, 6, 30),
                                   (3, 9, 35), (4, 8, 26), (5, 7, 40),
                                   (9, 10, 5)]);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({1: 6, 2: 3, 3: 2, 4: 8, 5: 7,
                                                 6: 1, 7: 5, 8: 4, 9: 10, 10: 9}));

    auto test_nasty_blossom_least_slack() {
        /** Create blossom, relabel as T, expand such that a new
        least-slack S-to-free dge is produced, augment) {
         */
        G = xn::Graph();
        G.add_weighted_edges_from([(1, 2, 45), (1, 5, 45), (2, 3, 50),
                                   (3, 4, 45), (4, 5, 50), (1, 6, 30),
                                   (3, 9, 35), (4, 8, 28), (5, 7, 26),
                                   (9, 10, 5)]);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({1: 6, 2: 3, 3: 2, 4: 8, 5: 7,
                                                 6: 1, 7: 5, 8: 4, 9: 10, 10: 9}));

    auto test_nasty_blossom_augmenting() {
        /** Create nested blossom, relabel as T : more than one way */
        // expand outer blossom such that inner blossom ends up on an
        // augmenting path) {
        G = xn::Graph();
        G.add_weighted_edges_from([(1, 2, 45), (1, 7, 45), (2, 3, 50),
                                   (3, 4, 45), (4, 5, 95), (4, 6, 94),
                                   (5, 6, 94), (6, 7, 50), (1, 8, 30),
                                   (3, 11, 35), (5, 9, 36), (7, 10, 26),
                                   (11, 12, 5)]);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({1: 8, 2: 3, 3: 2, 4: 6, 5: 9, 6: 4,
                                                 7: 10, 8: 1, 9: 5, 10: 7, 11: 12, 12: 11}));

    auto test_nasty_blossom_expand_recursively() {
        /** Create nested S-blossom, relabel as S, expand recursively: */
        G = xn::Graph();
        G.add_weighted_edges_from([(1, 2, 40), (1, 3, 40), (2, 3, 60),
                                   (2, 4, 55), (3, 5, 55), (4, 5, 50),
                                   (1, 8, 15), (5, 7, 30), (7, 6, 10),
                                   (8, 10, 10), (4, 9, 30)]);
        assert_edges_equal(xn::max_weight_matching(G),
                           matching_dict_to_set({1: 2, 2: 1, 3: 5, 4: 9, 5: 3,
                                                 6: 7, 7: 6, 8: 10, 9: 4, 10: 8}));


class TestIsMatching: public object {
    /** Unit tests for the
    :func:`~xnetwork.algorithms.matching.is_matching` function.

     */

    auto test_dict() {
        G = xn::path_graph(4);
        assert_true(xn::is_matching(G, {0: 1, 1: 0, 2: 3, 3: 2}));

    auto test_empty_matching() {
        G = xn::path_graph(4);
        assert_true(xn::is_matching(G, set()));

    auto test_single_edge() {
        G = xn::path_graph(4);
        assert_true(xn::is_matching(G, {(1, 2)}));

    auto test_edge_order() {
        G = xn::path_graph(4);
        assert_true(xn::is_matching(G, {(0, 1), (2, 3)}));
        assert_true(xn::is_matching(G, {(1, 0), (2, 3)}));
        assert_true(xn::is_matching(G, {(0, 1), (3, 2)}));
        assert_true(xn::is_matching(G, {(1, 0), (3, 2)}));

    auto test_valid() {
        G = xn::path_graph(4);
        assert_true(xn::is_matching(G, {(0, 1), (2, 3)}));

    auto test_invalid() {
        G = xn::path_graph(4);
        assert_false(xn::is_matching(G, {(0, 1), (1, 2), (2, 3)}));


class TestIsMaximalMatching: public object {
    /** Unit tests for the
    :func:`~xnetwork.algorithms.matching.is_maximal_matching` function.

     */

    auto test_dict() {
        G = xn::path_graph(4);
        assert_true(xn::is_maximal_matching(G, {0: 1, 1: 0, 2: 3, 3: 2}));

    auto test_valid() {
        G = xn::path_graph(4);
        assert_true(xn::is_maximal_matching(G, {(0, 1), (2, 3)}));

    auto test_not_matching() {
        G = xn::path_graph(4);
        assert_false(xn::is_maximal_matching(G, {(0, 1), (1, 2), (2, 3)}));

    auto test_not_maximal() {
        G = xn::path_graph(4);
        assert_false(xn::is_maximal_matching(G, {(0, 1)}));


class TestIsPerfectMatching: public object {
    /** Unit tests for the
    :func:`~xnetwork.algorithms.matching.is_perfect_matching` function.

     */

    auto test_dict() {
        G = xn::path_graph(4);
        assert_true(xn::is_perfect_matching(G, {0: 1, 1: 0, 2: 3, 3: 2}));

    auto test_valid() {
        G = xn::path_graph(4);
        assert_true(xn::is_perfect_matching(G, {(0, 1), (2, 3)}));

    auto test_valid_not_path() {
        G = xn::cycle_graph(4);
        G.add_edge(0, 4);
        G.add_edge(1, 4);
        G.add_edge(5, 2);

        assert_true(xn::is_perfect_matching(G, {(1, 4), (0, 3), (5, 2)}));

    auto test_not_matching() {
        G = xn::path_graph(4);
        assert_false(xn::is_perfect_matching(G, {(0, 1), (1, 2), (2, 3)}));

    auto test_maximal_but_not_perfect() {
        G = xn::cycle_graph(4);
        G.add_edge(0, 4);
        G.add_edge(1, 4);

        assert_false(xn::is_perfect_matching(G, {(1, 4), (0, 3)}));


class TestMaximalMatching: public object {
    /** Unit tests for the
    :func:`~xnetwork.algorithms.matching.maximal_matching`.

     */

    auto test_valid_matching() {
        edges = [(1, 2), (1, 5), (2, 3), (2, 5), (3, 4), (3, 6), (5, 6)];
        G = xn::Graph(edges);
        matching = xn::maximal_matching(G);
        assert_true(xn::is_maximal_matching(G, matching));

    auto test_single_edge_matching() {
        // In the star graph, any maximal matching has just one edge.
        G = xn::star_graph(5);
        matching = xn::maximal_matching(G);
        assert_equal(1, len(matching));
        assert_true(xn::is_maximal_matching(G, matching));

    auto test_self_loops() {
        // Create the path graph with two self-loops.
        G = xn::path_graph(3);
        G.add_edges_from([(0, 0), (1, 1)]);
        matching = xn::maximal_matching(G);
        assert_equal(len(matching), 1);
        // The matching should never include self-loops.
        assert_false(any(u == v for u, v : matching));
        assert_true(xn::is_maximal_matching(G, matching));

    auto test_ordering() {
        /** Tests that a maximal matching is computed correctly
        regardless of the order : which nodes are added to the graph.

         */
        for (auto nodes : permutations(range(3)) {
            G = xn::Graph();
            G.add_nodes_from(nodes);
            G.add_edges_from([(0, 1), (0, 2)]);
            matching = xn::maximal_matching(G);
            assert_equal(len(matching), 1);
            assert_true(xn::is_maximal_matching(G, matching));
