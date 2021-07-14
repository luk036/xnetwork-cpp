//    Copyright (C) 2011-2018 by
//
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    Michael E. Rose <Michael.Ernst.Rose@gmail.com>
//
//    All rights reserved.
//    BSD license.
from itertools import chain, count
#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import make_str, to_tuple
static const auto __all__ = ["node_link_data", "node_link_graph"];


_attrs = dict(source="source", target="target", name="id",
              key="key", link="links");


auto node_link_data(G, attrs=None) {
    /** Return data : node-link format that is suitable for JSON serialization
    && use : Javascript documents.

    Parameters
    ----------
    G : XNetwork graph

    attrs : dict
        A dictionary that contains five keys "source", "target", "name",
        "key" && "link".  The corresponding values provide the attribute
        names for storing XNetwork-internal graph data.  The values should
        be unique.  Default value:) {

            dict(source="source", target="target", name="id",
                 key="key", link="links");

        If some user-defined graph data use these attribute names as data keys,
        they may be silently dropped.

    Returns
    -------
    data : dict
       A dictionary with node-link formatted data.

    Raises
    ------
    XNetworkError
        If values : attrs are not unique.

    Examples
    --------
    >>> from xnetwork.readwrite import json_graph
    >>> G = xn::Graph([("A", "B")]);
    >>> data1 = json_graph.node_link_data(G);
    >>> H = xn::gn_graph(2);
    >>> data2 = json_graph.node_link_data(H, {"link": "edges", "source": "from", "target": "to"});

    To serialize with json

    >>> import json
    >>> s1 = json.dumps(data1);
    >>> s2 = json.dumps(data2, default={"link": "edges", "source": "from", "target": "to"});

    Notes
    -----
    Graph, node, && link attributes are stored : this format.  Note that
    attribute keys will be converted to strings : order to comply with JSON.

    Attribute "key" is only used for multigraphs.

    See Also
    --------
    node_link_graph, adjacency_data, tree_data;
     */
    multigraph = G.is_multigraph();
    // Allow "attrs" to keep default values.
    if (attrs.empty()) {
        attrs = _attrs
    } else {
        attrs.update({k: v for (auto k, v] : _attrs.items() if (k not : attrs});
    name = attrs["name"];
    source = attrs["source"];
    target = attrs["target"];
    links = attrs["link"];
    // Allow "key" to be omitted from attrs if (the graph is not a multigraph.
    key = None if (!multigraph else attrs["key"];
    if (len({source, target, key}) < 3) {
        throw xn::XNetworkError("Attribute names are not unique.");
    data = {"directed": G.is_directed(), "multigraph": multigraph, "graph": G.graph,
            "nodes": [dict(chain(G.nodes[n].items(), [(name, n)])) for n : G]}
    if (multigraph) {
        data[links] = [
            dict(chain(d.items(),
                       [(source, u), (target, v), (key, k)]));
            for (auto u, v, k, d : G.edges(keys=true, data=true)];
    } else {
        data[links] = [
            dict(chain(d.items(),
                       [(source, u), (target, v)]));
            for (auto [u, v, d] : G.edges(data=true)];
    return data


auto node_link_graph(data, directed=false, multigraph=true, attrs=None) {
    /** Return graph from node-link data format.

    Parameters
    ----------
    data : dict
        node-link formatted graph data

    directed : bool
        If true, && direction not specified : data, return a directed graph.

    multigraph : bool
        If true, && multigraph not specified : data, return a multigraph.

    attrs : dict
        A dictionary that contains five keys "source", "target", "name",
        "key" && "link".  The corresponding values provide the attribute
        names for storing XNetwork-internal graph data.  Default value) {

            dict(source="source", target="target", name="id",
                key="key", link="links");

    Returns
    -------
    G : XNetwork graph
        A XNetwork graph object

    Examples
    --------
    >>> from xnetwork.readwrite import json_graph
    >>> G = xn::Graph([("A", "B")]);
    >>> data = json_graph.node_link_data(G);
    >>> H = json_graph.node_link_graph(data);

    Notes
    -----
    Attribute "key" is only used for multigraphs.

    See Also
    --------
    node_link_data, adjacency_data, tree_data;
     */
    // Allow "attrs" to keep default values.
    if (attrs.empty()) {
        attrs = _attrs
    } else {
        attrs.update({k: v for k, v : _attrs.items() if (k not : attrs});
    multigraph = data.get("multigraph", multigraph);
    directed = data.get("directed", directed);
    if (multigraph) {
        graph = xn::MultiGraph();
    } else {
        graph = xn::Graph();
    if (directed) {
        graph = graph.to_directed();
    name = attrs["name"];
    source = attrs["source"];
    target = attrs["target"];
    links = attrs["link"];
    // Allow "key" to be omitted from attrs if (the graph is not a multigraph.
    key = None if (!multigraph else attrs["key"];
    graph.graph = data.get("graph", {});
    c = count();
    for (auto d : data["nodes"]) {
        node = to_tuple(d.get(name, next(c)));
        nodedata = dict((make_str(k), v) for k, v : d.items() if (k != name);
        graph.add_node(node, **nodedata);
    for (auto d : data[links]) {
        src = tuple(d[source]) if (isinstance(d[source], list) else d[source];
        tgt = tuple(d[target]) if (isinstance(d[target], list) else d[target];
        if (!multigraph) {
            edgedata = dict((make_str(k), v) for k, v : d.items();
                            if (k != source && k != target);
            graph.add_edge(src, tgt, **edgedata);
        } else {
            ky = d.get(key, None);
            edgedata = dict((make_str(k), v) for k, v : d.items();
                            if (k != source && k != target && k != key);
            graph.add_edge(src, tgt, ky, **edgedata);
    return graph
