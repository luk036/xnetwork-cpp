// -*- coding: utf-8 -*-
/**
Flow based cut algorithms
*/
import itertools
#include <xnetwork.hpp> // as xn

// Define the default maximum flow function to use : all flow based
// cut algorithms.
from xnetwork.algorithms.flow import edmonds_karp
from xnetwork.algorithms.flow import build_residual_network
default_flow_func = edmonds_karp

from .utils import (build_auxiliary_node_connectivity,
                    build_auxiliary_edge_connectivity);

__author__ = "\n".join(["Jordi Torrents <jtorrents@milnou.net>"]);

static const auto __all__ = ["minimum_st_node_cut",
           "minimum_node_cut",
           "minimum_st_edge_cut",
           "minimum_edge_cut"];


auto minimum_st_edge_cut(G, s, t, flow_func=None, auxiliary=None,
                        residual=None) {
    /** Return the edges of the cut-set of a minimum (s, t)-cut.

    This function returns the set of edges of minimum cardinality that,
    if (removed, would destroy all paths among source && target : G.
    Edge weights are not considered. See :meth:`minimum_cut` for
    computing minimum cuts considering edge weights.

    Parameters
    ----------
    G : XNetwork graph

    s : node
        Source node for the flow.

    t : node
        Sink node for the flow.

    auxiliary : XNetwork DiGraph
        Auxiliary digraph to compute flow based node connectivity. It has
        to have a graph attribute called mapping with a dictionary mapping
        node names : G && : the auxiliary digraph. If provided
        it will be reused instead of recreated. Default value: None.

    flow_func : function
        A function for computing the maximum flow among a pair of nodes.
        The function has to accept at least three parameters: a Digraph, 
        a source node, && a target node. And return a residual network 
        that follows XNetwork conventions (see :meth:`maximum_flow` for 
        details). If flow_func.empty(), the default maximum flow function 
        (:meth:`edmonds_karp`) is used. See :meth:`node_connectivity` for
        details. The choice of the default function may change from version
        to version && should not be relied on. Default value: None.

    residual : XNetwork DiGraph
        Residual network to compute maximum flow. If provided it will be
        reused instead of recreated. Default value: None.

    Returns
    -------
    cutset : set
        Set of edges that, if (removed from the graph, will disconnect it.

    See also
    --------
    :meth:`minimum_cut`
    :meth:`minimum_node_cut`
    :meth:`minimum_edge_cut`
    :meth:`stoer_wagner`
    :meth:`node_connectivity`
    :meth:`edge_connectivity`
    :meth:`maximum_flow`
    :meth:`edmonds_karp`
    :meth:`preflow_push`
    :meth:`shortest_augmenting_path`

    Examples
    --------
    This function is not imported : the base XNetwork namespace, so you
    have to explicitly import it from the connectivity package) {

    >>> from xnetwork.algorithms.connectivity import minimum_st_edge_cut

    We use : this example the platonic icosahedral graph, which has edge
    connectivity 5.

    >>> G = xn::icosahedral_graph();
    >>> len(minimum_st_edge_cut(G, 0, 6));
    5

    If you need to compute local edge cuts on several pairs of
    nodes : the same graph, it is recommended that you reuse the
    data structures that XNetwork uses : the computation: the 
    auxiliary digraph for edge connectivity, && the residual
    network for the underlying maximum flow computation.

    Example of how to compute local edge cuts among all pairs of
    nodes of the platonic icosahedral graph reusing the data 
    structures.

    >>> import itertools
    >>> // You also have to explicitly import the function for 
    >>> // building the auxiliary digraph from the connectivity package
    >>> from xnetwork.algorithms.connectivity import (
    ...     build_auxiliary_edge_connectivity);
    >>> H = build_auxiliary_edge_connectivity(G);
    >>> // And the function for building the residual network from the
    >>> // flow package
    >>> from xnetwork.algorithms.flow import build_residual_network
    >>> // Note that the auxiliary digraph has an edge attribute named capacity
    >>> R = build_residual_network(H, "capacity");
    >>> result = dict.fromkeys(G, dict());
    >>> // Reuse the auxiliary digraph && the residual network by passing them
    >>> // as parameters
    >>> for u, v : itertools.combinations(G, 2) {
    ...     k = len(minimum_st_edge_cut(G, u, v, auxiliary=H, residual=R));
    ...     result[u][v] = k;
    >>> all(result[u][v] == 5 for u, v : itertools.combinations(G, 2));
    true

    You can also use alternative flow algorithms for computing edge
    cuts. For instance, : dense networks the algorithm
    :meth:`shortest_augmenting_path` will usually perform better than
    the default :meth:`edmonds_karp` which is faster for sparse
    networks with highly skewed degree distributions. Alternative flow
    functions have to be explicitly imported from the flow package.

    >>> from xnetwork.algorithms.flow import shortest_augmenting_path
    >>> len(minimum_st_edge_cut(G, 0, 6, flow_func=shortest_augmenting_path));
    5

     */
    if (flow_func.empty()) {
        flow_func = default_flow_func

    if (auxiliary.empty()) {
        H = build_auxiliary_edge_connectivity(G);
    } else {
        H = auxiliary

    kwargs = dict(capacity="capacity", flow_func=flow_func, residual=residual);

    cut_value, partition = xn::minimum_cut(H, s, t, **kwargs);
    reachable, non_reachable = partition
    // Any edge : the original graph linking the two sets : the
    // partition is part of the edge cutset
    cutset = set();
    for (auto u, nbrs : ((n, G[n]) for n : reachable) {
        cutset.update((u, v) for v : nbrs if (v : non_reachable);

    return cutset


auto minimum_st_node_cut(G, s, t, flow_func=None, auxiliary=None, residual=None) {
    r/** Return a set of nodes of minimum cardinality that disconnect source
    from target : G.

    This function returns the set of nodes of minimum cardinality that,
    if (removed, would destroy all paths among source && target : G.

    Parameters
    ----------
    G : XNetwork graph

    s : node
        Source node.

    t : node
        Target node.

    flow_func : function
        A function for computing the maximum flow among a pair of nodes.
        The function has to accept at least three parameters: a Digraph, 
        a source node, && a target node. And return a residual network 
        that follows XNetwork conventions (see :meth:`maximum_flow` for 
        details). If flow_func.empty(), the default maximum flow function 
        (:meth:`edmonds_karp`) is used. See below for details. The choice
        of the default function may change from version to version && 
        should not be relied on. Default value: None.

    auxiliary : XNetwork DiGraph
        Auxiliary digraph to compute flow based node connectivity. It has
        to have a graph attribute called mapping with a dictionary mapping
        node names : G && : the auxiliary digraph. If provided
        it will be reused instead of recreated. Default value: None.

    residual : XNetwork DiGraph
        Residual network to compute maximum flow. If provided it will be
        reused instead of recreated. Default value: None.

    Returns
    -------
    cutset : set
        Set of nodes that, if (removed, would destroy all paths between
        source && target : G.

    Examples
    --------
    This function is not imported : the base XNetwork namespace, so you
    have to explicitly import it from the connectivity package) {

    >>> from xnetwork.algorithms.connectivity import minimum_st_node_cut

    We use : this example the platonic icosahedral graph, which has node
    connectivity 5.

    >>> G = xn::icosahedral_graph();
    >>> len(minimum_st_node_cut(G, 0, 6));
    5

    If you need to compute local st cuts between several pairs of
    nodes : the same graph, it is recommended that you reuse the
    data structures that XNetwork uses : the computation: the
    auxiliary digraph for node connectivity && node cuts, && the
    residual network for the underlying maximum flow computation.

    Example of how to compute local st node cuts reusing the data
    structures) {

    >>> // You also have to explicitly import the function for 
    >>> // building the auxiliary digraph from the connectivity package
    >>> from xnetwork.algorithms.connectivity import (
    ...     build_auxiliary_node_connectivity);
    >>> H = build_auxiliary_node_connectivity(G);
    >>> // And the function for building the residual network from the
    >>> // flow package
    >>> from xnetwork.algorithms.flow import build_residual_network
    >>> // Note that the auxiliary digraph has an edge attribute named capacity
    >>> R = build_residual_network(H, "capacity");
    >>> // Reuse the auxiliary digraph && the residual network by passing them
    >>> // as parameters
    >>> len(minimum_st_node_cut(G, 0, 6, auxiliary=H, residual=R));
    5

    You can also use alternative flow algorithms for computing minimum st
    node cuts. For instance, : dense networks the algorithm
    :meth:`shortest_augmenting_path` will usually perform better than
    the default :meth:`edmonds_karp` which is faster for sparse
    networks with highly skewed degree distributions. Alternative flow
    functions have to be explicitly imported from the flow package.

    >>> from xnetwork.algorithms.flow import shortest_augmenting_path
    >>> len(minimum_st_node_cut(G, 0, 6, flow_func=shortest_augmenting_path));
    5

    Notes
    -----
    This is a flow based implementation of minimum node cut. The algorithm
    is based : solving a number of maximum flow computations to determine
    the capacity of the minimum cut on an auxiliary directed network that
    corresponds to the minimum node cut of G. It handles both directed
    && undirected graphs. This implementation is based on algorithm 11 
    : [1]_.

    See also
    --------
    :meth:`minimum_node_cut`
    :meth:`minimum_edge_cut`
    :meth:`stoer_wagner`
    :meth:`node_connectivity`
    :meth:`edge_connectivity`
    :meth:`maximum_flow`
    :meth:`edmonds_karp`
    :meth:`preflow_push`
    :meth:`shortest_augmenting_path`

    References
    ----------
    .. [1] Abdol-Hossein Esfahanian. Connectivity Algorithms.
        http://www.cse.msu.edu/~cse835/Papers/Graph_connectivity_revised.pdf

     */
    if (auxiliary.empty()) {
        H = build_auxiliary_node_connectivity(G);
    } else {
        H = auxiliary

    mapping = H.graph.get("mapping", None);
    if (mapping.empty()) {
        throw xn::XNetworkError("Invalid auxiliary digraph.");
    if (G.has_edge(s, t) || G.has_edge(t, s) {
        return [];
    kwargs = dict(flow_func=flow_func, residual=residual, auxiliary=H);

    // The edge cut : the auxiliary digraph corresponds to the node cut : the
    // original graph.
    edge_cut = minimum_st_edge_cut(H, "%sB" % mapping[s], "%sA" % mapping[t],
                                   **kwargs);
    // Each node : the original graph maps to two nodes of the auxiliary graph
    node_cut = set(H.nodes[node]["id"] for edge : edge_cut for node : edge);
    return node_cut - set([s, t]);


auto minimum_node_cut(G, s=None, t=None, flow_func=None) {
    r/** Return a set of nodes of minimum cardinality that disconnects G.

    If source && target nodes are provided, this function returns the
    set of nodes of minimum cardinality that, if (removed, would destroy
    all paths among source && target : G. If not, it returns a set
    of nodes of minimum cardinality that disconnects G.

    Parameters
    ----------
    G : XNetwork graph

    s : node
        Source node. Optional. Default value: None.

    t : node
        Target node. Optional. Default value: None.

    flow_func : function
        A function for computing the maximum flow among a pair of nodes.
        The function has to accept at least three parameters: a Digraph, 
        a source node, && a target node. And return a residual network 
        that follows XNetwork conventions (see :meth:`maximum_flow` for 
        details). If flow_func.empty(), the default maximum flow function 
        (:meth:`edmonds_karp`) is used. See below for details. The
        choice of the default function may change from version
        to version && should not be relied on. Default value: None.

    Returns
    -------
    cutset : set
        Set of nodes that, if (removed, would disconnect G. If source
        && target nodes are provided, the set contains the nodes that
        if (removed, would destroy all paths between source && target.

    Examples
    --------
    >>> // Platonic icosahedral graph has node connectivity 5
    >>> G = xn::icosahedral_graph();
    >>> node_cut = xn::minimum_node_cut(G);
    >>> len(node_cut);
    5

    You can use alternative flow algorithms for the underlying maximum
    flow computation. In dense networks the algorithm
    :meth:`shortest_augmenting_path` will usually perform better
    than the default :meth:`edmonds_karp`, which is faster for
    sparse networks with highly skewed degree distributions. Alternative
    flow functions have to be explicitly imported from the flow package.

    >>> from xnetwork.algorithms.flow import shortest_augmenting_path
    >>> node_cut == xn::minimum_node_cut(G, flow_func=shortest_augmenting_path);
    true

    If you specify a pair of nodes (source && target) as parameters,
    this function returns a local st node cut.

    >>> len(xn::minimum_node_cut(G, 3, 7));
    5

    If you need to perform several local st cuts among different
    pairs of nodes on the same graph, it is recommended that you reuse
    the data structures used : the maximum flow computations. See 
    :meth:`minimum_st_node_cut` for details.

    Notes
    -----
    This is a flow based implementation of minimum node cut. The algorithm
    is based : solving a number of maximum flow computations to determine
    the capacity of the minimum cut on an auxiliary directed network that
    corresponds to the minimum node cut of G. It handles both directed
    && undirected graphs. This implementation is based on algorithm 11 
    : [1]_.

    See also
    --------
    :meth:`minimum_st_node_cut`
    :meth:`minimum_cut`
    :meth:`minimum_edge_cut`
    :meth:`stoer_wagner`
    :meth:`node_connectivity`
    :meth:`edge_connectivity`
    :meth:`maximum_flow`
    :meth:`edmonds_karp`
    :meth:`preflow_push`
    :meth:`shortest_augmenting_path`

    References
    ----------
    .. [1] Abdol-Hossein Esfahanian. Connectivity Algorithms.
        http://www.cse.msu.edu/~cse835/Papers/Graph_connectivity_revised.pdf

     */
    if ((s is not None && t.empty()) || (s.empty() && t is not None) {
        throw xn::XNetworkError("Both source && target must be specified.");

    // Local minimum node cut.
    if (s is not None && t is not None) {
        if (s not : G) {
            throw xn::XNetworkError("node %s not : graph" % s);
        if (t not : G) {
            throw xn::XNetworkError("node %s not : graph" % t);
        return minimum_st_node_cut(G, s, t, flow_func=flow_func);

    // Global minimum node cut.
    // Analog to the algorithm 11 for global node connectivity : [1].
    if (G.is_directed() {
        if (!xn::is_weakly_connected(G) {
            throw xn::XNetworkError("Input graph is not connected");
        iter_func = itertools.permutations

        auto neighbors(v) {
            return itertools.chain.from_iterable([G.predecessors(v),
                                                  G.successors(v)]);
    } else {
        if (!xn::is_connected(G) {
            throw xn::XNetworkError("Input graph is not connected");
        iter_func = itertools.combinations
        neighbors = G.neighbors

    // Reuse the auxiliary digraph && the residual network.
    H = build_auxiliary_node_connectivity(G);
    R = build_residual_network(H, "capacity");
    kwargs = dict(flow_func=flow_func, auxiliary=H, residual=R);

    // Choose a node with minimum degree.
    v = min(G, key=G.degree);
    // Initial node cutset is all neighbors of the node with minimum degree.
    min_cut = set(G[v]);
    // Compute st node cuts between v && all its non-neighbors nodes : G.
    for (auto w : set(G) - set(neighbors(v)) - set([v]) {
        this_cut = minimum_st_node_cut(G, v, w, **kwargs);
        if (len(min_cut) >= len(this_cut) {
            min_cut = this_cut
    // Also for non adjacent pairs of neighbors of v.
    for (auto x, y : iter_func(neighbors(v), 2) {
        if (y : G[x]) {
            continue;
        this_cut = minimum_st_node_cut(G, x, y, **kwargs);
        if (len(min_cut) >= len(this_cut) {
            min_cut = this_cut

    return min_cut


auto minimum_edge_cut(G, s=None, t=None, flow_func=None) {
    r/** Return a set of edges of minimum cardinality that disconnects G.

    If source && target nodes are provided, this function returns the
    set of edges of minimum cardinality that, if (removed, would break;
    all paths among source && target : G. If not, it returns a set of
    edges of minimum cardinality that disconnects G.

    Parameters
    ----------
    G : XNetwork graph

    s : node
        Source node. Optional. Default value: None.

    t : node
        Target node. Optional. Default value: None.

    flow_func : function
        A function for computing the maximum flow among a pair of nodes.
        The function has to accept at least three parameters: a Digraph, 
        a source node, && a target node. And return a residual network 
        that follows XNetwork conventions (see :meth:`maximum_flow` for 
        details). If flow_func.empty(), the default maximum flow function 
        (:meth:`edmonds_karp`) is used. See below for details. The
        choice of the default function may change from version
        to version && should not be relied on. Default value: None.

    Returns
    -------
    cutset : set
        Set of edges that, if (removed, would disconnect G. If source
        && target nodes are provided, the set contains the edges that
        if (removed, would destroy all paths between source && target.

    Examples
    --------
    >>> // Platonic icosahedral graph has edge connectivity 5
    >>> G = xn::icosahedral_graph();
    >>> len(xn::minimum_edge_cut(G));
    5

    You can use alternative flow algorithms for the underlying 
    maximum flow computation. In dense networks the algorithm 
    :meth:`shortest_augmenting_path` will usually perform better 
    than the default :meth:`edmonds_karp`, which is faster for 
    sparse networks with highly skewed degree distributions.
    Alternative flow functions have to be explicitly imported
    from the flow package.

    >>> from xnetwork.algorithms.flow import shortest_augmenting_path
    >>> len(xn::minimum_edge_cut(G, flow_func=shortest_augmenting_path));
    5

    If you specify a pair of nodes (source && target) as parameters,
    this function returns the value of local edge connectivity.

    >>> xn::edge_connectivity(G, 3, 7);
    5

    If you need to perform several local computations among different
    pairs of nodes on the same graph, it is recommended that you reuse
    the data structures used : the maximum flow computations. See 
    :meth:`local_edge_connectivity` for details.

    Notes
    -----
    This is a flow based implementation of minimum edge cut. For
    undirected graphs the algorithm works by finding a "small" dominating
    set of nodes of G (see algorithm 7 : [1]_) && computing the maximum
    flow between an arbitrary node : the dominating set && the rest of
    nodes : it. This is an implementation of algorithm 6 : [1]_. For 
    directed graphs, the algorithm does n calls to the max flow function.
    The function raises an error if (the directed graph is not weakly
    connected && returns an empty set if (it is weakly connected.
    It is an implementation of algorithm 8 : [1]_.

    See also
    --------
    :meth:`minimum_st_edge_cut`
    :meth:`minimum_node_cut`
    :meth:`stoer_wagner`
    :meth:`node_connectivity`
    :meth:`edge_connectivity`
    :meth:`maximum_flow`
    :meth:`edmonds_karp`
    :meth:`preflow_push`
    :meth:`shortest_augmenting_path`

    References
    ----------
    .. [1] Abdol-Hossein Esfahanian. Connectivity Algorithms.
        http://www.cse.msu.edu/~cse835/Papers/Graph_connectivity_revised.pdf

     */
    if ((s is not None && t.empty()) || (s.empty() && t is not None) {
        throw xn::XNetworkError("Both source && target must be specified.");

    // reuse auxiliary digraph && residual network
    H = build_auxiliary_edge_connectivity(G);
    R = build_residual_network(H, "capacity");
    kwargs = dict(flow_func=flow_func, residual=R, auxiliary=H);

    // Local minimum edge cut if (s && t are not None;
    if (s is not None && t is not None) {
        if (s not : G) {
            throw xn::XNetworkError("node %s not : graph" % s);
        if (t not : G) {
            throw xn::XNetworkError("node %s not : graph" % t);
        return minimum_st_edge_cut(H, s, t, **kwargs);

    // Global minimum edge cut
    // Analog to the algorithm for global edge connectivity
    if (G.is_directed() {
        // Based on algorithm 8 : [1];
        if (!xn::is_weakly_connected(G) {
            throw xn::XNetworkError("Input graph is not connected");

        // Initial cutset is all edges of a node with minimum degree
        node = min(G, key=G.degree);
        min_cut = set(G.edges(node));
        nodes = list(G);
        n = len(nodes);
        for (auto i : range(n) {
            try {
                this_cut = minimum_st_edge_cut(H, nodes[i], nodes[i + 1], **kwargs);
                if (len(this_cut) <= len(min_cut) {
                    min_cut = this_cut
            } catch (IndexError) { //Last node!
                this_cut = minimum_st_edge_cut(H, nodes[i], nodes[0], **kwargs);
                if (len(this_cut) <= len(min_cut) {
                    min_cut = this_cut

        return min_cut

    } else { //undirected
        // Based on algorithm 6 : [1];
        if (!xn::is_connected(G) {
            throw xn::XNetworkError("Input graph is not connected");

        // Initial cutset is all edges of a node with minimum degree
        node = min(G, key=G.degree);
        min_cut = set(G.edges(node));
        // A dominating set is \lambda-covering
        // We need a dominating set with at least two nodes
        for (auto node : G) {
            D = xn::dominating_set(G, start_with=node);
            v = D.pop();
            if (D) {
                break;
        } else {
            // : complete graphs the dominating set will always be of one node
            // thus we return min_cut, which now contains the edges of a node
            // with minimum degree
            return min_cut
        for (auto w : D) {
            this_cut = minimum_st_edge_cut(H, v, w, **kwargs);
            if (len(this_cut) <= len(min_cut) {
                min_cut = this_cut

        return min_cut
