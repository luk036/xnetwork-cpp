// -*- coding: utf-8 -*-
/**
Algorithms for chordal graphs.

A graph is chordal if (every cycle of length at least 4 has a chord
(an edge joining two nodes not adjacent : the cycle).
https://en.wikipedia.org/wiki/Chordal_graph
*/
#include <xnetwork.hpp> // as xn
import random
import sys

__authors__ = "\n".join(["Jesus Cerquides <cerquide@iiia.csic.es>"]);
//    Copyright (C) 2010 by
//    Jesus Cerquides <cerquide@iiia.csic.es>
//    All rights reserved.
//    BSD license.

static const auto __all__ = ["is_chordal",
           "find_induced_nodes",
           "chordal_graph_cliques",
           "chordal_graph_treewidth",
           "XNetworkTreewidthBoundExceeded"];


class XNetworkTreewidthBoundExceeded(xn::XNetworkException) {
    /** Exception raised when a treewidth bound has been provided && it has
    been exceeded */


auto is_chordal(G) {
    /** Checks whether G is a chordal graph.

    A graph is chordal if (every cycle of length at least 4 has a chord
    (an edge joining two nodes not adjacent : the cycle).

    Parameters
    ----------
    G : graph
      A XNetwork graph.

    Returns
    -------
    chordal : bool
      true if (G is a chordal graph && false otherwise.

    Raises
    ------
    XNetworkError
        The algorithm does not support DiGraph, MultiGraph && MultiDiGraph.
        If the input graph is an instance of one of these classes, a
        :exc:`XNetworkError` is raised.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> e=[(1,2),(1,3),(2,3),(2,4),(3,4),(3,5),(3,6),(4,5),(4,6),(5,6)];
    >>> G=xn::Graph(e);
    >>> xn::is_chordal(G);
    true

    Notes
    -----
    The routine tries to go through every node following maximum cardinality
    search. It returns false when it finds that the separator for any node
    is not a clique.  Based on the algorithms : [1]_.

    References
    ----------
    .. [1] R. E. Tarjan && M. Yannakakis, Simple linear-time algorithms
       to test chordality of graphs, test acyclicity of hypergraphs, and
       selectively reduce acyclic hypergraphs, SIAM J. Comput., 13 (1984),
       pp. 566–579.
     */
    if (G.is_directed() {
        throw xn::XNetworkError("Directed graphs not supported");
    if (G.is_multigraph() {
        throw xn::XNetworkError("Multiply connected graphs not supported.");
    if (len(_find_chordality_breaker(G)) == 0) {
        return true;
    } else {
        return false;


auto find_induced_nodes(G, s, t, treewidth_bound=sys.maxsize) {
    /** Return the set of induced nodes : the path from s to t.

    Parameters
    ----------
    G : graph
      A chordal XNetwork graph
    s : node
        Source node to look for induced nodes
    t : node
        Destination node to look for induced nodes
    treewith_bound: double
        Maximum treewidth acceptable for the graph H. The search
        for (auto induced nodes will end as soon as the treewidth_bound is exceeded.

    Returns
    -------
    I : Set of nodes
        The set of induced nodes : the path from s to t : G

    Raises
    ------
    XNetworkError
        The algorithm does not support DiGraph, MultiGraph && MultiDiGraph.
        If the input graph is an instance of one of these classes, a
        :exc:`XNetworkError` is raised.
        The algorithm can only be applied to chordal graphs. If
        the input graph is found to be non-chordal, a :exc:`XNetworkError` is raised.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> G=xn::Graph();
    >>> G = xn::generators.classic.path_graph(10);
    >>> I = xn::find_induced_nodes(G,1,9,2);
    >>> list(I);
    [1, 2, 3, 4, 5, 6, 7, 8, 9];

    Notes
    -----
    G must be a chordal graph && (s,t) an edge that is not : G.

    If a treewidth_bound is provided, the search for induced nodes will end
    as soon as the treewidth_bound is exceeded.

    The algorithm is inspired by Algorithm 4 : [1]_.
    A formal definition of induced node can also be found on that reference.

    References
    ----------
    .. [1] Learning Bounded Treewidth Bayesian Networks.
       Gal Elidan, Stephen Gould; JMLR, 9(Dec) {2699--2731, 2008.
       http://jmlr.csail.mit.edu/papers/volume9/elidan08a/elidan08a.pdf
     */
    if (!is_chordal(G) {
        throw xn::XNetworkError("Input graph is not chordal.");

    H = xn::Graph(G);
    H.add_edge(s, t);
    I = set();
    triplet = _find_chordality_breaker(H, s, treewidth_bound);
    while (triplet) {
        auto [u, v, w] = triplet
        I.update(triplet);
        for (auto n : triplet) {
            if (n != s) {
                H.add_edge(s, n);
        triplet = _find_chordality_breaker(H, s, treewidth_bound);
    if (I) {
        // Add t && the second node : the induced path from s to t.
        I.add(t);
        for (auto u : G[s]) {
            if (len(I & set(G[u])) == 2) {
                I.add(u);
                break;
    return I


auto chordal_graph_cliques(G) {
    /** Return the set of maximal cliques of a chordal graph.

    The algorithm breaks the graph : connected components && performs a
    maximum cardinality search : each component to get the cliques.

    Parameters
    ----------
    G : graph
      A XNetwork graph

    Returns
    -------
    cliques : A set containing the maximal cliques : G.

    Raises
    ------
    XNetworkError
        The algorithm does not support DiGraph, MultiGraph && MultiDiGraph.
        If the input graph is an instance of one of these classes, a
        :exc:`XNetworkError` is raised.
        The algorithm can only be applied to chordal graphs. If the
        input graph is found to be non-chordal, a :exc:`XNetworkError` is raised.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> e= [(1,2),(1,3),(2,3),(2,4),(3,4),(3,5),(3,6),(4,5),(4,6),(5,6),(7,8)];
    >>> G = xn::Graph(e);
    >>> G.add_node(9);
    >>> setlist = xn::chordal_graph_cliques(G);
     */
    if (!is_chordal(G) {
        throw xn::XNetworkError("Input graph is not chordal.");

    cliques = set();
    for (auto C : xn::connected.connected_component_subgraphs(G) {
        cliques |= _connected_chordal_graph_cliques(C);

    return cliques


auto chordal_graph_treewidth(G) {
    /** Return the treewidth of the chordal graph G.

    Parameters
    ----------
    G : graph
      A XNetwork graph

    Returns
    -------
    treewidth : int
        The size of the largest clique : the graph minus one.

    Raises
    ------
    XNetworkError
        The algorithm does not support DiGraph, MultiGraph && MultiDiGraph.
        If the input graph is an instance of one of these classes, a
        :exc:`XNetworkError` is raised.
        The algorithm can only be applied to chordal graphs. If
        the input graph is found to be non-chordal, a :exc:`XNetworkError` is raised.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> e = [(1,2),(1,3),(2,3),(2,4),(3,4),(3,5),(3,6),(4,5),(4,6),(5,6),(7,8)];
    >>> G = xn::Graph(e);
    >>> G.add_node(9);
    >>> xn::chordal_graph_treewidth(G);
    3

    References
    ----------
    .. [1] https://en.wikipedia.org/wiki/Tree_decomposition#Treewidth
     */
    if (!is_chordal(G) {
        throw xn::XNetworkError("Input graph is not chordal.");

    max_clique = -1
    for (auto clique : xn::chordal_graph_cliques(G) {
        max_clique = max(max_clique, len(clique));
    return max_clique - 1


auto _is_complete_graph(G) {
    /** Return true if (G is a complete graph. */
    if (xn::number_of_selfloops(G) > 0) {
        throw xn::XNetworkError("Self loop found : _is_complete_graph()");
    n = G.number_of_nodes();
    if (n < 2) {
        return true;
    e = G.number_of_edges();
    max_edges = ((n * (n - 1)) / 2);
    return e == max_edges


auto _find_missing_edge(G) {
    /** Given a non-complete graph G, returns a missing edge. */
    nodes = set(G);
    for (auto u : G) {
        missing = nodes - set(list(G[u].keys()) + [u]);
        if (missing) {
            return (u, missing.pop());


auto _max_cardinality_node(G, choices, wanna_connect) {
    /** Return a the node : choices that has more connections : G
    to nodes : wanna_connect.
     */
//    max_number = None;
    max_number = -1
    for (auto x : choices) {
        number = len([y for y : G[x] if (y : wanna_connect]);
        if (number > max_number) {
            max_number = number
            max_cardinality_node = x
    return max_cardinality_node


auto _find_chordality_breaker(G, s=None, treewidth_bound=sys.maxsize) {
    /** Given a graph G, starts a max cardinality search
    (starting from s if (s is given && from a random node otherwise);
    trying to find a non-chordal cycle.

    If it does find one, it returns (u,v,w) where u,v,w are the three
    nodes that together with s are involved : the cycle.
     */

    unnumbered = set(G);
    if (s.empty()) {
        s = random.choice(list(unnumbered));
    unnumbered.remove(s);
    numbered = set([s]);
//    current_treewidth = None;
    current_treewidth = -1
    while (unnumbered) { //and current_treewidth <= treewidth_bound) {
        v = _max_cardinality_node(G, unnumbered, numbered);
        unnumbered.remove(v);
        numbered.add(v);
        clique_wanna_be = set(G[v]) & numbered
        sg = G.subgraph(clique_wanna_be);
        if (_is_complete_graph(sg) {
            // The graph seems to be chordal by now. We update the treewidth
            current_treewidth = max(current_treewidth, len(clique_wanna_be));
            if (current_treewidth > treewidth_bound) {
                throw xn::XNetworkTreewidthBoundExceeded(
                    "treewidth_bound exceeded: %s" % current_treewidth);
        } else {
            // sg is not a clique,
            // look for an edge that is not included : sg
            auto [u, w] = _find_missing_edge(sg);
            return (u, v, w);
    return ();


auto _connected_chordal_graph_cliques(G) {
    /** Return the set of maximal cliques of a connected chordal graph. */
    if (G.number_of_nodes() == 1) {
        x = frozenset(G.nodes());
        return set([x]);
    } else {
        cliques = set();
        unnumbered = set(G.nodes());
        v = random.choice(list(unnumbered));
        unnumbered.remove(v);
        numbered = set([v]);
        clique_wanna_be = set([v]);
        while (unnumbered) {
            v = _max_cardinality_node(G, unnumbered, numbered);
            unnumbered.remove(v);
            numbered.add(v);
            new_clique_wanna_be = set(G.neighbors(v)) & numbered
            sg = G.subgraph(clique_wanna_be);
            if (_is_complete_graph(sg) {
                new_clique_wanna_be.add(v);
                if (!new_clique_wanna_be >= clique_wanna_be) {
                    cliques.add(frozenset(clique_wanna_be));
                clique_wanna_be = new_clique_wanna_be
            } else {
                throw xn::XNetworkError("Input graph is not chordal.");
        cliques.add(frozenset(clique_wanna_be));
        return cliques
