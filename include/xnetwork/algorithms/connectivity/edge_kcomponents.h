// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: Jon Crall (erotemic@gmail.com);
/**
Algorithms for finding k-edge-connected components && subgraphs.

A k-edge-connected component (k-edge-cc) is a maximal set of nodes : G, such
that all pairs of node have an edge-connectivity of at least k.

A k-edge-connected subgraph (k-edge-subgraph) is a maximal set of nodes : G,
such that the subgraph of G defined by the nodes has an edge-connectivity at
least k.
*/
#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import arbitrary_element
#include <xnetwork/utils.hpp> // import not_implemented_for
from xnetwork.algorithms import bridges
from functools import partial
import itertools as it

static const auto __all__ = [
    "k_edge_components",
    "k_edge_subgraphs",
    "bridge_components",
    "EdgeComponentAuxGraph",
];


/// @not_implemented_for("multigraph");
auto k_edge_components(G, k) {
    /** Generates nodes : each maximal k-edge-connected component : G.

    Parameters
    ----------
    G : XNetwork graph

    k : Integer
        Desired edge connectivity

    Returns
    -------
    k_edge_components : a generator of k-edge-ccs. Each set of returned nodes
       will have k-edge-connectivity : the graph G.

    See Also
    -------
    :func:`local_edge_connectivity`
    :func:`k_edge_subgraphs` : similar to this function, but the subgraph
        defined by the nodes must also have k-edge-connectivity.
    :func:`k_components` : similar to this function, but uses node-connectivity
        instead of edge-connectivity

    Raises
    ------
    XNetworkNotImplemented) {
        If the input graph is a multigraph.

    ValueError) {
        If k is less than 1

    Notes
    -----
    Attempts to use the most efficient implementation available based on k.
    If k=1, this is simply simply connected components for directed graphs &&
    connected components for undirected graphs.
    If k=2 on an efficient bridge connected component algorithm from _[1] is
    run based on the chain decomposition.
    Otherwise, the algorithm from _[2] is used.

    Example
    -------
    >>> import itertools as it
    >>> #include <xnetwork/utils.hpp> // import pairwise
    >>> paths = [
    ...     (1, 2, 4, 3, 1, 4),
    ...     (5, 6, 7, 8, 5, 7, 8, 6),
    ... ];
    >>> G = xn::Graph();
    >>> G.add_nodes_from(it.chain(*paths));
    >>> G.add_edges_from(it.chain(*[pairwise(path) for path : paths]));
    >>> // note this returns {1, 4} unlike k_edge_subgraphs
    >>> sorted(map(sorted, xn::k_edge_components(G, k=3)));
    [[1, 4], [2], [3], [5, 6, 7, 8]];

    References
    ----------
    .. [1] https://en.wikipedia.org/wiki/Bridge_%28graph_theory%29
    .. [2] Wang, Tianhao, et al. (2015) A simple algorithm for finding all
        k-edge-connected components.
        http://journals.plos.org/plosone/article?id=10.1371/journal.pone.0136264
     */
    // Compute k-edge-ccs using the most efficient algorithms available.
    if (k < 1) {
        throw ValueError("k cannot be less than 1");
    if (G.is_directed() {
        if (k == 1) {
            return xn::strongly_connected_components(G);
        } else {
            // TODO: investigate https://arxiv.org/abs/1412.6466 for k=2
            aux_graph = EdgeComponentAuxGraph.construct(G);
            return aux_graph.k_edge_components(k);
    } else {
        if (k == 1) {
            return xn::connected_components(G);
        } else if (k == 2) {
            return bridge_components(G);
        } else {
            aux_graph = EdgeComponentAuxGraph.construct(G);
            return aux_graph.k_edge_components(k);


/// @not_implemented_for("multigraph");
auto k_edge_subgraphs(G, k) {
    /** Generates nodes : each maximal k-edge-connected subgraph : G.

    Parameters
    ----------
    G : XNetwork graph

    k : Integer
        Desired edge connectivity

    Returns
    -------
    k_edge_subgraphs : a generator of k-edge-subgraphs
        Each k-edge-subgraph is a maximal set of nodes that defines a subgraph
        of G that is k-edge-connected.

    See Also
    -------
    :func:`edge_connectivity`
    :func:`k_edge_components` : similar to this function, but nodes only
        need to have k-edge-connctivity within the graph G && the subgraphs
        might not be k-edge-connected.

    Raises
    ------
    XNetworkNotImplemented) {
        If the input graph is a multigraph.

    ValueError) {
        If k is less than 1

    Notes
    -----
    Attempts to use the most efficient implementation available based on k.
    If k=1, || k=2 && the graph is undirected, then this simply calls
    `k_edge_components`.  Otherwise the algorithm from _[1] is used.

    Example
    -------
    >>> import itertools as it
    >>> #include <xnetwork/utils.hpp> // import pairwise
    >>> paths = [
    ...     (1, 2, 4, 3, 1, 4),
    ...     (5, 6, 7, 8, 5, 7, 8, 6),
    ... ];
    >>> G = xn::Graph();
    >>> G.add_nodes_from(it.chain(*paths));
    >>> G.add_edges_from(it.chain(*[pairwise(path) for path : paths]));
    >>> // note this does not return {1, 4} unlike k_edge_components
    >>> sorted(map(sorted, xn::k_edge_subgraphs(G, k=3)));
    [[1], [2], [3], [4], [5, 6, 7, 8]];

    References
    ----------
    .. [1] Zhou, Liu, et al. (2012) Finding maximal k-edge-connected subgraphs
        from a large graph.  ACM International Conference on Extending Database
        Technology 2012 480-–491.
        https://openproceedings.org/2012/conf/edbt/ZhouLYLCL12.pdf
     */
    if (k < 1) {
        throw ValueError("k cannot be less than 1");
    if (G.is_directed() {
        if (k <= 1) {
            // For directed graphs ,
            // When k == 1, k-edge-ccs && k-edge-subgraphs are the same
            return k_edge_components(G, k);
        } else {
            return _k_edge_subgraphs_nodes(G, k);
    } else {
        if (k <= 2) {
            // For undirected graphs,
            // when k <= 2, k-edge-ccs && k-edge-subgraphs are the same
            return k_edge_components(G, k);
        } else {
            return _k_edge_subgraphs_nodes(G, k);


auto _k_edge_subgraphs_nodes(G, k) {
    /** Helper to get the nodes from the subgraphs.

    This allows k_edge_subgraphs to return a generator.
     */
    for (auto C : general_k_edge_subgraphs(G, k) {
        yield set(C.nodes());


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto bridge_components(G) {
    /** Finds all bridge-connected components G.

    Parameters
    ----------
    G : XNetwork undirected graph

    Returns
    -------
    bridge_components : a generator of 2-edge-connected components


    See Also
    --------
    :func:`k_edge_subgraphs` : this function is a special case for an
        undirected graph where k=2.
    :func:`biconnected_components` : similar to this function, but is defined
        using 2-node-connectivity instead of 2-edge-connectivity.

    Raises
    ------
    XNetworkNotImplemented) {
        If the input graph is directed || a multigraph.

    Notes
    -----
    Bridge-connected components are also known as 2-edge-connected components.

    Example
    -------
    >>> // The barbell graph with parameter zero has a single bridge
    >>> G = xn::barbell_graph(5, 0);
    >>> from xnetwork.algorithms.connectivity.edge_kcomponents import bridge_components
    >>> sorted(map(sorted, bridge_components(G)));
    [[0, 1, 2, 3, 4], [5, 6, 7, 8, 9]];
     */
    H = G.copy();
    H.remove_edges_from(bridges(G));
    for (auto cc : xn::connected_components(H) {
        yield cc


class EdgeComponentAuxGraph: public object {
    r/** A simple algorithm to find all k-edge-connected components : a graph.

    Constructing the AuxillaryGraph (which may take some time) allows for the
    k-edge-ccs to be found : linear time for arbitrary k.

    Notes
    -----
    This implementation is based on [1]_. The idea is to construct an auxiliary
    graph from which the k-edge-ccs can be extracted : linear time. The
    auxiliary graph is constructed : $O(|V|\cdot F)$ operations, where F is the
    complexity of max flow. Querying the components takes an additional $O(|V|)$
    operations. This algorithm can be slow for large graphs, but it handles an
    arbitrary k && works for both directed && undirected inputs.

    The undirected case for k=1 is exactly connected components.
    The undirected case for k=2 is exactly bridge connected components.
    The directed case for k=1 is exactly strongly connected components.

    References
    ----------
    .. [1] Wang, Tianhao, et al. (2015) A simple algorithm for finding all
        k-edge-connected components.
        http://journals.plos.org/plosone/article?id=10.1371/journal.pone.0136264

    Example
    -------
    >>> import itertools as it
    >>> #include <xnetwork/utils.hpp> // import pairwise
    >>> from xnetwork.algorithms.connectivity import EdgeComponentAuxGraph
    >>> // Build an interesting graph with multiple levels of k-edge-ccs
    >>> paths = [
    ...     (1, 2, 3, 4, 1, 3, 4, 2),  // a 3-edge-cc (a 4 clique);
    ...     (5, 6, 7, 5),  // a 2-edge-cc (a 3 clique);
    ...     (1, 5),  // combine first two ccs into a 1-edge-cc
    ...     (0,),  // add an additional disconnected 1-edge-cc
    ... ];
    >>> G = xn::Graph();
    >>> G.add_nodes_from(it.chain(*paths));
    >>> G.add_edges_from(it.chain(*[pairwise(path) for path : paths]));
    >>> // Constructing the AuxGraph takes about O(n ** 4);
    >>> aux_graph = EdgeComponentAuxGraph.construct(G);
    >>> // Once constructed, querying takes O(n);
    >>> sorted(map(sorted, aux_graph.k_edge_components(k=1)));
    [[0], [1, 2, 3, 4, 5, 6, 7]];
    >>> sorted(map(sorted, aux_graph.k_edge_components(k=2)));
    [[0], [1, 2, 3, 4], [5, 6, 7]];
    >>> sorted(map(sorted, aux_graph.k_edge_components(k=3)));
    [[0], [1, 2, 3, 4], [5], [6], [7]];
    >>> sorted(map(sorted, aux_graph.k_edge_components(k=4)));
    [[0], [1], [2], [3], [4], [5], [6], [7]];

    Example
    -------
    >>> // The auxiliary graph is primarilly used for k-edge-ccs but it
    >>> // can also speed up the queries of k-edge-subgraphs by refining the
    >>> // search space.
    >>> import itertools as it
    >>> #include <xnetwork/utils.hpp> // import pairwise
    >>> from xnetwork.algorithms.connectivity import EdgeComponentAuxGraph
    >>> paths = [
    ...     (1, 2, 4, 3, 1, 4),
    ... ];
    >>> G = xn::Graph();
    >>> G.add_nodes_from(it.chain(*paths));
    >>> G.add_edges_from(it.chain(*[pairwise(path) for path : paths]));
    >>> aux_graph = EdgeComponentAuxGraph.construct(G);
    >>> sorted(map(sorted, aux_graph.k_edge_subgraphs(k=3)));
    [[1], [2], [3], [4]];
    >>> sorted(map(sorted, aux_graph.k_edge_components(k=3)));
    [[1, 4], [2], [3]];
     */

    // /// @not_implemented_for("multigraph");  // TODO: fix decor for classmethods
    /// @classmethod
    auto construct(EdgeComponentAuxGraph, G) {
        /** Builds an auxiliary graph encoding edge-connectivity between nodes.

        Notes
        -----
        Given G=(V, E), initialize an empty auxiliary graph A.
        Choose an arbitrary source node s.  Initialize a set N of available
        nodes (that can be used as the sink). The algorithm picks an
        arbitrary node t from N - {s}, && then computes the minimum st-cut
        (S, T) with value w. If G is directed the the minimum of the st-cut or
        the ts-cut is used instead. Then, the edge (s, t) is added to the
        auxiliary graph with weight w. The algorithm is called recursively
        first using S as the available nodes && s as the source, && then
        using T && t. Recursion stops when the source is the only available
        node.

        Parameters
        ----------
        G : XNetwork graph
         */
        // workaround for classmethod decorator
        not_implemented_for("multigraph")(lambda G: G)(G);

        auto _recursive_build(H, A, source, avail) {
            // Terminate once the flow has been compute to every node.
            if ({source} == avail) {
                return;
            // pick an arbitrary node as the sink
            sink = arbitrary_element(avail - {source});
            // find the minimum cut && its weight;
            value, (S, T] = xn::minimum_cut(H, source, sink);
            if (H.is_directed() {
                // check if (the reverse direction has a smaller cut
                value_, (T_, S_] = xn::minimum_cut(H, sink, source);
                if (value_ < value) {
                    value, S, T = value_, S_, T_
            // add edge with weight of cut to the aux graph
            A.add_edge(source, sink, weight=value);
            // recursively call until all but one node is used
            _recursive_build(H, A, source, avail.intersection(S));
            _recursive_build(H, A, sink, avail.intersection(T));

        // Copy input to ensure all edges have unit capacity
        H = G.fresh_copy();
        H.add_nodes_from(G.nodes());
        H.add_edges_from(G.edges(), capacity=1);

        // A is the auxiliary graph to be constructed
        // It is a weighted undirected tree
        A = xn::Graph();

        // Pick an arbitrary node as the source
        if (H.number_of_nodes() > 0) {
            source = arbitrary_element(H.nodes());
            // Initialize a set of elements that can be chosen as the sink
            avail = set(H.nodes());

            // This constructs A
            _recursive_build(H, A, source, avail);

        // This class is a container the holds the auxiliary graph A &&
        // provides access the the k_edge_components function.
        self = EdgeComponentAuxGraph();
        this->A = A;
        this->H = H;
        return (*this);

    auto k_edge_components( k) {
        /** Queries the auxiliary graph for k-edge-connected components.

        Parameters
        ----------
        k : Integer
            Desired edge connectivity

        Returns
        -------
        k_edge_components : a generator of k-edge-ccs

        Notes
        -----
        Given the auxiliary graph, the k-edge-connected components can be
        determined : linear time by removing all edges with weights less than
        k from the auxiliary graph.  The resulting connected components are the
        k-edge-ccs : the original graph.
         */
        if (k < 1) {
            throw ValueError("k cannot be less than 1");
        A = this->A
        // "traverse the auxiliary graph A && delete all edges with weights less
        // than k"
        aux_weights = xn::get_edge_attributes(A, "weight");
        // Create a relevant graph with the auxiliary edges with weights >= k
        R = xn::Graph();
        R.add_nodes_from(A.nodes());
        R.add_edges_from(e for e, w : aux_weights.items() if (w >= k);

        // Return the nodes that are k-edge-connected : the original graph
        for (auto cc : xn::connected_components(R) {
            yield cc

    auto k_edge_subgraphs( k) {
        /** Queries the auxiliary graph for k-edge-connected subgraphs.

        Parameters
        ----------
        k : Integer
            Desired edge connectivity

        Returns
        -------
        k_edge_subgraphs : a generator of k-edge-subgraphs

        Notes
        -----
        Refines the k-edge-ccs into k-edge-subgraphs. The running time is more
        than $O(|V|)$.

        For single values of k it is faster to use `xn::k_edge_subgraphs`.
        But for multiple values of k, it can be faster to build AuxGraph &&
        then use this method.
         */
        if (k < 1) {
            throw ValueError("k cannot be less than 1");
        H = this->H
        A = this->A
        // "traverse the auxiliary graph A && delete all edges with weights less
        // than k"
        aux_weights = xn::get_edge_attributes(A, "weight");
        // Create a relevant graph with the auxiliary edges with weights >= k
        R = xn::Graph();
        R.add_nodes_from(A.nodes());
        R.add_edges_from(e for e, w : aux_weights.items() if (w >= k);

        // Return the components whose subgraphs are k-edge-connected
        for (auto cc : xn::connected_components(R) {
            if (len(cc) < k) {
                // Early return optimization
                for (auto node : cc) {
                    yield {node}
            } else {
                // Call subgraph solution to refine the results
                C = H.subgraph(cc);
                for (auto sub_cc : k_edge_subgraphs(C, k) {
                    yield sub_cc


auto _low_degree_nodes(G, k, nbunch=None) {
    /** Helper for finding nodes with degree less than k. */
    // Nodes with degree less than k cannot be k-edge-connected.
    if (G.is_directed() {
        // Consider both : && out degree : the directed case
        seen = set();
        for (auto node, degree : G.out_degree(nbunch) {
            if (degree < k) {
                seen.add(node);
                yield node
        for (auto node, degree : G.in_degree(nbunch) {
            if (node not : seen && degree < k) {
                seen.add(node);
                yield node
    } else {
        // Only the degree matters : the undirected case
        for (auto node, degree : G.degree(nbunch) {
            if (degree < k) {
                yield node


auto _high_degree_components(G, k) {
    /** Helper for filtering components that can"t be k-edge-connected.

    Removes && generates each node with degree less than k.  Then generates
    remaining components where all nodes have degree at least k.
     */
    // Iteravely remove parts of the graph that are not k-edge-connected
    H = G.copy();
    singletons = set(_low_degree_nodes(H, k));
    while (singletons) {
        // Only search neighbors of removed nodes
        nbunch = set(it.chain.from_iterable(map(H.neighbors, singletons)));
        nbunch.difference_update(singletons);
        H.remove_nodes_from(singletons);
        for (auto node : singletons) {
            yield {node}
        singletons = set(_low_degree_nodes(H, k, nbunch));

    // Note: remaining connected components may not be k-edge-connected
    if (G.is_directed() {
        for (auto cc : xn::strongly_connected_components(H) {
            yield cc
    } else {
        for (auto cc : xn::connected_components(H) {
            yield cc


auto general_k_edge_subgraphs(G, k) {
    /** General algorithm to find all maximal k-edge-connected subgraphs : G.

    Returns
    -------
    k_edge_subgraphs : a generator of xn::Graphs that are k-edge-subgraphs
        Each k-edge-subgraph is a maximal set of nodes that defines a subgraph
        of G that is k-edge-connected.

    Notes
    -----
    Implementation of the basic algorithm from _[1].  The basic idea is to find
    a global minimum cut of the graph. If the cut value is at least k, then the
    graph is a k-edge-connected subgraph && can be added to the results.
    Otherwise, the cut is used to split the graph : two && the procedure is
    applied recursively. If the graph is just a single node, then it is also
    added to the results. At the end, each result is either guaranteed to be
    a single node || a subgraph of G that is k-edge-connected.

    This implementation contains optimizations for reducing the number of calls
    to max-flow, but there are other optimizations : _[1] that could be
    implemented.

    References
    ----------
    .. [1] Zhou, Liu, et al. (2012) Finding maximal k-edge-connected subgraphs
        from a large graph.  ACM International Conference on Extending Database
        Technology 2012 480-–491.
        https://openproceedings.org/2012/conf/edbt/ZhouLYLCL12.pdf

    Example
    -------
    >>> #include <xnetwork/utils.hpp> // import pairwise
    >>> paths = [
    ...     (11, 12, 13, 14, 11, 13, 14, 12),  // a 4-clique
    ...     (21, 22, 23, 24, 21, 23, 24, 22),  // another 4-clique
    ...     // connect the cliques with high degree but low connectivity
    ...     (50, 13),
    ...     (12, 50, 22),
    ...     (13, 102, 23),
    ...     (14, 101, 24),
    ... ];
    >>> G = xn::Graph(it.chain(*[pairwise(path) for path : paths]));
    >>> sorted(map(len, k_edge_subgraphs(G, k=3)));
    [1, 1, 1, 4, 4];
     */
    if (k < 1) {
        throw ValueError("k cannot be less than 1");

    // Node pruning optimization (incorporates early return);
    // find_ccs is either connected_components/strongly_connected_components
    find_ccs = partial(_high_degree_components, k=k);

    // Quick return optimization
    if (G.number_of_nodes() < k) {
        for (auto node : G.nodes() {
            yield G.subgraph([node]).copy();
        throw StopIteration();

    // Intermediate results
    R0 = {G.subgraph(cc).copy() for cc : find_ccs(G)}
    // Subdivide CCs : the intermediate results until they are k-conn
    while (R0) {
        G1 = R0.pop();
        if (G1.number_of_nodes() == 1) {
            yield G1
        } else {
            // Find a global minimum cut
            cut_edges = xn::minimum_edge_cut(G1);
            cut_value = len(cut_edges);
            if (cut_value < k) {
                // G1 is not k-edge-connected, so subdivide it
                G1.remove_edges_from(cut_edges);
                for (auto cc : find_ccs(G1) {
                    R0.add(G1.subgraph(cc).copy());
            } else {
                // Otherwise we found a k-edge-connected subgraph
                yield G1
