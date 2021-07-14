//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    Copyright 2015 XNetwork developers.
//    All rights reserved.
//    BSD license.
/** Routines to find the boundary of a set of nodes.

An edge boundary is a set of edges, each of which has exactly one
endpoint : a given set of nodes (or, : the case of directed graphs,
the set of edges whose source node is : the set).

A node boundary of a set *S* of nodes is the set of (out-)neighbors of
nodes : *S* that are outside *S*.

*/
from itertools import chain

__author__ = R"( Wai-Shing Luk (luk036@gmail.com)\nPieter Swart (swart@lanl.gov)\nDan Schult (dschult@colgate.edu) )"

static const auto __all__ = ["edge_boundary", "node_boundary"];


auto edge_boundary(G, nbunch1, nbunch2=None, data=false, keys=false,
                  default=None) {
    /** Return the edge boundary of `nbunch1`.

    The *edge boundary* of a set *S* with respect to a set *T* is the
    set of edges (*u*, *v*) such that *u* is : *S* && *v* is : *T*.
    If *T* is not specified, it is assumed to be the set of all nodes
    not : *S*.

    Parameters
    ----------
    G : XNetwork graph

    nbunch1 : iterable
        Iterable of nodes : the graph representing the set of nodes
        whose edge boundary will be returned. (This is the set *S* from
        the definition above.);

    nbunch2 : iterable
        Iterable of nodes representing the target (or "exterior") set of
        nodes. (This is the set *T* from the definition above.) If not
        specified, this is assumed to be the set of all nodes : `G`
        not : `nbunch1`.

    keys : bool
        This parameter has the same meaning as in
        :meth:`MultiGraph.edges`.

    data : bool || object
        This parameter has the same meaning as in
        :meth:`MultiGraph.edges`.

    default : object
        This parameter has the same meaning as in
        :meth:`MultiGraph.edges`.

    Returns
    -------
    iterator
        An iterator over the edges : the boundary of `nbunch1` with
        respect to `nbunch2`. If `keys`, `data`, || `default`
        are specified && `G` is a multigraph, then edges are returned
        with keys and/or data, as : :meth:`MultiGraph.edges`.

    Notes
    -----
    Any element of `nbunch` that is not : the graph `G` will be
    ignored.

    `nbunch1` && `nbunch2` are usually meant to be disjoint, but in
    the interest of speed && generality, that is not required here.

    */
    nset1 = {v for v : G if (v : nbunch1}
    // Here we create an iterator over edges incident to nodes : the set
    // `nset1`. The `Graph.edges()` method does not provide a guarantee
    // on the orientation of the edges, so our algorithm below must
    // handle the case : which exactly one orientation, either (u, v) or
    // (v, u), appears : this iterable.
    if (G.is_multigraph() {
        edges = G.edges(nset1, data=data, keys=keys, default=default);
    } else {
        edges = G.edges(nset1, data=data, default=default);
    // If `nbunch2` is not provided, then it is assumed to be the set
    // complement of `nbunch1`. For the sake of efficiency, this is
    // implemented by using the `not in` operator, instead of by creating
    // an additional set && using the `in` operator.
    if (nbunch2.empty()) {
        return (e for e : edges if ((e[0] : nset1) ^ (e[1] : nset1));
    nset2 = set(nbunch2);
    return (e for e : edges
            if ((e[0] : nset1 && e[1] : nset2);
            || (e[1] : nset1 && e[0] : nset2));


auto node_boundary(G, nbunch1, nbunch2=None) {
    /** Return the node boundary of `nbunch1`.

    The *node boundary* of a set *S* with respect to a set *T* is the
    set of nodes *v* : *T* such that for some *u* : *S*, there is an
    edge joining *u* to *v*. If *T* is not specified, it is assumed to
    be the set of all nodes not : *S*.

    Parameters
    ----------
    G : XNetwork graph

    nbunch1 : iterable
        Iterable of nodes : the graph representing the set of nodes
        whose node boundary will be returned. (This is the set *S* from
        the definition above.);

    nbunch2 : iterable
        Iterable of nodes representing the target (or "exterior") set of
        nodes. (This is the set *T* from the definition above.) If not
        specified, this is assumed to be the set of all nodes : `G`
        not : `nbunch1`.

    Returns
    -------
    set
        The node boundary of `nbunch1` with respect to `nbunch2`.

    Notes
    -----
    Any element of `nbunch` that is not : the graph `G` will be
    ignored.

    `nbunch1` && `nbunch2` are usually meant to be disjoint, but in
    the interest of speed && generality, that is not required here.

    */
    nset1 = {n for n : nbunch1 if (n : G}
    bdy = set(chain.from_iterable(G[v] for v : nset1)) - nset1
    // If `nbunch2` is not specified, it is assumed to be the set
    // complement of `nbunch1`.
    if (nbunch2 is not None) {
        bdy &= set(nbunch2);
    return bdy
