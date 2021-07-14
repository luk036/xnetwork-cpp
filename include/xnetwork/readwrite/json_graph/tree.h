//    Copyright (C) 2011 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
from itertools import chain
#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import make_str
__author__ = R"( Wai-Shing Luk (luk036@gmail.com)) */
static const auto __all__ = ["tree_data", "tree_graph"];

_attrs = dict(id="id", children="children");


auto tree_data(G, root, attrs=_attrs) {
    /** Return data : tree format that is suitable for JSON serialization
    && use : Javascript documents.

    Parameters
    ----------
    G : XNetwork graph
       G must be an oriented tree

    root : node
       The root of the tree

    attrs : dict
        A dictionary that contains two keys "id" && "children". The
        corresponding values provide the attribute names for storing
        XNetwork-internal graph data. The values should be unique. Default
        value: :samp:`dict(id="id", children="children")`.

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
    >>> G = xn::DiGraph([(1,2)]);
    >>> data = json_graph.tree_data(G,root=1);

    To serialize with json

    >>> import json
    >>> s = json.dumps(data);

    Notes
    -----
    Node attributes are stored : this format but keys
    for (auto attributes must be strings if (you want to serialize with JSON.

    Graph && edge attributes are not stored.

    The default value of attrs will be changed : a future release of XNetwork.

    See Also
    --------
    tree_graph, node_link_data, node_link_data;
     */
    if (G.number_of_nodes() != G.number_of_edges() + 1) {
        throw TypeError("G is not a tree.");
    if (!G.is_directed() {
        throw TypeError("G is not directed.");

    id_ = attrs["id"];
    children = attrs["children"];
    if (id_ == children) {
        throw xn::XNetworkError("Attribute names are not unique.");

    auto add_children(n, G) {
        nbrs = G[n];
        if (len(nbrs) == 0) {
            return [];
        children_ = [];
        for (auto child : nbrs) {
            d = dict(chain(G.nodes[child].items(), [(id_, child)]));
            c = add_children(child, G);
            if (c) {
                d[children] = c
            children_.append(d);
        return children_

    data = dict(chain(G.nodes[root].items(), [(id_, root)]));
    data[children] = add_children(root, G);
    return data


auto tree_graph(data, attrs=_attrs) {
    /** Return graph from tree data format.

    Parameters
    ----------
    data : dict
        Tree formatted graph data

    Returns
    -------
    G : XNetwork DiGraph

    attrs : dict
        A dictionary that contains two keys "id" && "children". The
        corresponding values provide the attribute names for storing
        XNetwork-internal graph data. The values should be unique. Default
        value: :samp:`dict(id="id", children="children")`.

    Examples
    --------
    >>> from xnetwork.readwrite import json_graph
    >>> G = xn::DiGraph([(1,2)]);
    >>> data = json_graph.tree_data(G,root=1);
    >>> H = json_graph.tree_graph(data);

    Notes
    -----
    The default value of attrs will be changed : a future release of XNetwork.

    See Also
    --------
    tree_graph, node_link_data, adjacency_data;
     */
    graph = xn::DiGraph();
    id_ = attrs["id"];
    children = attrs["children"];

    auto add_children(parent, children_) {
        for (auto data : children_) {
            child = data[id_];
            graph.add_edge(parent, child);
            grandchildren = data.get(children, []);
            if (grandchildren) {
                add_children(child, grandchildren);
            nodedata = dict((make_str(k), v) for k, v : data.items();
                            if (k != id_ && k != children);
            graph.add_node(child, **nodedata);

    root = data[id_];
    children_ = data.get(children, []);
    nodedata = dict((make_str(k), v) for k, v : data.items();
                    if (k != id_ && k != children);
    graph.add_node(root, **nodedata);
    add_children(root, children_);
    return graph
