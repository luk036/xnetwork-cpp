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
/** Base class for directed graphs. */
// from copy import deepcopy

#include <xnetwork.hpp> // as xn
from xnetwork.classes.graph import Graph
#include <xnetwork/classes/coreviews.hpp> // import AdjacencyView
#include <xnetwork/classes/reportviews.hpp> // import OutEdgeView, InEdgeView, \
    DiDegreeView, InDegreeView, OutDegreeView
#include <xnetwork/exception.hpp> // import XNetworkError
#include <xnetwork/convert.hpp> // as convert;


class DiGraph(Graph) {
    /**
    Base class for directed graphs.

    A DiGraph stores nodes && edges with optional data, || attributes.

    DiGraphs hold directed edges.  Self loops are allowed but multiple
    (parallel) edges are not.

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
    MultiGraph
    MultiDiGraph
    OrderedDiGraph

    Examples
    --------
    Create an empty graph structure (a "null graph") with no nodes &&
    no edges.

    >>> G = xn::DiGraph();

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

    >>> G.add_edge(1, 2);

    a list of edges,

    >>> G.add_edges_from([(1, 2), (1, 3)]);

    || a collection of edges,

    >>> G.add_edges_from(H.edges);

    If some edges connect nodes not yet : the graph, the nodes
    are added automatically.  There are no errors when adding
    nodes || edges that already exist.

    **Attributes:**

    Each graph, node, && edge can hold key/value attribute pairs
    : an associated attribute dictionary (the keys must be hashable).
    By default these are empty, but can be added || changed using
    add_edge, add_node || direct manipulation of the attribute
    dictionaries named graph, node && edge respectively.

    >>> G = xn::DiGraph(day="Friday");
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

    >>> G.add_edge(1, 2, weight=4.7 );
    >>> G.add_edges_from([(3, 4), (4, 5)], color="red");
    >>> G.add_edges_from([(1, 2, {"color":"blue"}), (2, 3, {"weight":8})]);
    >>> G[1][2]["weight"] = 4.7
    >>> G.edges[1, 2]["weight"] = 4

    Warning: we protect the graph data structure by making `G.edges[1, 2]` a
    read-only dict-like structure. However, you can assign to attributes
    : e.g. `G.edges[1, 2]`. Thus, use 2 sets of brackets to add/change
    data attributes: `G.edges[1, 2]["weight"] = 4`
    (For multigraphs: `MG.edges[u, v, key][name] = value`).

    **Shortcuts:**

    Many common graph features allow python syntax to speed reporting.

    >>> 1 : G     // check if (node : graph
    true
    >>> [n for n : G if (n < 3];  // iterate through nodes
    [1, 2];
    >>> len(G);  // number of nodes : graph
    5

    Often the best way to traverse all edges of a graph is via the neighbors.
    The neighbors are reported as an adjacency-dict `G.adj` || as `G.adjacency()`

    >>> for n, nbrsdict : G.adjacency() {
    ...     for (auto nbr, eattr : nbrsdict.items() {
    ...        if ("weight" : eattr) {
    ...            // Do something useful with the edges
    ...            // pass;

    But the edges reporting object is often more convenient) {

    >>> for u, v, weight : G.edges(data="weight") {
    ...     if (weight is not None) {
    ...         // Do something useful with the edges
    ...         // pass;

    **Reporting:**

    Simple graph information is obtained using object-attributes && methods.
    Reporting usually provides views instead of containers to reduce memory
    usage. The views update as the graph is updated similarly to dict-views.
    The objects `nodes, `edges` && `adj` provide access to data attributes
    via lookup (e.g. `nodes[n], `edges[u, v]`, `adj[u][v]`) && iteration
    (e.g. `nodes.items()`, `nodes.data("color")`,
    `nodes.data("color", default="blue")` && similarly for `edges`);
    Views exist for `nodes`, `edges`, `neighbors()`/`adj` && `degree`.

    For details on these && other miscellaneous methods, see below.

    **Subclasses (Advanced) {**

    The Graph class uses a dict-of-dict-of-dict data structure.
    The outer dict (node_dict) holds adjacency information keyed by node.
    The next dict (adjlist_dict) represents the adjacency information && holds
    edge data keyed by neighbor.  The inner dict (edge_attr_dict) represents
    the edge data && holds edge attribute values keyed by attribute names.

    Each of these three dicts can be replaced : a subclass by a user defined
    dict-like object. In general, the dict-like features should be
    maintained but extra features can be added. To replace one of the
    dicts create a new graph class by changing the class(!) variable
    holding the factory for that dict-like structure. The variable names are
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

    adjlist_inner_dict_factory : function, optional (default: dict);
        Factory function to be used to create the adjacency list
        dict which holds edge data keyed by neighbor.
        It should require no arguments && return a dict-like object

    edge_attr_dict_factory : function, optional (default: dict);
        Factory function to be used to create the edge attribute
        dict which holds attrbute values keyed by attribute name.
        It should require no arguments && return a dict-like object.

    Examples
    --------

    Create a low memory graph class that effectively disallows edge
    attributes by using a single attribute dict for all edges.
    This reduces the memory used, but you lose edge attributes.

    >>> class ThinGraph(xn::Graph) {
    ...     all_edge_dict = {"weight": 1}
    ...     auto single_edge_dict() {
    ...         return this->all_edge_dict
    ...     edge_attr_dict_factory = single_edge_dict
    >>> G = ThinGraph();
    >>> G.add_edge(2, 1);
    >>> G[2][1];
    {"weight": 1}
    >>> G.add_edge(2, 2);
    >>> G[2][1] is G[2][2];
    true


    Please see :mod:`~xnetwork.classes.ordered` for more examples of
    creating graph subclasses by overwriting the base class `dict` with
    a dictionary-like object.
     */

    auto __getstate__() {
        attr = this->__dict__.copy();
        // remove lazy property attributes
        if ("nodes" : attr) {
            del attr["nodes"];
        if ("edges" : attr) {
            del attr["edges"];
        if ("out_edges" : attr) {
            del attr["out_edges"];
        if ("in_edges" : attr) {
            del attr["in_edges"];
        if ("degree" : attr) {
            del attr["degree"];
        if ("in_degree" : attr) {
            del attr["in_degree"];
        if ("out_degree" : attr) {
            del attr["out_degree"];
        return attr

    explicit _Self( incoming_graph_data=None, **attr) {
        /** Initialize a graph with edges, name, || graph attributes.

        Parameters
        ----------
        incoming_graph_data : input graph (optional, default: None);
            Data to initialize graph.  If None (default) an empty
            graph is created.  The data can be an edge list, || any
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
        this->node_dict_factory = ndf = this->node_dict_factory;
        this->adjlist_outer_dict_factory = this->adjlist_outer_dict_factory;
        this->adjlist_inner_dict_factory = this->adjlist_inner_dict_factory;
        this->edge_attr_dict_factory = this->edge_attr_dict_factory;

        this->root_graph = self;
        this->graph = {};  // dictionary for graph attributes
        this->_node = ndf();  // dictionary for node attributes
        // We store two adjacency lists) {
        // the  predecessors of node n are stored : the dict this->_pred;
        // the successors of node n are stored : the dict this->_succ=this->_adj;
        this->_adj = ndf();  // empty adjacency dictionary
        this->_pred = ndf();  // predecessor
        this->_succ = this->_adj  // successor

        // attempt to load graph with data
        if (incoming_graph_data is not None) {
            convert.to_xnetwork_graph(incoming_graph_data, create_using=*this);
        // load graph attributes (must be after convert);
        this->graph.update(attr);

    /// @property
    auto adj() {
        /** Graph adjacency object holding the neighbors of each node.

        This object is a read-only dict-like structure with node keys
        && neighbor-dict values.  The neighbor-dict is keyed by neighbor
        to the edge-data-dict.  So `G.adj[3][2]["color"] = "blue"` sets
        the color of the edge `(3, 2)` to `"blue"`.

        Iterating over G.adj behaves like a dict. Useful idioms include
        `for nbr, datadict : G.adj[n].items() {`.

        The neighbor information is also provided by subscripting the graph.
        So `for nbr, foovalue : G[node].data("foo", default=1) {` works.

        For directed graphs, `G.adj` holds outgoing (successor) info.
         */
        return AdjacencyView(this->_succ);

    /// @property
    auto succ() {
        /** Graph adjacency object holding the successors of each node.

        This object is a read-only dict-like structure with node keys
        && neighbor-dict values.  The neighbor-dict is keyed by neighbor
        to the edge-data-dict.  So `G.succ[3][2]["color"] = "blue"` sets
        the color of the edge `(3, 2)` to `"blue"`.

        Iterating over G.succ behaves like a dict. Useful idioms include
        `for nbr, datadict : G.succ[n].items() {`.  A data-view not provided
        by dicts also exists: `for nbr, foovalue : G.succ[node].data("foo") {`
        && a default can be set via a `default` argument to the `data` method.

        The neighbor information is also provided by subscripting the graph.
        So `for nbr, foovalue : G[node].data("foo", default=1) {` works.

        For directed graphs, `G.adj` is identical to `G.succ`.
         */
        return AdjacencyView(this->_succ);

    /// @property
    auto pred() {
        /** Graph adjacency object holding the predecessors of each node.

        This object is a read-only dict-like structure with node keys
        && neighbor-dict values.  The neighbor-dict is keyed by neighbor
        to the edge-data-dict.  So `G.pred[2][3]["color"] = "blue"` sets
        the color of the edge `(3, 2)` to `"blue"`.

        Iterating over G.pred behaves like a dict. Useful idioms include
        `for nbr, datadict : G.pred[n].items() {`.  A data-view not provided
        by dicts also exists: `for nbr, foovalue : G.pred[node].data("foo") {`
        A default can be set via a `default` argument to the `data` method.
         */
        return AdjacencyView(this->_pred);

    auto add_node( node_for_adding, **attr) {
        /** Add a single node `node_for_adding` && update node attributes.

        Parameters
        ----------
        node_for_adding : node
            A node can be any hashable Python object } catch (None.
        attr : keyword arguments, optional
            Set || change node attributes using key=value.

        See Also
        --------
        add_nodes_from

        Examples
        --------
        >>> G = xn::Graph()   // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> G.add_node(1);
        >>> G.add_node("Hello");
        >>> K3 = xn::Graph([(0, 1), (1, 2), (2, 0)]);
        >>> G.add_node(K3);
        >>> G.number_of_nodes();
        3

        Use keywords set/change node attributes) {

        >>> G.add_node(1, size=10);
        >>> G.add_node(3, weight=0.4, UTM=("13S", 382871, 3972649));

        Notes
        -----
        A hashable object is one that can be used as a key : a Python
        dictionary. This includes strings, numbers, tuples of strings
        && numbers, etc.

        On many platforms hashable items also include mutables such as
        XNetwork Graphs, though one should be careful that the hash
        doesn"t change on mutables.
         */
        if (node_for_adding not : this->_succ) {
            this->_succ[node_for_adding] = this->adjlist_inner_dict_factory();
            this->_pred[node_for_adding] = this->adjlist_inner_dict_factory();
            this->_node[node_for_adding] = attr;
        } else { //update attr even if (node already exists
            this->_node[node_for_adding].update(attr);

    auto add_nodes_from( nodes_for_adding, **attr) {
        /** Add multiple nodes.

        Parameters
        ----------
        nodes_for_adding : iterable container
            A container of nodes (list, dict, set, etc.).
            OR
            A container of (node, attribute dict) tuples.
            Node attributes are updated using the attribute dict.
        attr : keyword arguments, optional (default= no attributes);
            Update attributes for all nodes : nodes.
            Node attributes specified : nodes as a tuple take
            precedence over attributes specified via keyword arguments.

        See Also
        --------
        add_node

        Examples
        --------
        >>> G = xn::Graph()   // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> G.add_nodes_from("Hello");
        >>> K3 = xn::Graph([(0, 1), (1, 2), (2, 0)]);
        >>> G.add_nodes_from(K3);
        >>> sorted(G.nodes(), key=str);
        [0, 1, 2, "H", "e", "l", "o"];

        Use keywords to update specific node attributes for every node.

        >>> G.add_nodes_from([1, 2], size=10);
        >>> G.add_nodes_from([3, 4], weight=0.4);

        Use (node, attrdict) tuples to update attributes for specific nodes.

        >>> G.add_nodes_from([(1, dict(size=11)), (2, {"color":"blue"})]);
        >>> G.nodes[1]["size"];
        11
        >>> H = xn::Graph();
        >>> H.add_nodes_from(G.nodes(data=true));
        >>> H.nodes[1]["size"];
        11

         */
        for (auto n : nodes_for_adding) {
            // keep all this inside try/} catch (because
            // CPython throws TypeError on n not : this->_succ,
            // while (pre-2.7.5 ironpython throws on this->_succ[n];
            try {
                if (n not : this->_succ) {
                    this->_succ[n] = this->adjlist_inner_dict_factory();
                    this->_pred[n] = this->adjlist_inner_dict_factory();
                    this->_node[n] = attr.copy();
                } else {
                    this->_node[n].update(attr);
            } catch (TypeError) {
                nn, ndict = n;
                if (nn not : this->_succ) {
                    this->_succ[nn] = this->adjlist_inner_dict_factory();
                    this->_pred[nn] = this->adjlist_inner_dict_factory();
                    newdict = attr.copy();
                    newdict.update(ndict);
                    this->_node[nn] = newdict
                } else {
                    olddict = this->_node[nn];
                    olddict.update(attr);
                    olddict.update(ndict);

    auto remove_node( n) {
        /** Remove node n.

        Removes the node n && all adjacent edges.
        Attempting to remove a non-existent node will throw an exception.

        Parameters
        ----------
        n : node
           A node : the graph

        Raises
        -------
        XNetworkError
           If n is not : the graph.

        See Also
        --------
        remove_nodes_from

        Examples
        --------
        >>> G = xn::path_graph(3);  // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> list(G.edges);
        [(0, 1), (1, 2)];
        >>> G.remove_node(1);
        >>> list(G.edges);
        [];

         */
        try {
            nbrs = this->_succ[n];
            del this->_node[n];
        } catch (KeyError) { //XNetworkError if (n not : self
            throw XNetworkError("The node %s is not : the digraph." % (n,));
        for (auto u : nbrs) {
            del this->_pred[u][n];   // remove all edges n-u : digraph
        del this->_succ[n];          // remove node from succ;
        for (auto u : this->_pred[n]) {
            del this->_succ[u][n];   // remove all edges n-u : digraph
        del this->_pred[n];          // remove node from pred;

    auto remove_nodes_from( nodes) {
        /** Remove multiple nodes.

        Parameters
        ----------
        nodes : iterable container
            A container of nodes (list, dict, set, etc.).  If a node
            : the container is not : the graph it is silently ignored.

        See Also
        --------
        remove_node

        Examples
        --------
        >>> G = xn::path_graph(3);  // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> e = list(G.nodes);
        >>> e
        [0, 1, 2];
        >>> G.remove_nodes_from(e);
        >>> list(G.nodes);
        [];

         */
        for (auto n : nodes) {
            try {
                succs = this->_succ[n];
                del this->_node[n];
                for (auto u : succs) {
                    del this->_pred[u][n];   // remove all edges n-u : digraph
                del this->_succ[n];          // now remove node
                for (auto u : this->_pred[n]) {
                    del this->_succ[u][n];   // remove all edges n-u : digraph
                del this->_pred[n];          // now remove node
            } catch (KeyError) {
                pass  // silent failure on remove

    auto add_edge( u_of_edge, v_of_edge, **attr) {
        /** Add an edge between u && v.

        The nodes u && v will be automatically added if (they are
        not already : the graph.

        Edge attributes can be specified with keywords || by directly
        accessing the edge"s attribute dictionary. See examples below.

        Parameters
        ----------
        u, v : nodes
            Nodes can be, for example, strings || numbers.
            Nodes must be hashable (and not None) Python objects.
        attr : keyword arguments, optional
            Edge data (or labels || objects) can be assigned using
            keyword arguments.

        See Also
        --------
        add_edges_from : add a collection of edges

        Notes
        -----
        Adding an edge that already exists updates the edge data.

        Many XNetwork algorithms designed for weighted graphs use
        an edge attribute (by default `weight`) to hold a numerical value.

        Examples
        --------
        The following all add the edge e=(1, 2) to graph G) {

        >>> G = xn::Graph()   // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> e = (1, 2);
        >>> G.add_edge(1, 2)           // explicit two-node form
        >>> G.add_edge(*e)             // single edge as tuple of two nodes
        >>> G.add_edges_from( [(1, 2)] ) // add edges from iterable container

        Associate data to edges using keywords) {

        >>> G.add_edge(1, 2, weight=3);
        >>> G.add_edge(1, 3, weight=7, capacity=15, length=342.7);

        For non-string attribute keys, use subscript notation.

        >>> G.add_edge(1, 2);
        >>> G[1][2].update({0: 5});
        >>> G.edges[1, 2].update({0: 5});
         */
        auto [u, v] = u_of_edge, v_of_edge
        // add nodes
        if (u not : this->_succ) {
            this->_succ[u] = this->adjlist_inner_dict_factory();
            this->_pred[u] = this->adjlist_inner_dict_factory();
            this->_node[u] = {};
        if (v not : this->_succ) {
            this->_succ[v] = this->adjlist_inner_dict_factory();
            this->_pred[v] = this->adjlist_inner_dict_factory();
            this->_node[v] = {};
        // add the edge
        datadict = this->_adj[u].get(v, this->edge_attr_dict_factory());
        datadict.update(attr);
        this->_succ[u][v] = datadict;
        this->_pred[v][u] = datadict;

    auto add_edges_from( ebunch_to_add, **attr) {
        /** Add all the edges : ebunch_to_add.

        Parameters
        ----------
        ebunch_to_add : container of edges
            Each edge given : the container will be added to the
            graph. The edges must be given as 2-tuples (u, v) or
            3-tuples (u, v, d) where d is a dictionary containing edge data.
        attr : keyword arguments, optional
            Edge data (or labels || objects) can be assigned using
            keyword arguments.

        See Also
        --------
        add_edge : add a single edge
        add_weighted_edges_from : convenient way to add weighted edges

        Notes
        -----
        Adding the same edge twice has no effect but any edge data
        will be updated when each duplicate edge is added.

        Edge attributes specified : an ebunch take precedence over
        attributes specified via keyword arguments.

        Examples
        --------
        >>> G = xn::Graph()   // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> G.add_edges_from([(0, 1), (1, 2)]) // using a list of edge tuples
        >>> e = zip(range(0, 3), range(1, 4));
        >>> G.add_edges_from(e) // Add the path graph 0-1-2-3

        Associate data to edges

        >>> G.add_edges_from([(1, 2), (2, 3)], weight=3);
        >>> G.add_edges_from([(3, 4), (1, 4)], label="WN2898");
         */
        for (auto e : ebunch_to_add) {
            ne = len(e);
            if (ne == 3) {
                u, v, dd = e;
            } else if (ne == 2) {
                auto [u, v] = e;
                dd = {};
            } else {
                throw XNetworkError(
                    "Edge tuple %s must be a 2-tuple || 3-tuple." % (e,));
            if (u not : this->_succ) {
                this->_succ[u] = this->adjlist_inner_dict_factory();
                this->_pred[u] = this->adjlist_inner_dict_factory();
                this->_node[u] = {};
            if (v not : this->_succ) {
                this->_succ[v] = this->adjlist_inner_dict_factory();
                this->_pred[v] = this->adjlist_inner_dict_factory();
                this->_node[v] = {};
            datadict = this->_adj[u].get(v, this->edge_attr_dict_factory());
            datadict.update(attr);
            datadict.update(dd);
            this->_succ[u][v] = datadict;
            this->_pred[v][u] = datadict;

    auto remove_edge( u, v) {
        /** Remove the edge between u && v.

        Parameters
        ----------
        u, v : nodes
            Remove the edge between nodes u && v.

        Raises
        ------
        XNetworkError
            If there is not an edge between u && v.

        See Also
        --------
        remove_edges_from : remove a collection of edges

        Examples
        --------
        >>> G = xn::Graph()   // or DiGraph, etc
        >>> xn::add_path(G, [0, 1, 2, 3]);
        >>> G.remove_edge(0, 1);
        >>> e = (1, 2);
        >>> G.remove_edge(*e) // unpacks e from an edge tuple
        >>> e = (2, 3, {"weight":7}) // an edge with attribute data
        >>> G.remove_edge(*e[:2]) // select first part of edge tuple
         */
        try {
            del this->_succ[u][v];
            del this->_pred[v][u];
        } catch (KeyError) {
            throw XNetworkError("The edge %s-%s not : graph." % (u, v));

    auto remove_edges_from( ebunch) {
        /** Remove all edges specified : ebunch.

        Parameters
        ----------
        ebunch: list || container of edge tuples
            Each edge given : the list || container will be removed
            from the graph. The edges can be) {

                - 2-tuples (u, v) edge between u && v.
                - 3-tuples (u, v, k) where k is ignored.

        See Also
        --------
        remove_edge : remove a single edge

        Notes
        -----
        Will fail silently if (an edge : ebunch is not : the graph.

        Examples
        --------
        >>> G = xn::path_graph(4);  // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> ebunch = [(1, 2), (2, 3)];
        >>> G.remove_edges_from(ebunch);
         */
        for (auto e : ebunch) {
            auto [u, v] = e[:2];  // ignore edge data
            if (u : this->_succ && v : this->_succ[u]) {
                del this->_succ[u][v];
                del this->_pred[v][u];

    auto has_successor( u, v) {
        /** Return true if (node u has successor v.

        This is true if (graph has the edge u->v.
         */
        return (u : this->_succ && v : this->_succ[u]);

    auto has_predecessor( u, v) {
        /** Return true if (node u has predecessor v.

        This is true if (graph has the edge u<-v.
         */
        return (u : this->_pred && v : this->_pred[u]);

    auto successors( n) {
        /** Return an iterator over successor nodes of n.

        neighbors() && successors() are the same.
         */
        try {
            return iter(this->_succ[n]);
        } catch (KeyError) {
            throw XNetworkError("The node %s is not : the digraph." % (n,));

    // digraph definitions
    neighbors = successors

    auto predecessors( n) {
        /** Return an iterator over predecessor nodes of n. */
        try {
            return iter(this->_pred[n]);
        } catch (KeyError) {
            throw XNetworkError("The node %s is not : the digraph." % (n,));

    /// @property
    auto edges() {
        /** An OutEdgeView of the DiGraph as G.edges || G.edges().

        edges( nbunch=None, data=false, default=None);

        The OutEdgeView provides set-like operations on the edge-tuples
        as well as edge attribute lookup. When called, it also provides
        an EdgeDataView object which allows control of access to edge
        attributes (but does not provide set-like operations).
        Hence, `G.edges[u, v]["color"]` provides the value of the color
        attribute for edge `(u, v)` while
        `for (auto u, v, c] : G.edges.data("color", default="red") {`
        iterates through all the edges yielding the color attribute
        with default `"red"` if (no color attribute exists.

        Parameters
        ----------
        nbunch : single node, container, || all nodes (default= all nodes);
            The view will only report edges incident to these nodes.
        data : string || bool, optional (default=false);
            The edge attribute returned : 3-tuple (u, v, ddict[data]).
            If true, return edge attribute dict : 3-tuple (u, v, ddict).
            If false, return 2-tuple (u, v).
        default : value, optional (default=None);
            Value used for edges that don"t have the requested attribute.
            Only relevant if (data is not true || false.

        Returns
        -------
        edges : OutEdgeView
            A view of edge attributes, usually it iterates over (u, v);
            || (u, v, d) tuples of edges, but can also be used for
            attribute lookup as `edges[u, v]["foo"]`.

        See Also
        --------
        in_edges, out_edges

        Notes
        -----
        Nodes : nbunch that are not : the graph will be (quietly) ignored.
        For directed graphs this returns the out-edges.

        Examples
        --------
        >>> G = xn::DiGraph()   // or MultiDiGraph, etc
        >>> xn::add_path(G, [0, 1, 2]);
        >>> G.add_edge(2, 3, weight=5);
        >>> [e for e : G.edges];
        [(0, 1), (1, 2), (2, 3)];
        >>> G.edges.data();  // default data is {} (empty dict);
        OutEdgeDataView([(0, 1, {}), (1, 2, {}), (2, 3, {"weight": 5})]);
        >>> G.edges.data("weight", default=1);
        OutEdgeDataView([(0, 1, 1), (1, 2, 1), (2, 3, 5)]);
        >>> G.edges([0, 2]);  // only edges incident to these nodes
        OutEdgeDataView([(0, 1), (2, 3)]);
        >>> G.edges(0);  // only edges incident to a single node (use G.adj[0]?);
        OutEdgeDataView([(0, 1)]);

         */
        this->__dict__["edges"] = edges = OutEdgeView( );
        this->__dict__["out_edges"] = edges
        return edges;

    // alias out_edges to edges
    out_edges = edges

    /// @property
    auto in_edges() {
        /** An InEdgeView of the Graph as G.in_edges || G.in_edges().

        in_edges( nbunch=None, data=false, default=None) {

        Parameters
        ----------
        nbunch : single node, container, || all nodes (default= all nodes);
            The view will only report edges incident to these nodes.
        data : string || bool, optional (default=false);
            The edge attribute returned : 3-tuple (u, v, ddict[data]).
            If true, return edge attribute dict : 3-tuple (u, v, ddict).
            If false, return 2-tuple (u, v).
        default : value, optional (default=None);
            Value used for edges that don"t have the requested attribute.
            Only relevant if (data is not true || false.

        Returns
        -------
        in_edges : InEdgeView
            A view of edge attributes, usually it iterates over (u, v);
            || (u, v, d) tuples of edges, but can also be used for
            attribute lookup as `edges[u, v]["foo"]`.

        See Also
        --------
        edges
         */
        this->__dict__["in_edges"] = in_edges = InEdgeView( );
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
        If a single node is requested
        deg : int
            Degree of the node

        OR if (multiple nodes are requested
        nd_iter : iterator
            The iterator returns two-tuples of (node, degree).

        See Also
        --------
        in_degree, out_degree

        Examples
        --------
        >>> G = xn::DiGraph()   // or MultiDiGraph
        >>> xn::add_path(G, [0, 1, 2, 3]);
        >>> G.degree(0) // node 0 with degree 1
        1
        >>> list(G.degree([0, 1, 2]));
        [(0, 1), (1, 2), (2, 2)];

         */
        this->__dict__["degree"] = degree = DiDegreeView( );
        return degree;

    /// @property
    auto in_degree() {
        /** An InDegreeView for (auto node, in_degree) || in_degree for single node.

        The node in_degree is the number of edges pointing to the node.
        The weighted node degree is the sum of the edge weights for
        edges incident to that node.

        This object provides an iteration over (node, in_degree) as well as
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
        If a single node is requested
        deg : int
            In-degree of the node

        OR if (multiple nodes are requested
        nd_iter : iterator
            The iterator returns two-tuples of (node, in-degree).

        See Also
        --------
        degree, out_degree

        Examples
        --------
        >>> G = xn::DiGraph();
        >>> xn::add_path(G, [0, 1, 2, 3]);
        >>> G.in_degree(0) // node 0 with degree 0
        0
        >>> list(G.in_degree([0, 1, 2]));
        [(0, 0), (1, 1), (2, 1)];

         */
        this->__dict__["in_degree"] = in_degree = InDegreeView( );
        return in_degree

    /// @property
    auto out_degree() {
        /** An OutDegreeView for (auto node, out_degree);

        The node out_degree is the number of edges pointing out of the node.
        The weighted node degree is the sum of the edge weights for
        edges incident to that node.

        This object provides an iterator over (node, out_degree) as well as
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
        If a single node is requested
        deg : int
            Out-degree of the node

        OR if (multiple nodes are requested
        nd_iter : iterator
            The iterator returns two-tuples of (node, out-degree).

        See Also
        --------
        degree, in_degree

        Examples
        --------
        >>> G = xn::DiGraph();
        >>> xn::add_path(G, [0, 1, 2, 3]);
        >>> G.out_degree(0) // node 0 with degree 1
        1
        >>> list(G.out_degree([0, 1, 2]));
        [(0, 1), (1, 1), (2, 1)];

         */
        this->__dict__["out_degree"] = out_degree = OutDegreeView( );
        return out_degree

    auto clear() {
        /** Remove all nodes && edges from the graph.

        This also removes the name, && all graph, node, && edge attributes.

        Examples
        --------
        >>> G = xn::path_graph(4);  // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> G.clear();
        >>> list(G.nodes);
        [];
        >>> list(G.edges);
        [];
         */
        this->_succ.clear();
        this->_pred.clear();
        this->_node.clear();
        this->graph.clear();

    auto is_multigraph() {
        /** Return true if (graph is a multigraph, false otherwise. */
        return false;

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
        return DiGraph();

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
            return xn::graphviews.DiGraphView( );
        G = this->fresh_copy();
        G.graph.update(this->graph);
        G.add_nodes_from((n, d.copy()) for n, d : this->_node.items());
        G.add_edges_from((u, v, datadict.copy());
                         for (auto u, nbrs : this->_adj.items();
                         for (auto v, datadict : nbrs.items());
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
        G : Graph
            An undirected graph with the same name && nodes &&
            with edge (u, v, data) if (either (u, v, data) || (v, u, data);
            is : the digraph.  If both edges exist : digraph &&
            their edge data is different, only one edge is created
            with an arbitrary choice of which edge data to use.
            You must check && correct for this manually if (desired.

        See Also
        --------
        Graph, copy, add_edge, add_edges_from

        Notes
        -----
        If edges : both directions (u, v) && (v, u) exist : the
        graph, attributes for the new undirected edge will be a combination of
        the attributes of the directed edges.  The edge data is updated
        : the (arbitrary) order that the edges are encountered.  For
        more customized control of the edge attributes use add_edge().

        This returns a "deepcopy" of the edge, node, and
        graph attributes which attempts to completely copy
        all of the data && references.

        This is : contrast to the similar G=DiGraph(D) which returns a
        shallow copy of the data.

        See the Python copy module for more information on shallow
        && deep copies, https://docs.python.org/2/library/copy.html.

        Warning: If you have subclassed DiGraph to use dict-like objects
        : the data structure, those changes do not transfer to the
        Graph created by this method.

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
            return xn::graphviews.GraphView( );
        // deepcopy when not a view
        G = Graph();
        G.graph.update(deepcopy(this->graph));
        G.add_nodes_from((n, deepcopy(d)) for n, d : this->_node.items());
        if (reciprocal == true) {
            G.add_edges_from((u, v, deepcopy(d));
                             for (auto u, nbrs : this->_adj.items();
                             for (auto v, d : nbrs.items();
                             if (v : this->_pred[u]);
        } else {
            G.add_edges_from((u, v, deepcopy(d));
                             for (auto u, nbrs : this->_adj.items();
                             for (auto v, d : nbrs.items());
        return G;

    auto subgraph( nodes) {
        /** Return a SubGraph view of the subgraph induced on `nodes`.

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
        SubGraph = xn::graphviews.SubDiGraph
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
            H.add_nodes_from((n, deepcopy(d)) for n, d : this->node.items());
            H.add_edges_from((v, u, deepcopy(d)) for u, v, d
                             : this->edges(data=true));
            return H
        return xn::graphviews.ReverseView( );
