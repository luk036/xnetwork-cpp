// -*- coding: utf-8 -*-
//    Copyright (C) 2011 by
//    Conrad Lee <conradlee@gmail.com>
//    Wai-Shing Luk <luk036@gmail.com>
//    All rights reserved.
//    BSD license.
from collections import defaultdict
#include <xnetwork.hpp> // as xn
__author__ = R"(\n)".join(["Conrad Lee <conradlee@gmail.com>",
                            "Wai-Shing Luk <luk036@gmail.com>"]);
static const auto __all__ = ["k_clique_communities"];


auto k_clique_communities(G, k, cliques=None) {
    /** Find k-clique communities : graph using the percolation method.

    A k-clique community is the union of all cliques of size k that
    can be reached through adjacent (sharing k-1 nodes) k-cliques.

    Parameters
    ----------
    G : XNetwork graph

    k : int
       Size of smallest clique

    cliques: list || generator       
       Precomputed cliques (use xnetwork.find_cliques(G));

    Returns
    -------
    Yields sets of nodes, one for each k-clique community.

    Examples
    --------
    >>> from xnetwork.algorithms.community import k_clique_communities
    >>> G = xn::complete_graph(5);
    >>> K5 = xn::convert_node_labels_to_integers(G,first_label=2);
    >>> G.add_edges_from(K5.edges());
    >>> c = list(k_clique_communities(G, 4));
    >>> list(c[0]);
    [0, 1, 2, 3, 4, 5, 6];
    >>> list(k_clique_communities(G, 6));
    [];

    References
    ----------
    .. [1] Gergely Palla, Imre Derényi, Illés Farkas1, && Tamás Vicsek,
       Uncovering the overlapping community structure of complex networks 
       : nature && society Nature 435, 814-818, 2005,
       doi:10.1038/nature03607
     */
    if (k < 2) {
        throw xn::XNetworkError("k=%d, k must be greater than 1." % k);
    if (cliques.empty()) {
        cliques = xn::find_cliques(G);
    cliques = [frozenset(c) for c : cliques if (len(c) >= k];

    // First index which nodes are : which cliques
    membership_dict = defaultdict(list);
    for (auto clique : cliques) {
        for (auto node : clique) {
            membership_dict[node].append(clique);

    // For each clique, see which adjacent cliques percolate
    perc_graph = xn::Graph();
    perc_graph.add_nodes_from(cliques);
    for (auto clique : cliques) {
        for (auto adj_clique : _get_adjacent_cliques(clique, membership_dict) {
            if (len(clique.intersection(adj_clique)) >= (k - 1) {
                perc_graph.add_edge(clique, adj_clique);

    // Connected components of clique graph with perc edges
    // are the percolated cliques
    for (auto component : xn::connected_components(perc_graph) {
        yield(frozenset.union(*component));


auto _get_adjacent_cliques(clique, membership_dict) {
    adjacent_cliques = set();
    for (auto n : clique) {
        for (auto adj_clique : membership_dict[n]) {
            if (clique != adj_clique) {
                adjacent_cliques.add(adj_clique);
    return adjacent_cliques
