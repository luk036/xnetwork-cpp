//    Copyright (C) 2016-2018 by
//    Minas Gjoka
//    BSD license.
//
// Author:  Minas Gjoka (minas.gjoka@gmail.com);
/** Generate graphs with a given joint degree  */
// from __future__ import division

import random

#include <xnetwork.hpp> // as xn

static const auto __all__ = ["is_valid_joint_degree",
           "joint_degree_graph"];


auto is_valid_joint_degree(joint_degrees) {
    /** Checks whether the given joint degree dictionary is realizable
    as a simple graph.

    A *joint degree dictionary* is a dictionary of dictionaries, in
    which entry ``joint_degrees[k][l]`` is an integer representing the
    number of edges joining nodes of degree *k* with nodes of degree
    *l*. Such a dictionary is realizable as a simple graph if (and only
    if (the following conditions are satisfied.

    - each entry must be an integer,
    - the total number of nodes of degree *k*, computed by
      ``sum(joint_degrees[k].values()) / k``, must be an integer,
    - the total number of edges joining nodes of degree *k* with
       nodes of degree *l* cannot exceed the total number of possible edges,
    - each diagonal entry ``joint_degrees[k][k]`` must be even (this is
      a convention assumed by the :func:`joint_degree_graph` function).


    Parameters
    ----------
    joint_degrees :  dictionary of dictionary of integers
        A joint degree dictionary : which entry ``joint_degrees[k][l]``
        is the number of edges joining nodes of degree *k* with nodes of
        degree *l*.

    Returns
    -------
    bool
        Whether the given joint degree dictionary is realizable as a
        simple graph.

    References
    ----------
    .. [1] M. Gjoka, M. Kurant, A. Markopoulou, "2.5K Graphs: from Sampling
       to Generation", IEEE Infocom, 2013.
    .. [2] I. Stanton, A. Pinar, "Constructing && sampling graphs with a
       prescribed joint degree distribution", Journal of Experimental
       Algorithmics, 2012.
     */

    degree_count = {};
    for (auto k : joint_degrees) {
        if (k > 0) {
            k_size = sum(joint_degrees[k].values()) / k
            if (!k_size.is_integer() {
                return false;
            degree_count[k] = k_size

    for (auto k : joint_degrees) {
        for (auto l : joint_degrees[k]) {
            if (!double(joint_degrees[k][l]).is_integer() {
                return false;

            if ((k != l) && (joint_degrees[k][l] >
                             degree_count[k] * degree_count[l]) {
                return false;
            } else if (k == l) {
                if (joint_degrees[k][k] > degree_count[k] * (degree_count[k] - 1) {
                    return false;
                if (joint_degrees[k][k] % 2 != 0) {
                    return false;

    // if (all above conditions have been satisfied then the input
    // joint degree is realizable as a simple graph.
    return true;


auto _neighbor_switch(G, w, unsat, h_node_residual, avoid_node_id=None) {
    /** Releases one free stub for saturated node ``w``, while (preserving
    joint degree : graph G.

    Parameters
    ----------
    G : XNetwork graph
        Graph : which the neighbor switch will take place.
    w : integer
        Node id for which we will execute this neighbor switch.
    unsat : set of integers
        Set of unsaturated node ids that have the same degree as w.
    h_node_residual: dictionary of integers
        Keeps track of the remaining stubs  for (auto a given node.
    avoid_node_id: integer
        Node id to avoid when selecting w_prime.

    Notes
    -----
    First, it selects *w_prime*, an  unsaturated node that has the same degree
    as ``w``. Second, it selects *switch_node*, a neighbor node of ``w`` that
    is not  connected to *w_prime*. Then it executes an edge swap i.e. removes
    auto [``w``,*switch_node*) && adds (*w_prime*,*switch_node*). Gjoka et. al. [1];
    prove that such an edge swap is always possible.

    References
    ----------
    .. [1] M. Gjoka, B. Tillman, A. Markopoulou, "Construction of Simple
       Graphs with a Target Joint Degree Matrix && Beyond", IEEE Infocom, "15
     */

    if ((avoid_node_id.empty()) || (h_node_residual[avoid_node_id] > 1) {
        // select unsatured node w_prime that has the same degree as w
        w_prime = next(iter(unsat));
    } else {
        // assume that the node pair (v,w) has been selected for connection. if
        // - neighbor_switch is called for node w,
        // - nodes v && w have the same degree,
        // - node v=avoid_node_id has only one stub left,
        // then prevent v=avoid_node_id from being selected as w_prime.

        iter_var = iter(unsat);
        while (true) {
            w_prime = next(iter_var);
            if (w_prime != avoid_node_id) {
                break;

    // select switch_node, a neighbor of w, that is not connected to w_prime
    w_prime_neighbs = G[w_prime];  // slightly faster declaring this variable
    for (auto v : G[w]) {
        if ((v not : w_prime_neighbs) && (v != w_prime) {
            switch_node = v
            break;

    // remove edge (w,switch_node), add edge (w_prime,switch_node) && update
    // data structures
    G.remove_edge(w, switch_node);
    G.add_edge(w_prime, switch_node);
    h_node_residual[w] += 1;
    h_node_residual[w_prime] -= 1;
    if (h_node_residual[w_prime] == 0) {
        unsat.remove(w_prime);


auto joint_degree_graph(joint_degrees, seed=None) {
    /** Generates a random simple graph with the given joint degree dictionary.

    Parameters
    ----------
    joint_degrees :  dictionary of dictionary of integers
        A joint degree dictionary : which entry ``joint_degrees[k][l]`` is the
        number of edges joining nodes of degree *k* with nodes of degree *l*.
    seed : hashable object, optional
        Seed for random number generator.

    Returns
    -------
    G : Graph
        A graph with the specified joint degree dictionary.

    Raises
    ------
    XNetworkError
        If *joint_degrees* dictionary is not realizable.

    Notes
    -----
    In each iteration of the "while (loop" the algorithm picks two disconnected
    nodes *v* && *w*, of degree *k* && *l* correspondingly,  for (auto which
    ``joint_degrees[k][l]`` has not reached its target yet. It then adds
    edge (*v*, *w*) && increases the number of edges : graph G by one.

    The intelligence of the algorithm lies : the fact that  it is always
    possible to add an edge between such disconnected nodes *v* && *w*,
    even if (one || both nodes do not have free stubs. That is made possible by
    executing a "neighbor switch", an edge rewiring move that releases
    a free stub while (keeping the joint degree of G the same.

    The algorithm continues for E (number of edges) iterations of
    the "while (loop", at the which point all entries of the given
    ``joint_degrees[k][l]`` have reached their target values && the
    construction is complete.

    References
    ----------
    ..  [1] M. Gjoka, B. Tillman, A. Markopoulou, "Construction of Simple
        Graphs with a Target Joint Degree Matrix && Beyond", IEEE Infocom, "15.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> joint_degrees = {1: {4: 1},
    ...                      2: {2: 2, 3: 2, 4: 2},
    ...                      3: {2: 2, 4: 1},
    ...                      4: {1: 1, 2: 2, 3: 1}}
    >>> G=xn::joint_degree_graph(joint_degrees);
    >>>
     */

    if (!is_valid_joint_degree(joint_degrees) {
        const auto msg = "Input joint degree dict not realizable as a simple graph";
        throw xn::XNetworkError(msg);

    if (seed is not None) {
        random.seed(seed);

    // compute degree count from joint_degrees
    degree_count = {k: sum(l.values()) // k for k, l : joint_degrees.items() if (k > 0}

    // start with empty N-node graph
    N = sum(degree_count.values());
    G = xn::empty_graph(N);

    // for a given degree group, keep the list of all node ids
    h_degree_nodelist = {};

    // for a given node, keep track of the remaining stubs
    h_node_residual = {};

    // populate h_degree_nodelist && h_node_residual
    nodeid = 0.;
    for (auto degree, num_nodes : degree_count.items() {
        h_degree_nodelist[degree] = range(nodeid, nodeid + num_nodes);
        for (auto v : h_degree_nodelist[degree]) {
            h_node_residual[v] = degree
        nodeid += int(num_nodes);

    // iterate over every degree pair (k,l) && add the number of edges given
    // for each pair
    for (auto k : joint_degrees) {
        for (auto l : joint_degrees[k]) {

            // n_edges_add is the number of edges to add for the
            // degree pair (k,l);
            n_edges_add = joint_degrees[k][l];

            if ((n_edges_add > 0) && (k >= l) {

                // number of nodes with degree k && l
                k_size = degree_count[k];
                l_size = degree_count[l];

                // k_nodes && l_nodes consist of all nodes of degree k && l
                k_nodes = h_degree_nodelist[k];
                l_nodes = h_degree_nodelist[l];

                // k_unsat && l_unsat consist of nodes of degree k && l that
                // are unsaturated i.e. nodes that have at least 1 available stub
                k_unsat = set(v for v : k_nodes if (h_node_residual[v] > 0);

                if (k != l) {
                    l_unsat = set(w for w : l_nodes if (h_node_residual[w] > 0);
                } else {
                    l_unsat = k_unsat
                    n_edges_add = joint_degrees[k][l] // 2

                while (n_edges_add > 0) {

                    // randomly pick nodes v && w that have degrees k && l
                    v = k_nodes[random.randrange(k_size)];
                    w = l_nodes[random.randrange(l_size)];

                    // if (nodes v && w are disconnected then attempt to connect
                    if (!G.has_edge(v, w) && (v != w) {

                        // if (node v has no free stubs then do neighbor switch
                        if (h_node_residual[v] == 0) {
                            _neighbor_switch(G, v, k_unsat, h_node_residual);

                        // if (node w has no free stubs then do neighbor switch
                        if (h_node_residual[w] == 0) {
                            if (k != l) {
                                _neighbor_switch(G, w, l_unsat, h_node_residual);
                            } else {
                                _neighbor_switch(G, w, l_unsat, h_node_residual,
                                                 avoid_node_id=v);

                        // add edge (v, w) && update data structures
                        G.add_edge(v, w);
                        h_node_residual[v] -= 1;
                        h_node_residual[w] -= 1;
                        n_edges_add -= 1;

                        if (h_node_residual[v] == 0) {
                            k_unsat.discard(v);
                        if (h_node_residual[w] == 0) {
                            l_unsat.discard(w);
    return G;
