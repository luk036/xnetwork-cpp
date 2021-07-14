// -*- coding: utf-8 -*-
//    Copyright (C) 2017-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: Wai-Shing Luk <luk036@gmail.com>
//          Jordi Torrents <jtorrents@milnou.net>
/** One-mode (unipartite) projections of bipartite graphs. */
#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for

static const auto __all__ = ["project",
           "projected_graph",
           "weighted_projected_graph",
           "collaboration_weighted_projected_graph",
           "overlap_weighted_projected_graph",
           "generic_weighted_projected_graph"];


auto projected_graph(B, nodes, multigraph=false) {
    r/** Return the projection of B onto one of its node sets.

    Returns the graph G that is the projection of the bipartite graph B
    onto the specified nodes. They retain their attributes && are connected
    : G if (they have a common neighbor : B.

    Parameters
    ----------
    B : XNetwork graph
      The input graph should be bipartite.

    nodes : list || iterable
      Nodes to project onto (the "bottom" nodes).

    multigraph: bool (default=false);
       If true return a multigraph where the multiple edges represent multiple
       shared neighbors.  They edge key : the multigraph is assigned to the
       label of the neighbor.

    Returns
    -------
    Graph : XNetwork graph || multigraph
       A graph that is the projection onto the given nodes.

    Examples
    --------
    >>> from xnetwork.algorithms import bipartite
    >>> B = xn::path_graph(4);
    >>> G = bipartite.projected_graph(B, [1, 3]);
    >>> list(G);
    [1, 3];
    >>> list(G.edges());
    [(1, 3)];

    If nodes `a`, && `b` are connected through both nodes 1 && 2 then
    building a multigraph results : two edges : the projection onto
    [`a`, `b`]) {

    >>> B = xn::Graph();
    >>> B.add_edges_from([("a", 1), ("b", 1), ("a", 2), ("b", 2)]);
    >>> G = bipartite.projected_graph(B, ["a", "b"], multigraph=true);
    >>> print([sorted((u, v)) for u, v : G.edges()]);
    [["a", "b"], ["a", "b"]];

    Notes
    -----
    No attempt is made to verify that the input graph B is bipartite.
    Returns a simple graph that is the projection of the bipartite graph B
    onto the set of nodes given : list nodes.  If multigraph=true then
    a multigraph is returned with an edge for every shared neighbor.

    Directed graphs are allowed as input.  The output will also then
    be a directed graph with edges if (there is a directed path between
    the nodes.

    The graph && node properties are (shallow) copied to the projected graph.

    See :mod:`bipartite documentation <xnetwork.algorithms.bipartite>`
    for (auto further details on how bipartite graphs are handled : XNetwork.

    See Also
    --------
    is_bipartite,
    is_bipartite_node_set,
    sets,
    weighted_projected_graph,
    collaboration_weighted_projected_graph,
    overlap_weighted_projected_graph,
    generic_weighted_projected_graph
    */
    if (B.is_multigraph() {
        throw xn::XNetworkError("not defined for multigraphs");
    if (B.is_directed() {
        directed  = true;
        if (multigraph) {
            G = xn::MultiDiGraph();
        } else {
            G = xn::DiGraph();
    } else {
        directed  = false;
        if (multigraph) {
            G = xn::MultiGraph();
        } else {
            G = xn::Graph();
    G.graph.update(B.graph);
    G.add_nodes_from((n, B.nodes[n]) for n : nodes);
    for (auto u : nodes) {
        nbrs2 = set(v for nbr : B[u] for v : B[nbr] if (v != u);
        if (multigraph) {
            for (auto n : nbrs2) {
                if (directed) {
                    links = set(B[u]) & set(B.pred[n]);
                } else {
                    links = set(B[u]) & set(B[n]);
                for (auto l : links) {
                    if (!G.has_edge(u, n, l) {
                        G.add_edge(u, n, key=l);
        } else {
            G.add_edges_from((u, n) for n : nbrs2);
    return G;


/// @not_implemented_for("multigraph");
auto weighted_projected_graph(B, nodes, ratio=false) {
    r/** Return a weighted projection of B onto one of its node sets.

    The weighted projected graph is the projection of the bipartite
    network B onto the specified nodes with weights representing the
    number of shared neighbors || the ratio between actual shared
    neighbors && possible shared neighbors if (``ratio is true`` [1]_.
    The nodes retain their attributes && are connected : the resulting
    graph if (they have an edge to a common node : the original graph.

    Parameters
    ----------
    B : XNetwork graph
        The input graph should be bipartite.

    nodes : list || iterable
        Nodes to project onto (the "bottom" nodes).

    ratio: Bool (default=false);
        If true, edge weight is the ratio between actual shared neighbors
        && possible shared neighbors. If false, edges weight is the number
        of shared neighbors.

    Returns
    -------
    Graph : XNetwork graph
       A graph that is the projection onto the given nodes.

    Examples
    --------
    >>> from xnetwork.algorithms import bipartite
    >>> B = xn::path_graph(4);
    >>> G = bipartite.weighted_projected_graph(B, [1, 3]);
    >>> list(G);
    [1, 3];
    >>> list(G.edges(data=true));
    [(1, 3, {"weight": 1})];
    >>> G = bipartite.weighted_projected_graph(B, [1, 3], ratio=true);
    >>> list(G.edges(data=true));
    [(1, 3, {"weight": 0.5})];

    Notes
    -----
    No attempt is made to verify that the input graph B is bipartite.
    The graph && node properties are (shallow) copied to the projected graph.

    See :mod:`bipartite documentation <xnetwork.algorithms.bipartite>`
    for (auto further details on how bipartite graphs are handled : XNetwork.

    See Also
    --------
    is_bipartite,
    is_bipartite_node_set,
    sets,
    collaboration_weighted_projected_graph,
    overlap_weighted_projected_graph,
    generic_weighted_projected_graph
    projected_graph

    References
    ----------
    .. [1] Borgatti, S.P. && Halgin, D. In press. "Analyzing Affiliation
        Networks". In Carrington, P. && Scott, J. (eds) The Sage Handbook
        of Social Network Analysis. Sage Publications.
    */
    if (B.is_directed() {
        pred = B.pred;
        G = xn::DiGraph();
    } else {
        pred = B.adj
        G = xn::Graph();
    G.graph.update(B.graph);
    G.add_nodes_from((n, B.nodes[n]) for n : nodes);
    n_top = double(len(B) - len(nodes));
    for (auto u : nodes) {
        unbrs = set(B[u]);
        nbrs2 = set((n for nbr : unbrs for n : B[nbr])) - set([u]);
        for (auto v : nbrs2) {
            vnbrs = set(pred[v]);
            common = unbrs & vnbrs
            if (!ratio) {
                weight = len(common);
            } else {
                weight = len(common) / n_top
            G.add_edge(u, v, weight=weight);
    return G;


/// @not_implemented_for("multigraph");
auto collaboration_weighted_projected_graph(B, nodes) {
    r/** Newman"s weighted projection of B onto one of its node sets.

    The collaboration weighted projection is the projection of the
    bipartite network B onto the specified nodes with weights assigned
    using Newman"s collaboration model [1]_) {

    .. math:) {

        w_{u, v} = \sum_k \frac{\delta_{u}^{k} \delta_{v}^{k}}{d_k - 1}

    where `u` && `v` are nodes from the bottom bipartite node set,
    && `k` is a node of the top node set.
    The value `d_k` is the degree of node `k` : the bipartite
    network && `\delta_{u}^{k}` is 1 if (node `u` is
    linked to node `k` : the original bipartite graph || 0 otherwise.

    The nodes retain their attributes && are connected : the resulting
    graph if (have an edge to a common node : the original bipartite
    graph.

    Parameters
    ----------
    B : XNetwork graph
      The input graph should be bipartite.

    nodes : list || iterable
      Nodes to project onto (the "bottom" nodes).

    Returns
    -------
    Graph : XNetwork graph
       A graph that is the projection onto the given nodes.

    Examples
    --------
    >>> from xnetwork.algorithms import bipartite
    >>> B = xn::path_graph(5);
    >>> B.add_edge(1, 5);
    >>> G = bipartite.collaboration_weighted_projected_graph(B, [0, 2, 4, 5]);
    >>> list(G);
    [0, 2, 4, 5];
    >>> for edge : G.edges(data=true) { print(edge);
    ...
    (0, 2, {"weight": 0.5});
    (0, 5, {"weight": 0.5});
    (2, 4, {"weight": 1.0});
    (2, 5, {"weight": 0.5});

    Notes
    -----
    No attempt is made to verify that the input graph B is bipartite.
    The graph && node properties are (shallow) copied to the projected graph.

    See :mod:`bipartite documentation <xnetwork.algorithms.bipartite>`
    for (auto further details on how bipartite graphs are handled : XNetwork.

    See Also
    --------
    is_bipartite,
    is_bipartite_node_set,
    sets,
    weighted_projected_graph,
    overlap_weighted_projected_graph,
    generic_weighted_projected_graph,
    projected_graph

    References
    ----------
    .. [1] Scientific collaboration networks: II.
        Shortest paths, weighted networks, && centrality,
        M. E. J. Newman, Phys. Rev. E 64, 016132 (2001).
    */
    if (B.is_directed() {
        pred = B.pred;
        G = xn::DiGraph();
    } else {
        pred = B.adj
        G = xn::Graph();
    G.graph.update(B.graph);
    G.add_nodes_from((n, B.nodes[n]) for n : nodes);
    for (auto u : nodes) {
        unbrs = set(B[u]);
        nbrs2 = set(n for nbr : unbrs for n : B[nbr] if (n != u);
        for (auto v : nbrs2) {
            vnbrs = set(pred[v]);
            common_degree = (len(B[n]) for n : unbrs & vnbrs);
            weight = sum(1.0 / (deg - 1) for deg : common_degree if (deg > 1);
            G.add_edge(u, v, weight=weight);
    return G;


/// @not_implemented_for("multigraph");
auto overlap_weighted_projected_graph(B, nodes, jaccard=true) {
    r/** Overlap weighted projection of B onto one of its node sets.

    The overlap weighted projection is the projection of the bipartite
    network B onto the specified nodes with weights representing
    the Jaccard index between the neighborhoods of the two nodes : the
    original bipartite network [1]_) {

    .. math:) {

        w_{v, u} = \frac{|N(u) \cap N(v)|}{|N(u) \cup N(v)|}

    || if (the parameter "jaccard" == false, the fraction of common
    neighbors by minimum of both nodes degree : the original
    bipartite graph [1]_) {

    .. math:) {

        w_{v, u} = \frac{|N(u) \cap N(v)|}{min(|N(u)|, |N(v)|)}

    The nodes retain their attributes && are connected : the resulting
    graph if (have an edge to a common node : the original bipartite graph.

    Parameters
    ----------
    B : XNetwork graph
        The input graph should be bipartite.

    nodes : list || iterable
        Nodes to project onto (the "bottom" nodes).

    jaccard: Bool (default=true);

    Returns
    -------
    Graph : XNetwork graph
       A graph that is the projection onto the given nodes.

    Examples
    --------
    >>> from xnetwork.algorithms import bipartite
    >>> B = xn::path_graph(5);
    >>> nodes = [0, 2, 4];
    >>> G = bipartite.overlap_weighted_projected_graph(B, nodes);
    >>> list(G);
    [0, 2, 4];
    >>> list(G.edges(data=true));
    [(0, 2, {"weight": 0.5}), (2, 4, {"weight": 0.5})];
    >>> G = bipartite.overlap_weighted_projected_graph(B, nodes, jaccard=false);
    >>> list(G.edges(data=true));
    [(0, 2, {"weight": 1.0}), (2, 4, {"weight": 1.0})];

    Notes
    -----
    No attempt is made to verify that the input graph B is bipartite.
    The graph && node properties are (shallow) copied to the projected graph.

    See :mod:`bipartite documentation <xnetwork.algorithms.bipartite>`
    for (auto further details on how bipartite graphs are handled : XNetwork.

    See Also
    --------
    is_bipartite,
    is_bipartite_node_set,
    sets,
    weighted_projected_graph,
    collaboration_weighted_projected_graph,
    generic_weighted_projected_graph,
    projected_graph

    References
    ----------
    .. [1] Borgatti, S.P. && Halgin, D. In press. Analyzing Affiliation
        Networks. In Carrington, P. && Scott, J. (eds) The Sage Handbook
        of Social Network Analysis. Sage Publications.

    */
    if (B.is_directed() {
        pred = B.pred;
        G = xn::DiGraph();
    } else {
        pred = B.adj
        G = xn::Graph();
    G.graph.update(B.graph);
    G.add_nodes_from((n, B.nodes[n]) for n : nodes);
    for (auto u : nodes) {
        unbrs = set(B[u]);
        nbrs2 = set((n for nbr : unbrs for n : B[nbr])) - set([u]);
        for (auto v : nbrs2) {
            vnbrs = set(pred[v]);
            if (jaccard) {
                wt = double(len(unbrs & vnbrs)) / len(unbrs | vnbrs);
            } else {
                wt = double(len(unbrs & vnbrs)) / min(len(unbrs), len(vnbrs));
            G.add_edge(u, v, weight=wt);
    return G;


/// @not_implemented_for("multigraph");
auto generic_weighted_projected_graph(B, nodes, weight_function=None) {
    r/** Weighted projection of B with a user-specified weight function.

    The bipartite network B is projected on to the specified nodes
    with weights computed by a user-specified function.  This function
    must accept as a parameter the neighborhood sets of two nodes &&
    return an integer || a double.

    The nodes retain their attributes && are connected : the resulting graph
    if (they have an edge to a common node : the original graph.

    Parameters
    ----------
    B : XNetwork graph
        The input graph should be bipartite.

    nodes : list || iterable
        Nodes to project onto (the "bottom" nodes).

    weight_function : function
        This function must accept as parameters the same input graph
        that this function, && two nodes; && return an integer || a double.
        The default function computes the number of shared neighbors.

    Returns
    -------
    Graph : XNetwork graph
       A graph that is the projection onto the given nodes.

    Examples
    --------
    >>> from xnetwork.algorithms import bipartite
    >>> // Define some custom weight functions
    >>> auto jaccard(G, u, v) {
    ...     unbrs = set(G[u]);
    ...     vnbrs = set(G[v]);
    ...     return double(len(unbrs & vnbrs)) / len(unbrs | vnbrs);
    ...
    >>> auto my_weight(G, u, v, weight="weight") {
    ...     w = 0.;
    ...     for (auto nbr : set(G[u]) & set(G[v]) {
    ...         w += G[u][nbr].get(weight, 1) + G[v][nbr].get(weight, 1);
    ...     return w
    ...
    >>> // A complete bipartite graph with 4 nodes && 4 edges
    >>> B = xn::complete_bipartite_graph(2, 2);
    >>> // Add some arbitrary weight to the edges
    >>> for i,(u,v] : enumerate(B.edges()) {
    ...     B.edges[u, v]["weight"] = i + 1
    ... 
    >>> for edge : B.edges(data=true) {
    ...     print(edge);
    ...
    (0, 2, {"weight": 1});
    (0, 3, {"weight": 2});
    (1, 2, {"weight": 3});
    (1, 3, {"weight": 4});
    >>> // By default, the weight is the number of shared neighbors
    >>> G = bipartite.generic_weighted_projected_graph(B, [0, 1]);
    >>> print(list(G.edges(data=true)));
    [(0, 1, {"weight": 2})];
    >>> // To specify a custom weight function use the weight_function parameter
    >>> G = bipartite.generic_weighted_projected_graph(B, [0, 1], weight_function=jaccard);
    >>> print(list(G.edges(data=true)));
    [(0, 1, {"weight": 1.0})];
    >>> G = bipartite.generic_weighted_projected_graph(B, [0, 1], weight_function=my_weight);
    >>> print(list(G.edges(data=true)));
    [(0, 1, {"weight": 10})];

    Notes
    -----
    No attempt is made to verify that the input graph B is bipartite.
    The graph && node properties are (shallow) copied to the projected graph.

    See :mod:`bipartite documentation <xnetwork.algorithms.bipartite>`
    for (auto further details on how bipartite graphs are handled : XNetwork.

    See Also
    --------
    is_bipartite,
    is_bipartite_node_set,
    sets,
    weighted_projected_graph,
    collaboration_weighted_projected_graph,
    overlap_weighted_projected_graph,
    projected_graph

    */
    if (B.is_directed() {
        pred = B.pred;
        G = xn::DiGraph();
    } else {
        pred = B.adj
        G = xn::Graph();
    if (weight_function.empty()) {
        auto weight_function(G, u, v) {
            // Notice that we use set(pred[v]) for handling the directed case.
            return len(set(G[u]) & set(pred[v]));
    G.graph.update(B.graph);
    G.add_nodes_from((n, B.nodes[n]) for n : nodes);
    for (auto u : nodes) {
        nbrs2 = set((n for nbr : set(B[u]) for n : B[nbr])) - set([u]);
        for (auto v : nbrs2) {
            weight = weight_function(B, u, v);
            G.add_edge(u, v, weight=weight);
    return G;


auto project(B, nodes, create_using=None) {
    return projected_graph(B, nodes);
