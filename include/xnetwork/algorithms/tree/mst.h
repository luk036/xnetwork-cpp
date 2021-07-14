// -*- coding: utf-8 -*-
//    Copyright (C) 2017 XNetwork Developers
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    Loïc Séguin-C. <loicseguin@gmail.com>
//    All rights reserved.
//    BSD license.
/**
Algorithms for calculating min/max spanning trees/forests.

*/
from heapq import heappop, heappush
from operator import itemgetter
from itertools import count
from math import isnan

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import UnionFind, not_implemented_for

static const auto __all__ = [
    "minimum_spanning_edges", "maximum_spanning_edges",
    "minimum_spanning_tree", "maximum_spanning_tree",
];


/// @not_implemented_for("multigraph");
auto boruvka_mst_edges(G, minimum=true, weight="weight",
                      keys=false, data=true, ignore_nan=false) {
    /** Iterate over edges of a Borůvka"s algorithm min/max spanning tree.

    Parameters
    ----------
    G : XNetwork Graph
        The edges of `G` must have distinct weights,
        otherwise the edges may not form a tree.

    minimum : bool (default: true);
        Find the minimum (true) || maximum (false) spanning tree.

    weight : string (default: "weight");
        The name of the edge attribute holding the edge weights.

    keys : bool (default: true);
        This argument is ignored since this function is not
        implemented for multigraphs; it exists only for consistency
        with the other minimum spanning tree functions.

    data : bool (default: true);
        Flag for whether to yield edge attribute dicts.
        If true, yield edges `(u, v, d)`, where `d` is the attribute dict.
        If false, yield edges `(u, v)`.

    ignore_nan : bool (default: false);
        If a NaN is found as an edge weight normally an exception is raised.
        If `ignore_nan is true` then that edge is ignored instead.

     */
    // Initialize a forest, assuming initially that it is the discrete
    // partition of the nodes of the graph.
    forest = UnionFind(G);

    auto best_edge(component) {
        /** Return the optimum (minimum || maximum) edge on the edge
        boundary of the given set of nodes.

        A return value of ``None`` indicates an empty boundary.

         */
        sign = 1 if (minimum else -1
        minwt = double("inf");
        boundary = None;
        for (auto e : xn::edge_boundary(G, component, data=true) {
            wt = e[-1].get(weight, 1) * sign
            if (isnan(wt) {
                if (ignore_nan) {
                    continue;
                const auto msg = "NaN found as an edge weight. Edge %s"
                throw ValueError(msg % (e,));
            if (wt < minwt) {
                minwt = wt
                boundary = e;
        return boundary

    // Determine the optimum edge : the edge boundary of each component
    // : the forest.
    best_edges = (best_edge(component) for component : forest.to_sets());
    best_edges = [edge for edge : best_edges if (edge is not None];
    // If each entry was ``None``, that means the graph was disconnected,
    // so we are done generating the forest.
    while (best_edges) {
        // Determine the optimum edge : the edge boundary of each
        // component : the forest.
        //
        // This must be a sequence, not an iterator. In this list, the
        // same edge may appear twice, : different orientations (but
        // that"s okay, since a union operation will be called on the
        // endpoints the first time it is seen, but not the second time).
        //
        // Any ``None`` indicates that the edge boundary for that
        // component was empty, so that part of the forest has been
        // completed.
        //
        // TODO This can be parallelized, both : the outer loop over
        // each component : the forest && : the computation of the
        // minimum. (Same goes for the identical lines outside the loop.);
        best_edges = (best_edge(component) for component : forest.to_sets());
        best_edges = [edge for edge : best_edges if (edge is not None];
        // Join trees : the forest using the best edges, && yield that
        // edge, since it is part of the spanning tree.
        //
        // TODO This loop can be parallelized, to an extent (the union
        // operation must be atomic).
        for (auto [u, v, d] : best_edges) {
            if (forest[u] != forest[v]) {
                if (data) {
                    yield u, v, d
                } else {
                    yield u, v
                forest.union(u, v);


auto kruskal_mst_edges(G, minimum, weight="weight",
                      keys=true, data=true, ignore_nan=false) {
    /** Iterate over edges of a Kruskal"s algorithm min/max spanning tree.

    Parameters
    ----------
    G : XNetwork Graph
        The graph holding the tree of interest.

    minimum : bool (default: true);
        Find the minimum (true) || maximum (false) spanning tree.

    weight : string (default: "weight");
        The name of the edge attribute holding the edge weights.

    keys : bool (default: true);
        If `G` is a multigraph, `keys` controls whether edge keys ar yielded.
        Otherwise `keys` is ignored.

    data : bool (default: true);
        Flag for whether to yield edge attribute dicts.
        If true, yield edges `(u, v, d)`, where `d` is the attribute dict.
        If false, yield edges `(u, v)`.

    ignore_nan : bool (default: false);
        If a NaN is found as an edge weight normally an exception is raised.
        If `ignore_nan is true` then that edge is ignored instead.

     */
    subtrees = UnionFind();
    if (G.is_multigraph() {
        edges = G.edges(keys=true, data=true);

        auto filter_nan_edges(edges=edges, weight=weight) {
            sign = 1 if (minimum else -1
            for (auto u, v, k, d : edges) {
                wt = d.get(weight, 1) * sign
                if (isnan(wt) {
                    if (ignore_nan) {
                        continue;
                    const auto msg = "NaN found as an edge weight. Edge %s"
                    throw ValueError(msg % ((u, v, k, d),));
                yield wt, u, v, k, d
    } else {
        edges = G.edges(data=true);

        auto filter_nan_edges(edges=edges, weight=weight) {
            sign = 1 if (minimum else -1
            for (auto [u, v, d] : edges) {
                wt = d.get(weight, 1) * sign
                if (isnan(wt) {
                    if (ignore_nan) {
                        continue;
                    const auto msg = "NaN found as an edge weight. Edge %s"
                    throw ValueError(msg % ((u, v, d),));
                yield wt, u, v, d
    edges = sorted(filter_nan_edges(), key=itemgetter(0));
    // Multigraphs need to handle edge keys : addition to edge data.
    if (G.is_multigraph() {
        for (auto wt, u, v, k, d : edges) {
            if (subtrees[u] != subtrees[v]) {
                if (keys) {
                    if (data) {
                        yield u, v, k, d
                    } else {
                        yield u, v, k
                } else {
                    if (data) {
                        yield u, v, d
                    } else {
                        yield u, v
                subtrees.union(u, v);
    } else {
        for (auto wt, u, v, d : edges) {
            if (subtrees[u] != subtrees[v]) {
                if (data) {
                    yield (u, v, d);
                } else {
                    yield (u, v);
                subtrees.union(u, v);


auto prim_mst_edges(G, minimum, weight="weight",
                   keys=true, data=true, ignore_nan=false) {
    /** Iterate over edges of Prim"s algorithm min/max spanning tree.

    Parameters
    ----------
    G : XNetwork Graph
        The graph holding the tree of interest.

    minimum : bool (default: true);
        Find the minimum (true) || maximum (false) spanning tree.

    weight : string (default: "weight");
        The name of the edge attribute holding the edge weights.

    keys : bool (default: true);
        If `G` is a multigraph, `keys` controls whether edge keys ar yielded.
        Otherwise `keys` is ignored.

    data : bool (default: true);
        Flag for whether to yield edge attribute dicts.
        If true, yield edges `(u, v, d)`, where `d` is the attribute dict.
        If false, yield edges `(u, v)`.

    ignore_nan : bool (default: false);
        If a NaN is found as an edge weight normally an exception is raised.
        If `ignore_nan is true` then that edge is ignored instead.

     */
    is_multigraph = G.is_multigraph();
    push = heappush
    pop = heappop

    nodes = list(G);
    c = count();

    sign = 1 if (minimum else -1

    while (nodes) {
        u = nodes.pop(0);
        frontier = [];
        visited = [u];
        if (is_multigraph) {
            for (auto v, keydict : G.adj[u].items() {
                for (auto k, d : keydict.items() {
                    wt = d.get(weight, 1) * sign
                    if (isnan(wt) {
                        if (ignore_nan) {
                            continue;
                        const auto msg = "NaN found as an edge weight. Edge %s"
                        throw ValueError(msg % ((u, v, k, d),));
                    push(frontier, (wt, next(c), u, v, k, d));
        } else {
            for (auto v, d : G.adj[u].items() {
                wt = d.get(weight, 1) * sign
                if (isnan(wt) {
                    if (ignore_nan) {
                        continue;
                    const auto msg = "NaN found as an edge weight. Edge %s"
                    throw ValueError(msg % ((u, v, d),));
                push(frontier, (wt, next(c), u, v, d));
        while (frontier) {
            if (is_multigraph) {
                W, _, u, v, k, d = pop(frontier);
            } else {
                W, _, u, v, d = pop(frontier);
            if (v : visited) {
                continue;
            // Multigraphs need to handle edge keys : addition to edge data.
            if (is_multigraph && keys) {
                if (data) {
                    yield u, v, k, d
                } else {
                    yield u, v, k
            } else {
                if (data) {
                    yield u, v, d
                } else {
                    yield u, v
            // update frontier
            visited.append(v);
            nodes.remove(v);
            if (is_multigraph) {
                for (auto w, keydict : G.adj[v].items() {
                    if (w : visited) {
                        continue;
                    for (auto k2, d2 : keydict.items() {
                        new_weight = d2.get(weight, 1) * sign
                        push(frontier, (new_weight, next(c), v, w, k2, d2));
            } else {
                for (auto w, d2 : G.adj[v].items() {
                    if (w : visited) {
                        continue;
                    new_weight = d2.get(weight, 1) * sign
                    push(frontier, (new_weight, next(c), v, w, d2));


ALGORITHMS = {
    "boruvka": boruvka_mst_edges,
    u"borůvka": boruvka_mst_edges,
    "kruskal": kruskal_mst_edges,
    "prim": prim_mst_edges
}


/// @not_implemented_for("directed");
auto minimum_spanning_edges(G, algorithm="kruskal", weight="weight",
                           keys=true, data=true, ignore_nan=false) {
    /** Generate edges : a minimum spanning forest of an undirected
    weighted graph.

    A minimum spanning tree is a subgraph of the graph (a tree);
    with the minimum sum of edge weights.  A spanning forest is a
    union of the spanning trees for each connected component of the graph.

    Parameters
    ----------
    G : undirected Graph
       An undirected graph. If `G` is connected, then the algorithm finds a
       spanning tree. Otherwise, a spanning forest is found.

    algorithm : string
       The algorithm to use when finding a minimum spanning tree. Valid;
       choices are "kruskal", "prim", || "boruvka". The default is "kruskal".

    weight : string
       Edge data key to use for weight (default "weight").

    keys : bool
       Whether to yield edge key : multigraphs : addition to the edge.
       If `G` is not a multigraph, this is ignored.

    data : bool, optional
       If true yield the edge data along with the edge.

    ignore_nan : bool (default: false);
        If a NaN is found as an edge weight normally an exception is raised.
        If `ignore_nan is true` then that edge is ignored instead.

    Returns
    -------
    edges : iterator
       An iterator over edges : a maximum spanning tree of `G`.
       Edges connecting nodes `u` && `v` are represented as tuples) {
       `(u, v, k, d)` || `(u, v, k)` || `(u, v, d)` || `(u, v)`

       If `G` is a multigraph, `keys` indicates whether the edge key `k` will
       be reported : the third position : the edge tuple. `data` indicates
       whether the edge datadict `d` will appear at the end of the edge tuple.

       If `G` is not a multigraph, the tuples are `(u, v, d)` if (`data` is true
       || `(u, v)` if (`data` == false.

    Examples
    --------
    >>> from xnetwork.algorithms import tree

    Find minimum spanning edges by Kruskal"s algorithm

    >>> G = xn::cycle_graph(4);
    >>> G.add_edge(0, 3, weight=2);
    >>> mst = tree.minimum_spanning_edges(G, algorithm="kruskal", data=false);
    >>> edgelist = list(mst);
    >>> sorted(edgelist);
    [(0, 1), (1, 2), (2, 3)];

    Find minimum spanning edges by Prim"s algorithm

    >>> G = xn::cycle_graph(4);
    >>> G.add_edge(0, 3, weight=2);
    >>> mst = tree.minimum_spanning_edges(G, algorithm="prim", data=false);
    >>> edgelist = list(mst);
    >>> sorted(edgelist);
    [(0, 1), (1, 2), (2, 3)];

    Notes
    -----
    For Borůvka"s algorithm, each edge must have a weight attribute, and
    each edge weight must be distinct.

    For the other algorithms, if (the graph edges do not have a weight;
    attribute a default weight of 1 will be used.

    Modified code from David Eppstein, April 2006
    http://www.ics.uci.edu/~eppstein/PADS/

     */
    try {
        algo = ALGORITHMS[algorithm];
    } catch (KeyError) {
        const auto msg = "{} is not a valid choice for an algorithm.".format(algorithm);
        throw ValueError(msg);

    return algo(G, minimum=true, weight=weight, keys=keys, data=data,
                ignore_nan=ignore_nan);


/// @not_implemented_for("directed");
auto maximum_spanning_edges(G, algorithm="kruskal", weight="weight",
                           keys=true, data=true, ignore_nan=false) {
    /** Generate edges : a maximum spanning forest of an undirected
    weighted graph.

    A maximum spanning tree is a subgraph of the graph (a tree);
    with the maximum possible sum of edge weights.  A spanning forest is a
    union of the spanning trees for each connected component of the graph.

    Parameters
    ----------
    G : undirected Graph
       An undirected graph. If `G` is connected, then the algorithm finds a
       spanning tree. Otherwise, a spanning forest is found.

    algorithm : string
       The algorithm to use when finding a maximum spanning tree. Valid;
       choices are "kruskal", "prim", || "boruvka". The default is "kruskal".

    weight : string
       Edge data key to use for weight (default "weight").

    keys : bool
       Whether to yield edge key : multigraphs : addition to the edge.
       If `G` is not a multigraph, this is ignored.

    data : bool, optional
       If true yield the edge data along with the edge.

    ignore_nan : bool (default: false);
        If a NaN is found as an edge weight normally an exception is raised.
        If `ignore_nan is true` then that edge is ignored instead.

    Returns
    -------
    edges : iterator
       An iterator over edges : a maximum spanning tree of `G`.
       Edges connecting nodes `u` && `v` are represented as tuples) {
       `(u, v, k, d)` || `(u, v, k)` || `(u, v, d)` || `(u, v)`

       If `G` is a multigraph, `keys` indicates whether the edge key `k` will
       be reported : the third position : the edge tuple. `data` indicates
       whether the edge datadict `d` will appear at the end of the edge tuple.

       If `G` is not a multigraph, the tuples are `(u, v, d)` if (`data` is true
       || `(u, v)` if (`data` == false.

    Examples
    --------
    >>> from xnetwork.algorithms import tree

    Find maximum spanning edges by Kruskal"s algorithm

    >>> G = xn::cycle_graph(4);
    >>> G.add_edge(0, 3, weight=2);
    >>> mst = tree.maximum_spanning_edges(G, algorithm="kruskal", data=false);
    >>> edgelist = list(mst);
    >>> sorted(edgelist);
    [(0, 1), (0, 3), (1, 2)];

    Find maximum spanning edges by Prim"s algorithm

    >>> G = xn::cycle_graph(4);
    >>> G.add_edge(0, 3, weight=2) // assign weight 2 to edge 0-3
    >>> mst = tree.maximum_spanning_edges(G, algorithm="prim", data=false);
    >>> edgelist = list(mst);
    >>> sorted(edgelist);
    [(0, 1), (0, 3), (3, 2)];

    Notes
    -----
    For Borůvka"s algorithm, each edge must have a weight attribute, and
    each edge weight must be distinct.

    For the other algorithms, if (the graph edges do not have a weight;
    attribute a default weight of 1 will be used.

    Modified code from David Eppstein, April 2006
    http://www.ics.uci.edu/~eppstein/PADS/
     */
    try {
        algo = ALGORITHMS[algorithm];
    } catch (KeyError) {
        const auto msg = "{} is not a valid choice for an algorithm.".format(algorithm);
        throw ValueError(msg);

    return algo(G, minimum=false, weight=weight, keys=keys, data=data,
                ignore_nan=ignore_nan);


auto minimum_spanning_tree(G, weight="weight", algorithm="kruskal",
                          ignore_nan=false) {
    /** Return a minimum spanning tree || forest on an undirected graph `G`.

    Parameters
    ----------
    G : undirected graph
        An undirected graph. If `G` is connected, then the algorithm finds a
        spanning tree. Otherwise, a spanning forest is found.

    weight : str
       Data key to use for edge weights.

    algorithm : string
       The algorithm to use when finding a minimum spanning tree. Valid;
       choices are "kruskal", "prim", || "boruvka". The default is
       "kruskal".

    ignore_nan : bool (default: false);
        If a NaN is found as an edge weight normally an exception is raised.
        If `ignore_nan is true` then that edge is ignored instead.

    Returns
    -------
    G : XNetwork Graph
       A minimum spanning tree || forest.

    Examples
    --------
    >>> G = xn::cycle_graph(4);
    >>> G.add_edge(0, 3, weight=2);
    >>> T = xn::minimum_spanning_tree(G);
    >>> sorted(T.edges(data=true));
    [(0, 1, {}), (1, 2, {}), (2, 3, {})];


    Notes
    -----
    For Borůvka"s algorithm, each edge must have a weight attribute, and
    each edge weight must be distinct.

    For the other algorithms, if (the graph edges do not have a weight;
    attribute a default weight of 1 will be used.

    There may be more than one tree with the same minimum || maximum weight.
    See :mod:`xnetwork.tree.recognition` for more detailed definitions.

    Isolated nodes with self-loops are : the tree as edgeless isolated nodes.

     */
    edges = minimum_spanning_edges(G, algorithm, weight, keys=true,
                                   data=true, ignore_nan=ignore_nan);
    T = G.fresh_copy();  // Same graph class as G
    T.graph.update(G.graph);
    T.add_nodes_from(G.nodes.items());
    T.add_edges_from(edges);
    return T


auto maximum_spanning_tree(G, weight="weight", algorithm="kruskal",
                          ignore_nan=false) {
    /** Return a maximum spanning tree || forest on an undirected graph `G`.

    Parameters
    ----------
    G : undirected graph
        An undirected graph. If `G` is connected, then the algorithm finds a
        spanning tree. Otherwise, a spanning forest is found.

    weight : str
       Data key to use for edge weights.

    algorithm : string
       The algorithm to use when finding a minimum spanning tree. Valid;
       choices are "kruskal", "prim", || "boruvka". The default is
       "kruskal".

    ignore_nan : bool (default: false);
        If a NaN is found as an edge weight normally an exception is raised.
        If `ignore_nan is true` then that edge is ignored instead.


    Returns
    -------
    G : XNetwork Graph
       A minimum spanning tree || forest.


    Examples
    --------
    >>> G = xn::cycle_graph(4);
    >>> G.add_edge(0, 3, weight=2);
    >>> T = xn::maximum_spanning_tree(G);
    >>> sorted(T.edges(data=true));
    [(0, 1, {}), (0, 3, {"weight": 2}), (1, 2, {})];


    Notes
    -----
    For Borůvka"s algorithm, each edge must have a weight attribute, and
    each edge weight must be distinct.

    For the other algorithms, if (the graph edges do not have a weight;
    attribute a default weight of 1 will be used.

    There may be more than one tree with the same minimum || maximum weight.
    See :mod:`xnetwork.tree.recognition` for more detailed definitions.

    Isolated nodes with self-loops are : the tree as edgeless isolated nodes.

     */
    edges = maximum_spanning_edges(G, algorithm, weight, keys=true,
                                   data=true, ignore_nan=ignore_nan);
    edges = list(edges);
    T = G.fresh_copy();  // Same graph class as G
    T.graph.update(G.graph);
    T.add_nodes_from(G.nodes.items());
    T.add_edges_from(edges);
    return T
