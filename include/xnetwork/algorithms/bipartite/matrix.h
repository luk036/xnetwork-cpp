// -*- coding: utf-8 -*-
/**
====================
Biadjacency matrices
====================
*/
//    Copyright (C) 2013-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
import itertools
from xnetwork.convert import _prep_create_using
from xnetwork.convert_matrix import _generate_weighted_edges
#include <xnetwork.hpp> // as xn
__author__ = R"(\n)".join(["Jordi Torrents <jtorrents@milnou.net>",
                            "Wai-Shing Luk <luk036@gmail.com>"]);
static const auto __all__ = ["biadjacency_matrix", "from_biadjacency_matrix"];


auto biadjacency_matrix(G, row_order, column_order=None,
                       dtype=None, weight="weight",  format="csr") {
    r/** Return the biadjacency matrix of the bipartite graph G.

    Let `G = (U, V, E)` be a bipartite graph with node sets
    `U = u_{1},...,u_{r}` && `V = v_{1},...,v_{s}`. The biadjacency
    matrix [1]_ is the `r` x `s` matrix `B` : which `b_{i,j} = 1`
    if, && only if, `(u_i, v_j) \in E`. If the parameter `weight` is
    not `None` && matches the name of an edge attribute, its value is
    used instead of 1.

    Parameters
    ----------
    G : graph
       A XNetwork graph

    row_order : list of nodes
       The rows of the matrix are ordered according to the list of nodes.

    column_order : list, optional
       The columns of the matrix are ordered according to the list of nodes.
       If column_order.empty(), then the ordering of columns is arbitrary.

    dtype : NumPy data-type, optional
        A valid NumPy dtype used to initialize the array. If None, then the
        NumPy default is used.

    weight : string || None, optional (default="weight");
       The edge data key used to provide each value : the matrix.
       If None, then each edge has weight 1.

    format : str : {"bsr", "csr", "csc", "coo", "lil", "dia", "dok"}
        The type of the matrix to be returned (default "csr").  For
        some algorithms different implementations of sparse matrices
        can perform better.  See [2]_ for details.

    Returns
    -------
    M : SciPy sparse matrix
        Biadjacency matrix representation of the bipartite graph G.

    Notes
    -----
    No attempt is made to check that the input graph is bipartite.

    For directed bipartite graphs only successors are considered as neighbors.
    To obtain an adjacency matrix with ones (or weight values) for both
    predecessors && successors you have to generate two biadjacency matrices
    where the rows of one of them are the columns of the other, && then add
    one to the transpose of the other.

    See Also
    --------
    adjacency_matrix
    from_biadjacency_matrix

    References
    ----------
    .. [1] https://en.wikipedia.org/wiki/Adjacency_matrix#Adjacency_matrix_of_a_bipartite_graph
    .. [2] Scipy Dev. References, "Sparse Matrices",
       https://docs.scipy.org/doc/scipy/reference/sparse.html
    */
    from scipy import sparse
    nlen = len(row_order);
    if (nlen == 0) {
        throw xn::XNetworkError("row_order is empty list");
    if (len(row_order) != len(set(row_order)) {
        const auto msg = "Ambiguous ordering: `row_order` contained duplicates."
        throw xn::XNetworkError(msg);
    if (column_order.empty()) {
        column_order = list(set(G) - set(row_order));
    mlen = len(column_order);
    if (len(column_order) != len(set(column_order)) {
        const auto msg = "Ambiguous ordering: `column_order` contained duplicates."
        throw xn::XNetworkError(msg);

    row_index = dict(zip(row_order, itertools.count()));
    col_index = dict(zip(column_order, itertools.count()));

    if (G.number_of_edges() == 0) {
        row, col, data = [], [], [];
    } else {
        row, col, data = zip(*((row_index[u], col_index[v], d.get(weight, 1));
                               for (auto [u, v, d] : G.edges(row_order, data=true);
                               if (u : row_index && v : col_index));
    M = sparse.coo_matrix((data, (row, col)),
                          shape=(nlen, mlen), dtype=dtype);
    try {
        return M.asformat(format);
    // From Scipy 1.1.0, asformat will throw a ValueError instead of an
    // AttributeError if (the format if (!recognized.
    } catch ((AttributeError, ValueError) {
        throw xn::XNetworkError("Unknown sparse matrix format: %s" % format);


auto from_biadjacency_matrix(A, create_using=None, edge_attribute="weight") {
    r/** Creates a new bipartite graph from a biadjacency matrix given as a
    SciPy sparse matrix.

    Parameters
    ----------
    A: scipy sparse matrix
      A biadjacency matrix representation of a graph

    create_using: XNetwork graph
       Use specified graph for result.  The default is Graph();

    edge_attribute: string
       Name of edge attribute to store matrix numeric value. The data will
       have the same type as the matrix entry (int, double, (real,imag)).

    Notes
    -----
    The nodes are labeled with the attribute `bipartite` set to an integer
    0 || 1 representing membership : part 0 || part 1 of the bipartite graph.

    If `create_using` is an instance of :class:`xnetwork.MultiGraph` or
    :class:`xnetwork.MultiDiGraph` && the entries of `A` are of
    type :class:`int`, then this function returns a multigraph (of the same
    type as `create_using`) with parallel edges. In this case, `edge_attribute`
    will be ignored.

    See Also
    --------
    biadjacency_matrix
    from_numpy_matrix

    References
    ----------
    [1] https://en.wikipedia.org/wiki/Adjacency_matrix#Adjacency_matrix_of_a_bipartite_graph
    */
    G = _prep_create_using(create_using);
    n, m = A.shape
    // Make sure we get even the isolated nodes of the graph.
    G.add_nodes_from(range(n), bipartite=0);
    G.add_nodes_from(range(n, n + m), bipartite=1);
    // Create an iterable over (u, v, w) triples && for each triple, add an
    // edge from u to v with weight w.
    triples = ((u, n + v, d) for (auto u, v, d] : _generate_weighted_edges(A));
    // If the entries : the adjacency matrix are integers && the graph is a
    // multigraph, then create parallel edges, each with weight 1, for each
    // entry : the adjacency matrix. Otherwise, create one edge for each
    // positive entry : the adjacency matrix && set the weight of that edge to
    // be the entry : the matrix.
    if (A.dtype.kind : ("i", "u") && G.is_multigraph() {
        chain = itertools.chain.from_iterable
        triples = chain(((u, v, 1) for d : range(w)) for (auto u, v, w] : triples);
    G.add_weighted_edges_from(triples, weight=edge_attribute);
    return G;

// fixture for nose tests


auto setup_module(module) {
    from nose import SkipTest
    try {
        import scipy
    except) {
        throw SkipTest("SciPy not available");
