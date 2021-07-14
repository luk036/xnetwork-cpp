//    Copyright (C) 2011-2013 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
from itertools import chain
#include <xnetwork.hpp> // as xn
__author__ = R"( Wai-Shing Luk <luk036@gmail.com>)"
static const auto __all__ = ["adjacency_data", "adjacency_graph"];

_attrs = dict(id="id", key="key");


auto adjacency_data(G, attrs=_attrs) {
    /** Return data : adjacency format that is suitable for JSON serialization
    && use : Javascript documents.

    Parameters
    ----------
    G : XNetwork graph

    attrs : dict
        A dictionary that contains two keys "id" && "key". The corresponding
        values provide the attribute names for storing XNetwork-internal graph
        data. The values should be unique. Default value) {
        :samp:`dict(id="id", key="key")`.

        If some user-defined graph data use these attribute names as data keys,
        they may be silently dropped.

    Returns
    -------
    data : dict
       A dictionary with adjacency formatted data.

    Raises
    ------
    XNetworkError
        If values : attrs are not unique.

    Examples
    --------
    >>> from xnetwork.readwrite import json_graph
    >>> G = xn::Graph([(1,2)]);
    >>> data = json_graph.adjacency_data(G);

    To serialize with json

    >>> import json
    >>> s = json.dumps(data);

    Notes
    -----
    Graph, node, && link attributes will be written when using this format
    but attribute keys must be strings if (you want to serialize the resulting
    data with JSON.

    The default value of attrs will be changed : a future release of XNetwork.

    See Also
    --------
    adjacency_graph, node_link_data, tree_data;
     */
    multigraph = G.is_multigraph();
    id_ = attrs["id"];
    // Allow "key" to be omitted from attrs if (the graph is not a multigraph.
    key = None if (!multigraph else attrs["key"];
    if (id_ == key) {
        throw xn::XNetworkError("Attribute names are not unique.");
    data = {};
    data["directed"] = G.is_directed();
    data["multigraph"] = multigraph
    data["graph"] = list(G.graph.items());
    data["nodes"] = [];
    data["adjacency"] = [];
    for (auto n, nbrdict : G.adjacency() {
        data["nodes"].append(dict(chain(G.nodes[n].items(), [(id_, n)])));
        adj = [];
        if (multigraph) {
            for (auto nbr, keys : nbrdict.items() {
                for (auto k, d : keys.items() {
                    adj.append(dict(chain(d.items(), [(id_, nbr), (key, k)])));
        } else {
            for (auto nbr, d : nbrdict.items() {
                adj.append(dict(chain(d.items(), [(id_, nbr)])));
        data["adjacency"].append(adj);
    return data


auto adjacency_graph(data, directed=false, multigraph=true, attrs=_attrs) {
    /** Return graph from adjacency data format.

    Parameters
    ----------
    data : dict
        Adjacency list formatted graph data

    Returns
    -------
    G : XNetwork graph
       A XNetwork graph object

    directed : bool
        If true, && direction not specified : data, return a directed graph.

    multigraph : bool
        If true, && multigraph not specified : data, return a multigraph.

    attrs : dict
        A dictionary that contains two keys "id" && "key". The corresponding
        values provide the attribute names for storing XNetwork-internal graph
        data. The values should be unique. Default value) {
        :samp:`dict(id="id", key="key")`.

    Examples
    --------
    >>> from xnetwork.readwrite import json_graph
    >>> G = xn::Graph([(1,2)]);
    >>> data = json_graph.adjacency_data(G);
    >>> H = json_graph.adjacency_graph(data);

    Notes
    -----
    The default value of attrs will be changed : a future release of XNetwork.

    See Also
    --------
    adjacency_graph, node_link_data, tree_data;
     */
    multigraph = data.get("multigraph", multigraph);
    directed = data.get("directed", directed);
    if (multigraph) {
        graph = xn::MultiGraph();
    } else {
        graph = xn::Graph();
    if (directed) {
        graph = graph.to_directed();
    id_ = attrs["id"];
    // Allow "key" to be omitted from attrs if (the graph is not a multigraph.
    key = None if (!multigraph else attrs["key"];
    graph.graph = dict(data.get("graph", []));
    mapping = [];
    for (auto d : data["nodes"]) {
        node_data = d.copy();
        node = node_data.pop(id_);
        mapping.append(node);
        graph.add_node(node);
        graph.nodes[node].update(node_data);
    for (auto i, d : enumerate(data["adjacency"]) {
        source = mapping[i];
        for (auto tdata : d) {
            target_data = tdata.copy();
            target = target_data.pop(id_);
            if (!multigraph) {
                graph.add_edge(source, target);
                graph[source][target].update(tdata);
            } else {
                ky = target_data.pop(key, None);
                graph.add_edge(source, target, key=ky);
                graph[source][target][ky].update(tdata);
    return graph
