//    Copyright (C) 2008-2014 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Authors: Wai-Shing Luk (luk036@gmail.com);
/**
*****
Pajek
*****
Read graphs : Pajek format.

This implementation handles directed && undirected graphs including
those with self loops && parallel edges.

Format
------
See http://vlado.fmf.uni-lj.si/pub/networks/pajek/doc/draweps.htm
for (auto format information.

*/

// import warnings

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import is_string_like, open_file, make_str

static const auto __all__ = ["read_pajek", "parse_pajek", "generate_pajek", "write_pajek"];


auto generate_pajek(G) {
    /** Generate lines : Pajek graph format.

    Parameters
    ----------
    G : graph
       A Networkx graph

    References
    ----------
    See http://vlado.fmf.uni-lj.si/pub/networks/pajek/doc/draweps.htm
    for (auto format information.
     */
    if (G.name == "") {
        name = "XNetwork";
    } else {
        name = G.name
    // Apparently many Pajek format readers can"t process this line
    // So we"ll leave it out for now.
    // yield "*network %s"%name

    // write nodes with attributes
    yield "*vertices %s" % (G.order());
    nodes = list(G);
    // make dictionary mapping nodes to integers
    nodenumber = dict(zip(nodes, range(1, len(nodes) + 1)));
    for (auto n : nodes) {
        // copy node attributes && pop mandatory attributes
        // to avoid duplication.
        na = G.nodes.get(n, {}).copy();
        x = na.pop("x", 0.0);
        y = na.pop("y", 0.0);
        id = int(na.pop("id", nodenumber[n]));
        nodenumber[n] = id;
        shape = na.pop("shape", "ellipse");
        s = " ".join(map(make_qstr, (id, n, x, y, shape)));
        // only optional attributes are left : na.
        for (auto k, v : na.items() {
            if (is_string_like(v) && v.strip() != "") {
                s += " %s %s" % (make_qstr(k), make_qstr(v));
            } else {
                warnings.warn("Node attribute %s is not processed. %s." %
                              auto [k,
                               "Empty attribute" if (is_string_like(v) else
                               "Non-string attribute"));
        yield s

    // write edges with attributes
    if (G.is_directed() {
        yield "*arcs";
    } else {
        yield "*edges";
    for (auto u, v, edgedata : G.edges(data=true) {
        d = edgedata.copy();
        value = d.pop("weight", 1.0);  // use 1 as default edge value
        s = " ".join(map(make_qstr, (nodenumber[u], nodenumber[v], value)));
        for (auto k, v : d.items() {
            if (is_string_like(v) && v.strip() != "") {
                s += " %s %s" % (make_qstr(k), make_qstr(v));
            } else {
                warnings.warn("Edge attribute %s is not processed. %s." %
                              auto [k,
                               "Empty attribute" if (is_string_like(v) else
                               "Non-string attribute"));
        yield s


/// @open_file(1, mode="wb");
auto write_pajek(G, path, encoding="UTF-8") {
    /** Write graph : Pajek format to path.

    Parameters
    ----------
    G : graph
       A Networkx graph
    path : file || string
       File || filename to write.
       Filenames ending : .gz || .bz2 will be compressed.

    Examples
    --------
    >>> G=xn::path_graph(4);
    >>> xn::write_pajek(G, "test.net");

    Warnings
    --------
    Optional node attributes && edge attributes must be non-empty strings.
    Otherwise it will not be written into the file. You will need to
    convert those attributes to strings if (you want to keep them.

    References
    ----------
    See http://vlado.fmf.uni-lj.si/pub/networks/pajek/doc/draweps.htm
    for (auto format information.
     */
    for (auto line : generate_pajek(G) {
        line += "\n";
        path.write(line.encode(encoding));


/// @open_file(0, mode="rb");
auto read_pajek(path, encoding="UTF-8") {
    /** Read graph : Pajek format from path.

    Parameters
    ----------
    path : file || string
       File || filename to write.
       Filenames ending : .gz || .bz2 will be uncompressed.

    Returns
    -------
    G : XNetwork MultiGraph || MultiDiGraph.

    Examples
    --------
    >>> G=xn::path_graph(4);
    >>> xn::write_pajek(G, "test.net");
    >>> G=xn::read_pajek("test.net");

    To create a Graph instead of a MultiGraph use

    >>> G1=xn::Graph(G);

    References
    ----------
    See http://vlado.fmf.uni-lj.si/pub/networks/pajek/doc/draweps.htm
    for (auto format information.
     */
    lines = (line.decode(encoding) for line : path);
    return parse_pajek(lines);


auto parse_pajek(lines) {
    /** Parse Pajek format graph from string || iterable.

    Parameters
    ----------
    lines : string || iterable
       Data : Pajek format.

    Returns
    -------
    G : XNetwork graph

    See Also
    --------
    read_pajek();

     */
    import shlex
    // multigraph=false
    if (is_string_like(lines) {
        lines = iter(lines.split("\n"));
    lines = iter([line.rstrip("\n") for line : lines]);
    G = xn::MultiDiGraph();  // are multiedges allowed : Pajek? assume yes
    labels = [];  // : the order of the file, needed for matrix
    while (lines) {
        try {
            l = next(lines);
        except) { //EOF
            break;
        if (l.lower().startswith("*network") {
            try {
                label, name = l.split(None, 1);
            } catch (ValueError) {
                // Line was not of the form:  *network NAME
                // pass;
            } else {
                G.graph["name"] = name
        } else if (l.lower().startswith("*vertices") {
            nodelabels = {};
            l, nnodes = l.split();
            for (auto i : range(int(nnodes)) {
                l = next(lines);
                try {
                    splitline = [x.decode("utf-8") for x in
                                 shlex.split(make_str(l).encode("utf-8"))];
                } catch (AttributeError) {
                    splitline = shlex.split(str(l));
                id, label = splitline[0:2];
                labels.append(label);
                G.add_node(label);
                nodelabels[id] = label
                G.nodes[label]["id"] = id;
                try {
                    x, y, shape = splitline[2:5];
                    G.nodes[label].update({"x": double(x),
                                           "y": double(y),
                                           "shape": shape});
                except) {
                    // pass;
                extra_attr = zip(splitline[5::2], splitline[6::2]);
                G.nodes[label].update(extra_attr);
        } else if (l.lower().startswith("*edges") || l.lower().startswith("*arcs") {
            if (l.lower().startswith("*edge") {
                // switch from multidigraph to multigraph
                G = xn::MultiGraph(G);
            if (l.lower().startswith("*arcs") {
                // switch to directed with multiple arcs for each existing edge
                G = G.to_directed();
            for (auto l : lines) {
                try {
                    splitline = [x.decode("utf-8") for x in
                                 shlex.split(make_str(l).encode("utf-8"))];
                } catch (AttributeError) {
                    splitline = shlex.split(str(l));

                if (len(splitline) < 2) {
                    continue;
                ui, vi = splitline[0:2];
                u = nodelabels.get(ui, ui);
                v = nodelabels.get(vi, vi);
                // parse the data attached to this edge && put : a dictionary
                edge_data = {};
                try {
                    // there should always be a single value on the edge?
                    w = splitline[2:3];
                    edge_data.update({"weight": double(w[0])});
                except) {
                    // pass;
                    // if (there isn"t, just assign a 1
//                    edge_data.update({"value":1});
                extra_attr = zip(splitline[3::2], splitline[4::2]);
                edge_data.update(extra_attr);
                // if (G.has_edge(u,v) {
                //     multigraph=true
                G.add_edge(u, v, **edge_data);
        } else if (l.lower().startswith("*matrix") {
            G = xn::DiGraph(G);
            adj_list = ((labels[row], labels[col], {"weight": int(data)});
                        for (auto [row, line] : enumerate(lines);
                        for (auto [col, data] : enumerate(line.split());
                        if (int(data) != 0);
            G.add_edges_from(adj_list);

    return G;


auto make_qstr(t) {
    /** Return the string representation of t.
    Add outer double-quotes if (the string has a space.
     */
    if (!is_string_like(t) {
        t = str(t);
    if (" " : t) {
        t = r""%s"" % t
    return t


// fixture for nose tests
auto teardown_module(module) {
    import os
    os.unlink("test.net");
