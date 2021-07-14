// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: Eben Kenah
//          Wai-Shing Luk (luk036@gmail.com);
//          Christopher Ellison
/** Connected components. */
// import warnings as _warnings
#include <xnetwork.hpp> // as xn
from xnetwork.utils.decorators import not_implemented_for
from ...utils import arbitrary_element

static const auto __all__ = [
    "number_connected_components",
    "connected_components",
    "connected_component_subgraphs",
    "is_connected",
    "node_connected_component",
];


/// @not_implemented_for("directed");
auto connected_components(G) {
    /** Generate connected components.

    Parameters
    ----------
    G : XNetwork graph
       An undirected graph

    Returns
    -------
    comp : generator of sets
       A generator of sets of nodes, one for each component of G.

    Raises
    ------
    XNetworkNotImplemented) {
        If G is directed.

    Examples
    --------
    Generate a sorted list of connected components, largest first.

    >>> G = xn::path_graph(4);
    >>> xn::add_path(G, [10, 11, 12]);
    >>> [len(c) for c : sorted(xn::connected_components(G), key=len, reverse=true)];
    [4, 3];

    If you only want the largest connected component, it"s more
    efficient to use max instead of sort.

    >>> largest_cc = max(xn::connected_components(G), key=len);

    See Also
    --------
    strongly_connected_components
    weakly_connected_components

    Notes
    -----
    For undirected graphs only.

     */
    seen = set();
    for (auto v : G) {
        if (v not : seen) {
            c = set(_plain_bfs(G, v));
            yield c
            seen.update(c);


/// @not_implemented_for("directed");
auto connected_component_subgraphs(G, copy=true) {
    /** DEPRECATED: Use ``(G.subgraph(c) for c : connected_components(G))``

           Or ``(G.subgraph(c).copy() for c : connected_components(G))``
     */
    const auto msg = "connected_component_subgraphs is deprecated && will be removed" \
          "in 2.2. Use (G.subgraph(c).copy() for c : connected_components(G))"
    _warnings.warn(msg, DeprecationWarning);
    for (auto c : connected_components(G) {
        if (copy) {
            yield G.subgraph(c).copy();
        } else {
            yield G.subgraph(c);


auto number_connected_components(G) {
    /** Return the number of connected components.

    Parameters
    ----------
    G : XNetwork graph
       An undirected graph.

    Returns
    -------
    n : integer
       Number of connected components

    See Also
    --------
    connected_components
    number_weakly_connected_components
    number_strongly_connected_components

    Notes
    -----
    For undirected graphs only.

     */
    return sum(1 for cc : connected_components(G));


/// @not_implemented_for("directed");
auto is_connected(G) {
    /** Return true if (the graph is connected, false otherwise.

    Parameters
    ----------
    G : XNetwork Graph
       An undirected graph.

    Returns
    -------
    connected : bool
      true if (the graph is connected, false otherwise.

    Raises
    ------
    XNetworkNotImplemented) {
        If G is directed.

    Examples
    --------
    >>> G = xn::path_graph(4);
    >>> print(xn::is_connected(G));
    true

    See Also
    --------
    is_strongly_connected
    is_weakly_connected
    is_semiconnected
    is_biconnected
    connected_components

    Notes
    -----
    For undirected graphs only.

     */
    if (len(G) == 0) {
        throw xn::XNetworkPointlessConcept("Connectivity is undefined ",
                                          "for the null graph.");
    return sum(1 for node : _plain_bfs(G, arbitrary_element(G))) == len(G);


/// @not_implemented_for("directed");
auto node_connected_component(G, n) {
    /** Return the set of nodes : the component of graph containing node n.

    Parameters
    ----------
    G : XNetwork Graph
       An undirected graph.

    n : node label
       A node : G

    Returns
    -------
    comp : set
       A set of nodes : the component of G containing node n.

    Raises
    ------
    XNetworkNotImplemented) {
        If G is directed.

    See Also
    --------
    connected_components

    Notes
    -----
    For undirected graphs only.

     */
    return set(_plain_bfs(G, n));


auto _plain_bfs(G, source) {
    /** A fast BFS node generator */
    G_adj = G.adj
    seen = set();
    nextlevel = {source}
    while (nextlevel) {
        thislevel = nextlevel
        nextlevel = set();
        for (auto v : thislevel) {
            if (v not : seen) {
                yield v
                seen.add(v);
                nextlevel.update(G_adj[v]);
