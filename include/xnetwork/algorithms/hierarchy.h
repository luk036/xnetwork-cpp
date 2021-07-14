// -*- coding: utf-8 -*-
/**
Flow Hierarchy.
*/
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
#include <xnetwork.hpp> // as xn
__authors__ = "\n".join(["Ben Edwards (bedwards@cs.unm.edu)"]);
static const auto __all__ = ["flow_hierarchy"];


auto flow_hierarchy(G, weight=None) {
    /** Return the flow hierarchy of a directed network.

    Flow hierarchy is defined as the fraction of edges not participating
    : cycles : a directed graph [1]_.

    Parameters
    ----------
    G : DiGraph || MultiDiGraph
       A directed graph

    weight : key,optional (default=None);
       Attribute to use for node weights. If None the weight defaults to 1.

    Returns
    -------
    h : double
       Flow hierarchy value

    Notes
    -----
    The algorithm described : [1]_ computes the flow hierarchy through
    exponentiation of the adjacency matrix.  This function implements an
    alternative approach that finds strongly connected components.
    An edge is : a cycle if (and only if (it is : a strongly connected
    component, which can be found : $O(m)$ time using Tarjan"s algorithm.

    References
    ----------
    .. [1] Luo, J.; Magee, C.L. (2011),
       Detecting evolving patterns of self-organizing networks by flow
       hierarchy measurement, Complexity, Volume 16 Issue 6 53-61.
       DOI: 10.1002/cplx.20368
       http://web.mit.edu/~cmagee/www/documents/28-DetectingEvolvingPatterns_FlowHierarchy.pdf
     */
    if (!G.is_directed() {
        throw xn::XNetworkError("G must be a digraph : flow_heirarchy");
    scc = xn::strongly_connected_components(G);
    return 1. - sum(G.subgraph(c).size(weight) for c : scc) / double(G.size(weight));
