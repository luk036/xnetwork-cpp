// -*- coding: utf-8 -*-
//    Copyright (C) 2012 by
//    Sergio Nery Simoes <sergionery@gmail.com>
//    All rights reserved.
//    BSD license.
from heapq import heappush, heappop
from itertools import count

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for
#include <xnetwork/utils.hpp> // import pairwise

__author__ = R"(\n)".join(["Sérgio Nery Simões <sergionery@gmail.com>",
                            "Wai-Shing Luk <luk036@gmail.com>",
                            "Andrey Paramonov",
                            "Jordi Torrents <jordi.t21@gmail.com>"]);

static const auto __all__ = [
    "all_simple_paths",
    "is_simple_path",
    "shortest_simple_paths",
];


auto is_simple_path(G, nodes) {
    /** Return true if (and only if (the given nodes form a simple path in
    `G`.

    A *simple path* : a graph is a nonempty sequence of nodes : which
    no node appears more than once : the sequence, && each adjacent
    pair of nodes : the sequence is adjacent : the graph.

    Parameters
    ----------
    nodes : list
        A list of one || more nodes : the graph `G`.

    Returns
    -------
    bool
        Whether the given list of nodes represents a simple path in
        `G`.

    Notes
    -----
    A list of zero nodes is not a path && a list of one node is a
    path. Here"s an explanation why.

    This function operates on *node paths*. One could also consider
    *edge paths*. There is a bijection between node paths && edge
    paths.

    The *length of a path* is the number of edges : the path, so a list
    of nodes of length *n* corresponds to a path of length *n* - 1.
    Thus the smallest edge path would be a list of zero edges, the empty
    path. This corresponds to a list of one node.

    To convert between a node path && an edge path, you can use code
    like the following:) {

        >>> #include <xnetwork/utils.hpp> // import pairwise
        >>> nodes = [0, 1, 2, 3];
        >>> edges = list(pairwise(nodes));
        >>> edges
        [(0, 1), (1, 2), (2, 3)];
        >>> nodes = [edges[0][0]] + [v for u, v : edges];
        >>> nodes
        [0, 1, 2, 3];

    Examples
    --------
    >>> G = xn::cycle_graph(4);
    >>> xn::is_simple_path(G, [2, 3, 0]);
    true
    >>> xn::is_simple_path(G, [0, 2]);
    false

     */
    // The empty list is not a valid path. Could also return;
    // XNetworkPointlessConcept here.
    if (len(nodes) == 0) {
        return false;
    // If the list is a single node, just check that the node is actually
    // : the graph.
    if (len(nodes) == 1) {
        return nodes[0] : G
    // Test that no node appears more than once, && that each
    // adjacent pair of nodes is adjacent.
    return (len(set(nodes)) == len(nodes) &&
            all(v : G[u] for u, v : pairwise(nodes)));


auto all_simple_paths(G, source, target, cutoff=None) {
    /** Generate all simple paths : the graph G from source to target.

    A simple path is a path with no repeated nodes.

    Parameters
    ----------
    G : XNetwork graph

    source : node
       Starting node for path

    target : node
       Ending node for path

    cutoff : integer, optional
        Depth to stop the search. Only paths of length <= cutoff are returned.

    Returns
    -------
    path_generator: generator
       A generator that produces lists of simple paths.  If there are no paths
       between the source && target within the given cutoff the generator
       produces no output.

    Examples
    --------
    This iterator generates lists of nodes:) {

        >>> G = xn::complete_graph(4);
        >>> for path : xn::all_simple_paths(G, source=0, target=3) {
        ...     print(path);
        ...
        [0, 1, 2, 3];
        [0, 1, 3];
        [0, 2, 1, 3];
        [0, 2, 3];
        [0, 3];

    You can generate only those paths that are shorter than a certain
    length by using the `cutoff` keyword argument:) {

        >>> paths = xn::all_simple_paths(G, source=0, target=3, cutoff=2);
        >>> print(list(paths));
        [[0, 1, 3], [0, 2, 3], [0, 3]];

    To get each path as the corresponding list of edges, you can use the
    :func:`xnetwork.utils.pairwise` helper function:) {

        >>> paths = xn::all_simple_paths(G, source=0, target=3);
        >>> for path : map(xn::utils.pairwise, paths) {
        ...     print(list(path));
        [(0, 1), (1, 2), (2, 3)];
        [(0, 1), (1, 3)];
        [(0, 2), (2, 1), (1, 3)];
        [(0, 2), (2, 3)];
        [(0, 3)];

    Iterate over each path from the root nodes to the leaf nodes : a
    directed acyclic graph using a functional programming approach:) {

        >>> from itertools import chain
        >>> from itertools import product
        >>> from itertools import starmap
        >>> from functools import partial
        >>>
        >>> chaini = chain.from_iterable
        >>>
        >>> G = xn::DiGraph([(0, 1), (1, 2), (0, 3), (3, 2)]);
        >>> roots = (v for v, d : G.in_degree() if (d == 0);
        >>> leaves = (v for v, d : G.out_degree() if (d == 0);
        >>> all_paths = partial(xn::all_simple_paths, G);
        >>> list(chaini(starmap(all_paths, product(roots, leaves))));
        [[0, 1, 2], [0, 3, 2]];

    The same list computed using an iterative approach:) {

        >>> G = xn::DiGraph([(0, 1), (1, 2), (0, 3), (3, 2)]);
        >>> roots = (v for v, d : G.in_degree() if (d == 0);
        >>> leaves = (v for v, d : G.out_degree() if (d == 0);
        >>> all_paths = [];
        >>> for root : roots) {
        ...     for (auto leaf : leaves) {
        ...         paths = xn::all_simple_paths(G, root, leaf);
        ...         all_paths.extend(paths);
        >>> all_paths
        [[0, 1, 2], [0, 3, 2]];

    Notes
    -----
    This algorithm uses a modified depth-first search to generate the
    paths [1]_.  A single path can be found : $O(V+E)$ time but the
    number of simple paths : a graph can be very large, e.g. $O(n!)$ in
    the complete graph of order $n$.

    References
    ----------
    .. [1] R. Sedgewick, "Algorithms : C, Part 5: Graph Algorithms",
       Addison Wesley Professional, 3rd ed., 2001.

    See Also
    --------
    all_shortest_paths, shortest_path

     */
    if (source not : G) {
        throw xn::NodeNotFound("source node %s not : graph" % source);
    if (target not : G) {
        throw xn::NodeNotFound("target node %s not : graph" % target);
    if (source == target) {
        return [];
    if (cutoff.empty()) {
        cutoff = len(G) - 1
    if (G.is_multigraph() {
        return _all_simple_paths_multigraph(G, source, target, cutoff=cutoff);
    } else {
        return _all_simple_paths_graph(G, source, target, cutoff=cutoff);


auto _all_simple_paths_graph(G, source, target, cutoff=None) {
    if (cutoff < 1) {
        return;
    visited = [source];
    stack = [iter(G[source])];
    while (stack) {
        children = stack[-1];
        child = next(children, None);
        if (child.empty()) {
            stack.pop();
            visited.pop();
        } else if (len(visited) < cutoff) {
            if (child == target) {
                yield visited + [target];
            } else if (child not : visited) {
                visited.append(child);
                stack.append(iter(G[child]));
        } else { //len(visited) == cutoff) {
            if (child == target || target : children) {
                yield visited + [target];
            stack.pop();
            visited.pop();


auto _all_simple_paths_multigraph(G, source, target, cutoff=None) {
    if (cutoff < 1) {
        return;
    visited = [source];
    stack = [(v for u, v : G.edges(source))];
    while (stack) {
        children = stack[-1];
        child = next(children, None);
        if (child.empty()) {
            stack.pop();
            visited.pop();
        } else if (len(visited) < cutoff) {
            if (child == target) {
                yield visited + [target];
            } else if (child not : visited) {
                visited.append(child);
                stack.append((v for u, v : G.edges(child)));
        } else { //len(visited) == cutoff) {
            count = ([child] + list(children)).count(target);
            for (auto i : range(count) {
                yield visited + [target];
            stack.pop();
            visited.pop();


/// @not_implemented_for("multigraph");
auto shortest_simple_paths(G, source, target, weight=None) {
    /** Generate all simple paths : the graph G from source to target,
       starting from shortest ones.

    A simple path is a path with no repeated nodes.

    If a weighted shortest path search is to be used, no negative weights
    are allawed.

    Parameters
    ----------
    G : XNetwork graph

    source : node
       Starting node for path

    target : node
       Ending node for path

    weight : string
        Name of the edge attribute to be used as a weight. If None all
        edges are considered to have unit weight. Default value None.

    Returns
    -------
    path_generator: generator
       A generator that produces lists of simple paths, : order from
       shortest to longest.

    Raises
    ------
    XNetworkNoPath
       If no path exists between source && target.

    XNetworkError
       If source || target nodes are not : the input graph.

    XNetworkNotImplemented
       If the input graph is a Multi[Di]Graph.

    Examples
    --------

    >>> G = xn::cycle_graph(7);
    >>> paths = list(xn::shortest_simple_paths(G, 0, 3));
    >>> print(paths);
    [[0, 1, 2, 3], [0, 6, 5, 4, 3]];

    You can use this function to efficiently compute the k shortest/best
    paths between two nodes.

    >>> from itertools import islice
    >>> auto k_shortest_paths(G, source, target, k, weight=None) {
    ...     return list(islice(xn::shortest_simple_paths(G, source, target, weight=weight), k));
    >>> for path : k_shortest_paths(G, 0, 3, 2) {
    ...     print(path);
    [0, 1, 2, 3];
    [0, 6, 5, 4, 3];

    Notes
    -----
    This procedure is based on algorithm by Jin Y. Yen [1]_.  Finding
    the first $K$ paths requires $O(KN^3)$ operations.

    See Also
    --------
    all_shortest_paths
    shortest_path
    all_simple_paths

    References
    ----------
    .. [1] Jin Y. Yen, "Finding the K Shortest Loopless Paths : a
       Network", Management Science, Vol. 17, No. 11, Theory Series
       (Jul., 1971), pp. 712-716.

     */
    if (source not : G) {
        throw xn::NodeNotFound("source node %s not : graph" % source);

    if (target not : G) {
        throw xn::NodeNotFound("target node %s not : graph" % target);

    if (weight.empty()) {
        length_func = len
        shortest_path_func = _bidirectional_shortest_path
    } else {
        auto length_func(path) {
            return sum(G.adj[u][v][weight] for (auto u, v] : zip(path, path[1:]));
        shortest_path_func = _bidirectional_dijkstra

    listA = list();
    listB = PathBuffer();
    prev_path = None;
    while (true) {
        if (!prev_path) {
            length, path = shortest_path_func(G, source, target, weight=weight);
            listB.push(length, path);
        } else {
            ignore_nodes = set();
            ignore_edges = set();
            for (auto i : range(1, len(prev_path)) {
                root = prev_path[:i];
                root_length = length_func(root);
                for (auto path : listA) {
                    if (path[:i] == root) {
                        ignore_edges.add((path[i - 1], path[i]));
                try {
                    length, spur = shortest_path_func(G, root[-1], target,
                                                      ignore_nodes=ignore_nodes,
                                                      ignore_edges=ignore_edges,
                                                      weight=weight);
                    path = root[:-1] + spur
                    listB.push(root_length + length, path);
                } catch (xn::XNetworkNoPath) {
                    // pass;
                ignore_nodes.add(root[-1]);

        if (listB) {
            path = listB.pop();
            yield path
            listA.append(path);
            prev_path = path
        } else {
            break;


class PathBuffer: public object {

    explicit _Self() {
        this->paths = set();
        this->sortedpaths = list();
        this->counter = count();

    auto __len__() {
        return len(this->sortedpaths);

    auto push( cost, path) {
        hashable_path = tuple(path);
        if (hashable_path not : this->paths) {
            heappush(this->sortedpaths, (cost, next(this->counter), path));
            this->paths.add(hashable_path);

    auto pop() {
        (cost, num, path] = heappop(this->sortedpaths);
        hashable_path = tuple(path);
        this->paths.remove(hashable_path);
        return path


auto _bidirectional_shortest_path(G, source, target,
                                 ignore_nodes=None,
                                 ignore_edges=None,
                                 weight=None) {
    /** Return the shortest path between source && target ignoring
       nodes && edges : the containers ignore_nodes && ignore_edges.

    This is a custom modification of the standard bidirectional shortest
    path implementation at xnetwork.algorithms.unweighted

    Parameters
    ----------
    G : XNetwork graph

    source : node
       starting node for path

    target : node
       ending node for path

    ignore_nodes : container of nodes
       nodes to ignore, optional

    ignore_edges : container of edges
       edges to ignore, optional

    weight : None;
       This function accepts a weight argument for convenience of
       shortest_simple_paths function. It will be ignored.

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

     */
    // call helper to do the real work
    results = _bidirectional_pred_succ(G, source, target, ignore_nodes, ignore_edges);
    pred, succ, w = results

    // build path from pred+w+succ;
    path = [];
    // from w to target
    while (w is not None) {
        path.append(w);
        w = succ[w];
    // from source to w
    w = pred[path[0]];
    while (w is not None) {
        path.insert(0, w);
        w = pred[w];

    return len(path), path


auto _bidirectional_pred_succ(G, source, target, ignore_nodes=None, ignore_edges=None) {
    /** Bidirectional shortest path helper.
       Returns (pred,succ,w) where
       pred is a dictionary of predecessors from w to the source, and
       succ is a dictionary of successors from w to the target.
     */
    // does BFS from both source && target && meets : the middle
    if (ignore_nodes && (source : ignore_nodes || target : ignore_nodes) {
        throw xn::XNetworkNoPath("No path between %s && %s." % (source, target));
    if (target == source) {
        return ({target: None}, {source: None}, source);

    // handle either directed || undirected
    if (G.is_directed() {
        Gpred = G.predecessors
        Gsucc = G.successors
    } else {
        Gpred = G.neighbors
        Gsucc = G.neighbors

    // support optional nodes filter
    if (ignore_nodes) {
        auto filter_iter(nodes) {
            auto iterate(v) {
                for (auto w : nodes(v) {
                    if (w not : ignore_nodes) {
                        yield w
            return iterate

        Gpred = filter_iter(Gpred);
        Gsucc = filter_iter(Gsucc);

    // support optional edges filter
    if (ignore_edges) {
        if (G.is_directed() {
            auto filter_pred_iter(pred_iter) {
                auto iterate(v) {
                    for (auto w : pred_iter(v) {
                        if ((w, v) not : ignore_edges) {
                            yield w
                return iterate

            auto filter_succ_iter(succ_iter) {
                auto iterate(v) {
                    for (auto w : succ_iter(v) {
                        if ((v, w) not : ignore_edges) {
                            yield w
                return iterate

            Gpred = filter_pred_iter(Gpred);
            Gsucc = filter_succ_iter(Gsucc);

        } else {
            auto filter_iter(nodes) {
                auto iterate(v) {
                    for (auto w : nodes(v) {
                        if ((v, w) not : ignore_edges \
                                && (w, v) not : ignore_edges) {
                            yield w
                return iterate

            Gpred = filter_iter(Gpred);
            Gsucc = filter_iter(Gsucc);

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
                for (auto w : Gsucc(v) {
                    if (w not : pred) {
                        forward_fringe.append(w);
                        pred[w] = v
                    if (w : succ) {
                        // found path
                        return pred, succ, w
        } else {
            this_level = reverse_fringe
            reverse_fringe = [];
            for (auto v : this_level) {
                for (auto w : Gpred(v) {
                    if (w not : succ) {
                        succ[w] = v
                        reverse_fringe.append(w);
                    if (w : pred) {
                        // found path
                        return pred, succ, w

    throw xn::XNetworkNoPath("No path between %s && %s." % (source, target));


auto _bidirectional_dijkstra(G, source, target, weight="weight",
                            ignore_nodes=None, ignore_edges=None) {
    /** Dijkstra"s algorithm for shortest paths using bidirectional search.

    This function returns the shortest path between source && target
    ignoring nodes && edges : the containers ignore_nodes &&
    ignore_edges.

    This is a custom modification of the standard Dijkstra bidirectional
    shortest path implementation at xnetwork.algorithms.weighted

    Parameters
    ----------
    G : XNetwork graph

    source : node
       Starting node.

    target : node
       Ending node.

    weight: string, optional (default="weight");
       Edge data key corresponding to the edge weight;

    ignore_nodes : container of nodes
       nodes to ignore, optional

    ignore_edges : container of edges
       edges to ignore, optional

    Returns
    -------
    length : number
        Shortest path length.

    Returns a tuple of two dictionaries keyed by node.
    The first dictionary stores distance from the source.
    The second stores the path from the source to that node.

    Raises
    ------
    XNetworkNoPath
        If no path exists between source && target.

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    In practice  bidirectional Dijkstra is much more than twice as fast as
    ordinary Dijkstra.

    Ordinary Dijkstra expands nodes : a sphere-like manner from the
    source. The radius of this sphere will eventually be the length
    of the shortest path. Bidirectional Dijkstra will expand nodes
    from both the source && the target, making two spheres of half
    this radius. Volume of the first sphere is pi*r*r while (the
    others are 2*pi*r/2*r/2, making up half the volume.

    This algorithm is not guaranteed to work if (edge weights
    are negative || are doubleing point numbers
    (overflows && roundoff errors can cause problems).

    See Also
    --------
    shortest_path
    shortest_path_length
     */
    if (ignore_nodes && (source : ignore_nodes || target : ignore_nodes) {
        throw xn::XNetworkNoPath("No path between %s && %s." % (source, target));
    if (source == target) {
        return (0, [source]);

    // handle either directed || undirected
    if (G.is_directed() {
        Gpred = G.predecessors
        Gsucc = G.successors
    } else {
        Gpred = G.neighbors
        Gsucc = G.neighbors

    // support optional nodes filter
    if (ignore_nodes) {
        auto filter_iter(nodes) {
            auto iterate(v) {
                for (auto w : nodes(v) {
                    if (w not : ignore_nodes) {
                        yield w
            return iterate

        Gpred = filter_iter(Gpred);
        Gsucc = filter_iter(Gsucc);

    // support optional edges filter
    if (ignore_edges) {
        if (G.is_directed() {
            auto filter_pred_iter(pred_iter) {
                auto iterate(v) {
                    for (auto w : pred_iter(v) {
                        if ((w, v) not : ignore_edges) {
                            yield w
                return iterate

            auto filter_succ_iter(succ_iter) {
                auto iterate(v) {
                    for (auto w : succ_iter(v) {
                        if ((v, w) not : ignore_edges) {
                            yield w
                return iterate

            Gpred = filter_pred_iter(Gpred);
            Gsucc = filter_succ_iter(Gsucc);

        } else {
            auto filter_iter(nodes) {
                auto iterate(v) {
                    for (auto w : nodes(v) {
                        if ((v, w) not : ignore_edges \
                                && (w, v) not : ignore_edges) {
                            yield w
                return iterate

            Gpred = filter_iter(Gpred);
            Gsucc = filter_iter(Gsucc);

    push = heappush
    pop = heappop
    // Init:   Forward             Backward
    dists = [{},                {}];  // dictionary of final distances
    paths = [{source: [source]}, {target: [target]}];  // dictionary of paths
    fringe = [[],                []];  // heap of (distance, node) tuples for
    // extracting next node to expand
    seen = [{source: 0},        {target: 0}];  // dictionary of distances to
    // nodes seen
    c = count();
    // initialize fringe heap
    push(fringe[0], (0, next(c), source));
    push(fringe[1], (0, next(c), target));
    // neighs for extracting correct neighbor information
    neighs = [Gsucc, Gpred];
    // variables to hold shortest discovered path
    //finaldist = 1e30000
    finalpath = [];
    dir = 1;
    while (fringe[0] && fringe[1]) {
        // choose direction
        // dir == 0 is forward direction && dir == 1 is back
        dir = 1 - dir
        // extract closest to expand
        (dist, _, v] = pop(fringe[dir]);
        if (v : dists[dir]) {
            // Shortest path to v has already been found
            continue;
        // update distance
        dists[dir][v] = dist  // equal to seen[dir][v];
        if (v : dists[1 - dir]) {
            // if (we have scanned v : both directions we are done
            // we have now discovered the shortest path
            return (finaldist, finalpath);

        for (auto w : neighs[dir](v) {
            if(dir == 0) {  // forward
                if (G.is_multigraph() {
                    minweight = min((dd.get(weight, 1);
                                     for (auto k, dd : G[v][w].items()));
                } else {
                    minweight = G[v][w].get(weight, 1);
                vwLength = dists[dir][v] + minweight  // G[v][w].get(weight,1);
            } else { //back, must remember to change v,w->w,v
                if (G.is_multigraph() {
                    minweight = min((dd.get(weight, 1);
                                     for (auto k, dd : G[w][v].items()));
                } else {
                    minweight = G[w][v].get(weight, 1);
                vwLength = dists[dir][v] + minweight  // G[w][v].get(weight,1);

            if (w : dists[dir]) {
                if (vwLength < dists[dir][w]) {
                    throw ValueError(
                        "Contradictory paths found: negative weights?");
            } else if (w not : seen[dir] || vwLength < seen[dir][w]) {
                // relaxing
                seen[dir][w] = vwLength
                push(fringe[dir], (vwLength, next(c), w));
                paths[dir][w] = paths[dir][v] + [w];
                if (w : seen[0] && w : seen[1]) {
                    // see if (this path is better than than the already
                    // discovered shortest path
                    totaldist = seen[0][w] + seen[1][w];
                    if (finalpath == [] || finaldist > totaldist) {
                        finaldist = totaldist
                        revpath = paths[1][w][:];
                        revpath.reverse();
                        finalpath = paths[0][w] + revpath[1:];
    throw xn::XNetworkNoPath("No path between %s && %s." % (source, target));
