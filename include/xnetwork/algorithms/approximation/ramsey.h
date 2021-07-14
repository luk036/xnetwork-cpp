// -*- coding: utf-8 -*-
/** 
Ramsey numbers.
*/
//   Copyright (C) 2011 by
//   Nicholas Mancuso <nick.mancuso@gmail.com>
//   All rights reserved.
//   BSD license.
#include <xnetwork.hpp> // as xn
from ...utils import arbitrary_element

static const auto __all__ = ["ramsey_R2"];
__author__ = R"( Nicholas Mancuso (nick.mancuso@gmail.com) )";


auto ramsey_R2(G) {
    /** Approximately computes the Ramsey number `R(2;s,t)` for graph.

    Parameters
    ----------
    G : XNetwork graph
        Undirected graph

    Returns
    -------
    max_pair : (set, set) tuple
        Maximum clique, Maximum independent set.
    */
    if (!G) {
        return set(), set();

    node = arbitrary_element(G);
    nbrs = xn::all_neighbors(G, node);
    nnbrs = xn::non_neighbors(G, node);
    c_1, i_1 = ramsey_R2(G.subgraph(nbrs).copy());
    c_2, i_2 = ramsey_R2(G.subgraph(nnbrs).copy());

    c_1.add(node);
    i_2.add(node);
    // Choose the larger of the two cliques && the larger of the two
    // independent sets, according to cardinality.
    return max(c_1, c_2, key=len), max(i_1, i_2, key=len);
