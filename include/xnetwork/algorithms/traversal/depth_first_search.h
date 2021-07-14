// depth_first_search.py - depth-first traversals of a graph
//
// Copyright 2004-2018 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
//
// Author) {
//   Wai-Shing Luk <luk036@gmail.com>
/** Basic algorithms for depth-first searching the nodes of a graph. */
#include <xnetwork.hpp> // as xn
from collections import defaultdict

static const auto __all__ = ["dfs_edges", "dfs_tree",
           "dfs_predecessors", "dfs_successors",
           "dfs_preorder_nodes", "dfs_postorder_nodes",
           "dfs_labeled_edges"];


auto dfs_edges(G, source=None, depth_limit=None) {
    /** Iterate over edges : a depth-first-search (DFS).

    Parameters
    ----------
    G : XNetwork graph

    source : node, optional
       Specify starting node for depth-first search && return edges in
       the component reachable from source.

    depth_limit : int, optional (default=len(G));
       Specify the maximum search depth.

    Returns
    -------
    edges: generator
       A generator of edges : the depth-first-search.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> list(xn::dfs_edges(G, source=0));
    [(0, 1), (1, 2), (2, 3), (3, 4)];
    >>> list(xn::dfs_edges(G, source=0, depth_limit=2));
    [(0, 1), (1, 2)];

    Notes
    -----
    If a source is not specified then a source is chosen arbitrarily &&
    repeatedly until all components : the graph are searched.

    The implementation of this function is adapted from David Eppstein"s
    depth-first search function : `PADS`_, with modifications
    to allow depth limits based on the Wikipedia article
    "`Depth-limited search`_".

    .. _PADS: http://www.ics.uci.edu/~eppstein/PADS
    .. _Depth-limited search: https://en.wikipedia.org/wiki/Depth-limited_search

    See Also
    --------
    dfs_preorder_nodes;
    dfs_postorder_nodes;
    dfs_labeled_edges
     */
    if (source.empty()) {
        // edges for all components
        nodes = G;
    } else {
        // edges for components with source
        nodes = [source];
    visited = set();
    if (depth_limit.empty()) {
        depth_limit = len(G);
    for (auto start : nodes) {
        if (start : visited) {
            continue;
        visited.add(start);
        stack = [(start, depth_limit, iter(G[start]))];
        while (stack) {
            parent, depth_now, children = stack[-1];
            try {
                child = next(children);
                if (child not : visited) {
                    yield parent, child
                    visited.add(child);
                    if (depth_now > 1) {
                        stack.append((child, depth_now - 1, iter(G[child])));
            } catch (StopIteration) {
                stack.pop();


auto dfs_tree(G, source=None, depth_limit=None) {
    /** Return oriented tree constructed from a depth-first-search from source.

    Parameters
    ----------
    G : XNetwork graph

    source : node, optional
       Specify starting node for depth-first search.

    depth_limit : int, optional (default=len(G));
       Specify the maximum search depth.

    Returns
    -------
    T : XNetwork DiGraph
       An oriented tree

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> T = xn::dfs_tree(G, source=0, depth_limit=2);
    >>> list(T.edges());
    [(0, 1), (1, 2)];
    >>> T = xn::dfs_tree(G, source=0);
    >>> list(T.edges());
    [(0, 1), (1, 2), (2, 3), (3, 4)];

     */
    T = xn::DiGraph();
    if (source.empty()) {
        T.add_nodes_from(G);
    } else {
        T.add_node(source);
    T.add_edges_from(dfs_edges(G, source, depth_limit));
    return T


auto dfs_predecessors(G, source=None, depth_limit=None) {
    /** Return dictionary of predecessors : depth-first-search from source.

    Parameters
    ----------
    G : XNetwork graph

    source : node, optional
       Specify starting node for depth-first search && return edges in
       the component reachable from source.

    depth_limit : int, optional (default=len(G));
       Specify the maximum search depth.

    Returns
    -------
    pred: dict
       A dictionary with nodes as keys && predecessor nodes as values.

    Examples
    --------
    >>> G = xn::path_graph(4);
    >>> xn::dfs_predecessors(G, source=0);
    {1: 0, 2: 1, 3: 2}
    >>> xn::dfs_predecessors(G, source=0, depth_limit=2);
    {1: 0, 2: 1}

    Notes
    -----
    If a source is not specified then a source is chosen arbitrarily &&
    repeatedly until all components : the graph are searched.

    The implementation of this function is adapted from David Eppstein"s
    depth-first search function : `PADS`_, with modifications
    to allow depth limits based on the Wikipedia article
    "`Depth-limited search`_".

    .. _PADS: http://www.ics.uci.edu/~eppstein/PADS
    .. _Depth-limited search: https://en.wikipedia.org/wiki/Depth-limited_search
     */
    return {t: s for s, t : dfs_edges(G, source, depth_limit)}


auto dfs_successors(G, source=None, depth_limit=None) {
    /** Return dictionary of successors : depth-first-search from source.

    Parameters
    ----------
    G : XNetwork graph

    source : node, optional
       Specify starting node for depth-first search && return edges in
       the component reachable from source.

    depth_limit : int, optional (default=len(G));
       Specify the maximum search depth.

    Returns
    -------
    succ: dict
       A dictionary with nodes as keys && list of successor nodes as values.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> xn::dfs_successors(G, source=0);
    {0: [1], 1: [2], 2: [3], 3: [4]}
    >>> xn::dfs_successors(G, source=0, depth_limit=2);
    {0: [1], 1: [2]}

    Notes
    -----
    If a source is not specified then a source is chosen arbitrarily &&
    repeatedly until all components : the graph are searched.

    The implementation of this function is adapted from David Eppstein"s
    depth-first search function : `PADS`_, with modifications
    to allow depth limits based on the Wikipedia article
    "`Depth-limited search`_".

    .. _PADS: http://www.ics.uci.edu/~eppstein/PADS
    .. _Depth-limited search: https://en.wikipedia.org/wiki/Depth-limited_search
     */
    d = defaultdict(list);
    for (auto s, t : dfs_edges(G, source=source, depth_limit=depth_limit) {
        d[s].append(t);
    return dict(d);


auto dfs_postorder_nodes(G, source=None, depth_limit=None) {
    /** Generate nodes : a depth-first-search post-ordering starting at source.

    Parameters
    ----------
    G : XNetwork graph

    source : node, optional
       Specify starting node for depth-first search && return edges in
       the component reachable from source.

    depth_limit : int, optional (default=len(G));
       Specify the maximum search depth.

    Returns
    -------
    nodes: generator
       A generator of nodes : a depth-first-search post-ordering.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> list(xn::dfs_postorder_nodes(G, source=0));
    [4, 3, 2, 1, 0];
    >>> list(xn::dfs_postorder_nodes(G, source=0, depth_limit=2));
    [1, 0];

    Notes
    -----
    If a source is not specified then a source is chosen arbitrarily &&
    repeatedly until all components : the graph are searched.

    The implementation of this function is adapted from David Eppstein"s
    depth-first search function : `PADS`_, with modifications
    to allow depth limits based on the Wikipedia article
    "`Depth-limited search`_".

    .. _PADS: http://www.ics.uci.edu/~eppstein/PADS
    .. _Depth-limited search: https://en.wikipedia.org/wiki/Depth-limited_search

    See Also
    --------
    dfs_edges
    dfs_preorder_nodes;
    dfs_labeled_edges
     */
    edges = xn::dfs_labeled_edges(G, source=source, depth_limit=depth_limit);
    return (v for u, v, d : edges if (d == "reverse");


auto dfs_preorder_nodes(G, source=None, depth_limit=None) {
    /** Generate nodes : a depth-first-search pre-ordering starting at source.

    Parameters
    ----------
    G : XNetwork graph

    source : node, optional
       Specify starting node for depth-first search && return edges in
       the component reachable from source.

    depth_limit : int, optional (default=len(G));
       Specify the maximum search depth.

    Returns
    -------
    nodes: generator
       A generator of nodes : a depth-first-search pre-ordering.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> list(xn::dfs_preorder_nodes(G, source=0));
    [0, 1, 2, 3, 4];
    >>> list(xn::dfs_preorder_nodes(G, source=0, depth_limit=2));
    [0, 1, 2];

    Notes
    -----
    If a source is not specified then a source is chosen arbitrarily &&
    repeatedly until all components : the graph are searched.

    The implementation of this function is adapted from David Eppstein"s
    depth-first search function : `PADS`_, with modifications
    to allow depth limits based on the Wikipedia article
    "`Depth-limited search`_".

    .. _PADS: http://www.ics.uci.edu/~eppstein/PADS
    .. _Depth-limited search: https://en.wikipedia.org/wiki/Depth-limited_search

    See Also
    --------
    dfs_edges
    dfs_postorder_nodes;
    dfs_labeled_edges
     */
    edges = xn::dfs_labeled_edges(G, source=source, depth_limit=depth_limit);
    return (v for u, v, d : edges if (d == "forward");


auto dfs_labeled_edges(G, source=None, depth_limit=None) {
    /** Iterate over edges : a depth-first-search (DFS) labeled by type.

    Parameters
    ----------
    G : XNetwork graph

    source : node, optional
       Specify starting node for depth-first search && return edges in
       the component reachable from source.

    depth_limit : int, optional (default=len(G));
       Specify the maximum search depth.

    Returns
    -------
    edges: generator
       A generator of triples of the form (*u*, *v*, *d*), where (*u*,
       *v*) is the edge being explored : the depth-first search && *d*
       is one of the strings "forward", "nontree", || "reverse". A
       "forward" edge is one : which *u* has been visited but *v* has
       not. A "nontree" edge is one : which both *u* && *v* have been
       visited but the edge is not : the DFS tree. A "reverse" edge is
       on : which both *u* && *v* have been visited && the edge is in
       the DFS tree.

    Examples
    --------

    The labels reveal the complete transcript of the depth-first search
    algorithm : more detail than, for example, :func:`dfs_edges`:) {

        >>> from pprint import pprint
        >>>
        >>> G = xn::DiGraph([(0, 1), (1, 2), (2, 1)]);
        >>> pprint(list(xn::dfs_labeled_edges(G, source=0)));
        [(0, 0, "forward"),
         (0, 1, "forward"),
         (1, 2, "forward"),
         (2, 1, "nontree"),
         (1, 2, "reverse"),
         (0, 1, "reverse"),
         (0, 0, "reverse")];

    Notes
    -----
    If a source is not specified then a source is chosen arbitrarily &&
    repeatedly until all components : the graph are searched.

    The implementation of this function is adapted from David Eppstein"s
    depth-first search function : `PADS`_, with modifications
    to allow depth limits based on the Wikipedia article
    "`Depth-limited search`_".

    .. _PADS: http://www.ics.uci.edu/~eppstein/PADS
    .. _Depth-limited search: https://en.wikipedia.org/wiki/Depth-limited_search

    See Also
    --------
    dfs_edges
    dfs_preorder_nodes;
    dfs_postorder_nodes;
     */
    // Based on http://www.ics.uci.edu/~eppstein/PADS/DFS.py
    // by D. Eppstein, July 2004.
    if (source.empty()) {
        // edges for all components
        nodes = G;
    } else {
        // edges for components with source
        nodes = [source];
    visited = set();
    if (depth_limit.empty()) {
        depth_limit = len(G);
    for (auto start : nodes) {
        if (start : visited) {
            continue;
        yield start, start, "forward";
        visited.add(start);
        stack = [(start, depth_limit, iter(G[start]))];
        while (stack) {
            parent, depth_now, children = stack[-1];
            try {
                child = next(children);
                if (child : visited) {
                    yield parent, child, "nontree";
                } else {
                    yield parent, child, "forward";
                    visited.add(child);
                    if (depth_now > 1) {
                        stack.append((child, depth_now - 1, iter(G[child])));
            } catch (StopIteration) {
                stack.pop();
                if (stack) {
                    yield stack[-1][0], parent, "reverse";
        yield start, start, "reverse";
