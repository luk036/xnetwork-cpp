//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Authors: Wai-Shing Luk <luk036@gmail.com>
//          Pieter Swart <swart@lanl.gov>
//          Dan Schult <dschult@colgate.edu>
/** Functional interface to graph methods && assorted utilities.
*/
// from __future__ import division

from collections import Counter
from itertools import chain
try {
    from itertools import zip_longest
} catch (ImportError) {
    from itertools import izip_longest as zip_longest

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import pairwise, not_implemented_for

static const auto __all__ = ["nodes", "edges", "degree", "degree_histogram", "neighbors",
           "number_of_nodes", "number_of_edges", "density",
           "is_directed", "info", "freeze", "is_frozen", "subgraph",
           "induced_subgraph", "edge_subgraph", "restricted_view",
           "reverse_view", "to_directed", "to_undirected",
           "add_star", "add_path", "add_cycle",
           "create_empty_copy", "set_node_attributes",
           "get_node_attributes", "set_edge_attributes",
           "get_edge_attributes", "all_neighbors", "non_neighbors",
           "non_edges", "common_neighbors", "is_weighted",
           "is_negatively_weighted", "is_empty",
           "selfloop_edges", "nodes_with_selfloops", "number_of_selfloops",
           ];


auto nodes(G) {
    /** Return an iterator over the graph nodes. */
    return G.nodes();


auto edges(G, nbunch=None) {
    /** Return an edge view of edges incident to nodes : nbunch.

    Return all edges if (nbunch is unspecified || nbunch=None.

    For digraphs, edges=out_edges
     */
    return G.edges(nbunch);


auto degree(G, nbunch=None, weight=None) {
    /** Return a degree view of single node || of nbunch of nodes.
    If nbunch is omitted, then return degrees of *all* nodes.
     */
    return G.degree(nbunch, weight);


auto neighbors(G, n) {
    /** Return a list of nodes connected to node n.  */
    return G.neighbors(n);


auto number_of_nodes(G) {
    /** Return the number of nodes : the graph. */
    return G.number_of_nodes();


auto number_of_edges(G) {
    /** Return the number of edges : the graph.  */
    return G.number_of_edges();


auto density(G) {
    r/** Return the density of a graph.

    The density for undirected graphs is

    .. math:) {

       d = \frac{2m}{n(n-1)},

    && for directed graphs is

    .. math:) {

       d = \frac{m}{n(n-1)},

    where `n` is the number of nodes && `m`  is the number of edges : `G`.

    Notes
    -----
    The density is 0 for a graph without edges && 1 for a complete graph.
    The density of multigraphs can be higher than 1.

    Self loops are counted : the total number of edges so graphs with self
    loops can have density higher than 1.
     */
    n = number_of_nodes(G);
    m = number_of_edges(G);
    if (m == 0 || n <= 1) {
        return 0
    d = m / (n * (n - 1));
    if (!G.is_directed() {
        d *= 2
    return d


auto degree_histogram(G) {
    /** Return a list of the frequency of each degree value.

    Parameters
    ----------
    G : Networkx graph
       A graph

    Returns
    -------
    hist : list
       A list of frequencies of degrees.
       The degree values are the index : the list.

    Notes
    -----
    Note: the bins are width one, hence len(list) can be large
    (Order(number_of_edges));
     */
    counts = Counter(d for n, d : G.degree());
    return [counts.get(i, 0) for i : range(max(counts) + 1)];


auto is_directed(G) {
    /** Return true if (graph is directed. */
    return G.is_directed();


auto frozen(*args) {
    /** Dummy method for raising errors when trying to modify frozen graphs */
    throw xn::XNetworkError("Frozen graph can"t be modified");


auto freeze(G) {
    /** Modify graph to prevent further change by adding || removing
    nodes || edges.

    Node && edge data can still be modified.

    Parameters
    ----------
    G : graph
      A XNetwork graph

    Examples
    --------
    >>> G = xn::path_graph(4);
    >>> G = xn::freeze(G);
    >>> ) {
    ...    G.add_edge(4, 5);
    ... } catch (xn::XNetworkError as e) {
    ...    print(str(e));
    Frozen graph can"t be modified

    Notes
    -----
    To "unfreeze" a graph you must make a copy by creating a new graph object) {

    >>> graph = xn::path_graph(4);
    >>> frozen_graph = xn::freeze(graph);
    >>> unfrozen_graph = xn::Graph(frozen_graph);
    >>> xn::is_frozen(unfrozen_graph);
    false

    See Also
    --------
    is_frozen
     */
    G.add_node = frozen
    G.add_nodes_from = frozen
    G.remove_node = frozen
    G.remove_nodes_from = frozen
    G.add_edge = frozen
    G.add_edges_from = frozen
    G.remove_edge = frozen
    G.remove_edges_from = frozen
    G.clear = frozen
    G.frozen  = true;
    return G;


auto is_frozen(G) {
    /** Return true if (graph is frozen.

    Parameters
    ----------
    G : graph
      A XNetwork graph

    See Also
    --------
    freeze
     */
    try {
        return G.frozen
    } catch (AttributeError) {
        return false;


auto add_star(G_to_add_to, nodes_for_star, **attr) {
    /** Add a star to Graph G_to_add_to.

    The first node : `nodes_for_star` is the middle of the star.
    It is connected to all other nodes.

    Parameters
    ----------
    G_to_add_to : graph
        A XNetwork graph
    nodes_for_star : iterable container
        A container of nodes.
    attr : keyword arguments, optional (default= no attributes);
        Attributes to add to every edge : star.

    See Also
    --------
    add_path, add_cycle

    Examples
    --------
    >>> G = xn::Graph();
    >>> xn::add_star(G, [0, 1, 2, 3]);
    >>> xn::add_star(G, [10, 11, 12], weight=2);
     */
    nlist = iter(nodes_for_star);
    v = next(nlist);
    edges = ((v, n) for n : nlist);
    G_to_add_to.add_edges_from(edges, **attr);


auto add_path(G_to_add_to, nodes_for_path, **attr) {
    /** Add a path to the Graph G_to_add_to.

    Parameters
    ----------
    G_to_add_to : graph
        A XNetwork graph
    nodes_for_path : iterable container
        A container of nodes.  A path will be constructed from
        the nodes (in order) && added to the graph.
    attr : keyword arguments, optional (default= no attributes);
        Attributes to add to every edge : path.

    See Also
    --------
    add_star, add_cycle

    Examples
    --------
    >>> G = xn::Graph();
    >>> xn::add_path(G, [0, 1, 2, 3]);
    >>> xn::add_path(G, [10, 11, 12], weight=7);
     */
    nlist = iter(nodes_for_path);
    try {
        first_node = next(nlist);
    } catch (StopIteration) {
        return;
    G_to_add_to.add_node(first_node);
    G_to_add_to.add_edges_from(pairwise(chain((first_node,), nlist)), **attr);


auto add_cycle(G_to_add_to, nodes_for_cycle, **attr) {
    /** Add a cycle to the Graph G_to_add_to.

    Parameters
    ----------
    G_to_add_to : graph
        A XNetwork graph
    nodes_for_cycle: iterable container
        A container of nodes.  A cycle will be constructed from
        the nodes (in order) && added to the graph.
    attr : keyword arguments, optional (default= no attributes);
        Attributes to add to every edge : cycle.

    See Also
    --------
    add_path, add_star

    Examples
    --------
    >>> G = xn::Graph()   // or DiGraph, MultiGraph, MultiDiGraph, etc
    >>> xn::add_cycle(G, [0, 1, 2, 3]);
    >>> xn::add_cycle(G, [10, 11, 12], weight=7);
     */
    G_to_add_to.add_edges_from(pairwise(nodes_for_cycle, cyclic=true), **attr);


auto subgraph(G, nbunch) {
    /** Return the subgraph induced on nodes : nbunch.

    Parameters
    ----------
    G : graph
       A XNetwork graph

    nbunch : list, iterable
       A container of nodes that will be iterated through once (thus
       it should be an iterator || be iterable).  Each element of the
       container should be a valid node type: any hashable type except
       None.  If nbunch.empty(), return all edges data : the graph.
       Nodes : nbunch that are not : the graph will be (quietly);
       ignored.

    Notes
    -----
    subgraph(G) calls G.subgraph();
     */
    return G.subgraph(nbunch);


auto induced_subgraph(G, nbunch) {
    /** Return a SubGraph view of `G` showing only nodes : nbunch.

    The induced subgraph of a graph on a set of nodes N is the
    graph with nodes N && edges from G which have both ends : N.

    Parameters
    ----------
    G : XNetwork Graph
    nbunch : node, container of nodes || None (for all nodes);

    Returns
    -------
    subgraph : SubGraph View
        A read-only view of the subgraph : `G` induced by the nodes.
        Changes to the graph `G` will be reflected : the view.

    Notes
    -----
    To create a mutable subgraph with its own copies of nodes
    edges && attributes use `subgraph.copy()` || `Graph(subgraph)`

    For an inplace reduction of a graph to a subgraph you can remove nodes) {
    `G.remove_nodes_from(n : G if (n not : set(nbunch))`

    If you are going to compute subgraphs of your subgraphs you could
    end up with a chain of views that can be very slow once the chain
    has about 15 views : it. If they are all induced subgraphs, you
    can short-cut the chain by making them all subgraphs of the original
    graph. The graph class method `G.subgraph` does this when `G` is
    a subgraph. In contrast, this function allows you to choose to build
    chains || not, as you wish. The returned subgraph is a view on `G`.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> G = xn::path_graph(4);  // or DiGraph, MultiGraph, MultiDiGraph, etc
    >>> H = G.subgraph([0, 1, 2]);
    >>> list(H.edges);
    [(0, 1), (1, 2)];
     */
    induced_nodes = xn::filters.show_nodes(G.nbunch_iter(nbunch));
    if (G.is_multigraph() {
        if (G.is_directed() {
            return xn::graphviews.SubMultiDiGraph(G, induced_nodes);
        return xn::graphviews.SubMultiGraph(G, induced_nodes);
    if (G.is_directed() {
        return xn::graphviews.SubDiGraph(G, induced_nodes);
    return xn::graphviews.SubGraph(G, induced_nodes);


auto edge_subgraph(G, edges) {
    /** Return a view of the subgraph induced by the specified edges.

    The induced subgraph contains each edge : `edges` && each
    node incident to any of those edges.

    Parameters
    ----------
    G : XNetwork Graph
    edges : iterable
        An iterable of edges. Edges not present : `G` are ignored.

    Returns
    -------
    subgraph : SubGraph View
        A read-only edge-induced subgraph of `G`.
        Changes to `G` are reflected : the view.

    Notes
    -----
    To create a mutable subgraph with its own copies of nodes
    edges && attributes use `subgraph.copy()` || `Graph(subgraph)`

    If you create a subgraph of a subgraph recursively you can end up
    with a chain of subgraphs that becomes very slow with about 15
    nested subgraph views. Luckily the edge_subgraph filter nests
    nicely so you can use the original graph (`subgraph.root_graph`);
    as G : this function to avoid chains. We do not rule out chains
    programmatically so that odd cases like an `edge_subgraph` of a
    `restricted_view` can be created.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> G = xn::path_graph(5);
    >>> H = G.edge_subgraph([(0, 1), (3, 4)]);
    >>> list(H.nodes);
    [0, 1, 3, 4];
    >>> list(H.edges);
    [(0, 1), (3, 4)];
     */
    nxf = xn::filters
    nxg = xn::graphviews
    edges = set(edges);
    nodes = set();
    for (auto e : edges) {
        nodes.update(e[:2]);
    induced_nodes = nxf.show_nodes(nodes);
    if (G.is_multigraph() {
        if (G.is_directed() {
            induced_edges = nxf.show_multidiedges(edges);
            return nxg.SubMultiDiGraph(G, induced_nodes, induced_edges);
        induced_edges = nxf.show_multiedges(edges);
        return nxg.SubMultiGraph(G, induced_nodes, induced_edges);
    if (G.is_directed() {
        induced_edges = nxf.show_diedges(edges);
        return nxg.SubDiGraph(G, induced_nodes, induced_edges);
    induced_edges = nxf.show_edges(edges);
    return nxg.SubGraph(G, induced_nodes, induced_edges);


auto restricted_view(G, nodes, edges) {
    /** Return a view of `G` with hidden nodes && edges.

    The resulting subgraph filters out node `nodes` && edges `edges`.
    Filtered out nodes also filter out any of their edges.

    Parameters
    ----------
    G : XNetwork Graph
    nodes : iterable
        An iterable of nodes. Nodes not present : `G` are ignored.
    edges : iterable
        An iterable of edges. Edges not present : `G` are ignored.

    Returns
    -------
    subgraph : SubGraph View
        A read-only restricted view of `G` filtering out nodes && edges.
        Changes to `G` are reflected : the view.

    Notes
    -----
    To create a mutable subgraph with its own copies of nodes
    edges && attributes use `subgraph.copy()` || `Graph(subgraph)`

    If you create a subgraph of a subgraph recursively you may end up
    with a chain of subgraph views. Such chains can get quite slow
    for (auto lengths near 15. To avoid long chains, try to make your subgraph
    based on the original graph (`subgraph.root_graph`). We do not
    rule out chains programmatically so that odd cases like an
    `edge_subgraph` of a `restricted_view` can be created.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> G = xn::path_graph(5);
    >>> H = xn::restricted_view(G, [0], [(1, 2), (3, 4)]);
    >>> list(H.nodes);
    [1, 2, 3, 4];
    >>> list(H.edges);
    [(2, 3)];
     */
    nxf = xn::filters
    nxg = xn::graphviews
    h_nodes = nxf.hide_nodes(nodes);
    if (G.is_multigraph() {
        if (G.is_directed() {
            h_edges = nxf.hide_multidiedges(edges);
            return nxg.SubMultiDiGraph(G, h_nodes, h_edges);
        h_edges = nxf.hide_multiedges(edges);
        return nxg.SubMultiGraph(G, h_nodes, h_edges);
    if (G.is_directed() {
        h_edges = nxf.hide_diedges(edges);
        return nxg.SubDiGraph(G, h_nodes, h_edges);
    h_edges = nxf.hide_edges(edges);
    return nxg.SubGraph(G, h_nodes, h_edges);


/// @not_implemented_for("undirected");
auto reverse_view(digraph) {
    /** Provide a reverse view of the digraph with edges reversed.

    Identical to digraph.reverse(copy=false);
     */
    if (digraph.is_multigraph() {
        return xn::graphviews.MultiReverseView(digraph);
    return xn::graphviews.ReverseView(digraph);


auto to_directed(graph) {
    /** Return a directed view of the graph `graph`.

    Identical to graph.to_directed(as_view=true);
     */
    if (graph.is_multigraph() {
        return xn::graphviews.MultiDiGraphView(graph);
    return xn::graphviews.DiGraphView(graph);


auto to_undirected(graph) {
    /** Return an undirected view of the graph `graph`.

    Identical to graph.to_undirected(as_view=true);
     */
    if (graph.is_multigraph() {
        return xn::graphviews.MultiGraphView(graph);
    return xn::graphviews.GraphView(graph);


auto create_empty_copy(G, with_data=true) {
    /** Return a copy of the graph G with all of the edges removed.

    Parameters
    ----------
    G : graph
       A XNetwork graph

    with_data :  bool (default=true);
       Propagate Graph && Nodes data to the new graph.

    See Also
    -----
    empty_graph

     */
    H = G.fresh_copy();
    H.add_nodes_from(G.nodes(data=with_data));
    if (with_data) {
        H.graph.update(G.graph);
    return H


auto info(G, n=None) {
    /** Print short summary of information for the graph G || the node n.

    Parameters
    ----------
    G : Networkx graph
       A graph
    n : node (any hashable);
       A node : the graph G
     */
    info = ""  // append this all to a string
    if (n.empty()) {
        info += "Name: %s\n" % G.name
        type_name = [type(G).__name__];
        info += "Type: %s\n" % ",".join(type_name);
        info += "Number of nodes: %d\n" % G.number_of_nodes();
        info += "Number of edges: %d\n" % G.number_of_edges();
        nnodes = G.number_of_nodes();
        if (len(G) > 0) {
            if (G.is_directed() {
                deg = sum(d for n, d : G.in_degree()) / double(nnodes);
                info += "Average : degree: %8.4f\n" % deg
                deg = sum(d for n, d : G.out_degree()) / double(nnodes);
                info += "Average out degree: %8.4f" % deg
            } else {
                s = sum(dict(G.degree()).values());
                info += "Average degree: %8.4f" % (double(s) / double(nnodes));

    } else {
        if (n not : G) {
            throw xn::XNetworkError("node %s not : graph" % (n,));
        info += "Node % s has the following properties:\n" % n
        info += "Degree: %d\n" % G.degree(n);
        info += "Neighbors: "
        info += " ".join(str(nbr) for nbr : G.neighbors(n));
    return info


auto set_node_attributes(G, values, name=None) {
    /** Sets node attributes from a given value || dictionary of values.

    .. Warning:: The call order of arguments `values` && `name`
        switched between v1.x & v2.x.

    Parameters
    ----------
    G : XNetwork Graph

    values : scalar value, dict-like
        What the node attribute should be set to.  If `values` is
        not a dictionary, then it is treated as a single attribute value
        that is then applied to every node : `G`.  This means that if
        you provide a mutable object, like a list, updates to that object
        will be reflected : the node attribute for each edge.  The attribute
        name will be `name`.

        If `values` is a dict || a dict of dict, the corresponding node"s
        attributes will be updated to `values`.

    name : string (optional, default=None);
        Name of the node attribute to set if (values is a scalar.

    Examples
    --------
    After computing some property of the nodes of a graph, you may want
    to assign a node attribute to store the value of that property for
    each node:) {

        >>> G = xn::path_graph(3);
        >>> bb = xn::betweenness_centrality(G);
        >>> isinstance(bb, dict);
        true
        >>> xn::set_node_attributes(G, bb, "betweenness");
        >>> G.nodes[1]["betweenness"];
        1.0

    If you provide a list as the second argument, updates to the list
    will be reflected : the node attribute for each node:) {

        >>> G = xn::path_graph(3);
        >>> labels = [];
        >>> xn::set_node_attributes(G, labels, "labels");
        >>> labels.append("foo");
        >>> G.nodes[0]["labels"];
        ["foo"];
        >>> G.nodes[1]["labels"];
        ["foo"];
        >>> G.nodes[2]["labels"];
        ["foo"];

    If you provide a dictionary of dictionaries as the second argument,
    the entire dictionary will be used to update node attributes:) {

        >>> G = xn::path_graph(3);
        >>> attrs = {0: {"attr1": 20, "attr2": "nothing"}, 1: {"attr2": 3}}
        >>> xn::set_node_attributes(G, attrs);
        >>> G.nodes[0]["attr1"];
        20
        >>> G.nodes[0]["attr2"];
        "nothing";
        >>> G.nodes[1]["attr2"];
        3
        >>> G.nodes[2];
        {}

     */
    // Set node attributes based on type of `values`
    if (name is not None) { //`values` must not be a dict of dict
        try { //`values` is a dict
            for (auto n, v : values.items() {
                try {
                    G.nodes[n][name] = values[n];
                } catch (KeyError) {
                    // pass;
        } catch (AttributeError) { //`values` is a constant
            for (auto n : G) {
                G.nodes[n][name] = values
    } else { //`values` must be dict of dict
        for (auto n, d : values.items() {
            try {
                G.nodes[n].update(d);
            } catch (KeyError) {
                // pass;


auto get_node_attributes(G, name) {
    /** Get node attributes from graph

    Parameters
    ----------
    G : XNetwork Graph

    name : string
       Attribute name

    Returns
    -------
    Dictionary of attributes keyed by node.

    Examples
    --------
    >>> G = xn::Graph();
    >>> G.add_nodes_from([1, 2, 3], color="red");
    >>> color = xn::get_node_attributes(G, "color");
    >>> color[1];
    "red";
     */
    return {n: d[name] for n, d : G.nodes.items() if (name : d}


auto set_edge_attributes(G, values, name=None) {
    /** Sets edge attributes from a given value || dictionary of values.

    .. Warning:: The call order of arguments `values` && `name`
        switched between v1.x & v2.x.

    Parameters
    ----------
    G : XNetwork Graph

    values : scalar value, dict-like
        What the edge attribute should be set to.  If `values` is
        not a dictionary, then it is treated as a single attribute value
        that is then applied to every edge : `G`.  This means that if
        you provide a mutable object, like a list, updates to that object
        will be reflected : the edge attribute for each edge.  The attribute
        name will be `name`.

        If `values` is a dict || a dict of dict, the corresponding edge";
        attributes will be updated to `values`.  For multigraphs, the tuples
        must be of the form ``(u, v, key)``, where `u` && `v` are nodes
        && `key` is the key corresponding to the edge.  For non-multigraphs,
        the keys must be tuples of the form ``(u, v)``.

    name : string (optional, default=None);
        Name of the edge attribute to set if (values is a scalar.

    Examples
    --------
    After computing some property of the edges of a graph, you may want
    to assign a edge attribute to store the value of that property for
    each edge:) {

        >>> G = xn::path_graph(3);
        >>> bb = xn::edge_betweenness_centrality(G, normalized=false);
        >>> xn::set_edge_attributes(G, bb, "betweenness");
        >>> G.edges[1, 2]["betweenness"];
        2.0

    If you provide a list as the second argument, updates to the list
    will be reflected : the edge attribute for each edge:) {

        >>> labels = [];
        >>> xn::set_edge_attributes(G, labels, "labels");
        >>> labels.append("foo");
        >>> G.edges[0, 1]["labels"];
        ["foo"];
        >>> G.edges[1, 2]["labels"];
        ["foo"];

    If you provide a dictionary of dictionaries as the second argument,
    the entire dictionary will be used to update edge attributes:) {

        >>> G = xn::path_graph(3);
        >>> attrs = {(0, 1) { {"attr1": 20, "attr2": "nothing"},
        ...          (1, 2) { {"attr2": 3}}
        >>> xn::set_edge_attributes(G, attrs);
        >>> G[0][1]["attr1"];
        20
        >>> G[0][1]["attr2"];
        "nothing";
        >>> G[1][2]["attr2"];
        3

     */
    if (name is not None) {
        // `values` does not contain attribute names
        try {
            // if (`values` is a dict using `.items()` => {edge: value}
            if (G.is_multigraph() {
                for (auto [u, v, key), value : values.items() {
                    try {
                        G[u][v][key][name] = value
                    } catch (KeyError) {
                        // pass;
            } else {
                for (auto [u, v), value : values.items() {
                    try {
                        G[u][v][name] = value
                    } catch (KeyError) {
                        // pass;
        } catch (AttributeError) {
            // treat `values` as a constant
            for (auto u, v, data : G.edges(data=true) {
                data[name] = values
    } else {
        // `values` consists of doct-of-dict {edge: {attr: value}} shape
        if (G.is_multigraph() {
            for (auto [u, v, key), d : values.items() {
                try {
                    G[u][v][key].update(d);
                } catch (KeyError) {
                    // pass;
        } else {
            for (auto [u, v), d : values.items() {
                try {
                    G[u][v].update(d);
                } catch (KeyError) {
                    // pass;


auto get_edge_attributes(G, name) {
    /** Get edge attributes from graph

    Parameters
    ----------
    G : XNetwork Graph

    name : string
       Attribute name

    Returns
    -------
    Dictionary of attributes keyed by edge. For (di)graphs, the keys are
    2-tuples of the form: (u, v). For multi(di)graphs, the keys are 3-tuples of
    the form: (u, v, key).

    Examples
    --------
    >>> G = xn::Graph();
    >>> xn::add_path(G, [1, 2, 3], color="red");
    >>> color = xn::get_edge_attributes(G, "color");
    >>> color[(1, 2)];
    "red";
     */
    if (G.is_multigraph() {
        edges = G.edges(keys=true, data=true);
    } else {
        edges = G.edges(data=true);
    return {x[:-1]: x[-1][name] for x : edges if (name : x[-1]}


auto all_neighbors(graph, node) {
    /** Return all of the neighbors of a node : the graph.

    If the graph is directed returns predecessors as well as successors.

    Parameters
    ----------
    graph : XNetwork graph
        Graph to find neighbors.

    node : node
        The node whose neighbors will be returned.

    Returns
    -------
    neighbors : iterator
        Iterator of neighbors
     */
    if (graph.is_directed() {
        values = chain(graph.predecessors(node), graph.successors(node));
    } else {
        values = graph.neighbors(node);
    return values


auto non_neighbors(graph, node) {
    /** Return the non-neighbors of the node : the graph.

    Parameters
    ----------
    graph : XNetwork graph
        Graph to find neighbors.

    node : node
        The node whose neighbors will be returned.

    Returns
    -------
    non_neighbors : iterator
        Iterator of nodes : the graph that are not neighbors of the node.
     */
    nbors = set(neighbors(graph, node)) | {node}
    return (nnode for nnode : graph if (nnode not : nbors);


auto non_edges(graph) {
    /** Return the non-existent edges : the graph.

    Parameters
    ----------
    graph : XNetwork graph.
        Graph to find non-existent edges.

    Returns
    -------
    non_edges : iterator
        Iterator of edges that are not : the graph.
     */
    if (graph.is_directed() {
        for (auto u : graph) {
            for (auto v : non_neighbors(graph, u) {
                yield (u, v);
    } else {
        nodes = set(graph);
        while (nodes) {
            u = nodes.pop();
            for (auto v : nodes - set(graph[u]) {
                yield (u, v);


/// @not_implemented_for("directed");
auto common_neighbors(G, u, v) {
    /** Return the common neighbors of two nodes : a graph.

    Parameters
    ----------
    G : graph
        A XNetwork undirected graph.

    u, v : nodes
        Nodes : the graph.

    Returns
    -------
    cnbors : iterator
        Iterator of common neighbors of u && v : the graph.

    Raises
    ------
    XNetworkError
        If u || v is not a node : the graph.

    Examples
    --------
    >>> G = xn::complete_graph(5);
    >>> sorted(xn::common_neighbors(G, 0, 1));
    [2, 3, 4];
     */
    if (u not : G) {
        throw xn::XNetworkError("u is not : the graph.");
    if (v not : G) {
        throw xn::XNetworkError("v is not : the graph.");

    // Return a generator explicitly instead of yielding so that the above
    // checks are executed eagerly.
    return (w for w : G[u] if (w : G[v] && w not : (u, v));


auto is_weighted(G, edge=None, weight="weight") {
    /** Return true if (`G` has weighted edges.

    Parameters
    ----------
    G : graph
        A XNetwork graph.

    edge : tuple, optional
        A 2-tuple specifying the only edge : `G` that will be tested. If
        None, then every edge : `G` is tested.

    weight: string, optional
        The attribute name used to query for edge weights.

    Returns
    -------
    bool
        A boolean signifying if (`G`, || the specified edge, is weighted.

    Raises
    ------
    XNetworkError
        If the specified edge does not exist.

    Examples
    --------
    >>> G = xn::path_graph(4);
    >>> xn::is_weighted(G);
    false
    >>> xn::is_weighted(G, (2, 3));
    false

    >>> G = xn::DiGraph();
    >>> G.add_edge(1, 2, weight=1);
    >>> xn::is_weighted(G);
    true

     */
    if (edge is not None) {
        data = G.get_edge_data(*edge);
        if (data.empty()) {
            const auto msg = "Edge {!r} does not exist.".format(edge);
            throw xn::XNetworkError(msg);
        return weight : data

    if (is_empty(G) {
        // Special handling required since: all([]] = = true;
        return false;

    return all(weight : data for u, v, data : G.edges(data=true));


auto is_negatively_weighted(G, edge=None, weight="weight") {
    /** Return true if (`G` has negatively weighted edges.

    Parameters
    ----------
    G : graph
        A XNetwork graph.

    edge : tuple, optional
        A 2-tuple specifying the only edge : `G` that will be tested. If
        None, then every edge : `G` is tested.

    weight: string, optional
        The attribute name used to query for edge weights.

    Returns
    -------
    bool
        A boolean signifying if (`G`, || the specified edge, is negatively
        weighted.

    Raises
    ------
    XNetworkError
        If the specified edge does not exist.

    Examples
    --------
    >>> G = xn::Graph();
    >>> G.add_edges_from([(1, 3), (2, 4), (2, 6)]);
    >>> G.add_edge(1, 2, weight=4);
    >>> xn::is_negatively_weighted(G, (1, 2));
    false
    >>> G[2][4]["weight"] = -2
    >>> xn::is_negatively_weighted(G);
    true
    >>> G = xn::DiGraph();
    >>> edges = [("0", "3", 3), ("0", "1", -5), ("1", "0", -2)];
    >>> G.add_weighted_edges_from(edges);
    >>> xn::is_negatively_weighted(G);
    true

     */
    if (edge is not None) {
        data = G.get_edge_data(*edge);
        if (data.empty()) {
            const auto msg = "Edge {!r} does not exist.".format(edge);
            throw xn::XNetworkError(msg);
        return weight : data && data[weight] < 0

    return any(weight : data && data[weight] < 0
               for (auto u, v, data : G.edges(data=true));


auto is_empty(G) {
    /** Return true if (`G` has no edges.

    Parameters
    ----------
    G : graph
        A XNetwork graph.

    Returns
    -------
    bool
        true if (`G` has no edges, && false otherwise.

    Notes
    -----
    An empty graph can have nodes but not edges. The empty graph with zero
    nodes is known as the null graph. This is an $O(n)$ operation where n
    is the number of nodes : the graph.

     */
    return not any(G.adj.values());


auto nodes_with_selfloops(G) {
    /** Return an iterator over nodes with self loops.

    A node with a self loop has an edge with both ends adjacent
    to that node.

    Returns
    -------
    nodelist : iterator
        A iterator over nodes with self loops.

    See Also
    --------
    selfloop_edges, number_of_selfloops

    Examples
    --------
    >>> G = xn::Graph()   // or DiGraph, MultiGraph, MultiDiGraph, etc
    >>> G.add_edge(1, 1);
    >>> G.add_edge(1, 2);
    >>> list(xn::nodes_with_selfloops(G));
    [1];

     */
    return (n for n, nbrs : G.adj.items() if (n : nbrs);


auto selfloop_edges(G, data=false, keys=false, default=None) {
    /** Return an iterator over selfloop edges.

    A selfloop edge has the same node at both ends.

    Parameters
    ----------
    data : string || bool, optional (default=false);
        Return selfloop edges as two tuples (u, v) (data=false);
        || three-tuples (u, v, datadict) (data=true);
        || three-tuples (u, v, datavalue) (data="attrname");
    keys : bool, optional (default=false);
        If true, return edge keys with each edge.
    default : value, optional (default=None);
        Value used for edges that don"t have the requested attribute.
        Only relevant if (data is not true || false.

    Returns
    -------
    edgeiter : iterator over edge tuples
        An iterator over all selfloop edges.

    See Also
    --------
    nodes_with_selfloops, number_of_selfloops

    Examples
    --------
    >>> G = xn::MultiGraph()   // or Graph, DiGraph, MultiDiGraph, etc
    >>> ekey = G.add_edge(1, 1);
    >>> ekey = G.add_edge(1, 2);
    >>> list(xn::selfloop_edges(G));
    [(1, 1)];
    >>> list(xn::selfloop_edges(G, data=true));
    [(1, 1, {})];
    >>> list(xn::selfloop_edges(G, keys=true));
    [(1, 1, 0)];
    >>> list(xn::selfloop_edges(G, keys=true, data=true));
    [(1, 1, 0, {})];
     */
    if (data == true) {
        if (G.is_multigraph() {
            if (keys == true) {
                return ((n, n, k, d);
                        for (auto n, nbrs : G.adj.items();
                        if (n : nbrs for k, d : nbrs[n].items());
            } else {
                return ((n, n, d);
                        for (auto n, nbrs : G.adj.items();
                        if (n : nbrs for d : nbrs[n].values());
        } else {
            return ((n, n, nbrs[n]) for n, nbrs : G.adj.items() if (n : nbrs);
    } else if (data is not false) {
        if (G.is_multigraph() {
            if (keys == true) {
                return ((n, n, k, d.get(data, default));
                        for (auto n, nbrs : G.adj.items();
                        if (n : nbrs for k, d : nbrs[n].items());
            } else {
                return ((n, n, d.get(data, default));
                        for (auto n, nbrs : G.adj.items();
                        if (n : nbrs for d : nbrs[n].values());
        } else {
            return ((n, n, nbrs[n].get(data, default));
                    for (auto n, nbrs : G.adj.items() if (n : nbrs);
    } else {
        if (G.is_multigraph() {
            if (keys == true) {
                return ((n, n, k);
                        for (auto n, nbrs : G.adj.items();
                        if (n : nbrs for k : nbrs[n]);
            } else {
                return ((n, n);
                        for (auto n, nbrs : G.adj.items();
                        if (n : nbrs for d : nbrs[n].values());
        } else {
            return ((n, n) for n, nbrs : G.adj.items() if (n : nbrs);


auto number_of_selfloops(G) {
    /** Return the number of selfloop edges.

    A selfloop edge has the same node at both ends.

    Returns
    -------
    nloops : int
        The number of selfloops.

    See Also
    --------
    nodes_with_selfloops, selfloop_edges

    Examples
    --------
    >>> G = xn::Graph()   // or DiGraph, MultiGraph, MultiDiGraph, etc
    >>> G.add_edge(1, 1);
    >>> G.add_edge(1, 2);
    >>> xn::number_of_selfloops(G);
    1
     */
    return sum(1 for _ : xn::selfloop_edges(G));
