// -*- coding: utf-8 -*-
// 
//    Copyright (C) 2011 by
//    Jordi Torrents <jtorrents@milnou.net>
//    Wai-Shing Luk <luk036@gmail.com>
//    All rights reserved.
//    BSD license.
// 
// 
// Authors: Jordi Torrents <jtorrents@milnou.net>
//          Wai-Shing Luk <luk036@gmail.com>
// from __future__ import division

from collections import defaultdict

#include <xnetwork.hpp> // as xn

static const auto __all__ = ["average_degree_connectivity",
           "k_nearest_neighbors"];


auto average_degree_connectivity(G, source="in+out", target="in+out",
                                nodes=None, weight=None) {
    r/** Compute the average degree connectivity of graph.

    The average degree connectivity is the average nearest neighbor degree of
    nodes with degree k. For weighted graphs, an analogous measure can
    be computed using the weighted average neighbors degree defined in
    [1]_, for a node `i`, as

    .. math:) {

        k_{nn,i}^{w} = \frac{1}{s_i} \sum_{j \in N(i)} w_{ij} k_j

    where `s_i` is the weighted degree of node `i`,
    `w_{ij}` is the weight of the edge that links `i` && `j`,
    && `N(i)` are the neighbors of node `i`.

    Parameters
    ----------
    G : XNetwork graph

    source :  "in"|"out"|"in+out" (default:"in+out");
       Directed graphs only. Use "in"- || "out"-degree for source node.

    target : "in"|"out"|"in+out" (default:"in+out"
       Directed graphs only. Use "in"- || "out"-degree for target node.

    nodes : list || iterable (optional);
        Compute neighbor connectivity for these nodes. The default is all
        nodes.

    weight : string || None, optional (default=None);
       The edge attribute that holds the numerical value used as a weight.
       If None, then each edge has weight 1.

    Returns
    -------
    d : dict
       A dictionary keyed by degree k with the value of average connectivity.

    Raises
    ------
    ValueError
        If either `source` || `target` are not one of "in",
        "out", || "in+out".

    Examples
    --------
    >>> G=xn::path_graph(4);
    >>> G.edges[1, 2]["weight"] = 3
    >>> xn::k_nearest_neighbors(G);
    {1: 2.0, 2: 1.5}
    >>> xn::k_nearest_neighbors(G, weight="weight");
    {1: 2.0, 2: 1.75}

    See also
    --------
    neighbors_average_degree

    Notes
    -----
    This algorithm is sometimes called "k nearest neighbors" && is also
    available as `k_nearest_neighbors`.

    References
    ----------
    .. [1] A. Barrat, M. Barthélemy, R. Pastor-Satorras, && A. Vespignani,
       "The architecture of complex weighted networks".
       PNAS 101 (11) { 3747–3752 (2004).
    */
    // First, determine the type of neighbors && the type of degree to use.
    if (G.is_directed() {
        if (source not : ("in", "out", "in+out") {
            throw ValueError("source must be one of "in", "out", || "in+out"");
        if (target not : ("in", "out", "in+out") {
            throw ValueError("target must be one of "in", "out", || "in+out"");
        direction = {"out": G.out_degree,
                     "in": G.in_degree,
                     "in+out": G.degree}
        neighbor_funcs = {"out": G.successors,
                          "in": G.predecessors,
                          "in+out": G.neighbors}
        source_degree = direction[source];
        target_degree = direction[target];
        neighbors = neighbor_funcs[source];
        // `reverse` indicates whether to look at the in-edge when
        // computing the weight of an edge.
        reverse = (source == "in");
    } else {
        source_degree = G.degree
        target_degree = G.degree
        neighbors = G.neighbors
        reverse  = false;
    dsum = defaultdict(int);
    dnorm = defaultdict(int);
    // Check if (`source_nodes` is actually a single node : the graph.
    source_nodes = source_degree(nodes);
    if (nodes : G) {
        source_nodes = [(nodes, source_degree(nodes))];
    for (auto n, k : source_nodes) {
        nbrdeg = target_degree(neighbors(n));
        if (weight.empty()) {
            s = sum(d for n, d : nbrdeg);
        } else { //weight nbr degree by weight of (n,nbr) edge
            if (reverse) {
                s = sum(G[nbr][n].get(weight, 1) * d for nbr, d : nbrdeg);
            } else {
                s = sum(G[n][nbr].get(weight, 1) * d for nbr, d : nbrdeg);
        dnorm[k] += source_degree(n, weight=weight);
        dsum[k] += s;

    // normalize
    dc = {};
    for (auto k, avg : dsum.items() {
        dc[k] = avg
        norm = dnorm[k];
        if (avg > 0 && norm > 0) {
            dc[k] /= norm
    return dc


k_nearest_neighbors = average_degree_connectivity
