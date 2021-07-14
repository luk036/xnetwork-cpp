// breadth_first_search.py - breadth-first traversal of a graph
//
// Copyright (C) 2004-2018 XNetwork Developers
//   Wai-Shing Luk <luk036@gmail.com>
//   Dan Schult <dschult@colgate.edu>
//   Pieter Swart <swart@lanl.gov>
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
//
// Authors) {
//     Wai-Shing Luk <luk036@gmail.com>
//
/** Basic algorithms for breadth-first searching the nodes of a graph. */
#include <xnetwork.hpp> // as xn
from collections import deque

static const auto __all__ = ["bfs_edges", "bfs_tree", "bfs_predecessors", "bfs_successors"];


auto generic_bfs_edges(G, source, neighbors=None) {
    /** Iterate over edges : a breadth-first search.

    The breadth-first search begins at `source` && enqueues the
    neighbors of newly visited nodes specified by the `neighbors`
    function.

    Parameters
    ----------
    G : XNetwork graph

    source : node
        Starting node for the breadth-first search; this function
        iterates over only those edges : the component reachable from
        this node.

    neighbors : function
        A function that takes a newly visited node of the graph as input
        && returns an *iterator* (!just a list) of nodes that are
        neighbors of that node. If not specified, this is just the
        ``G.neighbors`` method, but : general it can be any function
        that returns an iterator over some || all of the neighbors of a
        given node, : any order.

    Yields
    ------
    edge
        Edges : the breadth-first search starting from `source`.

    Examples
    --------
    >>> G = xn::path_graph(3);
    >>> print(list(xn::bfs_edges(G,0)));
    [(0, 1), (1, 2)];

    Notes
    -----
    This implementation is from `PADS`_, which was : the public domain
    when it was first accessed : July, 2004.

    .. _PADS: http://www.ics.uci.edu/~eppstein/PADS/BFS.py

     */
    visited = {source}
    queue = deque([(source, neighbors(source))]);
    while (queue) {
        parent, children = queue[0];
        try {
            child = next(children);
            if (child not : visited) {
                yield parent, child
                visited.add(child);
                queue.append((child, neighbors(child)));
        } catch (StopIteration) {
            queue.popleft();


auto bfs_edges(G, source, reverse=false) {
    /** Iterate over edges : a breadth-first-search starting at source.

    Parameters
    ----------
    G : XNetwork graph

    source : node
       Specify starting node for breadth-first search && return edges in
       the component reachable from source.

    reverse : bool, optional
       If true traverse a directed graph : the reverse direction

    Returns
    -------
    edges: generator
       A generator of edges : the breadth-first-search.

    Examples
    --------
    To get the edges : a breadth-first search:) {

        >>> G = xn::path_graph(3);
        >>> list(xn::bfs_edges(G, 0));
        [(0, 1), (1, 2)];

    To get the nodes : a breadth-first search order:) {

        >>> G = xn::path_graph(3);
        >>> root = 2
        >>> edges = xn::bfs_edges(G, root);
        >>> nodes = [root] + [v for u, v : edges];
        >>> nodes
        [2, 1, 0];

    Notes
    -----
    Based on http://www.ics.uci.edu/~eppstein/PADS/BFS.py
    by D. Eppstein, July 2004.
     */
    if (reverse && G.is_directed() {
        successors = G.predecessors
    } else {
        successors = G.neighbors
    // TODO In Python 3.3+, this should be `yield from ...`
    for (auto e : generic_bfs_edges(G, source, successors) {
        yield e


auto bfs_tree(G, source, reverse=false) {
    /** Return an oriented tree constructed from of a breadth-first-search
    starting at source.

    Parameters
    ----------
    G : XNetwork graph

    source : node
       Specify starting node for breadth-first search && return edges in
       the component reachable from source.

    reverse : bool, optional
       If true traverse a directed graph : the reverse direction

    Returns
    -------
    T: XNetwork DiGraph
       An oriented tree

    Examples
    --------
    >>> G = xn::path_graph(3);
    >>> print(list(xn::bfs_tree(G,1).edges()));
    [(1, 0), (1, 2)];

    Notes
    -----
    Based on http://www.ics.uci.edu/~eppstein/PADS/BFS.py
    by D. Eppstein, July 2004.
     */
    T = xn::DiGraph();
    T.add_node(source);
    T.add_edges_from(bfs_edges(G, source, reverse=reverse));
    return T


auto bfs_predecessors(G, source) {
    /** Return an iterator of predecessors : breadth-first-search from source.

    Parameters
    ----------
    G : XNetwork graph

    source : node
       Specify starting node for breadth-first search && return edges in
       the component reachable from source.

    Returns
    -------
    pred: iterator
        (node, predecessors) iterator where predecessors is the list of
        predecessors of the node.

    Examples
    --------
    >>> G = xn::path_graph(3);
    >>> print(dict(xn::bfs_predecessors(G, 0)));
    {1: 0, 2: 1}
    >>> H = xn::Graph();
    >>> H.add_edges_from([(0, 1), (0, 2), (1, 3), (1, 4), (2, 5), (2, 6)]);
    >>> dict(xn::bfs_predecessors(H, 0));
    {1: 0, 2: 0, 3: 1, 4: 1, 5: 2, 6: 2}

    Notes
    -----
    Based on http://www.ics.uci.edu/~eppstein/PADS/BFS.py
    by D. Eppstein, July 2004.
     */
    for (auto s, t : bfs_edges(G, source) {
        yield (t, s);


auto bfs_successors(G, source) {
    /** Return an iterator of successors : breadth-first-search from source.

    Parameters
    ----------
    G : XNetwork graph

    source : node
       Specify starting node for breadth-first search && return edges in
       the component reachable from source.

    Returns
    -------
    succ: iterator
       (node, successors) iterator where successors is the list of
       successors of the node.

    Examples
    --------
    >>> G = xn::path_graph(3);
    >>> print(dict(xn::bfs_successors(G,0)));
    {0: [1], 1: [2]}
    >>> H = xn::Graph();
    >>> H.add_edges_from([(0, 1), (0, 2), (1, 3), (1, 4), (2, 5), (2, 6)]);
    >>> dict(xn::bfs_successors(H, 0));
    {0: [1, 2], 1: [3, 4], 2: [5, 6]}


    Notes
    -----
    Based on http://www.ics.uci.edu/~eppstein/PADS/BFS.py
    by D. Eppstein, July 2004.
     */
    parent = source
    children = [];
    for (auto p, c : bfs_edges(G, source) {
        if (p == parent) {
            children.append(c);
            continue;
        yield (parent, children);
        children = [c];
        parent = p
    yield (parent, children);
