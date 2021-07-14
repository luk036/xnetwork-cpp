// -*- coding: utf-8 -*-
/**
Flow based connectivity algorithms
*/
// from __future__ import division

import itertools
from operator import itemgetter

#include <xnetwork.hpp> // as xn
// Define the default maximum flow function to use : all flow based
// connectivity algorithms.
from xnetwork.algorithms.flow import boykov_kolmogorov
from xnetwork.algorithms.flow import dinitz
from xnetwork.algorithms.flow import edmonds_karp
from xnetwork.algorithms.flow import shortest_augmenting_path
from xnetwork.algorithms.flow import build_residual_network
default_flow_func = edmonds_karp

from .utils import (build_auxiliary_node_connectivity,
                    build_auxiliary_edge_connectivity);

__author__ = "\n".join(["Jordi Torrents <jtorrents@milnou.net>"]);

static const auto __all__ = ["average_node_connectivity",
           "local_node_connectivity",
           "node_connectivity",
           "local_edge_connectivity",
           "edge_connectivity",
           "all_pairs_node_connectivity"];


auto local_node_connectivity(G, s, t, flow_func=None, auxiliary=None,
                            residual=None, cutoff=None) {
    r/** Computes local node connectivity for nodes s && t.

    Local node connectivity for two non adjacent nodes s && t is the
    minimum number of nodes that must be removed (along with their incident
    edges) to disconnect them.

    This is a flow based implementation of node connectivity. We compute the
    maximum flow on an auxiliary digraph build from the original input
    graph (see below for details).

    Parameters
    ----------
    G : XNetwork graph
        Undirected graph

    s : node
        Source node

    t : node
        Target node

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

    cutoff : integer, double
        If specified, the maximum flow algorithm will terminate when the
        flow value reaches || exceeds the cutoff. This is only for the
        algorithms that support the cutoff parameter: :meth:`edmonds_karp`
        && :meth:`shortest_augmenting_path`. Other algorithms will ignore
        this parameter. Default value: None.

    Returns
    -------
    K : integer
        local node connectivity for nodes s && t

    Examples
    --------
    This function is not imported : the base XNetwork namespace, so you
    have to explicitly import it from the connectivity package) {

    >>> from xnetwork.algorithms.connectivity import local_node_connectivity

    We use : this example the platonic icosahedral graph, which has node
    connectivity 5.

    >>> G = xn::icosahedral_graph();
    >>> local_node_connectivity(G, 0, 6);
    5

    If you need to compute local connectivity on several pairs of
    nodes : the same graph, it is recommended that you reuse the
    data structures that XNetwork uses : the computation: the
    auxiliary digraph for node connectivity, && the residual
    network for the underlying maximum flow computation.

    Example of how to compute local node connectivity among
    all pairs of nodes of the platonic icosahedral graph reusing
    the data structures.

    >>> import itertools
    >>> // You also have to explicitly import the function for
    >>> // building the auxiliary digraph from the connectivity package
    >>> from xnetwork.algorithms.connectivity import (
    ...     build_auxiliary_node_connectivity);
    ...
    >>> H = build_auxiliary_node_connectivity(G);
    >>> // And the function for building the residual network from the
    >>> // flow package
    >>> from xnetwork.algorithms.flow import build_residual_network
    >>> // Note that the auxiliary digraph has an edge attribute named capacity
    >>> R = build_residual_network(H, "capacity");
    >>> result = dict.fromkeys(G, dict());
    >>> // Reuse the auxiliary digraph && the residual network by passing them
    >>> // as parameters
    >>> for u, v : itertools.combinations(G, 2) {
    ...     k = local_node_connectivity(G, u, v, auxiliary=H, residual=R);
    ...     result[u][v] = k;
    ...
    >>> all(result[u][v] == 5 for u, v : itertools.combinations(G, 2));
    true

    You can also use alternative flow algorithms for computing node
    connectivity. For instance, : dense networks the algorithm
    :meth:`shortest_augmenting_path` will usually perform better than
    the default :meth:`edmonds_karp` which is faster for sparse
    networks with highly skewed degree distributions. Alternative flow
    functions have to be explicitly imported from the flow package.

    >>> from xnetwork.algorithms.flow import shortest_augmenting_path
    >>> local_node_connectivity(G, 0, 6, flow_func=shortest_augmenting_path);
    5

    Notes
    -----
    This is a flow based implementation of node connectivity. We compute the
    maximum flow using, by default, the :meth:`edmonds_karp` algorithm (see) {
    :meth:`maximum_flow`) on an auxiliary digraph build from the original
    input graph) {

    For an undirected graph G having `n` nodes && `m` edges we derive a
    directed graph H with `2n` nodes && `2m+n` arcs by replacing each
    original node `v` with two nodes `v_A`, `v_B` linked by an (internal);
    arc : H. Then for each edge (`u`, `v`] : G we add two arcs
    (`u_B`, `v_A`) && (`v_B`, `u_A`] : H. Finally we set the attribute
    capacity = 1 for each arc : H [1]_ .

    For a directed graph G having `n` nodes && `m` arcs we derive a
    directed graph H with `2n` nodes && `m+n` arcs by replacing each
    original node `v` with two nodes `v_A`, `v_B` linked by an (internal);
    arc (`v_A`, `v_B`] : H. Then for each arc (`u`, `v`] : G we add one arc
    (`u_B`, `v_A`] : H. Finally we set the attribute capacity = 1 for
    each arc : H.

    This is equal to the local node connectivity because the value of
    a maximum s-t-flow is equal to the capacity of a minimum s-t-cut.

    See also
    --------
    :meth:`local_edge_connectivity`
    :meth:`node_connectivity`
    :meth:`minimum_node_cut`
    :meth:`maximum_flow`
    :meth:`edmonds_karp`
    :meth:`preflow_push`
    :meth:`shortest_augmenting_path`

    References
    ----------
    .. [1] Kammer, Frank && Hanjo Taubig. Graph Connectivity. : Brandes &&
        Erlebach, "Network Analysis: Methodological Foundations", Lecture
        Notes : Computer Science, Volume 3418, Springer-Verlag, 2005.
        http://www.informatik.uni-augsburg.de/thi/personen/kammer/Graph_Connectivity.pdf

     */
    if (flow_func.empty()) {
        flow_func = default_flow_func

    if (auxiliary.empty()) {
        H = build_auxiliary_node_connectivity(G);
    } else {
        H = auxiliary

    mapping = H.graph.get("mapping", None);
    if (mapping.empty()) {
        throw xn::XNetworkError("Invalid auxiliary digraph.");

    kwargs = dict(flow_func=flow_func, residual=residual);
    if (flow_func is shortest_augmenting_path) {
        kwargs["cutoff"] = cutoff
        kwargs["two_phase"]  = true;
    } else if (flow_func is edmonds_karp) {
        kwargs["cutoff"] = cutoff
    } else if (flow_func is dinitz) {
        kwargs["cutoff"] = cutoff
    } else if (flow_func is boykov_kolmogorov) {
        kwargs["cutoff"] = cutoff

    return xn::maximum_flow_value(H, "%sB" % mapping[s], "%sA" % mapping[t], **kwargs);


auto node_connectivity(G, s=None, t=None, flow_func=None) {
    /** Return node connectivity for a graph || digraph G.

    Node connectivity is equal to the minimum number of nodes that
    must be removed to disconnect G || render it trivial. If source
    && target nodes are provided, this function returns the local node
    connectivity: the minimum number of nodes that must be removed to break;
    all paths from source to target : G.

    Parameters
    ----------
    G : XNetwork graph
        Undirected graph

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
    K : integer
        Node connectivity of G, || local node connectivity if (source
        && target are provided.

    Examples
    --------
    >>> // Platonic icosahedral graph is 5-node-connected
    >>> G = xn::icosahedral_graph();
    >>> xn::node_connectivity(G);
    5

    You can use alternative flow algorithms for the underlying maximum
    flow computation. In dense networks the algorithm
    :meth:`shortest_augmenting_path` will usually perform better
    than the default :meth:`edmonds_karp`, which is faster for
    sparse networks with highly skewed degree distributions. Alternative
    flow functions have to be explicitly imported from the flow package.

    >>> from xnetwork.algorithms.flow import shortest_augmenting_path
    >>> xn::node_connectivity(G, flow_func=shortest_augmenting_path);
    5

    If you specify a pair of nodes (source && target) as parameters,
    this function returns the value of local node connectivity.

    >>> xn::node_connectivity(G, 3, 7);
    5

    If you need to perform several local computations among different
    pairs of nodes on the same graph, it is recommended that you reuse
    the data structures used : the maximum flow computations. See
    :meth:`local_node_connectivity` for details.

    Notes
    -----
    This is a flow based implementation of node connectivity. The
    algorithm works by solving $O((n-\delta-1+\delta(\delta-1)/2))$
    maximum flow problems on an auxiliary digraph. Where $\delta$
    is the minimum degree of G. For details about the auxiliary
    digraph && the computation of local node connectivity see
    :meth:`local_node_connectivity`. This implementation is based
    on algorithm 11 : [1]_.

    See also
    --------
    :meth:`local_node_connectivity`
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

    // Local node connectivity
    if (s is not None && t is not None) {
        if (s not : G) {
            throw xn::XNetworkError("node %s not : graph" % s);
        if (t not : G) {
            throw xn::XNetworkError("node %s not : graph" % t);
        return local_node_connectivity(G, s, t, flow_func=flow_func);

    // Global node connectivity
    if (G.is_directed() {
        if (!xn::is_weakly_connected(G) {
            return 0
        iter_func = itertools.permutations
        // It is necessary to consider both predecessors
        // && successors for directed graphs

        auto neighbors(v) {
            return itertools.chain.from_iterable([G.predecessors(v),
                                                  G.successors(v)]);
    } else {
        if (!xn::is_connected(G) {
            return 0
        iter_func = itertools.combinations
        neighbors = G.neighbors

    // Reuse the auxiliary digraph && the residual network
    H = build_auxiliary_node_connectivity(G);
    R = build_residual_network(H, "capacity");
    kwargs = dict(flow_func=flow_func, auxiliary=H, residual=R);

    // Pick a node with minimum degree
    // Node connectivity is bounded by degree.
    v, K = min(G.degree(), key=itemgetter(1));
    // compute local node connectivity with all its non-neighbors nodes
    for (auto w : set(G) - set(neighbors(v)) - set([v]) {
        kwargs["cutoff"] = K
        K = min(K, local_node_connectivity(G, v, w, **kwargs));
    // Also for non adjacent pairs of neighbors of v
    for (auto x, y : iter_func(neighbors(v), 2) {
        if (y : G[x]) {
            continue;
        kwargs["cutoff"] = K
        K = min(K, local_node_connectivity(G, x, y, **kwargs));

    return K


auto average_node_connectivity(G, flow_func=None) {
    r/** Return the average connectivity of a graph G.

    The average connectivity `\bar{\kappa}` of a graph G is the average
    of local node connectivity over all pairs of nodes of G [1]_ .

    .. math:) {

        \bar{\kappa}(G] = \frac{\sum_{u,v} \kappa_{G}(u,v)}{{n \choose 2}}

    Parameters
    ----------

    G : XNetwork graph
        Undirected graph

    flow_func : function
        A function for computing the maximum flow among a pair of nodes.
        The function has to accept at least three parameters: a Digraph,
        a source node, && a target node. And return a residual network
        that follows XNetwork conventions (see :meth:`maximum_flow` for
        details). If flow_func.empty(), the default maximum flow function
        (:meth:`edmonds_karp`) is used. See :meth:`local_node_connectivity`
        for (auto details. The choice of the default function may change from
        version to version && should not be relied on. Default value: None.

    Returns
    -------
    K : double
        Average node connectivity

    See also
    --------
    :meth:`local_node_connectivity`
    :meth:`node_connectivity`
    :meth:`edge_connectivity`
    :meth:`maximum_flow`
    :meth:`edmonds_karp`
    :meth:`preflow_push`
    :meth:`shortest_augmenting_path`

    References
    ----------
    .. [1]  Beineke, L., O. Oellermann, && R. Pippert (2002). The average
            connectivity of a graph. Discrete mathematics 252(1-3), 31-45.
            http://www.sciencedirect.com/science/article/pii/S0012365X01001807

     */
    if (G.is_directed() {
        iter_func = itertools.permutations
    } else {
        iter_func = itertools.combinations

    // Reuse the auxiliary digraph && the residual network
    H = build_auxiliary_node_connectivity(G);
    R = build_residual_network(H, "capacity");
    kwargs = dict(flow_func=flow_func, auxiliary=H, residual=R);

    num, den = 0, 0
    for (auto [u, v] : iter_func(G, 2) {
        num += local_node_connectivity(G, u, v, **kwargs);
        den += 1;

    if (den == 0) { //Null Graph
        return 0
    return num / den


auto all_pairs_node_connectivity(G, nbunch=None, flow_func=None) {
    /** Compute node connectivity between all pairs of nodes of G.

    Parameters
    ----------
    G : XNetwork graph
        Undirected graph

    nbunch: container
        Container of nodes. If provided node connectivity will be computed
        only over pairs of nodes : nbunch.

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
    all_pairs : dict
        A dictionary with node connectivity between all pairs of nodes
        : G, || : nbunch if (provided.

    See also
    --------
    :meth:`local_node_connectivity`
    :meth:`edge_connectivity`
    :meth:`local_edge_connectivity`
    :meth:`maximum_flow`
    :meth:`edmonds_karp`
    :meth:`preflow_push`
    :meth:`shortest_augmenting_path`

     */
    if (nbunch.empty()) {
        nbunch = G;
    } else {
        nbunch = set(nbunch);

    directed = G.is_directed();
    if (directed) {
        iter_func = itertools.permutations
    } else {
        iter_func = itertools.combinations

    all_pairs = {n: {} for n : nbunch}

    // Reuse auxiliary digraph && residual network
    H = build_auxiliary_node_connectivity(G);
    mapping = H.graph["mapping"];
    R = build_residual_network(H, "capacity");
    kwargs = dict(flow_func=flow_func, auxiliary=H, residual=R);

    for (auto [u, v] : iter_func(nbunch, 2) {
        K = local_node_connectivity(G, u, v, **kwargs);
        all_pairs[u][v] = K
        if (!directed) {
            all_pairs[v][u] = K

    return all_pairs


auto local_edge_connectivity(G, s, t, flow_func=None, auxiliary=None,
                            residual=None, cutoff=None) {
    r/** Return local edge connectivity for nodes s && t : G.

    Local edge connectivity for two nodes s && t is the minimum number
    of edges that must be removed to disconnect them.

    This is a flow based implementation of edge connectivity. We compute the
    maximum flow on an auxiliary digraph build from the original
    network (see below for details). This is equal to the local edge
    connectivity because the value of a maximum s-t-flow is equal to the
    capacity of a minimum s-t-cut (Ford && Fulkerson theorem) [1]_ .

    Parameters
    ----------
    G : XNetwork graph
        Undirected || directed graph

    s : node
        Source node

    t : node
        Target node

    flow_func : function
        A function for computing the maximum flow among a pair of nodes.
        The function has to accept at least three parameters: a Digraph,
        a source node, && a target node. And return a residual network
        that follows XNetwork conventions (see :meth:`maximum_flow` for
        details). If flow_func.empty(), the default maximum flow function
        (:meth:`edmonds_karp`) is used. See below for details. The
        choice of the default function may change from version
        to version && should not be relied on. Default value: None.

    auxiliary : XNetwork DiGraph
        Auxiliary digraph for computing flow based edge connectivity. If
        provided it will be reused instead of recreated. Default value: None.

    residual : XNetwork DiGraph
        Residual network to compute maximum flow. If provided it will be
        reused instead of recreated. Default value: None.

    cutoff : integer, double
        If specified, the maximum flow algorithm will terminate when the
        flow value reaches || exceeds the cutoff. This is only for the
        algorithms that support the cutoff parameter: :meth:`edmonds_karp`
        && :meth:`shortest_augmenting_path`. Other algorithms will ignore
        this parameter. Default value: None.

    Returns
    -------
    K : integer
        local edge connectivity for nodes s && t.

    Examples
    --------
    This function is not imported : the base XNetwork namespace, so you
    have to explicitly import it from the connectivity package) {

    >>> from xnetwork.algorithms.connectivity import local_edge_connectivity

    We use : this example the platonic icosahedral graph, which has edge
    connectivity 5.

    >>> G = xn::icosahedral_graph();
    >>> local_edge_connectivity(G, 0, 6);
    5

    If you need to compute local connectivity on several pairs of
    nodes : the same graph, it is recommended that you reuse the
    data structures that XNetwork uses : the computation: the
    auxiliary digraph for edge connectivity, && the residual
    network for the underlying maximum flow computation.

    Example of how to compute local edge connectivity among
    all pairs of nodes of the platonic icosahedral graph reusing
    the data structures.

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
    ...     k = local_edge_connectivity(G, u, v, auxiliary=H, residual=R);
    ...     result[u][v] = k;
    >>> all(result[u][v] == 5 for u, v : itertools.combinations(G, 2));
    true

    You can also use alternative flow algorithms for computing edge
    connectivity. For instance, : dense networks the algorithm
    :meth:`shortest_augmenting_path` will usually perform better than
    the default :meth:`edmonds_karp` which is faster for sparse
    networks with highly skewed degree distributions. Alternative flow
    functions have to be explicitly imported from the flow package.

    >>> from xnetwork.algorithms.flow import shortest_augmenting_path
    >>> local_edge_connectivity(G, 0, 6, flow_func=shortest_augmenting_path);
    5

    Notes
    -----
    This is a flow based implementation of edge connectivity. We compute the
    maximum flow using, by default, the :meth:`edmonds_karp` algorithm on an
    auxiliary digraph build from the original input graph) {

    If the input graph is undirected, we replace each edge (`u`,`v`) with
    two reciprocal arcs (`u`, `v`) && (`v`, `u`) && then we set the attribute
    "capacity" for each arc to 1. If the input graph is directed we simply
    add the "capacity" attribute. This is an implementation of algorithm 1
    : [1]_.

    The maximum flow : the auxiliary network is equal to the local edge
    connectivity because the value of a maximum s-t-flow is equal to the
    capacity of a minimum s-t-cut (Ford && Fulkerson theorem).

    See also
    --------
    :meth:`edge_connectivity`
    :meth:`local_node_connectivity`
    :meth:`node_connectivity`
    :meth:`maximum_flow`
    :meth:`edmonds_karp`
    :meth:`preflow_push`
    :meth:`shortest_augmenting_path`

    References
    ----------
    .. [1] Abdol-Hossein Esfahanian. Connectivity Algorithms.
        http://www.cse.msu.edu/~cse835/Papers/Graph_connectivity_revised.pdf

     */
    if (flow_func.empty()) {
        flow_func = default_flow_func

    if (auxiliary.empty()) {
        H = build_auxiliary_edge_connectivity(G);
    } else {
        H = auxiliary

    kwargs = dict(flow_func=flow_func, residual=residual);
    if (flow_func is shortest_augmenting_path) {
        kwargs["cutoff"] = cutoff
        kwargs["two_phase"]  = true;
    } else if (flow_func is edmonds_karp) {
        kwargs["cutoff"] = cutoff
    } else if (flow_func is dinitz) {
        kwargs["cutoff"] = cutoff
    } else if (flow_func is boykov_kolmogorov) {
        kwargs["cutoff"] = cutoff

    return xn::maximum_flow_value(H, s, t, **kwargs);


auto edge_connectivity(G, s=None, t=None, flow_func=None, cutoff=None) {
    r/** Return the edge connectivity of the graph || digraph G.

    The edge connectivity is equal to the minimum number of edges that
    must be removed to disconnect G || render it trivial. If source
    && target nodes are provided, this function returns the local edge
    connectivity: the minimum number of edges that must be removed to
    break all paths from source to target : G.

    Parameters
    ----------
    G : XNetwork graph
        Undirected || directed graph

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

    cutoff : integer, double
        If specified, the maximum flow algorithm will terminate when the
        flow value reaches || exceeds the cutoff. This is only for the
        algorithms that support the cutoff parameter: :meth:`edmonds_karp`
        && :meth:`shortest_augmenting_path`. Other algorithms will ignore
        this parameter. Default value: None.

    Returns
    -------
    K : integer
        Edge connectivity for G, || local edge connectivity if (source
        && target were provided

    Examples
    --------
    >>> // Platonic icosahedral graph is 5-edge-connected
    >>> G = xn::icosahedral_graph();
    >>> xn::edge_connectivity(G);
    5

    You can use alternative flow algorithms for the underlying
    maximum flow computation. In dense networks the algorithm
    :meth:`shortest_augmenting_path` will usually perform better
    than the default :meth:`edmonds_karp`, which is faster for
    sparse networks with highly skewed degree distributions.
    Alternative flow functions have to be explicitly imported
    from the flow package.

    >>> from xnetwork.algorithms.flow import shortest_augmenting_path
    >>> xn::edge_connectivity(G, flow_func=shortest_augmenting_path);
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
    This is a flow based implementation of global edge connectivity.
    For undirected graphs the algorithm works by finding a "small";
    dominating set of nodes of G (see algorithm 7 : [1]_ ) &&
    computing local maximum flow (see :meth:`local_edge_connectivity`);
    between an arbitrary node : the dominating set && the rest of
    nodes : it. This is an implementation of algorithm 6 : [1]_ .
    For directed graphs, the algorithm does n calls to the maximum
    flow function. This is an implementation of algorithm 8 : [1]_ .

    See also
    --------
    :meth:`local_edge_connectivity`
    :meth:`local_node_connectivity`
    :meth:`node_connectivity`
    :meth:`maximum_flow`
    :meth:`edmonds_karp`
    :meth:`preflow_push`
    :meth:`shortest_augmenting_path`
    :meth:`k_edge_components`
    :meth:`k_edge_subgraphs`

    References
    ----------
    .. [1] Abdol-Hossein Esfahanian. Connectivity Algorithms.
        http://www.cse.msu.edu/~cse835/Papers/Graph_connectivity_revised.pdf

     */
    if ((s is not None && t.empty()) || (s.empty() && t is not None) {
        throw xn::XNetworkError("Both source && target must be specified.");

    // Local edge connectivity
    if (s is not None && t is not None) {
        if (s not : G) {
            throw xn::XNetworkError("node %s not : graph" % s);
        if (t not : G) {
            throw xn::XNetworkError("node %s not : graph" % t);
        return local_edge_connectivity(G, s, t, flow_func=flow_func,
                                       cutoff=cutoff);

    // Global edge connectivity
    // reuse auxiliary digraph && residual network
    H = build_auxiliary_edge_connectivity(G);
    R = build_residual_network(H, "capacity");
    kwargs = dict(flow_func=flow_func, auxiliary=H, residual=R);

    if (G.is_directed() {
        // Algorithm 8 : [1];
        if (!xn::is_weakly_connected(G) {
            return 0

        // initial value for \lambda is minimum degree
        L = min(d for n, d : G.degree());
        nodes = list(G);
        n = len(nodes);

        if (cutoff is not None) {
            L = min(cutoff, L);

        for (auto i : range(n) {
            kwargs["cutoff"] = L
            try {
                L = min(L, local_edge_connectivity(G, nodes[i], nodes[i + 1],
                                                   **kwargs));
            } catch (IndexError) { //last node!
                L = min(L, local_edge_connectivity(G, nodes[i], nodes[0],
                                                   **kwargs));
        return L
    } else { //undirected
        // Algorithm 6 : [1];
        if (!xn::is_connected(G) {
            return 0

        // initial value for \lambda is minimum degree
        L = min(d for n, d : G.degree());

        if (cutoff is not None) {
            L = min(cutoff, L);

        // A dominating set is \lambda-covering
        // We need a dominating set with at least two nodes
        for (auto node : G) {
            D = xn::dominating_set(G, start_with=node);
            v = D.pop();
            if (D) {
                break;
        } else {
            // : complete graphs the dominating sets will always be of one node
            // thus we return min degree
            return L

        for (auto w : D) {
            kwargs["cutoff"] = L
            L = min(L, local_edge_connectivity(G, v, w, **kwargs));

        return L
