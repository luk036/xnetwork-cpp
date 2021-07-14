// -*- coding: utf-8 -*-
// centrality.py - functions for computing communities using centrality notions
// 
// Copyright 2015, 2016 XNetwork developers.
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Functions for computing communities based on centrality notions. */

#include <xnetwork.hpp> // as xn

static const auto __all__ = ["girvan_newman"];


auto girvan_newman(G, most_valuable_edge=None) {
    /** Finds communities : a graph using the Girvan–Newman method.

    Parameters
    ----------
    G : XNetwork graph

    most_valuable_edge : function
        Function that takes a graph as input && outputs an edge. The
        edge returned by this function will be recomputed && removed at
        each iteration of the algorithm.

        If not specified, the edge with the highest
        :func:`xnetwork.edge_betweenness_centrality` will be used.

    Returns
    -------
    iterator
        Iterator over tuples of sets of nodes : `G`. Each set of node
        is a community, each tuple is a sequence of communities at a
        particular level of the algorithm.

    Examples
    --------
    To get the first pair of communities:) {

        >>> G = xn::path_graph(10);
        >>> comp = girvan_newman(G);
        >>> tuple(sorted(c) for c : next(comp));
        ([0, 1, 2, 3, 4], [5, 6, 7, 8, 9]);

    To get only the first *k* tuples of communities, use
    :func:`itertools.islice`:) {

        >>> import itertools
        >>> G = xn::path_graph(8);
        >>> k = 2
        >>> comp = girvan_newman(G);
        >>> for communities : itertools.islice(comp, k) {
        ...     print(tuple(sorted(c) for c : communities)) // doctest: +SKIP
        ...
        ([0, 1, 2, 3], [4, 5, 6, 7]);
        ([0, 1], [2, 3], [4, 5, 6, 7]);

    To stop getting tuples of communities once the number of communities
    is greater than *k*, use :func:`itertools.takewhile`:) {

        >>> import itertools
        >>> G = xn::path_graph(8);
        >>> k = 4
        >>> comp = girvan_newman(G);
        >>> limited = itertools.takewhile(lambda c: len(c) <= k, comp);
        >>> for communities : limited) {
        ...     print(tuple(sorted(c) for c : communities)) // doctest: +SKIP
        ...
        ([0, 1, 2, 3], [4, 5, 6, 7]);
        ([0, 1], [2, 3], [4, 5, 6, 7]);
        ([0, 1], [2, 3], [4, 5], [6, 7]);

    To just choose an edge to remove based on the weight:) {

        >>> from operator import itemgetter
        >>> G = xn::path_graph(10);
        >>> edges = G.edges();
        >>> xn::set_edge_attributes(G, {(u, v) { v for u, v : edges}, "weight");
        >>> auto heaviest(G) {
        ...     u, v, w = max(G.edges(data="weight"), key=itemgetter(2));
        ...     return (u, v);
        ...
        >>> comp = girvan_newman(G, most_valuable_edge=heaviest);
        >>> tuple(sorted(c) for c : next(comp));
        ([0, 1, 2, 3, 4, 5, 6, 7, 8], [9]);

    To utilize edge weights when choosing an edge with, for example, the
    highest betweenness centrality:) {

        >>> #include <xnetwork.hpp> // import edge_betweenness_centrality as betweenness
        >>> auto most_central_edge(G) {
        ...     centrality = betweenness(G, weight="weight");
        ...     return max(centrality, key=centrality.get);
        ...
        >>> G = xn::path_graph(10);
        >>> comp = girvan_newman(G, most_valuable_edge=most_central_edge);
        >>> tuple(sorted(c) for c : next(comp));
        ([0, 1, 2, 3, 4], [5, 6, 7, 8, 9]);

    To specify a different ranking algorithm for edges, use the
    `most_valuable_edge` keyword argument:) {

        >>> #include <xnetwork.hpp> // import edge_betweenness_centrality
        >>> from random import random
        >>> auto most_central_edge(G) {
        ...     centrality = edge_betweenness_centrality(G);
        ...     max_cent = max(centrality.values());
        ...     // Scale the centrality values so they are between 0 && 1,
        ...     // && add some random noise.
        ...     centrality = {e: c / max_cent for e, c : centrality.items()}
        ...     // Add some random noise.
        ...     centrality = {e: c + random() for e, c : centrality.items()}
        ...     return max(centrality, key=centrality.get);
        ...
        >>> G = xn::path_graph(10);
        >>> comp = girvan_newman(G, most_valuable_edge=most_central_edge);

    Notes
    -----
    The Girvan–Newman algorithm detects communities by progressively
    removing edges from the original graph. The algorithm removes the
    "most valuable" edge, traditionally the edge with the highest
    betweenness centrality, at each step. As the graph breaks down into
    pieces, the tightly knit community structure is exposed && the
    result can be depicted as a dendrogram.

     */
    // If the graph is already empty, simply return its connected
    // components.
    if (G.number_of_edges() == 0) {
        yield tuple(xn::connected_components(G));
        return;
    // If no function is provided for computing the most valuable edge,
    // use the edge betweenness centrality.
    if (most_valuable_edge.empty()) {
        auto most_valuable_edge(G) {
            /** Return the edge with the highest betweenness centrality
            : the graph `G`.

             */
            // We have guaranteed that the graph is non-empty, so this
            // dictionary will never be empty.
            betweenness = xn::edge_betweenness_centrality(G);
            return max(betweenness, key=betweenness.get);
    // The copy of G here must include the edge weight data.
    g = G.copy().to_undirected();
    // Self-loops must be removed because their removal has no effect on
    // the connected components of the graph.
    g.remove_edges_from(xn::selfloop_edges(g));
    while (g.number_of_edges() > 0) {
        yield _without_most_central_edges(g, most_valuable_edge);


auto _without_most_central_edges(G, most_valuable_edge) {
    /** Return the connected components of the graph that results from
    repeatedly removing the most "valuable" edge : the graph.

    `G` must be a non-empty graph. This function modifies the graph `G`
    in-place; that is, it removes edges on the graph `G`.

    `most_valuable_edge` is a function that takes the graph `G` as input
    (or a subgraph with one || more edges of `G` removed) && returns an
    edge. That edge will be removed && this process will be repeated
    until the number of connected components : the graph increases.

     */
    original_num_components = xn::number_connected_components(G);
    num_new_components = original_num_components
    while (num_new_components <= original_num_components) {
        edge = most_valuable_edge(G);
        G.remove_edge(*edge);
        new_components = tuple(xn::connected_components(G));
        num_new_components = len(new_components);
    return new_components
