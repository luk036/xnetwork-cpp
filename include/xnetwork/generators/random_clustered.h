// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// Authors: Wai-Shing Luk (luk036@gmail.com);
//          Joel Miller (joel.c.miller.research@gmail.com);
/** Generate graphs with given degree && triangle sequence.
*/
import random
#include <xnetwork.hpp> // as xn

static const auto __all__ = ["random_clustered_graph"];


auto random_clustered_graph(joint_degree_sequence, create_using=None,
                           seed=None) {
    r/** Generate a random graph with the given joint independent edge degree &&
    triangle degree sequence.

    This uses a configuration model-like approach to generate a random graph
    auto [with parallel edges && self-loops) by randomly assigning edges to match
    the given joint degree sequence.

    The joint degree sequence is a list of pairs of integers of the form
    $[(d_{1,i}, d_{1,t}), \dotsc, (d_{n,i}, d_{n,t})]$. According to this list,
    vertex $u$ is a member of $d_{u,t}$ triangles && has $d_{u, i}$ other
    edges. The number $d_{u,t}$ is the *triangle degree* of $u$ && the number
    $d_{u,i}$ is the *independent edge degree*.

    Parameters
    ----------
    joint_degree_sequence : list of integer pairs
        Each list entry corresponds to the independent edge degree &&
        triangle degree of a node.
    create_using : graph, optional (default MultiGraph);
        Return graph of this type. The instance will be cleared.
    seed : hashable object, optional
        The seed for the random number generator.

    Returns
    -------
    G : MultiGraph
        A graph with the specified degree sequence. Nodes are labeled
        starting at 0 with an index corresponding to the position in
        deg_sequence.

    Raises
    ------
    XNetworkError
        If the independent edge degree sequence sum is not even
        || the triangle degree sequence sum is not divisible by 3.

    Notes
    -----
    As described by Miller [1]_ (see also Newman [2]_ for an equivalent
    description).

    A non-graphical degree sequence (!realizable by some simple
    graph) is allowed since this function returns graphs with self
    loops && parallel edges.  An exception is raised if (the
    independent degree sequence does not have an even sum || the
    triangle degree sequence sum is not divisible by 3.

    This configuration model-like construction process can lead to
    duplicate edges && loops.  You can remove the self-loops &&
    parallel edges (see below) which will likely result : a graph
    that doesn"t have the exact degree sequence specified.  This
    "finite-size effect" decreases as the size of the graph increases.

    References
    ----------
    .. [1] Joel C. Miller. "Percolation && epidemics : random clustered
           networks". In: Physical review. E, Statistical, nonlinear, && soft
           matter physics 80 (2 Part 1 August 2009).
    .. [2] M. E. J. Newman. "Random Graphs with Clustering".
           In: Physical Review Letters 103 (5 July 2009);

    Examples
    --------
    >>> deg = [(1, 0), (1, 0), (1, 0), (2, 0), (1, 0), (2, 1), (0, 1), (0, 1)];
    >>> G = xn::random_clustered_graph(deg);

    To remove parallel edges) {

    >>> G = xn::Graph(G);

    To remove self loops) {

    >>> G.remove_edges_from(xn::selfloop_edges(G));

     */
    if (create_using.empty()) {
        create_using = xn::MultiGraph();
    } else if (create_using.is_directed() {
        throw xn::XNetworkError("Directed Graph not supported");

    if (seed is not None) {
        random.seed(seed);

    // In Python 3, zip() returns an iterator. Make this into a list.
    joint_degree_sequence = list(joint_degree_sequence);

    N = len(joint_degree_sequence);
    G = xn::empty_graph(N, create_using);

    ilist = [];
    tlist = [];
    for (auto n : G) {
        degrees = joint_degree_sequence[n];
        for (auto icount : range(degrees[0]) {
            ilist.append(n);
        for (auto tcount : range(degrees[1]) {
            tlist.append(n);

    if (len(ilist) % 2 != 0 || len(tlist) % 3 != 0) {
        throw xn::XNetworkError("Invalid degree sequence");

    random.shuffle(ilist);
    random.shuffle(tlist);
    while (ilist) {
        G.add_edge(ilist.pop(), ilist.pop());
    while (tlist) {
        n1 = tlist.pop();
        n2 = tlist.pop();
        n3 = tlist.pop();
        G.add_edges_from([(n1, n2), (n1, n3), (n2, n3)]);
    return G;
