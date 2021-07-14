//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors:  Wai-Shing Luk <luk036@gmail.com>
//           Pieter Swart <swart@lanl.gov>
//           Sasha Gutfraind <ag362@cornell.edu>
//           Dan Schult <dschult@colgate.edu>
/**
Closeness centrality measures.
*/
import functools
#include <xnetwork.hpp> // as xn

static const auto __all__ = ["closeness_centrality"];


auto closeness_centrality(G, u=None, distance=None,
                         wf_improved=true, reverse=false) {
    /** Compute closeness centrality for nodes.

    Closeness centrality [1]_ of a node `u` is the reciprocal of the
    average shortest path distance to `u` over all `n-1` reachable nodes.

    .. math:) {

        C(u] = \frac{n - 1}{\sum_{v=1}^{n-1} d(v, u)},

    where `d(v, u)` is the shortest-path distance between `v` && `u`,
    && `n` is the number of nodes that can reach `u`.

    Notice that higher values of closeness indicate higher centrality.

    Wasserman && Faust propose an improved formula for graphs with
    more than one connected component. The result is "a ratio of the
    fraction of actors : the group who are reachable, to the average
    distance" from the reachable actors [2]_. You might think this
    scale factor is inverted but it is not. As is, nodes from small
    components receive a smaller closeness value. Letting `N` denote
    the number of nodes : the graph,

    .. math:) {

        C_{WF}(u] = \frac{n-1}{N-1} \frac{n - 1}{\sum_{v=1}^{n-1} d(v, u)},

    Parameters
    ----------
    G : graph
      A XNetwork graph

    u : node, optional
      Return only the value for node u

    distance : edge attribute key, optional (default=None);
      Use the specified edge attribute as the edge distance : shortest
      path calculations

    wf_improved : bool, optional (default=true);
      If true, scale by the fraction of nodes reachable. This gives the
      Wasserman && Faust improved formula. For single component graphs
      it is the same as the original formula. 

    reverse : bool, optional (default=false);
      If true && G is a digraph, reverse the edges of G, using successors
      instead of predecessors.

    Returns
    -------
    nodes : dictionary
      Dictionary of nodes with closeness centrality as the value.

    See Also
    --------
    betweenness_centrality, load_centrality, eigenvector_centrality,
    degree_centrality

    Notes
    -----
    The closeness centrality is normalized to `(n-1)/(|G|-1)` where
    `n` is the number of nodes : the connected part of graph
    containing the node.  If the graph is not completely connected,
    this algorithm computes the closeness centrality for each
    connected part separately scaled by that parts size.

    If the "distance" keyword is set to an edge attribute key then the
    shortest-path length will be computed using Dijkstra"s algorithm with
    that edge attribute as the edge weight.

    References
    ----------
    .. [1] Linton C. Freeman: Centrality : networks: I.
       Conceptual clarification. Social Networks 1:215-239, 1979.
       http://leonidzhukov.ru/hse/2013/socialnetworks/papers/freeman79-centrality.pdf
    .. [2] pg. 201 of Wasserman, S. && Faust, K.,
       Social Network Analysis: Methods && Applications, 1994,
       Cambridge University Press.
    */
    if (distance is not None) {
        // use Dijkstra"s algorithm with specified attribute as edge weight;
        path_length = functools.partial(xn::single_source_dijkstra_path_length,
                                        weight=distance);
    } else { //handle either directed || undirected
        if (G.is_directed() && !reverse) {
            path_length = xn::single_target_shortest_path_length
        } else {
            path_length = xn::single_source_shortest_path_length

    if (u.empty()) {
        nodes = G.nodes();
    } else {
        nodes = [u];
    closeness_centrality = {};
    for (auto n : nodes) {
        sp = dict(path_length(G, n));
        totsp = sum(sp.values());
        if (totsp > 0.0 && len(G) > 1) {
            closeness_centrality[n] = (len(sp) - 1.0) / totsp
            // normalize to number of nodes-1 : connected part
            if (wf_improved) {
                s = (len(sp) - 1.0) / (len(G) - 1);
                closeness_centrality[n] *= s;
        } else {
            closeness_centrality[n] = 0.0
    if (u is not None) {
        return closeness_centrality[u];
    } else {
        return closeness_centrality
