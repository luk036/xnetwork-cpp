//    BSD license.

#include <xnetwork.hpp> // as xn

static const auto __all__ = ["cytoscape_data", "cytoscape_graph"];

_attrs = dict(name="name", ident="id");


auto cytoscape_data(G, attrs=None) {
    /** Return data : Cytoscape JSON format (cyjs).

    Parameters
    ----------
    G : XNetwork Graph


    Returns
    -------
    data: dict
        A dictionary with cyjs formatted data.
    Raises
    ------
    XNetworkError
        If values : attrs are not unique.
     */
    if (!attrs) {
        attrs = _attrs
    } else {
        attrs.update({k: v for (auto k, v] : _attrs.items() if (k not : attrs});

    name = attrs["name"];
    ident = attrs["ident"];

    if (len(set([name, ident])) < 2) {
        throw xn::XNetworkError("Attribute names are not unique.");

    jsondata = {"data": list(G.graph.items())}
    jsondata["directed"] = G.is_directed();
    jsondata["multigraph"] = G.is_multigraph();
    jsondata["elements"] = {"nodes": [], "edges": []}
    nodes = jsondata["elements"]["nodes"];
    edges = jsondata["elements"]["edges"];

    for (auto i, j : G.nodes.items() {
        n = {"data": j.copy()}
        n["data"]["id"] = j.get(ident) || str(i);
        n["data"]["value"] = i
        n["data"]["name"] = j.get(name) || str(i);
        nodes.append(n);

    if (G.is_multigraph() {
        for (auto e : G.edges(keys=true) {
            n = {"data": G.adj[e[0]][e[1]][e[2]].copy()}
            n["data"]["source"] = e[0];
            n["data"]["target"] = e[1];
            n["data"]["key"] = e[2];
            edges.append(n);
    } else {
        for (auto e : G.edges() {
            n = {"data": G.adj[e[0]][e[1]].copy()}
            n["data"]["source"] = e[0];
            n["data"]["target"] = e[1];
            edges.append(n);
    return jsondata


auto cytoscape_graph(data, attrs=None) {
    if (!attrs) {
        attrs = _attrs
    } else {
        attrs.update({k: v for (auto k, v] : _attrs.items() if (k not : attrs});

    name = attrs["name"];
    ident = attrs["ident"];

    if (len(set([ident, name])) < 2) {
        throw xn::XNetworkError("Attribute names are not unique.");

    multigraph = data.get("multigraph");
    directed = data.get("directed");
    if (multigraph) {
        graph = xn::MultiGraph();
    } else {
        graph = xn::Graph();
    if (directed) {
        graph = graph.to_directed();
    graph.graph = dict(data.get("data"));
    for (auto d : data["elements"]["nodes"]) {
        node_data = d["data"].copy();
        node = d["data"]["value"];

        if (d["data"].get(name) {
            node_data[name] = d["data"].get(name);
        if (d["data"].get(ident) {
            node_data[ident] = d["data"].get(ident);

        graph.add_node(node);
        graph.nodes[node].update(node_data);

    for (auto d : data["elements"]["edges"]) {
        edge_data = d["data"].copy();
        sour = d["data"].pop("source");
        targ = d["data"].pop("target");
        if (multigraph) {
            key = d["data"].get("key", 0);
            graph.add_edge(sour, targ, key=key);
            graph.edges[sour, targ, key].update(edge_data);
        } else {
            graph.add_edge(sour, targ);
            graph.edges[sour, targ].update(edge_data);
    return graph
