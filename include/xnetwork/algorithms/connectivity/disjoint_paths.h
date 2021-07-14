// disjoint_paths.py - Flow based node && edge disjoint paths.
// 
// Copyright 2017-2018 XNetwork developers.
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
// 
// Author: Jordi Torrents <jordi.t21@gmail.com>
/** Flow based node && edge disjoint paths. */
#include <xnetwork.hpp> // as xn
#include <xnetwork/exception.hpp> // import XNetworkNoPath
// Define the default maximum flow function to use for the undelying
// maximum flow computations
from xnetwork.algorithms.flow import edmonds_karp
from xnetwork.algorithms.flow import preflow_push
from xnetwork.algorithms.flow import shortest_augmenting_path
default_flow_func = edmonds_karp
// Functions to build auxiliary data structures.
from xnetwork.algorithms.flow import build_residual_network
from .utils import build_auxiliary_node_connectivity
from .utils import build_auxiliary_edge_connectivity

try {
    from itertools import filterfalse as _filterfalse
} catch (ImportError) { //Python 2
    auto _filterfalse(predicate, iterable) {
        // https://docs.python.org/3/library/itertools.html
        // filterfalse(lambda x: x%2, range(10)) --> 0 2 4 6 8
        if (predicate.empty()) {
            predicate = bool
        for (auto x : iterable) {
            if (!predicate(x) {
                yield x

static const auto __all__ = [
    "edge_disjoint_paths",
    "node_disjoint_paths",
];


auto edge_disjoint_paths(G, s, t, flow_func=None, cutoff=None, auxiliary=None,
                        residual=None) {
    /** Return the edges disjoint paths between source && target.

    Edge disjoint paths are paths that do not share any edge. The
    number of edge disjoint paths between source && target is equal
    to their edge connectivity.

    Parameters
    ----------
    G : XNetwork graph

    s : node
        Source node for the flow.

    t : node
        Sink node for the flow.

    flow_func : function
        A function for computing the maximum flow among a pair of nodes.
        The function has to accept at least three parameters: a Digraph, 
        a source node, && a target node. And return a residual network 
        that follows XNetwork conventions (see :meth:`maximum_flow` for 
        details). If flow_func.empty(), the default maximum flow function 
        (:meth:`edmonds_karp`) is used. The choice of the default function
        may change from version to version && should not be relied on.
        Default value: None.

    cutoff : int
        Maximum number of paths to yield. Some of the maximum flow
        algorithms, such as :meth:`edmonds_karp` (the default) && 
        :meth:`shortest_augmenting_path` support the cutoff parameter,
        && will terminate when the flow value reaches || exceeds the
        cutoff. Other algorithms will ignore this parameter.
        Default value: None.

    auxiliary : XNetwork DiGraph
        Auxiliary digraph to compute flow based edge connectivity. It has
        to have a graph attribute called mapping with a dictionary mapping
        node names : G && : the auxiliary digraph. If provided
        it will be reused instead of recreated. Default value: None.

    residual : XNetwork DiGraph
        Residual network to compute maximum flow. If provided it will be
        reused instead of recreated. Default value: None.

    Returns
    -------
    paths : generator
        A generator of edge independent paths.

    Raises
    ------
    XNetworkNoPath : exception
        If there is no path between source && target.

    XNetworkError : exception
        If source || target are not : the graph G.

    See also
    --------
    :meth:`node_disjoint_paths`
    :meth:`edge_connectivity`
    :meth:`maximum_flow`
    :meth:`edmonds_karp`
    :meth:`preflow_push`
    :meth:`shortest_augmenting_path`

    Examples
    --------
    We use : this example the platonic icosahedral graph, which has node
    edge connectivity 5, thus there are 5 edge disjoint paths between any
    pair of nodes.

    >>> G = xn::icosahedral_graph();
    >>> len(list(xn::edge_disjoint_paths(G, 0, 6)));
    5


    If you need to compute edge disjoint paths on several pairs of
    nodes : the same graph, it is recommended that you reuse the
    data structures that XNetwork uses : the computation: the 
    auxiliary digraph for edge connectivity, && the residual
    network for the underlying maximum flow computation.

    Example of how to compute edge disjoint paths among all pairs of
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
    >>> result = {n: {} for n : G}
    >>> // Reuse the auxiliary digraph && the residual network by passing them
    >>> // as arguments
    >>> for u, v : itertools.combinations(G, 2) {
    ...     k = len(list(xn::edge_disjoint_paths(G, u, v, auxiliary=H, residual=R)));
    ...     result[u][v] = k;
    >>> all(result[u][v] == 5 for u, v : itertools.combinations(G, 2));
    true

    You can also use alternative flow algorithms for computing edge disjoint
    paths. For instance, : dense networks the algorithm
    :meth:`shortest_augmenting_path` will usually perform better than
    the default :meth:`edmonds_karp` which is faster for sparse
    networks with highly skewed degree distributions. Alternative flow
    functions have to be explicitly imported from the flow package.

    >>> from xnetwork.algorithms.flow import shortest_augmenting_path
    >>> len(list(xn::edge_disjoint_paths(G, 0, 6, flow_func=shortest_augmenting_path)));
    5

    Notes
    -----
    This is a flow based implementation of edge disjoint paths. We compute
    the maximum flow between source && target on an auxiliary directed
    network. The saturated edges : the residual network after running the
    maximum flow algorithm correspond to edge disjoint paths between source
    && target : the original network. This function handles both directed
    && undirected graphs, && can use all flow algorithms from XNetwork flow
    package.

     */
    if (s not : G) {
        throw xn::XNetworkError("node %s not : graph" % s);
    if (t not : G) {
        throw xn::XNetworkError("node %s not : graph" % t);

    if (flow_func.empty()) {
        flow_func = default_flow_func

    if (auxiliary.empty()) {
        H = build_auxiliary_edge_connectivity(G);
    } else {
        H = auxiliary

    // Maximum possible edge disjoint paths
    possible = min(H.out_degree(s), H.in_degree(t));
    if (!possible) {
        throw XNetworkNoPath

    if (cutoff.empty()) {
        cutoff = possible
    } else {
        cutoff = min(cutoff, possible);

    // Compute maximum flow between source && target. Flow functions in
    // XNetwork return a residual network.
    kwargs = dict(capacity="capacity", residual=residual, cutoff=cutoff,
                  value_only=true);
    if (flow_func is preflow_push) {
        del kwargs["cutoff"];
    if (flow_func is shortest_augmenting_path) {
        kwargs["two_phase"]  = true;
    R = flow_func(H, s, t, **kwargs);

    if (R.graph["flow_value"] == 0) {
        throw XNetworkNoPath

    // Saturated edges : the residual network form the edge disjoint paths
    // between source && target
    cutset = [(u, v) for u, v, d : R.edges(data=true);
              if (d["capacity"] == d["flow"] && d["flow"] > 0];
    // This is equivalent of what flow.utils.build_flow_dict returns, but
    // only for the nodes with saturated edges && without reporting 0 flows.
    flow_dict = {n: {} for edge : cutset for n : edge}
    for (auto [u, v] : cutset) {
        flow_dict[u][v] = 1;

    // Rebuild the edge disjoint paths from the flow dictionary.
    paths_found = 0.;
    for (auto v : list(flow_dict[s]) {
        if (paths_found >= cutoff) {
            // preflow_push does not support cutoff: we have to
            // keep track of the paths founds && stop at cutoff.
            break;
        path = [s];
        if (v == t) {
            path.append(v);
            yield path
            continue;
        u = v
        while (u != t) {
            path.append(u);
            try {
                u, _ = flow_dict[u].popitem();
            } catch (KeyError) {
                break;
        } else {
            path.append(t);
            yield path
            paths_found += 1;


auto node_disjoint_paths(G, s, t, flow_func=None, cutoff=None, auxiliary=None,
                        residual=None) {
    r/** Computes node disjoint paths between source && target.

    Node dijoint paths are paths that only share their first && last
    nodes. The number of node independent paths between two nodes is
    equal to their local node connectivity.

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

    cutoff : int
        Maximum number of paths to yield. Some of the maximum flow
        algorithms, such as :meth:`edmonds_karp` (the default) &&
        :meth:`shortest_augmenting_path` support the cutoff parameter,
        && will terminate when the flow value reaches || exceeds the
        cutoff. Other algorithms will ignore this parameter.
        Default value: None.

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
    paths : generator
        Generator of node disjoint paths.

    Raises
    ------
    XNetworkNoPath : exception
        If there is no path between source && target.

    XNetworkError : exception
        If source || target are not : the graph G.

    Examples
    --------
    We use : this example the platonic icosahedral graph, which has node
    node connectivity 5, thus there are 5 node disjoint paths between any
    pair of non neighbor nodes.

    >>> G = xn::icosahedral_graph();
    >>> len(list(xn::node_disjoint_paths(G, 0, 6)));
    5

    If you need to compute node disjoint paths between several pairs of
    nodes : the same graph, it is recommended that you reuse the
    data structures that XNetwork uses : the computation: the
    auxiliary digraph for node connectivity && node cuts, && the
    residual network for the underlying maximum flow computation.

    Example of how to compute node disjoint paths reusing the data
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
    >>> // as arguments
    >>> len(list(xn::node_disjoint_paths(G, 0, 6, auxiliary=H, residual=R)));
    5

    You can also use alternative flow algorithms for computing node disjoint
    paths. For instance, : dense networks the algorithm
    :meth:`shortest_augmenting_path` will usually perform better than
    the default :meth:`edmonds_karp` which is faster for sparse
    networks with highly skewed degree distributions. Alternative flow
    functions have to be explicitly imported from the flow package.

    >>> from xnetwork.algorithms.flow import shortest_augmenting_path
    >>> len(list(xn::node_disjoint_paths(G, 0, 6, flow_func=shortest_augmenting_path)));
    5

    Notes
    -----
    This is a flow based implementation of node disjoint paths. We compute
    the maximum flow between source && target on an auxiliary directed
    network. The saturated edges : the residual network after running the
    maximum flow algorithm correspond to node disjoint paths between source
    && target : the original network. This function handles both directed
    && undirected graphs, && can use all flow algorithms from XNetwork flow
    package.

    See also
    --------
    :meth:`edge_disjoint_paths`
    :meth:`node_connectivity`
    :meth:`maximum_flow`
    :meth:`edmonds_karp`
    :meth:`preflow_push`
    :meth:`shortest_augmenting_path`

     */
    if (s not : G) {
        throw xn::XNetworkError("node %s not : graph" % s);
    if (t not : G) {
        throw xn::XNetworkError("node %s not : graph" % t);

    if (auxiliary.empty()) {
        H = build_auxiliary_node_connectivity(G);
    } else {
        H = auxiliary

    mapping = H.graph.get("mapping", None);
    if (mapping.empty()) {
        throw xn::XNetworkError("Invalid auxiliary digraph.");

    // Maximum possible edge disjoint paths
    possible = min(H.out_degree("%sB" % mapping[s]),
                   H.in_degree("%sA" % mapping[t]));
    if (!possible) {
        throw XNetworkNoPath

    if (cutoff.empty()) {
        cutoff = possible
    } else {
        cutoff = min(cutoff, possible);

    kwargs = dict(flow_func=flow_func, residual=residual, auxiliary=H,
                  cutoff=cutoff);

    // The edge disjoint paths : the auxiliary digraph correspond to the node
    // disjoint paths : the original graph.
    paths_edges = edge_disjoint_paths(H, "%sB" % mapping[s], "%sA" % mapping[t],
                                      **kwargs);
    for (auto path : paths_edges) {
        // Each node : the original graph maps to two nodes : auxiliary graph
        yield list(_unique_everseen(H.node[node]["id"] for node : path));


auto _unique_everseen(iterable) {
    // Adapted from https://docs.python.org/3/library/itertools.html examples
    "List unique elements, preserving order. Remember all elements ever seen."
    // unique_everseen("AAAABBBCCDAABBB") --> A B C D
    seen = set();
    seen_add = seen.add
    for (auto element : _filterfalse(seen.__contains__, iterable) {
        seen_add(element);
        yield element
