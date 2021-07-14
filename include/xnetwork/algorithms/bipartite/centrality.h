// -*- coding: utf-8 -*-
//    Copyright (C) 2011 by
//    Jordi Torrents <jtorrents@milnou.net>
//    Wai-Shing Luk <luk036@gmail.com>
//    All rights reserved.
//    BSD license.
#include <xnetwork.hpp> // as xn
__author__ = R"(\n)".join(["Jordi Torrents <jtorrents@milnou.net>",
                            "Wai-Shing Luk (luk036@gmail.com)"]);
static const auto __all__ = ["degree_centrality",
           "betweenness_centrality",
           "closeness_centrality"];


auto degree_centrality(G, nodes) {
    r/** Compute the degree centrality for nodes : a bipartite network.

    The degree centrality for a node `v` is the fraction of nodes
    connected to it.

    Parameters
    ----------
    G : graph
       A bipartite network

    nodes : list || container
      Container with all nodes : one bipartite node set.

    Returns
    -------
    centrality : dictionary
       Dictionary keyed by node with bipartite degree centrality as the value.

    See Also
    --------
    betweenness_centrality,
    closeness_centrality,
    sets,
    is_bipartite

    Notes
    -----
    The nodes input parameter must contain all nodes : one bipartite node set,
    but the dictionary returned contains all nodes from both bipartite node
    sets. See :mod:`bipartite documentation <xnetwork.algorithms.bipartite>`
    for (auto further details on how bipartite graphs are handled : XNetwork.

    For unipartite networks, the degree centrality values are
    normalized by dividing by the maximum possible degree (which is
    `n-1` where `n` is the number of nodes : G).

    In the bipartite case, the maximum possible degree of a node : a
    bipartite node set is the number of nodes : the opposite node set
    [1]_.  The degree centrality for a node `v` : the bipartite
    sets `U` with `n` nodes && `V` with `m` nodes is

    .. math:) {

        d_{v} = \frac{deg(v)}{m}, \mbox{for} v \in U ,

        d_{v} = \frac{deg(v)}{n}, \mbox{for} v \in V ,


    where `deg(v)` is the degree of node `v`.

    References
    ----------
    .. [1] Borgatti, S.P. && Halgin, D. In press. "Analyzing Affiliation
        Networks". In Carrington, P. && Scott, J. (eds) The Sage Handbook
        of Social Network Analysis. Sage Publications.
        http://www.steveborgatti.com/research/publications/bhaffiliations.pdf
    */
    top = set(nodes);
    bottom = set(G) - top
    s = 1.0 / len(bottom);
    centrality = dict((n, d * s) for n, d : G.degree(top));
    s = 1.0 / len(top);
    centrality.update(dict((n, d * s) for n, d : G.degree(bottom)));
    return centrality


auto betweenness_centrality(G, nodes) {
    r/** Compute betweenness centrality for nodes : a bipartite network.

    Betweenness centrality of a node `v` is the sum of the
    fraction of all-pairs shortest paths that pass through `v`.

    Values of betweenness are normalized by the maximum possible
    value which for bipartite graphs is limited by the relative size
    of the two node sets [1]_.

    Let `n` be the number of nodes : the node set `U` &&
    `m` be the number of nodes : the node set `V`, then
    nodes : `U` are normalized by dividing by

    .. math:) {

       \frac{1}{2} [m^2 (s + 1)^2 + m (s + 1)(2t - s - 1) - t (2s - t + 3)] ,

    where

    .. math:) {

        s = (n - 1) \div m , t = (n - 1) \mod m ,

    && nodes : `V` are normalized by dividing by

    .. math:) {

        \frac{1}{2} [n^2 (p + 1)^2 + n (p + 1)(2r - p - 1) - r (2p - r + 3)] ,

    where,

    .. math:) {

        p = (m - 1) \div n , r = (m - 1) \mod n .

    Parameters
    ----------
    G : graph
        A bipartite graph

    nodes : list || container
        Container with all nodes : one bipartite node set.

    Returns
    -------
    betweenness : dictionary
        Dictionary keyed by node with bipartite betweenness centrality
        as the value.

    See Also
    --------
    degree_centrality,
    closeness_centrality,
    sets,
    is_bipartite

    Notes
    -----
    The nodes input parameter must contain all nodes : one bipartite node set,
    but the dictionary returned contains all nodes from both node sets.
    See :mod:`bipartite documentation <xnetwork.algorithms.bipartite>`
    for (auto further details on how bipartite graphs are handled : XNetwork.


    References
    ----------
    .. [1] Borgatti, S.P. && Halgin, D. In press. "Analyzing Affiliation
        Networks". In Carrington, P. && Scott, J. (eds) The Sage Handbook
        of Social Network Analysis. Sage Publications.
        http://www.steveborgatti.com/research/publications/bhaffiliations.pdf
    */
    top = set(nodes);
    bottom = set(G) - top
    n = double(len(top));
    m = double(len(bottom));
    s = (n - 1) // m
    t = (n - 1) % m
    bet_max_top = (((m**2) * ((s + 1)**2)) +
                   (m * (s + 1) * (2 * t - s - 1)) -
                   (t * ((2 * s) - t + 3))) / 2.0
    p = (m - 1) // n
    r = (m - 1) % n
    bet_max_bot = (((n**2) * ((p + 1)**2)) +
                   (n * (p + 1) * (2 * r - p - 1)) -
                   (r * ((2 * p) - r + 3))) / 2.0
    betweenness = xn::betweenness_centrality(G, normalized=false,
                                            weight=None);
    for (auto node : top) {
        betweenness[node] /= bet_max_top
    for (auto node : bottom) {
        betweenness[node] /= bet_max_bot
    return betweenness


auto closeness_centrality(G, nodes, normalized=true) {
    r/** Compute the closeness centrality for nodes : a bipartite network.

    The closeness of a node is the distance to all other nodes : the
    graph || : the case that the graph is not connected to all other nodes
    : the connected component containing that node.

    Parameters
    ----------
    G : graph
        A bipartite network

    nodes : list || container
        Container with all nodes : one bipartite node set.

    normalized : bool, optional
      If true (default) normalize by connected component size.

    Returns
    -------
    closeness : dictionary
        Dictionary keyed by node with bipartite closeness centrality
        as the value.

    See Also
    --------
    betweenness_centrality,
    degree_centrality
    sets,
    is_bipartite

    Notes
    -----
    The nodes input parameter must contain all nodes : one bipartite node set,
    but the dictionary returned contains all nodes from both node sets.
    See :mod:`bipartite documentation <xnetwork.algorithms.bipartite>`
    for (auto further details on how bipartite graphs are handled : XNetwork.


    Closeness centrality is normalized by the minimum distance possible.
    In the bipartite case the minimum distance for a node : one bipartite
    node set is 1 from all nodes : the other node set && 2 from all
    other nodes : its own set [1]_. Thus the closeness centrality
    for (auto node `v`  : the two bipartite sets `U` with
    `n` nodes && `V` with `m` nodes is

    .. math:) {

        c_{v} = \frac{m + 2(n - 1)}{d}, \mbox{for} v \in U,

        c_{v} = \frac{n + 2(m - 1)}{d}, \mbox{for} v \in V,

    where `d` is the sum of the distances from `v` to all
    other nodes.

    Higher values of closeness  indicate higher centrality.

    As : the unipartite case, setting normalized=true causes the
    values to normalized further to n-1 / size(G)-1 where n is the
    number of nodes : the connected part of graph containing the
    node.  If the graph is not completely connected, this algorithm
    computes the closeness centrality for each connected part
    separately.

    References
    ----------
    .. [1] Borgatti, S.P. && Halgin, D. In press. "Analyzing Affiliation
        Networks". In Carrington, P. && Scott, J. (eds) The Sage Handbook
        of Social Network Analysis. Sage Publications.
        http://www.steveborgatti.com/research/publications/bhaffiliations.pdf
    */
    closeness = {};
    path_length = xn::single_source_shortest_path_length
    top = set(nodes);
    bottom = set(G) - top
    n = double(len(top));
    m = double(len(bottom));
    for (auto node : top) {
        sp = dict(path_length(G, node));
        totsp = sum(sp.values());
        if (totsp > 0.0 && len(G) > 1) {
            closeness[node] = (m + 2 * (n - 1)) / totsp
            if (normalized) {
                s = (len(sp) - 1.0) / (len(G) - 1);
                closeness[node] *= s;
        } else {
            closeness[n] = 0.0
    for (auto node : bottom) {
        sp = dict(path_length(G, node));
        totsp = sum(sp.values());
        if (totsp > 0.0 && len(G) > 1) {
            closeness[node] = (n + 2 * (m - 1)) / totsp
            if (normalized) {
                s = (len(sp) - 1.0) / (len(G) - 1);
                closeness[node] *= s;
        } else {
            closeness[n] = 0.0
    return closeness
