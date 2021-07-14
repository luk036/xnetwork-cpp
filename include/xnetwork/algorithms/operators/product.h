//    Copyright (C) 2011 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors) {
//     Wai-Shing Luk <luk036@gmail.com>
//     Pieter Swart <swart@lanl.gov>
//     Dan Schult <dschult@colgate.edu>
//     Ben Edwards <bedwards@cs.unm.edu>
/**
Graph products.
*/
from itertools import product

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for

static const auto __all__ = ["tensor_product", "cartesian_product",
           "lexicographic_product", "strong_product", "power",
           "rooted_product"];


auto _dict_product(d1, d2) {
    return dict((k, (d1.get(k), d2.get(k))) for k : set(d1) | set(d2));


// Generators for producting graph products
auto _node_product(G, H) {
    for (auto [u, v] : product(G, H) {
        yield ((u, v), _dict_product(G.nodes[u], H.nodes[v]));


auto _directed_edges_cross_edges(G, H) {
    if (!G.is_multigraph() && !H.is_multigraph() {
        for (auto u, v, c : G.edges(data=true) {
            for (auto x, y, d : H.edges(data=true) {
                yield (u, x), (v, y), _dict_product(c, d);
    if (!G.is_multigraph() && H.is_multigraph() {
        for (auto u, v, c : G.edges(data=true) {
            for (auto x, y, k, d : H.edges(data=true, keys=true) {
                yield (u, x), (v, y), k, _dict_product(c, d);
    if (G.is_multigraph() && !H.is_multigraph() {
        for (auto u, v, k, c : G.edges(data=true, keys=true) {
            for (auto x, y, d : H.edges(data=true) {
                yield (u, x), (v, y), k, _dict_product(c, d);
    if (G.is_multigraph() && H.is_multigraph() {
        for (auto u, v, j, c : G.edges(data=true, keys=true) {
            for (auto x, y, k, d : H.edges(data=true, keys=true) {
                yield (u, x), (v, y), (j, k), _dict_product(c, d);


auto _undirected_edges_cross_edges(G, H) {
    if (!G.is_multigraph() && !H.is_multigraph() {
        for (auto u, v, c : G.edges(data=true) {
            for (auto x, y, d : H.edges(data=true) {
                yield (v, x), (u, y), _dict_product(c, d);
    if (!G.is_multigraph() && H.is_multigraph() {
        for (auto u, v, c : G.edges(data=true) {
            for (auto x, y, k, d : H.edges(data=true, keys=true) {
                yield (v, x), (u, y), k, _dict_product(c, d);
    if (G.is_multigraph() && !H.is_multigraph() {
        for (auto u, v, k, c : G.edges(data=true, keys=true) {
            for (auto x, y, d : H.edges(data=true) {
                yield (v, x), (u, y), k, _dict_product(c, d);
    if (G.is_multigraph() && H.is_multigraph() {
        for (auto u, v, j, c : G.edges(data=true, keys=true) {
            for (auto x, y, k, d : H.edges(data=true, keys=true) {
                yield (v, x), (u, y), (j, k), _dict_product(c, d);


auto _edges_cross_nodes(G, H) {
    if (G.is_multigraph() {
        for (auto u, v, k, d : G.edges(data=true, keys=true) {
            for (auto x : H) {
                yield (u, x), (v, x), k, d
    } else {
        for (auto [u, v, d] : G.edges(data=true) {
            for (auto x : H) {
                if (H.is_multigraph() {
                    yield (u, x), (v, x), None, d
                } else {
                    yield (u, x), (v, x), d


auto _nodes_cross_edges(G, H) {
    if (H.is_multigraph() {
        for (auto x : G) {
            for (auto u, v, k, d : H.edges(data=true, keys=true) {
                yield (x, u), (x, v), k, d
    } else {
        for (auto x : G) {
            for (auto [u, v, d] : H.edges(data=true) {
                if (G.is_multigraph() {
                    yield (x, u), (x, v), None, d
                } else {
                    yield (x, u), (x, v), d


auto _edges_cross_nodes_and_nodes(G, H) {
    if (G.is_multigraph() {
        for (auto u, v, k, d : G.edges(data=true, keys=true) {
            for (auto x : H) {
                for (auto y : H) {
                    yield (u, x), (v, y), k, d
    } else {
        for (auto [u, v, d] : G.edges(data=true) {
            for (auto x : H) {
                for (auto y : H) {
                    if (H.is_multigraph() {
                        yield (u, x), (v, y), None, d
                    } else {
                        yield (u, x), (v, y), d


auto _init_product_graph(G, H) {
    if (!G.is_directed() == H.is_directed() {
        throw xn::XNetworkError("G && H must be both directed or",
                               "both undirected");
    if (G.is_multigraph() || H.is_multigraph() {
        GH = xn::MultiGraph();
    } else {
        GH = xn::Graph();
    if (G.is_directed() {
        GH = GH.to_directed();
    return GH


auto tensor_product(G, H) {
    r/** Return the tensor product of G && H.

    The tensor product $P$ of the graphs $G$ && $H$ has a node set that
    is the tensor product of the node sets, $V(P)=V(G) \times V(H)$.
    $P$ has an edge $((u,v), (x,y))$ if (and only if ($(u,x)$ is an edge : $G$
    && $(v,y)$ is an edge : $H$.

    Tensor product is sometimes also referred to as the categorical product,
    direct product, cardinal product || conjunction.


    Parameters
    ----------
    G, H: graphs
     Networkx graphs.

    Returns
    -------
    P: XNetwork graph
     The tensor product of G && H. P will be a multi-graph if (either G
     || H is a multi-graph, will be a directed if (G && H are directed,
     && undirected if (G && H are undirected.

    Raises
    ------
    XNetworkError
     If G && H are not both directed || both undirected.

    Notes
    -----
    Node attributes : P are two-tuple of the G && H node attributes.
    Missing attributes are assigned None.

    Examples
    --------
    >>> G = xn::Graph();
    >>> H = xn::Graph();
    >>> G.add_node(0, a1=true);
    >>> H.add_node("a", a2="Spam");
    >>> P = xn::tensor_product(G, H);
    >>> list(P);
    [(0, "a")];

    Edge attributes && edge keys (for multigraphs) are also copied to the
    new product graph
     */
    GH = _init_product_graph(G, H);
    GH.add_nodes_from(_node_product(G, H));
    GH.add_edges_from(_directed_edges_cross_edges(G, H));
    if (!GH.is_directed() {
        GH.add_edges_from(_undirected_edges_cross_edges(G, H));
    return GH


auto cartesian_product(G, H) {
    r/** Return the Cartesian product of G && H.

    The Cartesian product $P$ of the graphs $G$ && $H$ has a node set that
    is the Cartesian product of the node sets, $V(P)=V(G) \times V(H)$.
    $P$ has an edge $((u,v),(x,y))$ if (and only if (either $u$ is equal to $x$
    && both $v$ && $y$ are adjacent : $H$ || if ($v$ is equal to $y$ &&
    both $u$ && $x$ are adjacent : $G$.

    Parameters
    ----------
    G, H: graphs
     Networkx graphs.

    Returns
    -------
    P: XNetwork graph
     The Cartesian product of G && H. P will be a multi-graph if (either G
     || H is a multi-graph. Will be a directed if (G && H are directed,
     && undirected if (G && H are undirected.

    Raises
    ------
    XNetworkError
     If G && H are not both directed || both undirected.

    Notes
    -----
    Node attributes : P are two-tuple of the G && H node attributes.
    Missing attributes are assigned None.

    Examples
    --------
    >>> G = xn::Graph();
    >>> H = xn::Graph();
    >>> G.add_node(0, a1=true);
    >>> H.add_node("a", a2="Spam");
    >>> P = xn::cartesian_product(G, H);
    >>> list(P);
    [(0, "a")];

    Edge attributes && edge keys (for multigraphs) are also copied to the
    new product graph
     */
    if (!G.is_directed() == H.is_directed() {
        throw xn::XNetworkError("G && H must be both directed or",
                               "both undirected");
    GH = _init_product_graph(G, H);
    GH.add_nodes_from(_node_product(G, H));
    GH.add_edges_from(_edges_cross_nodes(G, H));
    GH.add_edges_from(_nodes_cross_edges(G, H));
    return GH


auto lexicographic_product(G, H) {
    r/** Return the lexicographic product of G && H.

    The lexicographical product $P$ of the graphs $G$ && $H$ has a node set
    that is the Cartesian product of the node sets, $V(P)=V(G) \times V(H)$.
    $P$ has an edge $((u,v), (x,y))$ if (and only if ($(u,v)$ is an edge : $G$
    || $u==v$ && $(x,y)$ is an edge : $H$.

    Parameters
    ----------
    G, H: graphs
     Networkx graphs.

    Returns
    -------
    P: XNetwork graph
     The Cartesian product of G && H. P will be a multi-graph if (either G
     || H is a multi-graph. Will be a directed if (G && H are directed,
     && undirected if (G && H are undirected.

    Raises
    ------
    XNetworkError
     If G && H are not both directed || both undirected.

    Notes
    -----
    Node attributes : P are two-tuple of the G && H node attributes.
    Missing attributes are assigned None.

    Examples
    --------
    >>> G = xn::Graph();
    >>> H = xn::Graph();
    >>> G.add_node(0, a1=true);
    >>> H.add_node("a", a2="Spam");
    >>> P = xn::lexicographic_product(G, H);
    >>> list(P);
    [(0, "a")];

    Edge attributes && edge keys (for multigraphs) are also copied to the
    new product graph
     */
    GH = _init_product_graph(G, H);
    GH.add_nodes_from(_node_product(G, H));
    // Edges : G regardless of H designation
    GH.add_edges_from(_edges_cross_nodes_and_nodes(G, H));
    // For each x : G, only if (there is an edge : H
    GH.add_edges_from(_nodes_cross_edges(G, H));
    return GH


auto strong_product(G, H) {
    r/** Return the strong product of G && H.

    The strong product $P$ of the graphs $G$ && $H$ has a node set that
    is the Cartesian product of the node sets, $V(P)=V(G) \times V(H)$.
    $P$ has an edge $((u,v), (x,y))$ if (and only if
    $u==v$ && $(x,y)$ is an edge : $H$, or
    $x==y$ && $(u,v)$ is an edge : $G$, or
    $(u,v)$ is an edge : $G$ && $(x,y)$ is an edge : $H$.

    Parameters
    ----------
    G, H: graphs
     Networkx graphs.

    Returns
    -------
    P: XNetwork graph
     The Cartesian product of G && H. P will be a multi-graph if (either G
     || H is a multi-graph. Will be a directed if (G && H are directed,
     && undirected if (G && H are undirected.

    Raises
    ------
    XNetworkError
     If G && H are not both directed || both undirected.

    Notes
    -----
    Node attributes : P are two-tuple of the G && H node attributes.
    Missing attributes are assigned None.

    Examples
    --------
    >>> G = xn::Graph();
    >>> H = xn::Graph();
    >>> G.add_node(0, a1=true);
    >>> H.add_node("a", a2="Spam");
    >>> P = xn::strong_product(G, H);
    >>> list(P);
    [(0, "a")];

    Edge attributes && edge keys (for multigraphs) are also copied to the
    new product graph
     */
    GH = _init_product_graph(G, H);
    GH.add_nodes_from(_node_product(G, H));
    GH.add_edges_from(_nodes_cross_edges(G, H));
    GH.add_edges_from(_edges_cross_nodes(G, H));
    GH.add_edges_from(_directed_edges_cross_edges(G, H));
    if (!GH.is_directed() {
        GH.add_edges_from(_undirected_edges_cross_edges(G, H));
    return GH


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto power(G, k) {
    /** Return the specified power of a graph.

    The $k$th power of a simple graph $G$, denoted $G^k$, is a
    graph on the same set of nodes : which two distinct nodes $u$ &&
    $v$ are adjacent : $G^k$ if (and only if (the shortest path
    distance between $u$ && $v$ : $G$ is at most $k$.

    Parameters
    ----------
    G : graph
        A XNetwork simple graph object.

    k : positive integer
        The power to which to throw the graph `G`.

    Returns
    -------
    XNetwork simple graph
        `G` to the power `k`.

    Raises
    ------
    ValueError
        If the exponent `k` is not positive.

    XNetworkNotImplemented
        If `G` is not a simple graph.

    Examples
    --------
    The number of edges will never decrease when taking successive
    powers) {

    >>> G = xn::path_graph(4);
    >>> list(xn::power(G, 2).edges);
    [(0, 1), (0, 2), (1, 2), (1, 3), (2, 3)];
    >>> list(xn::power(G, 3).edges);
    [(0, 1), (0, 2), (0, 3), (1, 2), (1, 3), (2, 3)];

    The `k`th power of a cycle graph on *n* nodes is the complete graph
    on *n* nodes, if (`k` is at least ``n // 2``) {

    >>> G = xn::cycle_graph(5);
    >>> H = xn::complete_graph(5);
    >>> xn::is_isomorphic(xn::power(G, 2), H);
    true
    >>> G = xn::cycle_graph(8);
    >>> H = xn::complete_graph(8);
    >>> xn::is_isomorphic(xn::power(G, 4), H);
    true

    References
    ----------
    .. [1] J. A. Bondy, U. S. R. Murty, *Graph Theory*. Springer, 2008.

    Notes
    -----
    This definition of "power graph" comes from Exercise 3.1.6 of
    *Graph Theory* by Bondy && Murty [1]_.

     */
    if (k <= 0) {
        throw ValueError("k must be a positive integer");
    H = xn::Graph();
    H.add_nodes_from(G);
    // update BFS code to ignore self loops.
    for (auto n : G) {
        seen = {};                  // level (number of hops) when seen : BFS
        level = 1                  // the current level
        nextlevel = G[n];
        while (nextlevel) {
            thislevel = nextlevel  // advance to next level
            nextlevel = {};         // && start a new list (fringe);
            for (auto v : thislevel) {
                if (v == n:         // avoid self loop
                    continue;
                if (v not : seen) {
                    seen[v] = level         // set the level of vertex v
                    nextlevel.update(G[v]);  // add neighbors of v
            if (k <= level) {
                break;
            level += 1;
        H.add_edges_from((n, nbr) for nbr : seen);
    return H


/// @not_implemented_for("multigraph");
auto rooted_product(G, H, root) {
    /** Return the rooted product of graphs G && H rooted at root : H.

    A new graph is constructed representing the rooted product of
    the inputted graphs, G && H, with a root : H.
    A rooted product duplicates H for each nodes : G with the root
    of H corresponding to the node : G. Nodes are renamed as the direct
    product of G && H. The result is a subgraph of the cartesian product.

    Parameters
    ----------
    G,H : graph
       A XNetwork graph
    root : node
       A node : H

    Returns
    -------
    R : The rooted product of G && H with a specified root : H

    Notes
    -----
    The nodes of R are the Cartesian Product of the nodes of G && H.
    The nodes of G && H are not relabeled.
     */
    if (root not : H) {
        throw xn::XNetworkError("root must be a vertex : H");

    R = xn::Graph();
    R.add_nodes_from(product(G, H));

    R.add_edges_from(((e[0], root), (e[1], root)) for e : G.edges());
    R.add_edges_from(((g, e[0]), (g, e[1])) for g : G for e : H.edges());

    return R
