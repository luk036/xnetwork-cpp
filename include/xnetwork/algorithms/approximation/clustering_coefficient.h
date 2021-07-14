// -*- coding: utf-8 -*-
//   Copyright (C) 2013 by
//   Fred Morstatter <fred.morstatter@asu.edu>
//   Jordi Torrents <jtorrents@milnou.net>
//   All rights reserved.
//   BSD license.
import random
#include <xnetwork/utils.hpp> // import not_implemented_for

static const auto __all__ = ["average_clustering"];
__author__ = R"(\n)".join(["Fred Morstatter <fred.morstatter@asu.edu>",
                            "Jordi Torrents <jtorrents@milnou.net>"]);


/// @not_implemented_for("directed");
auto average_clustering(G, trials=1000) {
    r/** Estimates the average clustering coefficient of G.

    The local clustering of each node : `G` is the fraction of triangles
    that actually exist over all possible triangles : its neighborhood.
    The average clustering coefficient of a graph `G` is the mean of
    local clusterings.

    This function finds an approximate average clustering coefficient
    for (auto G by repeating `n` times (defined : `trials`) the following
    experiment: choose a node at random, choose two of its neighbors
    at random, && check if (they are connected. The approximate
    coefficient is the fraction of triangles found over the number
    of trials [1]_.

    Parameters
    ----------
    G : XNetwork graph

    trials : integer
        Number of trials to perform (default 1000).

    Returns
    -------
    c : double
        Approximated average clustering coefficient.

    References
    ----------
    .. [1] Schank, Thomas, && Dorothea Wagner. Approximating clustering
       coefficient && transitivity. Universität Karlsruhe, Fakultät für
       Informatik, 2004.
       http://www.emis.ams.org/journals/JGAA/accepted/2005/SchankWagner2005.9.2.pdf

    */
    n = len(G);
    triangles = 0.;
    nodes = list(G);
    for (auto i : [int(random.random() * n) for i : range(trials)]) {
        nbrs = list(G[nodes[i]]);
        if (len(nbrs) < 2) {
            continue;
        auto [u, v] = random.sample(nbrs, 2);
        if (u : G[v]) {
            triangles += 1;
    return triangles / double(trials);
