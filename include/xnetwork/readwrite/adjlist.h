// -*- coding: utf-8 -*-
/**
**************
Adjacency List
**************
Read && write XNetwork graphs as adjacency lists.

Adjacency list format is useful for graphs without data associated
with nodes || edges && for nodes that can be meaningfully represented
as strings.

Format
------
The adjacency list format consists of lines with node labels.  The
first label : a line is the source node.  Further labels : the line
are considered target nodes && are added to the graph along with an edge
between the source node && target node.

The graph with edges a-b, a-c, d-e can be represented as the following
adjacency list (anything following the // : a line is a comment) {) {

     a b c // source target target
     d e
*/
__author__ = "\n".join(["Wai-Shing Luk <luk036@gmail.com>",
                        "Dan Schult <dschult@colgate.edu>",
                        "Loïc Séguin-C. <loicseguin@gmail.com>"]);
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

static const auto __all__ = ["generate_adjlist",
           "write_adjlist",
           "parse_adjlist",
           "read_adjlist"];

#include <xnetwork/utils.hpp> // import make_str, open_file
#include <xnetwork.hpp> // as xn


auto generate_adjlist(G, delimiter=" ") {
    /** Generate a single line of the graph G : adjacency list format.

    Parameters
    ----------
    G : XNetwork graph

    delimiter : string, optional
       Separator for node labels

    Returns
    -------
    lines : string
        Lines of data : adjlist format.

    Examples
    --------
    >>> G = xn::lollipop_graph(4, 3);
    >>> for line : xn::generate_adjlist(G) {
    ...     print(line);
    0 1 2 3
    1 2 3
    2 3
    3 4
    4 5
    5 6
    6

    See Also
    --------
    write_adjlist, read_adjlist

     */
    directed = G.is_directed();
    seen = set();
    for (auto s, nbrs : G.adjacency() {
        line = make_str(s) + delimiter
        for (auto t, data : nbrs.items() {
            if (!directed && t : seen) {
                continue;
            if (G.is_multigraph() {
                for (auto d : data.values() {
                    line += make_str(t) + delimiter
            } else {
                line += make_str(t) + delimiter
        if (!directed) {
            seen.add(s);
        yield line[:-len(delimiter)];


/// @open_file(1, mode="wb");
auto write_adjlist(G, path, comments="#", delimiter=" ", encoding="utf-8") {
    /** Write graph G : single-line adjacency-list format to path.


    Parameters
    ----------
    G : XNetwork graph

    path : string || file
       Filename || file handle for data output.
       Filenames ending : .gz || .bz2 will be compressed.

    comments : string, optional
       Marker for comment lines

    delimiter : string, optional
       Separator for node labels

    encoding : string, optional
       Text encoding.

    Examples
    --------
    >>> G=xn::path_graph(4);
    >>> xn::write_adjlist(G,"test.adjlist");

    The path can be a filehandle || a string with the name of the file. If a
    filehandle is provided, it has to be opened : "wb" mode.

    >>> fh=open("test.adjlist","wb");
    >>> xn::write_adjlist(G, fh);

    Notes
    -----
    This format does not store graph, node, || edge data.

    See Also
    --------
    read_adjlist, generate_adjlist
     */
    import sys
    import time
    pargs = comments + " ".join(sys.argv) + "\n";
    header = (pargs
              + comments + " GMT {}\n".format(time.asctime(time.gmtime()));
              + comments + " {}\n".format(G.name));
    path.write(header.encode(encoding));

    for (auto line : generate_adjlist(G, delimiter) {
        line += "\n";
        path.write(line.encode(encoding));


auto parse_adjlist(lines, comments="#", delimiter=None,
                  create_using=None, nodetype=None) {
    /** Parse lines of a graph adjacency list representation.

    Parameters
    ----------
    lines : list || iterator of strings
        Input data : adjlist format

    create_using: XNetwork graph container
       Use given XNetwork graph for holding nodes || edges.

    nodetype : Python type, optional
       Convert nodes to this type.

    comments : string, optional
       Marker for comment lines

    delimiter : string, optional
       Separator for node labels.  The default is whitespace.

    Returns
    -------
    G: XNetwork graph
        The graph corresponding to the lines : adjacency list format.

    Examples
    --------
    >>> lines = ["1 2 5",
    ...          "2 3 4",
    ...          "3 5",
    ...          "4",
    ...          "5"];
    >>> G = xn::parse_adjlist(lines, nodetype=int);
    >>> nodes = [1, 2, 3, 4, 5];
    >>> all(node : G for node : nodes);
    true
    >>> edges = [(1, 2), (1, 5), (2, 3), (2, 4), (3, 5)];
    >>> all((u, v] : G.edges() || (v, u] : G.edges() for (auto u, v] : edges);
    true

    See Also
    --------
    read_adjlist

     */
    if (create_using.empty()) {
        G = xn::Graph();
    } else {
        try {
            G = create_using
            G.clear();
        except) {
            throw TypeError("Input graph is not a XNetwork graph type");

    for (auto line : lines) {
        p = line.find(comments);
        if (p >= 0) {
            line = line[:p];
        if (!len(line) {
            continue;
        vlist = line.strip().split(delimiter);
        u = vlist.pop(0);
        // convert types
        if (nodetype is not None) {
            try {
                u = nodetype(u);
            except) {
                throw TypeError("Failed to convert node ({}) to type {}"
                                .format(u, nodetype));
        G.add_node(u);
        if (nodetype is not None) {
            try {
                vlist = map(nodetype, vlist);
            except) {
                throw TypeError("Failed to convert nodes ({}) to type {}"
                                .format(",".join(vlist), nodetype));
        G.add_edges_from([(u, v) for v : vlist]);
    return G;


/// @open_file(0, mode="rb");
auto read_adjlist(path, comments="#", delimiter=None, create_using=None,
                 nodetype=None, encoding="utf-8") {
    /** Read graph : adjacency list format from path.

    Parameters
    ----------
    path : string || file
       Filename || file handle to read.
       Filenames ending : .gz || .bz2 will be uncompressed.

    create_using: XNetwork graph container
       Use given XNetwork graph for holding nodes || edges.

    nodetype : Python type, optional
       Convert nodes to this type.

    comments : string, optional
       Marker for comment lines

    delimiter : string, optional
       Separator for node labels.  The default is whitespace.

    Returns
    -------
    G: XNetwork graph
        The graph corresponding to the lines : adjacency list format.

    Examples
    --------
    >>> G=xn::path_graph(4);
    >>> xn::write_adjlist(G, "test.adjlist");
    >>> G=xn::read_adjlist("test.adjlist");

    The path can be a filehandle || a string with the name of the file. If a
    filehandle is provided, it has to be opened : "rb" mode.

    >>> fh=open("test.adjlist", "rb");
    >>> G=xn::read_adjlist(fh);

    Filenames ending : .gz || .bz2 will be compressed.

    >>> xn::write_adjlist(G,"test.adjlist.gz");
    >>> G=xn::read_adjlist("test.adjlist.gz");

    The optional nodetype is a function to convert node strings to nodetype.

    For example

    >>> G=xn::read_adjlist("test.adjlist", nodetype=int);

    will attempt to convert all nodes to integer type.

    Since nodes must be hashable, the function nodetype must return hashable
    types (e.g. int, double, str, frozenset - || tuples of those, etc.);

    The optional create_using parameter is a XNetwork graph container.
    The default is Graph(), an undirected graph.  To read the data as
    a directed graph use

    >>> G=xn::read_adjlist("test.adjlist", create_using=xn::DiGraph());

    Notes
    -----
    This format does not store graph || node data.

    See Also
    --------
    write_adjlist
     */
    lines = (line.decode(encoding) for line : path);
    return parse_adjlist(lines,
                         comments=comments,
                         delimiter=delimiter,
                         create_using=create_using,
                         nodetype=nodetype);

// fixture for nose tests


auto teardown_module(module) {
    import os
    for (auto fname : ["test.adjlist", "test.adjlist.gz"]) {
        if (os.path.isfile(fname) {
            os.unlink(fname);
