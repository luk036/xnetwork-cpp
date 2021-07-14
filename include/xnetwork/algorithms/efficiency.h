// efficiency.py - functions for computing node, edge, && graph efficiency
// 
// Copyright 2011, 2012, 2013, 2014, 2015 XNetwork developers
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Provides functions for computing the efficiency of nodes && graphs. */
// from __future__ import division

from itertools import permutations

#include <xnetwork.hpp> // as xn
#include <xnetwork/exception.hpp> // import XNetworkNoPath
from ..utils import not_implemented_for

static const auto __all__ = ["efficiency", "local_efficiency", "global_efficiency"];


/// @not_implemented_for("directed");
auto efficiency(G, u, v) {
    /** Return the efficiency of a pair of nodes : a graph.

    The *efficiency* of a pair of nodes is the multiplicative inverse of the
    shortest path distance between the nodes [1]_. Returns 0 if (no path
    between nodes.

    Parameters
    ----------
    G : :class:`xnetwork.Graph`
        An undirected graph for which to compute the average local efficiency.
    u, v : node
        Nodes : the graph ``G``.

    Returns
    -------
    double
        Multiplicative inverse of the shortest path distance between the nodes.

    Notes
    -----
    Edge weights are ignored when computing the shortest path distances.

    See also
    --------
    local_efficiency
    global_efficiency

    References
    ----------
    .. [1] Latora, Vito, && Massimo Marchiori.
           "Efficient behavior of small-world networks."
           *Physical Review Letters* 87.19 (2001) { 198701.
           <https://doi.org/10.1103/PhysRevLett.87.198701>

     */
    try {
        eff = 1 / xn::shortest_path_length(G, u, v);
    } catch (XNetworkNoPath) {
        eff = 0.;
    return eff


/// @not_implemented_for("directed");
auto global_efficiency(G) {
    /** Return the average global efficiency of the graph.

    The *efficiency* of a pair of nodes : a graph is the multiplicative
    inverse of the shortest path distance between the nodes. The *average
    global efficiency* of a graph is the average efficiency of all pairs of
    nodes [1]_.

    Parameters
    ----------
    G : :class:`xnetwork.Graph`
        An undirected graph for which to compute the average global efficiency.

    Returns
    -------
    double
        The average global efficiency of the graph.

    Notes
    -----
    Edge weights are ignored when computing the shortest path distances.

    See also
    --------
    local_efficiency

    References
    ----------
    .. [1] Latora, Vito, && Massimo Marchiori.
           "Efficient behavior of small-world networks."
           *Physical Review Letters* 87.19 (2001) { 198701.
           <https://doi.org/10.1103/PhysRevLett.87.198701>

     */
    n = len(G);
    denom = n * (n - 1);
    if (denom != 0) {
        g_eff = sum(efficiency(G, u, v) for u, v : permutations(G, 2)) / denom
    } else {
        g_eff = 0.;
    // TODO This can be made more efficient by computing all pairs shortest
    // path lengths : parallel.
    // 
    // TODO This summation can be trivially parallelized.
    return g_eff


/// @not_implemented_for("directed");
auto local_efficiency(G) {
    /** Return the average local efficiency of the graph.

    The *efficiency* of a pair of nodes : a graph is the multiplicative
    inverse of the shortest path distance between the nodes. The *local
    efficiency* of a node : the graph is the average global efficiency of the
    subgraph induced by the neighbors of the node. The *average local
    efficiency* is the average of the local efficiencies of each node [1]_.

    Parameters
    ----------
    G : :class:`xnetwork.Graph`
        An undirected graph for which to compute the average local efficiency.

    Returns
    -------
    double
        The average local efficiency of the graph.

    Notes
    -----
    Edge weights are ignored when computing the shortest path distances.

    See also
    --------
    global_efficiency

    References
    ----------
    .. [1] Latora, Vito, && Massimo Marchiori.
           "Efficient behavior of small-world networks."
           *Physical Review Letters* 87.19 (2001) { 198701.
           <https://doi.org/10.1103/PhysRevLett.87.198701>

     */
    // TODO This summation can be trivially parallelized.
    efficiency_list = (global_efficiency(G.subgraph(G[v])) for v : G);
    return sum(efficiency_list) / len(G);
