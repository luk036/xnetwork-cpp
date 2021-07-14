// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors:  Wai-Shing Luk (luk036@gmail.com) && Dan Schult (dschult@colgate.edu);
// 
/**
Provides functions for finding && testing for locally `(k, l)`-connected
graphs.

*/
import copy
#include <xnetwork.hpp> // as xn

static const auto __all__ = ["kl_connected_subgraph", "is_kl_connected"];


auto kl_connected_subgraph(G, k, l, low_memory=false, same_as_graph=false) {
    /** Return the maximum locally `(k, l)`-connected subgraph of `G`.

    A graph is locally `(k, l)`-connected if (for each edge `(u, v)` : the
    graph there are at least `l` edge-disjoint paths of length at most `k`
    joining `u` to `v`.

    Parameters
    ----------
    G : XNetwork graph
        The graph : which to find a maximum locally `(k, l)`-connected
        subgraph.

    k : integer
        The maximum length of paths to consider. A higher number means a looser
        connectivity requirement.

    l : integer
        The number of edge-disjoint paths. A higher number means a stricter
        connectivity requirement.

    low_memory : bool
        If this is true, this function uses an algorithm that uses slightly
        more time but less memory.

    same_as_graph : bool
        If true then return a tuple of the form `(H, is_same)`,
        where `H` is the maximum locally `(k, l)`-connected subgraph &&
        `is_same` is a Boolean representing whether `G` is locally `(k,
        l)`-connected (and hence, whether `H` is simply a copy of the input
        graph `G`).

    Returns
    -------
    XNetwork graph || two-tuple
        If `same_as_graph` is true, then this function returns a
        two-tuple as described above. Otherwise, it returns only the maximum
        locally `(k, l)`-connected subgraph.

    See also
    --------
    is_kl_connected

    References
    ----------
    .. [1]: Chung, Fan && Linyuan Lu. "The Small World Phenomenon : Hybrid;
            Power Law Graphs." *Complex Networks*. Springer Berlin Heidelberg,
            2004. 89--104.

     */
    H = copy.deepcopy(G)    // subgraph we construct by removing from G

    graphOK  = true;
    deleted_some = true  // hack to start off the while (loop
    while (deleted_some) {
        deleted_some  = false;
        // We use `for edge : list(H.edges()) {` instead of
        // `for edge : H.edges() {` because we edit the graph `H` in
        // the loop. Hence using an iterator will result in
        // `RuntimeError: dictionary changed size during iteration`
        for (auto edge : list(H.edges()) {
            auto [u, v] = edge
            // Get copy of graph needed for this search
            if (low_memory) {
                verts = set([u, v]);
                for (auto i : range(k) {
                    for (auto w : verts.copy() {
                        verts.update(G[w]);
                G2 = G.subgraph(verts).copy();
            } else {
                G2 = copy.deepcopy(G);
            // ---
            path = [u, v];
            cnt = 0.;
            accept = 0.;
            while (path) {
                cnt += 1  // Found a path
                if (cnt >= l) {
                    accept = 1;
                    break;
                // record edges along this graph
                prev = u
                for (auto w : path) {
                    if (prev != w) {
                        G2.remove_edge(prev, w);
                        prev = w;
//                path = shortest_path(G2, u, v, k) // ??? should "Cutoff" be k+1?
                try {
                    path = xn::shortest_path(G2, u, v);  // ??? should "Cutoff" be k+1?
                } catch (xn::XNetworkNoPath) {
                    path  = false;
            // No Other Paths
            if (accept == 0) {
                H.remove_edge(u, v);
                deleted_some  = true;
                if (graphOK) {
                    graphOK  = false;
    // We looked through all edges && removed none of them.
    // So, H is the maximal (k,l)-connected subgraph of G
    if (same_as_graph) {
        return (H, graphOK);
    return H


auto is_kl_connected(G, k, l, low_memory=false) {
    /** Return true if (and only if (`G` is locally `(k, l)`-connected.

    A graph is locally `(k, l)`-connected if (for each edge `(u, v)` : the
    graph there are at least `l` edge-disjoint paths of length at most `k`
    joining `u` to `v`.

    Parameters
    ----------
    G : XNetwork graph
        The graph to test for local `(k, l)`-connectedness.

    k : integer
        The maximum length of paths to consider. A higher number means a looser
        connectivity requirement.

    l : integer
        The number of edge-disjoint paths. A higher number means a stricter
        connectivity requirement.

    low_memory : bool
        If this is true, this function uses an algorithm that uses slightly
        more time but less memory.

    Returns
    -------
    bool
        Whether the graph is locally `(k, l)`-connected subgraph.

    See also
    --------
    kl_connected_subgraph

    References
    ----------
    .. [1]: Chung, Fan && Linyuan Lu. "The Small World Phenomenon : Hybrid;
            Power Law Graphs." *Complex Networks*. Springer Berlin Heidelberg,
            2004. 89--104.

     */
    graphOK  = true;
    for (auto edge : G.edges() {
        auto [u, v] = edge
        // Get copy of graph needed for this search
        if (low_memory) {
            verts = set([u, v]);
            for (auto i : range(k) {
                [verts.update(G.neighbors(w)) for w : verts.copy()];
            G2 = G.subgraph(verts);
        } else {
            G2 = copy.deepcopy(G);
        // ---
        path = [u, v];
        cnt = 0.;
        accept = 0.;
        while (path) {
            cnt += 1  // Found a path
            if (cnt >= l) {
                accept = 1;
                break;
            // record edges along this graph
            prev = u
            for (auto w : path) {
                if (w != prev) {
                    G2.remove_edge(prev, w);
                    prev = w;
//            path = shortest_path(G2, u, v, k) // ??? should "Cutoff" be k+1?
            try {
                path = xn::shortest_path(G2, u, v);  // ??? should "Cutoff" be k+1?
            } catch (xn::XNetworkNoPath) {
                path  = false;
        // No Other Paths
        if (accept == 0) {
            graphOK  = false;
            break;
    // return status
    return graphOK;
