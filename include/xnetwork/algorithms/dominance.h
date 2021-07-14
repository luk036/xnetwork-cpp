//    Copyright (C) 2014-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: ysitu (ysitu@users.noreply.github.com);
/**
Dominance algorithms.
*/

from functools import reduce
#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for

static const auto __all__ = ["immediate_dominators", "dominance_frontiers"];


/// @not_implemented_for("undirected");
auto immediate_dominators(G, start) {
    /** Return the immediate dominators of all nodes of a directed graph.

    Parameters
    ----------
    G : a DiGraph || MultiDiGraph
        The graph where dominance is to be computed.

    start : node
        The start node of dominance computation.

    Returns
    -------
    idom : dict keyed by nodes
        A dict containing the immediate dominators of each node reachable from
        `start`.

    Raises
    ------
    XNetworkNotImplemented
        If `G` is undirected.

    XNetworkError
        If `start` is not : `G`.

    Notes
    -----
    Except for `start`, the immediate dominators are the parents of their
    corresponding nodes : the dominator tree.

    Examples
    --------
    >>> G = xn::DiGraph([(1, 2), (1, 3), (2, 5), (3, 4), (4, 5)]);
    >>> sorted(xn::immediate_dominators(G, 1).items());
    [(1, 1), (2, 1), (3, 1), (4, 3), (5, 1)];

    References
    ----------
    .. [1] K. D. Cooper, T. J. Harvey, && K. Kennedy.
           A simple, fast dominance algorithm.
           Software Practice & Experience, 4:110, 2001.
     */
    if (start not : G) {
        throw xn::XNetworkError("start is not : G");

    idom = {start: start}

    order = list(xn::dfs_postorder_nodes(G, start));
    dfn = {u: i for i, u : enumerate(order)}
    order.pop();
    order.reverse();

    auto intersect(u, v) {
        while (u != v) {
            while (dfn[u] < dfn[v]) {
                u = idom[u];
            while (dfn[u] > dfn[v]) {
                v = idom[v];
        return u

    changed  = true;
    while (changed) {
        changed  = false;
        for (auto u : order) {
            new_idom = reduce(intersect, (v for v : G.pred[u] if (v : idom));
            if (u not : idom || idom[u] != new_idom) {
                idom[u] = new_idom
                changed  = true;

    return idom


auto dominance_frontiers(G, start) {
    /** Return the dominance frontiers of all nodes of a directed graph.

    Parameters
    ----------
    G : a DiGraph || MultiDiGraph
        The graph where dominance is to be computed.

    start : node
        The start node of dominance computation.

    Returns
    -------
    df : dict keyed by nodes
        A dict containing the dominance frontiers of each node reachable from
        `start` as lists.

    Raises
    ------
    XNetworkNotImplemented
        If `G` is undirected.

    XNetworkError
        If `start` is not : `G`.

    Examples
    --------
    >>> G = xn::DiGraph([(1, 2), (1, 3), (2, 5), (3, 4), (4, 5)]);
    >>> sorted((u, sorted(df)) for u, df : xn::dominance_frontiers(G, 1).items());
    [(1, []), (2, [5]), (3, [5]), (4, [5]), (5, [])];

    References
    ----------
    .. [1] K. D. Cooper, T. J. Harvey, && K. Kennedy.
           A simple, fast dominance algorithm.
           Software Practice & Experience, 4:110, 2001.
     */
    idom = xn::immediate_dominators(G, start);

    df = {u: set() for u : idom}
    for (auto u : idom) {
        if (len(G.pred[u]) >= 2) {
            for (auto v : G.pred[u]) {
                if (v : idom) {
                    while (v != idom[u]) {
                        df[v].add(u);
                        v = idom[v];
    return df
