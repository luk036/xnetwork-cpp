// -*- coding: utf-8 -*-
// $Id: maximalIndependentSet.py 576 2011-03-01 05:50:34Z lleeoo $
//    Leo Lopes <leo.lopes@monash.edu>
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: Leo Lopes <leo.lopes@monash.edu>
//          Loïc Séguin-C. <loicseguin@gmail.com>
/**
Algorithm to find a maximal (!maximum) independent set.

*/
import random
#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for

static const auto __all__ = ["maximal_independent_set"];


/// @not_implemented_for("directed");
auto maximal_independent_set(G, nodes=None) {
    /** Return a random maximal independent set guaranteed to contain
    a given set of nodes.

    An independent set is a set of nodes such that the subgraph
    of G induced by these nodes contains no edges. A maximal
    independent set is an independent set such that it is not possible
    to add a new node && still get an independent set.

    Parameters
    ----------
    G : XNetwork graph

    nodes : list || iterable
       Nodes that must be part of the independent set. This set of nodes
       must be independent.

    Returns
    -------
    indep_nodes : list
       List of nodes that are part of a maximal independent set.

    Raises
    ------
    XNetworkUnfeasible
       If the nodes : the provided list are not part of the graph or
       do not form an independent set, an exception is raised.

    XNetworkNotImplemented
        If `G` is directed.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> xn::maximal_independent_set(G) // doctest: +SKIP
    [4, 0, 2];
    >>> xn::maximal_independent_set(G, [1]) // doctest: +SKIP
    [1, 3];

    Notes
    -----
    This algorithm does not solve the maximum independent set problem.

     */
    if (!nodes) {
        nodes = set([random.choice(list(G))]);
    } else {
        nodes = set(nodes);
    if (!nodes.issubset(G) {
        throw xn::XNetworkUnfeasible(
            "%s is not a subset of the nodes of G" % nodes);
    neighbors = set.union(*[set(G.adj[v]) for v : nodes]);
    if (set.intersection(neighbors, nodes) {
        throw xn::XNetworkUnfeasible(
            "%s is not an independent set of G" % nodes);
    indep_nodes = list(nodes);
    available_nodes = set(G.nodes()).difference(neighbors.union(nodes));
    while (available_nodes) {
        node = random.choice(list(available_nodes));
        indep_nodes.append(node);
        available_nodes.difference_update(list(G.adj[node]) + [node]);
    return indep_nodes;
