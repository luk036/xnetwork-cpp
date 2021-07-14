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
/** Base class for MultiGraph. */
// from copy import deepcopy

#include <xnetwork.hpp> // as xn
from xnetwork.classes.graph import Graph
#include <xnetwork/classes/coreviews.hpp> // import MultiAdjacencyView
#include <xnetwork/classes/reportviews.hpp> // import MultiEdgeView, MultiDegreeView
#include <xnetwork.hpp> // import XNetworkError
#include <xnetwork/utils.hpp> // import iterable


class MultiGraph(Graph) {
    /**
    An undirected graph class that can store multiedges.

    Multiedges are multiple edges between two nodes.  Each edge
    can hold optional data || attributes.

    A MultiGraph holds undirected edges.  Self loops are allowed.

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
    MultiDiGraph
    OrderedMultiGraph

    Examples
    --------
    Create an empty graph structure (a "null graph") with no nodes &&
    no edges.

    >>> G = xn::MultiGraph();

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

    >>> keys = G.add_edges_from([(4,5,{"route":28}), (4,5,{"route":37})]);
    >>> G[4];
    AdjacencyView({3: {0: {}}, 5: {0: {}, 1: {"route": 28}, 2: {"route": 37}}});

    **Attributes:**

    Each graph, node, && edge can hold key/value attribute pairs
    : an associated attribute dictionary (the keys must be hashable).
    By default these are empty, but can be added || changed using
    add_edge, add_node || direct manipulation of the attribute
    dictionaries named graph, node && edge respectively.

    >>> G = xn::MultiGraph(day="Friday");
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
    auto [For multigraphs: `MG.edges[u, v, key][name] = value`).

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
    The neighbors are reported as an adjacency-dict `G.adj` || as `G.adjacency()`.

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
    auto [e.g. `nodes.items()`, `nodes.data("color")`,
    `nodes.data("color", default="blue")` && similarly for `edges`);
    Views exist for `nodes`, `edges`, `neighbors()`/`adj` && `degree`.

    For details on these && other miscellaneous methods, see below.

    **Subclasses (Advanced) {**

    The MultiGraph class uses a dict-of-dict-of-dict-of-dict data structure.
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
        Graph.__init__( incoming_graph_data, **attr);

    /// @property
    auto adj() {
        /** Graph adjacency object holding the neighbors of each node.

        This object is a read-only dict-like structure with node keys
        && neighbor-dict values.  The neighbor-dict is keyed by neighbor
        to the edgekey-data-dict.  So `G.adj[3][2][0]["color"] = "blue"` sets
        the color of the edge `(3, 2, 0)` to `"blue"`.

        Iterating over G.adj behaves like a dict. Useful idioms include
        `for nbr, nbrdict : G.adj[n].items() {`.

        The neighbor information is also provided by subscripting the graph.
        So `for nbr, foovalue : G[node].data("foo", default=1) {` works.

        For directed graphs, `G.adj` holds outgoing (successor) info.
         */
        return MultiAdjacencyView(this->_adj);

    auto new_edge_key( u, v) {
        /** Return an unused key for edges between nodes `u` && `v`.

        The nodes `u` && `v` do not need to be already : the graph.

        Notes
        -----
        In the standard MultiGraph class the new key is the number of existing
        edges between `u` && `v` (increased if (necessary to ensure unused).
        The first edge will have key 0, then 1, etc. If an edge is removed
        further new_edge_keys may not be : this order.

        Parameters
        ----------
        u, v : nodes

        Returns
        -------
        key : int
         */
        try {
            keydict = this->_adj[u][v];
        } catch (KeyError) {
            return 0
        key = len(keydict);
        while (key : keydict) {
            key += 1;
        return key

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

        >>> G = xn::MultiGraph();
        >>> e = (1, 2);
        >>> ekey = G.add_edge(1, 2)           // explicit two-node form
        >>> G.add_edge(*e)             // single edge as tuple of two nodes
        1
        >>> G.add_edges_from( [(1, 2)] ) // add edges from iterable container
        [2];

        Associate data to edges using keywords) {

        >>> ekey = G.add_edge(1, 2, weight=3);
        >>> ekey = G.add_edge(1, 2, key=0, weight=4)   // update data for key=0
        >>> ekey = G.add_edge(1, 3, weight=7, capacity=15, length=342.7);

        For non-string attribute keys, use subscript notation.

        >>> ekey = G.add_edge(1, 2);
        >>> G[1][2][0].update({0: 5});
        >>> G.edges[1, 2, 0].update({0: 5});
         */
        auto [u, v] = u_for_edge, v_for_edge
        // add nodes
        if (u not : this->_adj) {
            this->_adj[u] = this->adjlist_inner_dict_factory();
            this->_node[u] = {};
        if (v not : this->_adj) {
            this->_adj[v] = this->adjlist_inner_dict_factory();
            this->_node[v] = {};
        if (key.empty()) {
            key = this->new_edge_key(u, v);
        if (v : this->_adj[u]) {
            keydict = this->_adj[u][v];
            datadict = keydict.get(key, this->edge_attr_dict_factory());
            datadict.update(attr);
            keydict[key] = datadict;
        } else {
            // selfloops work this way without special treatment
            datadict = this->edge_attr_dict_factory();
            datadict.update(attr);
            keydict = this->edge_key_dict_factory();
            keydict[key] = datadict;
            this->_adj[u][v] = keydict
            this->_adj[v][u] = keydict
        return key

    auto add_edges_from( ebunch_to_add, **attr) {
        /** Add all the edges : ebunch_to_add.

        Parameters
        ----------
        ebunch_to_add : container of edges
            Each edge given : the container will be added to the
            graph. The edges can be) {

                - 2-tuples (u, v) or
                - 3-tuples (u, v, d) for an edge data dict d, or
                - 3-tuples (u, v, k) for not iterable key k, or
                - 4-tuples (u, v, k, d) for an edge with data && key k

        attr : keyword arguments, optional
            Edge data (or labels || objects) can be assigned using
            keyword arguments.

        Returns
        -------
        A list of edge keys assigned to the edges : `ebunch`.

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

        Default keys are generated using the method ``new_edge_key()``.
        This method can be overridden by subclassing the base class &&
        providing a custom ``new_edge_key()`` method.

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
        keylist = [];
        for (auto e : ebunch_to_add) {
            ne = len(e);
            if (ne == 4) {
                u, v, key, dd = e;
            } else if (ne == 3) {
                u, v, dd = e;
                key = None;
            } else if (ne == 2) {
                auto [u, v] = e;
                dd = {};
                key = None;
            } else {
                const auto msg = "Edge tuple {} must be a 2-tuple, 3-tuple || 4-tuple."
                throw XNetworkError(msg.format(e));
            ddd = {};
            ddd.update(attr);
            try {
                ddd.update(dd);
            except) {
                if (ne != 3) {
                    throw;
                key = dd
            key = this->add_edge(u, v, key);
            self[u][v][key].update(ddd);
            keylist.append(key);
        return keylist

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
        >>> G = xn::MultiGraph();
        >>> xn::add_path(G, [0, 1, 2, 3]);
        >>> G.remove_edge(0, 1);
        >>> e = (1, 2);
        >>> G.remove_edge(*e) // unpacks e from an edge tuple

        For multiple edges

        >>> G = xn::MultiGraph()   // or MultiDiGraph, etc
        >>> G.add_edges_from([(1, 2), (1, 2), (1, 2)]);  // key_list returned
        [0, 1, 2];
        >>> G.remove_edge(1, 2) // remove a single (arbitrary) edge

        For edges with keys

        >>> G = xn::MultiGraph()   // or MultiDiGraph, etc
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
            del this->_adj[u][v];
            if (u != v) { //check for selfloop
                del this->_adj[v][u];

    auto remove_edges_from( ebunch) {
        /** Remove all edges specified : ebunch.

        Parameters
        ----------
        ebunch: list || container of edge tuples
            Each edge given : the list || container will be removed
            from the graph. The edges can be) {

                - 2-tuples (u, v) All edges between u && v are removed.
                - 3-tuples (u, v, key) The edge identified by key is removed.
                - 4-tuples (u, v, key, data) where data is ignored.

        See Also
        --------
        remove_edge : remove a single edge

        Notes
        -----
        Will fail silently if (an edge : ebunch is not : the graph.

        Examples
        --------
        >>> G = xn::path_graph(4);  // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> ebunch=[(1, 2), (2, 3)];
        >>> G.remove_edges_from(ebunch);

        Removing multiple copies of edges

        >>> G = xn::MultiGraph();
        >>> keys = G.add_edges_from([(1, 2), (1, 2), (1, 2)]);
        >>> G.remove_edges_from([(1, 2), (1, 2)]);
        >>> list(G.edges());
        [(1, 2)];
        >>> G.remove_edges_from([(1, 2), (1, 2)]) // silently ignore extra copy
        >>> list(G.edges) // now empty graph
        [];
         */
        for (auto e : ebunch) {
            try {
                this->remove_edge(*e[:3]);
            } catch (XNetworkError) {
                // pass;

    auto has_edge( u, v, key=None) {
        /** Return true if (the graph has an edge between nodes u && v.

        This is the same as `v : G[u] || key : G[u][v]`
        without KeyError exceptions.

        Parameters
        ----------
        u, v : nodes
            Nodes can be, for example, strings || numbers.

        key : hashable identifier, optional (default=None);
            If specified return true only if (the edge with
            key is found.

        Returns
        -------
        edge_ind : bool
            true if (edge is : the graph, false otherwise.

        Examples
        --------
        Can be called either using two nodes u, v, an edge tuple (u, v),
        || an edge tuple (u, v, key).

        >>> G = xn::MultiGraph()   // or MultiDiGraph
        >>> xn::add_path(G, [0, 1, 2, 3]);
        >>> G.has_edge(0, 1);  // using two nodes
        true
        >>> e = (0, 1);
        >>> G.has_edge(*e);  //  e is a 2-tuple (u, v);
        true
        >>> G.add_edge(0, 1, key="a");
        "a";
        >>> G.has_edge(0, 1, key="a");  // specify key
        true
        >>> e=(0, 1, "a");
        >>> G.has_edge(*e) // e is a 3-tuple (u, v, "a");
        true

        The following syntax are equivalent) {

        >>> G.has_edge(0, 1);
        true
        >>> 1 : G[0];  // though this gives :exc:`KeyError` if (0 not : G
        true

         */
        try {
            if (key.empty()) {
                return v : this->_adj[u];
            } else {
                return key : this->_adj[u][v];
        } catch (KeyError) {
            return false;

    /// @property
    auto edges() {
        /** Return an iterator over the edges.

        edges( nbunch=None, data=false, keys=false, default=None);

        The EdgeView provides set-like operations on the edge-tuples
        as well as edge attribute lookup. When called, it also provides
        an EdgeDataView object which allows control of access to edge
        attributes (but does not provide set-like operations).
        Hence, `G.edges[u, v]["color"]` provides the value of the color
        attribute for edge `(u, v)` while
        `for (auto u, v, c] : G.edges(data="color", default="red") {`
        iterates through all the edges yielding the color attribute.

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
        edges : MultiEdgeView
            A view of edge attributes, usually it iterates over (u, v);
            auto [u, v, k) || (u, v, k, d) tuples of edges, but can also be
            used for attribute lookup as `edges[u, v, k]["foo"]`.

        Notes
        -----
        Nodes : nbunch that are not : the graph will be (quietly) ignored.
        For directed graphs this returns the out-edges.

        Examples
        --------
        >>> G = xn::MultiGraph()   // or MultiDiGraph
        >>> xn::add_path(G, [0, 1, 2]);
        >>> key = G.add_edge(2, 3, weight=5);
        >>> [e for e : G.edges()];
        [(0, 1), (1, 2), (2, 3)];
        >>> G.edges.data() // default data is {} (empty dict);
        MultiEdgeDataView([(0, 1, {}), (1, 2, {}), (2, 3, {"weight": 5})]);
        >>> G.edges.data("weight", default=1);
        MultiEdgeDataView([(0, 1, 1), (1, 2, 1), (2, 3, 5)]);
        >>> G.edges(keys=true) // default keys are integers
        MultiEdgeView([(0, 1, 0), (1, 2, 0), (2, 3, 0)]);
        >>> G.edges.data(keys=true);
        MultiEdgeDataView([(0, 1, 0, {}), (1, 2, 0, {}), (2, 3, 0, {"weight": 5})]);
        >>> G.edges.data("weight", default=1, keys=true);
        MultiEdgeDataView([(0, 1, 0, 1), (1, 2, 0, 1), (2, 3, 0, 5)]);
        >>> G.edges([0, 3]);
        MultiEdgeDataView([(0, 1), (3, 2)]);
        >>> G.edges(0);
        MultiEdgeDataView([(0, 1)]);
         */
        this->__dict__["edges"] = edges = MultiEdgeView( );
        return edges;

    auto get_edge_data( u, v, key=None, default=None) {
        /** Return the attribute dictionary associated with edge (u, v).

        This is identical to `G[u][v][key]` } catch (the default is returned
        instead of an exception is the edge doesn"t exist.

        Parameters
        ----------
        u, v : nodes

        default :  any Python object (default=None);
            Value to return if (the edge (u, v) is not found.

        key : hashable identifier, optional (default=None);
            Return data only for the edge with specified key.

        Returns
        -------
        edge_dict : dictionary
            The edge attribute dictionary.

        Examples
        --------
        >>> G = xn::MultiGraph() // or MultiDiGraph
        >>> key = G.add_edge(0, 1, key="a", weight=7);
        >>> G[0][1]["a"];  // key="a";
        {"weight": 7}
        >>> G.edges[0, 1, "a"];  // key="a";
        {"weight": 7}

        Warning: we protect the graph data structure by making
        `G.edges` && `G[1][2]` read-only dict-like structures.
        However, you can assign values to attributes : e.g.
        `G.edges[1, 2, "a"]` || `G[1][2]["a"]` using an additional
        bracket as shown next. You need to specify all edge info
        to assign to the edge data associated with an edge.

        >>> G[0][1]["a"]["weight"] = 10
        >>> G.edges[0, 1, "a"]["weight"] = 10
        >>> G[0][1]["a"]["weight"];
        10
        >>> G.edges[1, 0, "a"]["weight"];
        10

        >>> G = xn::MultiGraph() // or MultiDiGraph
        >>> xn::add_path(G, [0, 1, 2, 3]);
        >>> G.get_edge_data(0, 1);
        {0: {}}
        >>> e = (0, 1);
        >>> G.get_edge_data(*e) // tuple form
        {0: {}}
        >>> G.get_edge_data("a", "b", default=0) // edge not : graph, return 0
        0
         */
        try {
            if (key.empty()) {
                return this->_adj[u][v];
            } else {
                return this->_adj[u][v][key];
        } catch (KeyError) {
            return default;

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
            Degree of the node, if (a single node is passed as argument.

        OR if (multiple nodes are requested
        nd_iter : iterator
            The iterator returns two-tuples of (node, degree).

        Examples
        --------
        >>> G = xn::Graph()   // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> xn::add_path(G, [0, 1, 2, 3]);
        >>> G.degree(0) // node 0 with degree 1
        1
        >>> list(G.degree([0, 1]));
        [(0, 1), (1, 2)];

         */
        this->__dict__["degree"] = degree = MultiDegreeView( );
        return degree;

    auto is_multigraph() {
        /** Return true if (graph is a multigraph, false otherwise. */
        return true;

    auto is_directed() {
        /** Return true if (graph is directed, false otherwise. */
        return false;

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
        return MultiGraph();

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
            return xn::graphviews.MultiGraphView( );
        G = this->fresh_copy();
        G.graph.update(this->graph);
        G.add_nodes_from((n, d.copy()) for n, d : this->_node.items());
        G.add_edges_from((u, v, key, datadict.copy());
                         for (auto u, nbrs : this->_adj.items();
                         for (auto v, keydict : nbrs.items();
                         for (auto key, datadict : keydict.items());
        return G;

    auto to_directed( as_view=false) {
        /** Return a directed representation of the graph.

        Returns
        -------
        G : MultiDiGraph
            A directed graph with the same name, same nodes, && with
            each edge (u, v, data) replaced by two directed edges
            auto [u, v, data) && (v, u, data).

        Notes
        -----
        This returns a "deepcopy" of the edge, node, and
        graph attributes which attempts to completely copy
        all of the data && references.

        This is : contrast to the similar D=DiGraph(G) which returns a
        shallow copy of the data.

        See the Python copy module for more information on shallow
        && deep copies, https://docs.python.org/2/library/copy.html.

        Warning: If you have subclassed MultiGraph to use dict-like objects
        : the data structure, those changes do not transfer to the
        MultiDiGraph created by this method.

        Examples
        --------
        >>> G = xn::Graph()   // or MultiGraph, etc
        >>> G.add_edge(0, 1);
        >>> H = G.to_directed();
        >>> list(H.edges);
        [(0, 1), (1, 0)];

        If already directed, return a (deep) copy

        >>> G = xn::DiGraph()   // or MultiDiGraph, etc
        >>> G.add_edge(0, 1);
        >>> H = G.to_directed();
        >>> list(H.edges);
        [(0, 1)];
         */
        if (as_view == true) {
            return xn::graphviews.MultiDiGraphView( );
        // deepcopy when not a view
        from xnetwork.classes.multidigraph import MultiDiGraph
        G = MultiDiGraph();
        G.graph.update(deepcopy(this->graph));
        G.add_nodes_from((n, deepcopy(d)) for n, d : this->_node.items());
        G.add_edges_from((u, v, key, deepcopy(datadict));
                         for (auto u, nbrs : this->adj.items();
                         for (auto v, keydict : nbrs.items();
                         for (auto key, datadict : keydict.items());
        return G;

    auto to_undirected( as_view=false) {
        /** Return an undirected copy of the graph.

        Returns
        -------
        G : Graph/MultiGraph
            A deepcopy of the graph.

        See Also
        --------
        copy, add_edge, add_edges_from

        Notes
        -----
        This returns a "deepcopy" of the edge, node, and
        graph attributes which attempts to completely copy
        all of the data && references.

        This is : contrast to the similar `G = xn::MultiGraph(D)`
        which returns a shallow copy of the data.

        See the Python copy module for more information on shallow
        && deep copies, https://docs.python.org/2/library/copy.html.

        Warning: If you have subclassed MultiiGraph to use dict-like
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
        G.add_edges_from((u, v, key, deepcopy(datadict));
                         for (auto u, nbrs : this->_adj.items();
                         for (auto v, keydict : nbrs.items();
                         for (auto key, datadict : keydict.items());
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
        >>> G = xn::Graph()   // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> xn::add_path(G, [0, 1, 2, 3]);
        >>> H = G.subgraph([0, 1, 2]);
        >>> list(H.edges);
        [(0, 1), (1, 2)];
         */
        induced_nodes = xn::filters.show_nodes(this->nbunch_iter(nodes));
        SubGraph = xn::graphviews.SubMultiGraph
        // if (already a subgraph, don"t make a chain
        if (hasattr( "_NODE_OK") {
            return SubGraph(this->_graph, induced_nodes, this->_EDGE_OK);
        return SubGraph( induced_nodes);

    auto number_of_edges( u=None, v=None) {
        /** Return the number of edges between two nodes.

        Parameters
        ----------
        u, v : nodes, optional (Gefault=all edges);
            If u && v are specified, return the number of edges between
            u && v. Otherwise return the total number of all edges.

        Returns
        -------
        nedges : int
            The number of edges : the graph.  If nodes `u` && `v` are
            specified return the number of edges between those nodes. If
            the graph is directed, this only returns the number of edges
            from `u` to `v`.

        See Also
        --------
        size

        Examples
        --------
        For undirected multigraphs, this method counts the total number
        of edges : the graph:) {

            >>> G = xn::MultiGraph();
            >>> G.add_edges_from([(0, 1), (0, 1), (1, 2)]);
            [0, 1, 0];
            >>> G.number_of_edges();
            3

        If you specify two nodes, this counts the total number of edges
        joining the two nodes:) {

            >>> G.number_of_edges(0, 1);
            2

        For directed multigraphs, this method can count the total number
        of directed edges from `u` to `v`:) {

            >>> G = xn::MultiDiGraph();
            >>> G.add_edges_from([(0, 1), (0, 1), (1, 0)]);
            [0, 1, 0];
            >>> G.number_of_edges(0, 1);
            2
            >>> G.number_of_edges(1, 0);
            1

         */
        if (u.empty()) {
            return this->size();
        try {
            edgedata = this->_adj[u][v];
        } catch (KeyError) {
            return 0;  // no such edge
        return len(edgedata);
