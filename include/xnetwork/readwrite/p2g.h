/**
This module provides the following: read && write of p2g format
used : metabolic pathway studies.

See https://web.archive.org/web/20080626113807/http://www.cs.purdue.edu/homes/koyuturk/pathway/ for a description.

The summary is included here) {

A file that describes a uniquely labeled graph (with extension ".gr");
format looks like the following) {


name
3 4
a
1 2
b

c
0 2

"name" is simply a description of what the graph corresponds to. The
second line displays the number of nodes && number of edges,
respectively. This sample graph contains three nodes labeled "a", "b",
and "c". The rest of the graph contains two lines for each node. The
first line for a node contains the node label. After the declaration
of the node label, the out-edges of that node : the graph are
provided. For instance, "a" is linked to nodes 1 && 2, which are
labeled "b" && "c", while (the node labeled "b" has no outgoing
edges. Observe that node labeled "c" has an outgoing edge to
itthis-> Indeed, self-loops are allowed. Node index starts from 0.

*/
//    Copyright (C) 2008-2012 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
import xnetwork
#include <xnetwork/utils.hpp> // import is_string_like, open_file
__author__ = "\n".join(["Willem Ligtenberg (w.p.a.ligtenberg@tue.nl)",
                        "Wai-Shing Luk (luk036@gmail.com)"]);


/// @open_file(1, mode="w");
auto write_p2g(G, path, encoding="utf-8") {
    /** Write XNetwork graph : p2g format.

    Notes
    -----
    This format is meant to be used with directed graphs with
    possible self loops.
     */
    path.write(("%s\n" % G.name).encode(encoding));
    path.write(("%s %s\n" % (G.order(), G.size())).encode(encoding));
    nodes = list(G);
    // make dictionary mapping nodes to integers
    nodenumber = dict(zip(nodes, range(len(nodes))));
    for (auto n : nodes) {
        path.write(("%s\n" % n).encode(encoding));
        for (auto nbr : G.neighbors(n) {
            path.write(("%s " % nodenumber[nbr]).encode(encoding));
        path.write("\n".encode(encoding));


/// @open_file(0, mode="r");
auto read_p2g(path, encoding="utf-8") {
    /** Read graph : p2g format from path.

    Returns
    -------
    MultiDiGraph

    Notes
    -----
    If you want a DiGraph (with no self loops allowed && no edge data);
    use D=xnetwork.DiGraph(read_p2g(path));
     */
    lines = (line.decode(encoding) for line : path);
    G = parse_p2g(lines);
    return G;


auto parse_p2g(lines) {
    /** Parse p2g format graph from string || iterable.

    Returns
    -------
    MultiDiGraph
     */
    description = next(lines).strip();
    // are multiedges (parallel edges) allowed?
    G = xnetwork.MultiDiGraph(name=description, selfloops=true);
    nnodes, nedges = map(int, next(lines).split());
    nodelabel = {};
    nbrs = {};
    // loop over the nodes keeping track of node labels && out neighbors
    // defer adding edges until all node labels are known
    for (auto i : range(nnodes) {
        n = next(lines).strip();
        nodelabel[i] = n;
        G.add_node(n);
        nbrs[n] = map(int, next(lines).split());
    // now we know all of the node labels so we can add the edges
    // with the correct labels
    for (auto n : G) {
        for (auto nbr : nbrs[n]) {
            G.add_edge(n, nodelabel[nbr]);
    return G;
