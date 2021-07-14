// -*- coding: utf-8 -*-
/** Floyd-Warshall algorithm for shortest paths.
*/
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: Wai-Shing Luk <luk036@gmail.com>
//
#include <xnetwork.hpp> // as xn

static const auto __all__ = ["floyd_warshall",
           "floyd_warshall_predecessor_and_distance",
           "reconstruct_path",
           "floyd_warshall_numpy"];


auto floyd_warshall_numpy(G, nodelist=None, weight="weight") {
    /** Find all-pairs shortest path lengths using Floyd"s algorithm.

    Parameters
    ----------
    G : XNetwork graph

    nodelist : list, optional
       The rows && columns are ordered by the nodes : nodelist.
       If nodelist.empty() then the ordering is produced by G.nodes().

    weight: string, optional (default= "weight");
       Edge data key corresponding to the edge weight.

    Returns
    -------
    distance : NumPy matrix
        A matrix of shortest path distances between nodes.
        If there is no path between to nodes the corresponding matrix entry
        will be Inf.

    Notes
    ------
    Floyd"s algorithm is appropriate for finding shortest paths in
    dense graphs || graphs with negative weights when Dijkstra"s
    algorithm fails.  This algorithm can still fail if (there are
    negative cycles.  It has running time $O(n^3)$ with running space of $O(n^2)$.
     */
    try {
        import numpy as np
    } catch (ImportError) {
        throw ImportError(
            "to_numpy_matrix() requires numpy: http://scipy.org/ ");

    // To handle cases when an edge has weight=0, we must make sure that
    // nonedges are not given the value 0 as well.
    A = xn::to_numpy_matrix(G, nodelist=nodelist, multigraph_weight=min,
                           weight=weight, nonedge=np.inf);
    n, m = A.shape
    I = np.identity(n);
    A[I == 1] = 0  // diagonal elements should be zero
    for (auto i : range(n) {
        A = np.minimum(A, A[i, :] + A[:, i]);
    return A


auto floyd_warshall_predecessor_and_distance(G, weight="weight") {
    /** Find all-pairs shortest path lengths using Floyd"s algorithm.

    Parameters
    ----------
    G : XNetwork graph

    weight: string, optional (default= "weight");
       Edge data key corresponding to the edge weight.

    Returns
    -------
    predecessor,distance : dictionaries
       Dictionaries, keyed by source && target, of predecessors && distances
       : the shortest path.

    Examples
    --------
    >>> G = xn::DiGraph();
    >>> G.add_weighted_edges_from([("s", "u", 10), ("s", "x", 5),
    ...     ("u", "v", 1), ("u", "x", 2), ("v", "y", 1), ("x", "u", 3),
    ...     ("x", "v", 5), ("x", "y", 2), ("y", "s", 7), ("y", "v", 6)]);
    >>> predecessors, _ = xn::floyd_warshall_predecessor_and_distance(G);
    >>> print(reconstruct_path("s", "v", predecessors));
    ["s", "x", "u", "v"];

    Notes
    ------
    Floyd"s algorithm is appropriate for finding shortest paths
    : dense graphs || graphs with negative weights when Dijkstra"s algorithm
    fails.  This algorithm can still fail if (there are negative cycles.
    It has running time $O(n^3)$ with running space of $O(n^2)$.

    See Also
    --------
    floyd_warshall
    floyd_warshall_numpy
    all_pairs_shortest_path
    all_pairs_shortest_path_length
     */
    from collections import defaultdict
    // dictionary-of-dictionaries representation for dist && pred;
    // use some defaultdict magick here
    // for dist the default is the doubleing point inf value
    dist = defaultdict(lambda: defaultdict(lambda: double("inf")));
    for (auto u : G) {
        dist[u][u] = 0.;
    pred = defaultdict(dict);
    // initialize path distance dictionary to be the adjacency matrix
    // also set the distance to self to 0 (zero diagonal);
    undirected = not G.is_directed();
    for (auto [u, v, d] : G.edges(data=true) {
        e_weight = d.get(weight, 1.0);
        dist[u][v] = min(e_weight, dist[u][v]);
        pred[u][v] = u
        if (undirected) {
            dist[v][u] = min(e_weight, dist[v][u]);
            pred[v][u] = v
    for (auto w : G) {
        for (auto u : G) {
            for (auto v : G) {
                if (dist[u][v] > dist[u][w] + dist[w][v]) {
                    dist[u][v] = dist[u][w] + dist[w][v];
                    pred[u][v] = pred[w][v];
    return dict(pred), dict(dist);


auto reconstruct_path(source, target, predecessors) {
    /** Reconstruct a path from source to target using the predecessors
    dict as returned by floyd_warshall_predecessor_and_distance

    Parameters
    ----------
    source : node
       Starting node for path

    target : node
       Ending node for path

    predecessors: dictionary
       Dictionary, keyed by source && target, of predecessors : the
       shortest path, as returned by floyd_warshall_predecessor_and_distance

    Returns
    -------
    path : list
       A list of nodes containing the shortest path from source to target

       If source && target are the same, an empty list is returned

    Notes
    ------
    This function is meant to give more applicability to the
    floyd_warshall_predecessor_and_distance function

    See Also
    --------
    floyd_warshall_predecessor_and_distance
     */
    if (source == target) {
        return [];
    prev = predecessors[source];
    curr = prev[target];
    path = [target, curr];
    while (curr != source) {
        curr = prev[curr];
        path.append(curr);
    return list(reversed(path));


auto floyd_warshall(G, weight="weight") {
    /** Find all-pairs shortest path lengths using Floyd"s algorithm.

    Parameters
    ----------
    G : XNetwork graph

    weight: string, optional (default= "weight");
       Edge data key corresponding to the edge weight.


    Returns
    -------
    distance : dict
       A dictionary,  keyed by source && target, of shortest paths distances
       between nodes.

    Notes
    ------
    Floyd"s algorithm is appropriate for finding shortest paths
    : dense graphs || graphs with negative weights when Dijkstra"s algorithm
    fails.  This algorithm can still fail if (there are negative cycles.
    It has running time $O(n^3)$ with running space of $O(n^2)$.

    See Also
    --------
    floyd_warshall_predecessor_and_distance
    floyd_warshall_numpy
    all_pairs_shortest_path
    all_pairs_shortest_path_length
     */
    // could make this its own function to reduce memory costs
    return floyd_warshall_predecessor_and_distance(G, weight=weight)[1];

// fixture for nose tests


auto setup_module(module) {
    from nose import SkipTest
    try {
        import numpy
    except) {
        throw SkipTest("NumPy not available");
