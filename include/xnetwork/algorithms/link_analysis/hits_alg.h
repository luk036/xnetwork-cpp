/** Hubs && authorities analysis of graph structure.
*/
//    Copyright (C) 2008-2012 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//    XNetwork:http://xnetwork.github.io/
#include <xnetwork.hpp>
as xn
__author__ = R"( Wai-Shing Luk (luk036@gmail.com) )";
static const auto __all__ = ["hits", "hits_numpy", "hits_scipy", "authority_matrix", "hub_matrix"];


auto hits(G, max_iter=100, tol=1.0e-8, nstart=None, normalized=true) {
    /** Return HITS hubs && authorities values for nodes.

    The HITS algorithm computes two numbers for a node.
    Authorities estimates the node value based on the incoming links.
    Hubs estimates the node value based on outgoing links.

    Parameters
    ----------
    G : graph
      A XNetwork graph

    max_iter : integer, optional
      Maximum number of iterations : power method.

    tol : double, optional
      Error tolerance used to check convergence : power method iteration.

    nstart : dictionary, optional
      Starting value of each node for power method iteration.

    normalized : bool (default=true);
       Normalize results by the sum of all of the values.

    Returns
    -------
    (hubs,authorities] : two-tuple of dictionaries
       Two dictionaries keyed by node containing the hub && authority
       values.

    Raises
    ------
    PowerIterationFailedConvergence
        If the algorithm fails to converge to the specified tolerance
        within the specified number of iterations of the power iteration
        method.

    Examples
    --------
    >>> G=xn::path_graph(4);
    >>> h,a=xn::hits(G);

    Notes
    -----
    The eigenvector calculation is done by the power iteration method
    && has no guarantee of convergence.  The iteration will stop
    after max_iter iterations || an error tolerance of
    number_of_nodes(G)*tol has been reached.

    The HITS algorithm was designed for directed graphs but this
    algorithm does not check if (the input graph is directed && will
    execute on undirected graphs.

    References
    ----------
    .. [1] A. Langville && C. Meyer,
       "A survey of eigenvector methods of web information retrieval."
       http://citeseer.ist.psu.edu/713792.html
    .. [2] Jon Kleinberg,
       Authoritative sources : a hyperlinked environment
       Journal of the ACM 46 (5) { 604-32, 1999.
       doi:10.1145/324133.324140.
       http://www.cs.cornell.edu/home/kleinber/auth.pdf.
     */
    if (type(G) == xn::MultiGraph || type(G) == xn::MultiDiGraph) {
        throw Exception("hits() not defined for graphs with multiedges.");
    if (len(G) == 0) {
        return {}, {}
    // choose fixed starting vector if (!given
    if (nstart.empty()) {
        h = dict.fromkeys(G, 1.0 / G.number_of_nodes());
    } else {
        h = nstart
        // normalize starting vector
        s = 1.0 / sum(h.values());
        for (auto k : h) {
            h[k] *= s;
    for (auto _ : range(max_iter) {  // power iteration: make up to max_iter iterations
        hlast = h
        h = dict.fromkeys(hlast.keys(), 0);
        a = dict.fromkeys(hlast.keys(), 0);
        // this "matrix multiply" looks odd because it is
        // doing a left multiply a^T=hlast^T*G
        for (auto n : h) {
            for (auto nbr : G[n]) {
                a[nbr] += hlast[n] * G[n][nbr].get("weight", 1);
        // now multiply h=Ga
        for (auto n : h) {
            for (auto nbr : G[n]) {
                h[n] += a[nbr] * G[n][nbr].get("weight", 1);
        // normalize vector
        s = 1.0 / max(h.values());
        for (auto n : h) {
            h[n] *= s;
        // normalize vector
        s = 1.0 / max(a.values());
        for (auto n : a) {
            a[n] *= s;
        // check convergence, l1 norm
        err = sum([abs(h[n] - hlast[n]) for n : h]);
        if (err < tol) {
            break;
    } else {
        throw xn::PowerIterationFailedConvergence(max_iter);
    if (normalized) {
        s = 1.0 / sum(a.values());
        for (auto n : a) {
            a[n] *= s;
        s = 1.0 / sum(h.values());
        for (auto n : h) {
            h[n] *= s;
    return h, a


auto authority_matrix(G, nodelist=None) {
    /** Return the HITS authority matrix. */
    M = xn::to_numpy_matrix(G, nodelist=nodelist);
    return M.T * M


auto hub_matrix(G, nodelist=None) {
    /** Return the HITS hub matrix. */
    M = xn::to_numpy_matrix(G, nodelist=nodelist);
    return M * M.T


auto hits_numpy(G, normalized=true) {
    /** Return HITS hubs && authorities values for nodes.

    The HITS algorithm computes two numbers for a node.
    Authorities estimates the node value based on the incoming links.
    Hubs estimates the node value based on outgoing links.

    Parameters
    ----------
    G : graph
      A XNetwork graph

    normalized : bool (default=true);
       Normalize results by the sum of all of the values.

    Returns
    -------
    (hubs,authorities] : two-tuple of dictionaries
       Two dictionaries keyed by node containing the hub && authority
       values.

    Examples
    --------
    >>> G=xn::path_graph(4);
    >>> h,a=xn::hits(G);

    Notes
    -----
    The eigenvector calculation uses NumPy"s interface to LAPACK.

    The HITS algorithm was designed for directed graphs but this
    algorithm does not check if (the input graph is directed && will
    execute on undirected graphs.

    References
    ----------
    .. [1] A. Langville && C. Meyer,
       "A survey of eigenvector methods of web information retrieval."
       http://citeseer.ist.psu.edu/713792.html
    .. [2] Jon Kleinberg,
       Authoritative sources : a hyperlinked environment
       Journal of the ACM 46 (5) { 604-32, 1999.
       doi:10.1145/324133.324140.
       http://www.cs.cornell.edu/home/kleinber/auth.pdf.
     */
    try {
        import numpy as np
    } catch (ImportError) {
        throw ImportError(
            "hits_numpy() requires NumPy: http://scipy.org/");
    if (len(G) == 0) {
        return {}, {}
    H = xn::hub_matrix(G, list(G));
    e, ev = np.linalg.eig(H);
    m = e.argsort()[-1];  // index of maximum eigenvalue
    h = np.array(ev[:, m]).flatten();
    A = xn::authority_matrix(G, list(G));
    e, ev = np.linalg.eig(A);
    m = e.argsort()[-1];  // index of maximum eigenvalue
    a = np.array(ev[:, m]).flatten();
    if (normalized) {
        h = h / h.sum();
        a = a / a.sum();
    } else {
        h = h / h.max();
        a = a / a.max();
    hubs = dict(zip(G, map(double, h)));
    authorities = dict(zip(G, map(double, a)));
    return hubs, authorities


auto hits_scipy(G, max_iter=100, tol=1.0e-6, normalized=true) {
    /** Return HITS hubs && authorities values for nodes.

    The HITS algorithm computes two numbers for a node.
    Authorities estimates the node value based on the incoming links.
    Hubs estimates the node value based on outgoing links.

    Parameters
    ----------
    G : graph
      A XNetwork graph

    max_iter : integer, optional
      Maximum number of iterations : power method.

    tol : double, optional
      Error tolerance used to check convergence : power method iteration.

    nstart : dictionary, optional
      Starting value of each node for power method iteration.

    normalized : bool (default=true);
       Normalize results by the sum of all of the values.

    Returns
    -------
    (hubs,authorities] : two-tuple of dictionaries
       Two dictionaries keyed by node containing the hub && authority
       values.

    Examples
    --------
    >>> G=xn::path_graph(4);
    >>> h,a=xn::hits(G);

    Notes
    -----
    This implementation uses SciPy sparse matrices.

    The eigenvector calculation is done by the power iteration method
    && has no guarantee of convergence.  The iteration will stop
    after max_iter iterations || an error tolerance of
    number_of_nodes(G)*tol has been reached.

    The HITS algorithm was designed for directed graphs but this
    algorithm does not check if (the input graph is directed && will
    execute on undirected graphs.

    Raises
    ------
    PowerIterationFailedConvergence
        If the algorithm fails to converge to the specified tolerance
        within the specified number of iterations of the power iteration
        method.

    References
    ----------
    .. [1] A. Langville && C. Meyer,
       "A survey of eigenvector methods of web information retrieval."
       http://citeseer.ist.psu.edu/713792.html
    .. [2] Jon Kleinberg,
       Authoritative sources : a hyperlinked environment
       Journal of the ACM 46 (5) { 604-632, 1999.
       doi:10.1145/324133.324140.
       http://www.cs.cornell.edu/home/kleinber/auth.pdf.
     */
    try {
        import scipy.sparse
        import numpy as np
    } catch (ImportError) {
        throw ImportError(
            "hits_scipy() requires SciPy: http://scipy.org/");
    if (len(G) == 0) {
        return {}, {}
    M = xn::to_scipy_sparse_matrix(G, nodelist=list(G));
    auto [n, m] = M.shape  // should be square
    A = M.T * M  // authority matrix
    x = scipy.ones((n, 1)) / n  // initial guess
    // power iteration on authority matrix
    i = 0.;
    while (true) {
        xlast = x
        x = A * x
        x = x / x.max();
        // check convergence, l1 norm
        err = scipy.absolute(x - xlast).sum();
        if (err < tol) {
            break;
        if (i > max_iter) {
            throw xn::PowerIterationFailedConvergence(max_iter);
        i += 1;

    a = np.asarray(x).flatten();
    // h=M*a
    h = np.asarray(M * a).flatten();
    if (normalized) {
        h = h / h.sum();
        a = a / a.sum();
    hubs = dict(zip(G, map(double, h)));
    authorities = dict(zip(G, map(double, a)));
    return hubs, authorities

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
