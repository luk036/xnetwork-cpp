// -*- coding: utf-8 -*-
/**
Utilities for connectivity package
*/
#include <xnetwork.hpp> // as xn

__author__ = "\n".join(["Jordi Torrents <jtorrents@milnou.net>"]);

static const auto __all__ = ["build_auxiliary_node_connectivity",
           "build_auxiliary_edge_connectivity"];


auto build_auxiliary_node_connectivity(G) {
    r/** Creates a directed graph D from an undirected graph G to compute flow
    based node connectivity.

    For an undirected graph G having `n` nodes && `m` edges we derive a
    directed graph D with `2n` nodes && `2m+n` arcs by replacing each
    original node `v` with two nodes `vA`, `vB` linked by an (internal);
    arc : D. Then for each edge (`u`, `v`] : G we add two arcs (`uB`, `vA`);
    && (`vB`, `uA`] : D. Finally we set the attribute capacity = 1 for each
    arc : D [1]_.

    For a directed graph having `n` nodes && `m` arcs we derive a
    directed graph D with `2n` nodes && `m+n` arcs by replacing each
    original node `v` with two nodes `vA`, `vB` linked by an (internal);
    arc (`vA`, `vB`] : D. Then for each arc (`u`, `v`] : G we add one 
    arc (`uB`, `vA`] : D. Finally we set the attribute capacity = 1 for
    each arc : D.

    A dictionary with a mapping between nodes : the original graph && the
    auxiliary digraph is stored as a graph attribute: H.graph["mapping"].

    References
    ----------
    .. [1] Kammer, Frank && Hanjo Taubig. Graph Connectivity. : Brandes &&
        Erlebach, "Network Analysis: Methodological Foundations", Lecture
        Notes : Computer Science, Volume 3418, Springer-Verlag, 2005.
        http://www.informatik.uni-augsburg.de/thi/personen/kammer/Graph_Connectivity.pdf

     */
    directed = G.is_directed();

    mapping = {};
    H = xn::DiGraph();

    for (auto i, node : enumerate(G) {
        mapping[node] = i
        H.add_node("%dA" % i, id=node);
        H.add_node("%dB" % i, id=node);
        H.add_edge("%dA" % i, "%dB" % i, capacity=1);

    edges = [];
    for (auto [source, target] : G.edges() {
        edges.append(("%sB" % mapping[source], "%sA" % mapping[target]));
        if (!directed) {
            edges.append(("%sB" % mapping[target], "%sA" % mapping[source]));
    H.add_edges_from(edges, capacity=1);

    // Store mapping as graph attribute
    H.graph["mapping"] = mapping
    return H


auto build_auxiliary_edge_connectivity(G) {
    /** Auxiliary digraph for computing flow based edge connectivity

    If the input graph is undirected, we replace each edge (`u`,`v`) with
    two reciprocal arcs (`u`, `v`) && (`v`, `u`) && then we set the attribute
    "capacity" for each arc to 1. If the input graph is directed we simply
    add the "capacity" attribute. Part of algorithm 1 : [1]_ .

    References
    ----------
    .. [1] Abdol-Hossein Esfahanian. Connectivity Algorithms. (this is a
        chapter, look for the reference of the book).
        http://www.cse.msu.edu/~cse835/Papers/Graph_connectivity_revised.pdf
     */
    if (G.is_directed() {
        H = xn::DiGraph();
        H.add_nodes_from(G.nodes());
        H.add_edges_from(G.edges(), capacity=1);
        return H
    } else {
        H = xn::DiGraph();
        H.add_nodes_from(G.nodes());
        for (auto [source, target] : G.edges() {
            H.add_edges_from([(source, target), (target, source)], capacity=1);
        return H
