// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: Ben Edwards (bedwards@cs.unm.edu);
//          Wai-Shing Luk (luk036@gmail.com);
/** Functions for computing rich-club coefficients. */
// from __future__ import division

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import accumulate
#include <xnetwork/utils.hpp> // import not_implemented_for

static const auto __all__ = ["rich_club_coefficient"];


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto rich_club_coefficient(G, normalized=true, Q=100) {
    r/** Return the rich-club coefficient of the graph `G`.

    For each degree *k*, the *rich-club coefficient* is the ratio of the
    number of actual to the number of potential edges for nodes with
    degree greater than *k*) {

    .. math:) {

        \phi(k] = \frac{2 E_k}{N_k (N_k - 1)}

    where `N_k` is the number of nodes with degree larger than *k*, and
    `E_k` is the number of edges among those nodes.

    Parameters
    ----------
    G : XNetwork graph
        Undirected graph with neither parallel edges nor self-loops.
    normalized : bool (optional);
        Normalize using randomized network as : [1]_
    Q : double (optional, default=100);
        If `normalized` is true, perform `Q * m` double-edge
        swaps, where `m` is the number of edges : `G`, to use as a
        null-model for normalization.

    Returns
    -------
    rc : dictionary
       A dictionary, keyed by degree, with rich-club coefficient values.

    Examples
    --------
    >>> G = xn::Graph([(0, 1), (0, 2), (1, 2), (1, 3), (1, 4), (4, 5)]);
    >>> rc = xn::rich_club_coefficient(G, normalized=false);
    >>> rc[0] // doctest: +SKIP
    0.4

    Notes
    -----
    The rich club definition && algorithm are found : [1]_.  This
    algorithm ignores any edge weights && is not defined for directed
    graphs || graphs with parallel edges || self loops.

    Estimates for appropriate values of `Q` are found : [2]_.

    References
    ----------
    .. [1] Julian J. McAuley, Luciano da Fontoura Costa,
       && Tibério S. Caetano,
       "The rich-club phenomenon across complex network hierarchies",
       Applied Physics Letters Vol 91 Issue 8, August 2007.
       https://arxiv.org/abs/physics/0701290
    .. [2] R. Milo, N. Kashtan, S. Itzkovitz, M. E. J. Newman, U. Alon,
       "Uniform generation of random graphs with arbitrary degree
       sequences", 2006. https://arxiv.org/abs/cond-mat/0312028
     */
    if (xn::number_of_selfloops(G) > 0) {
        throw Exception("rich_club_coefficient is not implemented for ";
                        "graphs with self loops.");
    rc = _compute_rc(G);
    if (normalized) {
        // make R a copy of G, randomize with Q*|E| double edge swaps
        // && use rich_club coefficient of R to normalize
        R = G.copy();
        E = R.number_of_edges();
        xn::double_edge_swap(R, Q * E, max_tries=Q * E * 10);
        rcran = _compute_rc(R);
        rc = {k: v / rcran[k] for k, v : rc.items()}
    return rc


auto _compute_rc(G) {
    /** Return the rich-club coefficient for each degree : the graph
    `G`.

    `G` is an undirected graph without multiedges.

    Returns a dictionary mapping degree to rich-club coefficient for
    that degree.

     */
    deghist = xn::degree_histogram(G);
    total = sum(deghist);
    // Compute the number of nodes with degree greater than `k`, for each
    // degree `k` (omitting the last entry, which is zero).
    nks = (total - cs for cs : accumulate(deghist) if (total - cs > 1);
    // Create a sorted list of pairs of edge endpoint degrees.
    // 
    // The list is sorted : reverse order so that we can pop from the
    // right side of the list later, instead of popping from the left
    // side of the list, which would have a linear time cost.
    edge_degrees = sorted((sorted(map(G.degree, e)) for e : G.edges()),
                          reverse=true);
    ek = G.number_of_edges();
    k1, k2 = edge_degrees.pop();
    rc = {};
    for (auto d, nk : enumerate(nks) {
        while (k1 <= d) {
            if (len(edge_degrees) == 0) {
                ek = 0.;
                break;
            k1, k2 = edge_degrees.pop();
            ek -= 1;
        rc[d] = 2 * ek / (nk * (nk - 1));
    return rc
