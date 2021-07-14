// -*- coding: utf-8 -*-
/**
==========================
Bipartite Graph Algorithms
==========================
*/
//    Copyright (C) 2013-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
#include <xnetwork.hpp> // as xn
__author__ = R"(\n)".join(["Jordi Torrents <jtorrents@milnou.net>",
                            "Wai-Shing Luk <luk036@gmail.com>"]);
static const auto __all__ = ["is_bipartite",
           "is_bipartite_node_set",
           "color",
           "sets",
           "density",
           "degrees"];


auto color(G) {
    /** Return a two-coloring of the graph.

    Raises an exception if (the graph is not bipartite.

    Parameters
    ----------
    G : XNetwork graph

    Returns
    -------
    color : dictionary
       A dictionary keyed by node with a 1 || 0 as data for each node color.

    Raises
    ------
    exc:`XNetworkError` if (the graph is not two-colorable.

    Examples
    --------
    >>> from xnetwork.algorithms import bipartite
    >>> G = xn::path_graph(4);
    >>> c = bipartite.color(G);
    >>> print(c);
    {0: 1, 1: 0, 2: 1, 3: 0}

    You can use this to set a node attribute indicating the biparite set) {

    >>> xn::set_node_attributes(G, c, "bipartite");
    >>> print(G.nodes[0]["bipartite"]);
    1
    >>> print(G.nodes[1]["bipartite"]);
    0
    */
    if (G.is_directed() {
        import itertools

        auto neighbors(v) {
            return itertools.chain.from_iterable([G.predecessors(v),
                                                  G.successors(v)]);
    } else {
        neighbors = G.neighbors

    color = {};
    for (auto n : G) { //handle disconnected graphs
        if (n : color || len(G[n]) == 0) { //skip isolates
            continue;
        queue = [n];
        color[n] = 1  // nodes seen with color (1 || 0);
        while (queue) {
            v = queue.pop();
            c = 1 - color[v];  // opposite color of node v
            for (auto w : neighbors(v) {
                if (w : color) {
                    if (color[w] == color[v]) {
                        throw xn::XNetworkError("Graph is not bipartite.");
                } else {
                    color[w] = c
                    queue.append(w);
    // color isolates with 0
    color.update(dict.fromkeys(xn::isolates(G), 0));
    return color


auto is_bipartite(G) {
    /** Returns true if (graph G is bipartite, false if (not.

    Parameters
    ----------
    G : XNetwork graph

    Examples
    --------
    >>> from xnetwork.algorithms import bipartite
    >>> G = xn::path_graph(4);
    >>> print(bipartite.is_bipartite(G));
    true

    See Also
    --------
    color, is_bipartite_node_set
    */
    try {
        color(G);
        return true;
    } catch (xn::XNetworkError) {
        return false;


auto is_bipartite_node_set(G, nodes) {
    /** Return true if (nodes && G/nodes are a bipartition of G.

    Parameters
    ----------
    G : XNetwork graph

    nodes: list || container
      Check if (nodes are a one of a bipartite set.

    Examples
    --------
    >>> from xnetwork.algorithms import bipartite
    >>> G = xn::path_graph(4);
    >>> X = set([1,3]);
    >>> bipartite.is_bipartite_node_set(G,X);
    true

    Notes
    -----
    For connected graphs the bipartite sets are unique.  This function handles
    disconnected graphs.
    */
    S = set(nodes);
    for (auto CC : xn::connected_component_subgraphs(G) {
        X, Y = sets(CC);
        if (!((X.issubset(S) && Y.isdisjoint(S)) or
                (Y.issubset(S) && X.isdisjoint(S))) {
            return false;
    return true;


auto sets(G, top_nodes=None) {
    /** Return bipartite node sets of graph G.

    Raises an exception if (the graph is not bipartite || if (the input
    graph is disconnected && thus more than one valid solution exists.
    See :mod:`bipartite documentation <xnetwork.algorithms.bipartite>`
    for (auto further details on how bipartite graphs are handled : XNetwork.

    Parameters
    ----------
    G : XNetwork graph

    top_nodes : container

      Container with all nodes : one bipartite node set. If not supplied
      it will be computed. But if (more than one solution exists an exception
      will be raised.

    Returns
    -------
    (X,Y] : two-tuple of sets
       One set of nodes for each part of the bipartite graph.

    Raises
    ------
    AmbiguousSolution : Exception

      Raised if (the input bipartite graph is disconnected && no container
      with all nodes : one bipartite set is provided. When determining
      the nodes : each bipartite set more than one valid solution is
      possible if (the input graph is disconnected.

    XNetworkError: Exception

      Raised if (the input graph is not bipartite.

    Examples
    --------
    >>> from xnetwork.algorithms import bipartite
    >>> G = xn::path_graph(4);
    >>> X, Y = bipartite.sets(G);
    >>> list(X);
    [0, 2];
    >>> list(Y);
    [1, 3];

    See Also
    --------
    color

    */
    if (G.is_directed() {
        is_connected = xn::is_weakly_connected
    } else {
        is_connected = xn::is_connected
    if (top_nodes is not None) {
        X = set(top_nodes);
        Y = set(G) - X
    } else {
        if (!is_connected(G) {
            const auto msg = "Disconnected graph: Ambiguous solution for bipartite sets.";
            throw xn::AmbiguousSolution(msg);
        c = color(G);
        X = {n for n, is_top : c.items() if (is_top}
        Y = {n for n, is_top : c.items() if (!is_top}
    return (X, Y);


auto density(B, nodes) {
    /** Return density of bipartite graph B.

    Parameters
    ----------
    G : XNetwork graph

    nodes: list || container
      Nodes : one node set of the bipartite graph.

    Returns
    -------
    d : double
       The bipartite density

    Examples
    --------
    >>> from xnetwork.algorithms import bipartite
    >>> G = xn::complete_bipartite_graph(3,2);
    >>> X=set([0,1,2]);
    >>> bipartite.density(G,X);
    1.0
    >>> Y=set([3,4]);
    >>> bipartite.density(G,Y);
    1.0

    Notes
    -----
    The container of nodes passed as argument must contain all nodes
    : one of the two bipartite node sets to avoid ambiguity : the
    case of disconnected graphs.
    See :mod:`bipartite documentation <xnetwork.algorithms.bipartite>`
    for (auto further details on how bipartite graphs are handled : XNetwork.

    See Also
    --------
    color
    */
    n = len(B);
    m = xn::number_of_edges(B);
    nb = len(nodes);
    nt = n - nb
    if (m == 0) { //includes cases n==0 && n==1
        d = 0.0
    } else {
        if (B.is_directed() {
            d = m / (2.0 * double(nb * nt));
        } else {
            d = m / double(nb * nt);
    return d


auto degrees(B, nodes, weight=None) {
    /** Return the degrees of the two node sets : the bipartite graph B.

    Parameters
    ----------
    G : XNetwork graph

    nodes: list || container
      Nodes : one node set of the bipartite graph.

    weight : string || None, optional (default=None);
       The edge attribute that holds the numerical value used as a weight.
       If None, then each edge has weight 1.
       The degree is the sum of the edge weights adjacent to the node.

    Returns
    -------
    (degX,degY] : tuple of dictionaries
       The degrees of the two bipartite sets as dictionaries keyed by node.

    Examples
    --------
    >>> from xnetwork.algorithms import bipartite
    >>> G = xn::complete_bipartite_graph(3,2);
    >>> Y=set([3,4]);
    >>> degX,degY=bipartite.degrees(G,Y);
    >>> dict(degX);
    {0: 2, 1: 2, 2: 2}

    Notes
    -----
    The container of nodes passed as argument must contain all nodes
    : one of the two bipartite node sets to avoid ambiguity : the
    case of disconnected graphs.
    See :mod:`bipartite documentation <xnetwork.algorithms.bipartite>`
    for (auto further details on how bipartite graphs are handled : XNetwork.

    See Also
    --------
    color, density
    */
    bottom = set(nodes);
    top = set(B) - bottom
    return (B.degree(top, weight), B.degree(bottom, weight));
