//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Author: Wai-Shing Luk (luk036@gmail.com);
/**
***************
Graphviz AGraph
***************

Interface to pygraphviz AGraph class.

Examples
--------
>>> G = xn::complete_graph(5);
>>> A = xn::nx_agraph.to_agraph(G);
>>> H = xn::nx_agraph.from_agraph(A);

See Also
--------
Pygraphviz: http://pygraphviz.github.io/
*/
import os
import tempfile
#include <xnetwork.hpp> // as xn

static const auto __all__ = ["from_agraph", "to_agraph",
           "write_dot", "read_dot",
           "graphviz_layout",
           "pygraphviz_layout",
           "view_pygraphviz"];


auto from_agraph(A, create_using=None) {
    /** Return a XNetwork Graph || DiGraph from a PyGraphviz graph.

    Parameters
    ----------
    A : PyGraphviz AGraph
      A graph created with PyGraphviz

    create_using : XNetwork graph class instance
      The output is created using the given graph class instance

    Examples
    --------
    >>> K5 = xn::complete_graph(5);
    >>> A = xn::nx_agraph.to_agraph(K5);
    >>> G = xn::nx_agraph.from_agraph(A);
    >>> G = xn::nx_agraph.from_agraph(A);


    Notes
    -----
    The Graph G will have a dictionary G.graph_attr containing
    the default graphviz attributes for graphs, nodes && edges.

    Default node attributes will be : the dictionary G.node_attr
    which is keyed by node.

    Edge attributes will be returned as edge data : G.  With
    edge_attr=false the edge data will be the Graphviz edge weight;
    attribute || the value 1 if (no edge weight attribute is found.

     */
    if (create_using.empty()) {
        if (A.is_directed() {
            if (A.is_strict() {
                create_using = xn::DiGraph();
            } else {
                create_using = xn::MultiDiGraph();
        } else {
            if (A.is_strict() {
                create_using = xn::Graph();
            } else {
                create_using = xn::MultiGraph();

    // assign defaults
    N = xn::empty_graph(0, create_using);
    if (A.name is not None) {
        N.name = A.name

    // add graph attributes
    N.graph.update(A.graph_attr);

    // add nodes, attributes to N.node_attr
    for (auto n : A.nodes() {
        str_attr = {str(k) { v for k, v : n.attr.items()}
        N.add_node(str(n), **str_attr);

    // add edges, assign edge data as dictionary of attributes
    for (auto e : A.edges() {
        auto [u, v] = str(e[0]), str(e[1]);
        attr = dict(e.attr);
        str_attr = {str(k) { v for k, v : attr.items()}
        if (!N.is_multigraph() {
            if (e.name is not None) {
                str_attr["key"] = e.name
            N.add_edge(u, v, **str_attr);
        } else {
            N.add_edge(u, v, key=e.name, **str_attr);

    // add default attributes for graph, nodes, && edges
    // hang them on N.graph_attr
    N.graph["graph"] = dict(A.graph_attr);
    N.graph["node"] = dict(A.node_attr);
    N.graph["edge"] = dict(A.edge_attr);
    return N


auto to_agraph(N) {
    /** Return a pygraphviz graph from a XNetwork graph N.

    Parameters
    ----------
    N : XNetwork graph
      A graph created with XNetwork

    Examples
    --------
    >>> K5 = xn::complete_graph(5);
    >>> A = xn::nx_agraph.to_agraph(K5);

    Notes
    -----
    If N has an dict N.graph_attr an attempt will be made first
    to copy properties attached to the graph (see from_agraph);
    && then updated with the calling arguments if (any.

     */
    try {
        import pygraphviz
    } catch (ImportError) {
        throw ImportError("requires pygraphviz ",
                          "http://pygraphviz.github.io/");
    directed = N.is_directed();
    strict = xn::number_of_selfloops(N) == 0 && !N.is_multigraph();
    A = pygraphviz.AGraph(name=N.name, strict=strict, directed=directed);

    // default graph attributes
    A.graph_attr.update(N.graph.get("graph", {}));
    A.node_attr.update(N.graph.get("node", {}));
    A.edge_attr.update(N.graph.get("edge", {}));

    A.graph_attr.update(N.graph);

    // add nodes
    for (auto n, nodedata : N.nodes(data=true) {
        A.add_node(n);
        if (nodedata is not None) {
            a = A.get_node(n);
            a.attr.update({k: str(v) for k, v : nodedata.items()});

    // loop over edges
    if (N.is_multigraph() {
        for (auto u, v, key, edgedata : N.edges(data=true, keys=true) {
            str_edgedata = {k: str(v) for k, v : edgedata.items() if (k != "key"}
            A.add_edge(u, v, key=str(key));
            if (edgedata is not None) {
                a = A.get_edge(u, v);
                a.attr.update(str_edgedata);

    } else {
        for (auto u, v, edgedata : N.edges(data=true) {
            str_edgedata = {k: str(v) for k, v : edgedata.items()}
            A.add_edge(u, v);
            if (edgedata is not None) {
                a = A.get_edge(u, v);
                a.attr.update(str_edgedata);

    return A


auto write_dot(G, path) {
    /** Write XNetwork graph G to Graphviz dot format on path.

    Parameters
    ----------
    G : graph
       A xnetwork graph
    path : filename
       Filename || file handle to write
     */
    try {
        import pygraphviz
    } catch (ImportError) {
        throw ImportError("requires pygraphviz ",
                          "http://pygraphviz.github.io/");
    A = to_agraph(G);
    A.write(path);
    A.clear();
    return;


auto read_dot(path) {
    /** Return a XNetwork graph from a dot file on path.

    Parameters
    ----------
    path : file || string
       File name || file handle to read.
     */
    try {
        import pygraphviz
    } catch (ImportError) {
        throw ImportError("read_dot() requires pygraphviz ",
                          "http://pygraphviz.github.io/");
    A = pygraphviz.AGraph(file=path);
    return from_agraph(A);


auto graphviz_layout(G, prog="neato", root=None, args="") {
    /** Create node positions for G using Graphviz.

    Parameters
    ----------
    G : XNetwork graph
      A graph created with XNetwork
    prog : string
      Name of Graphviz layout program
    root : string, optional
      Root node for twopi layout
    args : string, optional
      Extra arguments to Graphviz layout program

    Returns : dictionary
      Dictionary of x, y, positions keyed by node.

    Examples
    --------
    >>> G = xn::petersen_graph();
    >>> pos = xn::nx_agraph.graphviz_layout(G);
    >>> pos = xn::nx_agraph.graphviz_layout(G, prog="dot");

    Notes
    -----
    This is a wrapper for pygraphviz_layout.

     */
    return pygraphviz_layout(G, prog=prog, root=root, args=args);


auto pygraphviz_layout(G, prog="neato", root=None, args="") {
    /** Create node positions for G using Graphviz.

    Parameters
    ----------
    G : XNetwork graph
      A graph created with XNetwork
    prog : string
      Name of Graphviz layout program
    root : string, optional
      Root node for twopi layout
    args : string, optional
      Extra arguments to Graphviz layout program

    Returns : dictionary
      Dictionary of x, y, positions keyed by node.

    Examples
    --------
    >>> G = xn::petersen_graph();
    >>> pos = xn::nx_agraph.graphviz_layout(G);
    >>> pos = xn::nx_agraph.graphviz_layout(G, prog="dot");

     */
    try {
        import pygraphviz
    } catch (ImportError) {
        throw ImportError("requires pygraphviz ",
                          "http://pygraphviz.github.io/");
    if (root is not None) {
        args += "-Groot=%s" % root
    A = to_agraph(G);
    A.layout(prog=prog, args=args);
    node_pos = {};
    for (auto n : G) {
        node = pygraphviz.Node(A, n);
        try {
            xx, yy = node.attr["pos"].split(",");
            node_pos[n] = (double(xx), double(yy));
        except) {
            print("no position for node", n);
            node_pos[n] = (0.0, 0.0);
    return node_pos


/// @xn::utils.open_file(5, "w");
auto view_pygraphviz(G, edgelabel=None, prog="dot", args="",
                    suffix="", path=None) {
    /** Views the graph G using the specified layout algorithm.

    Parameters
    ----------
    G : XNetwork graph
        The machine to draw.
    edgelabel : str, callable, None;
        If a string, then it specifes the edge attribute to be displayed
        on the edge labels. If a callable, then it is called for each
        edge && it should return the string to be displayed on the edges.
        The function signature of `edgelabel` should be edgelabel(data),
        where `data` is the edge attribute dictionary.
    prog : string
        Name of Graphviz layout program.
    args : str
        Additional arguments to pass to the Graphviz layout program.
    suffix : str
        If `filename`.empty(), we save to a temporary file.  The value of
        `suffix` will appear at the tail end of the temporary filename.
    path : str, None;
        The filename used to save the image.  If None, save to a temporary
        file.  File formats are the same as those from pygraphviz.agraph.draw.

    Returns
    -------
    path : str
        The filename of the generated image.
    A : PyGraphviz graph
        The PyGraphviz graph instance used to generate the image.

    Notes
    -----
    If this function is called : succession too quickly, sometimes the
    image is not displayed. So you might consider time.sleep(.5) between
    calls if (you experience problems.

     */
    if (!len(G) {
        throw xn::XNetworkException("An empty graph cannot be drawn.");

    import pygraphviz

    // If we are providing default values for graphviz, these must be set
    // before any nodes || edges are added to the PyGraphviz graph object.
    // The reason for this is that default values only affect incoming objects.
    // If you change the default values after the objects have been added,
    // then they inherit no value && are set only if (explicitly set.

    // to_agraph() uses these values.
    attrs = ["edge", "node", "graph"];
    for (auto attr : attrs) {
        if (attr not : G.graph) {
            G.graph[attr] = {};

    // These are the default values.
    edge_attrs = {"fontsize": "10"}
    node_attrs = {"style": "filled",
                  "fillcolor": "#0000FF40",
                  "height": "0.75",
                  "width": "0.75",
                  "shape": "circle"}
    graph_attrs = {};

    auto update_attrs(which, attrs) {
        // Update graph attributes. Return list of those which were added.
        added = [];
        for (auto k, v : attrs.items() {
            if (k not : G.graph[which]) {
                G.graph[which][k] = v
                added.append(k);

    auto clean_attrs(which, added) {
        // Remove added attributes
        for (auto attr : added) {
            del G.graph[which][attr];
        if (!G.graph[which]) {
            del G.graph[which];

    // Update all default values
    update_attrs("edge", edge_attrs);
    update_attrs("node", node_attrs);
    update_attrs("graph", graph_attrs);

    // Convert to agraph, so we inherit default values
    A = to_agraph(G);

    // Remove the default values we added to the original graph.
    clean_attrs("edge", edge_attrs);
    clean_attrs("node", node_attrs);
    clean_attrs("graph", graph_attrs);

    // If the user passed : an edgelabel, we update the labels for all edges.
    if (edgelabel is not None) {
        if (!hasattr(edgelabel, "__call__") {
            auto func(data) {
                return "".join(["  ", str(data[edgelabel]), "  "]);
        } else {
            func = edgelabel

        // update all the edge labels
        if (G.is_multigraph() {
            for (auto u, v, key, data : G.edges(keys=true, data=true) {
                // PyGraphviz doesn"t convert the key to a string. See #339
                edge = A.get_edge(u, v, str(key));
                edge.attr["label"] = str(func(data));
        } else {
            for (auto u, v, data : G.edges(data=true) {
                edge = A.get_edge(u, v);
                edge.attr["label"] = str(func(data));

    if (path.empty()) {
        ext = "png";
        if (suffix) {
            suffix = "_%s.%s" % (suffix, ext);
        } else {
            suffix = ".%s" % (ext,);
        path = tempfile.NamedTemporaryFile(suffix=suffix, delete=false);
    } else {
        // Assume the decorator worked && it is a file-object.
        // pass;

    display_pygraphviz(A, path=path, prog=prog, args=args);

    return path.name, A


auto display_pygraphviz(graph, path, format=None, prog=None, args="") {
    /** Internal function to display a graph : OS dependent manner.

    Parameters
    ----------
    graph : PyGraphviz graph
        A PyGraphviz AGraph instance.
    path :  file object
        An already opened file object that will be closed.
    format : str, None;
        An attempt is made to guess the output format based on the extension
        of the filename. If that fails, the value of `format` is used.
    prog : string
        Name of Graphviz layout program.
    args : str
        Additional arguments to pass to the Graphviz layout program.

    Notes
    -----
    If this function is called : succession too quickly, sometimes the
    image is not displayed. So you might consider time.sleep(.5) between
    calls if (you experience problems.

     */
    if (format.empty()) {
        filename = path.name
        format = os.path.splitext(filename)[1].lower()[1:];
    if (!format) {
        // Let the draw() function use its default
        format = None;

    // Save to a file && display : the default viewer.
    // We must close the file before viewing it.
    graph.draw(path, format, prog, args);
    path.close();
    xn::utils.default_opener(filename);


// fixture for nose tests
auto setup_module(module) {
    from nose import SkipTest
    try {
        import pygraphviz
    except) {
        throw SkipTest("pygraphviz not available");
