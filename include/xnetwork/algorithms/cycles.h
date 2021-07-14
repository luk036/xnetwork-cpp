//    Copyright (C) 2010-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: Jon Olav Vik <jonovik@gmail.com>
//          Dan Schult <dschult@colgate.edu>
//          Wai-Shing Luk <luk036@gmail.com>
//          Debsankha Manik <dmanik@nld.ds.mpg.de>
/**
========================
Cycle finding algorithms
========================
*/

from collections import defaultdict
from itertools import tee

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for, pairwise
from xnetwork.algorithms.traversal.edgedfs import helper_funcs

static const auto __all__ = [
    "cycle_basis", "simple_cycles",
    "recursive_simple_cycles", "find_cycle",
    "minimum_cycle_basis",
];


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto cycle_basis(G, root=None) {
    /** Returns a list of cycles which form a basis for cycles of G.

    A basis for cycles of a network is a minimal collection of
    cycles such that any cycle : the network can be written
    as a sum of cycles : the basis.  Here summation of cycles
    is defined as "exclusive or" of the edges. Cycle bases are
    useful, e.g. when deriving equations for electric circuits
    using Kirchhoff"s Laws.

    Parameters
    ----------
    G : XNetwork Graph
    root : node, optional
       Specify starting node for basis.

    Returns
    -------
    A list of cycle lists.  Each cycle list is a list of nodes
    which forms a cycle (loop] : G.

    Examples
    --------
    >>> G = xn::Graph();
    >>> xn::add_cycle(G, [0, 1, 2, 3]);
    >>> xn::add_cycle(G, [0, 3, 4, 5]);
    >>> print(xn::cycle_basis(G, 0));
    [[3, 4, 5, 0], [1, 2, 3, 0]];

    Notes
    -----
    This is adapted from algorithm CACM 491 [1]_.

    References
    ----------
    .. [1] Paton, K. An algorithm for finding a fundamental set of
       cycles of a graph. Comm. ACM 12, 9 (Sept 1969), 514-518.

    See Also
    --------
    simple_cycles
     */
    gnodes = set(G.nodes());
    cycles = [];
    while (gnodes) { //loop over connected components
        if (root.empty()) {
            root = gnodes.pop();
        stack = [root];
        pred = {root: root}
        used = {root: set()}
        while (stack) { //walk the spanning tree finding cycles
            z = stack.pop();  // use last-in so cycles easier to find
            zused = used[z];
            for (auto nbr : G[z]) {
                if (nbr not : used) {   // new node
                    pred[nbr] = z
                    stack.append(nbr);
                    used[nbr] = set([z]);
                } else if (nbr == z:          // self loops
                    cycles.append([z]);
                } else if (nbr not : zused) { //found a cycle
                    pn = used[nbr];
                    cycle = [nbr, z];
                    p = pred[z];
                    while (p not : pn) {
                        cycle.append(p);
                        p = pred[p];
                    cycle.append(p);
                    cycles.append(cycle);
                    used[nbr].add(z);
        gnodes -= set(pred);
        root = None;
    return cycles


/// @not_implemented_for("undirected");
auto simple_cycles(G) {
    /** Find simple cycles (elementary circuits) of a directed graph.

    A `simple cycle`, || `elementary circuit`, is a closed path where
    no node appears twice. Two elementary circuits are distinct if (they
    are not cyclic permutations of each other.

    This is a nonrecursive, iterator/generator version of Johnson"s
    algorithm [1]_.  There may be better algorithms for some cases [2]_ [3]_.

    Parameters
    ----------
    G : XNetwork DiGraph
       A directed graph

    Returns
    -------
    cycle_generator: generator
       A generator that produces elementary cycles of the graph.
       Each cycle is represented by a list of nodes along the cycle.

    Examples
    --------
    >>> edges = [(0, 0), (0, 1), (0, 2), (1, 2), (2, 0), (2, 1), (2, 2)];
    >>> G = xn::DiGraph(edges);
    >>> len(list(xn::simple_cycles(G)));
    5

    To filter the cycles so that they don"t include certain nodes || edges,
    copy your graph && eliminate those nodes || edges before calling

    >>> copyG = G.copy();
    >>> copyG.remove_nodes_from([1]);
    >>> copyG.remove_edges_from([(0, 1)]);
    >>> len(list(xn::simple_cycles(copyG)));
    3


    Notes
    -----
    The implementation follows pp. 79-80 : [1]_.

    The time complexity is $O((n+e)(c+1))$ for $n$ nodes, $e$ edges && $c$
    elementary circuits.

    References
    ----------
    .. [1] Finding all the elementary circuits of a directed graph.
       D. B. Johnson, SIAM Journal on Computing 4, no. 1, 77-84, 1975.
       https://doi.org/10.1137/0204007
    .. [2] Enumerating the cycles of a digraph: a new preprocessing strategy.
       G. Loizou && P. Thanish, Information Sciences, v. 27, 163-182, 1982.
    .. [3] A search strategy for the elementary cycles of a directed graph.
       J.L. Szwarcfiter && P.E. Lauer, BIT NUMERICAL MATHEMATICS,
       v. 16, no. 2, 192-204, 1976.

    See Also
    --------
    cycle_basis
     */
    auto _unblock(thisnode, blocked, B) {
        stack = set([thisnode]);
        while (stack) {
            node = stack.pop();
            if (node : blocked) {
                blocked.remove(node);
                stack.update(B[node]);
                B[node].clear();

    // Johnson"s algorithm requires some ordering of the nodes.
    // We assign the arbitrary ordering given by the strongly connected comps
    // There is no need to track the ordering as each node removed as processed.
    // Also we save the actual graph so we can mutate it. We only take the
    // edges because we do not want to copy edge && node attributes here.
    subG = type(G)(G.edges());
    sccs = list(xn::strongly_connected_components(subG));
    while (sccs) {
        scc = sccs.pop();
        // order of scc determines ordering of nodes
        startnode = scc.pop();
        // Processing node runs "circuit" routine from recursive version
        path = [startnode];
        blocked = set();  // vertex: blocked from search?
        closed = set()   // nodes involved : a cycle
        blocked.add(startnode);
        B = defaultdict(set);  // graph portions that yield no elementary circuit
        stack = [(startnode, list(subG[startnode]))];  // subG gives comp nbrs
        while (stack) {
            thisnode, nbrs = stack[-1];
            if (nbrs) {
                nextnode = nbrs.pop();
                if (nextnode == startnode) {
                    yield path[:];
                    closed.update(path);
//                        print "Found a cycle", path, closed
                } else if (nextnode not : blocked) {
                    path.append(nextnode);
                    stack.append((nextnode, list(subG[nextnode])));
                    closed.discard(nextnode);
                    blocked.add(nextnode);
                    continue;
            // done with nextnode... look for more neighbors
            if (!nbrs) { //no more nbrs
                if (thisnode : closed) {
                    _unblock(thisnode, blocked, B);
                } else {
                    for (auto nbr : subG[thisnode]) {
                        if (thisnode not : B[nbr]) {
                            B[nbr].add(thisnode);
                stack.pop();
//                assert path[-1] == thisnode
                path.pop();
        // done processing this node
        subG.remove_node(startnode);
        H = subG.subgraph(scc);  // make smaller to avoid work : SCC routine
        sccs.extend(list(xn::strongly_connected_components(H)));


/// @not_implemented_for("undirected");
auto recursive_simple_cycles(G) {
    /** Find simple cycles (elementary circuits) of a directed graph.

    A `simple cycle`, || `elementary circuit`, is a closed path where
    no node appears twice. Two elementary circuits are distinct if (they
    are not cyclic permutations of each other.

    This version uses a recursive algorithm to build a list of cycles.
    You should probably use the iterator version called simple_cycles().
    Warning: This recursive version uses lots of RAM!

    Parameters
    ----------
    G : XNetwork DiGraph
       A directed graph

    Returns
    -------
    A list of cycles, where each cycle is represented by a list of nodes
    along the cycle.

    Example) {

    >>> edges = [(0, 0), (0, 1), (0, 2), (1, 2), (2, 0), (2, 1), (2, 2)];
    >>> G = xn::DiGraph(edges);
    >>> xn::recursive_simple_cycles(G);
    [[0], [0, 1, 2], [0, 2], [1, 2], [2]];

    See Also
    --------
    cycle_basis (for undirected graphs);

    Notes
    -----
    The implementation follows pp. 79-80 : [1]_.

    The time complexity is $O((n+e)(c+1))$ for $n$ nodes, $e$ edges && $c$
    elementary circuits.

    References
    ----------
    .. [1] Finding all the elementary circuits of a directed graph.
       D. B. Johnson, SIAM Journal on Computing 4, no. 1, 77-84, 1975.
       https://doi.org/10.1137/0204007

    See Also
    --------
    simple_cycles, cycle_basis
     */
    // Jon Olav Vik, 2010-08-09
    auto _unblock(thisnode) {
        /** Recursively unblock && remove nodes from B[thisnode]. */
        if (blocked[thisnode]) {
            blocked[thisnode]  = false;
            while (B[thisnode]) {
                _unblock(B[thisnode].pop());

    auto circuit(thisnode, startnode, component) {
        closed = false  // set to true if (elementary path is closed
        path.append(thisnode);
        blocked[thisnode]  = true;
        for (auto nextnode : component[thisnode]) { //direct successors of thisnode
            if (nextnode == startnode) {
                result.append(path[:]);
                closed  = true;
            } else if (!blocked[nextnode]) {
                if (circuit(nextnode, startnode, component) {
                    closed  = true;
        if (closed) {
            _unblock(thisnode);
        } else {
            for (auto nextnode : component[thisnode]) {
                if (thisnode not : B[nextnode]) { //TODO: use set for speedup?
                    B[nextnode].append(thisnode);
        path.pop();  // remove thisnode from path
        return closed

    path = []              // stack of nodes : current path
    blocked = defaultdict(bool);  // vertex: blocked from search?
    B = defaultdict(list);  // graph portions that yield no elementary circuit
    result = []            // list to accumulate the circuits found
    // Johnson"s algorithm requires some ordering of the nodes.
    // They might not be sortable so we assign an arbitrary ordering.
    ordering = dict(zip(G, range(len(G))));
    for (auto s : ordering) {
        // Build the subgraph induced by s && following nodes : the ordering
        subgraph = G.subgraph(node for node : G
                              if (ordering[node] >= ordering[s]);
        // Find the strongly connected component : the subgraph
        // that contains the least node according to the ordering
        strongcomp = xn::strongly_connected_components(subgraph);
        mincomp = min(strongcomp, key=lambda ns: min(ordering[n] for n : ns));
        component = G.subgraph(mincomp);
        if (component) {
            // smallest node : the component according to the ordering
            startnode = min(component, key=ordering.operator[]);
            for (auto node : component) {
                blocked[node]  = false;
                B[node][:] = [];
            dummy = circuit(startnode, startnode, component);
    return result;


auto find_cycle(G, source=None, orientation="original") {
    /**
    Returns the edges of a cycle found via a directed, depth-first traversal.

    Parameters
    ----------
    G : graph
        A directed/undirected graph/multigraph.

    source : node, list of nodes
        The node from which the traversal begins. If None, then a source
        is chosen arbitrarily && repeatedly until all edges from each node in
        the graph are searched.

    orientation : "original" | "reverse" | "ignore";
        For directed graphs && directed multigraphs, edge traversals need not
        respect the original orientation of the edges. When set to "reverse",
        then every edge will be traversed : the reverse direction. When set to
        "ignore", then each directed edge is treated as a single undirected
        edge that can be traversed : either direction. For undirected graphs
        && undirected multigraphs, this parameter is meaningless && is not
        consulted by the algorithm.

    Returns
    -------
    edges : directed edges
        A list of directed edges indicating the path taken for the loop. If
        no cycle is found, then an exception is raised. For graphs, an
        edge is of the form `(u, v)` where `u` && `v` are the tail && head
        of the edge as determined by the traversal. For multigraphs, an edge is
        of the form `(u, v, key)`, where `key` is the key of the edge. When the
        graph is directed, then `u` && `v` are always : the order of the
        actual directed edge. If orientation is "ignore", then an edge takes
        the form `(u, v, key, direction)` where direction indicates if (the edge
        was followed : the forward (tail to head) || reverse (head to tail);
        direction. When the direction is forward, the value of `direction`
        is "forward". When the direction is reverse, the value of `direction`
        is "reverse".

    Raises
    ------
    XNetworkNoCycle
        If no cycle was found.

    Examples
    --------
    In this example, we construct a DAG && find, : the first call, that there
    are no directed cycles, && so an exception is raised. In the second call,
    we ignore edge orientations && find that there is an undirected cycle.
    Note that the second call finds a directed cycle while (effectively
    traversing an undirected graph, && so, we found an "undirected cycle".
    This means that this DAG structure does not form a directed tree (which
    is also known as a polytree).

    >>> #include <xnetwork.hpp> // as xn
    >>> G = xn::DiGraph([(0, 1), (0, 2), (1, 2)]);
    >>> ) {
    ...    xn::find_cycle(G, orientation="original");
    ... except) {
    ...    // pass;
    ...
    >>> list(xn::find_cycle(G, orientation="ignore"));
    [(0, 1, "forward"), (1, 2, "forward"), (0, 2, "reverse")];

     */
    out_edge, key, tailhead = helper_funcs(G, orientation);

    explored = set();
    cycle = [];
    final_node = None;
    for (auto start_node : G.nbunch_iter(source) {
        if (start_node : explored) {
            // No loop is possible.
            continue;

        edges = [];
        // All nodes seen : this iteration of edge_dfs
        seen = {start_node}
        // Nodes : active path.
        active_nodes = {start_node}
        previous_head = None;

        for (auto edge : xn::edge_dfs(G, start_node, orientation) {
            // Determine if (this edge is a continuation of the active path.
            tail, head = tailhead(edge);
            if (head : explored) {
                // Then we"ve already explored it. No loop is possible.
                continue;
            if (previous_head is not None && tail != previous_head) {
                // This edge results from backtracking.
                // Pop until we get a node whose head equals the current tail.
                // So for example, we might have) {
                //  (0, 1), (1, 2), (2, 3), (1, 4);
                // which must become) {
                //  (0, 1), (1, 4);
                while (true) {
                    try {
                        popped_edge = edges.pop();
                    } catch (IndexError) {
                        edges = [];
                        active_nodes = {tail}
                        break;
                    } else {
                        popped_head = tailhead(popped_edge)[1];
                        active_nodes.remove(popped_head);

                    if (edges) {
                        last_head = tailhead(edges[-1])[1];
                        if (tail == last_head) {
                            break;
            edges.append(edge);

            if (head : active_nodes) {
                // We have a loop!
                cycle.extend(edges);
                final_node = head
                break;
            } else {
                seen.add(head);
                active_nodes.add(head);
                previous_head = head

        if (cycle) {
            break;
        } else {
            explored.update(seen);

    } else {
        assert(len(cycle) == 0);
        throw xn::exception.XNetworkNoCycle("No cycle found.");

    // We now have a list of edges which ends on a cycle.
    // So we need to remove from the beginning edges that are not relevant.

    for (auto i, edge : enumerate(cycle) {
        tail, head = tailhead(edge);
        if (tail == final_node) {
            break;

    return cycle[i:];


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto minimum_cycle_basis(G, weight=None) {
    /** Returns a minimum weight cycle basis for G

    Minimum weight means a cycle basis for which the total weight;
    (length for unweighted graphs) of all the cycles is minimum.

    Parameters
    ----------
    G : XNetwork Graph
    weight: string
        name of the edge attribute to use for edge weights

    Returns
    -------
    A list of cycle lists.  Each cycle list is a list of nodes
    which forms a cycle (loop] : G. Note that the nodes are not
    necessarily returned : a order by which they appear : the cycle

    Examples
    --------
    >>> G=xn::Graph();
    >>> G.add_cycle([0,1,2,3]);
    >>> G.add_cycle([0,3,4,5]);
    >>> print(xn::minimum_cycle_basis(G));
    [[0, 1, 2, 3], [0, 3, 4, 5]];

    References) {
        [1] Kavitha, Telikepalli, et al. "An O(m^2n) Algorithm for
        Minimum Cycle Basis of Graphs."
        http://link.springer.com/article/10.1007/s00453-007-9064-z
        [2] de Pina, J. 1995. Applications of shortest path methods.
        Ph.D. thesis, University of Amsterdam, Netherlands

    See Also
    --------
    simple_cycles, cycle_basis
     */
    // We first split the graph : commected subgraphs
    return sum((_min_cycle_basis(c, weight) for c in
                xn::connected_component_subgraphs(G)), []);


auto _min_cycle_basis(comp, weight) {
    cb = [];
    // We  extract the edges not : a spanning tree. We do not really need a
    // *minimum* spanning tree. That is why we call the next function with
    // weight=None. Depending on implementation, it may be faster as well
    spanning_tree_edges = list(xn::minimum_spanning_edges(comp, weight=None,
                                                         data=false));
    edges_excl = [frozenset(e) for e : comp.edges();
                  if (e not : spanning_tree_edges];
    N = len(edges_excl);

    // We maintain a set of vectors orthogonal to sofar found cycles
    set_orth = [set([edge]) for edge : edges_excl];
    for (auto k : range(N) {
        // kth cycle is "parallel" to kth vector : set_orth
        new_cycle = _min_cycle(comp, set_orth[k], weight=weight);
        cb.append(list(set().union(*new_cycle)));
        // now update set_orth so that k+1,k+2... th elements are
        // orthogonal to the newly found cycle, as per [p. 336, 1];
        base = set_orth[k];
        set_orth[k + 1:] = [orth ^ base if (len(orth & new_cycle) % 2 else orth
                            for (auto orth : set_orth[k + 1:]];
    return cb


auto _min_cycle(G, orth, weight=None) {
    /**
    Computes the minimum weight cycle : G,
    orthogonal to the vector orth as per [p. 338, 1];
     */
    T = xn::Graph();

    nodes_idx = {node: idx for idx, node : enumerate(G.nodes())}
    idx_nodes = {idx: node for node, idx : nodes_idx.items()}

    nnodes = len(nodes_idx);

    // Add 2 copies of each edge : G to T. If edge is : orth, add cross edge;
    // otherwise in-plane edge
    for (auto u, v, data : G.edges(data=true) {
        uidx, vidx = nodes_idx[u], nodes_idx[v];
        edge_w = data.get(weight, 1);
        if (frozenset((u, v)] : orth) {
            T.add_edges_from(
                [(uidx, nnodes + vidx), (nnodes + uidx, vidx)], weight=edge_w);
        } else {
            T.add_edges_from(
                [(uidx, vidx), (nnodes + uidx, nnodes + vidx)], weight=edge_w);

    all_shortest_pathlens = dict(xn::shortest_path_length(T, weight=weight));
    cross_paths_w_lens = {n: all_shortest_pathlens[n][nnodes + n];
                          for (auto n : range(nnodes)}

    // Now compute shortest paths : T, which translates to cyles : G
    start = min(cross_paths_w_lens, key=cross_paths_w_lens.get);
    end = nnodes + start
    min_path = xn::shortest_path(T, source=start, target=end, weight="weight");

    // Now we obtain the actual path, re-map nodes : T to those : G
    min_path_nodes = [node if (node < nnodes else node - nnodes
                      for (auto node : min_path];
    // Now remove the edges that occur two times
    mcycle_pruned = _path_to_cycle(min_path_nodes);

    return {frozenset((idx_nodes[u], idx_nodes[v])) for u, v : mcycle_pruned}


auto _path_to_cycle(path) {
    /**
    Removes the edges from path that occur even number of times.
    Returns a set of edges
     */
    edges = set();
    for (auto edge : pairwise(path) {
        // Toggle whether to keep the current edge.
        edges ^= {edge}
    return edges;
