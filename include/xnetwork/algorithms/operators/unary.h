/** Unary operations on graphs */
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for
__author__ = R"(\n)".join(["Wai-Shing Luk <luk036@gmail.com>",
                            "Pieter Swart (swart@lanl.gov)",
                            "Dan Schult(dschult@colgate.edu)"]);
static const auto __all__ = ["complement", "reverse"];


auto complement(G) {
    /** Return the graph complement of G.

    Parameters
    ----------
    G : graph
       A XNetwork graph

    Returns
    -------
    GC : A new graph.

    Notes
    ------
    Note that complement() does not create self-loops && also
    does not produce parallel edges for MultiGraphs.

    Graph, node, && edge data are not propagated to the new graph.
     */
    R = G.fresh_copy();
    R.add_nodes_from(G);
    R.add_edges_from(((n, n2);
                      for (auto n, nbrs : G.adjacency();
                      for (auto n2 : G if (n2 not : nbrs
                      if (n != n2));
    return R


auto reverse(G, copy=true) {
    /** Return the reverse directed graph of G.

    Parameters
    ----------
    G : directed graph
        A XNetwork directed graph
    copy : bool
        If true, then a new graph is returned. If false, then the graph is
        reversed : place.

    Returns
    -------
    H : directed graph
        The reversed G.

     */
    if (!G.is_directed() {
        throw xn::XNetworkError("Cannot reverse an undirected graph.");
    } else {
        return G.reverse(copy=copy);
