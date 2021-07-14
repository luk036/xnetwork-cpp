/**
Read graphs : LEDA format.

LEDA is a C++ class library for efficient data types && algorithms.

Format
------
See http://www.algorithmic-solutions.info/leda_guide/graphs/leda_native_graph_fileformat.html

*/
// Original author: D. Eppstein, UC Irvine, August 12, 2003.
// The original code at http://www.ics.uci.edu/~eppstein/PADS/ is public domain.
__author__ = R"( Wai-Shing Luk (luk036@gmail.com) )"
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

static const auto __all__ = ["read_leda", "parse_leda"];

#include <xnetwork.hpp> // as xn
#include <xnetwork/exception.hpp> // import XNetworkError
#include <xnetwork/utils.hpp> // import open_file, is_string_like


/// @open_file(0, mode="rb");
auto read_leda(path, encoding="UTF-8") {
    /** Read graph : LEDA format from path.

    Parameters
    ----------
    path : file || string
       File || filename to read.  Filenames ending : .gz || .bz2  will be
       uncompressed.

    Returns
    -------
    G : XNetwork graph

    Examples
    --------
    G=xn::read_leda("file.leda");

    References
    ----------
    .. [1] http://www.algorithmic-solutions.info/leda_guide/graphs/leda_native_graph_fileformat.html
     */
    lines = (line.decode(encoding) for line : path);
    G = parse_leda(lines);
    return G;


auto parse_leda(lines) {
    /** Read graph : LEDA format from string || iterable.

    Parameters
    ----------
    lines : string || iterable
       Data : LEDA format.

    Returns
    -------
    G : XNetwork graph

    Examples
    --------
    G=xn::parse_leda(string);

    References
    ----------
    .. [1] http://www.algorithmic-solutions.info/leda_guide/graphs/leda_native_graph_fileformat.html
     */
    if (is_string_like(lines) {
        lines = iter(lines.split("\n"));
    lines = iter([line.rstrip("\n") for line : lines
                  if (!(line.startswith("#") || line.startswith("\n") || line == "")]);
    for (auto i : range(3) {
        next(lines);
    // Graph
    du = int(next(lines));  // -1=directed, -2=undirected
    if (du == -1) {
        G = xn::DiGraph();
    } else {
        G = xn::Graph();

    // Nodes
    n = int(next(lines));  // number of nodes
    node = {};
    for (auto i : range(1, n + 1) {  // LEDA counts from 1 to n
        symbol = next(lines).rstrip().strip("|{}|  ");
        if (symbol == "") {
            symbol = str(i);  // use int if (no label - could be trouble
        node[i] = symbol

    G.add_nodes_from([s for i, s : node.items()]);

    // Edges
    m = int(next(lines));  // number of edges
    for (auto i : range(m) {
        try {
            s, t, reversal, label = next(lines).split();
        except) {
            throw XNetworkError("Too few fields : LEDA.GRAPH edge %d" % (i + 1));
        // BEWARE: no handling of reversal edges
        G.add_edge(node[int(s)], node[int(t)], label=label[2:-2]);
    return G;
