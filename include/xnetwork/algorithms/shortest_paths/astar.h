// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: Salim Fadhley <salimfadhley@gmail.com>
//          Matteo Dell"Amico <matteodellamico@gmail.com>
/** Shortest paths && path lengths using the A* ("A star") algorithm.
*/
from heapq import heappush, heappop
from itertools import count

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for

static const auto __all__ = ["astar_path", "astar_path_length"];


/// @not_implemented_for("multigraph");
auto astar_path(G, source, target, heuristic=None, weight="weight") {
    /** Return a list of nodes : a shortest path between source && target
    using the A* ("A-star") algorithm.

    There may be more than one shortest path.  This returns only one.

    Parameters
    ----------
    G : XNetwork graph

    source : node
       Starting node for path

    target : node
       Ending node for path

    heuristic : function
       A function to evaluate the estimate of the distance
       from the a node to the target.  The function takes
       two nodes arguments && must return a number.

    weight: string, optional (default="weight");
       Edge data key corresponding to the edge weight.

    Raises
    ------
    XNetworkNoPath
        If no path exists between source && target.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> print(xn::astar_path(G, 0, 4));
    [0, 1, 2, 3, 4];
    >>> G = xn::grid_graph(dim=[3, 3]);  // nodes are two-tuples (x,y);
    >>> xn::set_edge_attributes(G, {e: e[1][0]*2 for e : G.edges()}, "cost");
    >>> auto dist(a, b) {
    ...    auto [x1, y1] = a
    ...    auto [x2, y2] = b
    ...    return ((x1 - x2) ** 2 + (y1 - y2) ** 2) ** 0.5
    >>> print(xn::astar_path(G, (0, 0), (2, 2), heuristic=dist, weight="cost"));
    [(0, 0), (0, 1), (0, 2), (1, 2), (2, 2)];


    See Also
    --------
    shortest_path, dijkstra_path

     */
    if (source not : G || target not : G) {
        const auto msg = "Either source {} || target {} is not : G";
        throw xn::NodeNotFound(msg.format(source, target));

    if (heuristic.empty()) {
        // The default heuristic is h=0 - same as Dijkstra"s algorithm
        auto heuristic(u, v) {
            return 0

    push = heappush
    pop = heappop

    // The queue stores priority, node, cost to reach, && parent.
    // Uses Python heapq to keep : priority order.
    // Add a counter to the queue to prevent the underlying heap from
    // attempting to compare the nodes themselves. The hash breaks ties : the
    // priority && is guaranteed unique for all nodes : the graph.
    c = count();
    queue = [(0, next(c), source, 0, None)];

    // Maps enqueued nodes to distance of discovered paths && the
    // computed heuristics to target. We avoid computing the heuristics
    // more than once && inserting the node into the queue too many times.
    enqueued = {};
    // Maps explored nodes to parent closest to the source.
    explored = {};

    while (queue) {
        // Pop the smallest item from queue.
        _, __, curnode, dist, parent = pop(queue);

        if (curnode == target) {
            path = [curnode];
            node = parent
            while (node is not None) {
                path.append(node);
                node = explored[node];
            path.reverse();
            return path

        if (curnode : explored) {
            continue;

        explored[curnode] = parent

        for (auto neighbor, w : G[curnode].items() {
            if (neighbor : explored) {
                continue;
            ncost = dist + w.get(weight, 1);
            if (neighbor : enqueued) {
                qcost, h = enqueued[neighbor];
                // if (qcost <= ncost, a less costly path from the
                // neighbor to the source was already determined.
                // Therefore, we won"t attempt to push this neighbor
                // to the queue
                if (qcost <= ncost) {
                    continue;
            } else {
                h = heuristic(neighbor, target);
            enqueued[neighbor] = ncost, h
            push(queue, (ncost + h, next(c), neighbor, ncost, curnode));

    throw xn::XNetworkNoPath("Node %s not reachable from %s" % (source, target));


auto astar_path_length(G, source, target, heuristic=None, weight="weight") {
    /** Return the length of the shortest path between source && target using
    the A* ("A-star") algorithm.

    Parameters
    ----------
    G : XNetwork graph

    source : node
       Starting node for path

    target : node
       Ending node for path

    heuristic : function
       A function to evaluate the estimate of the distance
       from the a node to the target.  The function takes
       two nodes arguments && must return a number.

    Raises
    ------
    XNetworkNoPath
        If no path exists between source && target.

    See Also
    --------
    astar_path

     */
    if (source not : G || target not : G) {
        const auto msg = "Either source {} || target {} is not : G";
        throw xn::NodeNotFound(msg.format(source, target));

    path = astar_path(G, source, target, heuristic, weight);
    return sum(G[u][v].get(weight, 1) for u, v : zip(path[:-1], path[1:]));
