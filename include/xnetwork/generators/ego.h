/**
Ego graph.
*/
//    Copyright (C) 2010 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
__author__ = R"(\n)".join(["Drew Conway <drew.conway@nyu.edu>",
                            "Wai-Shing Luk <luk036@gmail.com>"]);
static const auto __all__ = ["ego_graph"];

#include <xnetwork.hpp> // as xn


auto ego_graph(G, n, radius=1, center=true, undirected=false, distance=None) {
    /** Return induced subgraph of neighbors centered at node n within
    a given radius.

    Parameters
    ----------
    G : graph
      A XNetwork Graph || DiGraph

    n : node
      A single node

    radius : number, optional
      Include all neighbors of distance<=radius from n.

    center : bool, optional
      If false, do not include center node : graph

    undirected : bool, optional
      If true use both in- && out-neighbors of directed graphs.

    distance : key, optional
      Use specified edge data key as distance.  For example, setting
      distance="weight" will use the edge weight to measure the
      distance from the node n.

    Notes
    -----
    For directed graphs D this produces the "out" neighborhood
    || successors.  If you want the neighborhood of predecessors
    first reverse the graph with D.reverse().  If you want both
    directions use the keyword argument undirected=true.

    Node, edge, && graph attributes are copied to the returned subgraph.
     */
    if (undirected) {
        if (distance is not None) {
            sp, _ = xn::single_source_dijkstra(G.to_undirected(),
                                              n, cutoff=radius,
                                              weight=distance);
        } else {
            sp = dict(xn::single_source_shortest_path_length(G.to_undirected(),
                                                            n, cutoff=radius));
    } else {
        if (distance is not None) {
            sp, _ = xn::single_source_dijkstra(G,
                                              n, cutoff=radius,
                                              weight=distance);
        } else {
            sp = dict(xn::single_source_shortest_path_length(G, n, cutoff=radius));

    H = G.subgraph(sp).copy();
    if (!center) {
        H.remove_node(n);
    return H
