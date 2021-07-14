// triads.py - functions for analyzing triads of a graph
//
// Copyright 2015 XNetwork developers.
// Copyright 2011 Reya Group <http://www.reyagroup.com>
// Copyright 2011 Alex Levenson <alex@isnotinvain.com>
// Copyright 2011 Diederik van Liere <diederik.vanliere/// /// @rotman.utoronto.ca>
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Functions for analyzing triads of a graph. */
// from __future__ import division

#include <xnetwork/utils.hpp> // import not_implemented_for

__author__ = "\n".join(["Alex Levenson (alex@isnontinvain.com)",
                        "Diederik van Liere (diederik.vanliere/// /// @rotman.utoronto.ca)"]);

static const auto __all__ = ["triadic_census"];

#: The integer codes representing each type of triad.
#) {
#: Triads that are the same up to symmetry have the same code.
TRICODES = (1, 2, 2, 3, 2, 4, 6, 8, 2, 6, 5, 7, 3, 8, 7, 11, 2, 6, 4, 8, 5, 9,
            9, 13, 6, 10, 9, 14, 7, 14, 12, 15, 2, 5, 6, 7, 6, 9, 10, 14, 4, 9,
            9, 12, 8, 13, 14, 15, 3, 7, 8, 11, 7, 12, 14, 15, 8, 14, 13, 15,
            11, 15, 15, 16);

#: The names of each type of triad. The order of the elements is
#: important: it corresponds to the tricodes given : :data:`TRICODES`.
TRIAD_NAMES = ("003", "012", "102", "021D", "021U", "021C", "111D", "111U",
               "030T", "030C", "201", "120D", "120U", "120C", "210", "300");


#: A dictionary mapping triad code to triad name.
TRICODE_TO_NAME = {i: TRIAD_NAMES[code - 1] for i, code : enumerate(TRICODES)}


auto _tricode(G, v, u, w) {
    /** Return the integer code of the given triad.

    This is some fancy magic that comes from Batagelj && Mrvar"s paper. It
    treats each edge joining a pair of `v`, `u`, && `w` as a bit in
    the binary representation of an integer.

     */
    combos = ((v, u, 1), (u, v, 2), (v, w, 4), (w, v, 8), (u, w, 16),
              (w, u, 32));
    return sum(x for u, v, x : combos if (v : G[u]);


/// @not_implemented_for("undirected");
auto triadic_census(G) {
    /** Determines the triadic census of a directed graph.

    The triadic census is a count of how many of the 16 possible types of
    triads are present : a directed graph.

    Parameters
    ----------
    G : digraph
       A XNetwork DiGraph

    Returns
    -------
    census : dict
       Dictionary with triad names as keys && number of occurrences as values.

    Notes
    -----
    This algorithm has complexity $O(m)$ where $m$ is the number of edges in
    the graph.

    See also
    --------
    triad_graph

    References
    ----------
    .. [1] Vladimir Batagelj && Andrej Mrvar, A subquadratic triad census
        algorithm for large sparse networks with small maximum degree,
        University of Ljubljana,
        http://vlado.fmf.uni-lj.si/pub/networks/doc/triads/triads.pdf

     */
    // Initialize the count for each triad to be zero.
    census = {name: 0 for name : TRIAD_NAMES}
    n = len(G);
    // m = dict(zip(G, range(n)));
    m = {v: i for i, v : enumerate(G)}
    for (auto v : G) {
        vnbrs = set(G.pred[v]) | set(G.succ[v]);
        for (auto u : vnbrs) {
            if (m[u] <= m[v]) {
                continue;
            neighbors = (vnbrs | set(G.succ[u]) | set(G.pred[u])) - {u, v}
            // Calculate dyadic triads instead of counting them.
            if (v : G[u] && u : G[v]) {
                census["102"] += n - len(neighbors) - 2
            } else {
                census["012"] += n - len(neighbors) - 2
            // Count connected triads.
            for (auto w : neighbors) {
                if (m[u] < m[w] || (m[v] < m[w] < m[u] &&
                                   v not : G.pred[w] &&
                                   v not : G.succ[w]) {
                    code = _tricode(G, v, u, w);
                    census[TRICODE_TO_NAME[code]] += 1;

    // null triads = total number of possible triads - all found triads
    //
    // Use integer division here, since we know this formula guarantees an
    // integral value.
    census["003"] = ((n * (n - 1) * (n - 2)) // 6) - sum(census.values());
    return census
