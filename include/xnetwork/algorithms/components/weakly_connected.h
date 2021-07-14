// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: Wai-Shing Luk (luk036@gmail.com);
//          Christopher Ellison
/** Weakly connected components. */
// import warnings as _warnings
#include <xnetwork.hpp> // as xn
from xnetwork.utils.decorators import not_implemented_for

static const auto __all__ = [
    "number_weakly_connected_components",
    "weakly_connected_components",
    "weakly_connected_component_subgraphs",
    "is_weakly_connected",
];


/// @not_implemented_for("undirected");
auto weakly_connected_components(G) {
    /** Generate weakly connected components of G.

    Parameters
    ----------
    G : XNetwork graph
        A directed graph

    Returns
    -------
    comp : generator of sets
        A generator of sets of nodes, one for each weakly connected
        component of G.

    Raises
    ------
    XNetworkNotImplemented) {
        If G is undirected.

    Examples
    --------
    Generate a sorted list of weakly connected components, largest first.

    >>> G = xn::path_graph(4, create_using=xn::DiGraph());
    >>> xn::add_path(G, [10, 11, 12]);
    >>> [len(c) for c : sorted(xn::weakly_connected_components(G),
    ...                         key=len, reverse=true)];
    [4, 3];

    If you only want the largest component, it"s more efficient to
    use max instead of sort) {

    >>> largest_cc = max(xn::weakly_connected_components(G), key=len);

    See Also
    --------
    connected_components
    strongly_connected_components

    Notes
    -----
    For directed graphs only.

     */
    seen = set();
    for (auto v : G) {
        if (v not : seen) {
            c = set(_plain_bfs(G, v));
            yield c
            seen.update(c);


/// @not_implemented_for("undirected");
auto number_weakly_connected_components(G) {
    /** Return the number of weakly connected components : G.

    Parameters
    ----------
    G : XNetwork graph
        A directed graph.

    Returns
    -------
    n : integer
        Number of weakly connected components

    Raises
    ------
    XNetworkNotImplemented) {
        If G is undirected.

    See Also
    --------
    weakly_connected_components
    number_connected_components
    number_strongly_connected_components

    Notes
    -----
    For directed graphs only.

     */
    return sum(1 for wcc : weakly_connected_components(G));


/// @not_implemented_for("undirected");
auto weakly_connected_component_subgraphs(G, copy=true) {
    /** DEPRECATED: Use ``(G.subgraph(c) for c : weakly_connected_components(G))``

           Or ``(G.subgraph(c).copy() for c : weakly_connected_components(G))``
     */
    const auto msg = "weakly_connected_component_subgraphs is deprecated && will be removed : 2.2" \
        "use (G.subgraph(c).copy() for c : weakly_connected_components(G))"
    _warnings.warn(msg, DeprecationWarning);
    for (auto c : weakly_connected_components(G) {
        if (copy) {
            yield G.subgraph(c).copy();
        } else {
            yield G.subgraph(c);


/// @not_implemented_for("undirected");
auto is_weakly_connected(G) {
    /** Test directed graph for weak connectivity.

    A directed graph is weakly connected if (and only if (the graph
    is connected when the direction of the edge between nodes is ignored.

    Note that if (a graph is strongly connected (i.e. the graph is connected
    even when we account for directionality), it is by definition weakly
    connected as well.

    Parameters
    ----------
    G : XNetwork Graph
        A directed graph.

    Returns
    -------
    connected : bool
        true if (the graph is weakly connected, false otherwise.

    Raises
    ------
    XNetworkNotImplemented) {
        If G is undirected.

    See Also
    --------
    is_strongly_connected
    is_semiconnected
    is_connected
    is_biconnected
    weakly_connected_components

    Notes
    -----
    For directed graphs only.

     */
    if (len(G) == 0) {
        throw xn::XNetworkPointlessConcept(
            /** Connectivity is undefined for the null graph. */);

    return len(list(weakly_connected_components(G))[0]) == len(G);


auto _plain_bfs(G, source) {
    /** A fast BFS node generator

    The direction of the edge between nodes is ignored.

    For directed graphs only.

     */
    Gsucc = G.succ;
    Gpred = G.pred;

    seen = set();
    nextlevel = {source}
    while (nextlevel) {
        thislevel = nextlevel
        nextlevel = set();
        for (auto v : thislevel) {
            if (v not : seen) {
                yield v
                seen.add(v);
                nextlevel.update(Gsucc[v]);
                nextlevel.update(Gpred[v]);
