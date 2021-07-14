#pragma once

#include <any>
#include <boost/coroutine2/all.hpp>
// #include <cppcoro/generator.hpp>
#include <cassert>
#include <py2cpp/py2cpp.hpp>
// #include <range/v3/view/enumerate.hpp>
#include <type_traits>
#include <utility>
#include <vector>
#include <xnetwork/classes/coreviews.hpp> // import AtlasView, AdjacencyView
#include <xnetwork/classes/graph.hpp>
#include <xnetwork/classes/reportviews.hpp> // import NodeView, EdgeView, DegreeView

namespace xn
{

/*! Base class for directed graphs.

    A DiGraphS stores nodes and edges with optional data, or attributes.

    DiGraphSs hold directed edges.  Self loops are allowed but multiple
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
    Graph
    DiGraph
    MultiGraph
    MultiDiGraph
    OrderedGraph

    Examples
    --------
    Create an empty graph structure (a "null graph") with 5 nodes and
    no edges.

    >>> auto v = std::vector{3, 4, 2, 8};
    >>> auto G = xn::DiGraphS(v);

    >>> auto va = py::dict{{3, 0.1}, {4, 0.5}, {2, 0.2}};
    >>> auto G = xn::DiGraphS(va);

    >>> auto r = py::range(100);
    >>> auto G = xn::DiGraphS(r);

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
    or even another DiGraphS.

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

    The DiGraphS class uses a container-of-container-of-container data
   structure. The outer dict (node_dict) holds adjacency information keyed by
   node. The next dict (adjlist_dict) represents the adjacency information and
   holds edge data keyed by neighbor.  The inner dict (edge_attr_dict)
   represents the edge data and holds edge attribute values keyed by attribute
   names.

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
    By default these methods create a DiGraph/DiGraphS class and you probably
    want them to create your extension of a DiGraph/DiGraphS. To facilitate
    this we define two class variables that you can set in your subclass.

    to_directed_class : callable, (default: DiGraph or MultiDiGraph)
        Class to create a new graph structure in the `to_directed` method.
        If `None`, a NetworkX class (DiGraph or MultiDiGraph) is used.

    to_undirected_class : callable, (default: DiGraphS or MultiGraph)
        Class to create a new graph structure in the `to_undirected` method.
        If `None`, a NetworkX class (DiGraphS or MultiGraph) is used.

    Examples
    --------

    Create a low memory graph class that effectively disallows edge
    attributes by using a single attribute dict for all edges.
    This reduces the memory used, but you lose edge attributes.

    >>> class ThinGraph(xn::DiGraphS):
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
template <typename nodeview_t,
    typename adjlist_t = py::dict<Value_type<nodeview_t>, int>,
    typename adjlist_outer_dict_factory =
        py::dict<Value_type<nodeview_t>, adjlist_t>>
class DiGraphS : public Graph<nodeview_t, adjlist_t, adjlist_outer_dict_factory>
{
    using _Base = Graph<nodeview_t, adjlist_t, adjlist_outer_dict_factory>;

  public:
    using Node = typename _Base::Node; // luk
    using edge_t = std::pair<Node, Node>;
    using graph_attr_dict_factory = typename _Base::graph_attr_dict_factory;
    // using adjlist_outer_dict_factory =
    //     typename _Base::adjlist_outer_dict_factory;
    using key_type = typename _Base::key_type;
    using value_type = typename _Base::value_type;

  public:
    adjlist_outer_dict_factory& _succ; // successor

    /*! Initialize a graph with edges, name, or graph attributes.

        Parameters
        ----------
        node_container : input nodes

        Examples
        --------
        >>> v = std::vector{5, 3, 2};
        >>> G = xn::DiGraphS(v);  // or DiGraph, MultiGraph, MultiDiGraph, etc

        >>> r = py::range(100);
        >>> G = xn::DiGraphS(r, r);  // or DiGraph, MultiGraph, MultiDiGraph,
       etc
    */
    explicit DiGraphS(const nodeview_t& Nodes)
        : _Base {Nodes}
        , _succ {_Base::_adj}
    {
    }

    explicit DiGraphS(int num_nodes)
        : _Base {num_nodes}
        , _succ {_Base::_adj}
    {
    }

    /// @property
    /*! DiGraphS adjacency object holding the neighbors of each node.

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
        using T = decltype(this->_succ);
        return AdjacencyView<T>(this->_succ);
    }

    /// @property
    /*! Graph adjacency object holding the successors of each node.

        This object is a read-only dict-like structure with node keys
        and neighbor-dict values.  The neighbor-dict is keyed by neighbor
        to the edge-data-dict.  So `G.succ[3][2]['color'] = 'blue'` sets
        the color of the edge `(3, 2)` to `"blue"`.

        Iterating over G.succ behaves like a dict. Useful idioms include
        `for nbr, datadict in G.succ[n].items():`.  A data-view not provided
        by dicts also exists: `for nbr, foovalue in G.succ[node].data('foo'):`
        and a default can be set via a `default` argument to the `data` method.

        The neighbor information is also provided by subscripting the graph.
        So `for nbr, foovalue in G[node].data('foo', default=1):` works.

        For directed graphs, `G.adj` is identical to `G.succ`.
    */
    auto succ() const
    {
        using T = decltype(this->_succ);
        return AdjacencyView<T>(this->_succ);
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

        >>> G = xn::DiGraphS()   // or DiGraph, MultiGraph, MultiDiGraph, etc
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
    auto add_edge(
        const Node& u, const Node& v) -> typename std::enable_if<std::is_same<U, value_type>::value>::type
    {
        // auto [u, v] = u_of_edge, v_of_edge;
        // add nodes
        // assert(this->s->_node.contains(u));
        // assert(this->s->_node.contains(v));
        // add the edge
        // datadict = this->_adj[u].get(v, this->edge_attr_dict_factory());
        // datadict.update(attr);
        this->_succ[u].insert(v);
        // this->_prev[v].insert(u);
        this->_num_of_edges += 1;
    }

    template <typename U = key_type>
    auto add_edge(
        const Node& u, const Node& v) -> typename std::enable_if<!std::is_same<U, value_type>::value>::type
    {
        // auto [u, v] = u_of_edge, v_of_edge;
        // add nodes
        // assert(this->s->_node.contains(u));
        // assert(this->s->_node.contains(v));
        // add the edge
        // datadict = this->_adj[u].get(v, this->edge_attr_dict_factory());
        // datadict.update(attr);
        using T = typename adjlist_t::mapped_type;
        auto data = this->_adj[u].get(v, T {});
        this->_succ[u][v] = data;
        // this->_prev[v][u] = data;
        this->_num_of_edges += 1;
    }

    template <typename T>
    auto add_edge(const Node& u, const Node& v, const T& data)
    {
        // assert(this->s->_node.contains(u));
        // assert(this->s->_node.contains(v));
        this->_succ[u][v] = data;
        this->_num_of_edges += 1;
    }

    template <typename C1, typename C2>
    auto add_edges_from(const C1& edges, const C2& data)
    {
        auto N = edges.size();
        for (auto i = 0U; i != N; ++i)
        {
            const auto& e = edges[i];
            this->add_edge(e.first, e.second, data[i]);
        }
    }

    /*! Returns True if node u has successor v.

        This is true if graph has the edge u->v.
    */
    auto has_successor(const Node& u, const Node& v) -> bool
    {
        return this->_node.contains(u) && this->_succ[u].contains(v);
    }

    /*! Returns an iterator over successor nodes of n.

        A successor of n is a node m such that there exists a directed
        edge from n to m.

        Parameters
        ----------
        n : node
           A node in the graph

        Raises
        -------
        NetworkXError
           If n is not in the graph.

        See Also
        --------
        predecessors

        Notes
        -----
        neighbors() and successors() are the same.
    */
    auto successors(const Node& n) -> auto&
    {
        return this->_succ[n];
    }

    auto successors(const Node& n) const -> const auto&
    {
        return this->_succ[n];
    }

    /// @property
    /*! An OutEdgeView of the DiGraph as G.edges().

        edges(self, nbunch=None, data=False, default=None)

        The OutEdgeView provides set-like operations on the edge-tuples
        as well as edge attribute lookup. When called, it also provides
        an EdgeDataView object which allows control of access to edge
        attributes (but does not provide set-like operations).
        Hence, `G.edges()[u, v]['color']` provides the value of the color
        attribute for edge `(u, v)` while
        `for (u, v, c) in G.edges().data('color', default='red'):`
        iterates through all the edges yielding the color attribute
        with default `'red'` if no color attribute exists.

        Parameters
        ----------
        nbunch : single node, container, or all nodes (default= all nodes)
            The view will only report edges incident to these nodes.
        data : string or bool, optional (default=False)
            The edge attribute returned in 3-tuple (u, v, ddict[data]).
            If True, return edge attribute dict in 3-tuple (u, v, ddict).
            If False, return 2-tuple (u, v).
        default : value, optional (default=None)
            Value used for edges that don't have the requested attribute.
            Only relevant if data is not True or False.

        Returns
        -------
        edges : OutEdgeView
            A view of edge attributes, usually it iterates over (u, v)
            or (u, v, d) tuples of edges, but can also be used for
            attribute lookup as `edges[u, v]['foo']`.

        See Also
        --------
        in_edges, out_edges

        Notes
        -----
        Nodes in nbunch that are not in the graph will be (quietly) ignored.
        For directed graphs this returns the out-edges.

        Examples
        --------
        >>> G = nx.DiGraph()   # or MultiDiGraph, etc
        >>> nx.add_path(G, [0, 1, 2])
        >>> G.add_edge(2, 3, weight=5)
        >>> [e for e in G.edges()]
        [(0, 1), (1, 2), (2, 3)]
        >>> G.edges().data()  # default data is {} (empty dict)
        OutEdgeDataView([(0, 1, {}), (1, 2, {}), (2, 3, {'weight': 5})])
        >>> G.edges().data('weight', default=1)
        OutEdgeDataView([(0, 1, 1), (1, 2, 1), (2, 3, 5)])
        >>> G.edges()([0, 2])  # only edges incident to these nodes
        OutEdgeDataView([(0, 1), (2, 3)])
        >>> G.edges()(0)  # only edges incident to a single node (use G.adj[0]?)
        OutEdgeDataView([(0, 1)])

    */
    using coro_t = boost::coroutines2::coroutine<edge_t>;
    using pull_t = typename coro_t::pull_type;

    /// @TODO: sync with networkx
    auto edges() const -> pull_t
    {
        auto func = [&](typename coro_t::push_type& yield)
        {
            if constexpr (std::is_same_v<nodeview_t,
                              decltype(py::range<int>(0))>)
            { // this->_succ???
                for (auto&& [n, nbrs] : py::enumerate(this->_adj))
                {
                    for (auto&& nbr : nbrs)
                    {
                        yield(edge_t {Node(n), Node(nbr)});
                    }
                }
            }
            else
            {
                for (auto&& [n, nbrs] : this->_adj.items())
                {
                    for (auto&& nbr : nbrs)
                    {
                        yield(edge_t {n, nbr});
                    }
                }
            }
        };


        return pull_t(func);
    }

    // cppcoro::generator<edge_t> edges() const
    // {
    //     for (auto&& [n, nbrs] : this->_nodes_nbrs())
    //     {
    //         for (auto&& nbr : nbrs)
    //         {
    //             co_yield edge_t{Node(n), Node(nbr)};
    //         }
    //     }
    // }

    // auto edges() {
    //     return OutEdgeView(*this);
    // }

    // auto in_edges() {
    //     return InEdgeView(*this);
    // }

    auto degree(const Node& n) const
    {
        return this->_succ[n].size();
    }

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
    auto clear()
    {
        this->_succ.clear();
        // this->_pred.clear()
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
        return true;
    }
};


using SimpleDiGraphS = DiGraphS<decltype(py::range<int>(1)), py::dict<int, int>,
    std::vector<py::dict<int, int>>>;

// template <typename nodeview_t,
//           typename adjlist_t> DiGraphS(int )
// -> DiGraphS<decltype(py::range<int>(1)), py::set<int>>;

} // namespace xn
