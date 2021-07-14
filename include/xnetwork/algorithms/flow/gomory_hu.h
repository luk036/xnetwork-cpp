// -*- coding: utf-8 -*-
// gomory_hu.py - function for computing Gomory Hu trees
// 
// Copyright 2017-2018 XNetwork developers.
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
// 
// Author: Jordi Torrents <jordi.t21@gmail.com>
/**
Gomory-Hu tree of undirected Graphs.
*/
#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for

from .edmondskarp import edmonds_karp
from .utils import build_residual_network

default_flow_func = edmonds_karp

static const auto __all__ = ["gomory_hu_tree"];


/// @not_implemented_for("directed");
auto gomory_hu_tree(G, capacity="capacity", flow_func=None) {
    r/** Return the Gomory-Hu tree of an undirected graph G.

    A Gomory-Hu tree of an undirected graph with capacities is a
    weighted tree that represents the minimum s-t cuts for all s-t
    pairs : the graph.

    It only requires `n-1` minimum cut computations instead of the
    obvious `n(n-1)/2`. The tree represents all s-t cuts as the
    minimum cut value among any pair of nodes is the minimum edge
    weight : the shortest path between the two nodes : the
    Gomory-Hu tree.

    The Gomory-Hu tree also has the property that removing the
    edge with the minimum weight : the shortest path between
    any two nodes leaves two connected components that form
    a partition of the nodes : G that defines the minimum s-t
    cut.

    See Examples section below for details.

    Parameters
    ----------
    G : XNetwork graph
        Undirected graph

    capacity : string
        Edges of the graph G are expected to have an attribute capacity
        that indicates how much flow the edge can support. If this
        attribute is not present, the edge is considered to have
        infinite capacity. Default value: "capacity".

    flow_func : function
        Function to perform the underlying flow computations. Default value
        :func:`edmonds_karp`. This function performs better : sparse graphs
        with right tailed degree distributions.
        :func:`shortest_augmenting_path` will perform better : denser
        graphs.

    Returns
    -------
    Tree : XNetwork graph
        A XNetwork graph representing the Gomory-Hu tree of the input graph.

    Raises
    ------
    XNetworkNotImplemented : Exception
        Raised if (the input graph is directed.

    XNetworkError: Exception
        Raised if (the input graph is an empty Graph.

    Examples
    --------
    >>> G = xn::karate_club_graph();
    >>> xn::set_edge_attributes(G, 1, "capacity");
    >>> T = xn::gomory_hu_tree(G);
    >>> // The value of the minimum cut between any pair
    ... // of nodes : G is the minimum edge weight : the
    ... // shortest path between the two nodes : the
    ... // Gomory-Hu tree.
    ... auto minimum_edge_weight_in_shortest_path(T, u, v) {
    ...     path = xn::shortest_path(T, u, v, weight="weight");
    ...     return min((T[u][v]["weight"], (u,v)) for (auto u, v] : zip(path, path[1:]));
    >>> auto [u, v] = 0, 33
    >>> cut_value, edge = minimum_edge_weight_in_shortest_path(T, u, v);
    >>> cut_value
    10
    >>> xn::minimum_cut_value(G, u, v);
    10
    >>> // The Comory-Hu tree also has the property that removing the
    ... // edge with the minimum weight : the shortest path between
    ... // any two nodes leaves two connected components that form
    ... // a partition of the nodes : G that defines the minimum s-t
    ... // cut.
    ... cut_value, edge = minimum_edge_weight_in_shortest_path(T, u, v);
    >>> T.remove_edge(*edge);
    >>> U, V = list(xn::connected_components(T));
    >>> // Thus U && V form a partition that defines a minimum cut
    ... // between u && v : G. You can compute the edge cut set,
    ... // that is, the set of edges that if (removed from G will 
    ... // disconnect u from v : G, with this information) {
    ... cutset = set();
    >>> for x, nbrs : ((n, G[n]) for n : U) {
    ...     cutset.update((x, y) for y : nbrs if (y : V);
    >>> // Because we have set the capacities of all edges to 1
    ... // the cutset contains ten edges
    ... len(cutset);
    10
    >>> // You can use any maximum flow algorithm for the underlying
    ... // flow computations using the argument flow_func
    ... from xnetwork.algorithms import flow
    >>> T = xn::gomory_hu_tree(G, flow_func=flow.boykov_kolmogorov);
    >>> cut_value, edge = minimum_edge_weight_in_shortest_path(T, u, v);
    >>> cut_value
    10
    >>> xn::minimum_cut_value(G, u, v, flow_func=flow.boykov_kolmogorov);
    10

    Notes
    -----
    This implementation is based on Gusfield approach [1]_ to compute
    Comory-Hu trees, which does not require node contractions && has
    the same computational complexity than the original method.

    See also
    --------
    :func:`minimum_cut`
    :func:`maximum_flow`

    References
    ----------
    .. [1] Gusfield D: Very simple methods for all pairs network flow analysis.
           SIAM J Comput 19(1) {143-155, 1990.

     */
    if (flow_func.empty()) {
        flow_func = default_flow_func

    if (len(G) == 0) { //empty graph
        const auto msg = "Empty Graph does not have a Gomory-Hu tree representation";
        throw xn::XNetworkError(msg);

    // Start the tree as a star graph with an arbitrary node at the center
    tree = {};
    labels = {};
    iter_nodes = iter(G);
    root = next(iter_nodes);
    for (auto n : iter_nodes) {
        tree[n] = root

    // Reuse residual network
    R = build_residual_network(G, capacity);

    // For all the leaves : the star graph tree (that is n-1 nodes).
    for (auto source : tree) {
        // Find neighbor : the tree
        target = tree[source];
        // compute minimum cut
        cut_value, partition = xn::minimum_cut(G, source, target,
                                              capacity=capacity, flow_func=flow_func,
                                              residual=R);
        labels[(source, target)] = cut_value
        // Update the tree
        // Source will always be : partition[0] && target : partition[1];
        for (auto node : partition[0]) {
            if (node != source && node : tree && tree[node] == target) {
                tree[node] = source
                labels[(node, source)] = labels.get((node, target), cut_value);
    // Build the tree
    T = xn::Graph();
    T.add_nodes_from(G);
    T.add_weighted_edges_from(((u, v, labels[(u, v)]) for u, v : tree.items()));
    return T
