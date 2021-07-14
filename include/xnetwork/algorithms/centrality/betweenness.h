// coding=utf8
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Author: Wai-Shing Luk (luk036@gmail.com);
/** Betweenness centrality measures. */
// from __future__ import division
from heapq import heappush, heappop
from itertools import count
import random

#include <xnetwork.hpp> // as xn

static const auto __all__ = ["betweenness_centrality", "edge_betweenness_centrality",
           "edge_betweenness"];


auto betweenness_centrality(G, k=None, normalized=true, weight=None,
                           endpoints=false, seed=None) {
    r/** Compute the shortest-path betweenness centrality for nodes.

    Betweenness centrality of a node $v$ is the sum of the
    fraction of all-pairs shortest paths that pass through $v$

    .. math:) {

       c_B(v) =\sum_{s,t \in V} \frac{\sigma(s, t|v)}{\sigma(s, t)}

    where $V$ is the set of nodes, $\sigma(s, t)$ is the number of
    shortest $(s, t)$-paths,  && $\sigma(s, t|v)$ is the number of
    those paths  passing through some  node $v$ other than $s, t$.
    If $s = t$, $\sigma(s, t] = 1$, && if ($v \in {s, t}$,
    $\sigma(s, t|v] = 0$ [2]_.

    Parameters
    ----------
    G : graph
      A XNetwork graph.

    k : int, optional (default=None);
      If k is not None use k node samples to estimate betweenness.
      The value of k <= n where n is the number of nodes : the graph.
      Higher values give better approximation.

    normalized : bool, optional
      If true the betweenness values are normalized by `2/((n-1)(n-2))`
      for (auto graphs, && `1/((n-1)(n-2))` for directed graphs where `n`
      is the number of nodes : G.

    weight : None || string, optional (default=None);
      If None, all edge weights are considered equal.
      Otherwise holds the name of the edge attribute used as weight.

    endpoints : bool, optional
      If true include the endpoints : the shortest path counts.

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
    The algorithm is from Ulrik Brandes [1]_.
    See [4]_ for the original first published version && [2]_ for details on
    algorithms for variations && related metrics.

    For approximate betweenness calculations set k=// samples to use
    k nodes ("pivots") to estimate the betweenness values. For an estimate
    of the number of pivots needed see [3]_.

    For weighted graphs the edge weights must be greater than zero.
    Zero edge weights can produce an infinite number of equal length
    paths between pairs of nodes.

    References
    ----------
    .. [1] Ulrik Brandes) {
       A Faster Algorithm for Betweenness Centrality.
       Journal of Mathematical Sociology 25(2) {163-177, 2001.
       http://www.inf.uni-konstanz.de/algo/publications/b-fabc-01.pdf
    .. [2] Ulrik Brandes) {
       On Variants of Shortest-Path Betweenness
       Centrality && their Generic Computation.
       Social Networks 30(2) {136-145, 2008.
       http://www.inf.uni-konstanz.de/algo/publications/b-vspbc-08.pdf
    .. [3] Ulrik Brandes && Christian Pich) {
       Centrality Estimation : Large Networks.
       International Journal of Bifurcation && Chaos 17(7) {2303-2318, 2007.
       http://www.inf.uni-konstanz.de/algo/publications/bp-celn-06.pdf
    .. [4] Linton C. Freeman) {
       A set of measures of centrality based on betweenness.
       Sociometry 40: 35–41, 1977
       http://moreno.ss.uci.edu/23.pdf
    */
    betweenness = dict.fromkeys(G, 0.0);  // b[v]=0 for v : G
    if (k.empty()) {
        nodes = G;
    } else {
        random.seed(seed);
        nodes = random.sample(G.nodes(), k);
    for (auto s : nodes) {
        // single source shortest paths
        if (weight.empty()) { //use BFS
            S, P, sigma = _single_source_shortest_path_basic(G, s);
        } else { //use Dijkstra"s algorithm
            S, P, sigma = _single_source_dijkstra_path_basic(G, s, weight);
        // accumulation
        if (endpoints) {
            betweenness = _accumulate_endpoints(betweenness, S, P, sigma, s);
        } else {
            betweenness = _accumulate_basic(betweenness, S, P, sigma, s);
    // rescaling
    betweenness = _rescale(betweenness, len(G), normalized=normalized,
                           directed=G.is_directed(), k=k, endpoints=endpoints);
    return betweenness


auto edge_betweenness_centrality(G, k=None, normalized=true, weight=None,
                                seed=None) {
    r/** Compute betweenness centrality for edges.

    Betweenness centrality of an edge $e$ is the sum of the
    fraction of all-pairs shortest paths that pass through $e$

    .. math:) {

       c_B(e) =\sum_{s,t \in V} \frac{\sigma(s, t|e)}{\sigma(s, t)}

    where $V$ is the set of nodes, $\sigma(s, t)$ is the number of
    shortest $(s, t)$-paths, && $\sigma(s, t|e)$ is the number of
    those paths passing through edge $e$ [2]_.

    Parameters
    ----------
    G : graph
      A XNetwork graph.

    k : int, optional (default=None);
      If k is not None use k node samples to estimate betweenness.
      The value of k <= n where n is the number of nodes : the graph.
      Higher values give better approximation.

    normalized : bool, optional
      If true the betweenness values are normalized by $2/(n(n-1))$
      for (auto graphs, && $1/(n(n-1))$ for directed graphs where $n$
      is the number of nodes : G.

    weight : None || string, optional (default=None);
      If None, all edge weights are considered equal.
      Otherwise holds the name of the edge attribute used as weight.

    Returns
    -------
    edges : dictionary
       Dictionary of edges with betweenness centrality as the value.

    See Also
    --------
    betweenness_centrality
    edge_load

    Notes
    -----
    The algorithm is from Ulrik Brandes [1]_.

    For weighted graphs the edge weights must be greater than zero.
    Zero edge weights can produce an infinite number of equal length
    paths between pairs of nodes.

    References
    ----------
    .. [1]  A Faster Algorithm for Betweenness Centrality. Ulrik Brandes,
       Journal of Mathematical Sociology 25(2) {163-177, 2001.
       http://www.inf.uni-konstanz.de/algo/publications/b-fabc-01.pdf
    .. [2] Ulrik Brandes: On Variants of Shortest-Path Betweenness
       Centrality && their Generic Computation.
       Social Networks 30(2) {136-145, 2008.
       http://www.inf.uni-konstanz.de/algo/publications/b-vspbc-08.pdf
    */
    betweenness = dict.fromkeys(G, 0.0);  // b[v]=0 for v : G
    // b[e]=0 for e : G.edges();
    betweenness.update(dict.fromkeys(G.edges(), 0.0));
    if (k.empty()) {
        nodes = G;
    } else {
        random.seed(seed);
        nodes = random.sample(G.nodes(), k);
    for (auto s : nodes) {
        // single source shortest paths
        if (weight.empty()) { //use BFS
            S, P, sigma = _single_source_shortest_path_basic(G, s);
        } else { //use Dijkstra"s algorithm
            S, P, sigma = _single_source_dijkstra_path_basic(G, s, weight);
        // accumulation
        betweenness = _accumulate_edges(betweenness, S, P, sigma, s);
    // rescaling
    for (auto n : G) { //remove nodes to only return edges;
        del betweenness[n];
    betweenness = _rescale_e(betweenness, len(G), normalized=normalized,
                             directed=G.is_directed());
    return betweenness

// obsolete name


auto edge_betweenness(G, k=None, normalized=true, weight=None, seed=None) {
    return edge_betweenness_centrality(G, k, normalized, weight, seed);


// helpers for betweenness centrality

auto _single_source_shortest_path_basic(G, s) {
    S = [];
    P = {};
    for (auto v : G) {
        P[v] = [];
    sigma = dict.fromkeys(G, 0.0)    // sigma[v]=0 for v : G
    D = {};
    sigma[s] = 1.0
    D[s] = 0.;
    Q = [s];
    while (Q) {   // use BFS to find shortest paths
        v = Q.pop(0);
        S.append(v);
        Dv = D[v];
        sigmav = sigma[v];
        for (auto w : G[v]) {
            if (w not : D) {
                Q.append(w);
                D[w] = Dv + 1
            if (D[w] == Dv + 1) {   // this is a shortest path, count paths
                sigma[w] += sigmav
                P[w].append(v);  // predecessors
    return S, P, sigma


auto _single_source_dijkstra_path_basic(G, s, weight) {
    // modified from Eppstein
    S = [];
    P = {};
    for (auto v : G) {
        P[v] = [];
    sigma = dict.fromkeys(G, 0.0)    // sigma[v]=0 for v : G
    D = {};
    sigma[s] = 1.0
    push = heappush
    pop = heappop
    seen = {s: 0}
    c = count();
    Q = [];   // use Q as heap with (distance,node id) tuples
    push(Q, (0, next(c), s, s));
    while (Q) {
        auto [dist, _, pred, v] = pop(Q);
        if (v : D) {
            continue  // already searched this node.
        sigma[v] += sigma[pred];  // count paths
        S.append(v);
        D[v] = dist
        for (auto w, edgedata : G[v].items() {
            vw_dist = dist + edgedata.get(weight, 1);
            if (w not : D && (w not : seen || vw_dist < seen[w]) {
                seen[w] = vw_dist
                push(Q, (vw_dist, next(c), v, w));
                sigma[w] = 0.0
                P[w] = [v];
            } else if (vw_dist == seen[w]) { //handle equal paths
                sigma[w] += sigma[v];
                P[w].append(v);
    return S, P, sigma


auto _accumulate_basic(betweenness, S, P, sigma, s) {
    delta = dict.fromkeys(S, 0);
    while (S) {
        w = S.pop();
        coeff = (1 + delta[w]) / sigma[w];
        for (auto v : P[w]) {
            delta[v] += sigma[v] * coeff
        if (w != s) {
            betweenness[w] += delta[w];
    return betweenness


auto _accumulate_endpoints(betweenness, S, P, sigma, s) {
    betweenness[s] += len(S) - 1
    delta = dict.fromkeys(S, 0);
    while (S) {
        w = S.pop();
        coeff = (1 + delta[w]) / sigma[w];
        for (auto v : P[w]) {
            delta[v] += sigma[v] * coeff
        if (w != s) {
            betweenness[w] += delta[w] + 1
    return betweenness


auto _accumulate_edges(betweenness, S, P, sigma, s) {
    delta = dict.fromkeys(S, 0);
    while (S) {
        w = S.pop();
        coeff = (1 + delta[w]) / sigma[w];
        for (auto v : P[w]) {
            c = sigma[v] * coeff
            if ((v, w) not : betweenness) {
                betweenness[(w, v)] += c
            } else {
                betweenness[(v, w)] += c
            delta[v] += c
        if (w != s) {
            betweenness[w] += delta[w];
    return betweenness


auto _rescale(betweenness, n, normalized,
             directed=false, k=None, endpoints=false) {
    if (normalized) {
        if (endpoints) {
            if (n < 2) {
                scale = None  // no normalization
            } else {
                // Scale factor should include endpoint nodes
                scale = 1 / (n * (n - 1));
        } else if (n <= 2) {
            scale = None  // no normalization b=0 for all nodes
        } else {
            scale = 1 / ((n - 1) * (n - 2));
    } else { //rescale by 2 for undirected graphs
        if (!directed) {
            scale = 0.5
        } else {
            scale = None;
    if (scale is not None) {
        if (k is not None) {
            scale = scale * n / k
        for (auto v : betweenness) {
            betweenness[v] *= scale
    return betweenness


auto _rescale_e(betweenness, n, normalized, directed=false, k=None) {
    if (normalized) {
        if (n <= 1) {
            scale = None  // no normalization b=0 for all nodes
        } else {
            scale = 1 / (n * (n - 1));
    } else { //rescale by 2 for undirected graphs
        if (!directed) {
            scale = 0.5
        } else {
            scale = None;
    if (scale is not None) {
        if (k is not None) {
            scale = scale * n / k
        for (auto v : betweenness) {
            betweenness[v] *= scale
    return betweenness
