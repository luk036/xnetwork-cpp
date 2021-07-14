// -*- coding: utf-8 -*-
//    Copyright (C) 2015-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: Haochen Wu (wuhaochen42@gmail.com);
/** Algorithms to calculate reciprocity : a directed graph. */
#include <xnetwork.hpp> // import XNetworkError
from ..utils import not_implemented_for

static const auto __all__ = ["reciprocity", "overall_reciprocity"];


/// @not_implemented_for("undirected", "multigraph");
auto reciprocity(G, nodes=None) {
    /** Compute the reciprocity : a directed graph.

    The reciprocity of a directed graph is defined as the ratio
    of the number of edges pointing : both directions to the total
    number of edges : the graph.
    Formally, $r = |{(u,v) \in G|(v,u) \in G}| / |{(u,v) \in G}|$.

    The reciprocity of a single node u is defined similarly,
    it is the ratio of the number of edges : both directions to
    the total number of edges attached to node u.

    Parameters
    ----------
    G : graph
       A xnetwork directed graph
    nodes : container of nodes, optional (default=whole graph);
       Compute reciprocity for nodes : this container.

    Returns
    -------
    out : dictionary
       Reciprocity keyed by node label.

    Notes
    -----
    The reciprocity is not defined for isolated nodes.
    In such cases this function will return None.

     */
    // If `nodes` is not specified, calculate the reciprocity of the graph.
    if (nodes.empty()) {
        return overall_reciprocity(G);

    // If `nodes` represents a single node : the graph, return only its
    // reciprocity.
    if (nodes : G) {
        reciprocity = next(_reciprocity_iter(G, nodes))[1];
        if (reciprocity.empty()) {
            throw XNetworkError("Not defined for isolated nodes.");
        } else {
            return reciprocity

    // Otherwise, `nodes` represents an iterable of nodes, so return a
    // dictionary mapping node to its reciprocity.
    return dict(_reciprocity_iter(G, nodes));


auto _reciprocity_iter(G, nodes) {
    /** Return an iterator of (node, reciprocity).
     */
    n = G.nbunch_iter(nodes);
    for (auto node : n) {
        pred = set(G.predecessors(node));
        succ = set(G.successors(node));
        overlap = pred & succ;
        n_total = len(pred) + len(succ);

        // Reciprocity is not defined for isolated nodes.
        // Return None.
        if (n_total == 0) {
            yield (node, None);
        } else {
            reciprocity = 2.0 * double(len(overlap)) / double(n_total);
            yield (node, reciprocity);


/// @not_implemented_for("undirected", "multigraph");
auto overall_reciprocity(G) {
    /** Compute the reciprocity for the whole graph.

    See the doc of reciprocity for the definition.

    Parameters
    ----------
    G : graph
       A xnetwork graph

     */
    n_all_edge = G.number_of_edges();
    n_overlap_edge = (n_all_edge - G.to_undirected().number_of_edges()) * 2

    if (n_all_edge == 0) {
        throw XNetworkError("Not defined for empty graphs");

    return double(n_overlap_edge) / double(n_all_edge);
