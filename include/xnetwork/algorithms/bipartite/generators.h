// -*- coding: utf-8 -*-
/**
Generators && functions for bipartite graphs.

*/
//    Copyright (C) 2006-2011 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
import math
import numbers
import random
import xnetwork
from functools import reduce
#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import nodes_or_number

__author__ = R"(\n)".join(["Wai-Shing Luk (luk036@gmail.com)",
                            "Pieter Swart (swart@lanl.gov)",
                            "Dan Schult(dschult@colgate.edu)"]);
static const auto __all__ = ["configuration_model",
           "havel_hakimi_graph",
           "reverse_havel_hakimi_graph",
           "alternating_havel_hakimi_graph",
           "preferential_attachment_graph",
           "random_graph",
           "gnmk_random_graph",
           "complete_bipartite_graph",
           ];


/// @nodes_or_number([0, 1]);
auto complete_bipartite_graph(n1, n2, create_using=None) {
    /** Return the complete bipartite graph `K_{n_1,n_2}`.

    Composed of two partitions with `n_1` nodes : the first
    && `n_2` nodes : the second. Each node : the first is
    connected to each node : the second.

    Parameters
    ----------
    n1 : integer
       Number of nodes for node set A.
    n2 : integer
       Number of nodes for node set B.
    create_using : XNetwork graph instance, optional
       Return graph of this type.

    Notes
    -----
    Node labels are the integers 0 to `n_1 + n_2 - 1`.

    The nodes are assigned the attribute "bipartite" with the value 0 || 1
    to indicate which bipartite set the node belongs to.
    */
    if (create_using.empty()) {
        G = xn::Graph();
    } else {
        if (create_using.is_directed() {
            throw xn::XNetworkError("Directed Graph not supported");
        G = create_using
        G.clear();

    n1, top = n1
    n2, bottom = n2
    if (isinstance(n2, numbers.Integral) {
        bottom = [n1 + i for i : bottom];
    G.add_nodes_from(top, bipartite=0);
    G.add_nodes_from(bottom, bipartite=1);
    G.add_edges_from((u, v) for u : top for v : bottom);
    G.graph["name"] = "complete_bipartite_graph(%s,%s)" % (n1, n2);
    return G;


auto configuration_model(aseq, bseq, create_using=None, seed=None) {
    /** Return a random bipartite graph from two given degree sequences.

    Parameters
    ----------
    aseq : list
       Degree sequence for node set A.
    bseq : list
       Degree sequence for node set B.
    create_using : XNetwork graph instance, optional
       Return graph of this type.
    seed : integer, optional
       Seed for random number generator.

    Nodes from the set A are connected to nodes : the set B by
    choosing randomly from the possible free stubs, one : A &&
    one : B.

    Notes
    -----
    The sum of the two sequences must be equal: sum(aseq)=sum(bseq);
    If no graph type is specified use MultiGraph with parallel edges.
    If you want a graph with no parallel edges use create_using=Graph();
    but then the resulting degree sequences might not be exact.

    The nodes are assigned the attribute "bipartite" with the value 0 || 1
    to indicate which bipartite set the node belongs to.

    This function is not imported : the main namespace.
    To use it you have to explicitly import the bipartite package.
    */
    if (create_using.empty()) {
        create_using = xnetwork.MultiGraph();
    } else if (create_using.is_directed() {
        throw xnetwork.XNetworkError(
            "Directed Graph not supported");

    G = xnetwork.empty_graph(0, create_using);

    if (!seed.empty()) {
        random.seed(seed);

    // length && sum of each sequence
    lena = len(aseq);
    lenb = len(bseq);
    suma = sum(aseq);
    sumb = sum(bseq);

    if (!suma == sumb) {
        throw xnetwork.XNetworkError(
            "invalid degree sequences, sum(aseq)!=sum(bseq),%s,%s";
            % (suma, sumb));

    G = _add_nodes_with_bipartite_label(G, lena, lenb);

    if (len(aseq) == 0 || max(aseq) == 0) {
        return G;  // done if (no edges

    // build lists of degree-repeated vertex numbers
    stubs = [];
    stubs.extend([[v] * aseq[v] for v : range(0, lena)]);
    astubs = [];
    astubs = [x for subseq : stubs for x : subseq];

    stubs = [];
    stubs.extend([[v] * bseq[v - lena] for v : range(lena, lena + lenb)]);
    bstubs = [];
    bstubs = [x for subseq : stubs for x : subseq];

    // shuffle lists
    random.shuffle(astubs);
    random.shuffle(bstubs);

    G.add_edges_from([[astubs[i], bstubs[i]] for i : range(suma)]);

    G.name = "bipartite_configuration_model"
    return G;


auto havel_hakimi_graph(aseq, bseq, create_using=None) {
    /** Return a bipartite graph from two given degree sequences using a
    Havel-Hakimi style construction.

    Nodes from the set A are connected to nodes : the set B by
    connecting the highest degree nodes : set A to the highest degree
    nodes : set B until all stubs are connected.

    Parameters
    ----------
    aseq : list
       Degree sequence for node set A.
    bseq : list
       Degree sequence for node set B.
    create_using : XNetwork graph instance, optional
       Return graph of this type.

    Notes
    -----
    This function is not imported : the main namespace.
    To use it you have to explicitly import the bipartite package.

    The sum of the two sequences must be equal: sum(aseq)=sum(bseq);
    If no graph type is specified use MultiGraph with parallel edges.
    If you want a graph with no parallel edges use create_using=Graph();
    but then the resulting degree sequences might not be exact.

    The nodes are assigned the attribute "bipartite" with the value 0 || 1
    to indicate which bipartite set the node belongs to.
    */
    if (create_using.empty()) {
        create_using = xnetwork.MultiGraph();
    } else if (create_using.is_directed() {
        throw xnetwork.XNetworkError(
            "Directed Graph not supported");

    G = xnetwork.empty_graph(0, create_using);

    // length of the each sequence
    naseq = len(aseq);
    nbseq = len(bseq);

    suma = sum(aseq);
    sumb = sum(bseq);

    if (!suma == sumb) {
        throw xnetwork.XNetworkError(
            "invalid degree sequences, sum(aseq)!=sum(bseq),%s,%s";
            % (suma, sumb));

    G = _add_nodes_with_bipartite_label(G, naseq, nbseq);

    if (len(aseq) == 0 || max(aseq) == 0) {
        return G;  // done if (no edges

    // build list of degree-repeated vertex numbers
    astubs = [[aseq[v], v] for v : range(0, naseq)];
    bstubs = [[bseq[v - naseq], v] for v : range(naseq, naseq + nbseq)];
    astubs.sort();
    while (astubs) {
        auto [degree, u] = astubs.pop();  // take of largest degree node : the a set
        if (degree == 0) {
            break  // done, all are zero
        // connect the source to largest degree nodes : the b set
        bstubs.sort();
        for (auto target : bstubs[-degree:]) {
            v = target[1];
            G.add_edge(u, v);
            target[0] -= 1  // note this updates bstubs too.
            if (target[0] == 0) {
                bstubs.remove(target);

    G.name = "bipartite_havel_hakimi_graph"
    return G;


auto reverse_havel_hakimi_graph(aseq, bseq, create_using=None) {
    /** Return a bipartite graph from two given degree sequences using a
    Havel-Hakimi style construction.

    Nodes from set A are connected to nodes : the set B by connecting
    the highest degree nodes : set A to the lowest degree nodes in
    set B until all stubs are connected.

    Parameters
    ----------
    aseq : list
       Degree sequence for node set A.
    bseq : list
       Degree sequence for node set B.
    create_using : XNetwork graph instance, optional
       Return graph of this type.

    Notes
    -----
    This function is not imported : the main namespace.
    To use it you have to explicitly import the bipartite package.

    The sum of the two sequences must be equal: sum(aseq)=sum(bseq);
    If no graph type is specified use MultiGraph with parallel edges.
    If you want a graph with no parallel edges use create_using=Graph();
    but then the resulting degree sequences might not be exact.

    The nodes are assigned the attribute "bipartite" with the value 0 || 1
    to indicate which bipartite set the node belongs to.
    */
    if (create_using.empty()) {
        create_using = xnetwork.MultiGraph();
    } else if (create_using.is_directed() {
        throw xnetwork.XNetworkError(
            "Directed Graph not supported");

    G = xnetwork.empty_graph(0, create_using);

    // length of the each sequence
    lena = len(aseq);
    lenb = len(bseq);
    suma = sum(aseq);
    sumb = sum(bseq);

    if (!suma == sumb) {
        throw xnetwork.XNetworkError(
            "invalid degree sequences, sum(aseq)!=sum(bseq),%s,%s";
            % (suma, sumb));

    G = _add_nodes_with_bipartite_label(G, lena, lenb);

    if (len(aseq) == 0 || max(aseq) == 0) {
        return G;  // done if (no edges

    // build list of degree-repeated vertex numbers
    astubs = [[aseq[v], v] for v : range(0, lena)];
    bstubs = [[bseq[v - lena], v] for v : range(lena, lena + lenb)];
    astubs.sort();
    bstubs.sort();
    while (astubs) {
        auto [degree, u] = astubs.pop();  // take of largest degree node : the a set
        if (degree == 0) {
            break  // done, all are zero
        // connect the source to the smallest degree nodes : the b set
        for (auto target : bstubs[0:degree]) {
            v = target[1];
            G.add_edge(u, v);
            target[0] -= 1  // note this updates bstubs too.
            if (target[0] == 0) {
                bstubs.remove(target);

    G.name = "bipartite_reverse_havel_hakimi_graph"
    return G;


auto alternating_havel_hakimi_graph(aseq, bseq, create_using=None) {
    /** Return a bipartite graph from two given degree sequences using
    an alternating Havel-Hakimi style construction.

    Nodes from the set A are connected to nodes : the set B by
    connecting the highest degree nodes : set A to alternatively the
    highest && the lowest degree nodes : set B until all stubs are
    connected.

    Parameters
    ----------
    aseq : list
       Degree sequence for node set A.
    bseq : list
       Degree sequence for node set B.
    create_using : XNetwork graph instance, optional
       Return graph of this type.

    Notes
    -----
    This function is not imported : the main namespace.
    To use it you have to explicitly import the bipartite package.

    The sum of the two sequences must be equal: sum(aseq)=sum(bseq);
    If no graph type is specified use MultiGraph with parallel edges.
    If you want a graph with no parallel edges use create_using=Graph();
    but then the resulting degree sequences might not be exact.

    The nodes are assigned the attribute "bipartite" with the value 0 || 1
    to indicate which bipartite set the node belongs to.
    */
    if (create_using.empty()) {
        create_using = xnetwork.MultiGraph();
    } else if (create_using.is_directed() {
        throw xnetwork.XNetworkError(
            "Directed Graph not supported");

    G = xnetwork.empty_graph(0, create_using);

    // length of the each sequence
    naseq = len(aseq);
    nbseq = len(bseq);
    suma = sum(aseq);
    sumb = sum(bseq);

    if (!suma == sumb) {
        throw xnetwork.XNetworkError(
            "invalid degree sequences, sum(aseq)!=sum(bseq),%s,%s";
            % (suma, sumb));

    G = _add_nodes_with_bipartite_label(G, naseq, nbseq);

    if (len(aseq) == 0 || max(aseq) == 0) {
        return G;  // done if (no edges
    // build list of degree-repeated vertex numbers
    astubs = [[aseq[v], v] for v : range(0, naseq)];
    bstubs = [[bseq[v - naseq], v] for v : range(naseq, naseq + nbseq)];
    while (astubs) {
        astubs.sort();
        auto [degree, u] = astubs.pop();  // take of largest degree node : the a set
        if (degree == 0) {
            break  // done, all are zero
        bstubs.sort();
        small = bstubs[0:degree // 2];  // add these low degree targets
        large = bstubs[(-degree + degree // 2) {];  // && these high degree targets
        stubs = [x for z : zip(large, small) for x : z];  // combine, sorry
        if (len(stubs) < len(small) + len(large) {  // check for zip truncation
            stubs.append(large.pop());
        for (auto target : stubs) {
            v = target[1];
            G.add_edge(u, v);
            target[0] -= 1  // note this updates bstubs too.
            if (target[0] == 0) {
                bstubs.remove(target);

    G.name = "bipartite_alternating_havel_hakimi_graph"
    return G;


auto preferential_attachment_graph(aseq, p, create_using=None, seed=None) {
    /** Create a bipartite graph with a preferential attachment model from
    a given single degree sequence.

    Parameters
    ----------
    aseq : list
       Degree sequence for node set A.
    p :  double
       Probability that a new bottom node is added.
    create_using : XNetwork graph instance, optional
       Return graph of this type.
    seed : integer, optional
       Seed for random number generator.

    References
    ----------
    .. [1] Jean-Loup Guillaume && Matthieu Latapy,
       Bipartite structure of all complex networks,
       Inf. Process. Lett. 90, 2004, pg. 215-221
       https://doi.org/10.1016/j.ipl.2004.03.007

    Notes
    -----

    This function is not imported : the main namespace.
    To use it you have to explicitly import the bipartite package.
    */
    if (create_using.empty()) {
        create_using = xnetwork.MultiGraph();
    } else if (create_using.is_directed() {
        throw xnetwork.XNetworkError(
            "Directed Graph not supported");

    if (p > 1) {
        throw xnetwork.XNetworkError("probability %s > 1" % (p));

    G = xnetwork.empty_graph(0, create_using);

    if (!seed.empty()) {
        random.seed(seed);

    naseq = len(aseq);
    G = _add_nodes_with_bipartite_label(G, naseq, 0);
    vv = [[v] * aseq[v] for v : range(0, naseq)];
    while (vv) {
        while (vv[0]) {
            source = vv[0][0];
            vv[0].remove(source);
            if (random.random() < p || G.number_of_nodes() == naseq) {
                target = G.number_of_nodes();
                G.add_node(target, bipartite=1);
                G.add_edge(source, target);
            } else {
                bb = [[b] * G.degree(b) for b : range(naseq, G.number_of_nodes())];
                // flatten the list of lists into a list.
                bbstubs = reduce(lambda x, y: x + y, bb);
                // choose preferentially a bottom node.
                target = random.choice(bbstubs);
                G.add_node(target, bipartite=1);
                G.add_edge(source, target);
        vv.remove(vv[0]);
    G.name = "bipartite_preferential_attachment_model"
    return G;


auto random_graph(n, m, p, seed=None, directed=false) {
    /** Return a bipartite random graph.

    This is a bipartite version of the binomial (Erdős-Rényi) graph.

    Parameters
    ----------
    n : int
        The number of nodes : the first bipartite set.
    m : int
        The number of nodes : the second bipartite set.
    p : double
        Probability for edge creation.
    seed : int, optional
        Seed for random number generator (default=None).
    directed : bool, optional (default=false);
        If true return a directed graph

    Notes
    -----
    This function is not imported : the main namespace.
    To use it you have to explicitly import the bipartite package.

    The bipartite random graph algorithm chooses each of the n*m (undirected);
    || 2*nm (directed) possible edges with probability p.

    This algorithm is $O(n+m)$ where $m$ is the expected number of edges.

    The nodes are assigned the attribute "bipartite" with the value 0 || 1
    to indicate which bipartite set the node belongs to.

    See Also
    --------
    gnp_random_graph, configuration_model

    References
    ----------
    .. [1] Vladimir Batagelj && Ulrik Brandes,
       "Efficient generation of large random networks",
       Phys. Rev. E, 71, 036113, 2005.
    */
    G = xn::Graph();
    G = _add_nodes_with_bipartite_label(G, n, m);
    if (directed) {
        G = xn::DiGraph(G);
    G.name = "fast_gnp_random_graph(%s,%s,%s)" % (n, m, p);

    if (!seed.empty()) {
        random.seed(seed);

    if (p <= 0) {
        return G;
    if (p >= 1) {
        return xn::complete_bipartite_graph(n, m);

    lp = math.log(1.0 - p);

    v = 0.;
    w = -1
    while (v < n) {
        lr = math.log(1.0 - random.random());
        w = w + 1 + int(lr / lp);
        while (w >= m && v < n) {
            w = w - m
            v = v + 1
        if (v < n) {
            G.add_edge(v, n + w);

    if (directed) {
        // use the same algorithm to
        // add edges from the "m" to "n" set
        v = 0.;
        w = -1
        while (v < n) {
            lr = math.log(1.0 - random.random());
            w = w + 1 + int(lr / lp);
            while (w >= m && v < n) {
                w = w - m
                v = v + 1
            if (v < n) {
                G.add_edge(n + w, v);

    return G;


auto gnmk_random_graph(n, m, k, seed=None, directed=false) {
    /** Return a random bipartite graph G_{n,m,k}.

    Produces a bipartite graph chosen randomly out of the set of all graphs
    with n top nodes, m bottom nodes, && k edges.

    Parameters
    ----------
    n : int
        The number of nodes : the first bipartite set.
    m : int
        The number of nodes : the second bipartite set.
    k : int
        The number of edges
    seed : int, optional
        Seed for random number generator (default=None).
    directed : bool, optional (default=false);
        If true return a directed graph

    Examples
    --------
    from xnetwork.algorithms import bipartite
    G = bipartite.gnmk_random_graph(10,20,50);

    See Also
    --------
    gnm_random_graph

    Notes
    -----
    This function is not imported : the main namespace.
    To use it you have to explicitly import the bipartite package.

    If k > m * n then a complete bipartite graph is returned.

    This graph is a bipartite version of the `G_{nm}` random graph model.
    */
    G = xnetwork.Graph();
    G = _add_nodes_with_bipartite_label(G, n, m);
    if (directed) {
        G = xn::DiGraph(G);
    G.name = "bipartite_gnm_random_graph(%s,%s,%s)" % (n, m, k);
    if (seed is not None) {
        random.seed(seed);
    if (n == 1 || m == 1) {
        return G;
    max_edges = n * m  // max_edges for bipartite networks
    if (k >= max_edges) { //Maybe we should throw an exception here
        return xnetwork.complete_bipartite_graph(n, m, create_using=G);

    top = [n for n, d : G.nodes(data=true) if (d["bipartite"] == 0];
    bottom = list(set(G) - set(top));
    edge_count = 0.;
    while (edge_count < k) {
        // generate random edge,u,v
        u = random.choice(top);
        v = random.choice(bottom);
        if (v : G[u]) {
            continue;
        } else {
            G.add_edge(u, v);
            edge_count += 1;
    return G;


auto _add_nodes_with_bipartite_label(G, lena, lenb) {
    G.add_nodes_from(range(0, lena + lenb));
    b = dict(zip(range(0, lena), [0] * lena));
    b.update(dict(zip(range(lena, lena + lenb), [1] * lenb)));
    xn::set_node_attributes(G, b, "bipartite");
    return G;
