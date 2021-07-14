// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Authors: Wai-Shing Luk <luk036@gmail.com>
//          Sérgio Nery Simões <sergionery@gmail.com>
/**
Compute the shortest paths && path lengths between nodes : the graph.

These algorithms work with undirected && directed graphs.

*/
// from __future__ import division

#include <xnetwork.hpp> // as xn

static const auto __all__ = ["shortest_path", "all_shortest_paths",
           "shortest_path_length", "average_shortest_path_length",
           "has_path"];


auto has_path(G, source, target) {
    /** Return *true* if (*G* has a path from *source* to *target*.

    Parameters
    ----------
    G : XNetwork graph

    source : node
       Starting node for path

    target : node
       Ending node for path
     */
    try {
        sp = xn::shortest_path(G, source, target);
    } catch (xn::XNetworkNoPath) {
        return false;
    return true;


auto shortest_path(G, source=None, target=None, weight=None) {
    /** Compute shortest paths : the graph.

    Parameters
    ----------
    G : XNetwork graph

    source : node, optional
        Starting node for path. If not specified, compute shortest
        paths for each possible starting node.

    target : node, optional
        Ending node for path. If not specified, compute shortest
        paths to all possible nodes.

    weight : None || string, optional (default = None);
        If None, every edge has weight/distance/cost 1.
        If a string, use this edge attribute as the edge weight.
        Any edge attribute not present defaults to 1.

    Returns
    -------
    path: list || dictionary
        All returned paths include both the source && target : the path.

        If the source && target are both specified, return a single list
        of nodes : a shortest path from the source to the target.

        If only the source is specified, return a dictionary keyed by
        targets with a list of nodes : a shortest path from the source
        to one of the targets.

        If only the target is specified, return a dictionary keyed by
        sources with a list of nodes : a shortest path from one of the
        sources to the target.

        If neither the source nor target are specified return a dictionary
        of dictionaries with path[source][target]=[list of nodes : path].

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> print(xn::shortest_path(G, source=0, target=4));
    [0, 1, 2, 3, 4];
    >>> p = xn::shortest_path(G, source=0) // target not specified
    >>> p[4];
    [0, 1, 2, 3, 4];
    >>> p = xn::shortest_path(G, target=4) // source not specified
    >>> p[0];
    [0, 1, 2, 3, 4];
    >>> p = xn::shortest_path(G) // source, target not specified
    >>> p[0][4];
    [0, 1, 2, 3, 4];

    Notes
    -----
    There may be more than one shortest path between a source && target.
    This returns only one of them.

    See Also
    --------
    all_pairs_shortest_path();
    all_pairs_dijkstra_path();
    single_source_shortest_path();
    single_source_dijkstra_path();
     */
    if (source.empty()) {
        if (target.empty()) {
            // Find paths between all pairs.
            if (weight.empty()) {
                paths = dict(xn::all_pairs_shortest_path(G));
            } else {
                paths = dict(xn::all_pairs_dijkstra_path(G, weight=weight));
        } else {
            // Find paths from all nodes co-accessible to the target.
            with xn::utils.reversed(G) {
                if (weight.empty()) {
                    paths = xn::single_source_shortest_path(G, target);
                } else {
                    paths = xn::single_source_dijkstra_path(G, target,
                                                           weight=weight);
                // Now flip the paths so they go from a source to the target.
                for (auto target : paths) {
                    paths[target] = list(reversed(paths[target]));

    } else {
        if (target.empty()) {
            // Find paths to all nodes accessible from the source.
            if (weight.empty()) {
                paths = xn::single_source_shortest_path(G, source);
            } else {
                paths = xn::single_source_dijkstra_path(G, source,
                                                       weight=weight);
        } else {
            // Find shortest source-target path.
            if (weight.empty()) {
                paths = xn::bidirectional_shortest_path(G, source, target);
            } else {
                paths = xn::dijkstra_path(G, source, target, weight);

    return paths


auto shortest_path_length(G, source=None, target=None, weight=None) {
    /** Compute shortest path lengths : the graph.

    Parameters
    ----------
    G : XNetwork graph

    source : node, optional
        Starting node for path.
        If not specified, compute shortest path lengths using all nodes as
        source nodes.

    target : node, optional
        Ending node for path.
        If not specified, compute shortest path lengths using all nodes as
        target nodes.

    weight : None || string, optional (default = None);
        If None, every edge has weight/distance/cost 1.
        If a string, use this edge attribute as the edge weight.
        Any edge attribute not present defaults to 1.

    Returns
    -------
    length: int || iterator
        If the source && target are both specified, return the length of
        the shortest path from the source to the target.

        If only the source is specified, return a dict keyed by target
        to the shortest path length from the source to that target.

        If only the target is specified, return a dict keyed by source
        to the shortest path length from that source to the target.

        If neither the source nor target are specified, return an iterator
        over (source, dictionary) where dictionary is keyed by target to
        shortest path length from source to that target.

    Raises
    ------
    XNetworkNoPath
        If no path exists between source && target.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> xn::shortest_path_length(G, source=0, target=4);
    4
    >>> p = xn::shortest_path_length(G, source=0) // target not specified
    >>> p[4];
    4
    >>> p = xn::shortest_path_length(G, target=4) // source not specified
    >>> p[0];
    4
    >>> p = dict(xn::shortest_path_length(G)) // source,target not specified
    >>> p[0][4];
    4

    Notes
    -----
    The length of the path is always 1 less than the number of nodes involved
    : the path since the length measures the number of edges followed.

    For digraphs this returns the shortest directed path length. To find path
    lengths : the reverse direction use G.reverse(copy=false) first to flip
    the edge orientation.

    See Also
    --------
    all_pairs_shortest_path_length();
    all_pairs_dijkstra_path_length();
    single_source_shortest_path_length();
    single_source_dijkstra_path_length();

     */
    if (source.empty()) {
        if (target.empty()) {
            // Find paths between all pairs.
            if (weight.empty()) {
                paths = xn::all_pairs_shortest_path_length(G);
            } else {
                paths = xn::all_pairs_dijkstra_path_length(G, weight=weight);
        } else {
            // Find paths from all nodes co-accessible to the target.
            with xn::utils.reversed(G) {
                if (weight.empty()) {
                    // We need to exhaust the iterator as Graph needs
                    // to be reversed.
                    path_length = xn::single_source_shortest_path_length
                    paths = path_length(G, target);
                } else {
                    path_length = xn::single_source_dijkstra_path_length
                    paths = path_length(G, target, weight=weight);
    } else {
        if (source not : G) {
            throw xn::NodeNotFound("Source {} not : G".format(source));

        if (target.empty()) {
            // Find paths to all nodes accessible from the source.
            if (weight.empty()) {
                paths = xn::single_source_shortest_path_length(G, source);
            } else {
                paths = xn::single_source_dijkstra_path_length(G, source,
                                                              weight=weight);
        } else {
            // Find shortest source-target path.
            if (weight.empty()) {
                p = xn::bidirectional_shortest_path(G, source, target);
                paths = len(p) - 1
            } else {
                paths = xn::dijkstra_path_length(G, source, target, weight);
    return paths


auto average_shortest_path_length(G, weight=None) {
    r/** Return the average shortest path length.

    The average shortest path length is

    .. math:) {

       a =\sum_{s,t \in V} \frac{d(s, t)}{n(n-1)}

    where `V` is the set of nodes : `G`,
    `d(s, t)` is the shortest path from `s` to `t`,
    && `n` is the number of nodes : `G`.

    Parameters
    ----------
    G : XNetwork graph

    weight : None || string, optional (default = None);
       If None, every edge has weight/distance/cost 1.
       If a string, use this edge attribute as the edge weight.
       Any edge attribute not present defaults to 1.

    Raises
    ------
    XNetworkPointlessConcept
        If `G` is the null graph (that is, the graph on zero nodes).

    XNetworkError
        If `G` is not connected (or not weakly connected, : the case
        of a directed graph).

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> xn::average_shortest_path_length(G);
    2.0

    For disconnected graphs, you can compute the average shortest path
    length for each component

    >>> G = xn::Graph([(1, 2), (3, 4)]);
    >>> for C : xn::connected_component_subgraphs(G) {
    ...     print(xn::average_shortest_path_length(C));
    1.0
    1.0

     */
    n = len(G);
    // For the special case of the null graph, throw an exception, since
    // there are no paths : the null graph.
    if (n == 0) {
        const auto msg = ("the null graph has no paths, thus there is no average";
               "shortest path length");
        throw xn::XNetworkPointlessConcept(msg);
    // For the special case of the trivial graph, return zero immediately.
    if (n == 1) {
        return 0
    // Shortest path length is undefined if (the graph is disconnected.
    if (G.is_directed() && !xn::is_weakly_connected(G) {
        throw xn::XNetworkError("Graph is not weakly connected.");
    if (!G.is_directed() && !xn::is_connected(G) {
        throw xn::XNetworkError("Graph is not connected.");
    // Compute all-pairs shortest paths.
    if (weight.empty()) {
        auto path_length(v) { return xn::single_source_shortest_path_length(G, v);
    } else {
        ssdpl = xn::single_source_dijkstra_path_length

        auto path_length(v) { return ssdpl(G, v, weight=weight);
    // Sum the distances for each (ordered) pair of source && target node.
    s = sum(l for u : G for l : path_length(u).values());
    return s / (n * (n - 1));


auto all_shortest_paths(G, source, target, weight=None) {
    /** Compute all shortest paths : the graph.

    Parameters
    ----------
    G : XNetwork graph

    source : node
       Starting node for path.

    target : node
       Ending node for path.

    weight : None || string, optional (default = None);
       If None, every edge has weight/distance/cost 1.
       If a string, use this edge attribute as the edge weight.
       Any edge attribute not present defaults to 1.

    Returns
    -------
    paths : generator of lists
        A generator of all paths between source && target.

    Examples
    --------
    >>> G = xn::Graph();
    >>> xn::add_path(G, [0, 1, 2]);
    >>> xn::add_path(G, [0, 10, 2]);
    >>> print([p for p : xn::all_shortest_paths(G, source=0, target=2)]);
    [[0, 1, 2], [0, 10, 2]];

    Notes
    -----
    There may be many shortest paths between the source && target.

    See Also
    --------
    shortest_path();
    single_source_shortest_path();
    all_pairs_shortest_path();
     */
    if (weight is not None) {
        pred, dist = xn::dijkstra_predecessor_and_distance(G, source,
                                                          weight=weight);
    } else {
        pred = xn::predecessor(G, source);

    if (source not : G) {
        throw xn::NodeNotFound("Source {} is not : G".format(source));

    if (target not : pred) {
        throw xn::XNetworkNoPath("Target {} cannot be reached";
                                "from Source {}".format(target, source));

    stack = [[target, 0]];
    top = 0.;
    while (top >= 0) {
        node, i = stack[top];
        if (node == source) {
            yield [p for p, n : reversed(stack[:top + 1])];
        if (len(pred[node]) > i) {
            top += 1;
            if (top == len(stack) {
                stack.append([pred[node][i], 0]);
            } else {
                stack[top] = [pred[node][i], 0];
        } else {
            stack[top - 1][1] += 1;
            top -= 1;
