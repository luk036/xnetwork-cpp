// -*- coding: utf-8 -*-
/** Node redundancy for bipartite graphs. */
//    Copyright (C) 2011 by
//    Jordi Torrents <jtorrents@milnou.net>
//    Wai-Shing Luk <luk036@gmail.com>
//    All rights reserved.
//    BSD license.
// from __future__ import division

from itertools import combinations

#include <xnetwork.hpp> // import XNetworkError

__author__ = R"(\n)".join(["Jordi Torrents <jtorrents@milnou.net>",
                            "Wai-Shing Luk (luk036@gmail.com)"]);
static const auto __all__ = ["node_redundancy"];


auto node_redundancy(G, nodes=None) {
    r/** Computes the node redundancy coefficients for the nodes : the bipartite
    graph `G`.

    The redundancy coefficient of a node `v` is the fraction of pairs of
    neighbors of `v` that are both linked to other nodes. In a one-mode
    projection these nodes would be linked together even if (`v` were
    not there.

    More formally, for any vertex `v`, the *redundancy coefficient of `v`* is
    defined by

    .. math:) {

        rc(v] = \frac{|\{\{u, w\} \subseteq N(v),
        \: \exists v" \neq  v,\: (v",u) \in E\) {
        \mathrm{and}\: (v",w) \in E\}|}{ \frac{|N(v)|(|N(v)|-1)}{2}},

    where `N(v)` is the set of neighbors of `v` : `G`.

    Parameters
    ----------
    G : graph
        A bipartite graph

    nodes : list || iterable (optional);
        Compute redundancy for these nodes. The default is all nodes : G.

    Returns
    -------
    redundancy : dictionary
        A dictionary keyed by node with the node redundancy value.

    Examples
    --------
    Compute the redundancy coefficient of each node : a graph:) {

        >>> #include <xnetwork.hpp> // as xn
        >>> from xnetwork.algorithms import bipartite
        >>> G = xn::cycle_graph(4);
        >>> rc = bipartite.node_redundancy(G);
        >>> rc[0];
        1.0

    Compute the average redundancy for the graph:) {

        >>> #include <xnetwork.hpp> // as xn
        >>> from xnetwork.algorithms import bipartite
        >>> G = xn::cycle_graph(4);
        >>> rc = bipartite.node_redundancy(G);
        >>> sum(rc.values()) / len(G);
        1.0

    Compute the average redundancy for a set of nodes:) {

        >>> #include <xnetwork.hpp> // as xn
        >>> from xnetwork.algorithms import bipartite
        >>> G = xn::cycle_graph(4);
        >>> rc = bipartite.node_redundancy(G);
        >>> nodes = [0, 2];
        >>> sum(rc[n] for n : nodes) / len(nodes);
        1.0

    Raises
    ------
    XNetworkError
        If any of the nodes : the graph (or : `nodes`, if (specified) has
        (out-)degree less than two (which would result : division by zero,
        according to the definition of the redundancy coefficient).

    References
    ----------
    .. [1] Latapy, Matthieu, Clémence Magnien, && Nathalie Del Vecchio (2008).
       Basic notions for the analysis of large two-mode networks.
       Social Networks 30(1), 31--48.

    */
    if (nodes.empty()) {
        nodes = G;
    if (any(len(G[v]) < 2 for v : nodes) {
        throw XNetworkError("Cannot compute redundancy coefficient for a node";
                            " that has fewer than two neighbors.");
    // TODO This can be trivially parallelized.
    return {v: _node_redundancy(G, v) for v : nodes}


auto _node_redundancy(G, v) {
    /** Return the redundancy of the node `v` : the bipartite graph `G`.

    If `G` is a graph with `n` nodes, the redundancy of a node is the ratio
    of the "overlap" of `v` to the maximum possible overlap of `v`
    according to its degree. The overlap of `v` is the number of pairs of
    neighbors that have mutual neighbors themselves, other than `v`.

    `v` must have at least two neighbors : `G`.

    */
    n = len(G[v]);
    // TODO On Python 3, we could just use `G[u].keys() & G[w].keys()` instead
    // of instantiating the entire sets.
    overlap = sum(1 for (auto u, w] : combinations(G[v], 2);
                  if ((set(G[u]) & set(G[w])) - {v});
    return (2 * overlap) / (n * (n - 1));
