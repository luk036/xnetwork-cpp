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
        explicit VertexView(Graph &&gra) noexcept : Graph{std::forward<Graph>(gra)} {}

        auto begin() const {
            // auto [v_iter, v_end] = boost::vertices(*this);
            // return v_iter;
            return boost::vertices(*this).first;
        }

        auto end() const {
            // auto [v_iter, v_end] = boost::vertices(*this);
            // return v_end;
            return boost::vertices(*this).second;
        }

        auto cbegin() const {
            // auto [v_iter, v_end] = boost::vertices(*this);
            // return v_iter;
            return boost::vertices(*this).first;
        }

        auto cend() const {
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
        const Graph &gra;

      public:
        /**
         * @brief Construct a new Edge View object
         *
         * @param[in] gra
         */
        explicit EdgeView(const Graph &gra) : gra{gra} {}

        auto begin() const {
            // auto [e_iter, e_end] = boost::edges(_gra);
            // return e_iter;
            return boost::edges(this->gra).first;
        }

        /**
         * @brief
         *
         * @return auto
         */
        auto end() const {
            // auto [e_iter, e_end] = boost::edges(_gra);
            // return e_end;
            return boost::edges(this->gra).second;
        }

        /**
         * @brief
         *
         * @return auto
         */
        auto cbegin() const {
            // auto [e_iter, e_end] = boost::edges(_gra);
            // return e_iter;
            return boost::edges(this->gra).first;
        }

        /**
         * @brief
         *
         * @return auto
         */
        auto cend() const {
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
        const Graph &gra;

      public:
        /**
         * @brief Construct a new Atlas View object
         *
         * @param[in] v
         * @param[in] gra
         */
        AtlasView(Vertex v, const Graph &gra) : _v{v}, gra{gra} {}

        /**
         * @brief
         *
         * @return auto
         */
        auto begin() const {
            // auto [e_iter, e_end] = boost::out_edges(_v, _gra);
            // return e_iter;
            return boost::out_edges(this->_v, this->gra).first;
        }

        /**
         * @brief
         *
         * @return auto
         */
        auto end() const {
            // auto [e_iter, e_end] = boost::out_edges(_v, _gra);
            // return e_end;
            return boost::out_edges(this->_v, this->gra).second;
        }

        /**
         * @brief
         *
         * @return auto
         */
        auto cbegin() const {
            // auto [e_iter, e_end] = boost::out_edges(_v, _gra);
            // return e_iter;
            return boost::out_edges(this->_v, this->gra).first;
        }

        /**
         * @brief
         *
         * @return auto
         */
        auto cend() const {
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
        explicit GrAdaptor(_Graph &&gra) noexcept : VertexView<_Graph>{std::forward<_Graph>(gra)} {}

        // GrAdaptor(const GrAdaptor&) = delete;            // don't copy
        // GrAdaptor& operator=(const GrAdaptor&) = delete; // don't assign
        // GrAdaptor(GrAdaptor&&) noexcept = default;                // don't copy

        auto number_of_nodes() const { return boost::num_vertices(*this); }

        auto number_of_edges() const { return boost::num_edges(*this); }

        /**
         * @brief
         *
         * @return EdgeView<_Graph>
         */
        auto edges() const -> EdgeView<_Graph> { return EdgeView<_Graph>(*this); }

        /**
         * @brief
         *
         * @param[in] v
         * @return AtlasView<Vertex, _Graph>
         */
        auto neighbors(Vertex v) const -> AtlasView<Vertex, _Graph> {
            return AtlasView<Vertex, _Graph>(v, *this);
        }

        /**
         * @brief
         *
         * @param[in] u
         * @param[in] v
         */
        auto add_edge(int u, int v) { return boost::add_edge(u, v, *this); }

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
         * @param[in] e
         * @return Vertex
         */
        template <typename Edge> auto source(const Edge &e) const -> Vertex {
            return boost::source(e, *this);
        }

        /**
         * @brief
         *
         * @tparam Edge
         * @param[in] e
         * @return Vertex
         */
        template <typename Edge> auto target(const Edge &e) const -> Vertex {
            return boost::target(e, *this);
        }

        /**
         * @brief
         *
         * @tparam Edge
         * @param[in] e
         */
        template <typename Edge> auto end_points(const Edge &e) const {
            auto s = boost::source(e, *this);
            auto t = boost::target(e, *this);
            return std::make_pair(s, t);
        }
    };

}  // namespace py
