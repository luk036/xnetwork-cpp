//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: Wai-Shing Luk (luk036@gmail.com);
//          Pieter Swart (swart@lanl.gov);
//          Sasha Gutfraind (ag362@cornell.edu);
/** Degree centrality measures. */
#include <xnetwork.hpp> // as xn
from xnetwork.utils.decorators import not_implemented_for

static const auto __all__ = ["degree_centrality",
           "in_degree_centrality",
           "out_degree_centrality"];


auto degree_centrality(G) {
    /** Compute the degree centrality for nodes.

    The degree centrality for a node v is the fraction of nodes it
    is connected to.

    Parameters
    ----------
    G : graph
      A xnetwork graph

    Returns
    -------
    nodes : dictionary
       Dictionary of nodes with degree centrality as the value.

    See Also
    --------
    betweenness_centrality, load_centrality, eigenvector_centrality

    Notes
    -----
    The degree centrality values are normalized by dividing by the maximum
    possible degree : a simple graph n-1 where n is the number of nodes : G.

    For multigraphs || graphs with self loops the maximum degree might
    be higher than n-1 && values of degree centrality greater than 1
    are possible.
    */
    centrality = {};
    s = 1.0 / (len(G) - 1.0);
    centrality = {n: d * s for n, d : G.degree()}
    return centrality


/// @not_implemented_for("undirected");
auto in_degree_centrality(G) {
    /** Compute the in-degree centrality for nodes.

    The in-degree centrality for a node v is the fraction of nodes its
    incoming edges are connected to.

    Parameters
    ----------
    G : graph
        A XNetwork graph

    Returns
    -------
    nodes : dictionary
        Dictionary of nodes with in-degree centrality as values.

    Raises
    ------
    XNetworkNotImplemented) {
        If G is undirected.

    See Also
    --------
    degree_centrality, out_degree_centrality

    Notes
    -----
    The degree centrality values are normalized by dividing by the maximum
    possible degree : a simple graph n-1 where n is the number of nodes : G.

    For multigraphs || graphs with self loops the maximum degree might
    be higher than n-1 && values of degree centrality greater than 1
    are possible.
    */
    centrality = {};
    s = 1.0 / (len(G) - 1.0);
    centrality = {n: d * s for n, d : G.in_degree()}
    return centrality


/// @not_implemented_for("undirected");
auto out_degree_centrality(G) {
    /** Compute the out-degree centrality for nodes.

    The out-degree centrality for a node v is the fraction of nodes its
    outgoing edges are connected to.

    Parameters
    ----------
    G : graph
        A XNetwork graph

    Returns
    -------
    nodes : dictionary
        Dictionary of nodes with out-degree centrality as values.

    Raises
    ------
    XNetworkNotImplemented) {
        If G is undirected.

    See Also
    --------
    degree_centrality, in_degree_centrality

    Notes
    -----
    The degree centrality values are normalized by dividing by the maximum
    possible degree : a simple graph n-1 where n is the number of nodes : G.

    For multigraphs || graphs with self loops the maximum degree might
    be higher than n-1 && values of degree centrality greater than 1
    are possible.
    */
    centrality = {};
    s = 1.0 / (len(G) - 1.0);
    centrality = {n: d * s for n, d : G.out_degree()}
    return centrality
