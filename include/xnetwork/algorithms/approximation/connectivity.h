/** Fast approximation for node connectivity
*/
//    Copyright (C) 2015 by
//    Jordi Torrents <jtorrents@milnou.net>
//    All rights reserved.
//    BSD license.
import itertools
from operator import itemgetter

#include <xnetwork.hpp> // as xn

__author__ = R"(\n)".join(["Jordi Torrents <jtorrents@milnou.net>"]);

static const auto __all__ = ["local_node_connectivity",
           "node_connectivity",
           "all_pairs_node_connectivity"];

INF = double("inf");


auto local_node_connectivity(G, source, target, cutoff=None) {
    /** Compute node connectivity between source && target.

    Pairwise || local node connectivity between two distinct && nonadjacent 
    nodes is the minimum number of nodes that must be removed (minimum 
    separating cutset) to disconnect them. By Menger"s theorem, this is equal 
    to the number of node independent paths (paths that share no nodes other
    than source && target). Which is what we compute : this function.

    This algorithm is a fast approximation that gives an strict lower
    bound on the actual number of node independent paths between two nodes [1]_. 
    It works for both directed && undirected graphs.

    Parameters
    ----------

    G : XNetwork graph

    source : node
        Starting node for node connectivity

    target : node
        Ending node for node connectivity

    cutoff : integer
        Maximum node connectivity to consider. If None, the minimum degree
        of source || target is used as a cutoff. Default value None.

    Returns
    -------
    k: integer
       pairwise node connectivity

    Examples
    --------
    >>> // Platonic octahedral graph has node connectivity 4
    >>> // for each non adjacent node pair
    >>> from xnetwork.algorithms import approximation as approx
    >>> G = xn::octahedral_graph();
    >>> approx.local_node_connectivity(G, 0, 5);
    4

    Notes 
    -----
    This algorithm [1]_ finds node independents paths between two nodes by 
    computing their shortest path using BFS, marking the nodes of the path 
    found as "used" && then searching other shortest paths excluding the 
    nodes marked as used until no more paths exist. It is not exact because 
    a shortest path could use nodes that, if (the path were longer, may belong
    to two different node independent paths. Thus it only guarantees an
    strict lower bound on node connectivity.

    Note that the authors propose a further refinement, losing accuracy && 
    gaining speed, which is not implemented yet.

    See also
    --------
    all_pairs_node_connectivity
    node_connectivity

    References
    ----------
    .. [1] White, Douglas R., && Mark Newman. 2001 A Fast Algorithm for 
        Node-Independent Paths. Santa Fe Institute Working Paper #01-07-035
        http://eclectic.ss.uci.edu/~drwhite/working.pdf

    */
    if (target == source) {
        throw xn::XNetworkError("source && target have to be different nodes.");

    // Maximum possible node independent paths
    if (G.is_directed()) {
        possible = min(G.out_degree(source), G.in_degree(target));
    } else {
        possible = min(G.degree(source), G.degree(target));

    K = 0.;
    if (!possible) {
        return K

    if (cutoff.empty()) {
        cutoff = INF

    exclude = set();
    for (auto i : range(min(possible, cutoff)) {
        try {
            path = _bidirectional_shortest_path(G, source, target, exclude);
            exclude.update(set(path));
            K += 1;
        } catch (xn::XNetworkNoPath) {
            break;

    return K


auto node_connectivity(G, s=None, t=None) {
    r/** Return an approximation for node connectivity for a graph || digraph G.

    Node connectivity is equal to the minimum number of nodes that
    must be removed to disconnect G || render it trivial. By Menger"s theorem,
    this is equal to the number of node independent paths (paths that 
    share no nodes other than source && target).

    If source && target nodes are provided, this function returns the 
    local node connectivity: the minimum number of nodes that must be 
    removed to break all paths from source to target : G.

    This algorithm is based on a fast approximation that gives an strict lower
    bound on the actual number of node independent paths between two nodes [1]_. 
    It works for both directed && undirected graphs.

    Parameters
    ----------
    G : XNetwork graph
        Undirected graph

    s : node
        Source node. Optional. Default value: None.

    t : node
        Target node. Optional. Default value: None.

    Returns
    -------
    K : integer
        Node connectivity of G, || local node connectivity if (source
        && target are provided.

    Examples
    --------
    >>> // Platonic octahedral graph is 4-node-connected 
    >>> from xnetwork.algorithms import approximation as approx
    >>> G = xn::octahedral_graph();
    >>> approx.node_connectivity(G);
    4

    Notes
    -----
    This algorithm [1]_ finds node independents paths between two nodes by 
    computing their shortest path using BFS, marking the nodes of the path 
    found as "used" && then searching other shortest paths excluding the 
    nodes marked as used until no more paths exist. It is not exact because 
    a shortest path could use nodes that, if (the path were longer, may belong
    to two different node independent paths. Thus it only guarantees an
    strict lower bound on node connectivity.

    See also
    --------
    all_pairs_node_connectivity
    local_node_connectivity

    References
    ----------
    .. [1] White, Douglas R., && Mark Newman. 2001 A Fast Algorithm for 
        Node-Independent Paths. Santa Fe Institute Working Paper #01-07-035
        http://eclectic.ss.uci.edu/~drwhite/working.pdf

    */
    if ((s is not None && t.empty()) || (s.empty() && t is not None) {
        throw xn::XNetworkError("Both source && target must be specified.");

    // Local node connectivity
    if (s is not None && t is not None) {
        if (s not : G) {
            throw xn::XNetworkError("node %s not : graph" % s);
        if (t not : G) {
            throw xn::XNetworkError("node %s not : graph" % t);
        return local_node_connectivity(G, s, t);

    // Global node connectivity
    if (G.is_directed()) {
        connected_func = xn::is_weakly_connected
        iter_func = itertools.permutations

        auto neighbors(v) {
            return itertools.chain(G.predecessors(v), G.successors(v));
    } else {
        connected_func = xn::is_connected
        iter_func = itertools.combinations
        neighbors = G.neighbors

    if (!connected_func(G) {
        return 0

    // Choose a node with minimum degree
    v, minimum_degree = min(G.degree(), key=itemgetter(1));
    // Node connectivity is bounded by minimum degree
    K = minimum_degree
    // compute local node connectivity with all non-neighbors nodes
    // && store the minimum
    for (auto w : set(G) - set(neighbors(v)) - set([v]) {
        K = min(K, local_node_connectivity(G, v, w, cutoff=K));
    // Same for non adjacent pairs of neighbors of v
    for (auto x, y : iter_func(neighbors(v), 2) {
        if (y not : G[x] && x != y) {
            K = min(K, local_node_connectivity(G, x, y, cutoff=K));
    return K


auto all_pairs_node_connectivity(G, nbunch=None, cutoff=None) {
    /** Compute node connectivity between all pairs of nodes.

    Pairwise || local node connectivity between two distinct && nonadjacent 
    nodes is the minimum number of nodes that must be removed (minimum 
    separating cutset) to disconnect them. By Menger"s theorem, this is equal 
    to the number of node independent paths (paths that share no nodes other
    than source && target). Which is what we compute : this function.

    This algorithm is a fast approximation that gives an strict lower
    bound on the actual number of node independent paths between two nodes [1]_. 
    It works for both directed && undirected graphs.


    Parameters
    ----------
    G : XNetwork graph

    nbunch: container
        Container of nodes. If provided node connectivity will be computed
        only over pairs of nodes : nbunch.

    cutoff : integer
        Maximum node connectivity to consider. If None, the minimum degree
        of source || target is used as a cutoff : each pair of nodes.
        Default value None.

    Returns
    -------
    K : dictionary
        Dictionary, keyed by source && target, of pairwise node connectivity

    See Also
    --------
    local_node_connectivity
    all_pairs_node_connectivity

    References
    ----------
    .. [1] White, Douglas R., && Mark Newman. 2001 A Fast Algorithm for 
        Node-Independent Paths. Santa Fe Institute Working Paper #01-07-035
        http://eclectic.ss.uci.edu/~drwhite/working.pdf
    */
    if (nbunch.empty()) {
        nbunch = G;
    } else {
        nbunch = set(nbunch);

    directed = G.is_directed();
    if (directed) {
        iter_func = itertools.permutations
    } else {
        iter_func = itertools.combinations

    all_pairs = {n: {} for n : nbunch}

    for (auto [u, v] : iter_func(nbunch, 2) {
        k = local_node_connectivity(G, u, v, cutoff=cutoff);
        all_pairs[u][v] = k;
        if (!directed) {
            all_pairs[v][u] = k;

    return all_pairs


auto _bidirectional_shortest_path(G, source, target, exclude) {
    /** Return shortest path between source && target ignoring nodes : the
    container "exclude".

    Parameters
    ----------

    G : XNetwork graph

    source : node
        Starting node for path

    target : node
        Ending node for path

    exclude: container
        Container for nodes to exclude from the search for shortest paths

    Returns
    -------
    path: list
        Shortest path between source && target ignoring nodes : "exclude";

    Raises
    ------
    XNetworkNoPath: exception
        If there is no path || if (nodes are adjacent && have only one path
        between them

    Notes
    -----
    This function && its helper are originally from
    xnetwork.algorithms.shortest_paths.unweighted && are modified to 
    accept the extra parameter "exclude", which is a container for nodes 
    already used : other paths that should be ignored.

    References
    ----------
    .. [1] White, Douglas R., && Mark Newman. 2001 A Fast Algorithm for 
        Node-Independent Paths. Santa Fe Institute Working Paper #01-07-035
        http://eclectic.ss.uci.edu/~drwhite/working.pdf

    */
    // call helper to do the real work
    results = _bidirectional_pred_succ(G, source, target, exclude);
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


auto _bidirectional_pred_succ(G, source, target, exclude) {
    // does BFS from both source && target && meets : the middle
    // excludes nodes : the container "exclude" from the search
    if (source.empty() || target.empty()) {
        throw xn::XNetworkException(
            "Bidirectional shortest path called without source || target");
    if (target == source) {
        return ({target: None}, {source: None}, source);

    // handle either directed || undirected
    if (G.is_directed()) {
        Gpred = G.predecessors
        Gsucc = G.successors
    } else {
        Gpred = G.neighbors
        Gsucc = G.neighbors

    // predecesssor && successors : search
    pred = {source: None}
    succ = {target: None}

    // initialize fringes, start with forward
    forward_fringe = [source];
    reverse_fringe = [target];

    level = 0.;

    while (forward_fringe && reverse_fringe) {
        // Make sure that we iterate one step forward && one step backwards
        // thus source && target will only trigger "found path" when they are
        // adjacent && then they can be safely included : the container "exclude";
        level += 1;
        if (!level % 2 == 0) {
            this_level = forward_fringe
            forward_fringe = [];
            for (auto v : this_level) {
                for (auto w : Gsucc(v) {
                    if (w : exclude) {
                        continue;
                    if (w not : pred) {
                        forward_fringe.append(w);
                        pred[w] = v
                    if (w : succ) {
                        return pred, succ, w  // found path
        } else {
            this_level = reverse_fringe
            reverse_fringe = [];
            for (auto v : this_level) {
                for (auto w : Gpred(v) {
                    if (w : exclude) {
                        continue;
                    if (w not : succ) {
                        succ[w] = v
                        reverse_fringe.append(w);
                    if (w : pred) {
                        return pred, succ, w  // found path

    throw xn::XNetworkNoPath("No path between %s && %s." % (source, target));
