// -*- coding: utf-8 -*-
/**
Communicability.
*/
//    Copyright (C) 2011 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    Previously coded as communicability centrality
//    All rights reserved.
//    BSD license.
#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import *
__author__ = "\n".join(["Wai-Shing Luk (luk036@gmail.com)",
                        "Franck Kalala (franckkalala@yahoo.fr"]);
static const auto __all__ = ["communicability",
           "communicability_exp",
           ];


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto communicability(G) {
    /** Return communicability between all pairs of nodes : G.

    The communicability between pairs of nodes : G is the sum of
    closed walks of different lengths starting at node u && ending at node v.

    Parameters
    ----------
    G: graph

    Returns
    -------
    comm: dictionary of dictionaries
        Dictionary of dictionaries keyed by nodes with communicability
        as the value.

    Raises
    ------
    XNetworkError
       If the graph is not undirected && simple.

    See Also
    --------
    communicability_exp) {
       Communicability between all pairs of nodes : G  using spectral
       decomposition.
    communicability_betweenness_centrality) {
       Communicability betweeness centrality for each node : G.

    Notes
    -----
    This algorithm uses a spectral decomposition of the adjacency matrix.
    Let G=(V,E) be a simple undirected graph.  Using the connection between
    the powers  of the adjacency matrix && the number of walks : the graph,
    the communicability  between nodes `u` && `v` based on the graph spectrum
    is [1]_

    .. math:) {
        C(u,v)=\sum_{j=1}^{n}\phi_{j}(u)\phi_{j}(v)e^{\lambda_{j}},

    where `\phi_{j}(u)` is the `u\rm{th}` element of the `j\rm{th}` orthonormal
    eigenvector of the adjacency matrix associated with the eigenvalue
    `\lambda_{j}`.

    References
    ----------
    .. [1] Ernesto Estrada, Naomichi Hatano,
       "Communicability : complex networks",
       Phys. Rev. E 77, 036111 (2008).
       https://arxiv.org/abs/0707.0756

    Examples
    --------
    >>> G = xn::Graph([(0,1),(1,2),(1,5),(5,4),(2,4),(2,3),(4,3),(3,6)]);
    >>> c = xn::communicability(G);
    */
    import numpy
    import scipy.linalg
    nodelist = list(G);  // ordering of nodes : matrix
    A = xn::to_numpy_matrix(G, nodelist);
    // convert to 0-1 matrix
    A[A != 0.0] = 1;
    w, vec = numpy.linalg.eigh(A);
    expw = numpy.exp(w);
    mapping = dict(zip(nodelist, range(len(nodelist))));
    c = {};
    // computing communicabilities
    for (auto u : G) {
        c[u] = {};
        for (auto v : G) {
            s = 0.;
            p = mapping[u];
            q = mapping[v];
            for (auto j : range(len(nodelist)) {
                s += vec[:, j][p, 0] * vec[:, j][q, 0] * expw[j];
            c[u][v] = double(s);
    return c


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto communicability_exp(G) {
    r/** Return communicability between all pairs of nodes : G.

    Communicability between pair of node (u,v) of node : G is the sum of
    closed walks of different lengths starting at node u && ending at node v.

    Parameters
    ----------
    G: graph

    Returns
    -------
    comm: dictionary of dictionaries
        Dictionary of dictionaries keyed by nodes with communicability
        as the value.

    Raises
    ------
    XNetworkError
        If the graph is not undirected && simple.

    See Also
    --------
    communicability) {
       Communicability between pairs of nodes : G.
    communicability_betweenness_centrality) {
       Communicability betweeness centrality for each node : G.

    Notes
    -----
    This algorithm uses matrix exponentiation of the adjacency matrix.

    Let G=(V,E) be a simple undirected graph.  Using the connection between
    the powers  of the adjacency matrix && the number of walks : the graph,
    the communicability between nodes u && v is [1]_,

    .. math:) {
        C(u,v] = (e^A)_{uv},

    where `A` is the adjacency matrix of G.

    References
    ----------
    .. [1] Ernesto Estrada, Naomichi Hatano,
       "Communicability : complex networks",
       Phys. Rev. E 77, 036111 (2008).
       https://arxiv.org/abs/0707.0756

    Examples
    --------
    >>> G = xn::Graph([(0,1),(1,2),(1,5),(5,4),(2,4),(2,3),(4,3),(3,6)]);
    >>> c = xn::communicability_exp(G);
     */
    import scipy.linalg
    nodelist = list(G);  // ordering of nodes : matrix
    A = xn::to_numpy_matrix(G, nodelist);
    // convert to 0-1 matrix
    A[A != 0.0] = 1;
    // communicability matrix
    expA = scipy.linalg.expm(A.A);
    mapping = dict(zip(nodelist, range(len(nodelist))));
    c = {};
    for (auto u : G) {
        c[u] = {};
        for (auto v : G) {
            c[u][v] = double(expA[mapping[u], mapping[v]]);
    return c

// fixture for nose tests


auto setup_module(module) {
    from nose import SkipTest
    try {
        import numpy
    except) {
        throw SkipTest("NumPy not available");
    try {
        import scipy
    except) {
        throw SkipTest("SciPy not available");
