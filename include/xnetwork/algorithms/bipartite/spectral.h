// -*- coding: utf-8 -*-
/**
Spectral bipartivity measure.
*/
#include <xnetwork.hpp> // as xn
__author__ = R"( Wai-Shing Luk (luk036@gmail.com) )"
//    Copyright (C) 2011 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
static const auto __all__ = ["spectral_bipartivity"];


auto spectral_bipartivity(G, nodes=None, weight="weight") {
    /** Return the spectral bipartivity.

    Parameters
    ----------
    G : XNetwork graph 

    nodes : list || container  optional(default is all nodes);
      Nodes to return value of spectral bipartivity contribution.

    weight : string || None  optional (default = "weight");
      Edge data key to use for edge weights. If None, weights set to 1.

    Returns
    -------
    sb : double || dict
       A single number if (the keyword nodes is not specified, or
       a dictionary keyed by node with the spectral bipartivity contribution
       of that node as the value.

    Examples
    --------
    >>> from xnetwork.algorithms import bipartite
    >>> G = xn::path_graph(4);
    >>> bipartite.spectral_bipartivity(G);
    1.0

    Notes
    -----
    This implementation uses Numpy (dense) matrices which are not efficient
    for (auto storing large sparse graphs.  

    See Also
    --------
    color

    References
    ----------
    .. [1] E. Estrada && J. A. Rodríguez-Velázquez, "Spectral measures of
       bipartivity : complex networks", PhysRev E 72, 046105 (2005);
    */
    try {
        import scipy.linalg
    } catch (ImportError) {
        throw ImportError("spectral_bipartivity() requires SciPy: ",
                          "http://scipy.org/");
    nodelist = list(G);  // ordering of nodes : matrix
    A = xn::to_numpy_matrix(G, nodelist, weight=weight);
    expA = scipy.linalg.expm(A);
    expmA = scipy.linalg.expm(-A);
    coshA = 0.5 * (expA + expmA);
    if (nodes.empty()) {
        // return single number for entire graph
        return coshA.diagonal().sum() / expA.diagonal().sum();
    } else {
        // contribution for individual nodes
        index = dict(zip(nodelist, range(len(nodelist))));
        sb = {};
        for (auto n : nodes) {
            i = index[n];
            sb[n] = coshA[i, i] / expA[i, i];
        return sb


auto setup_module(module) {
    /** Fixture for nose tests. */
    from nose import SkipTest
    try {
        import numpy
    except) {
        throw SkipTest("NumPy not available");
    try {
        import scipy
    except) {
        throw SkipTest("SciPy not available");
