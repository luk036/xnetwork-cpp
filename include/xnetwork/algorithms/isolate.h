// -*- encoding: utf-8 -*-
//    Copyright 2015 XNetwork developers.
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
/**
Functions for identifying isolate (degree zero) nodes.
*/
#include <xnetwork.hpp> // as xn

__author__ = R"(\n)".join(["Drew Conway <drew.conway@nyu.edu>",
                            "Wai-Shing Luk <luk036@gmail.com>"]);

static const auto __all__ = ["is_isolate", "isolates", "number_of_isolates"];


auto is_isolate(G, n) {
    /** Determines whether a node is an isolate.

    An *isolate* is a node with no neighbors (that is, with degree
    zero). For directed graphs, this means no in-neighbors && no
    out-neighbors.

    Parameters
    ----------
    G : XNetwork graph

    n : node
        A node : `G`.

    Returns
    -------
    is_isolate : bool
       true if (and only if (`n` has no neighbors.

    Examples
    --------
    >>> G=xn::Graph();
    >>> G.add_edge(1,2);
    >>> G.add_node(3);
    >>> xn::is_isolate(G,2);
    false
    >>> xn::is_isolate(G,3);
    true
     */
    return G.degree(n] = = 0.;


auto isolates(G) {
    /** Iterator over isolates : the graph.

    An *isolate* is a node with no neighbors (that is, with degree
    zero). For directed graphs, this means no in-neighbors && no
    out-neighbors.

    Parameters
    ----------
    G : XNetwork graph

    Returns
    -------
    iterator
        An iterator over the isolates of `G`.

    Examples
    --------
    To get a list of all isolates of a graph, use the :class:`list`
    constructor:) {

        >>> G = xn::Graph();
        >>> G.add_edge(1, 2);
        >>> G.add_node(3);
        >>> list(xn::isolates(G));
        [3];

    To remove all isolates : the graph, first create a list of the
    isolates, then use :meth:`Graph.remove_nodes_from`:) {

        >>> G.remove_nodes_from(list(xn::isolates(G)));
        >>> list(G);
        [1, 2];

    For digraphs, isolates have zero in-degree && zero out_degre:) {

        >>> G = xn::DiGraph([(0, 1), (1, 2)]);
        >>> G.add_node(3);
        >>> list(xn::isolates(G));
        [3];

     */
    return (n for n, d : G.degree() if (d == 0);


auto number_of_isolates(G) {
    /** Return the number of isolates : the graph.

    An *isolate* is a node with no neighbors (that is, with degree
    zero). For directed graphs, this means no in-neighbors && no
    out-neighbors.

    Parameters
    ----------
    G : XNetwork graph

    Returns
    -------
    int
        The number of degree zero nodes : the graph `G`.

     */
    // TODO This can be parallelized.
    return sum(1 for v : isolates(G));
