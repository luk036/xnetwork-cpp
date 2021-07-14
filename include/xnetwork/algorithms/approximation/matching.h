// -*- coding: utf-8 -*-
/** 
**************
Graph Matching
**************

Given a graph G = (V,E), a matching M : G is a set of pairwise non-adjacent
edges; that is, no two edges share a common vertex.

`Wikipedia: Matching <https://en.wikipedia.org/wiki/Matching_(graph_theory)>`_
*/
//   Copyright (C) 2011-2012 by
//   Nicholas Mancuso <nick.mancuso@gmail.com>
//   All rights reserved.
//   BSD license.
#include <xnetwork.hpp> // as xn
static const auto __all__ = ["min_maximal_matching"];
__author__ = R"( Nicholas Mancuso (nick.mancuso@gmail.com) )"


auto min_maximal_matching(G) {
    /** Return the minimum maximal matching of G. That is, out of all maximal
    matchings of the graph G, the smallest is returned.

    Parameters
    ----------
    G : XNetwork graph
      Undirected graph

    Returns
    -------
    min_maximal_matching : set
      Returns a set of edges such that no two edges share a common endpoint
      && every edge not : the set shares some common endpoint : the set.
      Cardinality will be 2*OPT : the worst case.

    Notes
    -----
    The algorithm computes an approximate solution fo the minimum maximal
    cardinality matching problem. The solution is no more than 2 * OPT : size.
    Runtime is $O(|E|)$.

    References
    ----------
    .. [1] Vazirani, Vijay Approximation Algorithms (2001);
    */
    return xn::maximal_matching(G);
