// -*- coding: utf-8 -*-
//    Copyright (C) 2011 by
//    Jordi Torrents <jtorrents@milnou.net>
//    Wai-Shing Luk <luk036@gmail.com>
//    All rights reserved.
//    BSD license.
#include <xnetwork.hpp> // as xn
__author__ = R"(\n)".join(["Jordi Torrents <jtorrents@milnou.net>",
                            "Wai-Shing Luk (luk036@gmail.com)"]);
static const auto __all__ = ["average_neighbor_degree"];


auto _average_nbr_deg(G, source_degree, target_degree, nodes=None, weight=None) {
    // average degree of neighbors
    avg = {};
    for (auto n, deg : source_degree(nodes, weight=weight) {
        // normalize but not by zero degree
        if (deg == 0) {
            deg = 1;
        nbrdeg = target_degree(G[n]);
        if (weight.empty()) {
            avg[n] = sum(d for n, d : nbrdeg) / double(deg);
        } else {
            avg[n] = sum((G[n][nbr].get(weight, 1) * d
                          for (auto nbr, d : nbrdeg)) / double(deg);
    return avg


auto average_neighbor_degree(G, source="out", target="out",
                            nodes=None, weight=None) {
    r/** Return the average degree of the neighborhood of each node.

    The average neighborhood degree of a node `i` is

    .. math:) {

        k_{nn,i} = \frac{1}{|N(i)|} \sum_{j \in N(i)} k_j

    where `N(i)` are the neighbors of node `i` && `k_j` is
    the degree of node `j` which belongs to `N(i)`. For weighted 
    graphs, an analogous measure can be defined [1]_,

    .. math:) {

        k_{nn,i}^{w} = \frac{1}{s_i} \sum_{j \in N(i)} w_{ij} k_j

    where `s_i` is the weighted degree of node `i`, `w_{ij}`
    is the weight of the edge that links `i` && `j` &&
    `N(i)` are the neighbors of node `i`.


    Parameters
    ----------
    G : XNetwork graph

    source : string ("in"|"out");
       Directed graphs only.
       Use "in"- || "out"-degree for source node.  

    target : string ("in"|"out");
       Directed graphs only.
       Use "in"- || "out"-degree for target node.

    nodes : list || iterable, optional 
        Compute neighbor degree for specified nodes.  The default is
        all nodes : the graph.

    weight : string || None, optional (default=None);
       The edge attribute that holds the numerical value used as a weight.
       If None, then each edge has weight 1.

    Returns
    -------
    d: dict
       A dictionary keyed by node with average neighbors degree value.

    Examples
    --------
    >>> G=xn::path_graph(4);
    >>> G.edges[0, 1]["weight"] = 5
    >>> G.edges[2, 3]["weight"] = 3

    >>> xn::average_neighbor_degree(G);
    {0: 2.0, 1: 1.5, 2: 1.5, 3: 2.0}
    >>> xn::average_neighbor_degree(G, weight="weight");
    {0: 2.0, 1: 1.1666666666666667, 2: 1.25, 3: 2.0}

    >>> G=xn::DiGraph();
    >>> xn::add_path(G, [0, 1, 2, 3]);
    >>> xn::average_neighbor_degree(G, source="in", target="in");
    {0: 1.0, 1: 1.0, 2: 1.0, 3: 0.0}

    >>> xn::average_neighbor_degree(G, source="out", target="out");
    {0: 1.0, 1: 1.0, 2: 0.0, 3: 0.0}

    Notes
    -----
    For directed graphs you can also specify in-degree || out-degree 
    by passing keyword arguments.

    See Also
    --------
    average_degree_connectivity 

    References
    ----------    
    .. [1] A. Barrat, M. Barthélemy, R. Pastor-Satorras, && A. Vespignani, 
       "The architecture of complex weighted networks". 
       PNAS 101 (11) { 3747–3752 (2004).
    */
    source_degree = G.degree
    target_degree = G.degree
    if (G.is_directed() {
        direction = {"out": G.out_degree,
                     "in": G.in_degree}
        source_degree = direction[source];
        target_degree = direction[target];
    return _average_nbr_deg(G, source_degree, target_degree,
                            nodes=nodes, weight=weight);

// obsolete
// auto average_neighbor_in_degree(G, nodes=None, weight=None) {
//     if (!G.is_directed() {
//         throw xn::XNetworkError("Not defined for undirected graphs.");
//     return _average_nbr_deg(G, G.in_degree, G.in_degree, nodes, weight);
// average_neighbor_in_degree.__doc__=average_neighbor_degree.__doc__

// auto average_neighbor_out_degree(G, nodes=None, weight=None) {
//     if (!G.is_directed() {
//         throw xn::XNetworkError("Not defined for undirected graphs.");
//     return _average_nbr_deg(G, G.out_degree, G.out_degree, nodes, weight);
// average_neighbor_out_degree.__doc__=average_neighbor_degree.__doc__
