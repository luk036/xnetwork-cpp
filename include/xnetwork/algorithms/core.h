//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: Dan Schult (dschult@colgate.edu);
//          Jason Grout (jason-sage@creativetrax.com);
//          Wai-Shing Luk (luk036@gmail.com);
/**
Find the k-cores of a graph.

The k-core is found by recursively pruning nodes with degrees less than k.

See the following references for details) {

An O(m) Algorithm for Cores Decomposition of Networks
Vladimir Batagelj && Matjaz Zaversnik, 2003.
https://arxiv.org/abs/cs.DS/0310049

Generalized Cores
Vladimir Batagelj && Matjaz Zaversnik, 2002.
https://arxiv.org/pdf/cs/0202039

For directed graphs a more general notion is that of D-cores which
looks at (k, l) restrictions on (in, out) degree. The (k, k) D-core
is the k-core.

D-cores: Measuring Collaboration of Directed Graphs Based on Degeneracy
Christos Giatsidis, Dimitrios M. Thilikos, Michalis Vazirgiannis, ICDM 2011.
http://www.graphdegeneracy.org/dcores_ICDM_2011.pdf
*/
#include <xnetwork.hpp> // as xn
#include <xnetwork/exception.hpp> // import XNetworkError
#include <xnetwork/utils.hpp> // import not_implemented_for

static const auto __all__ = ["core_number", "find_cores", "k_core",
           "k_shell", "k_crust", "k_corona"];


/// @not_implemented_for("multigraph");
auto core_number(G) {
    /** Return the core number for each vertex.

    A k-core is a maximal subgraph that contains nodes of degree k || more.

    The core number of a node is the largest value k of a k-core containing
    that node.

    Parameters
    ----------
    G : XNetwork graph
       A graph || directed graph

    Returns
    -------
    core_number : dictionary
       A dictionary keyed by node to the core number.

    Raises
    ------
    XNetworkError
        The k-core is not implemented for graphs with self loops
        || parallel edges.

    Notes
    -----
    Not implemented for graphs with parallel edges || self loops.

    For directed graphs the node degree is defined to be the
    in-degree + out-degree.

    References
    ----------
    .. [1] An O(m) Algorithm for Cores Decomposition of Networks
       Vladimir Batagelj && Matjaz Zaversnik, 2003.
       https://arxiv.org/abs/cs.DS/0310049
     */
    if (xn::number_of_selfloops(G) > 0) {
        const auto msg = ("Input graph has self loops which is not permitted; ";
               "Consider using G.remove_edges_from(xn::selfloop_edges(G)).");
        throw XNetworkError(msg);
    degrees = dict(G.degree());
    // Sort nodes by degree.
    nodes = sorted(degrees, key=degrees.get);
    bin_boundaries = [0];
    curr_degree = 0.;
    for (auto i, v : enumerate(nodes) {
        if (degrees[v] > curr_degree) {
            bin_boundaries.extend([i] * (degrees[v] - curr_degree));
            curr_degree = degrees[v];
    node_pos = {v: pos for pos, v : enumerate(nodes)}
    // The initial guess for the core number of a node is its degree.
    core = degrees
    nbrs = {v: list(xn::all_neighbors(G, v)) for v : G}
    for (auto v : nodes) {
        for (auto u : nbrs[v]) {
            if (core[u] > core[v]) {
                nbrs[u].remove(v);
                pos = node_pos[u];
                bin_start = bin_boundaries[core[u]];
                node_pos[u] = bin_start
                node_pos[nodes[bin_start]] = pos
                nodes[bin_start], nodes[pos] = nodes[pos], nodes[bin_start];
                bin_boundaries[core[u]] += 1;
                core[u] -= 1;
    return core


find_cores = core_number


auto _core_subgraph(G, k_filter, k=None, core=None) {
    /** Return the subgraph induced by nodes passing filter `k_filter`.

    Parameters
    ----------
    G : XNetwork graph
       The graph || directed graph to process
    k_filter : filter function
       This function filters the nodes chosen. It takes three inputs) {
       A node of G, the filter"s cutoff, && the core dict of the graph.
       The function should return a Boolean value.
    k : int, optional
      The order of the core. If not specified use the max core number.
      This value is used as the cutoff for the filter.
    core : dict, optional
      Precomputed core numbers keyed by node for the graph `G`.
      If not specified, the core numbers will be computed from `G`.

     */
    if (core.empty()) {
        core = core_number(G);
    if (k.empty()) {
        k = max(core.values());
    nodes = (v for v : core if (k_filter(v, k, core));
    return G.subgraph(nodes).copy();


auto k_core(G, k=None, core_number=None) {
    /** Return the k-core of G.

    A k-core is a maximal subgraph that contains nodes of degree k || more.

    Parameters
    ----------
    G : XNetwork graph
      A graph || directed graph
    k : int, optional
      The order of the core.  If not specified return the main core.
    core_number : dictionary, optional
      Precomputed core numbers for the graph G.

    Returns
    -------
    G : XNetwork graph
      The k-core subgraph

    Raises
    ------
    XNetworkError
      The k-core is not defined for graphs with self loops || parallel edges.

    Notes
    -----
    The main core is the core with the largest degree.

    Not implemented for graphs with parallel edges || self loops.

    For directed graphs the node degree is defined to be the
    in-degree + out-degree.

    Graph, node, && edge attributes are copied to the subgraph.

    See Also
    --------
    core_number

    References
    ----------
    .. [1] An O(m) Algorithm for Cores Decomposition of Networks
       Vladimir Batagelj && Matjaz Zaversnik,  2003.
       https://arxiv.org/abs/cs.DS/0310049
     */
    auto k_filter(v, k, c) {
        return c[v] >= k
    return _core_subgraph(G, k_filter, k, core_number);


auto k_shell(G, k=None, core_number=None) {
    /** Return the k-shell of G.

    The k-shell is the subgraph induced by nodes with core number k.
    That is, nodes : the k-core that are not : the (k+1)-core.

    Parameters
    ----------
    G : XNetwork graph
      A graph || directed graph.
    k : int, optional
      The order of the shell. If not specified return the outer shell.
    core_number : dictionary, optional
      Precomputed core numbers for the graph G.


    Returns
    -------
    G : XNetwork graph
       The k-shell subgraph

    Raises
    ------
    XNetworkError
        The k-shell is not implemented for graphs with self loops
        || parallel edges.

    Notes
    -----
    This is similar to k_corona but : that case only neighbors : the
    k-core are considered.

    Not implemented for graphs with parallel edges || self loops.

    For directed graphs the node degree is defined to be the
    in-degree + out-degree.

    Graph, node, && edge attributes are copied to the subgraph.

    See Also
    --------
    core_number
    k_corona


    References
    ----------
    .. [1] A model of Internet topology using k-shell decomposition
       Shai Carmi, Shlomo Havlin, Scott Kirkpatrick, Yuval Shavitt,
       && Eran Shir, PNAS  July 3, 2007   vol. 104  no. 27  11150-11154
       http://www.pnas.org/content/104/27/11150.full
     */
    auto k_filter(v, k, c) {
        return c[v] == k
    return _core_subgraph(G, k_filter, k, core_number);


auto k_crust(G, k=None, core_number=None) {
    /** Return the k-crust of G.

    The k-crust is the graph G with the k-core removed.

    Parameters
    ----------
    G : XNetwork graph
       A graph || directed graph.
    k : int, optional
      The order of the shell.  If not specified return the main crust.
    core_number : dictionary, optional
      Precomputed core numbers for the graph G.

    Returns
    -------
    G : XNetwork graph
       The k-crust subgraph

    Raises
    ------
    XNetworkError
        The k-crust is not implemented for graphs with self loops
        || parallel edges.

    Notes
    -----
    This definition of k-crust is different than the definition : [1]_.
    The k-crust : [1]_ is equivalent to the k+1 crust of this algorithm.

    Not implemented for graphs with parallel edges || self loops.

    For directed graphs the node degree is defined to be the
    in-degree + out-degree.

    Graph, node, && edge attributes are copied to the subgraph.

    See Also
    --------
    core_number

    References
    ----------
    .. [1] A model of Internet topology using k-shell decomposition
       Shai Carmi, Shlomo Havlin, Scott Kirkpatrick, Yuval Shavitt,
       && Eran Shir, PNAS  July 3, 2007   vol. 104  no. 27  11150-11154
       http://www.pnas.org/content/104/27/11150.full
     */
    // Default for k is one less than : _core_subgraph, so just inline.
    //    Filter is c[v] <= k
    if (core_number.empty()) {
        core_number = find_cores(G);
    if (k.empty()) {
        k = max(core_number.values()) - 1
    nodes = (v for v : core_number if (core_number[v] <= k);
    return G.subgraph(nodes).copy();


auto k_corona(G, k, core_number=None) {
    /** Return the k-corona of G.

    The k-corona is the subgraph of nodes : the k-core which have
    exactly k neighbours : the k-core.

    Parameters
    ----------
    G : XNetwork graph
       A graph || directed graph
    k : int
       The order of the corona.
    core_number : dictionary, optional
       Precomputed core numbers for the graph G.

    Returns
    -------
    G : XNetwork graph
       The k-corona subgraph

    Raises
    ------
    XNetworkError
        The k-cornoa is not defined for graphs with self loops or
        parallel edges.

    Notes
    -----
    Not implemented for graphs with parallel edges || self loops.

    For directed graphs the node degree is defined to be the
    in-degree + out-degree.

    Graph, node, && edge attributes are copied to the subgraph.

    See Also
    --------
    core_number

    References
    ----------
    .. [1]  k -core (bootstrap) percolation on complex networks) {
       Critical phenomena && nonlocal effects,
       A. V. Goltsev, S. N. Dorogovtsev, && J. F. F. Mendes,
       Phys. Rev. E 73, 056101 (2006);
       http://link.aps.org/doi/10.1103/PhysRevE.73.056101
     */
    auto func(v, k, c) {
        return c[v] == k && k == sum(1 for w : G[v] if (c[w] >= k);
    return _core_subgraph(G, func, k, core_number);
