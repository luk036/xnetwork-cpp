#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_utility.hpp>
#include <type_traits>

namespace py {

    /**
     * @brief A view class for iterating over vertices in a Boost Graph
     *
     * This class wraps a Boost Graph and provides iterators for traversing
     * all vertices in the graph.
     *
     * @tparam Graph The Boost graph type
     */
    template <typename Graph> class VertexView : public Graph {
      public:
        /**
         * @brief Construct a new Vertex View object
         *
         * @param[in] gra The graph to create a view of
         */
        explicit VertexView(Graph&& gra) noexcept : Graph{std::forward<Graph>(gra)} {}

        /**
         * @brief Get iterator to the first vertex
         * @return Iterator to the first vertex
         */
        typename boost::graph_traits<Graph>::vertex_iterator begin() const {
            // auto [v_iter, v_end] = boost::vertices(*this);
            // return v_iter;
            return boost::vertices(*this).first;
        }

        /**
         * @brief Get iterator past the last vertex
         * @return Iterator past the last vertex
         */
        typename boost::graph_traits<Graph>::vertex_iterator end() const {
            // auto [v_iter, v_end] = boost::vertices(*this);
            // return v_end;
            return boost::vertices(*this).second;
        }

        /**
         * @brief Get const iterator to the first vertex
         * @return Const iterator to the first vertex
         */
        typename boost::graph_traits<Graph>::vertex_iterator cbegin() const {
            // auto [v_iter, v_end] = boost::vertices(*this);
            // return v_iter;
            return boost::vertices(*this).first;
        }

        /**
         * @brief Get const iterator past the last vertex
         * @return Const iterator past the last vertex
         */
        typename boost::graph_traits<Graph>::vertex_iterator cend() const {
            // auto [v_iter, v_end] = boost::vertices(*this);
            // return v_end;
            return boost::vertices(*this).second;
        }
    };

    /**
     * @brief A view class for iterating over edges in a Boost Graph
     *
     * This class provides iterators for traversing all edges in the graph.
     *
     * @tparam Graph The Boost graph type
     */
    template <typename Graph> class EdgeView {
      private:
        const Graph& gra;

      public:
        /**
         * @brief Construct a new Edge View object
         *
         * @param[in] gra The graph to create a view of
         */
        explicit EdgeView(const Graph& gra) : gra{gra} {}

        /**
         * @brief Get iterator to the first edge
         * @return Iterator to the first edge
         */
        typename boost::graph_traits<Graph>::edge_iterator begin() const {
            // auto [e_iter, e_end] = boost::edges(_gra);
            // return e_iter;
            return boost::edges(this->gra).first;
        }

        /**
         * @brief Get iterator past the last edge
         * @return Iterator past the last edge
         */
        typename boost::graph_traits<Graph>::edge_iterator end() const {
            // auto [e_iter, e_end] = boost::edges(_gra);
            // return e_end;
            return boost::edges(this->gra).second;
        }

        /**
         * @brief Get const iterator to the first edge
         * @return Const iterator to the first edge
         */
        typename boost::graph_traits<Graph>::edge_iterator cbegin() const {
            // auto [e_iter, e_end] = boost::edges(_gra);
            // return e_iter;
            return boost::edges(this->gra).first;
        }

        /**
         * @brief Get const iterator past the last edge
         * @return Const iterator past the last edge
         */
        typename boost::graph_traits<Graph>::edge_iterator cend() const {
            // auto [e_iter, e_end] = boost::edges(_gra);
            // return e_end;
            return boost::edges(this->gra).second;
        }
    };

    /**
     * @brief A view class for iterating over outgoing edges of a vertex
     *
     * This class provides iterators for traversing all outgoing edges
     * from a given vertex in the graph.
     *
     * @tparam Vertex The vertex descriptor type
     * @tparam Graph The Boost graph type
     */
    template <typename Vertex, typename Graph> class AtlasView {
      private:
        Vertex _v;
        const Graph& gra;

      public:
        /**
         * @brief Construct a new Atlas View object
         *
         * @param[in] vertex The source vertex
         * @param[in] gra The graph to create a view of
         */
        AtlasView(Vertex vertex, const Graph& gra) : _v{vertex}, gra{gra} {}

        /**
         * @brief Get iterator to the first outgoing edge
         * @return Iterator to the first outgoing edge
         */
        typename boost::graph_traits<Graph>::out_edge_iterator begin() const {
            // auto [e_iter, e_end] = boost::out_edges(_v, _gra);
            // return e_iter;
            return boost::out_edges(this->_v, this->gra).first;
        }

        /**
         * @brief Get iterator past the last outgoing edge
         * @return Iterator past the last outgoing edge
         */
        typename boost::graph_traits<Graph>::out_edge_iterator end() const {
            // auto [e_iter, e_end] = boost::out_edges(_v, _gra);
            // return e_end;
            return boost::out_edges(this->_v, this->gra).second;
        }

        /**
         * @brief Get const iterator to the first outgoing edge
         * @return Const iterator to the first outgoing edge
         */
        typename boost::graph_traits<Graph>::out_edge_iterator cbegin() const {
            // auto [e_iter, e_end] = boost::out_edges(_v, _gra);
            // return e_iter;
            return boost::out_edges(this->_v, this->gra).first;
        }

        /**
         * @brief Get const iterator past the last outgoing edge
         * @return Const iterator past the last outgoing edge
         */
        typename boost::graph_traits<Graph>::out_edge_iterator cend() const {
            // auto [e_iter, e_end] = boost::out_edges(_v, _gra);
            // return e_end;
            return boost::out_edges(this->_v, this->gra).second;
        }
    };

    /**
     * @brief Graph adaptor for Boost Graph Library interoperability
     *
     * This class provides a bridge between xnetwork and Boost Graph Library,
     * allowing Boost algorithms to work with xnetwork graph structures.
     *
     * @tparam _Graph The Boost graph type
     */
    template <typename _Graph> class GrAdaptor : public VertexView<_Graph> {
      public:
        using Vertex = typename boost::graph_traits<_Graph>::vertex_descriptor;
        using node_t = Vertex;
        using edge_t = typename boost::graph_traits<_Graph>::edge_descriptor;

        // using edge_wt_t = decltype( boost::get(boost::edge_weight,
        // std::declval<_Graph>()) );

        /**
         * @brief Default constructor (deleted)
         */
        GrAdaptor() = delete;

        /**
         * @brief Construct a new GrAdaptor from a graph
         *
         * @param[in] gra The graph to adapt
         */
        explicit GrAdaptor(_Graph&& gra) noexcept : VertexView<_Graph>{std::forward<_Graph>(gra)} {}

        // GrAdaptor(const GrAdaptor&) = delete;            // don't copy
        // GrAdaptor& operator=(const GrAdaptor&) = delete; // don't assign
        // GrAdaptor(GrAdaptor&&) noexcept = default;                // don't copy

        /**
         * @brief Get the number of vertices in the graph
         * @return Number of vertices
         */
        typename boost::graph_traits<_Graph>::vertices_size_type number_of_nodes() const {
            return boost::num_vertices(*this);
        }

        /**
         * @brief Get the number of edges in the graph
         * @return Number of edges
         */
        typename boost::graph_traits<_Graph>::edges_size_type number_of_edges() const {
            return boost::num_edges(*this);
        }

        /**
         * @brief Get a view of all edges in the graph
         * @return EdgeView for iterating over edges
         */
        auto edges() const -> EdgeView<_Graph> { return EdgeView<_Graph>(*this); }

        /**
         * @brief Get a view of neighbors for a vertex
         *
         * @param[in] vertex The vertex to get neighbors for
         * @return AtlasView for iterating over outgoing edges
         */
        auto neighbors(Vertex vertex) const -> AtlasView<Vertex, _Graph> {
            return AtlasView<Vertex, _Graph>(vertex, *this);
        }

        /**
         * @brief Add an edge between two vertices
         *
         * @param[in] node_u Source vertex
         * @param[in] node_v Target vertex
         * @return Pair of edge descriptor and success flag
         */
        std::pair<typename boost::graph_traits<_Graph>::edge_descriptor, bool> add_edge(
            int node_u, int node_v) {
            return boost::add_edge(node_u, node_v, *this);
        }

        /**
         * @brief Get the null vertex descriptor
         * @return Null vertex descriptor
         */
        static auto null_vertex() -> Vertex { return boost::graph_traits<_Graph>::null_vertex(); }

        /**
         * @brief Get the source vertex of an edge
         *
         * @tparam Edge The edge descriptor type
         * @param[in] edge The edge to get source of
         * @return Source vertex
         */
        template <typename Edge> auto source(const Edge& edge) const -> Vertex {
            return boost::source(edge, *this);
        }

        /**
         * @brief Get the target vertex of an edge
         *
         * @tparam Edge The edge descriptor type
         * @param[in] edge The edge to get target of
         * @return Target vertex
         */
        template <typename Edge> auto target(const Edge& edge) const -> Vertex {
            return boost::target(edge, *this);
        }

        /**
         * @brief Get both endpoints of an edge
         *
         * @tparam Edge The edge descriptor type
         * @param[in] edge The edge to get endpoints of
         * @return Pair of source and target vertices
         */
        template <typename Edge> std::pair<Vertex, Vertex> end_points(const Edge& edge) const {
            auto src = boost::source(edge, *this);
            auto tgt = boost::target(edge, *this);
            return std::make_pair(src, tgt);
        }
    };

}  // namespace py
