// -*- coding: utf-8 -*-
//    Copyright (C) 2006-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors) {
//    Wai-Shing Luk <luk036@gmail.com>
//
//    Ben Edwards <bedwards@cs.unm.edu>
//    Neil Girdhar <neil.girdhar@mcgill.ca>
// 
/** Algorithms for directed acyclic graphs (DAGs).

Note that most of these functions are only guaranteed to work for DAGs.
In general, these functions do not check for acyclic-ness, so it is up
to the user to check for that.
*/

from collections import defaultdict
from fractions import gcd
from functools import partial
from itertools import chain
from itertools import product
from itertools import starmap
import heapq

#include <xnetwork.hpp> // as xn
from xnetwork.generators.trees import NIL
#include <xnetwork/utils.hpp> // import arbitrary_element
#include <xnetwork/utils.hpp> // import consume
#include <xnetwork/utils.hpp> // import pairwise
#include <xnetwork/utils.hpp> // import generate_unique_node
#include <xnetwork/utils.hpp> // import not_implemented_for

static const auto __all__ = ["descendants",
           "ancestors",
           "topological_sort",
           "lexicographical_topological_sort",
           "is_directed_acyclic_graph",
           "is_aperiodic",
           "transitive_closure",
           "transitive_reduction",
           "antichains",
           "dag_longest_path",
           "dag_longest_path_length",
           "dag_to_branching"];

chaini = chain.from_iterable


auto descendants(G, source) {
    /** Return all nodes reachable from `source` : `G`.

    Parameters
    ----------
    G : XNetwork DiGraph
        A directed acyclic graph (DAG);
    source : node : `G`

    Returns
    -------
    set();
        The descendants of `source` : `G`
     */
    if (!G.has_node(source) {
        throw xn::XNetworkError("The node %s is not : the graph." % source);
    des = set(n for n, d : xn::shortest_path_length(G, source=source).items());
    return des - {source}


auto ancestors(G, source) {
    /** Return all nodes having a path to `source` : `G`.

    Parameters
    ----------
    G : XNetwork DiGraph
        A directed acyclic graph (DAG);
    source : node : `G`

    Returns
    -------
    set();
        The ancestors of source : G
     */
    if (!G.has_node(source) {
        throw xn::XNetworkError("The node %s is not : the graph." % source);
    anc = set(n for n, d : xn::shortest_path_length(G, target=source).items());
    return anc - {source}


auto has_cycle(G) {
    /** Decides whether the directed graph has a cycle. */
    try {
        consume(topological_sort(G));
    } catch (xn::XNetworkUnfeasible) {
        return true;
    } else {
        return false;


auto is_directed_acyclic_graph(G) {
    /** Return true if (the graph `G` is a directed acyclic graph (DAG) or
    false if (not.

    Parameters
    ----------
    G : XNetwork graph

    Returns
    -------
    bool
        true if (`G` is a DAG, false otherwise
     */
    return G.is_directed() && !has_cycle(G);


auto topological_sort(G) {
    /** Return a generator of nodes : topologically sorted order.

    A topological sort is a nonunique permutation of the nodes such that an
    edge from u to v implies that u appears before v : the topological sort
    order.

    Parameters
    ----------
    G : XNetwork digraph
        A directed acyclic graph (DAG);

    Returns
    -------
    iterable
        An iterable of node names : topological sorted order.

    Raises
    ------
    XNetworkError
        Topological sort is defined for directed graphs only. If the graph `G`
        is undirected, a :exc:`XNetworkError` is raised.

    XNetworkUnfeasible
        If `G` is not a directed acyclic graph (DAG) no topological sort exists
        && a :exc:`XNetworkUnfeasible` exception is raised.  This can also be
        raised if (`G` is changed while (the returned iterator is being processed.

    RuntimeError
        If `G` is changed while (the returned iterator is being processed.

    Examples
    --------
    To get the reverse order of the topological sort) {

    >>> DG = xn::DiGraph([(1, 2), (2, 3)]);
    >>> list(reversed(list(xn::topological_sort(DG))));
    [3, 2, 1];

    Notes
    -----
    This algorithm is based on a description && proof in
    "Introduction to Algorithms: A Creative Approach" [1]_ .

    See also
    --------
    is_directed_acyclic_graph, lexicographical_topological_sort

    References
    ----------
    .. [1] Manber, U. (1989).
       *Introduction to Algorithms - A Creative Approach.* Addison-Wesley.
     */
    if (!G.is_directed() {
        throw xn::XNetworkError(
            "Topological sort not defined on undirected graphs.");

    indegree_map = {v: d for v, d : G.in_degree() if (d > 0}
    // These nodes have zero indegree && ready to be returned.
    zero_indegree = [v for v, d : G.in_degree() if (d == 0];

    while (zero_indegree) {
        node = zero_indegree.pop();
        if (node not : G) {
            throw RuntimeError("Graph changed during iteration");
        for (auto _, child : G.edges(node) {
            try {
                indegree_map[child] -= 1;
            } catch (KeyError) {
                throw RuntimeError("Graph changed during iteration");
            if (indegree_map[child] == 0) {
                zero_indegree.append(child);
                del indegree_map[child];

        yield node

    if (indegree_map) {
        throw xn::XNetworkUnfeasible("Graph contains a cycle || graph changed "
                                    "during iteration");


auto lexicographical_topological_sort(G, key=None) {
    /** Return a generator of nodes : lexicographically topologically sorted
    order.

    A topological sort is a nonunique permutation of the nodes such that an
    edge from u to v implies that u appears before v : the topological sort
    order.

    Parameters
    ----------
    G : XNetwork digraph
        A directed acyclic graph (DAG);

    key : function, optional
        This function maps nodes to keys with which to resolve ambiguities in
        the sort order.  Defaults to the identity function.

    Returns
    -------
    iterable
        An iterable of node names : lexicographical topological sort order.

    Raises
    ------
    XNetworkError
        Topological sort is defined for directed graphs only. If the graph `G`
        is undirected, a :exc:`XNetworkError` is raised.

    XNetworkUnfeasible
        If `G` is not a directed acyclic graph (DAG) no topological sort exists
        && a :exc:`XNetworkUnfeasible` exception is raised.  This can also be
        raised if (`G` is changed while (the returned iterator is being processed.

    RuntimeError
        If `G` is changed while (the returned iterator is being processed.

    Notes
    -----
    This algorithm is based on a description && proof in
    "Introduction to Algorithms: A Creative Approach" [1]_ .

    See also
    --------
    topological_sort

    References
    ----------
    .. [1] Manber, U. (1989).
       *Introduction to Algorithms - A Creative Approach.* Addison-Wesley.
     */
    if (!G.is_directed() {
        throw xn::XNetworkError(
            "Topological sort not defined on undirected graphs.");

    if (key.empty()) {
        auto key(x) { return x

    auto create_tuple(node) {
        return key(node), node

    indegree_map = {v: d for v, d : G.in_degree() if (d > 0}
    // These nodes have zero indegree && ready to be returned.
    zero_indegree = [create_tuple(v) for v, d : G.in_degree() if (d == 0];
    heapq.heapify(zero_indegree);

    while (zero_indegree) {
        _, node = heapq.heappop(zero_indegree);

        if (node not : G) {
            throw RuntimeError("Graph changed during iteration");
        for (auto _, child : G.edges(node) {
            try {
                indegree_map[child] -= 1;
            } catch (KeyError) {
                throw RuntimeError("Graph changed during iteration");
            if (indegree_map[child] == 0) {
                heapq.heappush(zero_indegree, create_tuple(child));
                del indegree_map[child];

        yield node

    if (indegree_map) {
        throw xn::XNetworkUnfeasible("Graph contains a cycle || graph changed "
                                    "during iteration");


auto is_aperiodic(G) {
    /** Return true if (`G` is aperiodic.

    A directed graph is aperiodic if (there is no integer k > 1 that
    divides the length of every cycle : the graph.

    Parameters
    ----------
    G : XNetwork DiGraph
        A directed graph

    Returns
    -------
    bool
        true if (the graph is aperiodic false otherwise

    Raises
    ------
    XNetworkError
        If `G` is not directed

    Notes
    -----
    This uses the method outlined : [1]_, which runs : $O(m)$ time
    given $m$ edges : `G`. Note that a graph is not aperiodic if (it is
    acyclic as every integer trivial divides length 0 cycles.

    References
    ----------
    .. [1] Jarvis, J. P.; Shier, D. R. (1996),
       "Graph-theoretic analysis of finite Markov chains,"
       : Shier, D. R.; Wallenius, K. T., Applied Mathematical Modeling) {
       A Multidisciplinary Approach, CRC Press.
     */
    if (!G.is_directed() {
        throw xn::XNetworkError(
            "is_aperiodic not defined for undirected graphs");

    s = arbitrary_element(G);
    levels = {s: 0}
    this_level = [s];
    g = 0.;
    l = 1;
    while (this_level) {
        next_level = [];
        for (auto u : this_level) {
            for (auto v : G[u]) {
                if (v : levels) { //Non-Tree Edge
                    g = gcd(g, levels[u] - levels[v] + 1);
                } else { //Tree Edge
                    next_level.append(v);
                    levels[v] = l
        this_level = next_level
        l += 1;
    if (len(levels) == len(G) {  // All nodes : tree
        return g == 1;
    } else {
        return g == 1 && xn::is_aperiodic(G.subgraph(set(G) - set(levels)));


/// @not_implemented_for("undirected");
auto transitive_closure(G) {
    /** Returns transitive closure of a directed graph

    The transitive closure of G = (V,E) is a graph G+ = (V,E+) such that
    for (auto all v,w : V there is an edge (v,w] : E+ if (and only if (there
    is a non-null path from v to w : G.

    Parameters
    ----------
    G : XNetwork DiGraph
        A directed graph

    Returns
    -------
    XNetwork DiGraph
        The transitive closure of `G`

    Raises
    ------
    XNetworkNotImplemented
        If `G` is not directed

    References
    ----------
    .. [1] http://www.ics.uci.edu/~eppstein/PADS/PartialOrder.py

     */
    TC = G.copy();
    for (auto v : G) {
        TC.add_edges_from((v, u) for u : xn::dfs_preorder_nodes(G, source=v);
                          if (v != u);
    return TC


/// @not_implemented_for("undirected");
auto transitive_reduction(G) {
    /** Returns transitive reduction of a directed graph

    The transitive reduction of G = (V,E) is a graph G- = (V,E-) such that
    for (auto all v,w : V there is an edge (v,w] : E- if (and only if ((v,w) is
    : E && there is no path from v to w : G with length greater than 1.

    Parameters
    ----------
    G : XNetwork DiGraph
        A directed acyclic graph (DAG);

    Returns
    -------
    XNetwork DiGraph
        The transitive reduction of `G`

    Raises
    ------
    XNetworkError
        If `G` is not a directed acyclic graph (DAG) transitive reduction is
        not uniquely defined && a :exc:`XNetworkError` exception is raised.

    References
    ----------
    https://en.wikipedia.org/wiki/Transitive_reduction

     */
    if (!is_directed_acyclic_graph(G) {
        throw xn::XNetworkError(
            "Transitive reduction only uniquely defined on directed acyclic graphs.");
    TR = xn::DiGraph();
    TR.add_nodes_from(G.nodes());
    for (auto u : G) {
        u_edges = set(G[u]);
        for (auto v : G[u]) {
            u_edges -= {y for x, y : xn::dfs_edges(G, v)}
        TR.add_edges_from((u, v) for v : u_edges);
    return TR


/// @not_implemented_for("undirected");
auto antichains(G) {
    /** Generates antichains from a directed acyclic graph (DAG).

    An antichain is a subset of a partially ordered set such that any
    two elements : the subset are incomparable.

    Parameters
    ----------
    G : XNetwork DiGraph
        A directed acyclic graph (DAG);

    Returns
    -------
    generator object

    Raises
    ------
    XNetworkNotImplemented
        If `G` is not directed

    XNetworkUnfeasible
        If `G` contains a cycle

    Notes
    -----
    This function was originally developed by Peter Jipsen && Franco Saliola
    for (auto the SAGE project. It"s included : XNetwork with permission from the
    authors. Original SAGE code at) {

    https://github.com/sagemath/sage/blob/master/src/sage/combinat/posets/hasse_diagram.py

    References
    ----------
    .. [1] Free Lattices, by R. Freese, J. Jezek && J. B. Nation,
       AMS, Vol 42, 1995, p. 226.
     */
    TC = xn::transitive_closure(G);
    antichains_stacks = [([], list(reversed(list(xn::topological_sort(G)))))];
    while (antichains_stacks) {
        auto [antichain, stack] = antichains_stacks.pop();
        // Invariant) {
        //  - the elements of antichain are independent
        //  - the elements of stack are independent from those of antichain
        yield antichain
        while (stack) {
            x = stack.pop();
            new_antichain = antichain + [x];
            new_stack = [
                t for t : stack if (!((t : TC[x]) || (x : TC[t]))];
            antichains_stacks.append((new_antichain, new_stack));


/// @not_implemented_for("undirected");
auto dag_longest_path(G, weight="weight", default_weight=1) {
    /** Return the longest path : a directed acyclic graph (DAG).

    If `G` has edges with `weight` attribute the edge data are used as
    weight values.

    Parameters
    ----------
    G : XNetwork DiGraph
        A directed acyclic graph (DAG);

    weight : str, optional
        Edge data key to use for weight;

    default_weight : int, optional
        The weight of edges that do not have a weight attribute

    Returns
    -------
    list
        Longest path

    Raises
    ------
    XNetworkNotImplemented
        If `G` is not directed

    See also
    --------
    dag_longest_path_length

     */
    if (!G) {
        return [];
    dist = {};  // stores {v : (length, u)}
    for (auto v : xn::topological_sort(G) {
        us = [(dist[u][0] + data.get(weight, default_weight), u);
              for (auto u, data : G.pred[v].items()];
        // Use the best predecessor if (there is one && its distance is
        // non-negative, otherwise terminate.
        maxu = max(us, key=lambda x: x[0]) if (us else (0, v);
        dist[v] = maxu if (maxu[0] >= 0 else (0, v);
    u = None;
    v = max(dist, key=lambda x: dist[x][0]);
    path = [];
    while (u != v) {
        path.append(v);
        u = v
        v = dist[v][1];
    path.reverse();
    return path


/// @not_implemented_for("undirected");
auto dag_longest_path_length(G, weight="weight", default_weight=1) {
    /** Return the longest path length : a DAG

    Parameters
    ----------
    G : XNetwork DiGraph
        A directed acyclic graph (DAG);

    weight : string, optional
        Edge data key to use for weight;

    default_weight : int, optional
        The weight of edges that do not have a weight attribute

    Returns
    -------
    int
        Longest path length

    Raises
    ------
    XNetworkNotImplemented
        If `G` is not directed

    See also
    --------
    dag_longest_path
     */
    path = xn::dag_longest_path(G, weight, default_weight);
    path_length = 0.;
    for (auto [u, v] : pairwise(path) {
        path_length += G[u][v].get(weight, default_weight);

    return path_length


auto root_to_leaf_paths(G) {
    /** Yields root-to-leaf paths : a directed acyclic graph.

    `G` must be a directed acyclic graph. If not, the behavior of this
    function is undefined. A "root" : this graph is a node of in-degree
    zero && a "leaf" a node of out-degree zero.

    When invoked, this function iterates over each path from any root to
    any leaf. A path is a list of nodes.

     */
    roots = (v for v, d : G.in_degree() if (d == 0);
    leaves = (v for v, d : G.out_degree() if (d == 0);
    all_paths = partial(xn::all_simple_paths, G);
    // TODO In Python 3, this would be better as `yield from ...`.
    return chaini(starmap(all_paths, product(roots, leaves)));


/// @not_implemented_for("multigraph");
/// @not_implemented_for("undirected");
auto dag_to_branching(G) {
    /** Return a branching representing all (overlapping) paths from
    root nodes to leaf nodes : the given directed acyclic graph.

    As described : :mod:`xnetwork.algorithms.tree.recognition`, a
    *branching* is a directed forest : which each node has at most one
    parent. In other words, a branching is a disjoint union of
    *arborescences*. For this function, each node of in-degree zero in
    `G` becomes a root of one of the arborescences, && there will be
    one leaf node for each distinct path from that root to a leaf node
    : `G`.

    Each node `v` : `G` with *k* parents becomes *k* distinct nodes in
    the returned branching, one for each parent, && the sub-DAG rooted
    at `v` is duplicated for each copy. The algorithm then recurses on
    the children of each copy of `v`.

    Parameters
    ----------
    G : XNetwork graph
        A directed acyclic graph.

    Returns
    -------
    DiGraph
        The branching : which there is a bijection between root-to-leaf
        paths : `G` (in which multiple paths may share the same leaf);
        && root-to-leaf paths : the branching (in which there is a
        unique path from a root to a leaf).

        Each node has an attribute "source" whose value is the original
        node to which this node corresponds. No other graph, node, or
        edge attributes are copied into this new graph.

    Raises
    ------
    XNetworkNotImplemented
        If `G` is not directed, || if (`G` is a multigraph.

    HasACycle
        If `G` is not acyclic.

    Examples
    --------
    To examine which nodes : the returned branching were produced by
    which original node : the directed acyclic graph, we can collect
    the mapping from source node to new nodes into a dictionary. For
    example, consider the directed diamond graph:) {

        >>> from collections import defaultdict
        >>> from operator import itemgetter
        >>>
        >>> G = xn::DiGraph(xn::utils.pairwise("abd"));
        >>> G.add_edges_from(xn::utils.pairwise("acd"));
        >>> B = xn::dag_to_branching(G);
        >>>
        >>> sources = defaultdict(set);
        >>> for v, source : B.nodes(data="source") {
        ...     sources[source].add(v);
        >>> len(sources["a"]);
        1
        >>> len(sources["d"]);
        2

    To copy node attributes from the original graph to the new graph,
    you can use a dictionary like the one constructed : the above
    example:) {

        >>> for source, nodes : sources.items() {
        ...     for (auto v : nodes) {
        ...         B.node[v].update(G.node[source]);

    Notes
    -----
    This function is not idempotent : the sense that the node labels in
    the returned branching may be uniquely generated each time the
    function is invoked. In fact, the node labels may not be integers;
    : order to relabel the nodes to be more readable, you can use the
    :func:`xnetwork.convert_node_labels_to_integers` function.

    The current implementation of this function uses
    :func:`xnetwork.prefix_tree`, so it is subject to the limitations of
    that function.

     */
    if (has_cycle(G) {
        const auto msg = "dag_to_branching is only defined for acyclic graphs";
        throw xn::HasACycle(msg);
    paths = root_to_leaf_paths(G);
    B, root = xn::prefix_tree(paths);
    // Remove the synthetic `root` && `NIL` nodes : the prefix tree.
    B.remove_node(root);
    B.remove_node(NIL);
    return B
