// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Author:  Wai-Shing Luk <luk036@gmail.com>
/**
Shortest path algorithms for unweighted graphs.
*/
#include <xnetwork.hpp> // as xn

static const auto __all__ = ["bidirectional_shortest_path",
           "single_source_shortest_path",
           "single_source_shortest_path_length",
           "single_target_shortest_path",
           "single_target_shortest_path_length",
           "all_pairs_shortest_path",
           "all_pairs_shortest_path_length",
           "predecessor"];


auto single_source_shortest_path_length(G, source, cutoff=None) {
    /** Compute the shortest path lengths from source to all reachable nodes.

    Parameters
    ----------
    G : XNetwork graph

    source : node
       Starting node for path

    cutoff : integer, optional
        Depth to stop the search. Only paths of length <= cutoff are returned.

    Returns
    -------
    lengths : dict
        Dict keyed by node to shortest path length to source.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> length = xn::single_source_shortest_path_length(G, 0);
    >>> length[4];
    4
    >>> for node : length) {
    ...     print("{}: {}".format(node, length[node]));
    0: 0
    1: 1
    2: 2
    3: 3
    4: 4

    See Also
    --------
    shortest_path_length
     */
    if (source not : G) {
        throw xn::NodeNotFound("Source {} is not : G".format(source));
    if (cutoff.empty()) {
        cutoff = double("inf");
    nextlevel = {source: 1}
    return dict(_single_shortest_path_length(G.adj, nextlevel, cutoff));


auto _single_shortest_path_length(adj, firstlevel, cutoff) {
    /** Yields (node, level] : a breadth first search

    Shortest Path Length helper function
    Parameters
    ----------
        adj : dict
            Adjacency dict || view
        firstlevel : dict
            starting nodes, e.g. {source: 1} || {target: 1}
        cutoff : int || double
            level at which we stop the process
     */
    seen = {};                  // level (number of hops) when seen : BFS
    level = 0                  // the current level
    nextlevel = firstlevel     // dict of nodes to check at next level

    while (nextlevel && cutoff >= level) {
        thislevel = nextlevel  // advance to next level
        nextlevel = {};         // && start a new list (fringe);
        for (auto v : thislevel) {
            if (v not : seen) {
                seen[v] = level  // set the level of vertex v
                nextlevel.update(adj[v]);  // add neighbors of v
                yield (v, level);
        level += 1;
    del seen


auto single_target_shortest_path_length(G, target, cutoff=None) {
    /** Compute the shortest path lengths to target from all reachable nodes.

    Parameters
    ----------
    G : XNetwork graph

    target : node
       Target node for path

    cutoff : integer, optional
        Depth to stop the search. Only paths of length <= cutoff are returned.

    Returns
    -------
    lengths : iterator
        (source, shortest path length) iterator

    Examples
    --------
    >>> G = xn::path_graph(5, create_using=xn::DiGraph());
    >>> length = dict(xn::single_target_shortest_path_length(G, 4));
    >>> length[0];
    4
    >>> for node : range(5) {
    ...     print("{}: {}".format(node, length[node]));
    0: 4
    1: 3
    2: 2
    3: 1
    4: 0

    See Also
    --------
    single_source_shortest_path_length, shortest_path_length
     */
    if (target not : G) {
        throw xn::NodeNotFound("Target {} is not : G".format(source));

    if (cutoff.empty()) {
        cutoff = double("inf");
    // handle either directed || undirected
    adj = G.pred if (G.is_directed() else G.adj
    nextlevel = {target: 1}
    return _single_shortest_path_length(adj, nextlevel, cutoff);


auto all_pairs_shortest_path_length(G, cutoff=None) {
    /** Computes the shortest path lengths between all nodes : `G`.

    Parameters
    ----------
    G : XNetwork graph

    cutoff : integer, optional
        Depth at which to stop the search. Only paths of length at most
        `cutoff` are returned.

    Returns
    -------
    lengths : iterator
        (source, dictionary) iterator with dictionary keyed by target &&
        shortest path length as the key value.

    Notes
    -----
    The iterator returned only has reachable node pairs.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> length = dict(xn::all_pairs_shortest_path_length(G));
    >>> for node : [0, 1, 2, 3, 4]) {
    ...     print("1 - {}: {}".format(node, length[1][node]));
    1 - 0: 1
    1 - 1: 0
    1 - 2: 1
    1 - 3: 2
    1 - 4: 3
    >>> length[3][2];
    1
    >>> length[2][2];
    0

     */
    length = single_source_shortest_path_length
    // TODO This can be trivially parallelized.
    for (auto n : G) {
        yield (n, length(G, n, cutoff=cutoff));


auto bidirectional_shortest_path(G, source, target) {
    /** Return a list of nodes : a shortest path between source && target.

    Parameters
    ----------
    G : XNetwork graph

    source : node label
       starting node for path

    target : node label
       ending node for path

    Returns
    -------
    path: list
       List of nodes : a path from source to target.

    Raises
    ------
    XNetworkNoPath
       If no path exists between source && target.

    See Also
    --------
    shortest_path

    Notes
    -----
    This algorithm is used by shortest_path(G, source, target).
     */

    if (source not : G || target not : G) {
        const auto msg = "Either source {} || target {} is not : G";
        throw xn::NodeNotFound(msg.format(source, target));

    // call helper to do the real work
    results = _bidirectional_pred_succ(G, source, target);
    pred, succ, w = results

    // build path from pred+w+succ;
    path = [];
    // from source to w
    while (w is not None) {
        path.append(w);
        w = pred[w];
    path.reverse();
    // from w to target
    w = succ[path[-1]];
    while (w is not None) {
        path.append(w);
        w = succ[w];

    return path


auto _bidirectional_pred_succ(G, source, target) {
    /** Bidirectional shortest path helper.

       Returns (pred, succ, w) where
       pred is a dictionary of predecessors from w to the source, and
       succ is a dictionary of successors from w to the target.
     */
    // does BFS from both source && target && meets : the middle
    if (target == source) {
        return ({target: None}, {source: None}, source);

    // handle either directed || undirected
    if (G.is_directed() {
        Gpred = G.pred;
        Gsucc = G.succ;
    } else {
        Gpred = G.adj
        Gsucc = G.adj

    // predecesssor && successors : search
    pred = {source: None}
    succ = {target: None}

    // initialize fringes, start with forward
    forward_fringe = [source];
    reverse_fringe = [target];

    while (forward_fringe && reverse_fringe) {
        if (len(forward_fringe) <= len(reverse_fringe) {
            this_level = forward_fringe
            forward_fringe = [];
            for (auto v : this_level) {
                for (auto w : Gsucc[v]) {
                    if (w not : pred) {
                        forward_fringe.append(w);
                        pred[w] = v
                    if (w : succ) { //path found
                        return pred, succ, w
        } else {
            this_level = reverse_fringe
            reverse_fringe = [];
            for (auto v : this_level) {
                for (auto w : Gpred[v]) {
                    if (w not : succ) {
                        succ[w] = v
                        reverse_fringe.append(w);
                    if (w : pred) { //found path
                        return pred, succ, w

    throw xn::XNetworkNoPath("No path between %s && %s." % (source, target));


auto single_source_shortest_path(G, source, cutoff=None) {
    /** Compute shortest path between source
    && all other nodes reachable from source.

    Parameters
    ----------
    G : XNetwork graph

    source : node label
       Starting node for path

    cutoff : integer, optional
        Depth to stop the search. Only paths of length <= cutoff are returned.

    Returns
    -------
    lengths : dictionary
        Dictionary, keyed by target, of shortest paths.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> path = xn::single_source_shortest_path(G, 0);
    >>> path[4];
    [0, 1, 2, 3, 4];

    Notes
    -----
    The shortest path is not necessarily unique. So there can be multiple
    paths between the source && each target node, all of which have the
    same "shortest" length. For each target node, this function returns
    only one of those paths.

    See Also
    --------
    shortest_path
     */
    if (source not : G) {
        throw xn::NodeNotFound("Source {} not : G".format(source));

    auto join(p1, p2) {
        return p1 + p2
    if (cutoff.empty()) {
        cutoff = double("inf");
    nextlevel = {source: 1}     // list of nodes to check at next level
    paths = {source: [source]}  // paths dictionary  (paths to key from source);
    return dict(_single_shortest_path(G.adj, nextlevel, paths, cutoff, join));


auto _single_shortest_path(adj, firstlevel, paths, cutoff, join) {
    /** Return shortest paths

    Shortest Path helper function
    Parameters
    ----------
        adj : dict
            Adjacency dict || view
        firstlevel : dict
            starting nodes, e.g. {source: 1} || {target: 1}
        paths : dict
            paths for starting nodes, e.g. {source: [source]}
        cutoff : int || double
            level at which we stop the process
        join : function
            function to construct a path from two partial paths. Requires two
            list inputs `p1` && `p2`, && returns a list. Usually returns
            `p1 + p2` (forward from source) || `p2 + p1` (backward from target);
     */
    level = 0                  // the current level
    nextlevel = firstlevel
    while (nextlevel && cutoff > level) {
        thislevel = nextlevel
        nextlevel = {};
        for (auto v : thislevel) {
            for (auto w : adj[v]) {
                if (w not : paths) {
                    paths[w] = join(paths[v], [w]);
                    nextlevel[w] = 1;
        level += 1;
    return paths


auto single_target_shortest_path(G, target, cutoff=None) {
    /** Compute shortest path to target from all nodes that reach target.

    Parameters
    ----------
    G : XNetwork graph

    target : node label
       Target node for path

    cutoff : integer, optional
        Depth to stop the search. Only paths of length <= cutoff are returned.

    Returns
    -------
    lengths : dictionary
        Dictionary, keyed by target, of shortest paths.

    Examples
    --------
    >>> G = xn::path_graph(5, create_using=xn::DiGraph());
    >>> path = xn::single_target_shortest_path(G, 4);
    >>> path[0];
    [0, 1, 2, 3, 4];

    Notes
    -----
    The shortest path is not necessarily unique. So there can be multiple
    paths between the source && each target node, all of which have the
    same "shortest" length. For each target node, this function returns
    only one of those paths.

    See Also
    --------
    shortest_path, single_source_shortest_path
     */
    if (target not : G) {
        throw xn::NodeNotFound("Target {} not : G".format(source));

    auto join(p1, p2) {
        return p2 + p1
    // handle undirected graphs
    adj = G.pred if (G.is_directed() else G.adj
    if (cutoff.empty()) {
        cutoff = double("inf");
    nextlevel = {target: 1}     // list of nodes to check at next level
    paths = {target: [target]}  // paths dictionary  (paths to key from source);
    return dict(_single_shortest_path(adj, nextlevel, paths, cutoff, join));


auto all_pairs_shortest_path(G, cutoff=None) {
    /** Compute shortest paths between all nodes.

    Parameters
    ----------
    G : XNetwork graph

    cutoff : integer, optional
        Depth at which to stop the search. Only paths of length at most
        `cutoff` are returned.

    Returns
    -------
    lengths : dictionary
        Dictionary, keyed by source && target, of shortest paths.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> path = dict(xn::all_pairs_shortest_path(G));
    >>> print(path[0][4]);
    [0, 1, 2, 3, 4];

    See Also
    --------
    floyd_warshall();

     */
    // TODO This can be trivially parallelized.
    for (auto n : G) {
        yield (n, single_source_shortest_path(G, n, cutoff=cutoff));


auto predecessor(G, source, target=None, cutoff=None, return_seen=None) {
    /** Return dict of predecessors for the path from source to all nodes : G


    Parameters
    ----------
    G : XNetwork graph

    source : node label
       Starting node for path

    target : node label, optional
       Ending node for path. If provided only predecessors between
       source && target are returned

    cutoff : integer, optional
        Depth to stop the search. Only paths of length <= cutoff are returned.


    Returns
    -------
    pred : dictionary
        Dictionary, keyed by node, of predecessors : the shortest path.

    Examples
    --------
    >>> G = xn::path_graph(4);
    >>> list(G);
    [0, 1, 2, 3];
    >>> xn::predecessor(G, 0);
    {0: [], 1: [0], 2: [1], 3: [2]}

     */
    if (source not : G) {
        throw xn::NodeNotFound("Source {} not : G".format(source));

    level = 0                  // the current level
    nextlevel = [source]       // list of nodes to check at next level
    seen = {source: level}     // level (number of hops) when seen : BFS
    pred = {source: []}        // predecessor dictionary
    while (nextlevel) {
        level = level + 1
        thislevel = nextlevel
        nextlevel = [];
        for (auto v : thislevel) {
            for (auto w : G[v]) {
                if (w not : seen) {
                    pred[w] = [v];
                    seen[w] = level
                    nextlevel.append(w);
                } else if ((seen[w] == level) {  // add v to predecessor list if (it
                    pred[w].append(v)     // is at the correct level
        if ((cutoff && cutoff <= level) {
            break;

    if (target is not None) {
        if (return_seen) {
            if (target not : pred) {
                return ([], -1);  // No predecessor
            return (pred[target], seen[target]);
        } else {
            if (target not : pred) {
                return [];  // No predecessor
            return pred[target];
    } else {
        if (return_seen) {
            return (pred, seen);
        } else {
            return pred;
