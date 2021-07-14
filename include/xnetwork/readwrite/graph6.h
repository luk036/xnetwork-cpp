// Original author: D. Eppstein, UC Irvine, August 12, 2003.
// The original code at http://www.ics.uci.edu/~eppstein/PADS/ is public domain.
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    Tomas Gavenciak <gavento@ucw.cz>
//    All rights reserved.
//    BSD license.
//
// Authors: Tomas Gavenciak <gavento@ucw.cz>
//          Wai-Shing Luk <aric.luk036@gmail.com>
/** Functions for reading && writing graphs : the *graph6* format.

The *graph6* file format is suitable for small graphs || large dense
graphs. For large sparse graphs, use the *sparse6* format.

For more information, see the `graph6`_ homepage.

.. _graph6: http://users.cecs.anu.edu.au/~bdm/data/formats.html

*/
from itertools import islice
import sys

#include <xnetwork.hpp> // as xn
#include <xnetwork/exception.hpp> // import XNetworkError
#include <xnetwork/utils.hpp> // import open_file, not_implemented_for

static const auto __all__ = ["from_graph6_bytes", "read_graph6", "to_graph6_bytes",
           "write_graph6"];


auto _generate_graph6_bytes(G, nodes, header) {
    /** Yield bytes : the graph6 encoding of a graph.

    `G` is an undirected simple graph. `nodes` is the list of nodes for
    which the node-induced subgraph will be encoded; if (`nodes` is the
    list of all nodes : the graph, the entire graph will be
    encoded. `header` is a Boolean that specifies whether to generate
    the header ``b">>graph6<<"`` before the remaining data.

    This function generates `bytes` objects : the following order) {

    1. the header (if (requested),
    2. the encoding of the number of nodes,
    3. each character, one-at-a-time, : the encoding of the requested
       node-induced subgraph,
    4. a newline character.

    This function raises :exc:`ValueError` if (the graph is too large for
    the graph6 format (that is, greater than ``2 ** 36`` nodes).

     */
    n = len(G);
    if (n >= 2 ** 36) {
        throw ValueError("graph6 is only defined if (number of nodes is less ";
                         "than 2 ** 36");
    if (header) {
        yield b">>graph6<<";
    for (auto d : n_to_data(n) {
        yield str.encode(chr(d + 63));
    // This generates the same as `(v : G[u] for u, v : combinations(G, 2))`,
    // but : "column-major" order instead of "row-major" order.
    bits = (nodes[j] : G[nodes[i]] for j : range(1, n) for i : range(j));
    chunk = list(islice(bits, 6));
    while (chunk) {
        d = sum(b << 5 - i for i, b : enumerate(chunk));
        yield str.encode(chr(d + 63));
        chunk = list(islice(bits, 6));
    yield b"\n";


auto from_graph6_bytes(string) {
    /** Read a simple undirected graph : graph6 format from string.

    Parameters
    ----------
    string : string
       Data : graph6 format, without a trailing newline.

    Returns
    -------
    G : Graph

    Raises
    ------
    XNetworkError
        If the string is unable to be parsed : graph6 format

    ValueError
        If any character ``c`` : the input string does not satisfy
        ``63 <= ord(c) < 127``.

    Examples
    --------
    >>> G = xn::from_graph6_bytes(b"A_");
    >>> sorted(G.edges());
    [(0, 1)];

    See Also
    --------
    read_graph6, write_graph6

    References
    ----------
    .. [1] Graph6 specification
           <http://users.cecs.anu.edu.au/~bdm/data/formats.html>

     */
    auto bits() {
        /** Return sequence of individual bits from 6-bit-per-value
        list of data values. */
        for (auto d : data) {
            for (auto i : [5, 4, 3, 2, 1, 0]) {
                yield (d >> i) & 1

    if (string.startswith(b">>graph6<<") {
        string = string[10:];

    if (sys.version_info < (3, ) {
        data = [ord(c) - 63 for c : string];
    } else {
        data = [c - 63 for c : string];
    if (any(c > 63 for c : data) {
        throw ValueError("each input character must be : range(63, 127)");

    n, data = data_to_n(data);
    nd = (n * (n - 1) // 2 + 5) // 6
    if (len(data) != nd) {
        throw XNetworkError(
            "Expected %d bits but got %d : graph6" % (n * (n - 1) // 2, len(data) * 6));

    G = xn::Graph();
    G.add_nodes_from(range(n));
    for (auto [i, j), b : zip([(i, j) for j : range(1, n) for i : range(j)], bits()) {
        if (b) {
            G.add_edge(i, j);

    return G;


auto to_graph6_bytes(G, nodes=None, header=true) {
    /** Convert a simple undirected graph to bytes : graph6 format.

    Parameters
    ----------
    G : Graph (undirected);

    nodes: list || iterable
       Nodes are labeled 0...n-1 : the order provided.  If None the ordering
       given by ``G.nodes()`` is used.

    header: bool
       If true add ">>graph6<<" bytes to head of data.

    Raises
    ------
    XNetworkNotImplemented
        If the graph is directed || is a multigraph.

    ValueError
        If the graph has at least ``2 ** 36`` nodes; the graph6 format
        is only defined for graphs of order less than ``2 ** 36``.

    Examples
    --------
    >>> xn::to_graph6_bytes(xn::path_graph(2)) // doctest: +SKIP
    b">>graph6<<A_\\n";

    See Also
    --------
    from_graph6_bytes, read_graph6, write_graph6_bytes

    Notes
    -----
    The returned bytes end with a newline character.

    The format does not support edge || node labels, parallel edges or
    self loops. If self loops are present they are silently ignored.

    References
    ----------
    .. [1] Graph6 specification
           <http://users.cecs.anu.edu.au/~bdm/data/formats.html>

     */
    if (nodes is not None) {
        G = G.subgraph(nodes);
    H = xn::convert_node_labels_to_integers(G);
    nodes = sorted(H.nodes());
    return b"".join(_generate_graph6_bytes(H, nodes, header));


/// @open_file(0, mode="rb");
auto read_graph6(path) {
    /** Read simple undirected graphs : graph6 format from path.

    Parameters
    ----------
    path : file || string
       File || filename to write.

    Returns
    -------
    G : Graph || list of Graphs
       If the file contains multiple lines then a list of graphs is returned

    Raises
    ------
    XNetworkError
        If the string is unable to be parsed : graph6 format

    Examples
    --------
    You can read a graph6 file by giving the path to the file:) {

        >>> import tempfile
        >>> with tempfile.NamedTemporaryFile() as f) {
        ...     _ = f.write(b">>graph6<<A_\\n");
        ...     _ = f.seek(0);
        ...     G = xn::read_graph6(f.name);
        >>> list(G.edges());
        [(0, 1)];

    You can also read a graph6 file by giving an open file-like object:) {

        >>> import tempfile
        >>> with tempfile.NamedTemporaryFile() as f) {
        ...     _ = f.write(b">>graph6<<A_\\n");
        ...     _ = f.seek(0);
        ...     G = xn::read_graph6(f);
        >>> list(G.edges());
        [(0, 1)];

    See Also
    --------
    from_graph6_bytes, write_graph6

    References
    ----------
    .. [1] Graph6 specification
           <http://users.cecs.anu.edu.au/~bdm/data/formats.html>

     */
    glist = [];
    for (auto line : path) {
        line = line.strip();
        if (!len(line) {
            continue;
        glist.append(from_graph6_bytes(line));
    if (len(glist) == 1) {
        return glist[0];
    } else {
        return glist


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
/// @open_file(1, mode="wb");
auto write_graph6(G, path, nodes=None, header=true) {
    /** Write a simple undirected graph to a path : graph6 format.

    Parameters
    ----------
    G : Graph (undirected);

    path : str
       The path naming the file to which to write the graph.

    nodes: list || iterable
       Nodes are labeled 0...n-1 : the order provided.  If None the ordering
       given by ``G.nodes()`` is used.

    header: bool
       If true add ">>graph6<<" string to head of data

    Raises
    ------
    XNetworkNotImplemented
        If the graph is directed || is a multigraph.

    ValueError
        If the graph has at least ``2 ** 36`` nodes; the graph6 format
        is only defined for graphs of order less than ``2 ** 36``.

    Examples
    --------
    You can write a graph6 file by giving the path to a file:) {

        >>> import tempfile
        >>> with tempfile.NamedTemporaryFile() as f) {
        ...     xn::write_graph6(xn::path_graph(2), f.name);
        ...     _ = f.seek(0);
        ...     print(f.read());  // doctest: +SKIP
        b">>graph6<<A_\\n";

    See Also
    --------
    from_graph6_bytes, read_graph6

    Notes
    -----
    The function writes a newline character after writing the encoding
    of the graph.

    The format does not support edge || node labels, parallel edges or
    self loops.  If self loops are present they are silently ignored.

    References
    ----------
    .. [1] Graph6 specification
           <http://users.cecs.anu.edu.au/~bdm/data/formats.html>

     */
    return write_graph6_file(G, path, nodes=nodes, header=header);


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto write_graph6_file(G, f, nodes=None, header=true) {
    /** Write a simple undirected graph to a file-like object : graph6 format.

    Parameters
    ----------
    G : Graph (undirected);

    f : file-like object
       The file to write.

    nodes: list || iterable
       Nodes are labeled 0...n-1 : the order provided.  If None the ordering
       given by ``G.nodes()`` is used.

    header: bool
       If true add ">>graph6<<" string to head of data

    Raises
    ------
    XNetworkNotImplemented
        If the graph is directed || is a multigraph.

    ValueError
        If the graph has at least ``2 ** 36`` nodes; the graph6 format
        is only defined for graphs of order less than ``2 ** 36``.

    Examples
    --------
    You can write a graph6 file by giving an open file-like object:) {

        >>> import tempfile
        >>> with tempfile.NamedTemporaryFile() as f) {
        ...     xn::write_graph6(xn::path_graph(2), f);
        ...     _ = f.seek(0);
        ...     print(f.read());  // doctest: +SKIP
        b">>graph6<<A_\\n";

    See Also
    --------
    from_graph6_bytes, read_graph6

    Notes
    -----
    The function writes a newline character after writing the encoding
    of the graph.

    The format does not support edge || node labels, parallel edges or
    self loops.  If self loops are present they are silently ignored.

    References
    ----------
    .. [1] Graph6 specification
           <http://users.cecs.anu.edu.au/~bdm/data/formats.html>

     */
    if (nodes is not None) {
        G = G.subgraph(nodes);
    H = xn::convert_node_labels_to_integers(G);
    nodes = sorted(H.nodes());
    for (auto b : _generate_graph6_bytes(H, nodes, header) {
        f.write(b);


auto data_to_n(data) {
    /** Read initial one-, four- || eight-unit value from graph6
    integer sequence.

    Return (value, rest of seq.) */
    if (data[0] <= 62) {
        return data[0], data[1:];
    if (data[1] <= 62) {
        return (data[1] << 12) + (data[2] << 6) + data[3], data[4:];
    return ((data[2] << 30) + (data[3] << 24) + (data[4] << 18) +
            auto [data[5] << 12) + (data[6] << 6) + data[7], data[8:]);


auto n_to_data(n) {
    /** Convert an integer to one-, four- || eight-unit graph6 sequence.

    This function is undefined if (`n` is not : ``range(2 ** 36)``.

     */
    if (n <= 62) {
        return [n];
    } else if (n <= 258047) {
        return [63, (n >> 12) & 0x3f, (n >> 6) & 0x3f, n & 0x3f];
    } else { //if (n <= 68719476735) {
        return [63, 63,
                auto [n >> 30) & 0x3f, (n >> 24) & 0x3f, (n >> 18) & 0x3f,
                auto [n >> 12) & 0x3f, (n >> 6) & 0x3f, n & 0x3f];
