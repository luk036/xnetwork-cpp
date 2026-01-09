#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_utility.hpp>
#include <type_traits>

namespace py {

    /**
     * @brief
     *
     * @tparam Graph
     */
    template <typename Graph> class VertexView : public Graph {
      public:
        /**
         * @brief Construct a new Vertex View object
         *
         * @param[in] gra
         */
        explicit VertexView(Graph&& gra) noexcept : Graph{std::forward<Graph>(gra)} {}

        typename boost::graph_traits<Graph>::vertex_iterator begin() const {
            // auto [v_iter, v_end] = boost::vertices(*this);
            // return v_iter;
            return boost::vertices(*this).first;
        }

        typename boost::graph_traits<Graph>::vertex_iterator end() const {
            // auto [v_iter, v_end] = boost::vertices(*this);
            // return v_end;
            return boost::vertices(*this).second;
        }

        typename boost::graph_traits<Graph>::vertex_iterator cbegin() const {
            // auto [v_iter, v_end] = boost::vertices(*this);
            // return v_iter;
            return boost::vertices(*this).first;
        }

        typename boost::graph_traits<Graph>::vertex_iterator cend() const {
            // auto [v_iter, v_end] = boost::vertices(*this);
            // return v_end;
            return boost::vertices(*this).second;
        }
    };

    /**
     * @brief
     *
     * @tparam Graph
     */
    template <typename Graph> class EdgeView {
      private:
        const Graph& gra;

      public:
        /**
         * @brief Construct a new Edge View object
         *
         * @param[in] gra
         */
        explicit EdgeView(const Graph& gra) : gra{gra} {}

        typename boost::graph_traits<Graph>::edge_iterator begin() const {
            // auto [e_iter, e_end] = boost::edges(_gra);
            // return e_iter;
            return boost::edges(this->gra).first;
        }

        /**
         * @brief
         *
         * @return auto
         */
        typename boost::graph_traits<Graph>::edge_iterator end() const {
            // auto [e_iter, e_end] = boost::edges(_gra);
            // return e_end;
            return boost::edges(this->gra).second;
        }

        /**
         * @brief
         *
         * @return auto
         */
        typename boost::graph_traits<Graph>::edge_iterator cbegin() const {
            // auto [e_iter, e_end] = boost::edges(_gra);
            // return e_iter;
            return boost::edges(this->gra).first;
        }

        /**
         * @brief
         *
         * @return auto
         */
        typename boost::graph_traits<Graph>::edge_iterator cend() const {
            // auto [e_iter, e_end] = boost::edges(_gra);
            // return e_end;
            return boost::edges(this->gra).second;
        }
    };

    /**
     * @brief
     *
     * @tparam Vertex
     * @tparam Graph
     */
    template <typename Vertex, typename Graph> class AtlasView {
      private:
        Vertex _v;
        const Graph& gra;

      public:
        /**
         * @brief Construct a new Atlas View object
         *
         * @param[in] vertex
         * @param[in] gra
         */
        AtlasView(Vertex vertex, const Graph& gra) : _v{vertex}, gra{gra} {}

        /**
         * @brief
         *
         * @return auto
         */
        typename boost::graph_traits<Graph>::out_edge_iterator begin() const {
            // auto [e_iter, e_end] = boost::out_edges(_v, _gra);
            // return e_iter;
            return boost::out_edges(this->_v, this->gra).first;
        }

        /**
         * @brief
         *
         * @return auto
         */
        typename boost::graph_traits<Graph>::out_edge_iterator end() const {
            // auto [e_iter, e_end] = boost::out_edges(_v, _gra);
            // return e_end;
            return boost::out_edges(this->_v, this->gra).second;
        }

        /**
         * @brief
         *
         * @return auto
         */
        typename boost::graph_traits<Graph>::out_edge_iterator cbegin() const {
            // auto [e_iter, e_end] = boost::out_edges(_v, _gra);
            // return e_iter;
            return boost::out_edges(this->_v, this->gra).first;
        }

        /**
         * @brief
         *
         * @return auto
         */
        typename boost::graph_traits<Graph>::out_edge_iterator cend() const {
            // auto [e_iter, e_end] = boost::out_edges(_v, _gra);
            // return e_end;
            return boost::out_edges(this->_v, this->gra).second;
        }
    };

    /**
     * @brief
     *
     * @tparam _Graph
     */
    template <typename _Graph> class GrAdaptor : public VertexView<_Graph> {
      public:
        using Vertex = typename boost::graph_traits<_Graph>::vertex_descriptor;
        using node_t = Vertex;
        using edge_t = typename boost::graph_traits<_Graph>::edge_descriptor;

        // using edge_wt_t = decltype( boost::get(boost::edge_weight,
        // std::declval<_Graph>()) );

        /**
         * @brief Construct a new gr Adaptor object
         *
         */
        GrAdaptor() = delete;

        /**
         * @brief Construct a new gr Adaptor object
         *
         * @param[in] gra
         */
        explicit GrAdaptor(_Graph&& gra) noexcept : VertexView<_Graph>{std::forward<_Graph>(gra)} {}

        // GrAdaptor(const GrAdaptor&) = delete;            // don't copy
        // GrAdaptor& operator=(const GrAdaptor&) = delete; // don't assign
        // GrAdaptor(GrAdaptor&&) noexcept = default;                // don't copy

        typename boost::graph_traits<_Graph>::vertices_size_type number_of_nodes() const { return boost::num_vertices(*this); }

        typename boost::graph_traits<_Graph>::edges_size_type number_of_edges() const { return boost::num_edges(*this); }

        /**
         * @brief
         *
         * @return EdgeView<_Graph>
         */
        auto edges() const -> EdgeView<_Graph> { return EdgeView<_Graph>(*this); }

        /**
         * @brief
         *
         * @param[in] vertex
         * @return AtlasView<Vertex, _Graph>
         */
        auto neighbors(Vertex vertex) const -> AtlasView<Vertex, _Graph> {
            return AtlasView<Vertex, _Graph>(vertex, *this);
        }

        /**
         * @brief
         *
         * @param[in] node_u
         * @param[in] node_v
         */
        std::pair<typename boost::graph_traits<_Graph>::edge_descriptor, bool> add_edge(int node_u, int node_v) { return boost::add_edge(node_u, node_v, *this); }

        /**
         * @brief
         *
         * @return Vertex
         */
        static auto null_vertex() -> Vertex { return boost::graph_traits<_Graph>::null_vertex(); }

        /**
         * @brief
         *
         * @tparam Edge
         * @param[in] edge
         * @return Vertex
         */
        template <typename Edge> auto source(const Edge& edge) const -> Vertex {
            return boost::source(edge, *this);
        }

        /**
         * @brief
         *
         * @tparam Edge
         * @param[in] edge
         * @return Vertex
         */
        template <typename Edge> auto target(const Edge& edge) const -> Vertex {
            return boost::target(edge, *this);
        }

        /**
         * @brief
         *
         * @tparam Edge
         * @param[in] edge
         */
        template <typename Edge> std::pair<Vertex, Vertex> end_points(const Edge& edge) const {
            auto src = boost::source(edge, *this);
            auto tgt = boost::target(edge, *this);
            return std::make_pair(src, tgt);
        }
    };

}  // namespace py
