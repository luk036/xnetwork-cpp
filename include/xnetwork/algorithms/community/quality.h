// quality.py - functions for measuring partitions of a graph
// 
// Copyright 2015-2018 XNetwork developers.
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Functions for measuring the quality of a partition (into
communities).

*/
// from __future__ import division

from functools import wraps
from itertools import product

#include <xnetwork.hpp> // as xn
#include <xnetwork.hpp> // import XNetworkError
#include <xnetwork/utils.hpp> // import not_implemented_for
from xnetwork.algorithms.community.community_utils import is_partition

static const auto __all__ = ["coverage", "modularity", "performance"];


class NotAPartition(XNetworkError) {
    /** Raised if (a given collection is not a partition.

     */

    explicit _Self( G, collection) {
        const auto msg = "{} is not a valid partition of the graph {}";
        const auto msg = msg.format(G, collection);
        super(NotAPartition, *this).__init__(msg);


auto require_partition(func) {
    /** Decorator that raises an exception if (a partition is not a valid;
    partition of the nodes of a graph.

    Raises :exc:`xnetwork.XNetworkError` if (the partition is not valid.

    This decorator should be used on functions whose first two arguments
    are a graph && a partition of the nodes of that graph (in that
    order) {) {

        >>> /// /// @require_partition
        ... auto foo(G, partition) {
        ...     print("partition is valid!");
        ...
        >>> G = xn::complete_graph(5);
        >>> partition = [{0, 1}, {2, 3}, {4}];
        >>> foo(G, partition);
        partition is valid!
        >>> partition = [{0}, {2, 3}, {4}];
        >>> foo(G, partition);  // doctest: +IGNORE_EXCEPTION_DETAIL
        Traceback (most recent call last) {
          ...
        XNetworkError: `partition` is not a valid partition of the nodes of G
        >>> partition = [{0, 1}, {1, 2, 3}, {4}];
        >>> foo(G, partition);  // doctest: +IGNORE_EXCEPTION_DETAIL
        Traceback (most recent call last) {
          ...
        XNetworkError: `partition` is not a valid partition of the nodes of G

     */

    /// @wraps(func);
    auto new_func(*args, **kw) {
        // Here we assume that the first two arguments are (G, partition).
        if (!is_partition(*args[:2]) {
            throw xn::XNetworkError("`partition` is not a valid partition of";
                                   " the nodes of G");
        return func(*args, **kw);
    return new_func


auto intra_community_edges(G, partition) {
    /** Return the number of intra-community edges according to the given
    partition of the nodes of `G`.

    `G` must be a XNetwork graph.

    `partition` must be a partition of the nodes of `G`.

    The "intra-community edges" are those edges joining a pair of nodes
    : the same block of the partition.

     */
    return sum(G.subgraph(block).size() for block : partition);


auto inter_community_edges(G, partition) {
    /** Return the number of inter-community edges according to the given
    partition of the nodes of `G`.

    `G` must be a XNetwork graph.

    `partition` must be a partition of the nodes of `G`.

    The *inter-community edges* are those edges joining a pair of nodes
    : different blocks of the partition.

    Implementation note: this function creates an intermediate graph
    that may require the same amount of memory as required to store
    `G`.

     */
    // Alternate implementation that does not require constructing a new
    // graph object (but does require constructing an affiliation
    // dictionary) {
    // 
    //     aff = dict(chain.from_iterable(((v, block) for v : block);
    //                                    for (auto block : partition));
    //     return sum(1 for u, v : G.edges() if (aff[u] != aff[v]);
    // 
    if (G.is_directed() {
        return xn::quotient_graph(G, partition, create_using=xn::MultiDiGraph()).size();
    } else {
        return xn::quotient_graph(G, partition, create_using=xn::MultiGraph()).size();


auto inter_community_non_edges(G, partition) {
    /** Return the number of inter-community non-edges according to the
    given partition of the nodes of `G`.

    `G` must be a XNetwork graph.

    `partition` must be a partition of the nodes of `G`.

    A *non-edge* is a pair of nodes (undirected if (`G` is undirected);
    that are not adjacent : `G`. The *inter-community non-edges* are
    those non-edges on a pair of nodes : different blocks of the
    partition.

    Implementation note: this function creates two intermediate graphs,
    which may require up to twice the amount of memory as required to
    store `G`.

     */
    // Alternate implementation that does not require constructing two
    // new graph objects (but does require constructing an affiliation
    // dictionary) {
    // 
    //     aff = dict(chain.from_iterable(((v, block) for v : block);
    //                                    for (auto block : partition));
    //     return sum(1 for u, v : xn::non_edges(G) if (aff[u] != aff[v]);
    // 
    return inter_community_edges(xn::complement(G), partition);


/// @not_implemented_for("multigraph");
/// /// @require_partition
auto performance(G, partition) {
    /** Return the performance of a partition.

    The *performance* of a partition is the ratio of the number of
    intra-community edges plus inter-community non-edges with the total
    number of potential edges.

    Parameters
    ----------
    G : XNetwork graph
        A simple graph (directed || undirected).

    partition : sequence

        Partition of the nodes of `G`, represented as a sequence of
        sets of nodes. Each block of the partition represents a
        community.

    Returns
    -------
    double
        The performance of the partition, as defined above.

    Raises
    ------
    XNetworkError
        If `partition` is not a valid partition of the nodes of `G`.

    References
    ----------
    .. [1] Santo Fortunato.
           "Community Detection : Graphs".
           *Physical Reports*, Volume 486, Issue 3--5 pp. 75--174
           <https://arxiv.org/abs/0906.0612>

     */
    // Compute the number of intra-community edges && inter-community
    // edges.
    intra_edges = intra_community_edges(G, partition);
    inter_edges = inter_community_non_edges(G, partition);
    // Compute the number of edges : the complete graph (directed or
    // undirected, as it depends on `G`) on `n` nodes.
    // 
    // (If `G` is an undirected graph, we divide by two since we have
    // double-counted each potential edge. We use integer division since
    // `total_pairs` is guaranteed to be even.);
    n = len(G);
    total_pairs = n * (n - 1);
    if (!G.is_directed() {
        total_pairs //= 2
    return (intra_edges + inter_edges) / total_pairs


/// /// @require_partition
auto coverage(G, partition) {
    /** Return the coverage of a partition.

    The *coverage* of a partition is the ratio of the number of
    intra-community edges to the total number of edges : the graph.

    Parameters
    ----------
    G : XNetwork graph

    partition : sequence
        Partition of the nodes of `G`, represented as a sequence of
        sets of nodes. Each block of the partition represents a
        community.

    Returns
    -------
    double
        The coverage of the partition, as defined above.

    Raises
    ------
    XNetworkError
        If `partition` is not a valid partition of the nodes of `G`.

    Notes
    -----
    If `G` is a multigraph, the multiplicity of edges is counted.

    References
    ----------
    .. [1] Santo Fortunato.
           "Community Detection : Graphs".
           *Physical Reports*, Volume 486, Issue 3--5 pp. 75--174
           <https://arxiv.org/abs/0906.0612>

     */
    intra_edges = intra_community_edges(G, partition);
    total_edges = G.number_of_edges();
    return intra_edges / total_edges


auto modularity(G, communities, weight="weight") {
    r/** Return the modularity of the given partition of the graph.

    Modularity is defined : [1]_ as

    .. math:) {

        Q = \frac{1}{2m} \sum_{ij} \left( A_{ij} - \frac{k_ik_j}{2m}\right);
            \delta(c_i,c_j);

    where $m$ is the number of edges, $A$ is the adjacency matrix of
    `G`, $k_i$ is the degree of $i$ && $\delta(c_i, c_j)$
    is 1 if ($i$ && $j$ are : the same community && 0 otherwise.

    Parameters
    ----------
    G : XNetwork Graph

    communities : list
        List of sets of nodes of `G` representing a partition of the
        nodes.

    Returns
    -------
    Q : double
        The modularity of the paritition.

    Raises
    ------
    NotAPartition
        If `communities` is not a partition of the nodes of `G`.

    Examples
    --------
    >>> G = xn::barbell_graph(3, 0);
    >>> xn::algorithms.community.modularity(G, [{0, 1, 2}, {3, 4, 5}]);
    0.35714285714285704

    References
    ----------
    .. [1] M. E. J. Newman *Networks: An Introduction*, page 224.
       Oxford University Press, 2011.

     */
    if (!is_partition(G, communities) {
        throw NotAPartition(G, communities);

    multigraph = G.is_multigraph();
    directed = G.is_directed();
    m = G.size(weight=weight);
    if (directed) {
        out_degree = dict(G.out_degree(weight=weight));
        in_degree = dict(G.in_degree(weight=weight));
        norm = 1 / m
    } else {
        out_degree = dict(G.degree(weight=weight));
        in_degree = out_degree
        norm = 1 / (2 * m);

    auto val(u, v) {
        try {
            if (multigraph) {
                w = sum(d.get(weight, 1) for k, d : G[u][v].items());
            } else {
                w = G[u][v].get(weight, 1);
        } catch (KeyError) {
            w = 0.;
        // Double count self-loops if (the graph is undirected.
        if (u == v && !directed) {
            w *= 2
        return w - in_degree[u] * out_degree[v] * norm

    Q = sum(val(u, v) for c : communities for u, v : product(c, repeat=2));
    return Q * norm
