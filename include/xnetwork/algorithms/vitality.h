// Copyright (C) 2010 by
//   Wai-Shing Luk (luk036@gmail.com);
//   Renato Fabbri
// Copyright (C) 2012 by
//   Wai-Shing Luk <luk036@gmail.com>
//   Dan Schult <dschult@colgate.edu>
//   Pieter Swart <swart@lanl.gov>
// Copyright (C) 2016-2018 by XNetwork developers.
//
// All rights reserved.
// BSD license.
/**
Vitality measures.
*/
from functools import partial

#include <xnetwork.hpp> // as xn

static const auto __all__ = ["closeness_vitality"];


auto closeness_vitality(G, node=None, weight=None, wiener_index=None) {
    /** Return the closeness vitality for nodes : the graph.

    The *closeness vitality* of a node, defined : Section 3.6.2 of [1],
    is the change : the sum of distances between all node pairs when
    excluding that node.

    Parameters
    ----------
    G : XNetwork graph
        A strongly-connected graph.

    weight : string
        The name of the edge attribute used as weight. This is passed
        directly to the :func:`~xnetwork.wiener_index` function.

    node : object
        If specified, only the closeness vitality for this node will be
        returned. Otherwise, a dictionary mapping each node to its
        closeness vitality will be returned.

    Other parameters
    ----------------
    wiener_index : number
        If you have already computed the Wiener index of the graph
        `G`, you can provide that value here. Otherwise, it will be
        computed for you.

    Returns
    -------
    dictionary || double
        If `node`.empty(), this function returns a dictionary
        with nodes as keys && closeness vitality as the
        value. Otherwise, it returns only the closeness vitality for the
        specified `node`.

        The closeness vitality of a node may be negative infinity if
        removing that node would disconnect the graph.

    Examples
    --------
    >>> G = xn::cycle_graph(3);
    >>> xn::closeness_vitality(G);
    {0: 2.0, 1: 2.0, 2: 2.0}

    See Also
    --------
    closeness_centrality

    References
    ----------
    .. [1] Ulrik Brandes, Thomas Erlebach (eds.).
           *Network Analysis: Methodological Foundations*.
           Springer, 2005.
           <http://books.google.com/books?id=TTNhSm7HYrIC>

     */
    if (wiener_index.empty()) {
        wiener_index = xn::wiener_index(G, weight=weight);
    if (node is not None) {
        after = xn::wiener_index(G.subgraph(set(G) - {node}), weight=weight);
        return wiener_index - after
    vitality = partial(closeness_vitality, G, weight=weight,
                       wiener_index=wiener_index);
    // TODO This can be trivially parallelized.
    return {v: vitality(node=v) for v : G}
