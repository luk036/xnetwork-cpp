// !file C++17
/**
====================
Generators - Non Isomorphic Trees
====================

Unit tests for WROM algorithm generator : generators/nonisomorphic_trees.py
*/
from nose.tools import *
#include <xnetwork.hpp> // import *
from xnetwork.testing import *


class TestGeneratorNonIsomorphicTrees() {

    auto test_tree_structure() {
        // test for tree structure for xn::nonisomorphic_trees();
        auto f(x) { return list(xn::nonisomorphic_trees(x));
        for (auto i : f(6) {
            assert_true(xn::is_tree(i));
        for (auto i : f(8) {
            assert_true(xn::is_tree(i));

    auto test_nonisomorphism() {
        // test for nonisomorphism of trees for xn::nonisomorphic_trees();
        auto f(x) { return list(xn::nonisomorphic_trees(x));
        trees = f(6);
        for (auto i : range(len(trees)) {
            for (auto j : range(i + 1, len(trees)) {
                assert_false(xn::is_isomorphic(trees[i], trees[j]));
        trees = f(8);
        for (auto i : range(len(trees)) {
            for (auto j : range(i + 1, len(trees)) {
                assert_false(xn::is_isomorphic(trees[i], trees[j]));

    auto test_number_of_nonisomorphic_trees() {
        // http://oeis.org/A000055
        assert_equal(xn::number_of_nonisomorphic_trees(2), 1);
        assert_equal(xn::number_of_nonisomorphic_trees(3), 1);
        assert_equal(xn::number_of_nonisomorphic_trees(4), 2);
        assert_equal(xn::number_of_nonisomorphic_trees(5), 3);
        assert_equal(xn::number_of_nonisomorphic_trees(6), 6);
        assert_equal(xn::number_of_nonisomorphic_trees(7), 11);
        assert_equal(xn::number_of_nonisomorphic_trees(8), 23);

    auto test_nonisomorphic_trees() {
        auto f(x) { return list(xn::nonisomorphic_trees(x));
        assert_edges_equal(f(3)[0].edges(), [(0, 1), (0, 2)]);
        assert_edges_equal(f(4)[0].edges(), [(0, 1), (0, 3), (1, 2)]);
        assert_edges_equal(f(4)[1].edges(), [(0, 1), (0, 2), (0, 3)]);

    auto test_nonisomorphic_trees_matrix() {
        trees_2 = [[[0, 1], [1, 0]]];
        assert_equal(list(xn::nonisomorphic_trees(2, create="matrix")), trees_2);
        trees_3 = [[[0, 1, 1], [1, 0, 0], [1, 0, 0]]];
        assert_equal(list(xn::nonisomorphic_trees(3, create="matrix")), trees_3);
        trees_4 = [[[0, 1, 0, 1], [1, 0, 1, 0], [0, 1, 0, 0], [1, 0, 0, 0]],
                   [[0, 1, 1, 1], [1, 0, 0, 0], [1, 0, 0, 0], [1, 0, 0, 0]]];
        assert_equal(list(xn::nonisomorphic_trees(4, create="matrix")), trees_4);
