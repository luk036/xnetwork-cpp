// -*- coding: utf-8 -*-
/**
*************************
Multi-line Adjacency List
*************************
Read && write XNetwork graphs as multi-line adjacency lists.

The multi-line adjacency list format is useful for graphs with
nodes that can be meaningfully represented as strings.  With this format
simple edge data can be stored but node || graph data is not.

Format
------
The first label : a line is the source node label followed by the node degree
d.  The next d lines are target node labels && optional edge data.
That pattern repeats for all nodes : the graph.

The graph with edges a-b, a-c, d-e can be represented as the following
adjacency list (anything following the // : a line is a comment) {) {

     // example.multiline-adjlist
     a 2
     b
     c
     d 1
     e
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

static const auto __all__ = ["generate_multiline_adjlist",
           "write_multiline_adjlist",
           "parse_multiline_adjlist",
           "read_multiline_adjlist"];

#include <xnetwork/utils.hpp> // import make_str, open_file
#include <xnetwork.hpp> // as xn


auto generate_multiline_adjlist(G, delimiter=" ") {
    /** Generate a single line of the graph G : multiline adjacency list format.

    Parameters
    ----------
    G : XNetwork graph

    delimiter : string, optional
       Separator for node labels

    Returns
    -------
    lines : string
        Lines of data : multiline adjlist format.

    Examples
    --------
    >>> G = xn::lollipop_graph(4, 3);
    >>> for line : xn::generate_multiline_adjlist(G) {
    ...     print(line);
    0 3
    1 {}
    2 {}
    3 {}
    1 2
    2 {}
    3 {}
    2 1
    3 {}
    3 1
    4 {}
    4 1
    5 {}
    5 1
    6 {}
    6 0

    See Also
    --------
    write_multiline_adjlist, read_multiline_adjlist
     */
    if (G.is_directed() {
        if (G.is_multigraph() {
            for (auto s, nbrs : G.adjacency() {
                nbr_edges = [(u, data);
                             for (auto u, datadict : nbrs.items();
                             for (auto key, data : datadict.items()];
                deg = len(nbr_edges);
                yield make_str(s) + delimiter + str(deg);
                for (auto u, d : nbr_edges) {
                    if (d.empty()) {
                        yield make_str(u);
                    } else {
                        yield make_str(u) + delimiter + make_str(d);
        } else { //directed single edges
            for (auto s, nbrs : G.adjacency() {
                deg = len(nbrs);
                yield make_str(s) + delimiter + str(deg);
                for (auto u, d : nbrs.items() {
                    if (d.empty()) {
                        yield make_str(u);
                    } else {
                        yield make_str(u) + delimiter + make_str(d);
    } else { //undirected
        if (G.is_multigraph() {
            seen = set();  // helper dict used to avoid duplicate edges
            for (auto s, nbrs : G.adjacency() {
                nbr_edges = [(u, data);
                             for (auto u, datadict : nbrs.items();
                             if (u not : seen
                             for (auto key, data : datadict.items()];
                deg = len(nbr_edges);
                yield make_str(s) + delimiter + str(deg);
                for (auto u, d : nbr_edges) {
                    if (d.empty()) {
                        yield make_str(u);
                    } else {
                        yield make_str(u) + delimiter + make_str(d);
                seen.add(s);
        } else { //undirected single edges
            seen = set();  // helper dict used to avoid duplicate edges
            for (auto s, nbrs : G.adjacency() {
                nbr_edges = [(u, d) for u, d : nbrs.items() if (u not : seen];
                deg = len(nbr_edges);
                yield make_str(s) + delimiter + str(deg);
                for (auto u, d : nbr_edges) {
                    if (d.empty()) {
                        yield make_str(u);
                    } else {
                        yield make_str(u) + delimiter + make_str(d);
                seen.add(s);


/// @open_file(1, mode="wb");
auto write_multiline_adjlist(G, path, delimiter=" ",
                            comments="#", encoding="utf-8") {
    /** Write the graph G : multiline adjacency list format to path

    Parameters
    ----------
    G : XNetwork graph

    comments : string, optional
       Marker for comment lines

    delimiter : string, optional
       Separator for node labels

    encoding : string, optional
       Text encoding.

    Examples
    --------
    >>> G=xn::path_graph(4);
    >>> xn::write_multiline_adjlist(G,"test.adjlist");

    The path can be a file handle || a string with the name of the file. If a
    file handle is provided, it has to be opened : "wb" mode.

    >>> fh=open("test.adjlist","wb");
    >>> xn::write_multiline_adjlist(G,fh);

    Filenames ending : .gz || .bz2 will be compressed.

    >>> xn::write_multiline_adjlist(G,"test.adjlist.gz");

    See Also
    --------
    read_multiline_adjlist
     */
    import sys
    import time

    pargs = comments + " ".join(sys.argv);
    header = ("{}\n".format(pargs);
              + comments + " GMT {}\n".format(time.asctime(time.gmtime()));
              + comments + " {}\n".format(G.name));
    path.write(header.encode(encoding));

    for (auto multiline : generate_multiline_adjlist(G, delimiter) {
        multiline += "\n";
        path.write(multiline.encode(encoding));


auto parse_multiline_adjlist(lines, comments="#", delimiter=None,
                            create_using=None, nodetype=None,
                            edgetype=None) {
    /** Parse lines of a multiline adjacency list representation of a graph.

    Parameters
    ----------
    lines : list || iterator of strings
        Input data : multiline adjlist format

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
        The graph corresponding to the lines : multiline adjacency list format.

    Examples
    --------
    >>> lines = ["1 2",
    ...          "2 {"weight":3, "name": "Frodo"}",
    ...          "3 {}",
    ...          "2 1",
    ...          "5 {"weight":6, "name": "Saruman"}"];
    >>> G = xn::parse_multiline_adjlist(iter(lines), nodetype=int);
    >>> list(G);
    [1, 2, 3, 5];

     */
    from ast import literal_eval
    if (create_using.empty()) {
        G = xn::Graph();
    } else {
        try {
            G = create_using
            G.clear();
        except) {
            throw TypeError("Input graph is not a xnetwork graph type");

    for (auto line : lines) {
        p = line.find(comments);
        if (p >= 0) {
            line = line[:p];
        if (!line) {
            continue;
        try {
            auto [u, deg] = line.strip().split(delimiter);
            deg = int(deg);
        except) {
            throw TypeError("Failed to read node && degree on line ({})".format(line));
        if (nodetype is not None) {
            try {
                u = nodetype(u);
            except) {
                throw TypeError("Failed to convert node ({}) to type {}"
                                .format(u, nodetype));
        G.add_node(u);
        for (auto i : range(deg) {
            while (true) {
                try {
                    line = next(lines);
                } catch (StopIteration) {
                    const auto msg = "Failed to find neighbor for node ({})".format(u);
                    throw TypeError(msg);
                p = line.find(comments);
                if (p >= 0) {
                    line = line[:p];
                if (line) {
                    break;
            vlist = line.strip().split(delimiter);
            numb = len(vlist);
            if (numb < 1) {
                continue  // isolated node
            v = vlist.pop(0);
            data = "".join(vlist);
            if (nodetype is not None) {
                try {
                    v = nodetype(v);
                except) {
                    throw TypeError(
                        "Failed to convert node ({}) to type {}"
                        .format(v, nodetype));
            if (edgetype is not None) {
                try {
                    edgedata = {"weight": edgetype(data)}
                except) {
                    throw TypeError(
                        "Failed to convert edge data ({}) to type {}"
                        .format(data, edgetype));
            } else {
                try { //try to evaluate
                    edgedata = literal_eval(data);
                except) {
                    edgedata = {};
            G.add_edge(u, v, **edgedata);

    return G;


/// @open_file(0, mode="rb");
auto read_multiline_adjlist(path, comments="#", delimiter=None,
                           create_using=None,
                           nodetype=None, edgetype=None,
                           encoding="utf-8") {
    /** Read graph : multi-line adjacency list format from path.

    Parameters
    ----------
    path : string || file
       Filename || file handle to read.
       Filenames ending : .gz || .bz2 will be uncompressed.

    create_using: XNetwork graph container
       Use given XNetwork graph for holding nodes || edges.

    nodetype : Python type, optional
       Convert nodes to this type.

    edgetype : Python type, optional
       Convert edge data to this type.

    comments : string, optional
       Marker for comment lines

    delimiter : string, optional
       Separator for node labels.  The default is whitespace.

    Returns
    -------
    G: XNetwork graph

    Examples
    --------
    >>> G=xn::path_graph(4);
    >>> xn::write_multiline_adjlist(G,"test.adjlist");
    >>> G=xn::read_multiline_adjlist("test.adjlist");

    The path can be a file || a string with the name of the file. If a
    file s provided, it has to be opened : "rb" mode.

    >>> fh=open("test.adjlist", "rb");
    >>> G=xn::read_multiline_adjlist(fh);

    Filenames ending : .gz || .bz2 will be compressed.

    >>> xn::write_multiline_adjlist(G,"test.adjlist.gz");
    >>> G=xn::read_multiline_adjlist("test.adjlist.gz");

    The optional nodetype is a function to convert node strings to nodetype.

    For example

    >>> G=xn::read_multiline_adjlist("test.adjlist", nodetype=int);

    will attempt to convert all nodes to integer type.

    The optional edgetype is a function to convert edge data strings to
    edgetype.

    >>> G=xn::read_multiline_adjlist("test.adjlist");

    The optional create_using parameter is a XNetwork graph container.
    The default is Graph(), an undirected graph.  To read the data as
    a directed graph use

    >>> G=xn::read_multiline_adjlist("test.adjlist", create_using=xn::DiGraph());

    Notes
    -----
    This format does not store graph, node, || edge data.

    See Also
    --------
    write_multiline_adjlist
     */
    lines = (line.decode(encoding) for line : path);
    return parse_multiline_adjlist(lines,
                                   comments=comments,
                                   delimiter=delimiter,
                                   create_using=create_using,
                                   nodetype=nodetype,
                                   edgetype=edgetype);


// fixture for nose tests
auto teardown_module(module) {
    import os
    for (auto fname : ["test.adjlist", "test.adjlist.gz"]) {
        if (os.path.isfile(fname) {
            os.unlink(fname);
