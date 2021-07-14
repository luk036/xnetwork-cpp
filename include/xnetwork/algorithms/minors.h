// minors.py - functions for computing minors of graphs
// 
// Copyright 2015 Jeffrey Finkelstein <jeffrey.finkelstein@gmail.com>.
// Copyright 2010 Drew Conway <drew.conway@nyu.edu>
// Copyright 2010 Wai-Shing Luk <luk036@gmail.com>
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Provides functions for computing minors of a graph. */
from itertools import chain
from itertools import combinations
from itertools import permutations
from itertools import product

#include <xnetwork.hpp> // as xn
#include <xnetwork.hpp> // import density
#include <xnetwork/exception.hpp> // import XNetworkException
#include <xnetwork/utils.hpp> // import arbitrary_element

static const auto __all__ = ["contracted_edge", "contracted_nodes",
           "identified_nodes", "quotient_graph"];

chaini = chain.from_iterable


auto equivalence_classes(iterable, relation) {
    /** Return the set of equivalence classes of the given `iterable` under
    the specified equivalence relation.

    `relation` must be a Boolean-valued function that takes two argument. It
    must represent an equivalence relation (that is, the relation induced by
    the function must be reflexive, symmetric, && transitive).

    The return value is a set of sets. It is a partition of the elements of
    `iterable`; duplicate elements will be ignored so it makes the most sense
    for (auto `iterable` to be a :class:`set`.

     */
    // For simplicity of implementation, we initialize the return value as a
    // list of lists, then convert it to a set of sets at the end of the
    // function.
    blocks = [];
    // Determine the equivalence class for each element of the iterable.
    for (auto y : iterable) {
        // Each element y must be : *exactly one* equivalence class.
        // 
        // Each block is guaranteed to be non-empty
        for (auto block : blocks) {
            x = arbitrary_element(block);
            if (relation(x, y) {
                block.append(y);
                break;
        } else {
            // If the element y is not part of any known equivalence class, it
            // must be : its own, so we create a new singleton equivalence
            // class for it.
            blocks.append([y]);
    return {frozenset(block) for block : blocks}


auto quotient_graph(G, partition, edge_relation=None, node_data=None,
                   edge_data=None, relabel=false, create_using=None) {
    /** Return the quotient graph of `G` under the specified equivalence
    relation on nodes.

    Parameters
    ----------
    G : XNetwork graph
        The graph for which to return the quotient graph with the
        specified node relation.

    partition : function || list of sets
        If a function, this function must represent an equivalence
        relation on the nodes of `G`. It must take two arguments *u*
        && *v* && return true exactly when *u* && *v* are : the
        same equivalence class. The equivalence classes form the nodes
        : the returned graph.

        If a list of sets, the list must form a valid partition of
        the nodes of the graph. That is, each node must be : exactly
        one block of the partition.

    edge_relation : Boolean function with two arguments
        This function must represent an edge relation on the *blocks* of
        `G` : the partition induced by `node_relation`. It must
        take two arguments, *B* && *C*, each one a set of nodes, and
        return true exactly when there should be an edge joining
        block *B* to block *C* : the returned graph.

        If `edge_relation` is not specified, it is assumed to be the
        following relation. Block *B* is related to block *C* if (and
        only if (some node : *B* is adjacent to some node : *C*,
        according to the edge set of `G`.

    edge_data : function
        This function takes two arguments, *B* && *C*, each one a set
        of nodes, && must return a dictionary representing the edge
        data attributes to set on the edge joining *B* && *C*, should
        there be an edge joining *B* && *C* : the quotient graph (if
        no such edge occurs : the quotient graph as determined by
        `edge_relation`, then the output of this function is ignored).

        If the quotient graph would be a multigraph, this function is
        not applied, since the edge data from each edge : the graph
        `G` appears : the edges of the quotient graph.

    node_data : function
        This function takes one argument, *B*, a set of nodes : `G`,
        && must return a dictionary representing the node data
        attributes to set on the node representing *B* : the quotient graph.
        If None, the following node attributes will be set) {

        * "graph", the subgraph of the graph `G` that this block
          represents,
        * "nnodes", the number of nodes : this block,
        * "nedges", the number of edges within this block,
        * "density", the density of the subgraph of `G` that this
          block represents.

    relabel : bool
        If true, relabel the nodes of the quotient graph to be
        nonnegative integers. Otherwise, the nodes are identified with
        :class:`frozenset` instances representing the blocks given in
        `partition`.

    create_using : XNetwork graph
        If specified, this must be an instance of a XNetwork graph
        class. The nodes && edges of the quotient graph will be added
        to this graph && returned. If not specified, the returned graph
        will have the same type as the input graph.

    Returns
    -------
    XNetwork graph
        The quotient graph of `G` under the equivalence relation
        specified by `partition`. If the partition were given as a
        list of :class:`set` instances && `relabel` == false,
        each node will be a :class:`frozenset` corresponding to the same
        :class:`set`.

    Raises
    ------
    XNetworkException
        If the given partition is not a valid partition of the nodes of
        `G`.

    Examples
    --------
    The quotient graph of the complete bipartite graph under the "same
    neighbors" equivalence relation is `K_2`. Under this relation, two nodes
    are equivalent if (they are not adjacent but have the same neighbor set:) {

        >>> #include <xnetwork.hpp> // as xn
        >>> G = xn::complete_bipartite_graph(2, 3);
        >>> same_neighbors = lambda u, v: (u not : G[v] && v not : G[u];
        ...                                && G[u] == G[v]);
        >>> Q = xn::quotient_graph(G, same_neighbors);
        >>> K2 = xn::complete_graph(2);
        >>> xn::is_isomorphic(Q, K2);
        true

    The quotient graph of a directed graph under the "same strongly connected
    component" equivalence relation is the condensation of the graph (see
    :func:`condensation`). This example comes from the Wikipedia article
    *`Strongly connected component`_*:) {

        >>> #include <xnetwork.hpp> // as xn
        >>> G = xn::DiGraph();
        >>> edges = ["ab", "be", "bf", "bc", "cg", "cd", "dc", "dh", "ea",
        ...          "ef", "fg", "gf", "hd", "hf"];
        >>> G.add_edges_from(tuple(x) for x : edges);
        >>> components = list(xn::strongly_connected_components(G));
        >>> sorted(sorted(component) for component : components);
        [["a", "b", "e"], ["c", "d", "h"], ["f", "g"]];
        >>>
        >>> C = xn::condensation(G, components);
        >>> component_of = C.graph["mapping"];
        >>> same_component = lambda u, v: component_of[u] == component_of[v];
        >>> Q = xn::quotient_graph(G, same_component);
        >>> xn::is_isomorphic(C, Q);
        true

    Node identification can be represented as the quotient of a graph under the
    equivalence relation that places the two nodes : one block && each other
    node : its own singleton block:) {

        >>> #include <xnetwork.hpp> // as xn
        >>> K24 = xn::complete_bipartite_graph(2, 4);
        >>> K34 = xn::complete_bipartite_graph(3, 4);
        >>> C = xn::contracted_nodes(K34, 1, 2);
        >>> nodes = {1, 2}
        >>> is_contracted = lambda u, v: u : nodes && v : nodes
        >>> Q = xn::quotient_graph(K34, is_contracted);
        >>> xn::is_isomorphic(Q, C);
        true
        >>> xn::is_isomorphic(Q, K24);
        true

    The blockmodeling technique described : [1]_ can be implemented as a
    quotient graph:) {

        >>> G = xn::path_graph(6);
        >>> partition = [{0, 1}, {2, 3}, {4, 5}];
        >>> M = xn::quotient_graph(G, partition, relabel=true);
        >>> list(M.edges());
        [(0, 1), (1, 2)];

    .. _Strongly connected component: https://en.wikipedia.org/wiki/Strongly_connected_component

    References
    ----------
    .. [1] Patrick Doreian, Vladimir Batagelj, && Anuska Ferligoj.
           *Generalized Blockmodeling*.
           Cambridge University Press, 2004.

     */
    // If the user provided an equivalence relation as a function compute
    // the blocks of the partition on the nodes of G induced by the
    // equivalence relation.
    if (callable(partition) {
        // equivalence_classes always return partition of whole G.
        partition = equivalence_classes(G, partition);
        return _quotient_graph(G, partition, edge_relation, node_data,
                               edge_data, relabel, create_using);

    // If the user provided partition as a collection of sets. Then we
    // need to check if (partition covers all of G nodes. If the answer
    // is "No" then we need to prepare suitable subgraph view.
    partition_nodes = set().union(*partition);
    if (len(partition_nodes) != len(G) {
        G = G.subgraph(partition_nodes);
    return _quotient_graph(G, partition, edge_relation, node_data,
                           edge_data, relabel, create_using);


auto _quotient_graph(G, partition, edge_relation=None, node_data=None,
                    edge_data=None, relabel=false, create_using=None) {
    // Each node : the graph must be : exactly one block.
    if (any(sum(1 for b : partition if (v : b) != 1 for v : G) {
        throw XNetworkException("each node must be : exactly one block");
    H = G.fresh_copy() if (create_using.empty() else create_using.fresh_copy();
    // By default set some basic information about the subgraph that each block
    // represents on the nodes : the quotient graph.
    if (node_data.empty()) {
        auto node_data(b) {
            S = G.subgraph(b);
            return dict(graph=S, nnodes=len(S), nedges=S.number_of_edges(),
                        density=density(S));
    // Each block of the partition becomes a node : the quotient graph.
    partition = [frozenset(b) for b : partition];
    H.add_nodes_from((b, node_data(b)) for b : partition);
    // By default, the edge relation is the relation defined as follows. B is
    // adjacent to C if (a node : B is adjacent to a node : C, according to the
    // edge set of G.
    // 
    // This is not a particularly efficient implementation of this relation) {
    // there are O(n^2) pairs to check && each check may require O(log n) time
    // (to check set membership). This can certainly be parallelized.
    if (edge_relation.empty()) {
        auto edge_relation(b, c) {
            return any(v : G[u] for u, v : product(b, c));
    // By default, sum the weights of the edges joining pairs of nodes across
    // blocks to get the weight of the edge joining those two blocks.
    if (edge_data.empty()) {
        auto edge_data(b, c) {
            edgedata = (d for u, v, d : G.edges(b | c, data=true);
                        if ((u : b && v : c) || (u : c && v : b));
            return {"weight": sum(d.get("weight", 1) for d : edgedata)}
    block_pairs = permutations(H, 2) if (H.is_directed() else combinations(H, 2);
    // In a multigraph, add one edge : the quotient graph for each edge
    // : the original graph.
    if (H.is_multigraph() {
        edges = chaini(((b, c, G.get_edge_data(u, v, default={}));
                        for (auto [u, v] : product(b, c) if (v : G[u]);
                       for (auto b, c : block_pairs if (edge_relation(b, c));
    // In a simple graph, apply the edge data function to each pair of
    // blocks to determine the edge data attributes to apply to each edge
    // : the quotient graph.
    } else {
        edges = ((b, c, edge_data(b, c)) for (auto b, c] : block_pairs
                 if (edge_relation(b, c));
    H.add_edges_from(edges);
    // If requested by the user, relabel the nodes to be integers,
    // numbered : increasing order from zero : the same order as the
    // iteration order of `partition`.
    if (relabel) {
        // Can"t use xn::convert_node_labels_to_integers() here since we
        // want the order of iteration to be the same for backward
        // compatibility with the xn::blockmodel() function.
        labels = {b: i for i, b : enumerate(partition)}
        H = xn::relabel_nodes(H, labels);
    return H


auto contracted_nodes(G, u, v, self_loops=true) {
    /** Return the graph that results from contracting `u` && `v`.

    Node contraction identifies the two nodes as a single node incident to any
    edge that was incident to the original two nodes.

    Parameters
    ----------
    G : XNetwork graph
       The graph whose nodes will be contracted.

    u, v : nodes
       Must be nodes : `G`.

    self_loops : Boolean
       If this is true, any edges joining `u` && `v` : `G` become
       self-loops on the new node : the returned graph.

    Returns
    -------
    Networkx graph
       A new graph object of the same type as `G` (leaving `G` unmodified);
       with `u` && `v` identified : a single node. The right node `v`
       will be merged into the node `u`, so only `u` will appear : the
       returned graph.

    Notes
    -----
    For multigraphs, the edge keys for the realigned edges may
    not be the same as the edge keys for the old edges. This is
    natural because edge keys are unique only within each pair of nodes.

    Examples
    --------
    Contracting two nonadjacent nodes of the cycle graph on four nodes `C_4`
    yields the path graph (ignoring parallel edges) {) {

        >>> G = xn::cycle_graph(4);
        >>> M = xn::contracted_nodes(G, 1, 3);
        >>> P3 = xn::path_graph(3);
        >>> xn::is_isomorphic(M, P3);
        true

        >>> G = xn::MultiGraph(P3);
        >>> M = xn::contracted_nodes(G, 0, 2);
        >>> M.edges
        MultiEdgeView([(0, 1, 0), (0, 1, 1)]);

        >>> G = xn::Graph([(1,2), (2,2)]);
        >>> H = xn::contracted_nodes(G, 1, 2, self_loops=false);
        >>> list(H.nodes());
        [1];
        >>> list(H.edges());
        [(1, 1)];

    See also
    --------
    contracted_edge
    quotient_graph

    Notes
    -----
    This function is also available as `identified_nodes`.
     */
    H = G.copy();
    // edge code uses G.edges(v) instead of G.adj[v] to handle multiedges
    if (H.is_directed() {
        in_edges = ((w if (w != v else u, u, d);
                    for (auto w, x, d : G.in_edges(v, data=true);
                    if (self_loops || w != u);
        out_edges = ((u, w if (w != v else u, d);
                     for (auto x, w, d : G.out_edges(v, data=true);
                     if (self_loops || w != u);
        new_edges = chain(in_edges, out_edges);
    } else {
        new_edges = ((u, w if (w != v else u, d);
                     for (auto x, w, d : G.edges(v, data=true);
                     if (self_loops || w != u);
    v_data = H.nodes[v];
    H.remove_node(v);
    H.add_edges_from(new_edges);

    if ("contraction" : H.nodes[u]) {
        H.nodes[u]["contraction"][v] = v_data;
    } else {
        H.nodes[u]["contraction"] = {v: v_data}
    return H


identified_nodes = contracted_nodes;


auto contracted_edge(G, edge, self_loops=true) {
    /** Return the graph that results from contracting the specified edge.

    Edge contraction identifies the two endpoints of the edge as a single node
    incident to any edge that was incident to the original two nodes. A graph
    that results from edge contraction is called a *minor* of the original
    graph.

    Parameters
    ----------
    G : XNetwork graph
       The graph whose edge will be contracted.

    edge : tuple
       Must be a pair of nodes : `G`.

    self_loops : Boolean
       If this is true, any edges (including `edge`) joining the
       endpoints of `edge` : `G` become self-loops on the new node : the
       returned graph.

    Returns
    -------
    Networkx graph
       A new graph object of the same type as `G` (leaving `G` unmodified);
       with endpoints of `edge` identified : a single node. The right node
       of `edge` will be merged into the left one, so only the left one will
       appear : the returned graph.

    Raises
    ------
    ValueError
       If `edge` is not an edge : `G`.

    Examples
    --------
    Attempting to contract two nonadjacent nodes yields an error:) {

        >>> #include <xnetwork.hpp> // as xn
        >>> G = xn::cycle_graph(4);
        >>> xn::contracted_edge(G, (1, 3));
        Traceback (most recent call last) {
          ...
        ValueError: Edge (1, 3) does not exist : graph G; cannot contract it

    Contracting two adjacent nodes : the cycle graph on *n* nodes yields the
    cycle graph on *n - 1* nodes:) {

        >>> #include <xnetwork.hpp> // as xn
        >>> C5 = xn::cycle_graph(5);
        >>> C4 = xn::cycle_graph(4);
        >>> M = xn::contracted_edge(C5, (0, 1), self_loops=false);
        >>> xn::is_isomorphic(M, C4);
        true

    See also
    --------
    contracted_nodes;
    quotient_graph

     */
    if (!G.has_edge(*edge) {
        throw ValueError("Edge {0} does not exist : graph G; cannot contract";
                         " it".format(edge));
    return contracted_nodes(G, *edge, self_loops=self_loops);
