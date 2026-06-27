/**
 * @file graph.hpp
 * @brief Undirected graph data structure for XNetwork
 *
 * Defines the Graph class template and related utilities for
 * representing undirected graphs with arbitrary node types.
 */

#pragma once

#include <cassert>
#include <cstdint>
#include <py2cpp/py2cpp.hpp>
#include <type_traits>
#include <vector>
#include <xnetwork/classes/coreviews.hpp>    // import AtlasView, AdjacencyView
#include <xnetwork/classes/reportviews.hpp>  // import NodeView, EdgeView, DegreeView

/** @brief Alias for the value_type of a container */
template <typename T> using Value_type = typename T::value_type;

namespace xnetwork {

    /** @brief Undirected graph with arbitrary node types
        @details A Graph stores nodes and edges with optional data or attributes.
        Graphs hold undirected edges. Self loops are allowed but multiple
        (parallel) edges are not. Nodes can be arbitrary (hashable) C++ objects.

        The Graph class uses a container-of-container-of-container data structure.
        The outer dict (node_dict) holds adjacency information keyed by node.
        The next dict (adjlist_dict) represents the adjacency information and holds
        edge data keyed by neighbor. The inner dict (edge_attr_dict) represents
        the edge data and holds edge attribute values keyed by attribute names.

        @tparam _nodeview_t The node container type
        @tparam adjlist_t The adjacency list type (default: py::set of node values)
        @tparam adjlist_outer_dict_factory The outer dict factory (default: py::dict) */

    // struct object : py::dict<const char*, std::any> {};

    template <typename _nodeview_t, typename adjlist_t = py::set<Value_type<_nodeview_t>>,
              typename adjlist_outer_dict_factory = py::dict<Value_type<_nodeview_t>, adjlist_t>>
    class Graph {
      public:
        using nodeview_t = _nodeview_t;
        using Node = typename nodeview_t::value_type;  // luk
        // using dict = py::dict<const char*, std::any>;
        // using graph_attr_dict_factory = dict;
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

        size_t _num_of_edges = 0;  ///< Number of edges in the graph (cached)

        // std::vector<Node > _Nodes{}
        nodeview_t _node;  ///< Container holding all nodes in the graph
        // graph_attr_dict_factory graph{};  // dictionary for graph attributes
        // node_dict_factory _node{};  // empty node attribute dict
        adjlist_outer_dict_factory _adj;  ///< Adjacency dictionary keyed by node

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

        /** @brief Construct a graph from a node container
            @param[in] Nodes Container of nodes to initialize the graph */
        explicit Graph(const nodeview_t& Nodes)
            : _node{Nodes},
              _adj{}  // py::dict???
        {}

        /** @brief Construct a graph with a given number of integer nodes
            @param[in] num_nodes Number of nodes (0 to num_nodes-1) */
        explicit Graph(uint32_t num_nodes)
            : _node{py::range<uint32_t>(num_nodes)},
              _adj(num_nodes)  // std::vector
        {}

        // Graph(const Graph&) = delete;            // don't copy
        // Graph& operator=(const Graph&) = delete; // don't copy
        // Graph(Graph&&) noexcept = default;

        /**
         * @brief For compatible with BGL adaptor
         *
         * @param[in] e
         * @return edge_t&
         */
        static auto end_points(edge_t& e) -> edge_t& { return e; }

        /**
         * @brief For compatible with BGL adaptor
         *
         * @param[in] e
         * @return edge_t&
         */
        static auto end_points(const edge_t& e) -> const edge_t& { return e; }

        /** @brief Get the adjacency mapping of the graph (const version)
            @return AdjacencyView of the internal adjacency structure */
        auto adj() const {
            using T = std::remove_reference_t<decltype(this->_adj)>;
            return AdjacencyView<const T&>(this->_adj);
        }

        /** @brief Get the adjacency mapping of the graph (non-const version)
            @return AdjacencyView of the internal adjacency structure */
        auto adj() {
            using T = std::remove_cv_t<decltype(this->_adj)>;
            return AdjacencyView<T>(this->_adj);
        }

        /** @brief Iterate over nodes and their neighbors
            @return An iterable of (node, adjacency dict) pairs */
        auto _nodes_nbrs() const {
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

        /** String identifier of the graph.

        This graph attribute appears : the attribute dict gra.graph
        keyed by the string `"name"`. as well as an attribute (technically
        a property) `gra.name`. This is entirely user controlled.
         */
        // auto get_name() {
        //     if (!this->graph.contains("name")) {
        //         return "";
        //     }
        //     return std::any_cast<const char*>(this->graph["name"]);
        // }

        // // @name.setter
        // auto set_name(std::string_view s) { this->graph["name"] = std::any(s); }

        /** @brief Begin iterator over nodes
            @return Iterator to the first node */
        auto begin() const { return std::begin(this->_node); }

        /** @brief End iterator over nodes
            @return Iterator past the last node */
        auto end() const { return std::end(this->_node); }

        /** @brief Check if a node is in the graph
            @param[in] node The node to check
            @return true if the node exists, false otherwise */
        auto contains(const Node& node) const -> bool { return this->_node.contains(node); }

        /** @brief Access adjacency dict of a node (const version)
            @param[in] node The node to look up
            @return Const reference to the adjacency dict of the node */
        auto operator[](const Node& node) const -> const auto& { return this->adj().at(node); }

        /** @brief Access adjacency dict of a node with bounds check (const version)
            @param[in] node The node to look up
            @return Const reference to the adjacency dict of the node */
        auto at(const Node& node) const -> const auto& { return this->adj().at(node); }

        /** @brief Access adjacency dict of a node (non-const version)
            @param[in] node The node to look up
            @return Reference to the adjacency dict of the node */
        auto operator[](const Node& node) -> auto& { return this->adj()[node]; }

        /** @brief Get a NodeView of the graph
            @return NodeView providing set-like operations and data lookup */
        auto nodes() {
            using T = std::remove_reference_t<decltype(*this)>;
            auto nodes = NodeView<T>(*this);
            // Lazy View creation: overload the (class) property on the instance
            // Then future gra.nodes use the existing View
            // setattr doesn"t work because attribute already exists
            // this->operator[]("nodes") = std::any(nodes);
            return nodes;
        }

        /** @brief Get the number of nodes in the graph
            @return Number of nodes */
        auto number_of_nodes() const -> size_t { return this->_node.size(); }

        /** @brief Get the number of nodes (same as number_of_nodes)
            @return Number of nodes */
        auto order() const { return this->_node.size(); }

        /** @brief Get the number of nodes (same as number_of_nodes)
            @return Number of nodes */
        auto size() const -> size_t { return this->_node.size(); }

        /** @brief Get the number of edges in the graph
            @return Number of edges (cached value, O(1)) */
        auto number_of_edges() const -> size_t { return this->_num_of_edges; }

        /** @brief Return a vector of all edges as (u, v) pairs
            @details Each undirected edge is reported once with u < v.
            @return Vector of (u, v) pairs representing all edges */
        auto edges() const -> std::vector<edge_t> {
            std::vector<edge_t> result;
            for (const auto& node : this->_node) {
                for (const auto& nbr : this->_adj[node]) {
                    if (node < nbr) {
                        result.emplace_back(node, nbr);
                    }
                }
            }
            return result;
        }

        /** @brief Check if the graph contains a node
            @param[in] node The node to check
            @return true if the node exists, false otherwise */
        auto has_node(const Node& node) const -> bool { return this->_node.contains(node); }

        /** @brief Add an edge between two nodes (for simple key type, SFINAE)
            @tparam U Key type parameter for SFINAE dispatch
            @param[in] node_u First endpoint of the edge
            @param[in] node_v Second endpoint of the edge */
        template <typename U = key_type> auto add_edge(const Node& node_u, const Node& node_v) ->
            typename std::enable_if<std::is_same<U, value_type>::value>::type {
            this->_adj[node_u].insert(node_v);
            this->_adj[node_v].insert(node_u);
            ++this->_num_of_edges;
        }

        /** @brief Add an edge between two nodes (for complex key type, SFINAE)
            @tparam U Key type parameter for SFINAE dispatch
            @param[in] node_u First endpoint of the edge
            @param[in] node_v Second endpoint of the edge */
        template <typename U = key_type> auto add_edge(const Node& node_u, const Node& node_v) ->
            typename std::enable_if<!std::is_same<U, value_type>::value>::type {
            using T = typename adjlist_t::mapped_type;
            auto data = this->_adj[node_u].get(node_v, T{});
            this->_adj[node_u][node_v] = data;
            this->_adj[node_v][node_u] = data;
            ++this->_num_of_edges;
        }

        /** @brief Add an edge with attached data
            @tparam T Type of edge data
            @param[in] node_u First endpoint
            @param[in] node_v Second endpoint
            @param[in] data Data to associate with the edge */
        template <typename T> auto add_edge(const Node& node_u, const Node& node_v, const T& data) {
            this->_adj[node_u][node_v] = data;
            this->_adj[node_v][node_u] = data;
            ++this->_num_of_edges;
        }

        /** @brief Add edges from a container of edge pairs
            @tparam C1 Container type for edges
            @param[in] edges Container of edge pairs */
        template <typename C1> auto add_edges_from(const C1& edges) {
            for (const auto& e : edges) {
                this->add_edge(e.first, e.second);
            }
        }

        /** @brief Add edges from a container with associated data
            @tparam C1 Container type for edges
            @tparam C2 Container type for edge data
            @param[in] edges Container of edge pairs
            @param[in] data Container of edge data values */
        template <typename C1, typename C2> auto add_edges_from(const C1& edges, const C2& data) {
            auto it = data.begin();
            for (const auto& e : edges) {
                this->add_edge(e.first, e.second, *it);
                ++it;
            }
        }

        /** @brief Check if an edge exists between two nodes
            @param[in] node_u First endpoint
            @param[in] node_v Second endpoint
            @return true if the edge exists, false otherwise */
        auto has_edge(const Node& node_u, const Node& node_v) const -> bool {
            return this->_adj.at(node_u).contains(node_v);
        }

        /** @brief Get the degree of a node
            @param[in] node The node to get the degree for
            @return The number of edges incident to the node */
        auto degree(const Node& node) const { return this->_adj[node].size(); }

        /** @brief Remove all nodes and edges from the graph */
        auto clear() {
            this->_adj.clear();
            // this->_node.clear();
            // this->graph.clear();
        }

        /** @brief Apply a callable to each edge without materializing a vector
            @details More efficient than edges() when you only need to iterate once,
            as it avoids allocating the result vector. Each edge (u, v) is yielded once with u < v.
            @tparam F Callable `void(const Node&, const Node&)` or similar
            @param[in] func Callable invoked for each edge */
        template <typename F> auto for_each_edge(F&& func) const -> void {
            for (const auto& node : this->_node) {
                for (const auto& nbr : this->_adj[node]) {
                    if (node < nbr) {
                        std::forward<F>(func)(node, nbr);
                    }
                }
            }
        }

        /** @brief Check if the graph is a multigraph
            @return false (this is a simple graph) */
        auto is_multigraph() const { return false; }

        /** @brief Check if the graph is directed
            @return false (this is an undirected graph) */
        auto is_directed() const { return false; }
    };

    /** @brief A simple undirected graph with integer nodes
        @details Convenience alias for a Graph with uint32_t nodes and vector-based adjacency
       storage */
    using SimpleGraph = Graph<decltype(py::range<uint32_t>(uint32_t{})), py::set<uint32_t>,
                              std::vector<py::set<uint32_t>>>;

    // template <typename nodeview_t,
    //           typename adjlist_t> Graph(int )
    // -> Graph<decltype(py::range(1)), py::set<int>>;

}  // namespace xnetwork
