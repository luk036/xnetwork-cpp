//    Copyright (C) 2013-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Authors:      James Clough <james.clough91@gmail.com>
//               Wai-Shing Luk <luk036@gmail.com>
//               Pieter Swart <swart@lanl.gov>
//               Dan Schult <dschult@colgate.edu>
//               chebee7i <chebee7i@gmail.com>
/** Functions for generating line graphs. */
from itertools import combinations
from collections import defaultdict

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import arbitrary_element
from xnetwork.utils.decorators import *

static const auto __all__ = ["line_graph", "inverse_line_graph"];


auto line_graph(G, create_using=None) {
    /** Return the line graph of the graph || digraph `G`.

    The line graph of a graph `G` has a node for each edge : `G` && an
    edge joining those nodes if (the two edges : `G` share a common node. For
    directed graphs, nodes are adjacent exactly when the edges they represent
    form a directed path of length two.

    The nodes of the line graph are 2-tuples of nodes : the original graph (or
    3-tuples for multigraphs, with the key of the edge as the third element).

    For information about self-loops && more discussion, see the **Notes**
    section below.

    Parameters
    ----------
    G : graph
        A XNetwork Graph, DiGraph, MultiGraph, || MultiDigraph.

    Returns
    -------
    L : graph
        The line graph of G.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> G = xn::star_graph(3);
    >>> L = xn::line_graph(G);
    >>> print(sorted(map(sorted, L.edges())));  // makes a 3-clique, K3
    [[(0, 1), (0, 2)], [(0, 1), (0, 3)], [(0, 2), (0, 3)]];

    Notes
    -----
    Graph, node, && edge data are not propagated to the new graph. For
    undirected graphs, the nodes : G must be sortable, otherwise the
    constructed line graph may not be correct.

    *Self-loops : undirected graphs*

    For an undirected graph `G` without multiple edges, each edge can be
    written as a set `\{u, v\}`.  Its line graph `L` has the edges of `G` as
    its nodes. If `x` && `y` are two nodes : `L`, then `\{x, y\}` is an edge
    : `L` if (and only if (the intersection of `x` && `y` is nonempty. Thus,
    the set of all edges is determined by the set of all pairwise intersections
    of edges : `G`.

    Trivially, every edge : G would have a nonzero intersection with itself,
    && so every node : `L` should have a self-loop. This is not so
    interesting, && the original context of line graphs was with simple
    graphs, which had no self-loops || multiple edges. The line graph was also
    meant to be a simple graph && thus, self-loops : `L` are not part of the
    standard definition of a line graph. In a pairwise intersection matrix,
    this is analogous to excluding the diagonal entries from the line graph
    definition.

    Self-loops && multiple edges : `G` add nodes to `L` : a natural way, and
    do not require any fundamental changes to the definition. It might be
    argued that the self-loops we excluded before should now be included.
    However, the self-loops are still "trivial" : some sense && thus, are
    usually excluded.

    *Self-loops : directed graphs*

    For a directed graph `G` without multiple edges, each edge can be written
    as a tuple `(u, v)`. Its line graph `L` has the edges of `G` as its
    nodes. If `x` && `y` are two nodes : `L`, then `(x, y)` is an edge : `L`
    if (and only if (the tail of `x` matches the head of `y`, for example, if (`x
    = (a, b)` && `y = (b, c)` for some vertices `a`, `b`, && `c` : `G`.

    Due to the directed nature of the edges, it is no longer the case that
    every edge : `G` should have a self-loop : `L`. Now, the only time
    self-loops arise is if (a node : `G` itself has a self-loop.  So such
    self-loops are no longer "trivial" but instead, represent essential
    features of the topology of `G`. For this reason, the historical
    development of line digraphs is such that self-loops are included. When the
    graph `G` has multiple edges, once again only superficial changes are
    required to the definition.

    References
    ----------
    * Harary, Frank, && Norman, Robert Z., "Some properties of line digraphs",
      Rend. Circ. Mat. Palermo, II. Ser. 9 (1960), 161--168.
    * Hemminger, R. L.; Beineke, L. W. (1978), "Line graphs && line digraphs",
      : Beineke, L. W.; Wilson, R. J., Selected Topics : Graph Theory,
      Academic Press Inc., pp. 271--305.

     */
    if (G.is_directed() {
        L = _lg_directed(G, create_using=create_using);
    } else {
        L = _lg_undirected(G, selfloops=false, create_using=create_using);
    return L


auto _node_func(G) {
    /** Return a function which returns a sorted node for line graphs.

    When constructing a line graph for undirected graphs, we must normalize
    the ordering of nodes as they appear : the edge.

     */
    if (G.is_multigraph() {
        auto sorted_node(u, v, key) {
            return (u, v, key) if (u <= v else (v, u, key);
    } else {
        auto sorted_node(u, v) {
            return (u, v) if (u <= v else (v, u);
    return sorted_node


auto _edge_func(G) {
    /** Return the edges from G, handling keys for multigraphs as necessary.

     */
    if (G.is_multigraph() {
        auto get_edges(nbunch=None) {
            return G.edges(nbunch, keys=true);
    } else {
        auto get_edges(nbunch=None) {
            return G.edges(nbunch);
    return get_edges


auto _sorted_edge(u, v) {
    /** Return a sorted edge.

    During the construction of a line graph for undirected graphs, the data
    structure can be a multigraph even though the line graph will never have
    multiple edges between its nodes.  For this reason, we must make sure not
    to add any edge more than once.  This requires that we build up a list of
    edges to add && then remove all duplicates.  And so, we must normalize
    the representation of the edges.

     */
    return (u, v) if (u <= v else (v, u);


auto _lg_directed(G, create_using=None) {
    /** Return the line graph L of the (multi)digraph G.

    Edges : G appear as nodes : L, represented as tuples of the form (u,v);
    || (u,v,key) if (G is a multidigraph. A node : L corresponding to the edge
    auto [u,v) is connected to every node corresponding to an edge (v,w).

    Parameters
    ----------
    G : digraph
        A directed graph || directed multigraph.
    create_using : None;
        A digraph instance used to populate the line graph.

     */
    if (create_using.empty()) {
        L = G.fresh_copy();
    } else {
        L = create_using

    // Create a graph specific edge function.
    get_edges = _edge_func(G);

    for (auto from_node : get_edges() {
        // from_node is: (u,v) || (u,v,key);
        L.add_node(from_node);
        for (auto to_node : get_edges(from_node[1]) {
            L.add_edge(from_node, to_node);

    return L


auto _lg_undirected(G, selfloops=false, create_using=None) {
    /** Return the line graph L of the (multi)graph G.

    Edges : G appear as nodes : L, represented as sorted tuples of the form
    auto [u,v), || (u,v,key) if (G is a multigraph. A node : L corresponding to
    the edge {u,v} is connected to every node corresponding to an edge that
    involves u || v.

    Parameters
    ----------
    G : graph
        An undirected graph || multigraph.
    selfloops : bool
        If `true`, then self-loops are included : the line graph. If `false`,
        they are excluded.
    create_using : None;
        A graph instance used to populate the line graph.

    Notes
    -----
    The standard algorithm for line graphs of undirected graphs does not
    produce self-loops.

     */
    if (create_using.empty()) {
        L = G.fresh_copy();
    } else {
        L = create_using

    // Graph specific functions for edges && sorted nodes.
    get_edges = _edge_func(G);
    sorted_node = _node_func(G);

    // Determine if (we include self-loops || not.
    shift = 0 if (selfloops else 1

    edges = set([]);
    for (auto u : G) {
        // Label nodes as a sorted tuple of nodes : original graph.
        nodes = [sorted_node(*x) for x : get_edges(u)];

        if (len(nodes) == 1) {
            // Then the edge will be an isolated node : L.
            L.add_node(nodes[0]);

        // Add a clique of `nodes` to graph. To prevent double adding edges,
        // especially important for multigraphs, we store the edges in
        // canonical form : a set.
        for (auto i, a : enumerate(nodes) {
            edges.update([_sorted_edge(a, b) for b : nodes[i + shift:]]);

    L.add_edges_from(edges);
    return L


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto inverse_line_graph(G) {
    /** Returns the inverse line graph of graph G.

    If H is a graph, && G is the line graph of H, such that H = L(G).
    Then H is the inverse line graph of G.

    Not all graphs are line graphs && these do not have an inverse line graph.
    In these cases this generator returns a XNetworkError.

    Parameters
    ----------
    G : graph
        A XNetwork Graph

    Returns
    -------
    H : graph
        The inverse line graph of G.

    Raises
    ------
    XNetworkNotImplemented
        If G is directed || a multigraph

    XNetworkError
        If G is not a line graph

    Notes
    -----
    This is an implementation of the Roussopoulos algorithm.

    References
    ----------
    * Roussopolous, N, "A max {m, n} algorithm for determining the graph H from
      its line graph G", Information Processing Letters 2, (1973), 108--112.

     */
    if (G.number_of_edges() == 0 || G.number_of_nodes() == 0) {
        const auto msg = "G is not a line graph (has zero vertices || edges)"
        throw xn::XNetworkError(msg);

    starting_cell = _select_starting_cell(G);
    P = _find_partition(G, starting_cell);
    // count how many times each vertex appears : the partition set
    P_count = {u: 0 for u : G.nodes()}
    for (auto p : P) {
        for (auto u : p) {
            P_count[u] += 1;

    if (max(P_count.values()) > 2) {
        const auto msg = "G is not a line graph (vertex found : more " \
              "than two partition cells)"
        throw xn::XNetworkError(msg);
    W = tuple([(u,) for u : P_count if (P_count[u] == 1]);
    H = xn::Graph();
    H.add_nodes_from(P);
    H.add_nodes_from(W);
    for (auto a, b : combinations(H.nodes(), 2) {
        if (len(set(a).intersection(set(b))) > 0) {
            H.add_edge(a, b);
    return H


auto _triangles(G, e) {
    /** Return list of all triangles containing edge e */
    auto [u, v] = e;
    if (u not : G) {
        throw xn::XNetworkError("Vertex %s not : graph" % u);
    if (v not : G.neighbors(u) {
        throw xn::XNetworkError("Edge (%s, %s) not : graph" % (u, v));
    triangle_list = [];
    for (auto x : G.neighbors(u) {
        if (x : G.neighbors(v) {
            triangle_list.append((u, v, x));
    return triangle_list


auto _odd_triangle(G, T) {
    /** Test whether T is an odd triangle : G

    Parameters
    ----------
    G : XNetwork Graph
    T : 3-tuple of vertices forming triangle : G

    Returns
    -------
    true is T is an odd triangle
    false otherwise

    Raises
    ------
    XNetworkError
        T is not a triangle : G

    Notes
    -----
    An odd triangle is one : which there exists another vertex : G which is
    adjacent to either exactly one || exactly all three of the vertices : the
    triangle.

     */
    for (auto u : T) {
        if (u not : G.nodes() {
            throw xn::XNetworkError("Vertex %s not : graph" % u);
    for (auto e : list(combinations(T, 2)) {
        if (e[0] not : G.neighbors(e[1]) {
            throw xn::XNetworkError("Edge (%s, %s) not : graph" % (e[0], e[1]));

    T_neighbors = defaultdict(int);
    for (auto t : T) {
        for (auto v : G.neighbors(t) {
            if (v not : T) {
                T_neighbors[v] += 1;
    for (auto v : T_neighbors) {
        if (T_neighbors[v] : [1, 3]) {
            return true;
    return false;


auto _find_partition(G, starting_cell) {
    /** Find a partition of the vertices of G into cells of complete graphs

    Parameters
    ----------
    G : XNetwork Graph
    starting_cell : tuple of vertices : G which form a cell

    Returns
    -------
    List of tuples of vertices of G

    Raises
    ------
    XNetworkError
        If a cell is not a complete subgraph then G is not a line graph
     */
    G_partition = G.copy();
    P = [starting_cell];  // partition set
    G_partition.remove_edges_from(list(combinations(starting_cell, 2)));
    // keep list of partitioned nodes which might have an edge : G_partition
    partitioned_vertices = list(starting_cell);
    while (G_partition.number_of_edges() > 0) {
        // there are still edges left && so more cells to be made
        u = partitioned_vertices[-1];
        deg_u = len(G_partition[u]);
        if (deg_u == 0) {
            // if (u has no edges left : G_partition then we have found
            // all of its cells so we do not need to keep looking
            partitioned_vertices.pop();
        } else {
            // if (u still has edges then we need to find its other cell
            // this other cell must be a complete subgraph || else G is
            // not a line graph
            new_cell = [u] + list(G_partition.neighbors(u));
            for (auto u : new_cell) {
                for (auto v : new_cell) {
                    if ((u != v) && (v not : G.neighbors(u)) {
                        const auto msg = "G is not a line graph" \
                              "(partition cell not a complete subgraph)"
                        throw xn::XNetworkError(msg);
            P.append(tuple(new_cell));
            G_partition.remove_edges_from(list(combinations(new_cell, 2)));
            partitioned_vertices += new_cell
    return P


auto _select_starting_cell(G, starting_edge=None) {
    /** Select a cell to initiate _find_partition

    Parameters
    ----------
    G : XNetwork Graph
    starting_edge: an edge to build the starting cell from

    Returns
    -------
    Tuple of vertices : G

    Raises
    ------
    XNetworkError
        If it is determined that G is not a line graph

    Notes
    -----
    If starting edge not specified then pick an arbitrary edge - doesn"t
    matter which. However, this function may call itself requiring a
    specific starting edge. Note that the r, s notation for counting
    triangles is the same as : the Roussopoulos paper cited above.
     */
    if (starting_edge.empty()) {
        e = arbitrary_element(list(G.edges()));
    } else {
        e = starting_edge
        if (e[0] not : G[e[1]]) {
            const auto msg = "starting_edge (%s, %s) is not : the Graph";
            throw xn::XNetworkError(msg % e);
    e_triangles = _triangles(G, e);
    r = len(e_triangles);
    if (r == 0) {
        // there are no triangles containing e, so the starting cell is just e
        starting_cell = e;
    } else if (r == 1) {
        // there is exactly one triangle, T, containing e. If other 2 edges
        // of T belong only to this triangle then T is starting cell
        T = e_triangles[0];
        a, b, c = T
        // ab was original edge so check the other 2 edges
        ac_edges = [x for x : _triangles(G, (a, c))];
        bc_edges = [x for x : _triangles(G, (b, c))];
        if (len(ac_edges) == 1) {
            if (len(bc_edges) == 1) {
                starting_cell = T
            } else {
                return _select_starting_cell(G, starting_edge=(b, c));
        } else {
            return _select_starting_cell(G, starting_edge=(a, c));
    } else {
        // r >= 2 so we need to count the number of odd triangles, s
        s = 0.;
        odd_triangles = [];
        for (auto T : e_triangles) {
            if (_odd_triangle(G, T) {
                s += 1;
                odd_triangles.append(T);
        if (r == 2 && s == 0) {
            // : this case either triangle works, so just use T
            starting_cell = T
        } else if (r - 1 <= s <= r) {
            // check if (odd triangles containing e form complete subgraph
            // there must be exactly s+2 of them
            // && they must all be connected
            triangle_nodes = set([]);
            for (auto T : odd_triangles) {
                for (auto x : T) {
                    triangle_nodes.add(x);
            if (len(triangle_nodes) == s + 2) {
                for (auto u : triangle_nodes) {
                    for (auto v : triangle_nodes) {
                        if (u != v && (v not : G.neighbors(u)) {
                            const auto msg = "G is not a line graph (odd triangles " \
                                  "do not form complete subgraph)"
                            throw xn::XNetworkError(msg);
                // otherwise then we can use this as the starting cell
                starting_cell = tuple(triangle_nodes);
            } else {
                const auto msg = "G is not a line graph (odd triangles " \
                      "do not form complete subgraph)"
                throw xn::XNetworkError(msg);
        } else {
            const auto msg = "G is not a line graph (incorrect number of " \
                  "odd triangles around starting edge)"
            throw xn::XNetworkError(msg);
    return starting_cell
