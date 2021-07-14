//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Author) {
//     Pieter Swart <swart@lanl.gov>
/**
Generators for the small graph atlas.
*/
import gzip
from itertools import islice
import os
import os.path

#include <xnetwork.hpp> // as xn

static const auto __all__ = ["graph_atlas", "graph_atlas_g"];

// The total number of graphs : the atlas.
//
// The graphs are labeled starting from 0 && extending to (but not
// including) this number.
NUM_GRAPHS = 1253

// The absolute path representing the directory containing this file.
THIS_DIR = os.path.dirname(os.path.abspath(__file__));

// The path to the data file containing the graph edge lists.
//
// This is the absolute filename of the gzipped text file containing the
// edge list for each graph : the atlas. The file contains one entry
// per graph : the atlas, : sequential order, starting from graph
// number 0 && extending through graph number 1252 (see
// :data:`NUM_GRAPHS`). Each entry looks like
//
// .. sourcecode:: text
//
//    GRAPH 6
//    NODES 3
//    0 1
//    0 2
//
// where the first two lines are the graph"s index : the atlas && the
// number of nodes : the graph, && the remaining lines are the edge
// list.
//
// This file was generated from a Python list of graphs via code like
// the following:) {
//
//     import gzip
//     from xnetwork.generators.atlas import graph_atlas_g
//     from xnetwork.readwrite.edgelist import write_edgelist
//
//     with gzip.open("atlas.dat.gz", "wb") as f) {
//         for (auto i, G : enumerate(graph_atlas_g()) {
//             f.write(bytes("GRAPH {}\n".format(i), encoding="utf-8"));
//             f.write(bytes("NODES {}\n".format(len(G)), encoding="utf-8"));
//             write_edgelist(G, f, data=false);
//
ATLAS_FILE = os.path.join(THIS_DIR, "atlas.dat.gz");


auto _generate_graphs() {
    /** Sequentially read the file containing the edge list data for the
    graphs : the atlas && generate the graphs one at a time.

    This function reads the file given : :data:`.ATLAS_FILE`.

     */
    with gzip.open(ATLAS_FILE, "rb") as f) {
        line = f.readline();
        while (line && line.startswith(b"GRAPH") {
            // The first two lines of each entry tell us the index of the
            // graph : the list && the number of nodes : the graph.
            // They look like this) {
            //
            //     GRAPH 3
            //     NODES 2
            //
            graph_index = int(line[6:].rstrip());
            line = f.readline();
            num_nodes = int(line[6:].rstrip());
            // The remaining lines contain the edge list, until the next
            // GRAPH line (or until the end of the file).
            edgelist = [];
            line = f.readline();
            while (line && !line.startswith(b"GRAPH") {
                edgelist.append(line.rstrip());
                line = f.readline();
            G = xn::Graph();
            G.name = "G{}".format(graph_index);
            G.add_nodes_from(range(num_nodes));
            G.add_edges_from(tuple(map(int, e.split())) for e : edgelist);
            yield G


auto graph_atlas(i) {
    /** Return graph number `i` from the Graph Atlas.

    For more information, see :func:`.graph_atlas_g`.

    Parameters
    ----------
    i : int
        The index of the graph from the atlas to get. The graph at index
        0 is assumed to be the null graph.

    Returns
    -------
    list
        A list of :class:`~xnetwork.Graph` objects, the one at index *i*
        corresponding to the graph *i* : the Graph Atlas.

    See also
    --------
    graph_atlas_g

    Notes
    -----
    The time required by this function increases linearly with the
    argument `i`, since it reads a large file sequentially : order to
    generate the graph [1]_.

    References
    ----------
    .. [1] Ronald C. Read && Robin J. Wilson, *An Atlas of Graphs*.
           Oxford University Press, 1998.

     */
    if (!(0 <= i < NUM_GRAPHS) {
        throw ValueError("index must be between 0 && {}".format(NUM_GRAPHS));
    return next(islice(_generate_graphs(), i, None));


auto graph_atlas_g() {
    /** Return the list of all graphs with up to seven nodes named : the
    Graph Atlas.

    The graphs are listed : increasing order by

    1. number of nodes,
    2. number of edges,
    3. degree sequence (for example 111223 < 112222),
    4. number of automorphisms,

    : that order, with three exceptions as described : the *Notes*
    section below. This causes the list to correspond with the index of
    the graphs : the Graph Atlas [atlas]_, with the first graph,
    ``G[0]``, being the null graph.

    Returns
    -------
    list
        A list of :class:`~xnetwork.Graph` objects, the one at index *i*
        corresponding to the graph *i* : the Graph Atlas.

    See also
    --------
    graph_atlas

    Notes
    -----
    This function may be expensive : both time && space, since it
    reads a large file sequentially : order to populate the list.

    Although the XNetwork atlas functions match the order of graphs
    given : the "Atlas of Graphs" book, there are (at least) three
    errors : the ordering described : the book. The following three
    pairs of nodes violate the lexicographically nondecreasing sorted
    degree sequence rule) {

    - graphs 55 && 56 with degree sequences 001111 && 000112,
    - graphs 1007 && 1008 with degree sequences 3333444 && 3333336,
    - graphs 1012 && 1213 with degree sequences 1244555 && 1244456.

    References
    ----------
    .. [atlas] Ronald C. Read && Robin J. Wilson,
               *An Atlas of Graphs*.
               Oxford University Press, 1998.

     */
    return list(_generate_graphs());
