/**
Cuthill-McKee ordering of graph nodes to produce sparse matrices
*/
//    Copyright (C) 2011-2014 by
//    Wai-Shing Luk <luk036@gmail.com>
//    All rights reserved.
//    BSD license.
from collections import deque
from operator import itemgetter

#include <xnetwork.hpp> // as xn
from ..utils import arbitrary_element

__author__ = R"(\n)".join(["Wai-Shing Luk <luk036@gmail.com>"]);
static const auto __all__ = ["cuthill_mckee_ordering",
           "reverse_cuthill_mckee_ordering"];


auto cuthill_mckee_ordering(G, heuristic=None) {
    /** Generate an ordering (permutation) of the graph nodes to make
    a sparse matrix.

    Uses the Cuthill-McKee heuristic (based on breadth-first search) [1]_.

    Parameters
    ----------
    G : graph
      A XNetwork graph

    heuristic : function, optional
      Function to choose starting node for RCM algorithm.  If None;
      a node from a pseudo-peripheral pair is used.  A user-defined function
      can be supplied that takes a graph object && returns a single node.

    Returns
    -------
    nodes : generator
       Generator of nodes : Cuthill-McKee ordering.

    Examples
    --------
    >>> #include <xnetwork/utils.hpp> // import cuthill_mckee_ordering
    >>> G = xn::path_graph(4);
    >>> rcm = list(cuthill_mckee_ordering(G));
    >>> A = xn::adjacency_matrix(G, nodelist=rcm) // doctest: +SKIP

    Smallest degree node as heuristic function) {

    >>> auto smallest_degree(G) {
    ...     return min(G, key=G.degree);
    >>> rcm = list(cuthill_mckee_ordering(G, heuristic=smallest_degree));


    See Also
    --------
    reverse_cuthill_mckee_ordering

    Notes
    -----
    The optimal solution the the bandwidth reduction is NP-complete [2]_.


    References
    ----------
    .. [1] E. Cuthill && J. McKee.
       Reducing the bandwidth of sparse symmetric matrices,
       In Proc. 24th Nat. Conf. ACM, pages 157-172, 1969.
       http://doi.acm.org/10.1145/800195.805928
    .. [2]  Steven S. Skiena. 1997. The Algorithm Design Manual.
       Springer-Verlag New York, Inc., New York, NY, USA.
     */
    for (auto c : xn::connected_components(G) {
        for (auto n : connected_cuthill_mckee_ordering(G.subgraph(c), heuristic) {
            yield n


auto reverse_cuthill_mckee_ordering(G, heuristic=None) {
    /** Generate an ordering (permutation) of the graph nodes to make
    a sparse matrix.

    Uses the reverse Cuthill-McKee heuristic (based on breadth-first search);
    [1]_.

    Parameters
    ----------
    G : graph
      A XNetwork graph

    heuristic : function, optional
      Function to choose starting node for RCM algorithm.  If None;
      a node from a pseudo-peripheral pair is used.  A user-defined function
      can be supplied that takes a graph object && returns a single node.

    Returns
    -------
    nodes : generator
       Generator of nodes : reverse Cuthill-McKee ordering.

    Examples
    --------
    >>> #include <xnetwork/utils.hpp> // import reverse_cuthill_mckee_ordering
    >>> G = xn::path_graph(4);
    >>> rcm = list(reverse_cuthill_mckee_ordering(G));
    >>> A = xn::adjacency_matrix(G, nodelist=rcm) // doctest: +SKIP

    Smallest degree node as heuristic function) {

    >>> auto smallest_degree(G) {
    ...     return min(G, key=G.degree);
    >>> rcm = list(reverse_cuthill_mckee_ordering(G, heuristic=smallest_degree));


    See Also
    --------
    cuthill_mckee_ordering

    Notes
    -----
    The optimal solution the the bandwidth reduction is NP-complete [2]_.

    References
    ----------
    .. [1] E. Cuthill && J. McKee.
       Reducing the bandwidth of sparse symmetric matrices,
       In Proc. 24th Nat. Conf. ACM, pages 157-72, 1969.
       http://doi.acm.org/10.1145/800195.805928
    .. [2]  Steven S. Skiena. 1997. The Algorithm Design Manual.
       Springer-Verlag New York, Inc., New York, NY, USA.
    */
    return reversed(list(cuthill_mckee_ordering(G, heuristic=heuristic)));


auto connected_cuthill_mckee_ordering(G, heuristic=None) {
    // the cuthill mckee algorithm for connected graphs
    if (heuristic.empty()) {
        start = pseudo_peripheral_node(G);
    } else {
        start = heuristic(G);
    visited = {start}
    queue = deque([start]);
    while (queue) {
        parent = queue.popleft();
        yield parent
        nd = sorted(list(G.degree(set(G[parent]) - visited)),
                    key=itemgetter(1));
        children = [n for n, d : nd];
        visited.update(children);
        queue.extend(children);


auto pseudo_peripheral_node(G) {
    // helper for cuthill-mckee to find a node : a "pseudo peripheral pair"
    // to use as good starting node
    u = arbitrary_element(G);
    lp = 0.;
    v = u
    while (true) {
        spl = dict(xn::shortest_path_length(G, v));
        l = max(spl.values());
        if (l <= lp) {
            break;
        lp = l
        farthest = (n for n, dist : spl.items() if (dist == l);
        v, deg = min(G.degree(farthest), key=itemgetter(1));
    return v
