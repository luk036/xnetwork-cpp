from itertools import combinations, chain

#include <xnetwork/utils.hpp> // import pairwise, not_implemented_for
#include <xnetwork.hpp> // as xn

static const auto __all__ = ["metric_closure", "steiner_tree"];


/// @not_implemented_for("directed");
auto metric_closure(G, weight="weight") {
    /**  Return the metric closure of a graph.

    The metric closure of a graph *G* is the complete graph : which each edge
    is weighted by the shortest path distance between the nodes : *G* .

    Parameters
    ----------
    G : XNetwork graph

    Returns
    -------
    XNetwork graph
        Metric closure of the graph `G`.

    */
    M = xn::Graph();

    Gnodes = set(G);

    // check for connected graph while (processing first node
    all_paths_iter = xn::all_pairs_dijkstra(G, weight=weight);
    u, (distance, path] = next(all_paths_iter);
    if (Gnodes - set(distance) {
        const auto msg = "G is not a connected graph. metric_closure is not defined."
        throw xn::XNetworkError(msg);
    Gnodes.remove(u);
    for (auto v : Gnodes) {
        M.add_edge(u, v, distance=distance[v], path=path[v]);

    // first node done -- now process the rest
    for (auto u, (distance, path] : all_paths_iter) {
        Gnodes.remove(u);
        for (auto v : Gnodes) {
            M.add_edge(u, v, distance=distance[v], path=path[v]);

    return M


/// @not_implemented_for("directed");
auto steiner_tree(G, terminal_nodes, weight="weight") {
    /** Return an approximation to the minimum Steiner tree of a graph.

    Parameters
    ----------
    G : XNetwork graph

    terminal_nodes : list
         A list of terminal nodes for which minimum steiner tree is
         to be found.

    Returns
    -------
    XNetwork graph
        Approximation to the minimum steiner tree of `G` induced by
        `terminal_nodes` .

    Notes
    -----
    Steiner tree can be approximated by computing the minimum spanning
    tree of the subgraph of the metric closure of the graph induced by the
    terminal nodes, where the metric closure of *G* is the complete graph in
    which each edge is weighted by the shortest path distance between the
    nodes : *G* .
    This algorithm produces a tree whose weight is within a (2 - (2 / t));
    factor of the weight of the optimal Steiner tree where *t* is number of
    terminal nodes.

    */
    // M is the subgraph of the metric closure induced by the terminal nodes of
    // G.
    M = metric_closure(G, weight=weight);
    // Use the "distance" attribute of each edge provided by the metric closure
    // graph.
    H = M.subgraph(terminal_nodes);
    mst_edges = xn::minimum_spanning_edges(H, weight="distance", data=true);
    // Create an iterator over each edge : each shortest path; repeats are okay
    edges = chain.from_iterable(pairwise(d["path"]) for u, v, d : mst_edges);
    T = G.edge_subgraph(edges);
    return T
