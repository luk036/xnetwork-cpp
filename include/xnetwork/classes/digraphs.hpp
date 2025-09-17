#pragma once

// #include <any>
// #include <boost/coroutine2/all.hpp>
#include <cassert>
#include <py2cpp/py2cpp.hpp>
// #include <range/v3/view/enumerate.hpp>
#include <type_traits>
#include <utility>
#include <vector>
#include <xnetwork/classes/coreviews.hpp>  // import AtlasView, AdjacencyView
#include <xnetwork/classes/graph.hpp>
#include <xnetwork/classes/reportviews.hpp>  // import NodeView, EdgeView, DegreeView

// #if __cplusplus > 201703L
// #include <cppcoro/generator.hpp>
// #endif

namespace xnetwork {

    /** Base class for directed graphs.

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

            > auto v = std::vector{3, 4, 2, 8};
            > auto gra = xnetwork::DiGraphS(v);

            > auto va = py::dict{{3, 0.1}, {4, 0.5}, {2, 0.2}};
            > auto gra = xnetwork::DiGraphS(va);

            > auto r = py::range(100);
            > auto gra = xnetwork::DiGraphS(r);

        gra can be grown in several ways.

        **Nodes:**

        Add one node at a time:

            > gra.add_node(1)

        Add the nodes from any container (a list, dict, set or
        even the lines from a file or the nodes from another graph).

            > gra.add_nodes_from([2, 3])
            > gra.add_nodes_from(range(100, 110))
            > H = xnetwork::path_graph(10)
            > gra.add_nodes_from(H)

        In addition to strings and integers any hashable C++ object
        (except None) can represent a node, e.g. a customized node object,
        or even another DiGraphS.

            > gra.add_node(H)

        **Edges:**

        gra can also be grown by adding edges.

        Add one edge,

            > gra.add_edge(1, 2);

        a list of edges,

            > gra.add_edges_from([(1, 2), (1, 3)]);

        or a collection of edges,

            > gra.add_edges_from(H.edges());

        If some edges connect nodes not yet in the graph, the nodes
        are added automatically.  There are no errors when adding
        nodes or edges that already exist.

        **Attributes:**

        Each graph can hold key/value attribute pairs
        in an associated attribute dictionary (the keys must be hashable).
        By default these are empty, but can be added or changed using
        direct manipulation of the attribute
        dictionaries named graph, node and edge respectively.

            > gra.graph["day"] = std::any("Friday");
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

            > class ThinGraph(xnetwork::DiGraphS):
        ...     all_edge_dict = {'weight': 1}
        ...     def single_edge_dict(self):
        ...         return self.all_edge_dict
        ...     edge_attr_dict_factory = single_edge_dict
            > gra = ThinGraph()
            > gra.add_edge(2, 1)
            > gra[2][1]
        {'weight': 1}
            > gra.add_edge(2, 2)
            > gra[2][1] is gra[2][2]
        True

        Please see :mod:`~networkx.classes.ordered` for more examples of
        creating graph subclasses by overwriting the base class `dict` with
        a dictionary-like object.
    */
    template <typename nodeview_t, typename adjlist_t = py::dict<Value_type<nodeview_t>, int>,
              typename adjlist_outer_dict_factory = py::dict<Value_type<nodeview_t>, adjlist_t>>
    class DiGraphS : public Graph<nodeview_t, adjlist_t, adjlist_outer_dict_factory> {
        using _Base = Graph<nodeview_t, adjlist_t, adjlist_outer_dict_factory>;

      public:
        using Node = typename _Base::Node;  // luk
        using edge_t = std::pair<Node, Node>;
        // using graph_attr_dict_factory = typename _Base::graph_attr_dict_factory;
        // using adjlist_outer_dict_factory =
        //     typename _Base::adjlist_outer_dict_factory;
        using key_type = typename _Base::key_type;
        using value_type = typename _Base::value_type;

      public:
        // adjlist_outer_dict_factory &_adj; // successor

        /** Initialize a graph with edges, name, or graph attributes.

            Parameters
            ----------
            node_container : input nodes

            Examples
            --------
                > v = std::vector{5, 3, 2};
                > gra = xnetwork::DiGraphS(v);  // or DiGraph, MultiGraph,
           MultiDiGraph, etc

                > r = py::range(100);
                > gra = xnetwork::DiGraphS(r, r);  // or DiGraph, MultiGraph,
           MultiDiGraph, etc
        */
        explicit DiGraphS(const nodeview_t &Nodes) : _Base{Nodes} {}

        explicit DiGraphS(uint32_t num_nodes) : _Base{num_nodes} {}

        /** DiGraphS adjacency object holding the neighbors of each node.

            This object is a read-only dict-like structure with node keys
            and neighbor-dict values.  The neighbor-dict is keyed by neighbor
            to the edge-data-dict.  So `gra.adj[3][2]['color'] = 'blue'` sets
            the color of the edge `(3, 2)` to `"blue"`.

            Iterating over gra.adj behaves like a dict. Useful idioms include
            `for nbr, datadict in gra.adj[n].items():`.

            The neighbor information is also provided by subscripting the graph.
            So `for nbr, foovalue in gra[node].data('foo', default=1):` works.

            For directed graphs, `gra.adj` holds outgoing (successor) info.
        */
        auto adj() const {
            using T = decltype(this->_adj);
            return AdjacencyView<T>(this->_adj);
        }

        /** Graph adjacency object holding the successors of each node.

            This object is a read-only dict-like structure with node keys
            and neighbor-dict values.  The neighbor-dict is keyed by neighbor
            to the edge-data-dict.  So `gra.succ[3][2]['color'] = 'blue'` sets
            the color of the edge `(3, 2)` to `"blue"`.

            Iterating over gra.succ behaves like a dict. Useful idioms include
            `for nbr, datadict in gra.succ[n].items():`.  A data-view not provided
            by dicts also exists: `for nbr, foovalue in gra.succ[node].data('foo'):`
            and a default can be set via a `default` argument to the `data` method.

            The neighbor information is also provided by subscripting the graph.
            So `for nbr, foovalue in gra[node].data('foo', default=1):` works.

            For directed graphs, `gra.adj` is identical to `gra.succ`.
        */
        auto succ() const {
            using T = decltype(this->_adj);
            return AdjacencyView<T>(this->_adj);
        }

        /** Add an edge between u and v.

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
            The following all add the edge e=(1, 2) to graph gra) {

                > gra = xnetwork::DiGraphS()   // or DiGraph, MultiGraph,
           MultiDiGraph, etc > e = (1, 2); > gra.add_edge(1, 2)           //
           explicit two-node form > gra.add_edges_from([(1, 2)]);  // add edges from
           iterable container

            Associate data to edges using keywords) {

                > gra.add_edge(1, 2);

            For non-string attribute keys, use subscript notation.

                > gra.add_edge(1, 2);
                > gra[1][2].update({0: 5});
                > gra.edges()[1, 2].update({0: 5});
         */
        template <typename U = key_type> auto add_edge(const Node &u, const Node &v) ->
            typename std::enable_if<std::is_same<U, value_type>::value>::type {
            this->_adj[u].insert(v);
        }

        template <typename U = key_type> auto add_edge(const Node &u, const Node &v) ->
            typename std::enable_if<!std::is_same<U, value_type>::value>::type {
            using T = typename adjlist_t::mapped_type;
            auto data = this->_adj[u].get(v, T{});
            this->_adj[u][v] = data;
        }

        template <typename T> auto add_edge(const Node &u, const Node &v, const T &data) {
            this->_adj[u][v] = data;
        }

        template <typename C1, typename C2> auto add_edges_from(const C1 &edges, const C2 &data) {
            auto N = edges.size();
            for (auto i = 0U; i != N; ++i) {
                const auto &e = edges[i];
                this->add_edge(e.first, e.second, data[i]);
            }
        }

        /** Returns True if node u has successor v.

            This is true if graph has the edge u->v.
        */
        auto has_successor(const Node &u, const Node &v) -> bool {
            return this->_node.contains(u) && this->_adj[u].contains(v);
        }

        /** Returns an iterator over successor nodes of n.

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
        auto successors(const Node &n) -> auto & { return this->_adj[n]; }

        auto successors(const Node &n) const -> const auto & { return this->_adj[n]; }

        /** An OutEdgeView of the DiGraph as gra.edges().

            edges(self, nbunch=None, data=False, default=None)

            The OutEdgeView provides set-like operations on the edge-tuples
            as well as edge attribute lookup. When called, it also provides
            an EdgeDataView object which allows control of access to edge
            attributes (but does not provide set-like operations).
            Hence, `gra.edges()[u, v]['color']` provides the value of the color
            attribute for edge `(u, v)` while
            `for (u, v, c) in gra.edges().data('color', default='red'):`
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
                > gra = nx.DiGraph()   # or MultiDiGraph, etc
                > nx.add_path(gra, [0, 1, 2])
                > gra.add_edge(2, 3, weight=5)
                > [e for e in gra.edges()]
            [(0, 1), (1, 2), (2, 3)]
                > gra.edges().data()  # default data is {} (empty dict)
            OutEdgeDataView([(0, 1, {}), (1, 2, {}), (2, 3, {'weight': 5})])
                > gra.edges().data('weight', default=1)
            OutEdgeDataView([(0, 1, 1), (1, 2, 1), (2, 3, 5)])
                > gra.edges()([0, 2])  # only edges incident to these nodes
            OutEdgeDataView([(0, 1), (2, 3)])
                > gra.edges()(0)  # only edges incident to a single node (use
           gra.adj[0]?) OutEdgeDataView([(0, 1)])

        */

        auto degree(const Node &n) const { return this->_adj[n].size(); }

        /** Remove all nodes and edges from the graph.

            This also removes the name, and all graph, node, and edge attributes.

            Examples
            --------
                > gra = xnetwork::path_graph(4);  // or DiGraph, MultiGraph,
           MultiDiGraph, etc > gra.clear(); > list(gra.nodes);
            [];
                > list(gra.edges());
            [];

        */
        auto clear() {
            this->_adj.clear();
            // this->_pred.clear()
            // this->_node.clear();
            this->graph.clear();
        }

        /** Return true if (graph is a multigraph, false otherwise. */
        auto is_multigraph() { return false; }

        /** Return true if (graph is directed, false otherwise. */
        auto is_directed() { return true; }
    };

    using SimpleDiGraphS = DiGraphS<decltype(py::range<uint32_t>(uint32_t{})),
                                    py::dict<uint32_t, int>, std::vector<py::dict<uint32_t, int>>>;

    // template <typename nodeview_t,
    //           typename adjlist_t> DiGraphS(int )
    // -> DiGraphS<decltype(py::range<int>(1)), py::set<int>>;

}  // namespace xnetwork
