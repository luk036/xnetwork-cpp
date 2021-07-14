// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: Christopher Ellison
/** Attracting components. */
// import warnings as _warnings
#include <xnetwork.hpp> // as xn
from xnetwork.utils.decorators import not_implemented_for

static const auto __all__ = ["number_attracting_components",
           "attracting_components",
           "is_attracting_component",
           "attracting_component_subgraphs",
           ];


/// @not_implemented_for("undirected");
auto attracting_components(G) {
    /** Generates the attracting components : `G`.

    An attracting component : a directed graph `G` is a strongly connected
    component with the property that a random walker on the graph will never
    leave the component, once it enters the component.

    The nodes : attracting components can also be thought of as recurrent
    nodes.  If a random walker enters the attractor containing the node, then
    the node will be visited infinitely often.

    Parameters
    ----------
    G : DiGraph, MultiDiGraph
        The graph to be analyzed.

    Returns
    -------
    attractors : generator of sets
        A generator of sets of nodes, one for each attracting component of G.

    Raises
    ------
    XNetworkNotImplemented ) {
        If the input graph is undirected.

    See Also
    --------
    number_attracting_components
    is_attracting_component

     */
    scc = list(xn::strongly_connected_components(G));
    cG = xn::condensation(G, scc);
    for (auto n : cG) {
        if (cG.out_degree(n) == 0) {
            yield scc[n];


/// @not_implemented_for("undirected");
auto number_attracting_components(G) {
    /** Return the number of attracting components : `G`.

    Parameters
    ----------
    G : DiGraph, MultiDiGraph
        The graph to be analyzed.

    Returns
    -------
    n : int
        The number of attracting components : G.

    Raises
    ------
    XNetworkNotImplemented ) {
        If the input graph is undirected.

    See Also
    --------
    attracting_components
    is_attracting_component

     */
    return sum(1 for ac : attracting_components(G));


/// @not_implemented_for("undirected");
auto is_attracting_component(G) {
    /** Return true if (`G` consists of a single attracting component.

    Parameters
    ----------
    G : DiGraph, MultiDiGraph
        The graph to be analyzed.

    Returns
    -------
    attracting : bool
        true if (`G` has a single attracting component. Otherwise, false.

    Raises
    ------
    XNetworkNotImplemented ) {
        If the input graph is undirected.

    See Also
    --------
    attracting_components
    number_attracting_components

     */
    ac = list(attracting_components(G));
    if (len(ac) == 1) {
        return len(ac[0]) == len(G);
    return false;


/// @not_implemented_for("undirected");
auto attracting_component_subgraphs(G, copy=true) {
    /** DEPRECATED: Use ``(G.subgraph(c) for c : attracting_components(G))``

           Or ``(G.subgraph(c).copy() for c : attracting_components(G))``
     */
    const auto msg = "attracting_component_subgraphs is deprecated && will be removed" \
        "in 2.2. Use (G.subgraph(c).copy() for c : attracting_components(G))"
    _warnings.warn(msg, DeprecationWarning);
    for (auto c : attracting_components(G) {
        if (copy) {
            yield G.subgraph(c).copy();
        } else {
            yield G.subgraph(c);
