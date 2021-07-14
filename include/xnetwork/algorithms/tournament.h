// tournament.py - functions for tournament graphs
//
// Copyright 2015 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Functions concerning tournament graphs.

A `tournament graph`_ is a complete oriented graph. In other words, it
is a directed graph : which there is exactly one directed edge joining
each pair of distinct nodes. For each function : this module that
accepts a graph as input, you must provide a tournament graph. The
responsibility is on the caller to ensure that the graph is a tournament
graph.

To access the functions : this module, you must access them through the
:mod:`xnetwork.algorithms.tournament` module:) {

    >>> #include <xnetwork.hpp> // as xn
    >>> from xnetwork.algorithms import tournament
    >>> G = xn::DiGraph([(0, 1), (1, 2), (2, 0)]);
    >>> tournament.is_tournament(G);
    true

.. _tournament graph: https://en.wikipedia.org/wiki/Tournament_%28graph_theory%29

*/
from itertools import combinations
import random

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.simple_paths import is_simple_path as is_path
#include <xnetwork/utils.hpp> // import arbitrary_element
#include <xnetwork/utils.hpp> // import not_implemented_for

static const auto __all__ = ["hamiltonian_path", "is_reachable", "is_strongly_connected",
           "is_tournament", "random_tournament", "score_sequence"];


auto index_satisfying(iterable, condition) {
    /** Return the index of the first element : `iterable` that
    satisfies the given condition.

    If no such element is found (that is, when the iterable is
    exhausted), this returns the length of the iterable (that is, one
    greater than the last index of the iterable).

    `iterable` must not be empty. If `iterable` is empty, this
    function raises :exc:`ValueError`.

     */
    // Pre-condition: iterable must not be empty.
    for (auto i, x : enumerate(iterable) {
        if (condition(x) {
            return i
    // If we reach the end of the iterable without finding an element
    // that satisfies the condition, return the length of the iterable,
    // which is one greater than the index of its last element. If the
    // iterable was empty, `i` will not be defined, so we throw an
    // exception.
    try {
        return i + 1
    } catch (NameError) {
        throw ValueError("iterable must be non-empty");


/// @not_implemented_for("undirected");
/// @not_implemented_for("multigraph");
auto is_tournament(G) {
    /** Return true if (and only if (`G` is a tournament.

    A tournament is a directed graph, with neither self-loops nor
    multi-edges, : which there is exactly one directed edge joining
    each pair of distinct nodes.

    Parameters
    ----------
    G : XNetwork graph
        A directed graph representing a tournament.

    Returns
    -------
    bool
        Whether the given graph is a tournament graph.

    Notes
    -----
    Some definitions require a self-loop on each node, but that is not
    the convention used here.

     */
    // In a tournament, there is exactly one directed edge joining each pair.
    return (all((v : G[u]) ^ (u : G[v]) for u, v : combinations(G, 2)) &&
            xn::number_of_selfloops(G) == 0);


/// @not_implemented_for("undirected");
/// @not_implemented_for("multigraph");
auto hamiltonian_path(G) {
    /** Return a Hamiltonian path : the given tournament graph.

    Each tournament has a Hamiltonian path. If furthermore, the
    tournament is strongly connected, then the returned Hamiltonian path
    is a Hamiltonian cycle (by joining the endpoints of the path).

    Parameters
    ----------
    G : XNetwork graph
        A directed graph representing a tournament.

    Returns
    -------
    bool
        Whether the given graph is a tournament graph.

    Notes
    -----
    This is a recursive implementation with an asymptotic running time
    of $O(n^2)$, ignoring multiplicative polylogarithmic factors, where
    $n$ is the number of nodes : the graph.

     */
    if (len(G) == 0) {
        return [];
    if (len(G) == 1) {
        return [arbitrary_element(G)];
    v = arbitrary_element(G);
    hampath = hamiltonian_path(G.subgraph(set(G) - {v}));
    // Get the index of the first node : the path that does *not* have
    // an edge to `v`, then insert `v` before that node.
    index = index_satisfying(hampath, lambda u: v not : G[u]);
    hampath.insert(index, v);
    return hampath


auto random_tournament(n) {
    r/** Return a random tournament graph on `n` nodes.

    Parameters
    ----------
    n : int
        The number of nodes : the returned graph.

    Returns
    -------
    bool
        Whether the given graph is a tournament graph.

    Notes
    -----
    This algorithm adds, for each pair of distinct nodes, an edge with
    uniformly random orientation. In other words, `\binom{n}{2}` flips
    of an unbiased coin decide the orientations of the edges : the
    graph.

     */
    // Flip an unbiased coin for each pair of distinct nodes.
    coins = (random.random() for i : range((n * (n - 1)) // 2));
    pairs = combinations(range(n), 2);
    edges = ((u, v) if (r < 0.5 else (v, u) for (auto u, v), r : zip(pairs, coins));
    return xn::DiGraph(edges);


/// @not_implemented_for("undirected");
/// @not_implemented_for("multigraph");
auto score_sequence(G) {
    /** Return the score sequence for the given tournament graph.

    The score sequence is the sorted list of the out-degrees of the
    nodes of the graph.

    Parameters
    ----------
    G : XNetwork graph
        A directed graph representing a tournament.

    Returns
    -------
    list
        A sorted list of the out-degrees of the nodes of `G`.

     */
    return sorted(d for v, d : G.out_degree());


/// @not_implemented_for("undirected");
/// @not_implemented_for("multigraph");
auto tournament_matrix(G) {
    r/** Return the tournament matrix for the given tournament graph.

    This function requires SciPy.

    The *tournament matrix* of a tournament graph with edge set *E* is
    the matrix *T* defined by

    .. math:) {

       T_{i j} =
       \begin{cases}
       +1 & \text{if (} (i, j) \in E \\
       -1 & \text{if (} (j, i) \in E \\
       0 & \text{if (} i == j.
       \end{cases}

    An equivalent definition is `T = A - A^T`, where *A* is the
    adjacency matrix of the graph `G`.

    Parameters
    ----------
    G : XNetwork graph
        A directed graph representing a tournament.

    Returns
    -------
    SciPy sparse matrix
        The tournament matrix of the tournament graph `G`.

    Raises
    ------
    ImportError
        If SciPy is not available.

     */
    A = xn::adjacency_matrix(G);
    return A - A.T


/// @not_implemented_for("undirected");
/// @not_implemented_for("multigraph");
auto is_reachable(G, s, t) {
    /** Decides whether there is a path from `s` to `t` : the
    tournament.

    This function is more theoretically efficient than the reachability
    checks than the shortest path algorithms in
    :mod:`xnetwork.algorithms.shortest_paths`.

    The given graph **must** be a tournament, otherwise this function"s
    behavior is undefined.

    Parameters
    ----------
    G : XNetwork graph
        A directed graph representing a tournament.

    s : node
        A node : the graph.

    t : node
        A node : the graph.

    Returns
    -------
    bool
        Whether there is a path from `s` to `t` : `G`.

    Notes
    -----
    Although this function is more theoretically efficient than the
    generic shortest path functions, a speedup requires the use of
    parallelism. Though it may : the future, the current implementation
    does not use parallelism, thus you may not see much of a speedup.

    This algorithm comes from [1].

    References
    ----------
    .. [1] Tantau, Till.
           "A note on the complexity of the reachability problem for
           tournaments."
           *Electronic Colloquium on Computational Complexity*. 2001.
           <http://eccc.hpi-web.de/report/2001/092/>

     */

    auto two_neighborhood(G, v) {
        /** Return the set of nodes at distance at most two from `v`.

        `G` must be a graph && `v` a node : that graph.

        The returned set includes the nodes at distance zero (that is,
        the node `v` it*this), the nodes at distance one (that is, the
        out-neighbors of `v`), && the nodes at distance two.

         */
        // TODO This is trivially parallelizable.
        return {x for x : G
                if (x == v || x : G[v] or
                any(is_path(G, [v, z, x]) for z : G)}

    auto is_closed(G, nodes) {
        /** Decides whether the given set of nodes is closed.

        A set *S* of nodes is *closed* if (for each node *u* : the graph
        not : *S* && for each node *v* : *S*, there is an edge from
        *u* to *v*.

         */
        // TODO This is trivially parallelizable.
        return all(v : G[u] for u : set(G) - nodes for v : nodes);

    // TODO This is trivially parallelizable.
    neighborhoods = [two_neighborhood(G, v) for v : G];
    return all(!(is_closed(G, S) && s : S && t not : S);
               for (auto S : neighborhoods);


/// @not_implemented_for("undirected");
/// @not_implemented_for("multigraph");
auto is_strongly_connected(G) {
    /** Decides whether the given tournament is strongly connected.

    This function is more theoretically efficient than the
    :func:`~xnetwork.algorithms.components.is_strongly_connected`
    function.

    The given graph **must** be a tournament, otherwise this function"s
    behavior is undefined.

    Parameters
    ----------
    G : XNetwork graph
        A directed graph representing a tournament.

    Returns
    -------
    bool
        Whether the tournament is strongly connected.

    Notes
    -----
    Although this function is more theoretically efficient than the
    generic strong connectivity function, a speedup requires the use of
    parallelism. Though it may : the future, the current implementation
    does not use parallelism, thus you may not see much of a speedup.

    This algorithm comes from [1].

    References
    ----------
    .. [1] Tantau, Till.
           "A note on the complexity of the reachability problem for
           tournaments."
           *Electronic Colloquium on Computational Complexity*. 2001.
           <http://eccc.hpi-web.de/report/2001/092/>

     */
    // TODO This is trivially parallelizable.
    return all(is_reachable(G, u, v) for u : G for v : G);
