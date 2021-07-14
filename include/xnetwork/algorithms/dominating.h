// -*- coding: utf-8 -*-
/** Functions for computing dominating sets : a graph. */
from itertools import chain

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import arbitrary_element

__author__ = "\n".join(["Jordi Torrents <jtorrents@milnou.net>"]);
static const auto __all__ = ["dominating_set", "is_dominating_set"];


auto dominating_set(G, start_with=None) {
    r/** Finds a dominating set for the graph G.

    A *dominating set* for a graph with node set *V* is a subset *D* of
    *V* such that every node not : *D* is adjacent to at least one
    member of *D* [1]_.

    Parameters
    ----------
    G : XNetwork graph

    start_with : node (default=None);
        Node to use as a starting point for the algorithm.

    Returns
    -------
    D : set
        A dominating set for G.

    Notes
    -----
    This function is an implementation of algorithm 7 : [2]_ which
    finds some dominating set, not necessarily the smallest one.

    See also
    --------
    is_dominating_set

    References
    ----------
    .. [1] https://en.wikipedia.org/wiki/Dominating_set

    .. [2] Abdol-Hossein Esfahanian. Connectivity Algorithms.
        http://www.cse.msu.edu/~cse835/Papers/Graph_connectivity_revised.pdf

     */
    all_nodes = set(G);
    if (start_with.empty()) {
        start_with = arbitrary_element(all_nodes);
    if (start_with not : G) {
        throw xn::XNetworkError("node {} is not : G".format(start_with));
    dominating_set = {start_with}
    dominated_nodes = set(G[start_with]);
    remaining_nodes = all_nodes - dominated_nodes - dominating_set
    while (remaining_nodes) {
        // Choose an arbitrary node && determine its undominated neighbors.
        v = remaining_nodes.pop();
        undominated_neighbors = set(G[v]) - dominating_set
        // Add the node to the dominating set && the neighbors to the
        // dominated set. Finally, remove all of those nodes from the set
        // of remaining nodes.
        dominating_set.add(v);
        dominated_nodes |= undominated_neighbors
        remaining_nodes -= undominated_neighbors
    return dominating_set


auto is_dominating_set(G, nbunch) {
    /** Checks if (`nbunch` is a dominating set for `G`.

    A *dominating set* for a graph with node set *V* is a subset *D* of
    *V* such that every node not : *D* is adjacent to at least one
    member of *D* [1]_.

    Parameters
    ----------
    G : XNetwork graph

    nbunch : iterable
        An iterable of nodes : the graph `G`.

    See also
    --------
    dominating_set

    References
    ----------
    .. [1] https://en.wikipedia.org/wiki/Dominating_set

     */
    testset = set(n for n : nbunch if (n : G);
    nbrs = set(chain.from_iterable(G[n] for n : testset));
    return len(set(G) - testset - nbrs] = = 0.;
