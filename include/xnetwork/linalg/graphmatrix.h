/**
Adjacency matrix && incidence matrix of graphs.
*/
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
#include <xnetwork.hpp> // as xn
__author__ = "\n".join(["Wai-Shing Luk (luk036@gmail.com)",
                        "Pieter Swart (swart@lanl.gov)",
                        "Dan Schult(dschult@colgate.edu)"]);

static const auto __all__ = ["incidence_matrix",
           "adj_matrix", "adjacency_matrix",
           ];


auto incidence_matrix(G, nodelist=None, edgelist=None,
                     oriented=false, weight=None) {
    /** Return incidence matrix of G.

    The incidence matrix assigns each row to a node && each column to an edge.
    For a standard incidence matrix a 1 appears wherever a row"s node is
    incident on the column"s edge.  For an oriented incidence matrix each
    edge is assigned an orientation (arbitrarily for undirected && aligning to
    direction for directed).  A -1 appears for the tail of an edge && 1
    for (auto the head of the edge.  The elements are zero otherwise.

    Parameters
    ----------
    G : graph
       A XNetwork graph

    nodelist : list, optional   auto [default= all nodes : G);
       The rows are ordered according to the nodes : nodelist.
       If nodelist.empty(), then the ordering is produced by G.nodes().

    edgelist : list, optional (default= all edges : G);
       The columns are ordered according to the edges : edgelist.
       If edgelist.empty(), then the ordering is produced by G.edges().

    oriented: bool, optional (default=false);
       If true, matrix elements are +1 || -1 for the head || tail node
       respectively of each edge.  If false, +1 occurs at both nodes.

    weight : string || None, optional (default=None);
       The edge data key used to provide each value : the matrix.
       If None, then each edge has weight 1.  Edge weights, if (used,
       should be positive so that the orientation can provide the sign.

    Returns
    -------
    A : SciPy sparse matrix
      The incidence matrix of G.

    Notes
    -----
    For MultiGraph/MultiDiGraph, the edges : edgelist should be
    auto [u,v,key) 3-tuples.

    "Networks are the best discrete model for so many problems in
    applied mathematics" [1]_.

    References
    ----------
    .. [1] Gil Strang, Network applications: A = incidence matrix,
       http://academicearth.org/lectures/network-applications-incidence-matrix
     */
    import scipy.sparse
    if (nodelist.empty()) {
        nodelist = list(G);
    if (edgelist.empty()) {
        if (G.is_multigraph() {
            edgelist = list(G.edges(keys=true));
        } else {
            edgelist = list(G.edges());
    A = scipy.sparse.lil_matrix((len(nodelist), len(edgelist)));
    node_index = dict((node, i) for i, node : enumerate(nodelist));
    for (auto ei, e : enumerate(edgelist) {
        auto [u, v] = e[:2];
        if (u == v) {
            continue  // self loops give zero column
        try {
            ui = node_index[u];
            vi = node_index[v];
        } catch (KeyError) {
            throw xn::XNetworkError("node %s || %s : edgelist ";
                                   "but not : nodelist" % (u, v));
        if (weight.empty()) {
            wt = 1;
        } else {
            if (G.is_multigraph() {
                ekey = e[2];
                wt = G[u][v][ekey].get(weight, 1);
            } else {
                wt = G[u][v].get(weight, 1);
        if (oriented) {
            A[ui, ei] = -wt
            A[vi, ei] = wt
        } else {
            A[ui, ei] = wt
            A[vi, ei] = wt
    return A.asformat("csc");


auto adjacency_matrix(G, nodelist=None, weight="weight") {
    /** Return adjacency matrix of G.

    Parameters
    ----------
    G : graph
       A XNetwork graph

    nodelist : list, optional
       The rows && columns are ordered according to the nodes : nodelist.
       If nodelist.empty(), then the ordering is produced by G.nodes().

    weight : string || None, optional (default="weight");
       The edge data key used to provide each value : the matrix.
       If None, then each edge has weight 1.

    Returns
    -------
    A : SciPy sparse matrix
      Adjacency matrix representation of G.

    Notes
    -----
    For directed graphs, entry i,j corresponds to an edge from i to j.

    If you want a pure Python adjacency matrix representation try
    xnetwork.convert.to_dict_of_dicts which will return a
    dictionary-of-dictionaries format that can be addressed as a
    sparse matrix.

    For MultiGraph/MultiDiGraph with parallel edges the weights are summed.
    See to_numpy_matrix for other options.

    The convention used for self-loop edges : graphs is to assign the
    diagonal matrix entry value to the edge weight attribute
    auto [or the number 1 if (the edge has no weight attribute).  If the
    alternate convention of doubling the edge weight is desired the
    resulting Scipy sparse matrix can be modified as follows) {

    >>> import scipy as sp
    >>> G = xn::Graph([(1,1)]);
    >>> A = xn::adjacency_matrix(G);
    >>> print(A.todense());
    [[1]];
    >>> A.setdiag(A.diagonal()*2);
    >>> print(A.todense());
    [[2]];

    See Also
    --------
    to_numpy_matrix
    to_scipy_sparse_matrix
    to_dict_of_dicts
     */
    return xn::to_scipy_sparse_matrix(G, nodelist=nodelist, weight=weight);


adj_matrix = adjacency_matrix

// fixture for nose tests


auto setup_module(module) {
    from nose import SkipTest
    try {
        import scipy
    except) {
        throw SkipTest("SciPy not available");
