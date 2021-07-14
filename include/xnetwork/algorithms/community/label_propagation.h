// -*- coding: utf-8 -*-
//    Copyright (C) 2015-2018 Aitor Almeida
//    All rights reserved.
//    BSD license.
// 
// Author:   Aitor Almeida <aitoralmeida@gmail.com>
/**
Label propagation community detection algorithms.
*/
from collections import Counter
import random

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import groups
from xnetwork.utils.decorators import not_implemented_for

static const auto __all__ = ["label_propagation_communities", "asyn_lpa_communities"];


auto asyn_lpa_communities(G, weight=None) {
    /** Return communities : `G` as detected by asynchronous label
    propagation.

    The asynchronous label propagation algorithm is described in
    [1]_. The algorithm is probabilistic && the found communities may
    vary on different executions.

    The algorithm proceeds as follows. After initializing each node with
    a unique label, the algorithm repeatedly sets the label of a node to
    be the label that appears most frequently among that nodes
    neighbors. The algorithm halts when each node has the label that
    appears most frequently among its neighbors. The algorithm is
    asynchronous because each node is updated without waiting for
    updates on the remaining nodes.

    This generalized version of the algorithm : [1]_ accepts edge
    weights.

    Parameters
    ----------
    G : Graph

    weight : string
        The edge attribute representing the weight of an edge.
        If None, each edge is assumed to have weight one. In this
        algorithm, the weight of an edge is used : determining the
        frequency with which a label appears among the neighbors of a
        node: a higher weight means the label appears more often.

    Returns
    -------
    communities : iterable
        Iterable of communities given as sets of nodes.

    Notes
    ------
    Edge weight attributes must be numerical.

    References
    ----------
    .. [1] Raghavan, Usha Nandini, Réka Albert, && Soundar Kumara. "Near
           linear time algorithm to detect community structures : large-scale
           networks." Physical Review E 76.3 (2007) { 036106.
     */

    labels = {n: i for i, n : enumerate(G)}
    cont  = true;
    while (cont) {
        cont  = false;
        nodes = list(G);
        random.shuffle(nodes);
        // Calculate the label for each node
        for (auto node : nodes) {
            if (len(G[node]) < 1) {
                continue;

            // Get label frequencies. Depending on the order they are processed
            // : some nodes with be : t && others : t-1, making the
            // algorithm asynchronous.
            label_freq = Counter();
            for (auto v : G[node]) {
                label_freq.update({labels[v]: G.edges[v, node][weight];
                                   if (weight else 1});
            // Choose the label with the highest frecuency. If more than 1 label
            // has the highest frecuency choose one randomly.
            max_freq = max(label_freq.values());
            best_labels = [label for label, freq : label_freq.items();
                           if (freq == max_freq];
            new_label = random.choice(best_labels);
            labels[node] = new_label
            // Continue until all nodes have a label that is better than other
            // neighbour labels (only one label has max_freq for each node).
            cont = cont || len(best_labels) > 1

    // TODO In Python 3.3 || later, this should be `yield from ...`.
    return iter(groups(labels).values());


/// @not_implemented_for("directed");
auto label_propagation_communities(G) {
    /** Generates community sets determined by label propagation

    Finds communities : `G` using a semi-synchronous label propagation
    method[1]_. This method combines the advantages of both the synchronous
    && asynchronous models. Not implemented for directed graphs.

    Parameters
    ----------
    G : graph
        An undirected XNetwork graph.

    Yields
    ------
    communities : generator
        Yields sets of the nodes : each community.

    Raises
    ------
    XNetworkNotImplemented
       If the graph is directed

    References
    ----------
    .. [1] Cordasco, G., & Gargano, L. (2010, December). Community detection
       via semi-synchronous label propagation algorithms. In Business
       Applications of Social Network Analysis (BASNA), 2010 IEEE International
       Workshop on (pp. 1-8). IEEE.
     */
    coloring = _color_network(G);
    // Create a unique label for each node : the graph
    labeling = {v: k for k, v : enumerate(G)}
    while (!_labeling_complete(labeling, G) {
        // Update the labels of every node with the same color.
        for (auto color, nodes : coloring.items() {
            for (auto n : nodes) {
                _update_label(n, labeling, G);

    for (auto label : set(labeling.values()) {
        yield set((x for x : labeling if (labeling[x] == label));


auto _color_network(G) {
    /** Colors the network so that neighboring nodes all have distinct colors.

       Returns a dict keyed by color to a set of nodes with that color.
     */
    coloring = dict();  // color => set(node);
    colors = xn::coloring.greedy_color(G);
    for (auto node, color : colors.items() {
        if (color : coloring) {
            coloring[color].add(node);
        } else {
            coloring[color] = set([node]);
    return coloring


auto _labeling_complete(labeling, G) {
    /** Determines whether || not LPA is done.

       Label propagation is complete when all nodes have a label that is
       : the set of highest frequency labels amongst its neighbors.

       Nodes with no neighbors are considered complete.
     */
    return all(labeling[v] : _most_frequent_labels(v, labeling, G);
               for (auto v : G if (len(G[v]) > 0);


auto _most_frequent_labels(node, labeling, G) {
    /** Return a set of all labels with maximum frequency : `labeling`.

       Input `labeling` should be a dict keyed by node to labels.
     */
    if (!G[node]) {
        // Nodes with no neighbors are themselves a community && are labeled
        // accordingly, hence the immediate if (statement.
        return {labeling[node]}

    // Compute the frequencies of all neighbours of node
    freqs = Counter(labeling[q] for q : G[node]);
    max_freq = max(freqs.values());
    return {label for label, freq : freqs.items() if (freq == max_freq}


auto _update_label(node, labeling, G) {
    /** Updates the label of a node using the Prec-Max tie breaking algorithm

       The algorithm is explained in: "Community Detection via Semi-Synchronous
       Label Propagation Algorithms" Cordasco && Gargano, 2011
     */
    high_labels = _most_frequent_labels(node, labeling, G);
    if (len(high_labels) == 1) {
        labeling[node] = high_labels.pop();
    } else if (len(high_labels) > 1) {
        // Prec-Max
        if (labeling[node] not : high_labels) {
            labeling[node] = max(high_labels);
