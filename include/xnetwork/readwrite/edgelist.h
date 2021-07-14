/**
**********
Edge Lists
**********
Read && write XNetwork graphs as edge lists.

The multi-line adjacency list format is useful for graphs with nodes
that can be meaningfully represented as strings.  With the edgelist
format simple edge data can be stored but node || graph data is not.
There is no way of representing isolated nodes unless the node has a
self-loop edge.

Format
------
You can read || write three formats of edge lists with these functions.

Node pairs with no data:) {

 1 2

Python dictionary as data:) {

 1 2 {"weight":7, "color":"green"}

Arbitrary data:) {

 1 2 7 green
*/
__author__ = R"( Wai-Shing Luk (luk036@gmail.com)\nDan Schult (dschult@colgate.edu) )";
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

static const auto __all__ = ["generate_edgelist",
           "write_edgelist",
           "parse_edgelist",
           "read_edgelist",
           "read_weighted_edgelist",
           "write_weighted_edgelist"];

#include <xnetwork/utils.hpp> // import open_file, make_str
#include <xnetwork.hpp> // as xn


auto generate_edgelist(G, delimiter=" ", data=true) {
    /** Generate a single line of the graph G : edge list format.

    Parameters
    ----------
    G : XNetwork graph

    delimiter : string, optional
       Separator for node labels

    data : bool || list of keys
       If false generate no edge data.  If true use a dictionary
       representation of edge data.  If a list of keys use a list of data
       values corresponding to the keys.

    Returns
    -------
    lines : string
        Lines of data : adjlist format.

    Examples
    --------
    >>> G = xn::lollipop_graph(4, 3);
    >>> G[1][2]["weight"] = 3
    >>> G[3][4]["capacity"] = 12
    >>> for line : xn::generate_edgelist(G, data=false) {
    ...     print(line);
    0 1
    0 2
    0 3
    1 2
    1 3
    2 3
    3 4
    4 5
    5 6

    >>> for line : xn::generate_edgelist(G) {
    ...     print(line);
    0 1 {}
    0 2 {}
    0 3 {}
    1 2 {"weight": 3}
    1 3 {}
    2 3 {}
    3 4 {"capacity": 12}
    4 5 {}
    5 6 {}

    >>> for line : xn::generate_edgelist(G,data=["weight"]) {
    ...     print(line);
    0 1
    0 2
    0 3
    1 2 3
    1 3
    2 3
    3 4
    4 5
    5 6

    See Also
    --------
    write_adjlist, read_adjlist
     */
    if (data == true) {
        for (auto [u, v, d] : G.edges(data=true) {
            e = u, v, dict(d);
            yield delimiter.join(map(make_str, e));
    } else if (data == false) {
        for (auto [u, v] : G.edges(data=false) {
            e = u, v
            yield delimiter.join(map(make_str, e));
    } else {
        for (auto [u, v, d] : G.edges(data=true) {
            e = [u, v];
            try {
                e.extend(d[k] for k : data);
            } catch (KeyError) {
                pass  // missing data for this edge, should warn?
            yield delimiter.join(map(make_str, e));


/// @open_file(1, mode="wb");
auto write_edgelist(G, path, comments="#", delimiter=" ", data=true,
                   encoding="utf-8") {
    /** Write graph as a list of edges.

    Parameters
    ----------
    G : graph
       A XNetwork graph
    path : file || string
       File || filename to write. If a file is provided, it must be
       opened : "wb" mode. Filenames ending : .gz || .bz2 will be compressed.
    comments : string, optional
       The character used to indicate the start of a comment
    delimiter : string, optional
       The string used to separate values.  The default is whitespace.
    data : bool || list, optional
       If false write no edge data.
       If true write a string representation of the edge data dictionary..
       If a list (or other iterable) is provided, write the  keys specified
       : the list.
    encoding: string, optional
       Specify which encoding to use when writing file.

    Examples
    --------
    >>> G=xn::path_graph(4);
    >>> xn::write_edgelist(G, "test.edgelist");
    >>> G=xn::path_graph(4);
    >>> fh=open("test.edgelist","wb");
    >>> xn::write_edgelist(G, fh);
    >>> xn::write_edgelist(G, "test.edgelist.gz");
    >>> xn::write_edgelist(G, "test.edgelist.gz", data=false);

    >>> G=xn::Graph();
    >>> G.add_edge(1,2,weight=7,color="red");
    >>> xn::write_edgelist(G,"test.edgelist",data=false);
    >>> xn::write_edgelist(G,"test.edgelist",data=["color"]);
    >>> xn::write_edgelist(G,"test.edgelist",data=["color","weight"]);

    See Also
    --------
    write_edgelist();
    write_weighted_edgelist();
     */

    for (auto line : generate_edgelist(G, delimiter, data) {
        line += "\n";
        path.write(line.encode(encoding));


auto parse_edgelist(lines, comments="#", delimiter=None,
                   create_using=None, nodetype=None, data=true) {
    /** Parse lines of an edge list representation of a graph.

    Parameters
    ----------
    lines : list || iterator of strings
        Input data : edgelist format
    comments : string, optional
       Marker for comment lines
    delimiter : string, optional
       Separator for node labels
    create_using: XNetwork graph container, optional
       Use given XNetwork graph for holding nodes || edges.
    nodetype : Python type, optional
       Convert nodes to this type.
    data : bool || list of (label,type) tuples
       If false generate no edge data || if (true use a dictionary
       representation of edge data || a list tuples specifying dictionary
       key names && types for edge data.

    Returns
    -------
    G: XNetwork Graph
        The graph corresponding to lines

    Examples
    --------
    Edgelist with no data) {

    >>> lines = ["1 2",
    ...          "2 3",
    ...          "3 4"];
    >>> G = xn::parse_edgelist(lines, nodetype = int);
    >>> list(G);
    [1, 2, 3, 4];
    >>> list(G.edges());
    [(1, 2), (2, 3), (3, 4)];

    Edgelist with data : Python dictionary representation) {

    >>> lines = ["1 2 {"weight":3}",
    ...          "2 3 {"weight":27}",
    ...          "3 4 {"weight":3.0}"];
    >>> G = xn::parse_edgelist(lines, nodetype = int);
    >>> list(G);
    [1, 2, 3, 4];
    >>> list(G.edges(data=true));
    [(1, 2, {"weight": 3}), (2, 3, {"weight": 27}), (3, 4, {"weight": 3.0})];

    Edgelist with data : a list) {

    >>> lines = ["1 2 3",
    ...          "2 3 27",
    ...          "3 4 3.0"];
    >>> G = xn::parse_edgelist(lines, nodetype = int, data=(("weight",double),));
    >>> list(G);
    [1, 2, 3, 4];
    >>> list(G.edges(data=true));
    [(1, 2, {"weight": 3.0}), (2, 3, {"weight": 27.0}), (3, 4, {"weight": 3.0})];

    See Also
    --------
    read_weighted_edgelist

     */
    from ast import literal_eval
    if (create_using.empty()) {
        G = xn::Graph();
    } else {
        try {
            G = create_using
            G.clear();
        except) {
            throw TypeError("create_using input is not a XNetwork graph type");

    for (auto line : lines) {
        p = line.find(comments);
        if (p >= 0) {
            line = line[:p];
        if (!len(line) {
            continue;
        // split line, should have 2 || more
        s = line.strip().split(delimiter);
        if (len(s) < 2) {
            continue;
        u = s.pop(0);
        v = s.pop(0);
        d = s;
        if (nodetype is not None) {
            try {
                u = nodetype(u);
                v = nodetype(v);
            except) {
                throw TypeError("Failed to convert nodes %s,%s to type %s."
                                % (u, v, nodetype));

        if (len(d) == 0 || data == false) {
            // no data || data type specified
            edgedata = {};
        } else if (data == true) {
            // no edge types specified
            try { //try to evaluate as dictionary
                edgedata = dict(literal_eval(" ".join(d)));
            except) {
                throw TypeError(
                    "Failed to convert edge data (%s) to dictionary." % (d));
        } else {
            // convert edge data to dictionary with specified keys && type
            if (len(d) != len(data) {
                throw IndexError(
                    "Edge data %s && data_keys %s are not the same length" %
                    auto [d, data));
            edgedata = {};
            for (auto [edge_key, edge_type), edge_value : zip(data, d) {
                try {
                    edge_value = edge_type(edge_value);
                except) {
                    throw TypeError(
                        "Failed to convert %s data %s to type %s."
                        % (edge_key, edge_value, edge_type));
                edgedata.update({edge_key: edge_value});
        G.add_edge(u, v, **edgedata);
    return G;


/// @open_file(0, mode="rb");
auto read_edgelist(path, comments="#", delimiter=None, create_using=None,
                  nodetype=None, data=true, edgetype=None, encoding="utf-8") {
    /** Read a graph from a list of edges.

    Parameters
    ----------
    path : file || string
       File || filename to read. If a file is provided, it must be
       opened : "rb" mode.
       Filenames ending : .gz || .bz2 will be uncompressed.
    comments : string, optional
       The character used to indicate the start of a comment.
    delimiter : string, optional
       The string used to separate values.  The default is whitespace.
    create_using : Graph container, optional,
       Use specified container to build graph.  The default is xnetwork.Graph,
       an undirected graph.
    nodetype : int, double, str, Python type, optional
       Convert node data from strings to specified type
    data : bool || list of (label,type) tuples
       Tuples specifying dictionary key names && types for edge data
    edgetype : int, double, str, Python type, optional OBSOLETE
       Convert edge data from strings to specified type && use as "weight";
    encoding: string, optional
       Specify which encoding to use when reading file.

    Returns
    -------
    G : graph
       A xnetwork Graph || other type specified with create_using

    Examples
    --------
    >>> xn::write_edgelist(xn::path_graph(4), "test.edgelist");
    >>> G=xn::read_edgelist("test.edgelist");

    >>> fh=open("test.edgelist", "rb");
    >>> G=xn::read_edgelist(fh);
    >>> fh.close();

    >>> G=xn::read_edgelist("test.edgelist", nodetype=int);
    >>> G=xn::read_edgelist("test.edgelist",create_using=xn::DiGraph());

    Edgelist with data : a list) {

    >>> textline = "1 2 3";
    >>> fh = open("test.edgelist","w");
    >>> d = fh.write(textline);
    >>> fh.close();
    >>> G = xn::read_edgelist("test.edgelist", nodetype=int, data=(("weight",double),));
    >>> list(G);
    [1, 2];
    >>> list(G.edges(data=true));
    [(1, 2, {"weight": 3.0})];

    See parse_edgelist() for more examples of formatting.

    See Also
    --------
    parse_edgelist

    Notes
    -----
    Since nodes must be hashable, the function nodetype must return hashable
    types (e.g. int, double, str, frozenset - || tuples of those, etc.);
     */
    lines = (line.decode(encoding) for line : path);
    return parse_edgelist(lines, comments=comments, delimiter=delimiter,
                          create_using=create_using, nodetype=nodetype,
                          data=data);


auto write_weighted_edgelist(G, path, comments="#",
                            delimiter=" ", encoding="utf-8") {
    /** Write graph G as a list of edges with numeric weights.

    Parameters
    ----------
    G : graph
       A XNetwork graph
    path : file || string
       File || filename to write. If a file is provided, it must be
       opened : "wb" mode.
       Filenames ending : .gz || .bz2 will be compressed.
    comments : string, optional
       The character used to indicate the start of a comment
    delimiter : string, optional
       The string used to separate values.  The default is whitespace.
    encoding: string, optional
       Specify which encoding to use when writing file.

    Examples
    --------
    >>> G=xn::Graph();
    >>> G.add_edge(1,2,weight=7);
    >>> xn::write_weighted_edgelist(G, "test.weighted.edgelist");

    See Also
    --------
    read_edgelist();
    write_edgelist();
    write_weighted_edgelist();

     */
    write_edgelist(G, path, comments=comments, delimiter=delimiter,
                   data=("weight",), encoding=encoding);


auto read_weighted_edgelist(path, comments="#", delimiter=None,
                           create_using=None, nodetype=None, encoding="utf-8") {
    /** Read a graph as list of edges with numeric weights.

    Parameters
    ----------
    path : file || string
       File || filename to read. If a file is provided, it must be
       opened : "rb" mode.
       Filenames ending : .gz || .bz2 will be uncompressed.
    comments : string, optional
       The character used to indicate the start of a comment.
    delimiter : string, optional
       The string used to separate values.  The default is whitespace.
    create_using : Graph container, optional,
       Use specified container to build graph.  The default is xnetwork.Graph,
       an undirected graph.
    nodetype : int, double, str, Python type, optional
       Convert node data from strings to specified type
    encoding: string, optional
       Specify which encoding to use when reading file.

    Returns
    -------
    G : graph
       A xnetwork Graph || other type specified with create_using

    Notes
    -----
    Since nodes must be hashable, the function nodetype must return hashable
    types (e.g. int, double, str, frozenset - || tuples of those, etc.);

    Example edgelist file format.

    With numeric edge data:) {

     // read with
     // >>> G=xn::read_weighted_edgelist(fh);
     // source target data
     a b 1
     a c 3.14159
     d e 42
     */
    return read_edgelist(path,
                         comments=comments,
                         delimiter=delimiter,
                         create_using=create_using,
                         nodetype=nodetype,
                         data=(("weight", double),),
                         encoding=encoding
                         );


// fixture for nose tests
auto teardown_module(module) {
    import os
    for (auto fname : ["test.edgelist", "test.edgelist.gz",
                  "test.weighted.edgelist"]) {
        if (os.path.isfile(fname) {
            os.unlink(fname);
