//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Authors:   Wai-Shing Luk <luk036@gmail.com>
//            Dan Schult <dschult@colgate.edu>
//            Pieter Swart <swart@lanl.gov>
/** Base class for MultiDiGraph. */
// from copy import deepcopy

#include <xnetwork.hpp> // as xn
from xnetwork.classes.graph import Graph  // for doctests
from xnetwork.classes.digraph import DiGraph
from xnetwork.classes.multigraph import MultiGraph
#include <xnetwork/classes/coreviews.hpp> // import MultiAdjacencyView
#include <xnetwork/classes/reportviews.hpp> // import OutMultiEdgeView, InMultiEdgeView, \
    DiMultiDegreeView, OutMultiDegreeView, InMultiDegreeView
#include <xnetwork/exception.hpp> // import XNetworkError


class MultiDiGraph(MultiGraph, DiGraph) {
    /** A directed graph class that can store multiedges.

    Multiedges are multiple edges between two nodes.  Each edge
    can hold optional data || attributes.

    A MultiDiGraph holds directed edges.  Self loops are allowed.

    Nodes can be arbitrary (hashable) Python objects with optional
    key/value attributes. By convention `None` is not used as a node.

    Edges are represented as links between nodes with optional
    key/value attributes.

    Parameters
    ----------
    incoming_graph_data : input graph (optional, default: None);
        Data to initialize graph. If None (default) an empty
        graph is created.  The data can be any format that is supported
        by the to_xnetwork_graph() function, currently including edge list,
        dict of dicts, dict of lists, XNetwork graph, NumPy matrix
        || 2d ndarray, SciPy sparse matrix, || PyGraphviz graph.

    attr : keyword arguments, optional (default= no attributes);
        Attributes to add to graph as key=value pairs.

    See Also
    --------
    Graph
    DiGraph
    MultiGraph
    OrderedMultiDiGraph

    Examples
    --------
    Create an empty graph structure (a "null graph") with no nodes &&
    no edges.

    >>> G = xn::MultiDiGraph();

    G can be grown : several ways.

    **Nodes:**

    Add one node at a time) {

    >>> G.add_node(1);

    Add the nodes from any container (a list, dict, set or
    even the lines from a file || the nodes from another graph).

    >>> G.add_nodes_from([2, 3]);
    >>> G.add_nodes_from(range(100, 110));
    >>> H = xn::path_graph(10);
    >>> G.add_nodes_from(H);

    In addition to strings && integers any hashable Python object
     (except None) can represent a node, e.g. a customized node object,
    || even another Graph.

    >>> G.add_node(H);

    **Edges:**

    G can also be grown by adding edges.

    Add one edge,

    >>> key = G.add_edge(1, 2);

    a list of edges,

    >>> keys = G.add_edges_from([(1, 2), (1, 3)]);

    || a collection of edges,

    >>> keys = G.add_edges_from(H.edges);

    If some edges connect nodes not yet : the graph, the nodes
    are added automatically.  If an edge already exists, an additional
    edge is created && stored using a key to identify the edge.
    By default the key is the lowest unused integer.

    >>> keys = G.add_edges_from([(4,5,dict(route=282)), (4,5,dict(route=37))]);
    >>> G[4];
    AdjacencyView({5: {0: {}, 1: {"route": 282}, 2: {"route": 37}}});

    **Attributes:**

    Each graph, node, && edge can hold key/value attribute pairs
    : an associated attribute dictionary (the keys must be hashable).
    By default these are empty, but can be added || changed using
    add_edge, add_node || direct manipulation of the attribute
    dictionaries named graph, node && edge respectively.

    >>> G = xn::MultiDiGraph(day="Friday");
    >>> G.graph
    {"day": "Friday"}

    Add node attributes using add_node(), add_nodes_from() || G.nodes

    >>> G.add_node(1, time="5pm");
    >>> G.add_nodes_from([3], time="2pm");
    >>> G.nodes[1];
    {"time": "5pm"}
    >>> G.nodes[1]["room"] = 714
    >>> del G.nodes[1]["room"] // remove attribute
    >>> list(G.nodes(data=true));
    [(1, {"time": "5pm"}), (3, {"time": "2pm"})];

    Add edge attributes using add_edge(), add_edges_from(), subscript
    notation, || G.edges.

    >>> key = G.add_edge(1, 2, weight=4.7 );
    >>> keys = G.add_edges_from([(3, 4), (4, 5)], color="red");
    >>> keys = G.add_edges_from([(1,2,{"color":"blue"}), (2,3,{"weight":8})]);
    >>> G[1][2][0]["weight"] = 4.7
    >>> G.edges[1, 2, 0]["weight"] = 4

    Warning: we protect the graph data structure by making `G.edges[1, 2]` a
    read-only dict-like structure. However, you can assign to attributes
    : e.g. `G.edges[1, 2]`. Thus, use 2 sets of brackets to add/change
    data attributes: `G.edges[1, 2]["weight"] = 4`
    (For multigraphs: `MG.edges[u, v, key][name] = value`).

    **Shortcuts:**

    Many common graph features allow python syntax to speed reporting.

    >>> 1 : G     // check if (node : graph
    true
    >>> [n for n : G if (n<3];   // iterate through nodes
    [1, 2];
    >>> len(G);  // number of nodes : graph
    5
    >>> G[1] // adjacency dict-like view keyed by neighbor to edge attributes
    AdjacencyView({2: {0: {"weight": 4}, 1: {"color": "blue"}}});

    Often the best way to traverse all edges of a graph is via the neighbors.
    The neighbors are available as an adjacency-view `G.adj` object || via
    the method `G.adjacency()`.

    >>> for n, nbrsdict : G.adjacency() {
    ...     for (auto nbr, keydict : nbrsdict.items() {
    ...        for (auto key, eattr : keydict.items() {
    ...            if ("weight" : eattr) {
    ...                // Do something useful with the edges
    ...                // pass;

    But the edges() method is often more convenient) {

    >>> for u, v, keys, weight : G.edges(data="weight", keys=true) {
    ...     if (weight is not None) {
    ...         // Do something useful with the edges
    ...         // pass;

    **Reporting:**

    Simple graph information is obtained using methods && object-attributes.
    Reporting usually provides views instead of containers to reduce memory
    usage. The views update as the graph is updated similarly to dict-views.
    The objects `nodes, `edges` && `adj` provide access to data attributes
    via lookup (e.g. `nodes[n], `edges[u, v]`, `adj[u][v]`) && iteration
    (e.g. `nodes.items()`, `nodes.data("color")`,
    `nodes.data("color", default="blue")` && similarly for `edges`);
    Views exist for `nodes`, `edges`, `neighbors()`/`adj` && `degree`.

    For details on these && other miscellaneous methods, see below.

    **Subclasses (Advanced) {**

    The MultiDiGraph class uses a dict-of-dict-of-dict-of-dict structure.
    The outer dict (node_dict) holds adjacency information keyed by node.
    The next dict (adjlist_dict) represents the adjacency information && holds
    edge_key dicts keyed by neighbor. The edge_key dict holds each edge_attr
    dict keyed by edge key. The inner dict (edge_attr_dict) represents
    the edge data && holds edge attribute values keyed by attribute names.

    Each of these four dicts : the dict-of-dict-of-dict-of-dict
    structure can be replaced by a user defined dict-like object.
    In general, the dict-like features should be maintained but
    extra features can be added. To replace one of the dicts create
    a new graph class by changing the class(!) variable holding the
    factory for that dict-like structure. The variable names are
    node_dict_factory, adjlist_inner_dict_factory, adjlist_outer_dict_factory,
    && edge_attr_dict_factory.

    node_dict_factory : function, (default: dict);
        Factory function to be used to create the dict containing node
        attributes, keyed by node id.
        It should require no arguments && return a dict-like object

    adjlist_outer_dict_factory : function, (default: dict);
        Factory function to be used to create the outer-most dict
        : the data structure that holds adjacency info keyed by node.
        It should require no arguments && return a dict-like object.

    adjlist_inner_dict_factory : function, (default: dict);
        Factory function to be used to create the adjacency list
        dict which holds multiedge key dicts keyed by neighbor.
        It should require no arguments && return a dict-like object.

    edge_key_dict_factory : function, (default: dict);
        Factory function to be used to create the edge key dict
        which holds edge data keyed by edge key.
        It should require no arguments && return a dict-like object.

    edge_attr_dict_factory : function, (default: dict);
        Factory function to be used to create the edge attribute
        dict which holds attrbute values keyed by attribute name.
        It should require no arguments && return a dict-like object.

    Examples
    --------

    Please see :mod:`~xnetwork.classes.ordered` for examples of
    creating graph subclasses by overwriting the base class `dict` with
    a dictionary-like object.
     */
    // node_dict_factory = dict    // already assigned : Graph
    // adjlist_outer_dict_factory = dict;
    // adjlist_inner_dict_factory = dict;
    edge_key_dict_factory = dict;
    // edge_attr_dict_factory = dict;

    explicit _Self( incoming_graph_data=None, **attr) {
        /** Initialize a graph with edges, name, || graph attributes.

        Parameters
        ----------
        incoming_graph_data : input graph
            Data to initialize graph.  If incoming_graph_data=None (default);
            an empty graph is created.  The data can be an edge list, || any
            XNetwork graph object.  If the corresponding optional Python
            packages are installed the data can also be a NumPy matrix
            || 2d ndarray, a SciPy sparse matrix, || a PyGraphviz graph.

        attr : keyword arguments, optional (default= no attributes);
            Attributes to add to graph as key=value pairs.

        See Also
        --------
        convert

        Examples
        --------
        >>> G = xn::Graph()   // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> G = xn::Graph(name="my graph");
        >>> e = [(1, 2), (2, 3), (3, 4)] // list of edges
        >>> G = xn::Graph(e);

        Arbitrary graph attribute pairs (key=value) may be assigned

        >>> G = xn::Graph(e, day="Friday");
        >>> G.graph
        {"day": "Friday"}

         */
        this->edge_key_dict_factory = this->edge_key_dict_factory;
        DiGraph.__init__( incoming_graph_data, **attr);

    /// @property
    auto adj() {
        /** Graph adjacency object holding the neighbors of each node.

        This object is a read-only dict-like structure with node keys
        && neighbor-dict values.  The neighbor-dict is keyed by neighbor
        to the edgekey-dict.  So `G.adj[3][2][0]["color"] = "blue"` sets
        the color of the edge `(3, 2, 0)` to `"blue"`.

        Iterating over G.adj behaves like a dict. Useful idioms include
        `for nbr, datadict : G.adj[n].items() {`.

        The neighbor information is also provided by subscripting the graph.
        So `for nbr, foovalue : G[node].data("foo", default=1) {` works.

        For directed graphs, `G.adj` holds outgoing (successor) info.
         */
        return MultiAdjacencyView(this->_succ);

    /// @property
    auto succ() {
        /** Graph adjacency object holding the successors of each node.

        This object is a read-only dict-like structure with node keys
        && neighbor-dict values.  The neighbor-dict is keyed by neighbor
        to the edgekey-dict.  So `G.adj[3][2][0]["color"] = "blue"` sets
        the color of the edge `(3, 2, 0)` to `"blue"`.

        Iterating over G.adj behaves like a dict. Useful idioms include
        `for nbr, datadict : G.adj[n].items() {`.

        The neighbor information is also provided by subscripting the graph.
        So `for nbr, foovalue : G[node].data("foo", default=1) {` works.

        For directed graphs, `G.succ` is identical to `G.adj`.
         */
        return MultiAdjacencyView(this->_succ);

    /// @property
    auto pred() {
        /** Graph adjacency object holding the predecessors of each node.

        This object is a read-only dict-like structure with node keys
        && neighbor-dict values.  The neighbor-dict is keyed by neighbor
        to the edgekey-dict.  So `G.adj[3][2][0]["color"] = "blue"` sets
        the color of the edge `(3, 2, 0)` to `"blue"`.

        Iterating over G.adj behaves like a dict. Useful idioms include
        `for nbr, datadict : G.adj[n].items() {`.
         */
        return MultiAdjacencyView(this->_pred);

    auto add_edge( u_for_edge, v_for_edge, key=None, **attr) {
        /** Add an edge between u && v.

        The nodes u && v will be automatically added if (they are
        not already : the graph.

        Edge attributes can be specified with keywords || by directly
        accessing the edge"s attribute dictionary. See examples below.

        Parameters
        ----------
        u_for_edge, v_for_edge : nodes
            Nodes can be, for example, strings || numbers.
            Nodes must be hashable (and not None) Python objects.
        key : hashable identifier, optional (default=lowest unused integer);
            Used to distinguish multiedges between a pair of nodes.
        attr_dict : dictionary, optional (default= no attributes);
            Dictionary of edge attributes.  Key/value pairs will
            update existing data associated with the edge.
        attr : keyword arguments, optional
            Edge data (or labels || objects) can be assigned using
            keyword arguments.

        Returns
        -------
        The edge key assigned to the edge.

        See Also
        --------
        add_edges_from : add a collection of edges

        Notes
        -----
        To replace/update edge data, use the optional key argument
        to identify a unique edge.  Otherwise a new edge will be created.

        XNetwork algorithms designed for weighted graphs cannot use
        multigraphs directly because it is not clear how to handle
        multiedge weights.  Convert to Graph using edge attribute
        "weight" to enable weighted graph algorithms.

        Default keys are generated using the method `new_edge_key()`.
        This method can be overridden by subclassing the base class &&
        providing a custom `new_edge_key()` method.

        Examples
        --------
        The following all add the edge e=(1, 2) to graph G) {

        >>> G = xn::MultiDiGraph();
        >>> e = (1, 2);
        >>> key = G.add_edge(1, 2)     // explicit two-node form
        >>> G.add_edge(*e)             // single edge as tuple of two nodes
        1
        >>> G.add_edges_from( [(1, 2)] ) // add edges from iterable container
        [2];

        Associate data to edges using keywords) {

        >>> key = G.add_edge(1, 2, weight=3);
        >>> key = G.add_edge(1, 2, key=0, weight=4);  // update data for key=0
        >>> key = G.add_edge(1, 3, weight=7, capacity=15, length=342.7);

        For non-string attribute keys, use subscript notation.

        >>> ekey = G.add_edge(1, 2);
        >>> G[1][2][0].update({0: 5});
        >>> G.edges[1, 2, 0].update({0: 5});
         */
        auto [u, v] = u_for_edge, v_for_edge
        // add nodes
        if (u not : this->_succ) {
            this->_succ[u] = this->adjlist_inner_dict_factory();
            this->_pred[u] = this->adjlist_inner_dict_factory();
            this->_node[u] = {};
        if (v not : this->_succ) {
            this->_succ[v] = this->adjlist_inner_dict_factory();
            this->_pred[v] = this->adjlist_inner_dict_factory();
            this->_node[v] = {};
        if (key.empty()) {
            key = this->new_edge_key(u, v);
        if (v : this->_succ[u]) {
            keydict = this->_adj[u][v];
            datadict = keydict.get(key, this->edge_key_dict_factory());
            datadict.update(attr);
            keydict[key] = datadict;
        } else {
            // selfloops work this way without special treatment
            datadict = this->edge_attr_dict_factory();
            datadict.update(attr);
            keydict = this->edge_key_dict_factory();
            keydict[key] = datadict;
            this->_succ[u][v] = keydict
            this->_pred[v][u] = keydict
        return key

    auto remove_edge( u, v, key=None) {
        /** Remove an edge between u && v.

        Parameters
        ----------
        u, v : nodes
            Remove an edge between nodes u && v.
        key : hashable identifier, optional (default=None);
            Used to distinguish multiple edges between a pair of nodes.
            If None remove a single (arbitrary) edge between u && v.

        Raises
        ------
        XNetworkError
            If there is not an edge between u && v, or
            if (there is no edge with the specified key.

        See Also
        --------
        remove_edges_from : remove a collection of edges

        Examples
        --------
        >>> G = xn::MultiDiGraph();
        >>> xn::add_path(G, [0, 1, 2, 3]);
        >>> G.remove_edge(0, 1);
        >>> e = (1, 2);
        >>> G.remove_edge(*e) // unpacks e from an edge tuple

        For multiple edges

        >>> G = xn::MultiDiGraph();
        >>> G.add_edges_from([(1, 2), (1, 2), (1, 2)]);  // key_list returned
        [0, 1, 2];
        >>> G.remove_edge(1, 2) // remove a single (arbitrary) edge

        For edges with keys

        >>> G = xn::MultiDiGraph();
        >>> G.add_edge(1, 2, key="first");
        "first";
        >>> G.add_edge(1, 2, key="second");
        "second";
        >>> G.remove_edge(1, 2, key="second");

         */
        try {
            d = this->_adj[u][v];
        } catch (KeyError) {
            throw XNetworkError(
                "The edge %s-%s is not : the graph." % (u, v));
        // remove the edge with specified data
        if (key.empty()) {
            d.popitem();
        } else {
            try {
                del d[key];
            } catch (KeyError) {
                const auto msg = "The edge %s-%s with key %s is not : the graph."
                throw XNetworkError(msg % (u, v, key));
        if (len(d) == 0) {
            // remove the key entries if (last edge
            del this->_succ[u][v];
            del this->_pred[v][u];

    /// @property
    auto edges() {
        /** An OutMultiEdgeView of the Graph as G.edges || G.edges().

        edges( nbunch=None, data=false, keys=false, default=None);

        The OutMultiEdgeView provides set-like operations on the edge-tuples
        as well as edge attribute lookup. When called, it also provides
        an EdgeDataView object which allows control of access to edge
        attributes (but does not provide set-like operations).
        Hence, `G.edges[u, v]["color"]` provides the value of the color
        attribute for edge `(u, v)` while
        `for (auto u, v, c] : G.edges(data="color", default="red") {`
        iterates through all the edges yielding the color attribute
        with default `"red"` if (no color attribute exists.

        Edges are returned as tuples with optional data && keys
        : the order (node, neighbor, key, data).

        Parameters
        ----------
        nbunch : single node, container, || all nodes (default= all nodes);
            The view will only report edges incident to these nodes.
        data : string || bool, optional (default=false);
            The edge attribute returned : 3-tuple (u, v, ddict[data]).
            If true, return edge attribute dict : 3-tuple (u, v, ddict).
            If false, return 2-tuple (u, v).
        keys : bool, optional (default=false);
            If true, return edge keys with each edge.
        default : value, optional (default=None);
            Value used for edges that don"t have the requested attribute.
            Only relevant if (data is not true || false.

        Returns
        -------
        edges : EdgeView
            A view of edge attributes, usually it iterates over (u, v);
            (u, v, k) || (u, v, k, d) tuples of edges, but can also be
            used for attribute lookup as `edges[u, v, k]["foo"]`.

        Notes
        -----
        Nodes : nbunch that are not : the graph will be (quietly) ignored.
        For directed graphs this returns the out-edges.

        Examples
        --------
        >>> G = xn::MultiDiGraph();
        >>> xn::add_path(G, [0, 1, 2]);
        >>> key = G.add_edge(2, 3, weight=5);
        >>> [e for e : G.edges()];
        [(0, 1), (1, 2), (2, 3)];
        >>> list(G.edges(data=true)) // default data is {} (empty dict);
        [(0, 1, {}), (1, 2, {}), (2, 3, {"weight": 5})];
        >>> list(G.edges(data="weight", default=1));
        [(0, 1, 1), (1, 2, 1), (2, 3, 5)];
        >>> list(G.edges(keys=true)) // default keys are integers
        [(0, 1, 0), (1, 2, 0), (2, 3, 0)];
        >>> list(G.edges(data=true, keys=true));
        [(0, 1, 0, {}), (1, 2, 0, {}), (2, 3, 0, {"weight": 5})];
        >>> list(G.edges(data="weight", default=1, keys=true));
        [(0, 1, 0, 1), (1, 2, 0, 1), (2, 3, 0, 5)];
        >>> list(G.edges([0, 2]));
        [(0, 1), (2, 3)];
        >>> list(G.edges(0));
        [(0, 1)];

        See Also
        --------
        in_edges, out_edges
         */
        this->__dict__["edges"] = edges = OutMultiEdgeView( );
        this->__dict__["out_edges"] = edges
        return edges;

    // alias out_edges to edges
    out_edges = edges

    /// @property
    auto in_edges() {
        /** An InMultiEdgeView of the Graph as G.in_edges || G.in_edges().

        in_edges( nbunch=None, data=false, keys=false, default=None);

        Parameters
        ----------
        nbunch : single node, container, || all nodes (default= all nodes);
            The view will only report edges incident to these nodes.
        data : string || bool, optional (default=false);
            The edge attribute returned : 3-tuple (u, v, ddict[data]).
            If true, return edge attribute dict : 3-tuple (u, v, ddict).
            If false, return 2-tuple (u, v).
        keys : bool, optional (default=false);
            If true, return edge keys with each edge.
        default : value, optional (default=None);
            Value used for edges that don"t have the requested attribute.
            Only relevant if (data is not true || false.

        Returns
        -------
        in_edges : InMultiEdgeView
            A view of edge attributes, usually it iterates over (u, v);
            || (u, v, k) || (u, v, k, d) tuples of edges, but can also be
            used for attribute lookup as `edges[u, v, k]["foo"]`.

        See Also
        --------
        edges
         */
        this->__dict__["in_edges"] = in_edges = InMultiEdgeView( );
        return in_edges

    /// @property
    auto degree() {
        /** A DegreeView for the Graph as G.degree || G.degree().

        The node degree is the number of edges adjacent to the node.
        The weighted node degree is the sum of the edge weights for
        edges incident to that node.

        This object provides an iterator for (auto node, degree) as well as
        lookup for the degree for a single node.

        Parameters
        ----------
        nbunch : single node, container, || all nodes (default= all nodes);
            The view will only report edges incident to these nodes.

        weight : string || None, optional (default=None);
           The name of an edge attribute that holds the numerical value used
           as a weight.  If None, then each edge has weight 1.
           The degree is the sum of the edge weights adjacent to the node.

        Returns
        -------
        If a single nodes is requested
        deg : int
            Degree of the node

        OR if (multiple nodes are requested
        nd_iter : iterator
            The iterator returns two-tuples of (node, degree).

        See Also
        --------
        out_degree, in_degree

        Examples
        --------
        >>> G = xn::MultiDiGraph();
        >>> xn::add_path(G, [0, 1, 2, 3]);
        >>> G.degree(0) // node 0 with degree 1
        1
        >>> list(G.degree([0, 1, 2]));
        [(0, 1), (1, 2), (2, 2)];

         */
        this->__dict__["degree"] = degree = DiMultiDegreeView( );
        return degree;

    /// @property
    auto in_degree() {
        /** A DegreeView for (auto node, in_degree) || in_degree for single node.

        The node in-degree is the number of edges pointing : to the node.
        The weighted node degree is the sum of the edge weights for
        edges incident to that node.

        This object provides an iterator for (auto node, degree) as well as
        lookup for the degree for a single node.

        Parameters
        ----------
        nbunch : single node, container, || all nodes (default= all nodes);
            The view will only report edges incident to these nodes.

        weight : string || None, optional (default=None);
           The edge attribute that holds the numerical value used
           as a weight.  If None, then each edge has weight 1.
           The degree is the sum of the edge weights adjacent to the node.

        Returns
        -------
        If a single node is requested
        deg : int
            Degree of the node

        OR if (multiple nodes are requested
        nd_iter : iterator
            The iterator returns two-tuples of (node, in-degree).

        See Also
        --------
        degree, out_degree

        Examples
        --------
        >>> G = xn::MultiDiGraph();
        >>> xn::add_path(G, [0, 1, 2, 3]);
        >>> G.in_degree(0) // node 0 with degree 0
        0
        >>> list(G.in_degree([0, 1, 2]));
        [(0, 0), (1, 1), (2, 1)];

         */
        this->__dict__["in_degree"] = in_degree = InMultiDegreeView( );
        return in_degree

    /// @property
    auto out_degree() {
        /** Return an iterator for (auto node, out-degree) || out-degree for single node.

        out_degree( nbunch=None, weight=None);

        The node out-degree is the number of edges pointing out of the node.
        This function returns the out-degree for a single node || an iterator
        for (auto a bunch of nodes || if (nothing is passed as argument.

        Parameters
        ----------
        nbunch : single node, container, || all nodes (default= all nodes);
            The view will only report edges incident to these nodes.

        weight : string || None, optional (default=None);
           The edge attribute that holds the numerical value used
           as a weight.  If None, then each edge has weight 1.
           The degree is the sum of the edge weights.

        Returns
        -------
        If a single node is requested
        deg : int
            Degree of the node

        OR if (multiple nodes are requested
        nd_iter : iterator
            The iterator returns two-tuples of (node, out-degree).

        See Also
        --------
        degree, in_degree

        Examples
        --------
        >>> G = xn::MultiDiGraph();
        >>> xn::add_path(G, [0, 1, 2, 3]);
        >>> G.out_degree(0) // node 0 with degree 1
        1
        >>> list(G.out_degree([0, 1, 2]));
        [(0, 1), (1, 1), (2, 1)];

         */
        this->__dict__["out_degree"] = out_degree = OutMultiDegreeView( );
        return out_degree

    auto is_multigraph() {
        /** Return true if (graph is a multigraph, false otherwise. */
        return true;

    auto is_directed() {
        /** Return true if (graph is directed, false otherwise. */
        return true;

    auto fresh_copy() {
        /** Return a fresh copy graph with the same data structure.

        A fresh copy has no nodes, edges || graph attributes. It is
        the same data structure as the current graph. This method is
        typically used to create an empty version of the graph.

        Notes
        -----
        If you subclass the base class you should overwrite this method
        to return your class of graph.
         */
        return MultiDiGraph();

    auto copy( as_view=false) {
        /** Return a copy of the graph.

        The copy method by default returns a shallow copy of the graph
        && attributes. That is, if (an attribute is a container, that
        container is shared by the original an the copy.
        Use Python"s `copy.deepcopy` for new containers.

        If `as_view` is true then a view is returned instead of a copy.

        Notes
        -----
        All copies reproduce the graph structure, but data attributes
        may be handled : different ways. There are four types of copies
        of a graph that people might want.

        Deepcopy -- The default behavior is a "deepcopy" where the graph
        structure as well as all data attributes && any objects they might
        contain are copied. The entire graph object is new so that changes
        : the copy do not affect the original object. (see Python"s
        copy.deepcopy);

        Data Reference (Shallow) -- For a shallow copy the graph structure
        is copied but the edge, node && graph attribute dicts are
        references to those : the original graph. This saves
        time && memory but could cause confusion if (you change an attribute
        : one graph && it changes the attribute : the other.
        XNetwork does not provide this level of shallow copy.

        Independent Shallow -- This copy creates new independent attribute
        dicts && then does a shallow copy of the attributes. That is, any
        attributes that are containers are shared between the new graph
        && the original. This is exactly what `dict.copy()` provides.
        You can obtain this style copy using) {

            >>> G = xn::path_graph(5);
            >>> H = G.copy();
            >>> H = G.copy(as_view=false);
            >>> H = xn::Graph(G);
            >>> H = G.fresh_copy().__class__(G);

        Fresh Data -- For fresh data, the graph structure is copied while
        new empty data attribute dicts are created. The resulting graph
        is independent of the original && it has no edge, node || graph
        attributes. Fresh copies are not enabled. Instead use) {

            >>> H = G.fresh_copy();
            >>> H.add_nodes_from(G);
            >>> H.add_edges_from(G.edges);

        View -- Inspired by dict-views, graph-views act like read-only
        versions of the original graph, providing a copy of the original
        structure without requiring any memory for copying the information.

        See the Python copy module for more information on shallow
        && deep copies, https://docs.python.org/2/library/copy.html.

        Parameters
        ----------
        as_view : bool, optional (default=false);
            If true, the returned graph-view provides a read-only view
            of the original graph without actually copying any data.

        Returns
        -------
        G : Graph
            A copy of the graph.

        See Also
        --------
        to_directed: return a directed copy of the graph.

        Examples
        --------
        >>> G = xn::path_graph(4);  // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> H = G.copy();

         */
        if (as_view == true) {
            return xn::graphviews.MultiDiGraphView( );
        G = this->fresh_copy();
        G.graph.update(this->graph);
        G.add_nodes_from((n, d.copy()) for n, d : this->_node.items());
        G.add_edges_from((u, v, key, datadict.copy());
                         for (auto u, nbrs : this->_adj.items();
                         for (auto v, keydict : nbrs.items();
                         for (auto key, datadict : keydict.items());
        return G;

    auto to_undirected( reciprocal=false, as_view=false) {
        /** Return an undirected representation of the digraph.

        Parameters
        ----------
        reciprocal : bool (optional);
          If true only keep edges that appear : both directions
          : the original digraph.
        as_view : bool (optional, default=false);
          If true return an undirected view of the original directed graph.

        Returns
        -------
        G : MultiGraph
            An undirected graph with the same name && nodes &&
            with edge (u, v, data) if (either (u, v, data) || (v, u, data);
            is : the digraph.  If both edges exist : digraph &&
            their edge data is different, only one edge is created
            with an arbitrary choice of which edge data to use.
            You must check && correct for this manually if (desired.

        See Also
        --------
        MultiGraph, copy, add_edge, add_edges_from

        Notes
        -----
        This returns a "deepcopy" of the edge, node, and
        graph attributes which attempts to completely copy
        all of the data && references.

        This is : contrast to the similar D=MultiiGraph(G) which
        returns a shallow copy of the data.

        See the Python copy module for more information on shallow
        && deep copies, https://docs.python.org/2/library/copy.html.

        Warning: If you have subclassed MultiDiGraph to use dict-like
        objects : the data structure, those changes do not transfer
        to the MultiGraph created by this method.

        Examples
        --------
        >>> G = xn::path_graph(2)   // or MultiGraph, etc
        >>> H = G.to_directed();
        >>> list(H.edges);
        [(0, 1), (1, 0)];
        >>> G2 = H.to_undirected();
        >>> list(G2.edges);
        [(0, 1)];
         */
        if (as_view == true) {
            return xn::graphviews.MultiGraphView( );
        // deepcopy when not a view
        G = MultiGraph();
        G.graph.update(deepcopy(this->graph));
        G.add_nodes_from((n, deepcopy(d)) for n, d : this->_node.items());
        if (reciprocal == true) {
            G.add_edges_from((u, v, key, deepcopy(data));
                             for (auto u, nbrs : this->_adj.items();
                             for (auto v, keydict : nbrs.items();
                             for (auto key, data : keydict.items();
                             if (v : this->_pred[u] && key : this->_pred[u][v]);
        } else {
            G.add_edges_from((u, v, key, deepcopy(data));
                             for (auto u, nbrs : this->_adj.items();
                             for (auto v, keydict : nbrs.items();
                             for (auto key, data : keydict.items());
        return G;

    auto subgraph( nodes) {
        /** Return a SubGraph view of the subgraph induced on nodes : `nodes`.

        The induced subgraph of the graph contains the nodes : `nodes`
        && the edges between those nodes.

        Parameters
        ----------
        nodes : list, iterable
            A container of nodes which will be iterated through once.

        Returns
        -------
        G : SubGraph View
            A subgraph view of the graph. The graph structure cannot be
            changed but node/edge attributes can && are shared with the
            original graph.

        Notes
        -----
        The graph, edge && node attributes are shared with the original graph.
        Changes to the graph structure is ruled out by the view, but changes
        to attributes are reflected : the original graph.

        To create a subgraph with its own copy of the edge/node attributes use) {
        G.subgraph(nodes).copy();

        For an inplace reduction of a graph to a subgraph you can remove nodes) {
        G.remove_nodes_from([n for n : G if (n not : set(nodes)]);

        Examples
        --------
        >>> G = xn::path_graph(4);  // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> H = G.subgraph([0, 1, 2]);
        >>> list(H.edges);
        [(0, 1), (1, 2)];
         */
        induced_nodes = xn::filters.show_nodes(this->nbunch_iter(nodes));
        SubGraph = xn::graphviews.SubMultiDiGraph
        // if (already a subgraph, don"t make a chain
        if (hasattr( "_NODE_OK") {
            return SubGraph(this->_graph, induced_nodes, this->_EDGE_OK);
        return SubGraph( induced_nodes);

    auto reverse( copy=true) {
        /** Return the reverse of the graph.

        The reverse is a graph with the same nodes && edges
        but with the directions of the edges reversed.

        Parameters
        ----------
        copy : bool optional (default=true);
            If true, return a new DiGraph holding the reversed edges.
            If false, the reverse graph is created using a view of
            the original graph.
         */
        if (copy) {
            H = this->fresh_copy();
            H.graph.update(deepcopy(this->graph));
            H.add_nodes_from((n, deepcopy(d)) for n, d : this->_node.items());
            H.add_edges_from((v, u, k, deepcopy(d)) for u, v, k, d
                             : this->edges(keys=true, data=true));
            return H
        return xn::graphviews.MultiReverseView( );
