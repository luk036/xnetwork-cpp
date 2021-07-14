#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_utility.hpp>
#include <type_traits>

namespace xn
{

/*!
 * @brief
 *
 * @tparam Graph
 */
template <typename Graph>
class VertexView : public Graph
{
  public:
    /*!
     * @brief Construct a new Vertex View object
     *
     * @param[in] G
     */
    explicit VertexView(Graph&& G) noexcept
        : Graph {std::forward<Graph>(G)}
    {
    }

    /*!
     * @brief
     *
     * @return auto
     */
    [[nodiscard]] auto begin() const
    {
        // auto [v_iter, v_end] = boost::vertices(*this);
        // return v_iter;
        return boost::vertices(*this).first;
    }

    /*!
     * @brief
     *
     * @return auto
     */
    [[nodiscard]] auto end() const
    {
        // auto [v_iter, v_end] = boost::vertices(*this);
        // return v_end;
        return boost::vertices(*this).second;
    }

    /*!
     * @brief
     *
     * @return auto
     */
    [[nodiscard]] auto cbegin() const
    {
        // auto [v_iter, v_end] = boost::vertices(*this);
        // return v_iter;
        return boost::vertices(*this).first;
    }

    /*!
     * @brief
     *
     * @return auto
     */
    [[nodiscard]] auto cend() const
    {
        // auto [v_iter, v_end] = boost::vertices(*this);
        // return v_end;
        return boost::vertices(*this).second;
    }
};

/*!
 * @brief
 *
 * @tparam Graph
 */
template <typename Graph>
class EdgeView
{
  private:
    const Graph& G;

  public:
    /*!
     * @brief Construct a new Edge View object
     *
     * @param[in] G
     */
    explicit EdgeView(const Graph& G)
        : G {G}
    {
    }

    /*!
     * @brief
     *
     * @return auto
     */
    [[nodiscard]] auto begin() const
    {
        // auto [e_iter, e_end] = boost::edges(_G);
        // return e_iter;
        return boost::edges(this->G).first;
    }

    /*!
     * @brief
     *
     * @return auto
     */
    [[nodiscard]] auto end() const
    {
        // auto [e_iter, e_end] = boost::edges(_G);
        // return e_end;
        return boost::edges(this->G).second;
    }

    /*!
     * @brief
     *
     * @return auto
     */
    [[nodiscard]] auto cbegin() const
    {
        // auto [e_iter, e_end] = boost::edges(_G);
        // return e_iter;
        return boost::edges(this->G).first;
    }

    /*!
     * @brief
     *
     * @return auto
     */
    [[nodiscard]] auto cend() const
    {
        // auto [e_iter, e_end] = boost::edges(_G);
        // return e_end;
        return boost::edges(this->G).second;
    }
};

/*!
 * @brief
 *
 * @tparam Vertex
 * @tparam Graph
 */
template <typename Vertex, typename Graph>
class AtlasView
{
  private:
    Vertex _v;
    const Graph& G;

  public:
    /*!
     * @brief Construct a new Atlas View object
     *
     * @param[in] v
     * @param[in] G
     */
    AtlasView(Vertex v, const Graph& G)
        : _v {v}
        , G {G}
    {
    }

    /*!
     * @brief
     *
     * @return auto
     */
    auto begin() const
    {
        // auto [e_iter, e_end] = boost::out_edges(_v, _G);
        // return e_iter;
        return boost::out_edges(this->_v, this->G).first;
    }

    /*!
     * @brief
     *
     * @return auto
     */
    auto end() const
    {
        // auto [e_iter, e_end] = boost::out_edges(_v, _G);
        // return e_end;
        return boost::out_edges(this->_v, this->G).second;
    }

    /*!
     * @brief
     *
     * @return auto
     */
    auto cbegin() const
    {
        // auto [e_iter, e_end] = boost::out_edges(_v, _G);
        // return e_iter;
        return boost::out_edges(this->_v, this->G).first;
    }

    /*!
     * @brief
     *
     * @return auto
     */
    auto cend() const
    {
        // auto [e_iter, e_end] = boost::out_edges(_v, _G);
        // return e_end;
        return boost::out_edges(this->_v, this->G).second;
    }
};

/*!
 * @brief
 *
 * @tparam Graph
 */
template <typename Graph>
class grAdaptor : public VertexView<Graph>
{
  public:
    using Vertex = typename boost::graph_traits<Graph>::vertex_descriptor;
    using node_t = Vertex;
    using edge_t = typename boost::graph_traits<Graph>::edge_descriptor;

    // using edge_wt_t = decltype( boost::get(boost::edge_weight,
    // std::declval<Graph>()) );

    /*!
     * @brief Construct a new gr Adaptor object
     *
     */
    grAdaptor() = delete;

    /*!
     * @brief Construct a new gr Adaptor object
     *
     * @param[in] G
     */
    explicit grAdaptor(Graph&& G) noexcept
        : VertexView<Graph> {std::forward<Graph>(G)}
    {
    }

    // grAdaptor(const grAdaptor&) = delete;            // don't copy
    // grAdaptor& operator=(const grAdaptor&) = delete; // don't assign
    // grAdaptor(grAdaptor&&) noexcept = default;                // don't copy

    /*!
     * @brief
     *
     * @return auto
     */
    [[nodiscard]] auto number_of_nodes() const
    {
        return boost::num_vertices(*this);
    }

    /*!
     * @brief
     *
     * @return auto
     */
    [[nodiscard]] auto number_of_edges() const
    {
        return boost::num_edges(*this);
    }

    /*!
     * @brief
     *
     * @return EdgeView<Graph>
     */
    [[nodiscard]] auto edges() const -> EdgeView<Graph>
    {
        return EdgeView<Graph>(*this);
    }

    /*!
     * @brief
     *
     * @param[in] v
     * @return AtlasView<Vertex, Graph>
     */
    [[nodiscard]] auto neighbors(Vertex v) const -> AtlasView<Vertex, Graph>
    {
        return AtlasView<Vertex, Graph>(v, *this);
    }

    /*!
     * @brief
     *
     * @param[in] u
     * @param[in] v
     * @return auto
     */
    auto add_edge(int u, int v)
    {
        return boost::add_edge(u, v, *this);
    }

    /*!
     * @brief
     *
     * @return Vertex
     */
    static auto null_vertex() -> Vertex
    {
        return boost::graph_traits<Graph>::null_vertex();
    }

    /*!
     * @brief
     *
     * @tparam Edge
     * @param[in] e
     * @return Vertex
     */
    template <typename Edge>
    auto source(const Edge& e) const -> Vertex
    {
        return boost::source(e, *this);
    }

    /*!
     * @brief
     *
     * @tparam Edge
     * @param[in] e
     * @return Vertex
     */
    template <typename Edge>
    auto target(const Edge& e) const -> Vertex
    {
        return boost::target(e, *this);
    }

    /*!
     * @brief
     *
     * @tparam Edge
     * @param[in] e
     * @return auto
     */
    template <typename Edge>
    [[nodiscard]] auto end_points(const Edge& e) const
    {
        auto s = boost::source(e, *this);
        auto t = boost::target(e, *this);
        return std::make_pair(s, t);
    }
};

} // namespace xn
