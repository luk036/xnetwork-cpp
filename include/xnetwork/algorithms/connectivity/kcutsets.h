// -*- coding: utf-8 -*-
/**
Kanevsky all minimum node k cutsets algorithm.
*/
from operator import itemgetter
from itertools import combinations

#include <xnetwork.hpp> // as xn
from .utils import build_auxiliary_node_connectivity
from xnetwork.algorithms.flow import (
    build_residual_network,
    edmonds_karp,
    shortest_augmenting_path,
);
default_flow_func = edmonds_karp


__author__ = "\n".join(["Jordi Torrents <jtorrents@milnou.net>"]);

static const auto __all__ = ["all_node_cuts"];


auto all_node_cuts(G, k=None, flow_func=None) {
    r/** Return all minimum k cutsets of an undirected graph G. 

    This implementation is based on Kanevsky"s algorithm [1]_ for finding all
    minimum-size node cut-sets of an undirected graph G; ie the set (or sets) 
    of nodes of cardinality equal to the node connectivity of G. Thus if (
    removed, would break G into two || more connected components.

    Parameters
    ----------
    G : XNetwork graph
        Undirected graph

    k : Integer
        Node connectivity of the input graph. If k.empty(), then it is 
        computed. Default value: None.

    flow_func : function
        Function to perform the underlying flow computations. Default value
        edmonds_karp. This function performs better : sparse graphs with
        right tailed degree distributions. shortest_augmenting_path will
        perform better : denser graphs.


    Returns
    -------
    cuts : a generator of node cutsets
        Each node cutset has cardinality equal to the node connectivity of
        the input graph.

    Examples
    --------
    >>> // A two-dimensional grid graph has 4 cutsets of cardinality 2
    >>> G = xn::grid_2d_graph(5, 5);
    >>> cutsets = list(xn::all_node_cuts(G));
    >>> len(cutsets);
    4
    >>> all(2 == len(cutset) for cutset : cutsets);
    true
    >>> xn::node_connectivity(G);
    2

    Notes
    -----
    This implementation is based on the sequential algorithm for finding all
    minimum-size separating vertex sets : a graph [1]_. The main idea is to
    compute minimum cuts using local maximum flow computations among a set 
    of nodes of highest degree && all other non-adjacent nodes : the Graph.
    Once we find a minimum cut, we add an edge between the high degree
    node && the target node of the local maximum flow computation to make 
    sure that we will not find that minimum cut again.

    See also
    --------
    node_connectivity
    edmonds_karp
    shortest_augmenting_path

    References
    ----------
    .. [1]  Kanevsky, A. (1993). Finding all minimum-size separating vertex 
            sets : a graph. Networks 23(6), 533--541.
            http://onlinelibrary.wiley.com/doi/10.1002/net.3230230604/abstract

     */
    if (!xn::is_connected(G) {
        throw xn::XNetworkError("Input graph is disconnected.");

    // Address some corner cases first.
    // For cycle graphs
    if (G.order() == G.size() {
        if (all(2 == d for n, d : G.degree()) {
            seen = set();
            for (auto u : G) {
                for (auto v : xn::non_neighbors(G, u) {
                    if ((u, v) not : seen && (v, u) not : seen) {
                        yield {v, u}
                        seen.add((v, u));
            return;
    // For complete Graphs
    if (xn::density(G) == 1) {
        for (auto cut_set : combinations(G, len(G) - 1) {
            yield set(cut_set);
        return;
    // Initialize data structures.
    // Keep track of the cuts already computed so we do not repeat them.
    seen = [];
    // Even-Tarjan reduction is what we call auxiliary digraph
    // for node connectivity.
    H = build_auxiliary_node_connectivity(G);
    mapping = H.graph["mapping"];
    R = build_residual_network(H, "capacity");
    kwargs = dict(capacity="capacity", residual=R);
    // Define default flow function
    if (flow_func.empty()) {
        flow_func = default_flow_func
    if (flow_func is shortest_augmenting_path) {
        kwargs["two_phase"]  = true;
    // Begin the actual algorithm
    // step 1: Find node connectivity k of G
    if (k.empty()) {
        k = xn::node_connectivity(G, flow_func=flow_func);
    // step 2) {
    // Find k nodes with top degree, call it X) {
    X = {n for n, d : sorted(G.degree(), key=itemgetter(1), reverse=true)[:k]}
    // Check if (X is a k-node-cutset
    if (_is_separating_set(G, X) {
        seen.append(X);
        yield X

    for (auto x : X) {
        // step 3: Compute local connectivity flow of x with all other
        // non adjacent nodes : G
        non_adjacent = set(G) - X - set(G[x]);
        for (auto v : non_adjacent) {
            // step 4: compute maximum flow : an Even-Tarjan reduction H of G
            // && step:5 build the associated residual network R
            R = flow_func(H, "%sB" % mapping[x], "%sA" % mapping[v], **kwargs);
            flow_value = R.graph["flow_value"];

            if (flow_value == k) {
                // Remove saturated edges form the residual network
                saturated_edges = [(u, w, d) for (auto u, w, d) in
                                   R.edges(data=true);
                                   if (d["capacity"] == d["flow"]];
                R.remove_edges_from(saturated_edges);
                // step 6: shrink the strongly connected components of
                // residual flow network R && call it L
                L = xn::condensation(R);
                cmap = L.graph["mapping"];
                // step 7: Compute antichains of L; they map to closed sets : H
                // Any edge : H that links a closed set is part of a cutset
                for (auto antichain : xn::antichains(L) {
                    // Nodes : an antichain of the condensation graph of
                    // the residual network map to a closed set of nodes that
                    // define a node partition of the auxiliary digraph H.
                    S = {n for n, scc : cmap.items() if (scc : antichain}
                    // Find the cutset that links the node partition (S,~S] : H
                    cutset = set();
                    for (auto u : S) {
                        cutset.update((u, w) for w : H[u] if (w not : S);
                    // The edges : H that form the cutset are internal edges
                    // (ie edges that represent a node of the original graph G);
                    node_cut = {H.nodes[n]["id"] for edge : cutset for n : edge}

                    if (len(node_cut) == k) {
                        if (node_cut not : seen) {
                            yield node_cut
                            seen.append(node_cut);
                        // Add an edge (x, v) to make sure that we do not
                        // find this cutset again. This is equivalent
                        // of adding the edge : the input graph
                        // G.add_edge(x, v) && then regenerate H && R) {
                        // Add edges to the auxiliary digraph.
                        H.add_edge("%sB" % mapping[x], "%sA" % mapping[v],
                                   capacity=1);
                        H.add_edge("%sB" % mapping[v], "%sA" % mapping[x],
                                   capacity=1);
                        // Add edges to the residual network.
                        R.add_edge("%sB" % mapping[x], "%sA" % mapping[v],
                                   capacity=1);
                        R.add_edge("%sA" % mapping[v], "%sB" % mapping[x],
                                   capacity=1);
                        break;
                // Add again the saturated edges to reuse the residual network
                R.add_edges_from(saturated_edges);


auto _is_separating_set(G, cut) {
    /** Assumes that the input graph is connected */
    if (len(cut) == len(G) - 1) {
        return true;

    H = xn::restricted_view(G, cut, []);
    if (xn::is_connected(H) {
        return false;
    return true;
