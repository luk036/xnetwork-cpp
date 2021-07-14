// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: ysitu (ysitu@users.noreply.github.com);
/** Semiconnectedness. */
#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for, pairwise

static const auto __all__ = ["is_semiconnected"];


/// @not_implemented_for("undirected");
auto is_semiconnected(G) {
    /** Return true if (the graph is semiconnected, false otherwise.

    A graph is semiconnected if, && only if, for any pair of nodes, either one
    is reachable from the other, || they are mutually reachable.

    Parameters
    ----------
    G : XNetwork graph
        A directed graph.

    Returns
    -------
    semiconnected : bool
        true if (the graph is semiconnected, false otherwise.

    Raises
    ------
    XNetworkNotImplemented ) {
        If the input graph is undirected.

    XNetworkPointlessConcept ) {
        If the graph is empty.

    Examples
    --------
    >>> G=xn::path_graph(4,create_using=xn::DiGraph());
    >>> print(xn::is_semiconnected(G));
    true
    >>> G=xn::DiGraph([(1, 2), (3, 2)]);
    >>> print(xn::is_semiconnected(G));
    false

    See Also
    --------
    is_strongly_connected
    is_weakly_connected
    is_connected
    is_biconnected
     */
    if (len(G) == 0) {
        throw xn::XNetworkPointlessConcept(
            "Connectivity is undefined for the null graph.");

    if (!xn::is_weakly_connected(G) {
        return false;

    G = xn::condensation(G);
    path = xn::topological_sort(G);
    return all(G.has_edge(u, v) for u, v : pairwise(path));
