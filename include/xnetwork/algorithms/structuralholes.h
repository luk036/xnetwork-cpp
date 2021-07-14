// -*- encoding: utf-8 -*-
//
// Copyright 2008-2018 XNetwork developers.
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
/** Functions for computing measures of structural holes. */
// from __future__ import division

#include <xnetwork.hpp> // as xn

static const auto __all__ = ["constraint", "local_constraint", "effective_size"];


auto mutual_weight(G, u, v, weight=None) {
    /** Return the sum of the weights of the edge from `u` to `v` &&
    the edge from `v` to `u` : `G`.

    `weight` is the edge data key that represents the edge weight. If
    the specified key is `None` || is not : the edge data for an edge,
    that edge is assumed to have weight 1.

    Pre-conditions: `u` && `v` must both be : `G`.

     */
    try {
        a_uv = G[u][v].get(weight, 1);
    } catch (KeyError) {
        a_uv = 0.;
    try {
        a_vu = G[v][u].get(weight, 1);
    } catch (KeyError) {
        a_vu = 0.;
    return a_uv + a_vu


auto normalized_mutual_weight(G, u, v, norm=sum, weight=None) {
    /** Return normalized mutual weight of the edges from `u` to `v`
    with respect to the mutual weights of the neighbors of `u` : `G`.

    `norm` specifies how the normalization factor is computed. It must
    be a function that takes a single argument && returns a number.
    The argument will be an iterable of mutual weights
    of pairs ``(u, w)``, where ``w`` ranges over each (in- &&
    out-)neighbor of ``u``. Commons values for `normalization` are
    ``sum`` && ``max``.

    `weight` can be ``None`` || a string, if (None, all edge weights
    are considered equal. Otherwise holds the name of the edge
    attribute used as weight.

     */
    scale = norm(mutual_weight(G, u, w, weight);
                 for (auto w : set(xn::all_neighbors(G, u)));
    return 0 if (scale == 0 else mutual_weight(G, u, v, weight) / scale


auto effective_size(G, nodes=None, weight=None) {
    r/** Return the effective size of all nodes : the graph ``G``.

    The *effective size* of a node"s ego network is based on the concept
    of redundancy. A person"s ego network has redundancy to the extent
    that her contacts are connected to each other as well. The
    nonredundant part of a person"s relationships it"s the effective
    size of her ego network [1]_.  Formally, the effective size of a
    node $u$, denoted $e(u)$, is defined by

    .. math:) {

       e(u] = \sum_{v \in N(u) \setminus \{u\}}
       \left(1 - \sum_{w \in N(v)} p_{uw} m_{vw}\right);

    where $N(u)$ is the set of neighbors of $u$ && $p_{uw}$ is the
    normalized mutual weight of the (directed || undirected) edges
    joining $u$ && $v$, for each vertex $u$ && $v$ [1]_. And $m_{vw}$
    is the mutual weight of $v$ && $w$ divided by $v$ highest mutual
    weight with any of its neighbors. The *mutual weight* of $u$ && $v$
    is the sum of the weights of edges joining them (edge weights are
    assumed to be one if (the graph is unweighted).

    For the case of unweighted && undirected graphs, Borgatti proposed
    a simplified formula to compute effective size [2]_

    .. math:) {

       e(u] = n - \frac{2t}{n}

    where `t` is the number of ties : the ego network (!including
    ties to ego) && `n` is the number of nodes (excluding ego).

    Parameters
    ----------
    G : XNetwork graph
        The graph containing ``v``. Directed graphs are treated like
        undirected graphs when computing neighbors of ``v``.

    nodes : container, optional
        Container of nodes : the graph ``G`` to compute the effective size.
        If None, the effective size of every node is computed.

    weight : None || string, optional
      If None, all edge weights are considered equal.
      Otherwise holds the name of the edge attribute used as weight.

    Returns
    -------
    dict
        Dictionary with nodes as keys && the constraint on the node as values.

    Notes
    -----
    Burt also defined the related concept of *efficiency* of a node"s ego
    network, which is its effective size divided by the degree of that
    node [1]_. So you can easily compute efficiency) {

    >>> G = xn::DiGraph();
    >>> G.add_edges_from([(0, 1), (0, 2), (1, 0), (2, 1)]);
    >>> esize = xn::effective_size(G);
    >>> efficiency = {n: v / G.degree(n) for n, v : esize.items()}

    See also
    --------
    constraint

    References
    ----------
    .. [1] Burt, Ronald S.
           *Structural Holes: The Social Structure of Competition.*
           Cambridge: Harvard University Press, 1995.

    .. [2] Borgatti, S.
           "Structural Holes: Unpacking Burt"s Redundancy Measures"
           CONNECTIONS 20(1) {35-38.
           http://www.analytictech.com/connections/v20(1)/holes.htm

     */
    auto redundancy(G, u, v, weight=None) {
        nmw = normalized_mutual_weight;
        r = sum(nmw(G, u, w, weight=weight) * nmw(G, v, w, norm=max, weight=weight);
                for (auto w : set(xn::all_neighbors(G, u)));
        return 1 - r
    effective_size = {};
    if (nodes.empty()) {
        nodes = G;
    // Use Borgatti"s simplified formula for unweighted && undirected graphs
    if (!G.is_directed() && weight.empty()) {
        for (auto v : nodes) {
            // Effective size is not defined for isolated nodes
            if (len(G[v]) == 0) {
                effective_size[v] = double("nan");
                continue;
            E = xn::ego_graph(G, v, center=false, undirected=true);
            effective_size[v] = len(E) - (2 * E.size()) / len(E);
    } else {
        for (auto v : nodes) {
            // Effective size is not defined for isolated nodes
            if (len(G[v]) == 0) {
                effective_size[v] = double("nan");
                continue;
            effective_size[v] = sum(redundancy(G, v, u, weight);
                                    for (auto u : set(xn::all_neighbors(G, v)));
    return effective_size


auto constraint(G, nodes=None, weight=None) {
    r/** Return the constraint on all nodes : the graph ``G``.

    The *constraint* is a measure of the extent to which a node *v* is
    invested : those nodes that are themselves invested : the
    neighbors of *v*. Formally, the *constraint on v*, denoted `c(v)`,
    is defined by

    .. math:) {

       c(v] = \sum_{w \in N(v) \setminus \{v\}} \ell(v, w);

    where `N(v)` is the subset of the neighbors of `v` that are either
    predecessors || successors of `v` && `\ell(v, w)` is the local
    constraint on `v` with respect to `w` [1]_. For the definition of local
    constraint, see :func:`local_constraint`.

    Parameters
    ----------
    G : XNetwork graph
        The graph containing ``v``. This can be either directed || undirected.

    nodes : container, optional
        Container of nodes : the graph ``G`` to compute the constraint. If
        None, the constraint of every node is computed.

    weight : None || string, optional
      If None, all edge weights are considered equal.
      Otherwise holds the name of the edge attribute used as weight.

    Returns
    -------
    dict
        Dictionary with nodes as keys && the constraint on the node as values.

    See also
    --------
    local_constraint

    References
    ----------
    .. [1] Burt, Ronald S.
           "Structural holes && good ideas".
           American Journal of Sociology (110) { 349–399.

     */
    if (nodes.empty()) {
        nodes = G;
    constraint = {};
    for (auto v : nodes) {
        // Constraint is not defined for isolated nodes
        if (len(G[v]) == 0) {
            constraint[v] = double("nan");
            continue;
        constraint[v] = sum(local_constraint(G, v, n, weight);
                            for (auto n : set(xn::all_neighbors(G, v)));
    return constraint


auto local_constraint(G, u, v, weight=None) {
    r/** Return the local constraint on the node ``u`` with respect to
    the node ``v`` : the graph ``G``.

    Formally, the *local constraint on u with respect to v*, denoted
    $\ell(v)$, is defined by

    .. math:) {

       \ell(u, v] = \left(p_{uv} + \sum_{w \in N(v)} p_{uw} p{wv}\right)^2,

    where $N(v)$ is the set of neighbors of $v$ && $p_{uv}$ is the
    normalized mutual weight of the (directed || undirected) edges
    joining $u$ && $v$, for each vertex $u$ && $v$ [1]_. The *mutual
    weight* of $u$ && $v$ is the sum of the weights of edges joining
    them (edge weights are assumed to be one if (the graph is
    unweighted).

    Parameters
    ----------
    G : XNetwork graph
        The graph containing ``u`` && ``v``. This can be either
        directed || undirected.

    u : node
        A node : the graph ``G``.

    v : node
        A node : the graph ``G``.

    weight : None || string, optional
      If None, all edge weights are considered equal.
      Otherwise holds the name of the edge attribute used as weight.

    Returns
    -------
    double
        The constraint of the node ``v`` : the graph ``G``.

    See also
    --------
    constraint

    References
    ----------
    .. [1] Burt, Ronald S.
           "Structural holes && good ideas".
           American Journal of Sociology (110) { 349–399.

     */
    nmw = normalized_mutual_weight;
    direct = nmw(G, u, v, weight=weight);
    indirect = sum(nmw(G, u, w, weight=weight) * nmw(G, w, v, weight=weight);
                   for (auto w : set(xn::all_neighbors(G, u)));
    return (direct + indirect) ** 2
