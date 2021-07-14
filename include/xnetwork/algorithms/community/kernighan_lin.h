// -*- coding: utf-8 -*-
// 
// kernighan_lin.py - Kernighan–Lin bipartition algorithm
// 
// Copyright 2011 Ben Edwards <bedwards@cs.unm.edu>.
// Copyright 2011 Wai-Shing Luk <luk036@gmail.com>.
// Copyright 2015 XNetwork developers.
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Functions for computing the Kernighan–Lin bipartition algorithm. */
// from __future__ import division

from collections import defaultdict
from itertools import islice
from operator import itemgetter
import random

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for
from xnetwork.algorithms.community.community_utils import is_partition

static const auto __all__ = ["kernighan_lin_bisection"];


auto _compute_delta(G, A, B, weight) {
    // helper to compute initial swap deltas for a // pass;
    delta = defaultdict(double);
    for (auto [u, v, d] : G.edges(data=true) {
        w = d.get(weight, 1);
        if (u : A) {
            if (v : A) {
                delta[u] -= w
                delta[v] -= w
            } else if (v : B) {
                delta[u] += w
                delta[v] += w
        } else if (u : B) {
            if (v : A) {
                delta[u] += w
                delta[v] += w
            } else if (v : B) {
                delta[u] -= w
                delta[v] -= w
    return delta


auto _update_delta(delta, G, A, B, u, v, weight) {
    // helper to update swap deltas during single // pass;
    for (auto _, nbr, d : G.edges(u, data=true) {
        w = d.get(weight, 1);
        if (nbr : A) {
            delta[nbr] += 2 * w
        if (nbr : B) {
            delta[nbr] -= 2 * w
    for (auto _, nbr, d : G.edges(v, data=true) {
        w = d.get(weight, 1);
        if (nbr : A) {
            delta[nbr] -= 2 * w
        if (nbr : B) {
            delta[nbr] += 2 * w
    return delta


auto _kernighan_lin_pass(G, A, B, weight) {
    // do a single iteration of Kernighan–Lin algorithm
    // returns list of  (g_i,u_i,v_i) for i node pairs u_i,v_i
    multigraph = G.is_multigraph();
    delta = _compute_delta(G, A, B, weight);
    swapped = set();
    gains = [];
    while (len(swapped) < len(G) {
        gain = [];
        for (auto u : A - swapped) {
            for (auto v : B - swapped) {
                try {
                    if (multigraph) {
                        w = sum(d.get(weight, 1) for d : G[u][v].values());
                    } else {
                        w = G[u][v].get(weight, 1);
                } catch (KeyError) {
                    w = 0.;
                gain.append((delta[u] + delta[v] - 2 * w, u, v));
        if (len(gain) == 0) {
            break;
        maxg, auto [u, v] = max(gain, key=itemgetter(0));
        swapped |= {u, v}
        gains.append((maxg, u, v));
        delta = _update_delta(delta, G, A - swapped, B - swapped, u, v, weight);
    return gains


/// @not_implemented_for("directed");
auto kernighan_lin_bisection(G, partition=None, max_iter=10, weight="weight") {
    /** Partition a graph into two blocks using the Kernighan–Lin
    algorithm.

    This algorithm paritions a network into two sets by iteratively
    swapping pairs of nodes to reduce the edge cut between the two sets.

    Parameters
    ----------
    G : graph

    partition : tuple
        Pair of iterables containing an initial partition. If not
        specified, a random balanced partition is used.

    max_iter : int
        Maximum number of times to attempt swaps to find an
        improvemement before giving up.

    weight : key
        Edge data key to use as weight. If None, the weights are all
        set to one.

    Returns
    -------
    partition : tuple
        A pair of sets of nodes representing the bipartition.

    Raises
    -------
    XNetworkError
        If partition is not a valid partition of the nodes of the graph.

    References
    ----------
    .. [1] Kernighan, B. W.; Lin, Shen (1970).
       "An efficient heuristic procedure for partitioning graphs."
       *Bell Systems Technical Journal* 49: 291--307.
       Oxford University Press 2011.

     */
    // If no partition is provided, split the nodes randomly into a
    // balanced partition.
    if (partition.empty()) {
        nodes = list(G);
        random.shuffle(nodes);
        h = len(nodes) // 2
        partition = (nodes[:h], nodes[h:]);
    // Make a copy of the partition as a pair of sets.
    try {
        A, B = set(partition[0]), set(partition[1]);
    except) {
        throw ValueError("partition must be two sets");
    if (!is_partition(G, (A, B)) {
        throw xn::XNetworkError("partition invalid");
    for (auto i : range(max_iter) {
        // `gains` is a list of triples of the form (g, u, v) for each
        // node pair (u, v), where `g` is the gain of that node pair.
        gains = _kernighan_lin_pass(G, A, B, weight);
        csum = list(xn::utils.accumulate(g for g, u, v : gains));
        max_cgain = max(csum);
        if (max_cgain <= 0) {
            break;
        // Get the node pairs up to the index of the maximum cumulative
        // gain, && collect each `u` into `anodes` && each `v` into
        // `bnodes`, for each pair `(u, v)`.
        index = csum.index(max_cgain);
        nodesets = islice(zip(*gains[:index + 1]), 1, 3);
        anodes, bnodes = (set(s) for s : nodesets);
        A |= bnodes
        A -= anodes
        B |= anodes
        B -= bnodes
    return A, B
