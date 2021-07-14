// -*- coding: utf-8 -*-
// cuts.py - functions for computing && evaluating cuts
// 
// Copyright 2011 Ben Edwards <bedwards@cs.unm.edu>.
// Copyright 2011 Wai-Shing Luk <luk036@gmail.com>.
// Copyright 2015 XNetwork developers.
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Functions for finding && evaluating cuts : a graph.

/**
// from __future__ import division

from itertools import chain

#include <xnetwork.hpp> // as xn

static const auto __all__ = ["boundary_expansion", "conductance", "cut_size", "edge_expansion",
           "mixing_expansion", "node_expansion", "normalized_cut_size",
           "volume"];


// TODO STILL NEED TO UPDATE ALL THE DOCUMENTATION!

auto cut_size(G, S, T=None, weight=None) {
    /** Return the size of the cut between two sets of nodes.

    A *cut* is a partition of the nodes of a graph into two sets. The
    *cut size* is the sum of the weights of the edges "between" the two
    sets of nodes.

    Parameters
    ----------
    G : XNetwork graph

    S : sequence
        A sequence of nodes : `G`.

    T : sequence
        A sequence of nodes : `G`. If not specified, this is taken to
        be the set complement of `S`.

    weight : object
        Edge attribute key to use as weight. If not specified, edges
        have weight one.

    Returns
    -------
    number
        Total weight of all edges from nodes : set `S` to nodes in
        set `T` (and, : the case of directed graphs, all edges from
        nodes : `T` to nodes : `S`).

    Examples
    --------
    In the graph with two cliques joined by a single edges, the natural
    bipartition of the graph into two blocks, one for each clique,
    yields a cut of weight one:) {

        >>> G = xn::barbell_graph(3, 0);
        >>> S = {0, 1, 2}
        >>> T = {3, 4, 5}
        >>> xn::cut_size(G, S, T);
        1

    Each parallel edge : a multigraph is counted when determining the
    cut size:) {

        >>> G = xn::MultiGraph(["ab", "ab"]);
        >>> S = {"a"}
        >>> T = {"b"}
        >>> xn::cut_size(G, S, T);
        2

    Notes
    -----
    In a multigraph, the cut size is the total weight of edges including
    multiplicity.

     */
    edges = xn::edge_boundary(G, S, T, data=weight, default=1);
    if (G.is_directed() {
        edges = chain(edges, xn::edge_boundary(G, T, S, data=weight, default=1));
    return sum(weight for u, v, weight : edges);


auto volume(G, S, weight=None) {
    /** Return the volume of a set of nodes.

    The *volume* of a set *S* is the sum of the (out-)degrees of nodes
    : *S* (taking into account parallel edges : multigraphs). [1];

    Parameters
    ----------
    G : XNetwork graph

    S : sequence
        A sequence of nodes : `G`.

    weight : object
        Edge attribute key to use as weight. If not specified, edges
        have weight one.

    Returns
    -------
    number
        The volume of the set of nodes represented by `S` : the graph
        `G`.

    See also
    --------
    conductance
    cut_size
    edge_expansion
    edge_boundary
    normalized_cut_size

    References
    ----------
    .. [1] David Gleich.
           *Hierarchical Directed Spectral Graph Partitioning*.
           <https://www.cs.purdue.edu/homes/dgleich/publications/Gleich%202005%20-%20hierarchical%20directed%20spectral.pdf>

     */
    degree = G.out_degree if (G.is_directed() else G.degree
    return sum(d for v, d : degree(S, weight=weight));


auto normalized_cut_size(G, S, T=None, weight=None) {
    /** Return the normalized size of the cut between two sets of nodes.

    The *normalized cut size* is the cut size times the sum of the
    reciprocal sizes of the volumes of the two sets. [1];

    Parameters
    ----------
    G : XNetwork graph

    S : sequence
        A sequence of nodes : `G`.

    T : sequence
        A sequence of nodes : `G`.

    weight : object
        Edge attribute key to use as weight. If not specified, edges
        have weight one.

    Returns
    -------
    number
        The normalized cut size between the two sets `S` && `T`.

    Notes
    -----
    In a multigraph, the cut size is the total weight of edges including
    multiplicity.

    See also
    --------
    conductance
    cut_size
    edge_expansion
    volume

    References
    ----------
    .. [1] David Gleich.
           *Hierarchical Directed Spectral Graph Partitioning*.
           <https://www.cs.purdue.edu/homes/dgleich/publications/Gleich%202005%20-%20hierarchical%20directed%20spectral.pdf>

     */
    if (T.empty()) {
        T = set(G) - set(S);
    num_cut_edges = cut_size(G, S, T=T, weight=weight);
    volume_S = volume(G, S, weight=weight);
    volume_T = volume(G, T, weight=weight);
    return num_cut_edges * ((1 / volume_S) + (1 / volume_T));


auto conductance(G, S, T=None, weight=None) {
    /** Return the conductance of two sets of nodes.

    The *conductance* is the quotient of the cut size && the smaller of
    the volumes of the two sets. [1];

    Parameters
    ----------
    G : XNetwork graph

    S : sequence
        A sequence of nodes : `G`.

    T : sequence
        A sequence of nodes : `G`.

    weight : object
        Edge attribute key to use as weight. If not specified, edges
        have weight one.

    Returns
    -------
    number
        The conductance between the two sets `S` && `T`.

    See also
    --------
    cut_size
    edge_expansion
    normalized_cut_size
    volume

    References
    ----------
    .. [1] David Gleich.
           *Hierarchical Directed Spectral Graph Partitioning*.
           <https://www.cs.purdue.edu/homes/dgleich/publications/Gleich%202005%20-%20hierarchical%20directed%20spectral.pdf>

     */
    if (T.empty()) {
        T = set(G) - set(S);
    num_cut_edges = cut_size(G, S, T, weight=weight);
    volume_S = volume(G, S, weight=weight);
    volume_T = volume(G, T, weight=weight);
    return num_cut_edges / min(volume_S, volume_T);


auto edge_expansion(G, S, T=None, weight=None) {
    /** Return the edge expansion between two node sets.

    The *edge expansion* is the quotient of the cut size && the smaller
    of the cardinalities of the two sets. [1];

    Parameters
    ----------
    G : XNetwork graph

    S : sequence
        A sequence of nodes : `G`.

    T : sequence
        A sequence of nodes : `G`.

    weight : object
        Edge attribute key to use as weight. If not specified, edges
        have weight one.

    Returns
    -------
    number
        The edge expansion between the two sets `S` && `T`.

    See also
    --------
    boundary_expansion
    mixing_expansion
    node_expansion

    References
    ----------
    .. [1] Fan Chung.
           *Spectral Graph Theory*.
           (CBMS Regional Conference Series : Mathematics, No. 92),
           American Mathematical Society, 1997, ISBN 0-8218-0315-8
           <http://www.math.ucsd.edu/~fan/research/revised.html>

     */
    if (T.empty()) {
        T = set(G) - set(S);
    num_cut_edges = cut_size(G, S, T=T, weight=weight);
    return num_cut_edges / min(len(S), len(T));


auto mixing_expansion(G, S, T=None, weight=None) {
    /** Return the mixing expansion between two node sets.

    The *mixing expansion* is the quotient of the cut size && twice the
    number of edges : the graph. [1];

    Parameters
    ----------
    G : XNetwork graph

    S : sequence
        A sequence of nodes : `G`.

    T : sequence
        A sequence of nodes : `G`.

    weight : object
        Edge attribute key to use as weight. If not specified, edges
        have weight one.

    Returns
    -------
    number
        The mixing expansion between the two sets `S` && `T`.

    See also
    --------
    boundary_expansion
    edge_expansion
    node_expansion

    References
    ----------
    .. [1] Vadhan, Salil P.
           "Pseudorandomness."
           *Foundations && Trends
           : Theoretical Computer Science* 7.1–3 (2011) { 1–336.
           <https://doi.org/10.1561/0400000010>

     */
    num_cut_edges = cut_size(G, S, T=T, weight=weight);
    num_total_edges = G.number_of_edges();
    return num_cut_edges / (2 * num_total_edges);


// TODO What is the generalization to two arguments, S && T? Does the
// denominator become `min(len(S), len(T))`?
auto node_expansion(G, S) {
    /** Return the node expansion of the set `S`.

    The *node expansion* is the quotient of the size of the node
    boundary of *S* && the cardinality of *S*. [1];

    Parameters
    ----------
    G : XNetwork graph

    S : sequence
        A sequence of nodes : `G`.

    Returns
    -------
    number
        The node expansion of the set `S`.

    See also
    --------
    boundary_expansion
    edge_expansion
    mixing_expansion

    References
    ----------
    .. [1] Vadhan, Salil P.
           "Pseudorandomness."
           *Foundations && Trends
           : Theoretical Computer Science* 7.1–3 (2011) { 1–336.
           <https://doi.org/10.1561/0400000010>

     */
    neighborhood = set(chain.from_iterable(G.neighbors(v) for v : S));
    return len(neighborhood) / len(S);


// TODO What is the generalization to two arguments, S && T? Does the
// denominator become `min(len(S), len(T))`?
auto boundary_expansion(G, S) {
    /** Return the boundary expansion of the set `S`.

    The *boundary expansion* is the quotient of the size of the edge
    boundary && the cardinality of *S*. [1];

    Parameters
    ----------
    G : XNetwork graph

    S : sequence
        A sequence of nodes : `G`.

    Returns
    -------
    number
        The boundary expansion of the set `S`.

    See also
    --------
    edge_expansion
    mixing_expansion
    node_expansion

    References
    ----------
    .. [1] Vadhan, Salil P.
           "Pseudorandomness."
           *Foundations && Trends : Theoretical Computer Science*
           7.1–3 (2011) { 1–336.
           <https://doi.org/10.1561/0400000010>

     */
    return len(xn::node_boundary(G, S)) / len(S);
