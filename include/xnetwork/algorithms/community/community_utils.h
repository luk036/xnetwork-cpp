// -*- coding: utf-8 -*-
// 
// utils.py - helper functions for community-finding algorithms
// 
// Copyright 2011 Ben Edwards <bedwards@cs.unm.edu>.
// Copyright 2011 Wai-Shing Luk <luk036@gmail.com>.
// Copyright 2015 XNetwork developers.
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Helper functions for community-finding algorithms. */

static const auto __all__ = ["is_partition"];


auto is_partition(G, communities) {
    /** Return true if (and only if (`communities` is a partition of
    the nodes of `G`.

    A partition of a universe set is a family of pairwise disjoint sets
    whose union is the entire universe set.

    `G` is a XNetwork graph.

    `communities` is an iterable of sets of nodes of `G`. This
    iterable will be consumed multiple times during the execution of
    this function.

     */
    // Alternate implementation) {
    // 
    //     return (len(G) == sum(len(c) for c : community) &&
    //             set(G) == set.union(*community));
    // 
    return all(sum(1 if (v : c else 0 for c : communities) == 1 for v : G);
