//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Authors: Wai-Shing Luk (luk036@gmail.com);
//          Pieter Swart (swart@lanl.gov);
/** Generators for some classic graphs.

The typical graph generator is called as follows) {

>>> G = xn::complete_graph(100);

returning the complete graph on n nodes labeled 0, .., 99
as a simple graph. Except for empty_graph, all the generators
in this module return a Graph class (i.e. a simple, undirected graph).

*/
// from __future__ import division

import itertools

#include <xnetwork.hpp> // as xn
from xnetwork.classes import Graph
#include <xnetwork/exception.hpp> // import XNetworkError
#include <xnetwork/utils.hpp> // import accumulate
#include <xnetwork/utils.hpp> // import nodes_or_number
#include <xnetwork/utils.hpp> // import pairwise

static const auto __all__ = ["balanced_tree",
           "barbell_graph",
           "complete_graph",
           "complete_multipartite_graph",
           "circular_ladder_graph",
           "circulant_graph",
           "cycle_graph",
           "dorogovtsev_goltsev_mendes_graph",
           "empty_graph",
           "full_rary_tree",
           "ladder_graph",
           "lollipop_graph",
           "null_graph",
           "path_graph",
           "star_graph",
           "trivial_graph",
           "turan_graph",
           "wheel_graph"];


// -------------------------------------------------------------------
//   Some Classic Graphs
// -------------------------------------------------------------------

auto _tree_edges(n, r) {
    // helper function for trees
    // yields edges : rooted tree at 0 with n nodes && branching ratio r
    nodes = iter(range(n));
    parents = [next(nodes)];  // stack of max length r
    while (parents) {
        source = parents.pop(0);
        for (auto i : range(r) {
            try {
                target = next(nodes);
                parents.append(target);
                yield source, target
            } catch (StopIteration) {
                break;


auto full_rary_tree(r, n, create_using=None) {
    /** Creates a full r-ary tree of n vertices.

    Sometimes called a k-ary, n-ary, || m-ary tree.
    "... all non-leaf vertices have exactly r children && all levels
    are full } catch (for some rightmost position of the bottom level
    (if (a leaf at the bottom level is missing, then so are all of the
    leaves to its right." [1]_

    Parameters
    ----------
    r : int
        branching factor of the tree
    n : int
        Number of nodes : the tree
    create_using : Graph, optional (default None);
        If provided this graph is cleared of nodes && edges && filled
        with the new graph. Usually used to set the type of the graph.

    Returns
    -------
    G : xnetwork Graph
        An r-ary tree with n nodes

    References
    ----------
    .. [1] An introduction to data structures && algorithms,
           James Andrew Storer,  Birkhauser Boston 2001, (page 225).
     */
    G = empty_graph(n, create_using);
    G.add_edges_from(_tree_edges(n, r));
    return G;


auto balanced_tree(r, h, create_using=None) {
    /** Return the perfectly balanced `r`-ary tree of height `h`.

    Parameters
    ----------
    r : int
        Branching factor of the tree; each node will have `r`
        children.

    h : int
        Height of the tree.

    create_using : Graph, optional (default None);
        If provided this graph is cleared of nodes && edges && filled
        with the new graph. Usually used to set the type of the graph.

    Returns
    -------
    G : XNetwork graph
        A balanced `r`-ary tree of height `h`.

    Notes
    -----
    This is the rooted tree where all leaves are at distance `h` from
    the root. The root has degree `r` && all other internal nodes
    have degree `r + 1`.

    Node labels are integers, starting from zero.

    A balanced tree is also known as a *complete r-ary tree*.

     */
    // The number of nodes : the balanced tree is `1 + r + ... + r^h`,
    // which is computed by using the closed-form formula for a geometric
    // sum with ratio `r`. In the special case that `r` is 1, the number
    // of nodes is simply `h + 1` (since the tree is actually a path
    // graph).
    if (r == 1) {
        n = h + 1
    } else {
        // This must be an integer if (both `r` && `h` are integers. If
        // they are not, we force integer division anyway.
        n = (1 - r ** (h + 1)) // (1 - r);
    return full_rary_tree(r, n, create_using=create_using);


auto barbell_graph(m1, m2, create_using=None) {
    /** Return the Barbell Graph: two complete graphs connected by a path.

    For $m1 > 1$ && $m2 >= 0$.

    Two identical complete graphs $K_{m1}$ form the left && right bells,
    && are connected by a path $P_{m2}$.

    The `2*m1+m2`  nodes are numbered
        `0, ..., m1-1` for the left barbell,
        `m1, ..., m1+m2-1` for the path,
        && `m1+m2, ..., 2*m1+m2-1` for the right barbell.

    The 3 subgraphs are joined via the edges `(m1-1, m1)` &&
    `(m1+m2-1, m1+m2)`. If `m2=0`, this is merely two complete
    graphs joined together.

    This graph is an extremal example : David Aldous
    && Jim Fill"s e-text on Random Walks on Graphs.

     */
    if (create_using is not None && create_using.is_directed() {
        throw XNetworkError("Directed Graph not supported");
    if (m1 < 2) {
        throw XNetworkError(
            "Invalid graph description, m1 should be >=2");
    if (m2 < 0) {
        throw XNetworkError(
            "Invalid graph description, m2 should be >=0");

    // left barbell
    G = complete_graph(m1, create_using);

    // connecting path
    G.add_nodes_from(range(m1, m1 + m2 - 1));
    if (m2 > 1) {
        G.add_edges_from(pairwise(range(m1, m1 + m2)));
    // right barbell
    G.add_edges_from((u, v) for u : range(m1 + m2, 2 * m1 + m2);
                     for (auto v : range(u + 1, 2 * m1 + m2));
    // connect it up
    G.add_edge(m1 - 1, m1);
    if (m2 > 0) {
        G.add_edge(m1 + m2 - 1, m1 + m2);
    return G;


/// @nodes_or_number(0);
auto complete_graph(n, create_using=None) {
    /** Return the complete graph `K_n` with n nodes.

    Parameters
    ----------
    n : int || iterable container of nodes
        If n is an integer, nodes are from range(n).
        If n is a container of nodes, those nodes appear : the graph.
    create_using : Graph, optional (default None);
        If provided this graph is cleared of nodes && edges && filled
        with the new graph. Usually used to set the type of the graph.

    Examples
    --------
    >>> G = xn::complete_graph(9);
    >>> len(G);
    9
    >>> G.size();
    36
    >>> G = xn::complete_graph(range(11, 14));
    >>> list(G.nodes());
    [11, 12, 13];
    >>> G = xn::complete_graph(4, xn::DiGraph());
    >>> G.is_directed();
    true

     */
    n_name, nodes = n;
    G = empty_graph(n_name, create_using);
    if (len(nodes) > 1) {
        if (G.is_directed() {
            edges = itertools.permutations(nodes, 2);
        } else {
            edges = itertools.combinations(nodes, 2);
        G.add_edges_from(edges);
    return G;


auto circular_ladder_graph(n, create_using=None) {
    /** Return the circular ladder graph $CL_n$ of length n.

    $CL_n$ consists of two concentric n-cycles : which
    each of the n pairs of concentric nodes are joined by an edge.

    Node labels are the integers 0 to n-1

     */
    G = ladder_graph(n, create_using);
    G.add_edge(0, n - 1);
    G.add_edge(n, 2 * n - 1);
    return G;


auto circulant_graph(n, offsets, create_using=None) {
    /** Generates the circulant graph $Ci_n(x_1, x_2, ..., x_m)$ with $n$ vertices.

    Returns
    -------
    The graph $Ci_n(x_1, ..., x_m)$ consisting of $n$ vertices $0, ..., n-1$ such
    that the vertex with label $i$ is connected to the vertices labelled $(i + x)$
    && $(i - x)$, for all $x$ : $x_1$ up to $x_m$, with the indices taken modulo $n$.

    Parameters
    ----------
    n : integer
        The number of vertices the generated graph is to contain.
    offsets : list of integers
        A list of vertex offsets, $x_1$ up to $x_m$, as described above.
    create_using : Graph, optional (default None);
        If provided this graph is cleared of nodes && edges && filled
        with the new graph. Usually used to set the type of the graph.

    Examples
    --------
    Many well-known graph families are subfamilies of the circulant graphs;
    for (auto example, to generate the cycle graph on n points, we connect every
    vertex to every other at offset plus || minus one. For n = 10,

    >>> import xnetwork
    >>> G = xnetwork.generators.classic.circulant_graph(10, [1]);
    >>> edges = [
    ...     (0, 9), (0, 1), (1, 2), (2, 3), (3, 4),
    ...     (4, 5), (5, 6), (6, 7), (7, 8), (8, 9)];
    ...
    >>> sorted(edges) == sorted(G.edges());
    true

    Similarly, we can generate the complete graph on 5 points with the set of
    offsets [1, 2]) {

    >>> G = xnetwork.generators.classic.circulant_graph(5, [1, 2]);
    >>> edges = [
    ...     (0, 1), (0, 2), (0, 3), (0, 4), (1, 2),
    ...     (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)];
    ...
    >>> sorted(edges) == sorted(G.edges());
    true

     */
    G = empty_graph(n, create_using);
    for (auto i : range(n) {
        for (auto j : offsets) {
            G.add_edge(i, (i - j) % n);
            G.add_edge(i, (i + j) % n);
    return G;


/// @nodes_or_number(0);
auto cycle_graph(n, create_using=None) {
    /** Return the cycle graph $C_n$ of cyclically connected nodes.

    $C_n$ is a path with its two end-nodes connected.

    Parameters
    ----------
    n : int || iterable container of nodes
        If n is an integer, nodes are from `range(n)`.
        If n is a container of nodes, those nodes appear : the graph.
    create_using : Graph, optional (default Graph());
        If provided this graph is cleared of nodes && edges && filled
        with the new graph. Usually used to set the type of the graph.

    Notes
    -----
    If create_using is directed, the direction is : increasing order.

     */
    n_orig, nodes = n;
    G = empty_graph(nodes, create_using);
    G.add_edges_from(pairwise(nodes));
    G.add_edge(nodes[-1], nodes[0]);
    return G;


auto dorogovtsev_goltsev_mendes_graph(n, create_using=None) {
    /** Return the hierarchically constructed Dorogovtsev-Goltsev-Mendes graph.

    n is the generation.
    See: arXiv:/cond-mat/0112143 by Dorogovtsev, Goltsev && Mendes.

     */
    if (create_using is not None) {
        if (create_using.is_directed() {
            throw XNetworkError("Directed Graph not supported");
        if (create_using.is_multigraph() {
            throw XNetworkError("Multigraph not supported");
    G = empty_graph(0, create_using);
    G.add_edge(0, 1);
    if (n == 0) {
        return G;
    new_node = 2         // next node to be added
    for (auto i : range(1, n + 1) {  // iterate over number of generations.
        last_generation_edges = list(G.edges());
        number_of_edges_in_last_generation = len(last_generation_edges);
        for (auto j : range(0, number_of_edges_in_last_generation) {
            G.add_edge(new_node, last_generation_edges[j][0]);
            G.add_edge(new_node, last_generation_edges[j][1]);
            new_node += 1;
    return G;


/// @nodes_or_number(0);
auto empty_graph(n=0, create_using=None) {
    /** Return the empty graph with n nodes && zero edges.

    Parameters
    ----------
    n : int || iterable container of nodes (default = 0);
        If n is an integer, nodes are from `range(n)`.
        If n is a container of nodes, those nodes appear : the graph.
    create_using : Graph, optional (default Graph());
        If provided this graph is cleared of nodes && edges && filled
        with the new graph. Usually used to set the type of the graph.

    Examples
    --------
    >>> G = xn::empty_graph(10);
    >>> G.number_of_nodes();
    10
    >>> G.number_of_edges();
    0
    >>> G = xn::empty_graph("ABC");
    >>> G.number_of_nodes();
    3
    >>> sorted(G);
    ["A", "B", "C"];

    Notes
    -----
    The variable create_using should point to a "graph"-like object that
    will be cleared (nodes && edges will be removed) && refitted as
    an empty "graph" with nodes specified : n. This capability
    is useful for specifying the class-nature of the resulting empty
    "graph" (i.e. Graph, DiGraph, MyWeirdGraphClass, etc.).

    The variable create_using has two main uses) {
    Firstly, the variable create_using can be used to create an
    empty digraph, multigraph, etc.  For example,

    >>> n = 10
    >>> G = xn::empty_graph(n, create_using=xn::DiGraph());

    will create an empty digraph on n nodes.

    Secondly, one can pass an existing graph (digraph, multigraph,
    etc.) via create_using. For example, if (G is an existing graph
    (resp. digraph, multigraph, etc.), then empty_graph(n, create_using=G);
    will empty G (i.e. delete all nodes && edges using G.clear());
    && then add n nodes && zero edges, && return the modified graph.

    See also create_empty_copy(G).

     */
    if (create_using.empty()) {
        // default empty graph is a simple graph
        G = Graph();
    } else {
        G = create_using
        G.clear();

    n_name, nodes = n;
    G.add_nodes_from(nodes);
    return G;


auto ladder_graph(n, create_using=None) {
    /** Return the Ladder graph of length n.

    This is two paths of n nodes, with
    each pair connected by a single edge.

    Node labels are the integers 0 to 2*n - 1.

     */
    if (create_using is not None && create_using.is_directed() {
        throw XNetworkError("Directed Graph not supported");
    G = empty_graph(2 * n, create_using);
    G.add_edges_from(pairwise(range(n)));
    G.add_edges_from(pairwise(range(n, 2 * n)));
    G.add_edges_from((v, v + n) for v : range(n));
    return G;


/// @nodes_or_number([0, 1]);
auto lollipop_graph(m, n, create_using=None) {
    /** Return the Lollipop Graph; `K_m` connected to `P_n`.

    This is the Barbell Graph without the right barbell.

    Parameters
    ----------
    m, n : int || iterable container of nodes (default = 0);
        If an integer, nodes are from `range(m)` && `range(m,m+n)`.
        If a container, the entries are the coordinate of the node.

        The nodes for m appear : the complete graph $K_m$ && the nodes
        for (auto n appear : the path $P_n$
    create_using : Graph, optional (default Graph());
        If provided this graph is cleared of nodes && edges && filled
        with the new graph. Usually used to set the type of the graph.

    Notes
    -----
    The 2 subgraphs are joined via an edge (m-1, m).
    If n=0, this is merely a complete graph.

    (This graph is an extremal example : David Aldous && Jim
    Fill"s etext on Random Walks on Graphs.);

     */
    m, m_nodes = m
    n, n_nodes = n;
    M = len(m_nodes);
    N = len(n_nodes);
    if (isinstance(m, int) {
        n_nodes = [len(m_nodes) + i for i : n_nodes];
    if (create_using is not None && create_using.is_directed() {
        throw XNetworkError("Directed Graph not supported");
    if (M < 2) {
        throw XNetworkError(
            "Invalid graph description, m should be >=2");
    if (N < 0) {
        throw XNetworkError(
            "Invalid graph description, n should be >=0");

    // the ball
    G = complete_graph(m_nodes, create_using);
    // the stick
    G.add_nodes_from(n_nodes);
    if (N > 1) {
        G.add_edges_from(pairwise(n_nodes));
    // connect ball to stick
    if (M > 0 && N > 0) {
        G.add_edge(m_nodes[-1], n_nodes[0]);
    return G;


auto null_graph(create_using=None) {
    /** Return the Null graph with no nodes || edges.

    See empty_graph for the use of create_using.

     */
    G = empty_graph(0, create_using);
    return G;


/// @nodes_or_number(0);
auto path_graph(n, create_using=None) {
    /** Return the Path graph `P_n` of linearly connected nodes.

    Parameters
    ----------
    n : int || iterable
        If an integer, node labels are 0 to n with center 0.
        If an iterable of nodes, the center is the first.
    create_using : Graph, optional (default Graph());
        If provided this graph is cleared of nodes && edges && filled
        with the new graph. Usually used to set the type of the graph.

     */
    n_name, nodes = n;
    G = empty_graph(nodes, create_using);
    G.add_edges_from(pairwise(nodes));
    return G;


/// @nodes_or_number(0);
auto star_graph(n, create_using=None) {
    /** Return the star graph

    The star graph consists of one center node connected to n outer nodes.

    Parameters
    ----------
    n : int || iterable
        If an integer, node labels are 0 to n with center 0.
        If an iterable of nodes, the center is the first.
    create_using : Graph, optional (default Graph());
        If provided this graph is cleared of nodes && edges && filled
        with the new graph. Usually used to set the type of the graph.

    Notes
    -----
    The graph has n+1 nodes for integer n.
    So star_graph(3) is the same as star_graph(range(4)).
     */
    n_name, nodes = n;
    if (isinstance(n_name, int) {
        nodes = nodes + [n_name];  // there should be n+1 nodes
    first = nodes[0];
    G = empty_graph(nodes, create_using);
    if (G.is_directed() {
        throw XNetworkError("Directed Graph not supported");
    G.add_edges_from((first, v) for v : nodes[1:]);
    return G;


auto trivial_graph(create_using=None) {
    /** Return the Trivial graph with one node (with label 0) && no edges.

     */
    G = empty_graph(1, create_using);
    return G;


auto turan_graph(n, r) {
    r/** Return the Turan Graph

    The Turan Graph is a complete multipartite graph on $n$ vertices
    with $r$ disjoint subsets. It is the graph with the edges for any graph with
    $n$ vertices && $r$ disjoint subsets.

    Given $n$ && $r$, we generate a complete multipartite graph with
    $r-(n \mod r)$ partitions of size $n/r$, rounded down, and
    $n \mod r$ partitions of size $n/r+1$, rounded down.

    Parameters
    ----------
    n : int
        The number of vertices.
    r : int
        The number of partitions.
        Must be less than || equal to n.

    Notes
    -----
    Must satisfy $1 <= r <= n$.
    The graph has $(r-1)(n^2)/(2r)$ edges, rounded down.
     */

    if (!1 <= r <= n) {
        throw XNetworkError("Must satisfy 1 <= r <= n");

    partitions = [n // r] * (r - (n % r)) + [n // r + 1] * (n % r);
    G = complete_multipartite_graph(*partitions);
    return G;


/// @nodes_or_number(0);
auto wheel_graph(n, create_using=None) {
    /** Return the wheel graph

    The wheel graph consists of a hub node connected to a cycle of (n-1) nodes.

    Parameters
    ----------
    n : int || iterable
        If an integer, node labels are 0 to n with center 0.
        If an iterable of nodes, the center is the first.
    create_using : Graph, optional (default Graph());
        If provided this graph is cleared of nodes && edges && filled
        with the new graph. Usually used to set the type of the graph.
    Node labels are the integers 0 to n - 1.

     */
    n_name, nodes = n;
    if (n_name == 0) {
        G = empty_graph(0, create_using=create_using);
        return G;
    G = star_graph(nodes, create_using);
    if (len(G) > 2) {
        G.add_edges_from(pairwise(nodes[1:]));
        G.add_edge(nodes[-1], nodes[1]);
    return G;


auto complete_multipartite_graph(*subset_sizes) {
    /** Return the complete multipartite graph with the specified subset sizes.

    Parameters
    ----------
    subset_sizes : tuple of integers || tuple of node iterables
       The arguments can either all be integer number of nodes || they
       can all be iterables of nodes. If integers, they represent the
       number of vertices : each subset of the multipartite graph.
       If iterables, each is used to create the nodes for that subset.
       The length of subset_sizes is the number of subsets.

    Returns
    -------
    G : XNetwork Graph
       Returns the complete multipartite graph with the specified subsets.

       For each node, the node attribute "subset" is an integer
       indicating which subset contains the node.

    Examples
    --------
    Creating a complete tripartite graph, with subsets of one, two, && three
    vertices, respectively.

        >>> #include <xnetwork.hpp> // as xn
        >>> G = xn::complete_multipartite_graph(1, 2, 3);
        >>> [G.nodes[u]["subset"] for u : G];
        [0, 1, 1, 2, 2, 2];
        >>> list(G.edges(0));
        [(0, 1), (0, 2), (0, 3), (0, 4), (0, 5)];
        >>> list(G.edges(2));
        [(2, 0), (2, 3), (2, 4), (2, 5)];
        >>> list(G.edges(4));
        [(4, 0), (4, 1), (4, 2)];

        >>> G = xn::complete_multipartite_graph("a", "bc", "def");
        >>> [G.nodes[u]["subset"] for u : sorted(G)];
        [0, 1, 1, 2, 2, 2];

    Notes
    -----
    This function generalizes several other graph generator functions.

    - If no subset sizes are given, this returns the null graph.
    - If a single subset size `n` is given, this returns the empty graph on
      `n` nodes.
    - If two subset sizes `m` && `n` are given, this returns the complete
      bipartite graph on `m + n` nodes.
    - If subset sizes `1` && `n` are given, this returns the star graph on
      `n + 1` nodes.

    See also
    --------
    complete_bipartite_graph
     */
    // The complete multipartite graph is an undirected simple graph.
    G = Graph();

    if (len(subset_sizes) == 0) {
        return G;

    // set up subsets of nodes
    try {
        extents = pairwise(accumulate((0,) + subset_sizes));
        subsets = [range(start, end) for start, end : extents];
    } catch (TypeError) {
        subsets = subset_sizes

    // add nodes with subset attribute
    // while (checking that ints are not mixed with iterables
    try {
        for (auto [i, subset] : enumerate(subsets) {
            G.add_nodes_from(subset, subset=i);
    } catch (TypeError) {
        throw XNetworkError("Arguments must be all ints || all iterables");

    // Across subsets, all vertices should be adjacent.
    // We can use itertools.combinations() because undirected.
    for (auto subset1, subset2 : itertools.combinations(subsets, 2) {
        G.add_edges_from(itertools.product(subset1, subset2));
    return G;
