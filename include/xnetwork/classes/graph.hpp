#pragma once

#include <any>
#include <cassert>
#include <py2cpp/py2cpp.hpp>
// #include <range/v3/view/enumerate.hpp>
#include <string_view>
#include <type_traits>
#include <vector>
#include <xnetwork/classes/coreviews.hpp> // import AtlasView, AdjacencyView
#include <xnetwork/classes/reportviews.hpp> // import NodeView, EdgeView, DegreeView

namespace xn
{

/*! Base class for undirected graphs.

    A Graph stores nodes and edges with optional data, or attributes.

    Graphs hold undirected edges.  Self loops are allowed but multiple
    (parallel) edges are not.

    Nodes can be arbitrary (hashable) C++ objects with optional
    key/value attributes. By convention `None` is not used as a node.

    Edges are represented as links between nodes with optional
    key/value attributes.

    Parameters
    ----------
    node_container : input graph (optional, default: None)
        Data to initialize graph. If None (default) an empty
        graph is created.  The data can be any format that is supported
        by the to_networkx_graph() function, currently including edge list,
        dict of dicts, dict of lists, NetworkX graph, NumPy matrix
        or 2d ndarray, SciPy sparse matrix, or PyGraphviz graph.

    See Also
    --------
    DiGraph
    MultiGraph
    MultiDiGraph
    OrderedGraph

    Examples
    --------
    Create an empty graph structure (a "null graph") with 5 nodes and
    no edges.

    >>> auto v = std::vector{3, 4, 2, 8};
    >>> auto G = xn::Graph(v);

    >>> auto va = py::dict{{3, 0.1}, {4, 0.5}, {2, 0.2}};
    >>> auto G = xn::Graph(va);

    >>> auto r = py::range(100);
    >>> auto G = xn::Graph(r);

    G can be grown in several ways.

    **Nodes:**

    Add one node at a time:

    >>> G.add_node(1)

    Add the nodes from any container (a list, dict, set or
    even the lines from a file or the nodes from another graph).

    >>> G.add_nodes_from([2, 3])
    >>> G.add_nodes_from(range(100, 110))
    >>> H = xn::path_graph(10)
    >>> G.add_nodes_from(H)

    In addition to strings and integers any hashable C++ object
    (except None) can represent a node, e.g. a customized node object,
    or even another Graph.

    >>> G.add_node(H)

    **Edges:**

    G can also be grown by adding edges.

    Add one edge,

    >>> G.add_edge(1, 2);

    a list of edges,

    >>> G.add_edges_from([(1, 2), (1, 3)]);

    or a collection of edges,

    >>> G.add_edges_from(H.edges());

    If some edges connect nodes not yet in the graph, the nodes
    are added automatically.  There are no errors when adding
    nodes or edges that already exist.

    **Attributes:**

    Each graph can hold key/value attribute pairs
    in an associated attribute dictionary (the keys must be hashable).
    By default these are empty, but can be added or changed using
    direct manipulation of the attribute
    dictionaries named graph, node and edge respectively.

    >>> G.graph["day"] = std::any("Friday");
    {'day': 'Friday'}

    **Subclasses (Advanced):**

    The Graph class uses a container-of-container-of-container data structure.
    The outer dict (node_dict) holds adjacency information keyed by node.
    The next dict (adjlist_dict) represents the adjacency information and holds
    edge data keyed by neighbor.  The inner dict (edge_attr_dict) represents
    the edge data and holds edge attribute values keyed by attribute names.

    Each of these three dicts can be replaced in a subclass by a user defined
    dict-like object. In general, the dict-like features should be
    maintained but extra features can be added. To replace one of the
    dicts create a new graph class by changing the class(!) variable
    holding the factory for that dict-like structure. The variable names are
    node_dict_factory, node_attr_dict_factory, adjlist_inner_dict_factory,
    adjlist_outer_dict_factory, edge_attr_dict_factory and
   graph_attr_dict_factory.

    node_dict_factory : function, (default: dict)
        Factory function to be used to create the dict containing node
        attributes, keyed by node id.
        It should require no arguments and return a dict-like object

    node_attr_dict_factory: function, (default: dict)
        Factory function to be used to create the node attribute
        dict which holds attribute values keyed by attribute name.
        It should require no arguments and return a dict-like object

    adjlist_outer_dict_factory : function, (default: dict)
        Factory function to be used to create the outer-most dict
        in the data structure that holds adjacency info keyed by node.
        It should require no arguments and return a dict-like object.

    adjlist_inner_dict_factory : function, (default: dict)
        Factory function to be used to create the adjacency list
        dict which holds edge data keyed by neighbor.
        It should require no arguments and return a dict-like object

    edge_attr_dict_factory : function, (default: dict)
        Factory function to be used to create the edge attribute
        dict which holds attribute values keyed by attribute name.
        It should require no arguments and return a dict-like object.

    graph_attr_dict_factory : function, (default: dict)
        Factory function to be used to create the graph attribute
        dict which holds attribute values keyed by attribute name.
        It should require no arguments and return a dict-like object.

    Typically, if your extension doesn't impact the data structure all
    methods will inherit without issue except: `to_directed/to_undirected`.
    By default these methods create a DiGraph/Graph class and you probably
    want them to create your extension of a DiGraph/Graph. To facilitate
    this we define two class variables that you can set in your subclass.

    to_directed_class : callable, (default: DiGraph or MultiDiGraph)
        Class to create a new graph structure in the `to_directed` method.
        If `None`, a NetworkX class (DiGraph or MultiDiGraph) is used.

    to_undirected_class : callable, (default: Graph or MultiGraph)
        Class to create a new graph structure in the `to_undirected` method.
        If `None`, a NetworkX class (Graph or MultiGraph) is used.

    Examples
    --------

    Create a low memory graph class that effectively disallows edge
    attributes by using a single attribute dict for all edges.
    This reduces the memory used, but you lose edge attributes.

    >>> class ThinGraph(xn::Graph):
    ...     all_edge_dict = {'weight': 1}
    ...     def single_edge_dict(self):
    ...         return self.all_edge_dict
    ...     edge_attr_dict_factory = single_edge_dict
    >>> G = ThinGraph()
    >>> G.add_edge(2, 1)
    >>> G[2][1]
    {'weight': 1}
    >>> G.add_edge(2, 2)
    >>> G[2][1] is G[2][2]
    True

    Please see :mod:`~networkx.classes.ordered` for more examples of
    creating graph subclasses by overwriting the base class `dict` with
    a dictionary-like object.
*/

struct object : py::dict<const char*, std::any>
{
};

template <typename _nodeview_t,
    typename adjlist_t = py::set<Value_type<_nodeview_t>>,
    typename adjlist_outer_dict_factory =
        py::dict<Value_type<_nodeview_t>, adjlist_t>>
class Graph : public object
{
  public:
    using nodeview_t = _nodeview_t;
    using Node = typename nodeview_t::value_type; // luk
    using dict = py::dict<const char*, std::any>;
    using graph_attr_dict_factory = dict;
    // using edge_attr_dict_factory = dict;
    // using node_attr_dict_factory = dict;
    // using node_dict_factory = py::dict<Node, node_attr_dict_factory>;
    // using adjlist_inner_dict_factory = py::dict<Node,
    // edge_attr_dict_factory>;
    using adjlist_inner_dict_factory = adjlist_t;
    using key_type = typename adjlist_t::key_type;
    using value_type = typename adjlist_t::value_type;
    using edge_t = std::pair<Node, Node>;
    using node_t = Node;

    size_t _num_of_edges = 0;

    // std::vector<Node > _Nodes{};
    nodeview_t _node;
    graph_attr_dict_factory graph {}; // dictionary for graph attributes
    // node_dict_factory _node{};  // empty node attribute dict
    adjlist_outer_dict_factory _adj; // empty adjacency dict

    // auto __getstate__() {
    //     attr = this->__dict__.copy();
    //     // remove lazy property attributes
    //     if ("nodes" : attr) {
    //         del attr["nodes"];
    //     }
    //     if ("edges" : attr) {
    //         del attr["edges"];
    //     }
    //     if ("degree" : attr) {
    //         del attr["degree"];
    //     }
    //     return attr;
    // }

    /*! Initialize a graph with edges, name, or graph attributes.

        Parameters
        ----------
        node_container : input nodes

        Examples
        --------
        >>> v = std::vector{5, 3, 2};
        >>> G = xn::Graph(v);  // or DiGraph, MultiGraph, MultiDiGraph, etc

        >>> r = py::range(100);
        >>> G = xn::Graph(r);  // or DiGraph, MultiGraph, MultiDiGraph, etc
    */
    explicit Graph(const nodeview_t& Nodes)
        : _node {Nodes}
        , _adj {} // py::dict???
    {
    }

    explicit Graph(uint32_t num_nodes)
        : _node {py::range<uint32_t>(num_nodes)}
        , _adj(num_nodes) // std::vector
    {
    }

    // Graph(const Graph&) = delete;            // don't copy
    // Graph& operator=(const Graph&) = delete; // don't copy
    // Graph(Graph&&) noexcept = default;

    /*!
     * @brief For compatible with BGL adaptor
     *
     * @param[in] e
     * @return edge_t&
     */
    static auto end_points(edge_t& e) -> edge_t&
    {
        return e;
    }

    /*!
     * @brief For compatible with BGL adaptor
     *
     * @param[in] e
     * @return edge_t&
     */
    static auto end_points(const edge_t& e) -> const edge_t&
    {
        return e;
    }

    /// @property
    /*! Graph adjacency object holding the neighbors of each node.

        This object is a read-only dict-like structure with node keys
        and neighbor-dict values.  The neighbor-dict is keyed by neighbor
        to the edge-data-dict.  So `G.adj[3][2]['color'] = 'blue'` sets
        the color of the edge `(3, 2)` to `"blue"`.

        Iterating over G.adj behaves like a dict. Useful idioms include
        `for nbr, datadict in G.adj[n].items():`.

        The neighbor information is also provided by subscripting the graph.
        So `for nbr, foovalue in G[node].data('foo', default=1):` works.

        For directed graphs, `G.adj` holds outgoing (successor) info.
    */
    auto adj() const
    {
        using T = std::remove_reference_t<decltype(this->_adj)>;
        return AdjacencyView<const T&>(this->_adj);
    }

    auto adj()
    {
        using T = std::remove_cv_t<decltype(this->_adj)>;
        return AdjacencyView<T>(this->_adj);
    }

    auto _nodes_nbrs() const
    {
        // @TODO support py:dict
        return py::enumerate(this->_adj);
    }

    // auto null_vertex() const -> const Node&
    // {
    //     return *(this->_node.end());
    // }

    // Node& null_vertex()
    // {
    //     return *(this->_node.end());
    // }

    /// @property
    auto get_name()
    {
        /*! String identifier of the graph.

        This graph attribute appears : the attribute dict G.graph
        keyed by the string `"name"`. as well as an attribute (technically
        a property) `G.name`. This is entirely user controlled.
         */
        if (!this->graph.contains("name"))
        {
            return "";
        }
        return std::any_cast<const char*>(this->graph["name"]);
    }

    // @name.setter
    auto set_name(std::string_view s)
    {
        this->graph["name"] = std::any(s);
    }

    /*! Iterate over the nodes. Use: "for (const auto& n : G)".
     *
    Returns
    -------
    niter : iterator
        An iterator over all nodes : the graph.

    Examples
    --------
    >>> G = xn::path_graph(4);  // or DiGraph, MultiGraph, MultiDiGraph, etc
    >>> [n for n : G];
    [0, 1, 2, 3];
    >>> list(G);
    [0, 1, 2, 3];
     */
    auto begin() const
    {
        return std::begin(this->_node);
    }

    auto end() const
    {
        return std::end(this->_node);
    }

    /*! Return true if (n is a node, false otherwise. Use: "n : G".

    Examples
    --------
    >>> G = xn::path_graph(4);  // or DiGraph, MultiGraph, MultiDiGraph, etc
    >>> 1 : G
    true
     */
    auto contains(const Node& n) -> bool
    {
        return this->_node.contains(n);
    }

    /*! Return a dict of neighbors of node n.  Use: "G[n]".

    Parameters
    ----------
    n : node
       A node in the graph.

    Returns
    -------
    adj_dict : dictionary
       The adjacency dictionary for nodes connected to n.

    Notes
    -----
    G[n] is the same as G.adj[n] and similar to G.neighbors(n);
    (which is an iterator over G.adj[n]);

    Examples
    --------
    >>> G = xn::path_graph(4);  // or DiGraph, MultiGraph, MultiDiGraph, etc
    >>> G[0];
    AtlasView({1: {}});
     */
    auto operator[](const Node& n) const -> const auto&
    {
        return this->adj()[n];
    }

    auto operator[](const Node& n) -> auto&
    {
        return this->adj()[n];
    }


    /// @property
    auto nodes()
    {
        /*! A NodeView of the Graph as G.nodes().

        Returns
        -------
        NodeView
            Allows set-like operations over the nodes as well as node
            attribute dict lookup and calling to get a NodeDataView.
            A NodeDataView iterates over `(n, data)` and has no set operations.
            A NodeView iterates over `n` and includes set operations.

            When called, if (data == false, an iterator over nodes.
            Otherwise an iterator of 2-tuples (node, attribute value);
            where the attribute is specified : `data`.
            If data is true then the attribute becomes the
            entire data dictionary.

        Notes
        -----
        If your node data is not needed, it is simpler and equivalent
        to use the expression ``for n : G``, or ``list(G)``.

        Examples
        --------
        There are two simple ways of getting a list of all nodes : the graph) {

        >>> G = xn::path_graph(3);
        >>> list(G.nodes);
        [0, 1, 2];
        >>> list(G);
        [0, 1, 2];

        To get the node data along with the nodes) {

        >>> G.add_node(1, time="5pm");
        >>> G.nodes[0]["foo"] = "bar";
        >>> list(G.nodes(data=true));
        [(0, {"foo": "bar"}), (1, {"time": "5pm"}), (2, {})];
        >>> list(G.nodes.data());
        [(0, {"foo": "bar"}), (1, {"time": "5pm"}), (2, {})];

        >>> list(G.nodes(data="foo"));
        [(0, "bar"), (1, None), (2, None)];
        >>> list(G.nodes.data("foo"));
        [(0, "bar"), (1, None), (2, None)];

        >>> list(G.nodes(data="time"));
        [(0, None), (1, "5pm"), (2, None)];
        >>> list(G.nodes.data("time"));
        [(0, None), (1, "5pm"), (2, None)];

        >>> list(G.nodes(data="time", default="Not Available"));
        [(0, "Not Available"), (1, "5pm"), (2, "Not Available")];
        >>> list(G.nodes.data("time", default="Not Available"));
        [(0, "Not Available"), (1, "5pm"), (2, "Not Available")];

        If some of your nodes have an attribute and the rest are assumed
        to have a default attribute value you can create a dictionary
        from node/attribute pairs using the `default` keyword argument
        to guarantee the value is never None:) {

            >>> G = xn::Graph();
            >>> G.add_node(0);
            >>> G.add_node(1, weight=2);
            >>> G.add_node(2, weight=3);
            >>> dict(G.nodes(data="weight", default=1));
            {0: 1, 1: 2, 2: 3}

         */
        using T = decltype(*this);
        auto nodes = NodeView<T>(*this);
        // Lazy View creation: overload the (class) property on the instance
        // Then future G.nodes use the existing View
        // setattr doesn"t work because attribute already exists
        this->operator[]("nodes") = std::any(nodes);
        return nodes;
    }

    /*! Return the number of nodes : the graph.

    Returns
    -------
    nnodes : int
        The number of nodes : the graph.

    See Also
    --------
    order, size  which are identical

    Examples
    --------
    >>> G = xn::path_graph(3);  // or DiGraph, MultiGraph, MultiDiGraph, etc
    >>> len(G);
    3
     */
    auto number_of_nodes() const
    {
        return this->_node.size();
    }

    auto number_of_edges() const
    {
        return this->_num_of_edges;
    }

    /*! Return the number of nodes : the graph.

    Returns
    -------
    nnodes : int
        The number of nodes : the graph.

    See Also
    --------
    number_of_nodes, size  which are identical
     */
    auto order()
    {
        return this->_node.size();
    }

    /*! Return the number of nodes : the graph.

    Returns
    -------
    nnodes : int
        The number of nodes : the graph.

    See Also
    --------
    number_of_nodes, order  which are identical
     */
    auto size()
    {
        return this->_node.size();
    }

    /*! Return true if (the graph contains the node n.

        Identical to `n : G`

        Parameters
        ----------
        n : node

        Examples
        --------
        >>> G = xn::path_graph(3);  // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> G.has_node(0);
        true
     */
    auto has_node(const Node& n)
    {
        return this->_node.contains(n);
    }

    /*! Add an edge between u and v.

        The nodes u and v will be automatically added if (they are
        not already : the graph.

        Edge attributes can be specified with keywords or by directly
        accessing the edge"s attribute dictionary. See examples below.

        Parameters
        ----------
        u, v : nodes
            Nodes can be, for example, strings or numbers.
            Nodes must be hashable (and not None) C++ objects.

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
        >>> G.add_edges_from([(1, 2)]);  // add edges from iterable container

        Associate data to edges using keywords) {

        >>> G.add_edge(1, 2);

        For non-string attribute keys, use subscript notation.

        >>> G.add_edge(1, 2);
        >>> G[1][2].update({0: 5});
        >>> G.edges()[1, 2].update({0: 5});
     */
    template <typename U = key_type>
    auto add_edge(const Node& u, const Node& v) ->
        typename std::enable_if<std::is_same<U, value_type>::value>::type
    {
        // auto [u, v] = u_of_edge, v_of_edge;
        // add nodes
        // assert(this->_node.contains(u));
        // assert(this->_node.contains(v));
        // add the edge
        // datadict = this->_adj[u].get(v, this->edge_attr_dict_factory());
        // datadict.update(attr);
        // set
        this->_adj[u].insert(v);
        this->_adj[v].insert(u);
        this->_num_of_edges += 1;
    }

    template <typename U = key_type>
    auto add_edge(const Node& u, const Node& v) ->
        typename std::enable_if<!std::is_same<U, value_type>::value>::type
    {
        // auto [u, v] = u_of_edge, v_of_edge;
        // add nodes
        // assert(this->_node.contains(u));
        // assert(this->_node.contains(v));
        // add the edge
        // datadict = this->_adj[u].get(v, this->edge_attr_dict_factory());
        // datadict.update(attr);
        using T = typename adjlist_t::mapped_type;
        auto data = this->_adj[u].get(v, T {});
        this->_adj[u][v] = data;
        this->_adj[v][u] = data; // ???
        this->_num_of_edges += 1;
    }

    template <typename T>
    auto add_edge(const Node& u, const Node& v, const T& data)
    {
        // assert(this->_node.contains(u));
        // assert(this->_node.contains(v));
        this->_adj[u][v] = data;
        this->_adj[v][u] = data;
        this->_num_of_edges += 1;
    }

    template <typename C1>
    auto add_edges_from(const C1& edges)
    {
        for (const auto& e : edges)
        {
            this->add_edge(e.first, e.second);
            this->_num_of_edges += 1;
        }
    }

    template <typename C1, typename C2>
    auto add_edges_from(const C1& edges, const C2& data)
    {
        auto it = data.begin();
        for (const auto& e : edges)
        {
            this->add_edge(e.first, e.second, *it);
            this->_num_of_edges += 1;
            ++it;
        }
    }

    auto has_edge(const Node& u, const Node& v) -> bool
    {
        /*! Return true if (the edge (u, v) is : the graph.

        This is the same as `v : G[u]` without KeyError exceptions.

        Parameters
        ----------
        u, v : nodes
            Nodes can be, for example, strings or numbers.
            Nodes must be hashable (and not None) C++ objects.

        Returns
        -------
        edge_ind : bool
            true if (edge is : the graph, false otherwise.

        Examples
        --------
        >>> G = xn::path_graph(4);  // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> G.has_edge(0, 1);  // using two nodes
        true
        >>> e = (0, 1);
        >>> G.has_edge(*e);  //  e is a 2-tuple (u, v);
        true
        >>> e = (0, 1, {"weight":7});
        >>> G.has_edge(*e[:2]);  // e is a 3-tuple (u, v, data_dictionary);
        true

        The following syntax are equivalent) {

        >>> G.has_edge(0, 1);
        true
        >>> 1 : G[0];  // though this gives KeyError if (0 not : G
        true

         */
        return this->_adj[u].contains(v);
    }

    auto degree(const Node& n) const
    {
        return this->_adj[n].size();
    }

    /// @property
    /*! An EdgeView of the Graph as G.edges().

        edges( nbunch=None, data=false, default=None);

        The EdgeView provides set-like operations on the edge-tuples
        as well as edge attribute lookup. When called, it also provides
        an EdgeDataView object which allows control of access to edge
        attributes (but does not provide set-like operations).
        Hence, `G.edges[u, v]["color"]` provides the value of the color
        attribute for edge `(u, v)` while
        `for (auto [u, v, c] : G.edges.data("color", default="red") {`
        iterates through all the edges yielding the color attribute
        with default `"red"` if (no color attribute exists.

        Parameters
        ----------
        nbunch : single node, container, or all nodes (default= all nodes);
            The view will only report edges incident to these nodes.
        data : string or bool, optional (default=false);
            The edge attribute returned : 3-tuple (u, v, ddict[data]).
            If true, return edge attribute dict : 3-tuple (u, v, ddict).
            If false, return 2-tuple (u, v).
        default : value, optional (default=None);
            Value used for edges that don"t have the requested attribute.
            Only relevant if (data is not true or false.

        Returns
        -------
        edges : EdgeView
            A view of edge attributes, usually it iterates over (u, v);
            or (u, v, d) tuples of edges, but can also be used for
            attribute lookup as `edges[u, v]["foo"]`.

        Notes
        -----
        Nodes : nbunch that are not : the graph will be (quietly) ignored.
        For directed graphs this returns the out-edges.

        Examples
        --------
        >>> G = xn::path_graph(3)   // or MultiGraph, etc
        >>> G.add_edge(2, 3, weight=5);
        >>> [e for e : G.edges];
        [(0, 1), (1, 2), (2, 3)];
        >>> G.edges.data();  // default data is {} (empty dict);
        EdgeDataView([(0, 1, {}), (1, 2, {}), (2, 3, {"weight": 5})]);
        >>> G.edges.data("weight", default=1);
        EdgeDataView([(0, 1, 1), (1, 2, 1), (2, 3, 5)]);
        >>> G.edges([0, 3]);  // only edges incident to these nodes
        EdgeDataView([(0, 1), (3, 2)]);
        >>> G.edges(0);  // only edges incident to a single node (use
        G.adj[0]?); EdgeDataView([(0, 1)]);
    */
    // auto edges() {
    //     auto edges = EdgeView(*this);
    //     this->operator[]("edges") = std::any(edges);
    //     return edges;
    // }

    // /// @property
    // auto degree() {
    //     /*! A DegreeView for the Graph as G.degree or G.degree().

    //     The node degree is the number of edges adjacent to the node.
    //     The weighted node degree is the sum of the edge weights for
    //     edges incident to that node.

    //     This object provides an iterator for (node, degree) as well as
    //     lookup for the degree for a single node.

    //     Parameters
    //     ----------
    //     nbunch : single node, container, or all nodes (default= all nodes);
    //         The view will only report edges incident to these nodes.

    //     weight : string or None, optional (default=None);
    //        The name of an edge attribute that holds the numerical value used
    //        as a weight.  If None, then each edge has weight 1.
    //        The degree is the sum of the edge weights adjacent to the node.

    //     Returns
    //     -------
    //     If a single node is requested
    //     deg : int
    //         Degree of the node

    //     OR if (multiple nodes are requested
    //     nd_view : A DegreeView object capable of iterating (node, degree)
    //     pairs

    //     Examples
    //     --------
    //     >>> G = xn::path_graph(4);  // or DiGraph, MultiGraph, MultiDiGraph,
    //     etc
    //     >>> G.degree[0];  // node 0 has degree 1
    //     1
    //     >>> list(G.degree([0, 1, 2]));
    //     [(0, 1), (1, 2), (2, 2)];
    //      */
    //     auto degree = DegreeView(*this);
    //     this->operator[]("degree") = std::any(degree);
    //     return degree;
    // }

    auto clear()
    {
        /*! Remove all nodes and edges from the graph.

        This also removes the name, and all graph, node, and edge attributes.

        Examples
        --------
        >>> G = xn::path_graph(4);  // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> G.clear();
        >>> list(G.nodes);
        [];
        >>> list(G.edges());
        [];

         */
        this->_adj.clear();
        // this->_node.clear();
        this->graph.clear();
    }

    /*! Return true if (graph is a multigraph, false otherwise. */
    auto is_multigraph()
    {
        return false;
    }

    /*! Return true if (graph is directed, false otherwise. */
    auto is_directed()
    {
        return false;
    }
};

using SimpleGraph =
    Graph<decltype(py::range<uint32_t>(uint32_t{})), py::set<uint32_t>, std::vector<py::set<uint32_t>>>;

// template <typename nodeview_t,
//           typename adjlist_t> Graph(int )
// -> Graph<decltype(py::range(1)), py::set<int>>;

} // namespace xn
