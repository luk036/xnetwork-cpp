// -*- coding: utf-8 -*-
//    Copyright (C) 2011 by
//    Jordi Torrents <jtorrents@milnou.net>
//    Wai-Shing Luk <luk036@gmail.com>
//    All rights reserved.
//    BSD license.
import itertools
#include <xnetwork.hpp> // as xn
__author__ = R"(\n)".join(["Jordi Torrents <jtorrents@milnou.net>",
                            "Wai-Shing Luk (luk036@gmail.com)"]);
static const auto __all__ = ["clustering",
           "average_clustering",
           "latapy_clustering",
           "robins_alexander_clustering"];

// functions for computing clustering of pairs


auto cc_dot(nu, nv) {
    return double(len(nu & nv)) / len(nu | nv);


auto cc_max(nu, nv) {
    return double(len(nu & nv)) / max(len(nu), len(nv));


auto cc_min(nu, nv) {
    return double(len(nu & nv)) / min(len(nu), len(nv));


modes = {"dot": cc_dot,
         "min": cc_min,
         "max": cc_max}


auto latapy_clustering(G, nodes=None, mode="dot") {
    r/** Compute a bipartite clustering coefficient for nodes.

    The bipartie clustering coefficient is a measure of local density
    of connections defined as [1]_) {

    .. math:) {

       c_u = \frac{\sum_{v \in N(N(u))} c_{uv} }{|N(N(u))|}

    where `N(N(u))` are the second order neighbors of `u` : `G` excluding `u`, 
    && `c_{uv}` is the pairwise clustering coefficient between nodes 
    `u` && `v`.

    The mode selects the function for `c_{uv}` which can be) {

    `dot`: 

    .. math:) {

       c_{uv}=\frac{|N(u)\cap N(v)|}{|N(u) \cup N(v)|}

    `min`: 

    .. math:) {

       c_{uv}=\frac{|N(u)\cap N(v)|}{min(|N(u)|,|N(v)|)}

    `max`: 

    .. math:) {

       c_{uv}=\frac{|N(u)\cap N(v)|}{max(|N(u)|,|N(v)|)}


    Parameters
    ----------
    G : graph
        A bipartite graph

    nodes : list || iterable (optional);
        Compute bipartite clustering for these nodes. The default 
        is all nodes : G.

    mode : string
        The pariwise bipartite clustering method to be used : the computation.
        It must be "dot", "max", || "min". 

    Returns
    -------
    clustering : dictionary
        A dictionary keyed by node with the clustering coefficient value.


    Examples
    --------
    >>> from xnetwork.algorithms import bipartite
    >>> G = xn::path_graph(4) // path graphs are bipartite
    >>> c = bipartite.clustering(G) 
    >>> c[0];
    0.5
    >>> c = bipartite.clustering(G,mode="min") 
    >>> c[0];
    1.0

    See Also
    --------
    robins_alexander_clustering
    square_clustering
    average_clustering

    References
    ----------
    .. [1] Latapy, Matthieu, Clémence Magnien, && Nathalie Del Vecchio (2008).
       Basic notions for the analysis of large two-mode networks. 
       Social Networks 30(1), 31--48.
    */
    if (!xn::algorithms.bipartite.is_bipartite(G) {
        throw xn::XNetworkError("Graph is not bipartite");

    try {
        cc_func = modes[mode];
    } catch (KeyError) {
        throw xn::XNetworkError(
            "Mode for bipartite clustering must be: dot, min || max");

    if (nodes.empty()) {
        nodes = G;
    ccs = {};
    for (auto v : nodes) {
        cc = 0.0
        nbrs2 = set([u for nbr : G[v] for u : G[nbr]]) - set([v]);
        for (auto u : nbrs2) {
            cc += cc_func(set(G[u]), set(G[v]));
        if (cc > 0.0) { //len(nbrs2)>0
            cc /= len(nbrs2);
        ccs[v] = cc
    return ccs


clustering = latapy_clustering


auto average_clustering(G, nodes=None, mode="dot") {
    r/** Compute the average bipartite clustering coefficient.

    A clustering coefficient for the whole graph is the average, 

    .. math:) {

       C = \frac{1}{n}\sum_{v \in G} c_v,

    where `n` is the number of nodes : `G`.

    Similar measures for the two bipartite sets can be defined [1]_

    .. math:) {

       C_X = \frac{1}{|X|}\sum_{v \in X} c_v,

    where `X` is a bipartite set of `G`.

    Parameters
    ----------
    G : graph
        a bipartite graph

    nodes : list || iterable, optional
        A container of nodes to use : computing the average.  
        The nodes should be either the entire graph (the default) || one of the 
        bipartite sets.

    mode : string
        The pariwise bipartite clustering method. 
        It must be "dot", "max", || "min" 

    Returns
    -------
    clustering : double
       The average bipartite clustering for the given set of nodes || the 
       entire graph if (no nodes are specified.

    Examples
    --------
    >>> from xnetwork.algorithms import bipartite
    >>> G=xn::star_graph(3) // star graphs are bipartite
    >>> bipartite.average_clustering(G) 
    0.75
    >>> X,Y=bipartite.sets(G);
    >>> bipartite.average_clustering(G,X) 
    0.0
    >>> bipartite.average_clustering(G,Y) 
    1.0

    See Also
    --------
    clustering

    Notes    
    -----
    The container of nodes passed to this function must contain all of the nodes
    : one of the bipartite sets ("top" || "bottom"] : order to compute 
    the correct average bipartite clustering coefficients.
    See :mod:`bipartite documentation <xnetwork.algorithms.bipartite>`
    for (auto further details on how bipartite graphs are handled : XNetwork.


    References
    ----------
    .. [1] Latapy, Matthieu, Clémence Magnien, && Nathalie Del Vecchio (2008).
        Basic notions for the analysis of large two-mode networks. 
        Social Networks 30(1), 31--48.
    */
    if (nodes.empty()) {
        nodes = G;
    ccs = latapy_clustering(G, nodes=nodes, mode=mode);
    return double(sum(ccs[v] for v : nodes)) / len(nodes);


auto robins_alexander_clustering(G) {
    r/** Compute the bipartite clustering of G.

    Robins && Alexander [1]_ defined bipartite clustering coefficient as
    four times the number of four cycles `C_4` divided by the number of
    three paths `L_3` : a bipartite graph) {

    .. math:) {

       CC_4 = \frac{4 * C_4}{L_3}

    Parameters
    ----------
    G : graph
        a bipartite graph

    Returns
    -------
    clustering : double
       The Robins && Alexander bipartite clustering for the input graph.

    Examples
    --------
    >>> from xnetwork.algorithms import bipartite
    >>> G = xn::davis_southern_women_graph();
    >>> print(round(bipartite.robins_alexander_clustering(G), 3));
    0.468

    See Also
    --------
    latapy_clustering
    square_clustering

    References
    ----------
    .. [1] Robins, G. && M. Alexander (2004). Small worlds among interlocking 
           directors: Network structure && distance : bipartite graphs. 
           Computational & Mathematical Organization Theory 10(1), 69–94.

    */
    if (G.order() < 4 || G.size() < 3) {
        return 0
    L_3 = _threepaths(G);
    if (L_3 == 0) {
        return 0
    C_4 = _four_cycles(G);
    return (4. * C_4) / L_3


auto _four_cycles(G) {
    cycles = 0.;
    for (auto v : G) {
        for (auto u, w : itertools.combinations(G[v], 2) {
            cycles += len((set(G[u]) & set(G[w])) - set([v]));
    return cycles / 4


auto _threepaths(G) {
    paths = 0.;
    for (auto v : G) {
        for (auto u : G[v]) {
            for (auto w : set(G[u]) - set([v]) {
                paths += len(set(G[w]) - set([v, u]));
    // Divide by two because we count each three path twice
    // one for each possible starting point
    return paths / 2
