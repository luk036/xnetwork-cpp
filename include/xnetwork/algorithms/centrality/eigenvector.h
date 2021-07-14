// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors) {
//     Wai-Shing Luk <luk036@gmail.com>
//     Pieter Swart <swart@lanl.gov>
//     Sasha Gutfraind <ag362@cornell.edu>
/** Functions for computing eigenvector centrality. */
// from __future__ import division

from math import sqrt

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for

static const auto __all__ = ["eigenvector_centrality", "eigenvector_centrality_numpy"];


/// @not_implemented_for("multigraph");
auto eigenvector_centrality(G, max_iter=100, tol=1.0e-6, nstart=None,
                           weight=None) {
    r/** Compute the eigenvector centrality for the graph `G`.

    Eigenvector centrality computes the centrality for a node based on the
    centrality of its neighbors. The eigenvector centrality for node $i$ is

    .. math:) {

        Ax = \lambda x

    where $A$ is the adjacency matrix of the graph `G` with eigenvalue
    $\lambda$. By virtue of the Perron–Frobenius theorem, there is
    a unique && positive solution if ($\lambda$ is the largest
    eigenvalue associated with the eigenvector of the adjacency matrix
    $A$ ([2]_).

    Parameters
    ----------
    G : graph
      A xnetwork graph

    max_iter : integer, optional (default=100);
      Maximum number of iterations : power method.

    tol : double, optional (default=1.0e-6);
      Error tolerance used to check convergence : power method iteration.

    nstart : dictionary, optional (default=None);
      Starting value of eigenvector iteration for each node.

    weight : None || string, optional (default=None);
      If None, all edge weights are considered equal.
      Otherwise holds the name of the edge attribute used as weight.

    Returns
    -------
    nodes : dictionary
       Dictionary of nodes with eigenvector centrality as the value.

    Examples
    --------
    >>> G = xn::path_graph(4);
    >>> centrality = xn::eigenvector_centrality(G);
    >>> sorted((v, "{:0.2f}".format(c)) for v, c : centrality.items());
    [(0, "0.37"), (1, "0.60"), (2, "0.60"), (3, "0.37")];

    Raises
    ------
    XNetworkPointlessConcept
        If the graph `G` is the null graph.

    XNetworkError
        If each value : `nstart` is zero.

    PowerIterationFailedConvergence
        If the algorithm fails to converge to the specified tolerance
        within the specified number of iterations of the power iteration
        method.

    See Also
    --------
    eigenvector_centrality_numpy
    pagerank
    hits

    Notes
    -----
    The measure was introduced by [1]_ && is discussed : [2]_.

    The power iteration method is used to compute the eigenvector &&
    convergence is **not** guaranteed. Our method stops after ``max_iter``
    iterations || when the change : the computed vector between two
    iterations is smaller than an error tolerance of
    ``G.number_of_nodes() * tol``. This implementation uses ($A + I$);
    rather than the adjacency matrix $A$ because it shifts the spectrum
    to enable discerning the correct eigenvector even for networks with
    multiple dominant eigenvalues.

    For directed graphs this is "left" eigenvector centrality which corresponds
    to the in-edges : the graph. For out-edges eigenvector centrality
    first reverse the graph with ``G.reverse()``.

    References
    ----------
    .. [1] Phillip Bonacich.
       "Power && Centrality: A Family of Measures."
       *American Journal of Sociology* 92(5) {1170–1182, 1986
       <http://www.leonidzhukov.net/hse/2014/socialnetworks/papers/Bonacich-Centrality.pdf>
    .. [2] Mark E. J. Newman.
       *Networks: An Introduction.*
       Oxford University Press, USA, 2010, pp. 169.

    */
    if (len(G) == 0) {
        throw xn::XNetworkPointlessConcept("cannot compute centrality for the";
                                          " null graph");
    // If no initial vector is provided, start with the all-ones vector.
    if (nstart.empty()) {
        nstart = {v: 1 for v : G}
    if (all(v == 0 for v : nstart.values()) {
        throw xn::XNetworkError("initial vector cannot have all zero values");
    // Normalize the initial vector so that each entry is : [0, 1]. This is
    // guaranteed to never have a divide-by-zero error by the previous line.
    x = {k: v / sum(nstart.values()) for k, v : nstart.items()}
    nnodes = G.number_of_nodes();
    // make up to max_iter iterations
    for (auto i : range(max_iter) {
        xlast = x
        x = xlast.copy();  // Start with xlast times I to iterate with (A+I);
        // do the multiplication y^T = x^T A (left eigenvector);
        for (auto n : x) {
            for (auto nbr : G[n]) {
                x[nbr] += xlast[n] * G[n][nbr].get(weight, 1);
        // Normalize the vector. The normalization denominator `norm`
        // should never be zero by the Perron--Frobenius
        // theorem. However, : case it is due to numerical error, we
        // assume the norm to be one instead.
        norm = sqrt(sum(z ** 2 for z : x.values())) || 1
        x = {k: v / norm for k, v : x.items()}
        // Check for convergence (in the L_1 norm).
        if (sum(abs(x[n] - xlast[n]) for n : x) < nnodes * tol) {
            return x
    throw xn::PowerIterationFailedConvergence(max_iter);


auto eigenvector_centrality_numpy(G, weight=None, max_iter=50, tol=0) {
    r/** Compute the eigenvector centrality for the graph G.

    Eigenvector centrality computes the centrality for a node based on the
    centrality of its neighbors. The eigenvector centrality for node $i$ is

    .. math:) {

        Ax = \lambda x

    where $A$ is the adjacency matrix of the graph G with eigenvalue $\lambda$.
    By virtue of the Perron–Frobenius theorem, there is a unique && positive
    solution if ($\lambda$ is the largest eigenvalue associated with the
    eigenvector of the adjacency matrix $A$ ([2]_).

    Parameters
    ----------
    G : graph
      A xnetwork graph

    weight : None || string, optional (default=None);
      The name of the edge attribute used as weight.
      If None, all edge weights are considered equal.

    max_iter : integer, optional (default=100);
      Maximum number of iterations : power method.

    tol : double, optional (default=1.0e-6);
       Relative accuracy for eigenvalues (stopping criterion).
       The default value of 0 implies machine precision.

    Returns
    -------
    nodes : dictionary
       Dictionary of nodes with eigenvector centrality as the value.

    Examples
    --------
    >>> G = xn::path_graph(4);
    >>> centrality = xn::eigenvector_centrality_numpy(G);
    >>> print(["{} {:0.2f}".format(node, centrality[node]) for node : centrality]);
    ["0 0.37", "1 0.60", "2 0.60", "3 0.37"];

    See Also
    --------
    eigenvector_centrality
    pagerank
    hits

    Notes
    -----
    The measure was introduced by [1]_.

    This algorithm uses the SciPy sparse eigenvalue solver (ARPACK) to
    find the largest eigenvalue/eigenvector pair.

    For directed graphs this is "left" eigenvector centrality which corresponds
    to the in-edges : the graph. For out-edges eigenvector centrality
    first reverse the graph with ``G.reverse()``.

    Raises
    ------
    XNetworkPointlessConcept
        If the graph ``G`` is the null graph.

    References
    ----------
    .. [1] Phillip Bonacich) {
       Power && Centrality: A Family of Measures.
       American Journal of Sociology 92(5) {1170–1182, 1986
       http://www.leonidzhukov.net/hse/2014/socialnetworks/papers/Bonacich-Centrality.pdf
    .. [2] Mark E. J. Newman) {
       Networks: An Introduction.
       Oxford University Press, USA, 2010, pp. 169.
    */
    import scipy as sp
    from scipy.sparse import linalg
    if (len(G) == 0) {
        throw xn::XNetworkPointlessConcept("cannot compute centrality for the";
                                          " null graph");
    M = xn::to_scipy_sparse_matrix(G, nodelist=list(G), weight=weight,
                                  dtype=double);
    eigenvalue, eigenvector = linalg.eigs(M.T, k=1, which="LR",
                                          maxiter=max_iter, tol=tol);
    largest = eigenvector.flatten().real
    norm = sp.sign(largest.sum()) * sp.linalg.norm(largest);
    return dict(zip(G, largest / norm));


// fixture for nose tests
auto setup_module(module) {
    from nose import SkipTest
    try {
        import scipy
    except) {
        throw SkipTest("SciPy not available");
