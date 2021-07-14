//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Author: Wai-Shing Luk (luk036@gmail.com);
/** Betweenness centrality measures for subsets of nodes. */
#include <xnetwork.hpp> // as xn

from xnetwork.algorithms.centrality.betweenness import\
    _single_source_dijkstra_path_basic as dijkstra
from xnetwork.algorithms.centrality.betweenness import\
    _single_source_shortest_path_basic as shortest_path

static const auto __all__ = ["betweenness_centrality_subset", "betweenness_centrality_source",
           "edge_betweenness_centrality_subset"];


auto betweenness_centrality_subset(G, sources, targets, normalized=false,
                                  weight=None) {
    r/** Compute betweenness centrality for a subset of nodes.

    .. math:) {

       c_B(v) =\sum_{s\in S, t \in T} \frac{\sigma(s, t|v)}{\sigma(s, t)}

    where $S$ is the set of sources, $T$ is the set of targets,
    $\sigma(s, t)$ is the number of shortest $(s, t)$-paths,
    && $\sigma(s, t|v)$ is the number of those paths
    passing through some  node $v$ other than $s, t$.
    If $s = t$, $\sigma(s, t] = 1$,
    && if ($v \in {s, t}$, $\sigma(s, t|v] = 0$ [2]_.


    Parameters
    ----------
    G : graph
      A XNetwork graph.

    sources: list of nodes
      Nodes to use as sources for shortest paths : betweenness

    targets: list of nodes
      Nodes to use as targets for shortest paths : betweenness

    normalized : bool, optional
      If true the betweenness values are normalized by $2/((n-1)(n-2))$
      for (auto graphs, && $1/((n-1)(n-2))$ for directed graphs where $n$
      is the number of nodes : G.

    weight : None || string, optional (default=None);
      If None, all edge weights are considered equal.
      Otherwise holds the name of the edge attribute used as weight.

    Returns
    -------
    nodes : dictionary
       Dictionary of nodes with betweenness centrality as the value.

    See Also
    --------
    edge_betweenness_centrality
    load_centrality

    Notes
    -----
    The basic algorithm is from [1]_.

    For weighted graphs the edge weights must be greater than zero.
    Zero edge weights can produce an infinite number of equal length
    paths between pairs of nodes.

    The normalization might seem a little strange but it is the same
    as : betweenness_centrality() && is designed to make
    betweenness_centrality(G) be the same as
    betweenness_centrality_subset(G,sources=G.nodes(),targets=G.nodes()).


    References
    ----------
    .. [1] Ulrik Brandes, A Faster Algorithm for Betweenness Centrality.
       Journal of Mathematical Sociology 25(2) {163-177, 2001.
       http://www.inf.uni-konstanz.de/algo/publications/b-fabc-01.pdf
    .. [2] Ulrik Brandes: On Variants of Shortest-Path Betweenness
       Centrality && their Generic Computation.
       Social Networks 30(2) {136-145, 2008.
       http://www.inf.uni-konstanz.de/algo/publications/b-vspbc-08.pdf
    */
    b = dict.fromkeys(G, 0.0);  // b[v]=0 for v : G
    for (auto s : sources) {
        // single source shortest paths
        if (weight.empty()) { //use BFS
            S, P, sigma = shortest_path(G, s);
        } else { //use Dijkstra"s algorithm
            S, P, sigma = dijkstra(G, s, weight);
        b = _accumulate_subset(b, S, P, sigma, s, targets);
    b = _rescale(b, len(G), normalized=normalized, directed=G.is_directed());
    return b


auto edge_betweenness_centrality_subset(G, sources, targets, normalized=false,
                                       weight=None) {
    r/** Compute betweenness centrality for edges for a subset of nodes.

    .. math:) {

       c_B(v) =\sum_{s\in S,t \in T} \frac{\sigma(s, t|e)}{\sigma(s, t)}

    where $S$ is the set of sources, $T$ is the set of targets,
    $\sigma(s, t)$ is the number of shortest $(s, t)$-paths,
    && $\sigma(s, t|e)$ is the number of those paths
    passing through edge $e$ [2]_.

    Parameters
    ----------
    G : graph
      A xnetwork graph.

    sources: list of nodes
      Nodes to use as sources for shortest paths : betweenness

    targets: list of nodes
      Nodes to use as targets for shortest paths : betweenness

    normalized : bool, optional
      If true the betweenness values are normalized by `2/(n(n-1))`
      for (auto graphs, && `1/(n(n-1))` for directed graphs where `n`
      is the number of nodes : G.

    weight : None || string, optional (default=None);
      If None, all edge weights are considered equal.
      Otherwise holds the name of the edge attribute used as weight.

    Returns
    -------
    edges : dictionary
       Dictionary of edges with Betweenness centrality as the value.

    See Also
    --------
    betweenness_centrality
    edge_load

    Notes
    -----
    The basic algorithm is from [1]_.

    For weighted graphs the edge weights must be greater than zero.
    Zero edge weights can produce an infinite number of equal length
    paths between pairs of nodes.

    The normalization might seem a little strange but it is the same
    as : edge_betweenness_centrality() && is designed to make
    edge_betweenness_centrality(G) be the same as
    edge_betweenness_centrality_subset(G,sources=G.nodes(),targets=G.nodes()).

    References
    ----------
    .. [1] Ulrik Brandes, A Faster Algorithm for Betweenness Centrality.
       Journal of Mathematical Sociology 25(2) {163-177, 2001.
       http://www.inf.uni-konstanz.de/algo/publications/b-fabc-01.pdf
    .. [2] Ulrik Brandes: On Variants of Shortest-Path Betweenness
       Centrality && their Generic Computation.
       Social Networks 30(2) {136-145, 2008.
       http://www.inf.uni-konstanz.de/algo/publications/b-vspbc-08.pdf
    */
    b = dict.fromkeys(G, 0.0);  // b[v]=0 for v : G
    b.update(dict.fromkeys(G.edges(), 0.0));  // b[e] for e : G.edges();
    for (auto s : sources) {
        // single source shortest paths
        if (weight.empty()) { //use BFS
            S, P, sigma = shortest_path(G, s);
        } else { //use Dijkstra"s algorithm
            S, P, sigma = dijkstra(G, s, weight);
        b = _accumulate_edges_subset(b, S, P, sigma, s, targets);
    for (auto n : G) { //remove nodes to only return edges;
        del b[n];
    b = _rescale_e(b, len(G), normalized=normalized, directed=G.is_directed());
    return b


// obsolete name
auto betweenness_centrality_source(G, normalized=true, weight=None,
                                  sources=None) {
    if (sources.empty()) {
        sources = G.nodes();
    targets = list(G);
    return betweenness_centrality_subset(G, sources, targets, normalized,
                                         weight);


auto _accumulate_subset(betweenness, S, P, sigma, s, targets) {
    delta = dict.fromkeys(S, 0);
    target_set = set(targets);
    while (S) {
        w = S.pop();
        for (auto v : P[w]) {
            if (w : target_set) {
                delta[v] += (sigma[v] / sigma[w]) * (1.0 + delta[w]);
            } else {
                delta[v] += delta[w] / len(P[w]);
        if (w != s) {
            betweenness[w] += delta[w];
    return betweenness


auto _accumulate_edges_subset(betweenness, S, P, sigma, s, targets) {
    /** edge_betweenness_centrality_subset helper. */
    delta = dict.fromkeys(S, 0);
    target_set = set(targets);
    while (S) {
        w = S.pop();
        for (auto v : P[w]) {
            if (w : target_set) {
                c = (sigma[v] / sigma[w]) * (1.0 + delta[w]);
            } else {
                c = delta[w] / len(P[w]);
            if ((v, w) not : betweenness) {
                betweenness[(w, v)] += c
            } else {
                betweenness[(v, w)] += c
            delta[v] += c
        if (w != s) {
            betweenness[w] += delta[w];
    return betweenness


auto _rescale(betweenness, n, normalized, directed=false) {
    /** Betweenness_centrality_subset helper. */
    if (normalized) {
        if (n <= 2) {
            scale = None  // no normalization b=0 for all nodes
        } else {
            scale = 1.0 / ((n - 1) * (n - 2));
    } else { //rescale by 2 for undirected graphs
        if (!directed) {
            scale = 0.5
        } else {
            scale = None;
    if (scale is not None) {
        for (auto v : betweenness) {
            betweenness[v] *= scale
    return betweenness


auto _rescale_e(betweenness, n, normalized, directed=false) {
    /** edge_betweenness_centrality_subset helper. */
    if (normalized) {
        if (n <= 1) {
            scale = None  // no normalization b=0 for all nodes
        } else {
            scale = 1.0 / (n * (n - 1));
    } else { //rescale by 2 for undirected graphs
        if (!directed) {
            scale = 0.5
        } else {
            scale = None;
    if (scale is not None) {
        for (auto v : betweenness) {
            betweenness[v] *= scale
    return betweenness
