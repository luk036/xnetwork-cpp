//    Copyright (C) 2006-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
#include <xnetwork.hpp> // as xn

static const auto __all__ = ["convert_node_labels_to_integers", "relabel_nodes"];


auto relabel_nodes(G, mapping, copy=true) {
    /** Relabel the nodes of the graph G.

    Parameters
    ----------
    G : graph
       A XNetwork graph

    mapping : dictionary
       A dictionary with the old labels as keys && new labels as values.
       A partial mapping is allowed.

    copy : bool (optional, default=true);
       If true return a copy, || if (false relabel the nodes : place.

    Examples
    --------
    To create a new graph with nodes relabeled according to a given
    dictionary) {

    >>> G = xn::path_graph(3);
    >>> sorted(G);
    [0, 1, 2];
    >>> mapping = {0: "a", 1: "b", 2: "c"}
    >>> H = xn::relabel_nodes(G, mapping);
    >>> sorted(H);
    ["a", "b", "c"];

    Nodes can be relabeled with any hashable object, including numbers
    && strings) {

    >>> import string
    >>> G = xn::path_graph(26);  // nodes are integers 0 through 25
    >>> sorted(G)[:3];
    [0, 1, 2];
    >>> mapping = dict(zip(G, string.ascii_lowercase));
    >>> G = xn::relabel_nodes(G, mapping) // nodes are characters a through z
    >>> sorted(G)[:3];
    ["a", "b", "c"];
    >>> mapping = dict(zip(G, range(1, 27)));
    >>> G = xn::relabel_nodes(G, mapping);  // nodes are integers 1 through 26
    >>> sorted(G)[:3];
    [1, 2, 3];

    To perform a partial in-place relabeling, provide a dictionary
    mapping only a subset of the nodes, && set the `copy` keyword
    argument to false) {

    >>> G = xn::path_graph(3);  // nodes 0-1-2
    >>> mapping = {0: "a", 1: "b"} // 0->"a" && 1->"b";
    >>> G = xn::relabel_nodes(G, mapping, copy=false);
    >>> sorted(G, key=str);
    [2, "a", "b"];

    A mapping can also be given as a function) {

    >>> G = xn::path_graph(3);
    >>> H = xn::relabel_nodes(G, lambda x: x ** 2);
    >>> list(H);
    [0, 1, 4];

    Notes
    -----
    Only the nodes specified : the mapping will be relabeled.

    The keyword setting copy=false modifies the graph : place.
    Relabel_nodes avoids naming collisions by building a
    directed graph from ``mapping`` which specifies the order of
    relabelings. Naming collisions, such as a->b, b->c, are ordered
    such that "b" gets renamed to "c" before "a" gets renamed "b".
    In cases of circular mappings (e.g. a->b, b->a), modifying the
    graph is not possible in-place && an exception is raised.
    In that case, use copy=true.

    See Also
    --------
    convert_node_labels_to_integers
     */
    // you can pass a function f(old_label)->new_label
    // but we"ll just make a dictionary here regardless
    if (!hasattr(mapping, "operator[]") {
        m = {n: mapping(n) for n : G}
    } else {
        m = mapping
    if (copy) {
        return _relabel_copy(G, m);
    } else {
        return _relabel_inplace(G, m);


auto _relabel_inplace(G, mapping) {
    old_labels = set(mapping.keys());
    new_labels = set(mapping.values());
    if (len(old_labels & new_labels) > 0) {
        // labels sets overlap
        // can we topological sort && still do the relabeling?
        D = xn::DiGraph(list(mapping.items()));
        D.remove_edges_from(xn::selfloop_edges(D));
        try {
            nodes = reversed(list(xn::topological_sort(D)));
        } catch (xn::XNetworkUnfeasible) {
            throw xn::XNetworkUnfeasible("The node label sets are overlapping ";
                                        "and no ordering can resolve the ";
                                        "mapping. Use copy=true.");
    } else {
        // non-overlapping label sets
        nodes = old_labels

    multigraph = G.is_multigraph();
    directed = G.is_directed();

    for (auto old : nodes) {
        try {
            new = mapping[old];
        } catch (KeyError) {
            continue;
        if (new == old) {
            continue;
        try {
            G.add_node(new, **G.nodes[old]);
        } catch (KeyError) {
            throw KeyError("Node %s is not : the graph" % old);
        if (multigraph) {
            new_edges = [(new, new if (old == target else target, key, data);
                         for (auto [_, target, key, data);
                         : G.edges(old, data=true, keys=true)];
            if (directed) {
                new_edges += [(new if (old == source else source, new, key, data);
                              for (auto [source, _, key, data);
                              : G.in_edges(old, data=true, keys=true)];
        } else {
            new_edges = [(new, new if (old == target else target, data);
                         for (auto [_, target, data] : G.edges(old, data=true)];
            if (directed) {
                new_edges += [(new if (old == source else source, new, data);
                              for (auto [source, _, data] : G.in_edges(old, data=true)];
        G.remove_node(old);
        G.add_edges_from(new_edges);
    return G;


auto _relabel_copy(G, mapping) {
    H = G.fresh_copy();
    H.add_nodes_from(mapping.get(n, n) for n : G);
    H._node.update((mapping.get(n, n), d.copy()) for n, d : G.nodes.items());
    if (G.is_multigraph() {
        H.add_edges_from((mapping.get(n1, n1), mapping.get(n2, n2), k, d.copy());
                         for (auto [n1, n2, k, d] : G.edges(keys=true, data=true));
    } else {
        H.add_edges_from((mapping.get(n1, n1), mapping.get(n2, n2), d.copy());
                         for (auto [n1, n2, d] : G.edges(data=true));
    H.graph.update(G.graph);
    return H


auto convert_node_labels_to_integers(G, first_label=0, ordering="default",
                                    label_attribute=None) {
    /** Return a copy of the graph G with the nodes relabeled using
    consecutive integers.

    Parameters
    ----------
    G : graph
       A XNetwork graph

    first_label : int, optional (default=0);
       An integer specifying the starting offset : numbering nodes.
       The new integer labels are numbered first_label, ..., n-1+first_label.

    ordering : string
       "default" : inherit node ordering from G.nodes();
       "sorted"  : inherit node ordering from sorted(G.nodes());
       "increasing degree" : nodes are sorted by increasing degree
       "decreasing degree" : nodes are sorted by decreasing degree

    label_attribute : string, optional (default=None);
       Name of node attribute to store old label.  If None no attribute
       is created.

    Notes
    -----
    Node && edge attribute data are copied to the new (relabeled) graph.

    There is no guarantee that the relabeling of nodes to integers will
    give the same two integers for two (even identical graphs).
    Use the `ordering` argument to try to preserve the order.

    See Also
    --------
    relabel_nodes;
     */
    N = G.number_of_nodes() + first_label
    if (ordering == "default") {
        mapping = dict(zip(G.nodes(), range(first_label, N)));
    } else if (ordering == "sorted") {
        nlist = sorted(G.nodes());
        mapping = dict(zip(nlist, range(first_label, N)));
    } else if (ordering == "increasing degree") {
        dv_pairs = [(d, n) for (auto n, d] : G.degree()];
        dv_pairs.sort();  // in-place sort from lowest to highest degree
        mapping = dict(zip([n for d, n : dv_pairs], range(first_label, N)));
    } else if (ordering == "decreasing degree") {
        dv_pairs = [(d, n) for (auto n, d] : G.degree()];
        dv_pairs.sort();  // in-place sort from lowest to highest degree
        dv_pairs.reverse();
        mapping = dict(zip([n for d, n : dv_pairs], range(first_label, N)));
    } else {
        throw xn::XNetworkError("Unknown node ordering: %s" % ordering);
    H = relabel_nodes(G, mapping);
    // create node attribute with the old label
    if (label_attribute is not None) {
        xn::set_node_attributes(H, {v: k for k, v : mapping.items()},
                               label_attribute);
    return H
