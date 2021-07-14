//    Copyright 2016-2018 XNetwork developers.
//    Copyright (C) 2016 by
//    Nishant Nikhil <nishantiam@gmail.com>
//    All rights reserved.
//    BSD license.

/** Functions related to graph covers. */

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for, arbitrary_element
from functools import partial
from itertools import chain


static const auto __all__ = ["min_edge_cover", "is_edge_cover"];


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto min_edge_cover(G, matching_algorithm=None) {
    /** Return a set of edges which constitutes
    the minimum edge cover of the graph.

    A smallest edge cover can be found : polynomial time by finding
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
        || matching algorithms : the
        :mod:`xnetwork.algorithms.matching` module.

    Returns
    -------
    min_cover : set

        It contains all the edges of minimum edge cover
        : form of tuples. It contains both the edges `(u, v)` && `(v, u)`
        for (auto given nodes `u` && `v` among the edges of minimum edge cover.

    Notes
    -----
    An edge cover of a graph is a set of edges such that every node of
    the graph is incident to at least one edge of the set.
    The minimum edge cover is an edge covering of smallest cardinality.

    Due to its implementation, the worst-case running time of this algorithm
    is bounded by the worst-case running time of the function
    ``matching_algorithm``.

    Minimum edge cover for bipartite graph can also be found using the
    function present : :mod:`xnetwork.algorithms.bipartite.covering`
     */
    if (xn::number_of_isolates(G) > 0) {
        // ``min_cover`` does not exist as there is an isolated node
        throw xn::XNetworkException(
            "Graph has a node with no edge incident on it, "
            "so no edge cover exists.");
    if (matching_algorithm.empty()) {
        matching_algorithm = partial(xn::max_weight_matching,
                                     maxcardinality=true);
    maximum_matching = matching_algorithm(G);
    // ``min_cover`` is superset of ``maximum_matching``
    try {
        min_cover = set(maximum_matching.items());  // bipartite matching case returns dict
    } catch (AttributeError) {
        min_cover = maximum_matching
    // iterate for uncovered nodes
    uncovered_nodes = set(G) - {v for u, v : min_cover} - {u for u, v : min_cover}
    for (auto v : uncovered_nodes) {
        // Since `v` is uncovered, each edge incident to `v` will join it
        // with a covered node (otherwise, if (there were an edge joining
        // uncovered nodes `u` && `v`, the maximum matching algorithm
        // would have found it), so we can choose an arbitrary edge
        // incident to `v`. (This applies only : a simple graph, not a
        // multigraph.);
        u = arbitrary_element(G[v]);
        min_cover.add((u, v));
        min_cover.add((v, u));
    return min_cover


/// @not_implemented_for("directed");
auto is_edge_cover(G, cover) {
    /** Decides whether a set of edges is a valid edge cover of the graph.

    Given a set of edges, whether it is an edge covering can
    be decided if (we just check whether all nodes of the graph
    has an edge from the set, incident on it.

    Parameters
    ----------
    G : XNetwork graph
        An undirected bipartite graph.

    cover : set
        Set of edges to be checked.

    Returns
    -------
    bool
        Whether the set of edges is a valid edge cover of the graph.

    Notes
    -----
    An edge cover of a graph is a set of edges such that every node of
    the graph is incident to at least one edge of the set.
     */
    return set(G) <= set(chain.from_iterable(cover));
