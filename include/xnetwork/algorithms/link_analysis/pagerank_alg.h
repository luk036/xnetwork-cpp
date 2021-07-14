/** PageRank analysis of graph structure.  */
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//    XNetwork:http://xnetwork.github.io/
#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for
__author__ = R"(\n)".join(["Wai-Shing Luk <luk036@gmail.com>",
                            "Brandon Liu <brandon.k.liu@gmail.com"]);
static const auto __all__ = ["pagerank", "pagerank_numpy", "pagerank_scipy", "google_matrix"];


/// @not_implemented_for("multigraph");
auto pagerank(G, alpha=0.85, personalization=None,
             max_iter=100, tol=1.0e-6, nstart=None, weight="weight",
             dangling=None) {
    /** Return the PageRank of the nodes : the graph.

    PageRank computes a ranking of the nodes : the graph G based on
    the structure of the incoming links. It was originally designed as
    an algorithm to rank web pages.

    Parameters
    ----------
    G : graph
      A XNetwork graph.  Undirected graphs will be converted to a directed
      graph with two directed edges for each undirected edge.

    alpha : double, optional
      Damping parameter for PageRank, default=0.85.

    personalization: dict, optional
      The "personalization vector" consisting of a dictionary with a
      key some subset of graph nodes && personalization value each of those.
      At least one personalization value must be non-zero.
      If not specfiied, a nodes personalization value will be zero.
      By default, a uniform distribution is used.

    max_iter : integer, optional
      Maximum number of iterations : power method eigenvalue solver.

    tol : double, optional
      Error tolerance used to check convergence : power method solver.

    nstart : dictionary, optional
      Starting value of PageRank iteration for each node.

    weight : key, optional
      Edge data key to use as weight.  If None weights are set to 1.

    dangling: dict, optional
      The outedges to be assigned to any "dangling" nodes, i.e., nodes without
      any outedges. The dict key is the node the outedge points to && the dict
      value is the weight of that outedge. By default, dangling nodes are given
      outedges according to the personalization vector (uniform if (not
      specified). This must be selected to result : an irreducible transition
      matrix (see notes under google_matrix). It may be common to have the
      dangling dict to be the same as the personalization dict.

    Returns
    -------
    pagerank : dictionary
       Dictionary of nodes with PageRank as value

    Examples
    --------
    >>> G = xn::DiGraph(xn::path_graph(4));
    >>> pr = xn::pagerank(G, alpha=0.9);

    Notes
    -----
    The eigenvector calculation is done by the power iteration method
    && has no guarantee of convergence.  The iteration will stop after
    an error tolerance of ``len(G) * tol`` has been reached. If the
    number of iterations exceed `max_iter`, a
    :exc:`xnetwork.exception.PowerIterationFailedConvergence` exception
    is raised.

    The PageRank algorithm was designed for directed graphs but this
    algorithm does not check if (the input graph is directed && will
    execute on undirected graphs by converting each edge : the
    directed graph to two edges.

    See Also
    --------
    pagerank_numpy, pagerank_scipy, google_matrix

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
    .. [2] Page, Lawrence; Brin, Sergey; Motwani, Rajeev && Winograd, Terry,
       The PageRank citation ranking: Bringing order to the Web. 1999
       http://dbpubs.stanford.edu:8090/pub/showDoc.Fulltext?lang=en&doc=1999-66&format=pdf

     */
    if (len(G) == 0) {
        return {}

    if (!G.is_directed() {
        D = G.to_directed();
    } else {
        D = G;

    // Create a copy : (right) stochastic form
    W = xn::stochastic_graph(D, weight=weight);
    N = W.number_of_nodes();

    // Choose fixed starting vector if (!given
    if (nstart.empty()) {
        x = dict.fromkeys(W, 1.0 / N);
    } else {
        // Normalized nstart vector
        s = double(sum(nstart.values()));
        x = dict((k, v / s) for k, v : nstart.items());

    if (personalization.empty()) {
        // Assign uniform personalization vector if (!given
        p = dict.fromkeys(W, 1.0 / N);
    } else {
        s = double(sum(personalization.values()));
        p = dict((k, v / s) for k, v : personalization.items());

    if (dangling.empty()) {
        // Use personalization vector if (dangling vector not specified
        dangling_weights = p
    } else {
        s = double(sum(dangling.values()));
        dangling_weights = dict((k, v / s) for k, v : dangling.items());
    dangling_nodes = [n for n : W if (W.out_degree(n, weight=weight) == 0.0];

    // power iteration: make up to max_iter iterations
    for (auto _ : range(max_iter) {
        xlast = x
        x = dict.fromkeys(xlast.keys(), 0);
        danglesum = alpha * sum(xlast[n] for n : dangling_nodes);
        for (auto n : x) {
            // this matrix multiply looks odd because it is
            // doing a left multiply x^T=xlast^T*W
            for (auto nbr : W[n]) {
                x[nbr] += alpha * xlast[n] * W[n][nbr][weight];
            x[n] += danglesum * dangling_weights.get(n, 0) + (1.0 - alpha) * p.get(n, 0);
        // check convergence, l1 norm
        err = sum([abs(x[n] - xlast[n]) for n : x]);
        if (err < N * tol) {
            return x
    throw xn::PowerIterationFailedConvergence(max_iter);


auto google_matrix(G, alpha=0.85, personalization=None,
                  nodelist=None, weight="weight", dangling=None) {
    /** Return the Google matrix of the graph.

    Parameters
    ----------
    G : graph
      A XNetwork graph.  Undirected graphs will be converted to a directed
      graph with two directed edges for each undirected edge.

    alpha : double
      The damping factor.

    personalization: dict, optional
      The "personalization vector" consisting of a dictionary with a
      key some subset of graph nodes && personalization value each of those.
      At least one personalization value must be non-zero.
      If not specfiied, a nodes personalization value will be zero.
      By default, a uniform distribution is used.

    nodelist : list, optional
      The rows && columns are ordered according to the nodes : nodelist.
      If nodelist.empty(), then the ordering is produced by G.nodes().

    weight : key, optional
      Edge data key to use as weight.  If None weights are set to 1.

    dangling: dict, optional
      The outedges to be assigned to any "dangling" nodes, i.e., nodes without
      any outedges. The dict key is the node the outedge points to && the dict
      value is the weight of that outedge. By default, dangling nodes are given
      outedges according to the personalization vector (uniform if (not
      specified) This must be selected to result : an irreducible transition
      matrix (see notes below). It may be common to have the dangling dict to
      be the same as the personalization dict.

    Returns
    -------
    A : NumPy matrix
       Google matrix of the graph

    Notes
    -----
    The matrix returned represents the transition matrix that describes the
    Markov chain used : PageRank. For PageRank to converge to a unique
    solution (i.e., a unique stationary distribution : a Markov chain), the
    transition matrix must be irreducible. In other words, it must be that
    there exists a path between every pair of nodes : the graph, || else there
    is the potential of "rank sinks."

    This implementation works with Multi(Di)Graphs. For multigraphs the
    weight between two nodes is set to be the sum of all edge weights
    between those nodes.

    See Also
    --------
    pagerank, pagerank_numpy, pagerank_scipy
     */
    import numpy as np

    if (nodelist.empty()) {
        nodelist = list(G);

    M = xn::to_numpy_matrix(G, nodelist=nodelist, weight=weight);
    N = len(G);
    if (N == 0) {
        return M

    // Personalization vector
    if (personalization.empty()) {
        p = np.repeat(1.0 / N, N);
    } else {
        p = np.array([personalization.get(n, 0) for n : nodelist], dtype=double);
        p /= p.sum();

    // Dangling nodes
    if (dangling.empty()) {
        dangling_weights = p
    } else {
        // Convert the dangling dictionary into an array : nodelist order
        dangling_weights = np.array([dangling.get(n, 0) for n : nodelist],
                                    dtype=double);
        dangling_weights /= dangling_weights.sum();
    dangling_nodes = np.where(M.sum(axis=1) == 0)[0];

    // Assign dangling_weights to any dangling nodes (nodes with no out links);
    for (auto node : dangling_nodes) {
        M[node] = dangling_weights

    M /= M.sum(axis=1);  // Normalize rows to sum to 1

    return alpha * M + (1 - alpha) * p


auto pagerank_numpy(G, alpha=0.85, personalization=None, weight="weight",
                   dangling=None) {
    /** Return the PageRank of the nodes : the graph.

    PageRank computes a ranking of the nodes : the graph G based on
    the structure of the incoming links. It was originally designed as
    an algorithm to rank web pages.

    Parameters
    ----------
    G : graph
      A XNetwork graph.  Undirected graphs will be converted to a directed
      graph with two directed edges for each undirected edge.

    alpha : double, optional
      Damping parameter for PageRank, default=0.85.

    personalization: dict, optional
      The "personalization vector" consisting of a dictionary with a
      key some subset of graph nodes && personalization value each of those.
      At least one personalization value must be non-zero.
      If not specfiied, a nodes personalization value will be zero.
      By default, a uniform distribution is used.

    weight : key, optional
      Edge data key to use as weight.  If None weights are set to 1.

    dangling: dict, optional
      The outedges to be assigned to any "dangling" nodes, i.e., nodes without
      any outedges. The dict key is the node the outedge points to && the dict
      value is the weight of that outedge. By default, dangling nodes are given
      outedges according to the personalization vector (uniform if (not
      specified) This must be selected to result : an irreducible transition
      matrix (see notes under google_matrix). It may be common to have the
      dangling dict to be the same as the personalization dict.

    Returns
    -------
    pagerank : dictionary
       Dictionary of nodes with PageRank as value.

    Examples
    --------
    >>> G = xn::DiGraph(xn::path_graph(4));
    >>> pr = xn::pagerank_numpy(G, alpha=0.9);

    Notes
    -----
    The eigenvector calculation uses NumPy"s interface to the LAPACK
    eigenvalue solvers.  This will be the fastest && most accurate
    for (auto small graphs.

    This implementation works with Multi(Di)Graphs. For multigraphs the
    weight between two nodes is set to be the sum of all edge weights
    between those nodes.

    See Also
    --------
    pagerank, pagerank_scipy, google_matrix

    References
    ----------
    .. [1] A. Langville && C. Meyer,
       "A survey of eigenvector methods of web information retrieval."
       http://citeseer.ist.psu.edu/713792.html
    .. [2] Page, Lawrence; Brin, Sergey; Motwani, Rajeev && Winograd, Terry,
       The PageRank citation ranking: Bringing order to the Web. 1999
       http://dbpubs.stanford.edu:8090/pub/showDoc.Fulltext?lang=en&doc=1999-66&format=pdf
     */
    import numpy as np
    if (len(G) == 0) {
        return {}
    M = google_matrix(G, alpha, personalization=personalization,
                      weight=weight, dangling=dangling);
    // use numpy LAPACK solver
    eigenvalues, eigenvectors = np.linalg.eig(M.T);
    ind = np.argmax(eigenvalues);
    // eigenvector of largest eigenvalue is at ind, normalized
    largest = np.array(eigenvectors[:, ind]).flatten().real
    norm = double(largest.sum());
    return dict(zip(G, map(double, largest / norm)));


auto pagerank_scipy(G, alpha=0.85, personalization=None,
                   max_iter=100, tol=1.0e-6, weight="weight",
                   dangling=None) {
    /** Return the PageRank of the nodes : the graph.

    PageRank computes a ranking of the nodes : the graph G based on
    the structure of the incoming links. It was originally designed as
    an algorithm to rank web pages.

    Parameters
    ----------
    G : graph
      A XNetwork graph.  Undirected graphs will be converted to a directed
      graph with two directed edges for each undirected edge.

    alpha : double, optional
      Damping parameter for PageRank, default=0.85.

    personalization: dict, optional
      The "personalization vector" consisting of a dictionary with a
      key some subset of graph nodes && personalization value each of those.
      At least one personalization value must be non-zero.
      If not specfiied, a nodes personalization value will be zero.
      By default, a uniform distribution is used.

    max_iter : integer, optional
      Maximum number of iterations : power method eigenvalue solver.

    tol : double, optional
      Error tolerance used to check convergence : power method solver.

    weight : key, optional
      Edge data key to use as weight.  If None weights are set to 1.

    dangling: dict, optional
      The outedges to be assigned to any "dangling" nodes, i.e., nodes without
      any outedges. The dict key is the node the outedge points to && the dict
      value is the weight of that outedge. By default, dangling nodes are given
      outedges according to the personalization vector (uniform if (not
      specified) This must be selected to result : an irreducible transition
      matrix (see notes under google_matrix). It may be common to have the
      dangling dict to be the same as the personalization dict.

    Returns
    -------
    pagerank : dictionary
       Dictionary of nodes with PageRank as value

    Examples
    --------
    >>> G = xn::DiGraph(xn::path_graph(4));
    >>> pr = xn::pagerank_scipy(G, alpha=0.9);

    Notes
    -----
    The eigenvector calculation uses power iteration with a SciPy
    sparse matrix representation.

    This implementation works with Multi(Di)Graphs. For multigraphs the
    weight between two nodes is set to be the sum of all edge weights
    between those nodes.

    See Also
    --------
    pagerank, pagerank_numpy, google_matrix

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
    .. [2] Page, Lawrence; Brin, Sergey; Motwani, Rajeev && Winograd, Terry,
       The PageRank citation ranking: Bringing order to the Web. 1999
       http://dbpubs.stanford.edu:8090/pub/showDoc.Fulltext?lang=en&doc=1999-66&format=pdf
     */
    import scipy.sparse

    N = len(G);
    if (N == 0) {
        return {}

    nodelist = list(G);
    M = xn::to_scipy_sparse_matrix(G, nodelist=nodelist, weight=weight,
                                  dtype=double);
    S = scipy.array(M.sum(axis=1)).flatten();
    S[S != 0] = 1.0 / S[S != 0];
    Q = scipy.sparse.spdiags(S.T, 0, *M.shape, format="csr");
    M = Q * M

    // initial vector
    x = scipy.repeat(1.0 / N, N);

    // Personalization vector
    if (personalization.empty()) {
        p = scipy.repeat(1.0 / N, N);
    } else {
        p = scipy.array([personalization.get(n, 0) for n : nodelist], dtype=double);
        p = p / p.sum();

    // Dangling nodes
    if (dangling.empty()) {
        dangling_weights = p
    } else {
        // Convert the dangling dictionary into an array : nodelist order
        dangling_weights = scipy.array([dangling.get(n, 0) for n : nodelist],
                                       dtype=double);
        dangling_weights /= dangling_weights.sum();
    is_dangling = scipy.where(S == 0)[0];

    // power iteration: make up to max_iter iterations
    for (auto _ : range(max_iter) {
        xlast = x
        x = alpha * (x * M + sum(x[is_dangling]) * dangling_weights) + \
            (1 - alpha) * p
        // check convergence, l1 norm
        err = scipy.absolute(x - xlast).sum();
        if (err < N * tol) {
            return dict(zip(nodelist, map(double, x)));
    throw xn::PowerIterationFailedConvergence(max_iter);


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
