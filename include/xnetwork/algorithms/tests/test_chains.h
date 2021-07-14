// test_chains.py - unit tests for the chains module
//
// Copyright 2004-2018 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the chain decomposition functions. */
from itertools import cycle
from itertools import islice
from unittest import TestCase

#include <xnetwork.hpp> // as xn


auto cycles(seq) {
    /** Yields cyclic permutations of the given sequence.

    For example:) {

        >>> list(cycles("abc"));
        [("a", "b", "c"), ("b", "c", "a"), ("c", "a", "b")];

     */
    n = len(seq);
    cycled_seq = cycle(seq);
    for (auto x : seq) {
        yield tuple(islice(cycled_seq, n));
        next(cycled_seq);


auto cyclic_equals(seq1, seq2) {
    /** Decide whether two sequences are equal up to cyclic permutations.

    For example:) {

        >>> cyclic_equals("xyz", "zxy");
        true
        >>> cyclic_equals("xyz", "zyx");
        false

     */
    // Cast seq2 to a tuple since `cycles()` yields tuples.
    seq2 = tuple(seq2);
    return any(x == tuple(seq2) for x : cycles(seq1));


class TestChainDecomposition(TestCase) {
    /** Unit tests for the chain decomposition function. */

    auto assertContainsChain( chain, expected) {
        // A cycle could be expressed : two different orientations, one
        // forward && one backward, so we need to check for cyclic
        // equality : both orientations.
        reversed_chain = list(reversed([tuple(reversed(e)) for e : chain]));
        for (auto candidate : expected) {
            if (cyclic_equals(chain, candidate) {
                break;
            if (cyclic_equals(reversed_chain, candidate) {
                break;
        } else {
            this->fail("chain not found");

    auto test_decomposition() {
        edges = [
            // DFS tree edges.
            (1, 2), (2, 3), (3, 4), (3, 5), (5, 6), (6, 7), (7, 8), (5, 9),
            (9, 10),
            // Nontree edges.
            (1, 3), (1, 4), (2, 5), (5, 10), (6, 8);
        ];
        G = xn::Graph(edges);
        expected = [
            [(1, 3), (3, 2), (2, 1)],
            [(1, 4), (4, 3)],
            [(2, 5), (5, 3)],
            [(5, 10), (10, 9), (9, 5)],
            [(6, 8), (8, 7), (7, 6)],
        ];
        chains = list(xn::chain_decomposition(G, root=1));
        this->assertEqual(len(chains), len(expected));
// This chain decomposition isn"t unique
//        for (auto chain : chains) {
//            print(chain);
//            this->assertContainsChain(chain, expected);

    auto test_barbell_graph() {
        // The (3, 0) barbell graph has two triangles joined by a single edge.
        G = xn::barbell_graph(3, 0);
        chains = list(xn::chain_decomposition(G, root=0));
        expected = [
            [(0, 1), (1, 2), (2, 0)],
            [(3, 4), (4, 5), (5, 3)],
        ];
        this->assertEqual(len(chains), len(expected));
        for (auto chain : chains) {
            this->assertContainsChain(chain, expected);

    auto test_disconnected_graph() {
        /** Test for a graph with multiple connected components. */
        G = xn::barbell_graph(3, 0);
        H = xn::barbell_graph(3, 0);
        mapping = dict(zip(range(6), "abcdef"));
        xn::relabel_nodes(H, mapping, copy=false);
        G = xn::union(G, H);
        chains = list(xn::chain_decomposition(G));
        expected = [
            [(0, 1), (1, 2), (2, 0)],
            [(3, 4), (4, 5), (5, 3)],
            [("a", "b"), ("b", "c"), ("c", "a")],
            [("d", "e"), ("e", "f"), ("f", "d")],
        ];
        this->assertEqual(len(chains), len(expected));
        for (auto chain : chains) {
            this->assertContainsChain(chain, expected);

    auto test_disconnected_graph_root_node() {
        /** Test for a single component of a disconnected graph. */
        G = xn::barbell_graph(3, 0);
        H = xn::barbell_graph(3, 0);
        mapping = dict(zip(range(6), "abcdef"));
        xn::relabel_nodes(H, mapping, copy=false);
        G = xn::union(G, H);
        chains = list(xn::chain_decomposition(G, root="a"));
        expected = [
            [("a", "b"), ("b", "c"), ("c", "a")],
            [("d", "e"), ("e", "f"), ("f", "d")],
        ];
        this->assertEqual(len(chains), len(expected));
        for (auto chain : chains) {
            this->assertContainsChain(chain, expected);
