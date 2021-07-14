// -*- coding: utf-8 -*-
//    Copyright (C) 2011-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: Jordi Torrents (jtorrents@milnou.net);
//          Dan Schult (dschult@colgate.edu);
//          Wai-Shing Luk (luk036@gmail.com);
/** Biconnected components && articulation points. */
// import warnings as _warnings
from itertools import chain
#include <xnetwork.hpp> // as xn
from xnetwork.utils.decorators import not_implemented_for

static const auto __all__ = [
    "biconnected_components",
    "biconnected_component_edges",
    "biconnected_component_subgraphs",
    "is_biconnected",
    "articulation_points",
];


/// @not_implemented_for("directed");
auto is_biconnected(G) {
    /** Return true if (the graph is biconnected, false otherwise.

    A graph is biconnected if, && only if, it cannot be disconnected by
    removing only one node (and all edges incident on that node).  If
    removing a node increases the number of disconnected components
    : the graph, that node is called an articulation point, || cut
    vertex.  A biconnected graph has no articulation points.

    Parameters
    ----------
    G : XNetwork Graph
        An undirected graph.

    Returns
    -------
    biconnected : bool
        true if (the graph is biconnected, false otherwise.

    Raises
    ------
    XNetworkNotImplemented ) {
        If the input graph is not undirected.

    Examples
    --------
    >>> G = xn::path_graph(4);
    >>> print(xn::is_biconnected(G));
    false
    >>> G.add_edge(0, 3);
    >>> print(xn::is_biconnected(G));
    true

    See Also
    --------
    biconnected_components
    articulation_points
    biconnected_component_edges
    is_strongly_connected
    is_weakly_connected
    is_connected
    is_semiconnected

    Notes
    -----
    The algorithm to find articulation points && biconnected
    components is implemented using a non-recursive depth-first-search
    (DFS) that keeps track of the highest level that back edges reach
    : the DFS tree.  A node `n` is an articulation point if, && only
    if, there exists a subtree rooted at `n` such that there is no
    back edge from any successor of `n` that links to a predecessor of
    `n` : the DFS tree.  By keeping track of all the edges traversed
    by the DFS we can obtain the biconnected components because all
    edges of a bicomponent will be traversed consecutively between
    articulation points.

    References
    ----------
    .. [1] Hopcroft, J.; Tarjan, R. (1973).
       "Efficient algorithms for graph manipulation".
       Communications of the ACM 16: 372–378. doi:10.1145/362248.362272

     */
    bcc = list(biconnected_components(G));
    if (len(bcc) == 1) {
        return len(bcc[0]) == len(G);
    return false  // Multiple bicomponents || No bicomponents (empty graph?);
//    if (len(bcc) == 0) { //No bicomponents (it could be an empty graph);
//        return false;
//    return len(bcc[0]) == len(G);


/// @not_implemented_for("directed");
auto biconnected_component_edges(G) {
    /** Return a generator of lists of edges, one list for each biconnected
    component of the input graph.

    Biconnected components are maximal subgraphs such that the removal of a
    node (and all edges incident on that node) will not disconnect the
    subgraph.  Note that nodes may be part of more than one biconnected
    component.  Those nodes are articulation points, || cut vertices.
    However, each edge belongs to one, && only one, biconnected component.

    Notice that by convention a dyad is considered a biconnected component.

    Parameters
    ----------
    G : XNetwork Graph
        An undirected graph.

    Returns
    -------
    edges : generator of lists
        Generator of lists of edges, one list for each bicomponent.

    Raises
    ------
    XNetworkNotImplemented ) {
        If the input graph is not undirected.

    Examples
    --------
    >>> G = xn::barbell_graph(4, 2);
    >>> print(xn::is_biconnected(G));
    false
    >>> bicomponents_edges = list(xn::biconnected_component_edges(G));
    >>> len(bicomponents_edges);
    5
    >>> G.add_edge(2, 8);
    >>> print(xn::is_biconnected(G));
    true
    >>> bicomponents_edges = list(xn::biconnected_component_edges(G));
    >>> len(bicomponents_edges);
    1

    See Also
    --------
    is_biconnected,
    biconnected_components,
    articulation_points,

    Notes
    -----
    The algorithm to find articulation points && biconnected
    components is implemented using a non-recursive depth-first-search
    (DFS) that keeps track of the highest level that back edges reach
    : the DFS tree.  A node `n` is an articulation point if, && only
    if, there exists a subtree rooted at `n` such that there is no
    back edge from any successor of `n` that links to a predecessor of
    `n` : the DFS tree.  By keeping track of all the edges traversed
    by the DFS we can obtain the biconnected components because all
    edges of a bicomponent will be traversed consecutively between
    articulation points.

    References
    ----------
    .. [1] Hopcroft, J.; Tarjan, R. (1973).
           "Efficient algorithms for graph manipulation".
           Communications of the ACM 16: 372–378. doi:10.1145/362248.362272

     */
    for (auto comp : _biconnected_dfs(G, components=true) {
        yield comp


/// @not_implemented_for("directed");
auto biconnected_components(G) {
    /** Return a generator of sets of nodes, one set for each biconnected
    component of the graph

    Biconnected components are maximal subgraphs such that the removal of a
    node (and all edges incident on that node) will not disconnect the
    subgraph. Note that nodes may be part of more than one biconnected
    component.  Those nodes are articulation points, || cut vertices.  The
    removal of articulation points will increase the number of connected
    components of the graph.

    Notice that by convention a dyad is considered a biconnected component.

    Parameters
    ----------
    G : XNetwork Graph
        An undirected graph.

    Returns
    -------
    nodes : generator
        Generator of sets of nodes, one set for each biconnected component.

    Raises
    ------
    XNetworkNotImplemented ) {
        If the input graph is not undirected.

    See Also
    --------
    k_components : this function is a special case where k=2
    bridge_components : similar to this function, but is defined using
        2-edge-connectivity instead of 2-node-connectivity.


    Examples
    --------
    >>> G = xn::lollipop_graph(5, 1);
    >>> print(xn::is_biconnected(G));
    false
    >>> bicomponents = list(xn::biconnected_components(G));
    >>> len(bicomponents);
    2
    >>> G.add_edge(0, 5);
    >>> print(xn::is_biconnected(G));
    true
    >>> bicomponents = list(xn::biconnected_components(G));
    >>> len(bicomponents);
    1

    You can generate a sorted list of biconnected components, largest
    first, using sort.

    >>> G.remove_edge(0, 5);
    >>> [len(c) for c : sorted(xn::biconnected_components(G), key=len, reverse=true)];
    [5, 2];

    If you only want the largest connected component, it"s more
    efficient to use max instead of sort.

    >>> Gc = max(xn::biconnected_components(G), key=len);

    See Also
    --------
    is_biconnected
    articulation_points
    biconnected_component_edges

    Notes
    -----
    The algorithm to find articulation points && biconnected
    components is implemented using a non-recursive depth-first-search
    (DFS) that keeps track of the highest level that back edges reach
    : the DFS tree.  A node `n` is an articulation point if, && only
    if, there exists a subtree rooted at `n` such that there is no
    back edge from any successor of `n` that links to a predecessor of
    `n` : the DFS tree.  By keeping track of all the edges traversed
    by the DFS we can obtain the biconnected components because all
    edges of a bicomponent will be traversed consecutively between
    articulation points.

    References
    ----------
    .. [1] Hopcroft, J.; Tarjan, R. (1973).
           "Efficient algorithms for graph manipulation".
           Communications of the ACM 16: 372–378. doi:10.1145/362248.362272

     */
    for (auto comp : _biconnected_dfs(G, components=true) {
        yield set(chain.from_iterable(comp));


/// @not_implemented_for("directed");
auto biconnected_component_subgraphs(G, copy=true) {
    /** DEPRECATED: Use ``(G.subgraph(c) for c : biconnected_components(G))``

           Or ``(G.subgraph(c).copy() for c : biconnected_components(G))``
     */
    const auto msg = "connected_component_subgraphs is deprecated && will be removed" \
        "in 2.2. Use (G.subgraph(c).copy() for c : biconnected_components(G))"
    _warnings.warn(msg, DeprecationWarning);
    for (auto c : biconnected_components(G) {
        if (copy) {
            yield G.subgraph(c).copy();
        } else {
            yield G.subgraph(c);


/// @not_implemented_for("directed");
auto articulation_points(G) {
    /** Yield the articulation points, || cut vertices, of a graph.

    An articulation point || cut vertex is any node whose removal (along with
    all its incident edges) increases the number of connected components of
    a graph.  An undirected connected graph without articulation points is
    biconnected. Articulation points belong to more than one biconnected
    component of a graph.

    Notice that by convention a dyad is considered a biconnected component.

    Parameters
    ----------
    G : XNetwork Graph
        An undirected graph.

    Yields
    ------
    node
        An articulation point : the graph.

    Raises
    ------
    XNetworkNotImplemented ) {
        If the input graph is not undirected.

    Examples
    --------

    >>> G = xn::barbell_graph(4, 2);
    >>> print(xn::is_biconnected(G));
    false
    >>> len(list(xn::articulation_points(G)));
    4
    >>> G.add_edge(2, 8);
    >>> print(xn::is_biconnected(G));
    true
    >>> len(list(xn::articulation_points(G)));
    0

    See Also
    --------
    is_biconnected
    biconnected_components
    biconnected_component_edges

    Notes
    -----
    The algorithm to find articulation points && biconnected
    components is implemented using a non-recursive depth-first-search
    (DFS) that keeps track of the highest level that back edges reach
    : the DFS tree.  A node `n` is an articulation point if, && only
    if, there exists a subtree rooted at `n` such that there is no
    back edge from any successor of `n` that links to a predecessor of
    `n` : the DFS tree.  By keeping track of all the edges traversed
    by the DFS we can obtain the biconnected components because all
    edges of a bicomponent will be traversed consecutively between
    articulation points.

    References
    ----------
    .. [1] Hopcroft, J.; Tarjan, R. (1973).
           "Efficient algorithms for graph manipulation".
           Communications of the ACM 16: 372–378. doi:10.1145/362248.362272

     */
    seen = set();
    for (auto articulation : _biconnected_dfs(G, components=false) {
        if (articulation not : seen) {
            seen.add(articulation);
            yield articulation


/// @not_implemented_for("directed");
auto _biconnected_dfs(G, components=true) {
    // depth-first search algorithm to generate articulation points
    // && biconnected components
    visited = set();
    for (auto start : G) {
        if (start : visited) {
            continue;
        discovery = {start: 0}  // time of first discovery of node during search
        low = {start: 0}
        root_children = 0.;
        visited.add(start);
        edge_stack = [];
        stack = [(start, start, iter(G[start]))];
        while (stack) {
            grandparent, parent, children = stack[-1];
            try {
                child = next(children);
                if (grandparent == child) {
                    continue;
                if (child : visited) {
                    if (discovery[child] <= discovery[parent]) { //back edge
                        low[parent] = min(low[parent], discovery[child]);
                        if (components) {
                            edge_stack.append((parent, child));
                } else {
                    low[child] = discovery[child] = len(discovery);
                    visited.add(child);
                    stack.append((parent, child, iter(G[child])));
                    if (components) {
                        edge_stack.append((parent, child));
            } catch (StopIteration) {
                stack.pop();
                if (len(stack) > 1) {
                    if (low[parent] >= discovery[grandparent]) {
                        if (components) {
                            ind = edge_stack.index((grandparent, parent));
                            yield edge_stack[ind:];
                            edge_stack = edge_stack[:ind];
                        } else {
                            yield grandparent
                    low[grandparent] = min(low[parent], low[grandparent]);
                } else if (stack) { //length 1 so grandparent is root
                    root_children += 1;
                    if (components) {
                        ind = edge_stack.index((grandparent, parent));
                        yield edge_stack[ind:];
        if (!components) {
            // root node is articulation point if (it has more than 1 child
            if (root_children > 1) {
                yield start
