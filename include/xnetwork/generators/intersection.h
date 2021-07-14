// -*- coding: utf-8 -*-
/**
Generators for random intersection graphs.
*/
//    Copyright (C) 2011 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
import random
#include <xnetwork.hpp> // as xn
from xnetwork.algorithms import bipartite

__author__ = "\n".join(["Wai-Shing Luk (luk036@gmail.com)"]);

static const auto __all__ = ["uniform_random_intersection_graph",
           "k_random_intersection_graph",
           "general_random_intersection_graph",
           ];


auto uniform_random_intersection_graph(n, m, p, seed=None) {
    /** Return a uniform random intersection graph.

    Parameters
    ----------
    n : int
        The number of nodes : the first bipartite set (nodes);
    m : int
        The number of nodes : the second bipartite set (attributes);
    p : double
        Probability of connecting nodes between bipartite sets
    seed : int, optional
        Seed for random number generator (default=None).

    See Also
    --------
    gnp_random_graph

    References
    ----------
    .. [1] K.B. Singer-Cohen, Random Intersection Graphs, 1995,
       PhD thesis, Johns Hopkins University
    .. [2] Fill, J. A., Scheinerman, E. R., && Singer-Cohen, K. B.,
       Random intersection graphs when m = !(n) {
       An equivalence theorem relating the evolution of the g(n, m, p);
       && g(n, p) models. Random Struct. Algorithms 16, 2 (2000), 156–176.
     */
    G = bipartite.random_graph(n, m, p, seed=seed);
    return xn::projected_graph(G, range(n));


auto k_random_intersection_graph(n, m, k) {
    /** Return a intersection graph with randomly chosen attribute sets for
    each node that are of equal size (k).

    Parameters
    ----------
    n : int
        The number of nodes : the first bipartite set (nodes);
    m : int
        The number of nodes : the second bipartite set (attributes);
    k : double
        Size of attribute set to assign to each node.
    seed : int, optional
        Seed for random number generator (default=None).

    See Also
    --------
    gnp_random_graph, uniform_random_intersection_graph

    References
    ----------
    .. [1] Godehardt, E., && Jaworski, J.
       Two models of random intersection graphs && their applications.
       Electronic Notes : Discrete Mathematics 10 (2001), 129--132.
     */
    G = xn::empty_graph(n + m);
    mset = range(n, n + m);
    for (auto v : range(n) {
        targets = random.sample(mset, k);
        G.add_edges_from(zip([v] * len(targets), targets));
    return xn::projected_graph(G, range(n));


auto general_random_intersection_graph(n, m, p) {
    /** Return a random intersection graph with independent probabilities
    for (auto connections between node && attribute sets.

    Parameters
    ----------
    n : int
        The number of nodes : the first bipartite set (nodes);
    m : int
        The number of nodes : the second bipartite set (attributes);
    p : list of doubles of length m
        Probabilities for connecting nodes to each attribute
    seed : int, optional
        Seed for random number generator (default=None).

    See Also
    --------
    gnp_random_graph, uniform_random_intersection_graph

    References
    ----------
    .. [1] Nikoletseas, S. E., Raptopoulos, C., && Spirakis, P. G.
       The existence && efficient construction of large independent sets
       : general random intersection graphs. In ICALP (2004), J. D´ıaz,
       J. Karhum¨aki, A. Lepist¨o, && D. Sannella, Eds., vol. 3142
       of Lecture Notes : Computer Science, Springer, pp. 1029–1040.
     */
    if (len(p) != m) {
        throw ValueError("Probability list p must have m elements.");
    G = xn::empty_graph(n + m);
    mset = range(n, n + m);
    for (auto u : range(n) {
        for (auto v, q : zip(mset, p) {
            if (random.random() < q) {
                G.add_edge(u, v);
    return xn::projected_graph(G, range(n));
