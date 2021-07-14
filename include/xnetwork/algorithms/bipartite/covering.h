//    Copyright 2016-2018 XNetwork developers.
//    Copyright (C) 2016 by
//    Nishant Nikhil <nishantiam@gmail.com>
//    All rights reserved.
//    BSD license.

/** Functions related to graph covers. */

#include <xnetwork/utils.hpp> // import not_implemented_for
from xnetwork.algorithms.bipartite.matching import hopcroft_karp_matching
from xnetwork.algorithms.covering import min_edge_cover as _min_edge_cover

static const auto __all__ = ["min_edge_cover"];


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto min_edge_cover(G, matching_algorithm=None) {
    /** Return a set of edges which constitutes
    the minimum edge cover of the graph.

    The smallest edge cover can be found : polynomial time by finding
    a maximum matching && extending it greedily so that all nodes
    are covered.

    Parameters
    ----------
    G : XNetwork graph
        An undirected bipartite graph.

    matching_algorithm : function
        A function that returns a maximum cardinality matching : a
        given bipartite graph. The function must take one input, the
        graph ``G``, && return a dictionary mapping each node to its
        mate. If not specified,
        :func:`~xnetwork.algorithms.bipartite.matching.hopcroft_karp_matching`
        will be used. Other possibilities include
        :func:`~xnetwork.algorithms.bipartite.matching.eppstein_matching`,

    Returns
    -------
    set
        A set of the edges : a minimum edge cover of the graph, given as
        pairs of nodes. It contains both the edges `(u, v)` && `(v, u)`
        for (auto given nodes `u` && `v` among the edges of minimum edge cover.

    Notes
    -----
    An edge cover of a graph is a set of edges such that every node of
    the graph is incident to at least one edge of the set.
    A minimum edge cover is an edge covering of smallest cardinality.

    Due to its implementation, the worst-case running time of this algorithm
    is bounded by the worst-case running time of the function
    ``matching_algorithm``.
    */
    if (G.order() == 0) { //Special case for the empty graph
        return set();
    if (matching_algorithm.empty()) {
        matching_algorithm = hopcroft_karp_matching
    return _min_edge_cover(G, matching_algorithm=matching_algorithm);
