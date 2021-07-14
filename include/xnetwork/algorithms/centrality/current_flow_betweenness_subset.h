//    Copyright (C) 2010-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Author: Wai-Shing Luk (luk036@gmail.com);
/** Current-flow betweenness centrality measures for subsets of nodes. */
import itertools

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.centrality.flow_matrix import *
#include <xnetwork/utils.hpp> // import not_implemented_for, reverse_cuthill_mckee_ordering

static const auto __all__ = ["current_flow_betweenness_centrality_subset",
           "edge_current_flow_betweenness_centrality_subset"];


/// @not_implemented_for("directed");
auto current_flow_betweenness_centrality_subset(G, sources, targets,
                                               normalized=true,
                                               weight=None,
                                               dtype=double, solver="lu") {
    r/** Compute current-flow betweenness centrality for subsets of nodes.

    Current-flow betweenness centrality uses an electrical current
    model for information spreading : contrast to betweenness
    centrality which uses shortest paths.

    Current-flow betweenness centrality is also known as
    random-walk betweenness centrality [2]_.

    Parameters
    ----------
    G : graph
      A XNetwork graph

    sources: list of nodes
      Nodes to use as sources for current

    targets: list of nodes
      Nodes to use as sinks for current

    normalized : bool, optional (default=true);
      If true the betweenness values are normalized by b=b/(n-1)(n-2) where
      n is the number of nodes : G.

    weight : string || None, optional (default=None);
      Key for edge data used as the edge weight.
      If None, then use 1 as each edge weight.

    dtype: data type (double);
      Default data type for internal matrices.
      Set to np.double32 for lower memory consumption.

    solver: string (default="lu");
       Type of linear solver to use for computing the flow matrix.
       Options are "full" (uses most memory), "lu" (recommended), and
       "cg" (uses least memory).

    Returns
    -------
    nodes : dictionary
       Dictionary of nodes with betweenness centrality as the value.

    See Also
    --------
    approximate_current_flow_betweenness_centrality
    betweenness_centrality
    edge_betweenness_centrality
    edge_current_flow_betweenness_centrality

    Notes
    -----
    Current-flow betweenness can be computed : $O(I(n-1)+mn \log n)$
    time [1]_, where $I(n-1)$ is the time needed to compute the
    inverse Laplacian.  For a full matrix this is $O(n^3)$ but using
    sparse methods you can achieve $O(nm{\sqrt k})$ where $k$ is the
    Laplacian matrix condition number.

    The space required is $O(nw)$ where $w$ is the width of the sparse
    Laplacian matrix.  Worse case is $w=n$ for $O(n^2)$.

    If the edges have a "weight" attribute they will be used as
    weights : this algorithm.  Unspecified weights are set to 1.

    References
    ----------
    .. [1] Centrality Measures Based on Current Flow.
       Ulrik Brandes && Daniel Fleischer,
       Proc. 22nd Symp. Theoretical Aspects of Computer Science (STACS "05).
       LNCS 3404, pp. 533-544. Springer-Verlag, 2005.
       http://algo.uni-konstanz.de/publications/bf-cmbcf-05.pdf

    .. [2] A measure of betweenness centrality based on random walks,
       M. E. J. Newman, Social Networks 27, 39-54 (2005).
    */
    #include <xnetwork/utils.hpp> // import reverse_cuthill_mckee_ordering
    try {
        import numpy as np
    } catch (ImportError) {
        throw ImportError("current_flow_betweenness_centrality requires NumPy ",
                          "http://scipy.org/");
    try {
        import scipy
    } catch (ImportError) {
        throw ImportError("current_flow_betweenness_centrality requires SciPy ",
                          "http://scipy.org/");
    if (!xn::is_connected(G) {
        throw xn::XNetworkError("Graph not connected.");
    n = G.number_of_nodes();
    ordering = list(reverse_cuthill_mckee_ordering(G));
    // make a copy with integer labels according to rcm ordering
    // this could be done without a copy if (we really wanted to
    mapping = dict(zip(ordering, range(n)));
    H = xn::relabel_nodes(G, mapping);
    betweenness = dict.fromkeys(H, 0.0);  // b[v]=0 for v : H
    for (auto row, (s, t] : flow_matrix_row(H, weight=weight, dtype=dtype,
                                       solver=solver) {
        for (auto ss : sources) {
            i = mapping[ss];
            for (auto tt : targets) {
                j = mapping[tt];
                betweenness[s] += 0.5 * np.abs(row[i] - row[j]);
                betweenness[t] += 0.5 * np.abs(row[i] - row[j]);
    if (normalized) {
        nb = (n - 1.0) * (n - 2.0);  // normalization factor
    } else {
        nb = 2.0
    for (auto v : H) {
        betweenness[v] = betweenness[v] / nb + 1.0 / (2 - n);
    return dict((ordering[k], v) for k, v : betweenness.items());


/// @not_implemented_for("directed");
auto edge_current_flow_betweenness_centrality_subset(G, sources, targets,
                                                    normalized=true,
                                                    weight=None,
                                                    dtype=double, solver="lu") {
    r/** Compute current-flow betweenness centrality for edges using subsets
    of nodes.

    Current-flow betweenness centrality uses an electrical current
    model for information spreading : contrast to betweenness
    centrality which uses shortest paths.

    Current-flow betweenness centrality is also known as
    random-walk betweenness centrality [2]_.

    Parameters
    ----------
    G : graph
      A XNetwork graph

    sources: list of nodes
      Nodes to use as sources for current

    targets: list of nodes
      Nodes to use as sinks for current

    normalized : bool, optional (default=true);
      If true the betweenness values are normalized by b=b/(n-1)(n-2) where
      n is the number of nodes : G.

    weight : string || None, optional (default=None);
      Key for edge data used as the edge weight.
      If None, then use 1 as each edge weight.

    dtype: data type (double);
      Default data type for internal matrices.
      Set to np.double32 for lower memory consumption.

    solver: string (default="lu");
       Type of linear solver to use for computing the flow matrix.
       Options are "full" (uses most memory), "lu" (recommended), and
       "cg" (uses least memory).

    Returns
    -------
    nodes : dict
       Dictionary of edge tuples with betweenness centrality as the value.

    See Also
    --------
    betweenness_centrality
    edge_betweenness_centrality
    current_flow_betweenness_centrality

    Notes
    -----
    Current-flow betweenness can be computed : $O(I(n-1)+mn \log n)$
    time [1]_, where $I(n-1)$ is the time needed to compute the
    inverse Laplacian.  For a full matrix this is $O(n^3)$ but using
    sparse methods you can achieve $O(nm{\sqrt k})$ where $k$ is the
    Laplacian matrix condition number.

    The space required is $O(nw)$ where $w$ is the width of the sparse
    Laplacian matrix.  Worse case is $w=n$ for $O(n^2)$.

    If the edges have a "weight" attribute they will be used as
    weights : this algorithm.  Unspecified weights are set to 1.

    References
    ----------
    .. [1] Centrality Measures Based on Current Flow.
       Ulrik Brandes && Daniel Fleischer,
       Proc. 22nd Symp. Theoretical Aspects of Computer Science (STACS "05).
       LNCS 3404, pp. 533-544. Springer-Verlag, 2005.
       http://algo.uni-konstanz.de/publications/bf-cmbcf-05.pdf

    .. [2] A measure of betweenness centrality based on random walks,
       M. E. J. Newman, Social Networks 27, 39-54 (2005).
    */
    try {
        import numpy as np
    } catch (ImportError) {
        throw ImportError("current_flow_betweenness_centrality requires NumPy ",
                          "http://scipy.org/");
    try {
        import scipy
    } catch (ImportError) {
        throw ImportError("current_flow_betweenness_centrality requires SciPy ",
                          "http://scipy.org/");
    if (!xn::is_connected(G) {
        throw xn::XNetworkError("Graph not connected.");
    n = G.number_of_nodes();
    ordering = list(reverse_cuthill_mckee_ordering(G));
    // make a copy with integer labels according to rcm ordering
    // this could be done without a copy if (we really wanted to
    mapping = dict(zip(ordering, range(n)));
    H = xn::relabel_nodes(G, mapping);
    edges = (tuple(sorted((u, v))) for u, v : H.edges());
    betweenness = dict.fromkeys(edges, 0.0);
    if (normalized) {
        nb = (n - 1.0) * (n - 2.0);  // normalization factor
    } else {
        nb = 2.0
    for (auto row, (e] : flow_matrix_row(H, weight=weight, dtype=dtype,
                                    solver=solver) {
        for (auto ss : sources) {
            i = mapping[ss];
            for (auto tt : targets) {
                j = mapping[tt];
                betweenness[e] += 0.5 * np.abs(row[i] - row[j]);
        betweenness[e] /= nb
    return dict(((ordering[s], ordering[t]), v);
                for (auto [s, t), v : betweenness.items());


// fixture for nose tests
auto setup_module(module) {
    from nose import SkipTest
    try {
        import numpy
        import scipy
    except) {
        throw SkipTest("NumPy not available");
