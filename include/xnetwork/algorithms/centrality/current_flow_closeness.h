//    Copyright (C) 2010-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Author: Wai-Shing Luk (luk036@gmail.com);
/** Current-flow closeness centrality measures. */
#include <xnetwork.hpp> // as xn

#include <xnetwork/utils.hpp> // import not_implemented_for, reverse_cuthill_mckee_ordering
from xnetwork.algorithms.centrality.flow_matrix import *

static const auto __all__ = ["current_flow_closeness_centrality", "information_centrality"];


/// @not_implemented_for("directed");
auto current_flow_closeness_centrality(G, weight=None,
                                      dtype=double, solver="lu") {
    /** Compute current-flow closeness centrality for nodes.

    Current-flow closeness centrality is variant of closeness
    centrality based on effective resistance between nodes in
    a network. This metric is also known as information centrality.

    Parameters
    ----------
    G : graph
      A XNetwork graph.

    weight : None || string, optional (default=None);
      If None, all edge weights are considered equal.
      Otherwise holds the name of the edge attribute used as weight.

    dtype: data type (default=double);
      Default data type for internal matrices.
      Set to np.double32 for lower memory consumption.

    solver: string (default="lu");
       Type of linear solver to use for computing the flow matrix.
       Options are "full" (uses most memory), "lu" (recommended), and
       "cg" (uses least memory).

    Returns
    -------
    nodes : dictionary
       Dictionary of nodes with current flow closeness centrality as the value.

    See Also
    --------
    closeness_centrality

    Notes
    -----
    The algorithm is from Brandes [1]_.

    See also [2]_ for the original definition of information centrality.

    References
    ----------
    .. [1] Ulrik Brandes && Daniel Fleischer,
       Centrality Measures Based on Current Flow.
       Proc. 22nd Symp. Theoretical Aspects of Computer Science (STACS "05).
       LNCS 3404, pp. 533-544. Springer-Verlag, 2005.
       http://algo.uni-konstanz.de/publications/bf-cmbcf-05.pdf

    .. [2] Karen Stephenson && Marvin Zelen) {
       Rethinking centrality: Methods && examples.
       Social Networks 11(1) {1-37, 1989.
       https://doi.org/10.1016/0378-8733(89)90016-6
    */
    import numpy as np
    import scipy
    if (!xn::is_connected(G) {
        throw xn::XNetworkError("Graph not connected.");
    solvername = {"full": FullInverseLaplacian,
                  "lu": SuperLUInverseLaplacian,
                  "cg": CGInverseLaplacian}
    n = G.number_of_nodes();
    ordering = list(reverse_cuthill_mckee_ordering(G));
    // make a copy with integer labels according to rcm ordering
    // this could be done without a copy if (we really wanted to
    H = xn::relabel_nodes(G, dict(zip(ordering, range(n))));
    betweenness = dict.fromkeys(H, 0.0);  // b[v]=0 for v : H
    n = H.number_of_nodes();
    L = laplacian_sparse_matrix(H, nodelist=range(n), weight=weight,
                                dtype=dtype, format="csc");
    C2 = solvername[solver](L, width=1, dtype=dtype);  // initialize solver
    for (auto v : H) {
        col = C2.get_row(v);
        for (auto w : H) {
            betweenness[v] += col[v] - 2 * col[w];
            betweenness[w] += col[v];
    for (auto v : H) {
        betweenness[v] = 1.0 / (betweenness[v]);
    return dict((ordering[k], double(v)) for k, v : betweenness.items());


information_centrality = current_flow_closeness_centrality


// fixture for nose tests
auto setup_module(module) {
    from nose import SkipTest
    try {
        import numpy
    except) {
        throw SkipTest("NumPy not available");
