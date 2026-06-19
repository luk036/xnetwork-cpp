/**
 * @file digraphs.hpp
 * @brief Directed graph data structure for XNetwork
 *
 * Defines the DiGraphS class template - a directed graph implementation
 * based on the undirected Graph base class.
 */

#pragma once

#include <cassert>
#include <py2cpp/py2cpp.hpp>
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

    /** @brief Directed graph with arbitrary node types
        @details A DiGraphS stores nodes and edges with optional data or attributes.
        Directed edges are stored. Self loops are allowed but multiple (parallel) edges are not.
        Inherits from the undirected Graph base class.
        @tparam nodeview_t The node container type
        @tparam adjlist_t The adjacency list type
        @tparam adjlist_outer_dict_factory The outer dict factory */
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

        /** @brief Construct a directed graph from a node container
            @param[in] Nodes Container of nodes */
        explicit DiGraphS(const nodeview_t& Nodes) : _Base{Nodes} {}

        /** @brief Construct a directed graph with a given number of integer nodes
            @param[in] num_nodes Number of nodes (0 to num_nodes-1) */
        explicit DiGraphS(uint32_t num_nodes) : _Base{num_nodes} {}

        /** @brief Get adjacency mapping (successors) of the directed graph (const version) */
        auto adj() const {
            using T = decltype(this->_adj);
            return AdjacencyView<T>(this->_adj);
        }

        /** @brief Get the successors mapping (same as adj)
            @return AdjacencyView of the successor structure */
        auto succ() const {
            using T = decltype(this->_adj);
            return AdjacencyView<T>(this->_adj);
        }

        /** Add an edge between node_u and node_v.

            The nodes node_u and node_v will be automatically added if (they are
            not already : the graph.

            Edge attributes can be specified with keywords or by directly
            accessing the edge"s attribute dictionary. See examples below.

            Parameters
            ----------
            node_u, node_v : nodes
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
        /** @brief Add a directed edge between two nodes (for simple key type, SFINAE)
            @tparam U Key type parameter for SFINAE dispatch
            @param[in] node_u Source node
            @param[in] node_v Target node */
        template <typename U = key_type> auto add_edge(const Node& node_u, const Node& node_v) ->
            typename std::enable_if<std::is_same<U, value_type>::value>::type {
            this->_adj[node_u].insert(node_v);
        }

        /**
         * @brief Add an edge between two nodes (for complex key type)
         *
         * @tparam U Key type parameter for SFINAE
         * @param node_u Source node
         * @param node_v Target node
         */
        template <typename U = key_type> auto add_edge(const Node& node_u, const Node& node_v) ->
            typename std::enable_if<!std::is_same<U, value_type>::value>::type {
            using T = typename adjlist_t::mapped_type;
            auto data = this->_adj[node_u].get(node_v, T{});
            this->_adj[node_u][node_v] = data;
        }

        /**
         * @brief Add an edge between two nodes with data
         *
         * @tparam T Type of edge data
         * @param node_u Source node
         * @param node_v Target node
         * @param data Edge data to associate with the edge
         */
        template <typename T> auto add_edge(const Node& node_u, const Node& node_v, const T& data) {
            this->_adj[node_u][node_v] = data;
        }

        /**
         * @brief Add edges from a container with associated data
         *
         * @tparam C1 Container type for edges
         * @tparam C2 Container type for edge data
         * @param edges Container of edge pairs
         * @param data Container of edge data corresponding to each edge
         */
        template <typename C1, typename C2> auto add_edges_from(const C1& edges, const C2& data) {
            auto N = edges.size();
            for (auto idx = 0U; idx != N; ++idx) {
                const auto& e = edges[idx];
                this->add_edge(e.first, e.second, data[idx]);
            }
        }

        /** @brief Check if a node has a specific successor
            @param[in] node_u Source node
            @param[in] node_v Potential successor node
            @return true if the edge u->v exists */
        auto has_successor(const Node& node_u, const Node& node_v) const -> bool {
            return this->_node.contains(node_u) && this->_adj.at(node_u).contains(node_v);
        }

        /** @brief Get iterator over successors of a node (non-const)
            @param[in] node The node to get successors for
            @return Reference to the adjacency list of the node */
        auto successors(const Node& node) -> auto& { return this->_adj[node]; }

        /** @brief Get iterator over successors of a node (const)
            @param[in] node The node to get successors for
            @return Const reference to the adjacency list of the node */
        auto successors(const Node& node) const -> const auto& { return this->_adj[node]; }

        /** @brief Get the out-degree of a node in the directed graph
            @param[in] node The node to get the degree for
            @return The number of outgoing edges from the node */
        auto degree(const Node& node) const { return this->_adj[node].size(); }

        /** @brief Get the number of edges in the directed graph
            @details Iterates through the adjacency structure to count edges.
            @return Number of edges */
        auto number_of_edges() const -> size_t {
            size_t n_edges = 0;
            for (const auto& node : this->_node) {
                n_edges += this->_adj.at(node).size();
            }
            return n_edges;  // No division needed since it's a directed graph
        }

        /** @brief Remove all nodes and edges from the graph */
        auto clear() {
            this->_adj.clear();
            // this->_pred.clear()
            // this->_node.clear();
            this->graph.clear();
        }

        /** @brief Check if the graph is a multigraph
            @return false (this is a simple directed graph) */
        auto is_multigraph() const { return false; }

        /** @brief Check if the graph is directed
            @return true (this is a directed graph) */
        auto is_directed() const { return true; }
    };

    /** @brief A simple directed graph with integer nodes
        @details Convenience alias for a DiGraphS with uint32_t nodes and vector-based adjacency storage */
    using SimpleDiGraphS = DiGraphS<decltype(py::range<uint32_t>(uint32_t{})),
                                    py::dict<uint32_t, int>, std::vector<py::dict<uint32_t, int>>>;

    // template <typename nodeview_t,
    //           typename adjlist_t> DiGraphS(int )
    // -> DiGraphS<decltype(py::range<int>(1)), py::set<int>>;

}  // namespace xnetwork
