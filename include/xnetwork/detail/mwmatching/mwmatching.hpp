/**
 * Algorithm for finding a maximum weight matching in general graphs.
 */

#ifndef MWMATCHING_H_
#define MWMATCHING_H_

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <deque>
#include <iterator>
#include <limits>
#include <list>
#include <stack>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>

#include "concatenable_queue.hpp"
#include "priority_queue.hpp"


namespace mwmatching {

/* **************************************************
 * **        public definitions                    **
 * ************************************************** */

/** Type representing the unique ID of a vertex. */
using VertexId = unsigned int;

/** Type representing a pair of vertices. */
using VertexPair = std::pair<VertexId, VertexId>;


/** Type representing a weighted edge.
 *
 * @tparam WeightType  Numeric type used to represent edge weights.
 *                     This must be a signed type.
 */
template <typename WeightType>
struct Edge
{
    static_assert(std::numeric_limits<WeightType>::is_specialized,
                  "Edge weight must be a numeric type");

    static_assert(std::numeric_limits<WeightType>::is_signed,
                  "WeightType must be signed");

    /** Incident vertices. */
    VertexPair vt;

    /** Edge weight. */
    WeightType weight;

    Edge(VertexPair vt, WeightType w)
      : vt(vt), weight(w)
    { }

    Edge(VertexId x, VertexId y, WeightType w)
      : vt(x, y), weight(w)
    { }
};


namespace impl {

/* **************************************************
 * **        private definitions                   **
 * ************************************************** */

/** Value used to mark an invalid or undefined vertex. */
constexpr VertexId NO_VERTEX = std::numeric_limits<VertexId>::max();

/** Type representing an index in the edge list. */
using EdgeId = unsigned int;

/** Value used to mark an invalid or undefined vertex. */
constexpr EdgeId NO_EDGE = std::numeric_limits<EdgeId>::max();

/** Top-level blossoms may be labeled "S" or "T" or unlabeled. */
enum BlossomLabel { LABEL_NONE = 0, LABEL_S = 1, LABEL_T = 2 };


/* **************************************************
 * **        private helper functions              **
 * ************************************************** */

/**
 * Check that the input is a valid graph.
 *
 * The graph may not have self-edges or multiple edges between
 * the same pair of vertices. Edge weights must be within a valid range.
 *
 * This function takes time O(m * log(m)).
 *
 * @param  edges  Vector of weighted edges defining the graph.
 * @throw  std::invalid_argument  If the input graph is not valid.
 */
template <typename WeightType>
void check_input_graph(const std::vector<Edge<WeightType>>& edges)
{
    const VertexId max_num_vertex = std::numeric_limits<VertexId>::max();

    if (edges.size() >= NO_EDGE) {
        throw std::invalid_argument("Too many edges");
    }

    for (const Edge<WeightType>& edge : edges) {

        // Check that vertex IDs are valid.
        if ((edge.vt.first >= max_num_vertex)
                || (edge.vt.second >= max_num_vertex)) {
            throw std::invalid_argument("Vertex ID out of range");
        }

        // Check that edge weight is a finite number.
        if (! std::numeric_limits<WeightType>::is_integer) {
            if (! std::isfinite(edge.weight)) {
                throw std::invalid_argument(
                    "Edge weights must be finite numbers");
            }
        }

        // Check that edge weight will not cause overflow.
        if (edge.weight > std::numeric_limits<WeightType>::max() / 6) {
            throw std::invalid_argument(
                "Edge weight exceeds maximum supported value");
        }
    }

    // Check that the graph has no self-edges.
    for (const Edge<WeightType>& edge : edges) {
        if (edge.vt.first == edge.vt.second) {
            throw std::invalid_argument("Self-edges are not supported");
        }
    }

    // Check that the graph does not contain duplicate edges.
    std::vector<VertexPair> edge_endpoints;
    edge_endpoints.reserve(edges.size());
    for (const Edge<WeightType>& edge : edges) {
        VertexPair vt = edge.vt;
        if (vt.first > vt.second) {
            std::swap(vt.first, vt.second);
        }
        edge_endpoints.push_back(vt);
    }

    std::sort(edge_endpoints.begin(), edge_endpoints.end());
    if (std::unique(edge_endpoints.begin(), edge_endpoints.end())
            != edge_endpoints.end()) {
        throw std::invalid_argument("Duplicate edges are not supported");
    }
}


/* **************************************************
 * **        struct Graph                          **
 * ************************************************** */

/**
 * Representation of the input graph.
 */
template <typename WeightType>
struct Graph
{
    using EdgeT = Edge<WeightType>;

    /** Graph edges. */
    const std::vector<EdgeT> edges;

    /** Number of vertices. */
    const VertexId num_vertex;

    /** For each vertex, a vector of pointers to its incident edges. */
    const std::vector<std::vector<EdgeId>> adjacent_edges;

    /**
     * Initialize the graph representation and prepare adjacent edge lists.
     *
     * This function takes time O(n + m).
     */
    explicit Graph(const std::vector<EdgeT>& edges_in)
      : edges(remove_negative_weight_edges(edges_in))
      , num_vertex(count_num_vertex(edges))
      , adjacent_edges(build_adjacent_edges(edges, num_vertex))
    { }

    // Prevent copying.
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;

    /**
     * Copy edges and remove edges with negative weight.
     *
     * This function takes time O(m).
     *
     * @param  edges  Vector of weighted edges defining the graph.
     * @return Vector of edges with non-negative weight.
     */
    static std::vector<EdgeT> remove_negative_weight_edges(
        const std::vector<EdgeT>& edges_in)
    {
        std::vector<EdgeT> real_edges;
        real_edges.reserve(edges_in.size());

        for (const Edge<WeightType>& edge : edges_in) {
            if (edge.weight >= 0) {
                real_edges.push_back(edge);
            }
        }

        return real_edges;
    }

    /** Count the number of vertices in the graph. */
    static VertexId count_num_vertex(const std::vector<EdgeT>& edges)
    {
        VertexId num_vertex = 0;
        for (const Edge<WeightType>& edge : edges) {
            VertexId m = std::max(edge.vt.first, edge.vt.second);
            assert(m < std::numeric_limits<VertexId>::max());
            num_vertex = std::max(num_vertex, m + 1);
        }

        return num_vertex;
    }

    /**
     * Build adjacency lists for a graph.
     *
     * Note that the adjacency lists contain pointers to edge instances,
     * and therefore become invalid if the input edge vector is modified
     * in any way that invalidates its iterators.
     *
     * This function takes time O(m).
     *
     * @param  num_vertex  Number of vertices in the graph.
     * @param  edges       List of edges in the graph.
     * @return Vector of incident edges for each vertex.
     */
    static std::vector<std::vector<EdgeId>> build_adjacent_edges(
        const std::vector<EdgeT>& edges,
        VertexId num_vertex)
    {
        // Count the number of incident edges per vertex.
        std::vector<VertexId> vertex_degree(num_vertex);
        for (const EdgeT& edge : edges) {
            assert(edge.vt.first < num_vertex);
            assert(edge.vt.second < num_vertex);
            vertex_degree[edge.vt.first]++;
            vertex_degree[edge.vt.second]++;
        }

        // Build adjacency lists.
        std::vector<std::vector<EdgeId>> adjacent_edges(num_vertex);
        for (VertexId i = 0; i < num_vertex; ++i) {
            adjacent_edges[i].reserve(vertex_degree[i]);
        }
        for (EdgeId e = 0; e < edges.size(); e++) {
            const EdgeT& edge = edges[e];
            adjacent_edges[edge.vt.first].push_back(e);
            adjacent_edges[edge.vt.second].push_back(e);
        }

        return adjacent_edges;
    }
};


/* **************************************************
 * **        struct Blossom                        **
 * ************************************************** */

// Forward declaration.
template <typename WeightType> struct NonTrivialBlossom;


/**
 * A blossom is an odd-length alternating cycle over sub-blossoms.
 * An alternating path consists of alternating matched and unmatched edges.
 * An alternating cycle is an alternating path that starts and ends in
 * the same sub-blossom.
 *
 * A single vertex by itself is also a blossom: a "trivial blossom".
 *
 * An instance of this class represents either a trivial blossom,
 * or a non-trivial blossom which consists of multiple sub-blossoms.
 *
 * Blossoms are recursive structures: A non-trivial blossoms contains
 * sub-blossoms, which may themselves contain sub-blossoms etc.
 *
 * Each blossom contains exactly one vertex that is not matched to another
 * vertex in the same blossom. This is the "base vertex" of the blossom.
 */
template <typename WeightType>
struct Blossom
{
    /** Parent of this blossom, or "nullptr" if this blossom is top-level. */
    NonTrivialBlossom<WeightType>* parent;

    /** Base vertex of this blossom. */
    VertexId base_vertex;

    /** Label S or T if this is a top-level blossom in an alternating tree. */
    BlossomLabel label;

    /** True if this is an instance of NonTrivialBlossom. */
    const bool is_nontrivial_blossom;

    /** Optional edge that attaches this blossom to the alternating tree. */
    VertexPair tree_edge;

    /** Base vertex of the blossom at the root of the alternating tree. */
    VertexId tree_root;

    /** Concatenable queue containing all vertices in the blossom. */
    ConcatenableQueue<WeightType, Blossom*, EdgeId> vertex_queue;

    /** Optional node in the global delta2 queue. */
    typename PriorityQueue<WeightType, Blossom*>::Node delta2_node;

    /**
     * Accumulated pending lazy updates to the dual variables of the vertices
     * inside the blossom.
     */
    WeightType vertex_dual_offset;

protected:
    /** Initialize base class. */
    Blossom(VertexId base_vertex, bool is_nontrivial_blossom)
      : parent(nullptr)
      , base_vertex(base_vertex)
      , label(LABEL_NONE)
      , is_nontrivial_blossom(is_nontrivial_blossom)
      , tree_root(NO_VERTEX)
      , vertex_queue(this)
      , vertex_dual_offset(0)
    { }

public:
    /** Initialize a trivial (single-vertex) blossom. */
    explicit Blossom(VertexId x = NO_VERTEX)
      : Blossom(x, false)
    { }

    /**
     * Cast this Blossom instance to NonTrivialBlossom if possible,
     * otherwise return "nullptr".
     */
    NonTrivialBlossom<WeightType>* nontrivial()
    {
        return (is_nontrivial_blossom ?
                    static_cast<NonTrivialBlossom<WeightType>*>(this)
                    : nullptr);
    }

    const NonTrivialBlossom<WeightType>* nontrivial() const
    {
        return (is_nontrivial_blossom ?
                    static_cast<const NonTrivialBlossom<WeightType>*>(this)
                    : nullptr);
    }
};


/* **************************************************
 * **        struct NonTrivialBlossom              **
 * ************************************************** */

/**
 * A non-trivial blossom is a blossom that contains multiple sub-blossoms
 * (at least 3 sub-blossoms, since all blossoms have odd length).
 *
 * Non-trivial blossoms maintain a list of their sub-blossoms and the edges
 * between their subblossoms.
 *
 * Unlike trivial blossoms, each non-trivial blossom is associated with
 * a variable in the dual LPP problem.
 *
 * Non-trivial blossoms are created and destroyed by the matching algorithm.
 * This implies that not every odd-length alternating cycle is a blossom;
 * it only becomes a blossom through an explicit action of the algorithm.
 * An existing blossom may change when the matching is augmented along
 * a path that runs through the blossom.
 */
template <typename WeightType>
struct NonTrivialBlossom : public Blossom<WeightType>
{
    struct SubBlossom {
        /** Pointer to sub-blossom. */
        Blossom<WeightType>* blossom;

        /**
         * Ordered pair of vertices (x, y) where "x" is a vertex in "blossom"
         * and "y" is a vertex in the next sub-blossom.
         */
        VertexPair edge;
    };

    /** Iterator for this blossom in the list of non-trivial blossoms. */
    typename std::list<NonTrivialBlossom>::iterator this_blossom_iterator;

    /**
     * List of sub-blossoms.
     * Ordered by their appearance in the alternating cycle.
     * First blossom in the list is the start/end of the alternating cycle
     * and contains the base vertex.
     */
    std::list<SubBlossom> subblossoms;

    /**
     * Modified dual variable of this blossom.
     *
     * Every non-trivial blossom has a variable in the dual LPP. The true
     * value of the dual variable changes through delta steps, but the
     * modified dual variable is invariant under delta steps.
     *
     * For an S-blossom "b":
     *   b.dual_var = z(b) - 2 * delta_sum
     *
     * For a T-blossom "b":
     *   b.dual_var = z(b) + 2 * delta_sum
     *
     * For an unlabeled blossom or non-top-level blossom "b":
     *   b.dual_var = z(b)
     *
     * where z(b) is the true dual variable of blossom "b".
     */
    WeightType dual_var;

    /** Top-level T-blossoms are elements in the delta4 queue. */
    typename PriorityQueue<WeightType, NonTrivialBlossom*>::Node delta4_node;

    /** Initialize a non-trivial blossom. */
    NonTrivialBlossom(
            const std::vector<Blossom<WeightType>*>& subblossoms,
            const std::deque<VertexPair>& edges)
      : Blossom<WeightType>(subblossoms.front()->base_vertex, true)
      , dual_var(0)
    {
        assert(subblossoms.size() == edges.size());
        assert(subblossoms.size() % 2 == 1);
        assert(subblossoms.size() >= 3);

        auto blossom_it = subblossoms.begin();
        auto blossom_end = subblossoms.end();
        auto edge_it = edges.begin();
        while (blossom_it != blossom_end) {
            this->subblossoms.emplace_back();
            this->subblossoms.back().blossom = *blossom_it;
            this->subblossoms.back().edge = *edge_it;
            ++blossom_it;
            ++edge_it;
        }
    }

    /** Find the position of the specified subblossom. */
    std::pair<VertexId, typename std::list<SubBlossom>::iterator>
    find_subblossom(Blossom<WeightType>* subblossom)
    {
        VertexId pos = 0;
        auto it = subblossoms.begin();
        while (it->blossom != subblossom) {
            ++it;
            ++pos;
            assert(it != subblossoms.end());
        }
        return make_pair(pos, it);
    }
};


/** Call a function for every vertex inside the specified blossom. */
template <typename WeightType, typename Func>
inline void for_vertices_in_blossom(const Blossom<WeightType>* blossom, Func func)
{
    const NonTrivialBlossom<WeightType>* ntb = blossom->nontrivial();
    if (ntb) {
        // Visit all vertices in the non-trivial blossom.
        // Use an explicit stack to avoid deep call chains.
        std::vector<const NonTrivialBlossom<WeightType>*> stack;
        stack.push_back(ntb);

        while (! stack.empty()) {
            const NonTrivialBlossom<WeightType>* b = stack.back();
            stack.pop_back();

            for (const auto& sub : b->subblossoms) {
                ntb = sub.blossom->nontrivial();
                if (ntb) {
                    stack.push_back(ntb);
                } else {
                    func(sub.blossom->base_vertex);
                }
            }
        }

    } else {
        // A trivial blossom contains just one vertex.
        func(blossom->base_vertex);
    }
}


/**
 * Represents a list of edges forming an alternating path or
 * an alternating cycle.
 *
 * The path is defined over top-level blossoms; it skips parts of the path
 * that are internal to blossoms. Vertex pairs are oriented to match the
 * direction of the path.
 */
struct AlternatingPath
{
    std::deque<VertexPair> edges;
};


/* **************************************************
 * **        struct MatchingContext                **
 * ************************************************** */

/**
 * This class holds all data used by the matching algorithm.
 *
 * It contains the input graph, a partial solution, and several
 * auxiliary data structures.
 */
template <typename WeightType>
class MatchingContext
{
public:

    using EdgeT = Edge<WeightType>;
    using BlossomT = Blossom<WeightType>;
    using NonTrivialBlossomT = NonTrivialBlossom<WeightType>;
    using EdgeQueueT = PriorityQueue<WeightType, EdgeId>;

    /** Marker value for invalid edge slack, larger than any valid slack. */
    static constexpr WeightType INVALID_SLACK =
        std::numeric_limits<WeightType>::max();

    /** Specification of a delta step. */
    struct DeltaStep
    {
        /** Type of delta step: 1, 2, 3 or 4. */
        int kind;

        /** Delta value. */
        WeightType value;

        /** Edge on which the minimum delta occurs (for delta type 2 or 3). */
        VertexPair edge;

        /** Blossom on which the minimum delta occurs (for delta type 4). */
        NonTrivialBlossomT* blossom;
    };

    /** Scale integer edge weights to enable integer calculations. */
    static constexpr WeightType weight_factor =
        std::numeric_limits<WeightType>::is_integer ? 2 : 1;

    /** Input graph. */
    const Graph<WeightType> graph;

    /**
     * Current state of the matching.
     *
     * vertex_mate[x] == y          if vertex "x" is matched to vertex "y".
     * vertex_mate[x] == NO_VERTEX  if vertex "x" is unmatched.
     */
    std::vector<VertexId> vertex_mate;

    /**
     * Each vertex is associated with a trivial blossom.
     *
     * "trivial_blossom[x]" is the trivial blossom that contains vertex "x".
     */
    std::vector<BlossomT> trivial_blossom;

    /**
     * Linked list of non-trivial blossoms.
     *
     * These blossoms are created and destroyed during the course of the
     * algorithm. Keep them in a linked list such that we can add and
     * remove elements without invalidating pointers to other blossoms.
     */
    std::list<NonTrivialBlossomT> nontrivial_blossom;

    /**
     * Modified dual variable of each vertex.
     *
     * Every vertex has a variable in the dual LPP. The true value of the dual
     * variable changes through delta steps, but the modified dual variables
     * are invariant under delta steps.
     *
     * For an S-vertex "x":
     *   vertex_dual[x] = u(x) + delta_sum
     *
     * For a T-vertex "x":
     *   vertex_dual[x] = u(x) - delta_sum - B(x).vertex_dual_offset
     *
     * For an unlabeled vertex:
     *   vertex_dual[x] = u(x) - B(x).vertex_dual_offset
     *
     * where u(x) is the true dual variable of vertex "x"
     * and B(x) is the top-level blossom that contains vertex "x".
     */
    std::vector<WeightType> vertex_dual;

    /**
     * Initial value of all vertex dual variables.
     *
     * This is equal to half of the maximum edge weight.
     */
    WeightType init_vertex_dual;

    /** Running sum of applied delta steps. */
    WeightType delta_sum;

    /** For each vertex, a node in its top-level blossom. */
    typedef ConcatenableQueue<WeightType, BlossomT*, EdgeId> VertexQueueT;
    std::vector<typename VertexQueueT::Node> vertex_queue_node;

    /**
     * For each T-vertex or unlabeled vertex, a queue of its edges to
     * any S-vertex. Priority of an edge is its modified slack.
     */
    std::vector<EdgeQueueT> vertex_sedge_queue;

    /** For each edge, a node in an S-edge queue. */
    std::vector<typename EdgeQueueT::Node> vertex_sedge_node;

    /**
     * Queue of unlabeled top-level blossoms that have at least one edge
     * to an S-blossom. The priority of a blossom is its least slack to
     * an S-blossom plus delta_sum.
     */
    PriorityQueue<WeightType, BlossomT*> delta2_queue;

    /**
     * Queue of edges between S-vertices in different top-level blossoms.
     * Priority of an edge is its modified slack.
     */
    EdgeQueueT delta3_queue;

    /** For each edge, an optional node in delta3_queue. */
    std::vector<typename EdgeQueueT::Node> delta3_node;

    /**
     * Queue of top-level non-trivial T-blossoms.
     * Priority of a blossom is its modified dual variable.
     */
    PriorityQueue<WeightType, NonTrivialBlossomT*> delta4_queue;

    /** Queue of S-vertices to be scanned. */
    std::vector<VertexId> scan_queue;

    /** Markers placed while tracing an alternating path. */
    std::vector<bool> vertex_marker;

    /** Vertices marked while tracing an alternating path. */
    std::vector<VertexId> marked_vertex;

    /**
     * Initialize the matching algorithm.
     *
     * This function takes time O(n + m).
     */
    explicit MatchingContext(const std::vector<EdgeT>& edges_in)
      : graph(edges_in)
      , trivial_blossom(graph.num_vertex)
      , vertex_queue_node(graph.num_vertex)
      , vertex_sedge_queue(graph.num_vertex)
      , vertex_sedge_node(edges_in.size())
      , delta3_node(edges_in.size())
    {
        // Initially all vertices are unmatched.
        vertex_mate.resize(graph.num_vertex, NO_VERTEX);

        // Initialize a trivial blossom for each vertex.
        for (VertexId x = 0; x < graph.num_vertex; ++x) {
            trivial_blossom[x].base_vertex = x;
        }

        // Insert each vertex as the only element in its own blossom.
        for (VertexId x = 0; x < graph.num_vertex; ++x) {
            trivial_blossom[x].vertex_queue.insert(
                &vertex_queue_node[x], INVALID_SLACK, x);
        }

        // Vertex duals are initialized to half the maximum edge weight.
        WeightType max_weight = 0;
        for (const EdgeT& edge : graph.edges) {
            max_weight = std::max(max_weight, edge.weight);
        }
        init_vertex_dual = max_weight * (weight_factor / 2);
        vertex_dual.resize(graph.num_vertex, init_vertex_dual);

        delta_sum = 0;

        // Allocate temporary arrays for path tracing.
        vertex_marker.resize(graph.num_vertex);
        marked_vertex.reserve(graph.num_vertex);
    }

    // Prevent copying.
    MatchingContext(const MatchingContext&) = delete;
    MatchingContext& operator=(const MatchingContext&) = delete;

    /* **********  Find top-level blossom:  ********** */

    /**
     * Find the top-level blossom that contains vertex "x".
     *
     * This function takes time O(log(n)).
     */
    BlossomT* top_level_blossom(VertexId x) const
    {
        return vertex_queue_node[x].find();
    }

    /* **********  Least slack edge tracking:  ********** */

    /**
     * Return the modified edge slack of the specified edge.
     *
     * Modified edge slack is related to true edge slack, but adjusted
     * to make it invariant under delta steps.
     */
    WeightType edge_pseudo_slack(EdgeId e) const
    {
        const EdgeT& edge = graph.edges[e];
        VertexId x = edge.vt.first;
        VertexId y = edge.vt.second;
        return vertex_dual[x] + vertex_dual[y] - weight_factor * edge.weight;
    }

    /**
     * Add edge "e" for delta2 tracking.
     *
     * Edge "e" connects an S-vertex to a T-vertex or unlabeled vertex "y".
     *
     * This function takes time O(log(n)).
     */
    void delta2_add_edge(EdgeId e, VertexId y, BlossomT* by)
    {
        WeightType prio = edge_pseudo_slack(e);

        // Insert the edge in the S-edge queue of vertex "y".
        // Check whether this improves the min-prio of the vertex.
        assert(! vertex_sedge_node[e].valid());
        bool improved = vertex_sedge_queue[y].empty()
                        || (prio < vertex_sedge_queue[y].min_prio());
        vertex_sedge_queue[y].insert(&vertex_sedge_node[e], prio, e);

        if (improved) {
            // Update the priority of vertex "y" in its ConcatenableQueue.
            vertex_queue_node[y].set_prio(prio);

            // Update the global delta2 queue if this blossom is unlabeled
            // and the new edge becomes its least-slack S-edge.
            if (by->label == LABEL_NONE) {
                prio += by->vertex_dual_offset;
                if (by->delta2_node.valid()) {
                    if (prio < by->delta2_node.prio()) {
                        delta2_queue.set_prio(&by->delta2_node, prio);
                    }
                } else {
                    delta2_queue.insert(&by->delta2_node, prio, by);
                }
            }
        }
    }

    /**
     * Remove edge "e" from delta2 tracking.
     *
     * Edge "e" connects vertices "x" and "y".
     * Vertex "x" was an S-vertex but now becomes unlabeled.
     * Vertex "y" is a T-vertex or unlabeled vertex.
     *
     * This function takes time O(log(n)).
     */
    void delta2_remove_edge(int e, int y, BlossomT* by)
    {
        if (vertex_sedge_node[e].valid()) {
            // Delete edge from the S-edge queue.
            vertex_sedge_queue[y].remove(&vertex_sedge_node[e]);

            // If necessary, update priority of "y" in its ConcatenableQueue.
            WeightType prio = INVALID_SLACK;
            if (! vertex_sedge_queue[y].empty()) {
                prio = vertex_sedge_queue[y].min_prio();
            }

            if (prio > vertex_queue_node[y].prio()) {
                vertex_queue_node[y].set_prio(prio);
                if (by->label == LABEL_NONE) {
                    // Update or delete blossom in the global delta2 queue.
                    assert(by->delta2_node.valid());
                    prio = by->vertex_queue.min_prio();
                    if (prio < INVALID_SLACK) {
                        prio += by->vertex_dual_offset;
                        if (prio > by->delta2_node.prio()) {
                            delta2_queue.set_prio(&by->delta2_node, prio);
                        }
                    } else {
                        delta2_queue.remove(&by->delta2_node);
                    }
                }
            }
        }
    }

    /**
     * Enable delta2 tracking for "blossom".
     *
     * This function is called when a blossom becomes an unlabeled top-level
     * blossom. If the blossom has at least one edge to an S-vertex,
     * the blossom is inserted in the global delta2 queue.
     *
     * This function takes time O(log(n)).
     */
    void delta2_enable_blossom(BlossomT* blossom)
    {
        assert(! blossom->delta2_node.valid());
        WeightType prio = blossom->vertex_queue.min_prio();
        if (prio < INVALID_SLACK) {
            prio += blossom->vertex_dual_offset;
            delta2_queue.insert(&blossom->delta2_node, prio, blossom);
        }
    }

    /**
     * Disable delta2 tracking for "blossom".
     *
     * The blossom is removed from the global delta2 queue.
     * This function is called when a blossom stops being an unlabeled
     * top-level blossom.
     *
     * This function takes time O(log(n)).
     */
    void delta2_disable_blossom(BlossomT* blossom)
    {
        if (blossom->delta2_node.valid()) {
            delta2_queue.remove(&blossom->delta2_node);
        }
    }

    /**
     * Clear delta2 tracking for vertex "x".
     *
     * This function is called when "x" becomes an S-vertex.
     * It is assumed that the blossom containing "x" has already been
     * disabled for delta2 tracking.
     *
     * This function takes time O(k + log(n)),
     * where "k" is the number of edges incident on "x".
     */
    void delta2_clear_vertex(int x)
    {
        vertex_sedge_queue[x].clear();
        vertex_queue_node[x].set_prio(INVALID_SLACK);
    }

    /**
     * Find the least-slack edge between any S-vertex and any unlabeled vertex.
     *
     * This function takes time O(1).
     *
     * @return Tuple (edge_index, slack) if there is an S-to-unlabeled edge,
     *         or (NO_EDGE, 0) if there is no such edge.
     */
    std::tuple<EdgeId, WeightType> delta2_get_min_edge()
    {
        if (delta2_queue.empty()) {
            return std::make_tuple(NO_EDGE, 0);
        }

        BlossomT* blossom = delta2_queue.min_elem();
        WeightType prio = delta2_queue.min_prio();
        assert(! blossom->parent);
        assert(blossom->label == LABEL_NONE);

        WeightType slack = prio - delta_sum;
        VertexId x = blossom->vertex_queue.min_elem();
        EdgeId e = vertex_sedge_queue[x].min_elem();

        return std::make_tuple(e, slack);
    }

    /**
     * Add specified edge for delta3 tracking.
     *
     * This function is called if a vertex becomes an S-vertex and edge "e"
     * connects it to an S-vertex in a different top-level blossom.
     *
     * This function takes time O(log(n)).
     */
    void delta3_add_edge(EdgeId e)
    {
        // The edge may already be in the delta3 queue, if it was previously
        // iscovered in the opposite direction. In that case do nothing.
        if (! delta3_node[e].valid()) {
            // Insert edge. Use modified edge slack as priority.
            WeightType prio = edge_pseudo_slack(e);
            delta3_queue.insert(&delta3_node[e], prio, e);
        }
    }

    /**
     * Remove specified edge from delta3 tracking.
     *
     * This function is called if a former S-vertex becomes unlabeled
     * and edge "e" connects it to another S-vertex.
     *
     * This function takes time O(log(n)).
     */
    void delta3_remove_edge(EdgeId e)
    {
        if (delta3_node[e].valid()) {
            delta3_queue.remove(&delta3_node[e]);
        }
    }

    /**
     * Find the least-slack edge between any pair of S-vertices
     * in different top-level blossoms.
     *
     * This function takes time O(1 + k * log(n)),
     * where "k" is the number of intra-blossom edges removed from the queue.
     *
     * @return Tuple (edge_index, slack) if there is an S-to-S edge,
     *         or (NO_EDGE, 0) if there is no suitable edge.
     */
    std::tuple<EdgeId, WeightType> delta3_get_min_edge()
    {
        while (! delta3_queue.empty()) {
            EdgeId e = delta3_queue.min_elem();
            const EdgeT& edge = graph.edges[e];
            BlossomT* bx = top_level_blossom(edge.vt.first);
            BlossomT* by = top_level_blossom(edge.vt.second);
            assert(bx->label == LABEL_S && by->label == LABEL_S);
            if (bx != by) {
                WeightType slack = delta3_node[e].prio() - 2 * delta_sum;
                return std::make_tuple(e, slack);
            }

            // Reject edges between vertices in the same top-level blossom.
            // Although such edges are never inserted into the queue,
            // existing edges in the queue may become intra-blossom when
            // blossoms are merged.
            delta3_queue.remove(&delta3_node[e]);
        }

        // Queue empty; no suitable edge exists.
        return std::make_tuple(NO_EDGE, 0);
    }

    /* **********  Managing blossom labels:  ********** */

    /**
     * Change an unlabeled top-level blossom into an S-blossom.
     *
     * For a blossom with "j" vertices and "k" incident edges,
     * this function takes time O(j * log(n) + k).
     *
     * This function is called at most once per blossom per stage.
     * It therefore takes total time O(n * log(n) + m) per stage.
     */
    void assign_blossom_label_s(BlossomT* blossom)
    {
        assert(! blossom->parent);
        assert(blossom->label == LABEL_NONE);

        blossom->label = LABEL_S;

        // Labeled blossoms must not be in the delta2 queue.
        delta2_disable_blossom(blossom);

        // Unlabeled blossoms and S-blossoms use different rules
        // for modified blossom duals. Adjust the modified dual variable
        // to preserve the true blossom dual while switching labels.
        NonTrivialBlossomT* ntb = blossom->nontrivial();
        if (ntb) {
            ntb->dual_var -= 2 * delta_sum;
        }

        // Unlabeled vertices and S-vertices use different rules for
        // modified vertex duals. Calculate the adjustment that must be
        // applied to modified vertex duals to preserve the true vertex duals
        // while switching labels.
        //
        // Unlabeled vertex: vertex_dual[x] = u(x) - B(x).vertex_dual_offset
        // S-vertex:         vertex_dual[x] = u(x) + delta_sum
        //
        // For S-blossoms, "vertex_dual_offset" is always 0.
        //
        WeightType dual_fixup = delta_sum + blossom->vertex_dual_offset;
        blossom->vertex_dual_offset = 0;

        // Loop over newly labeled S-vertices.
        for_vertices_in_blossom(blossom,
            [this,dual_fixup](VertexId x) {

                // Apply adjustment to modified dual variable.
                vertex_dual[x] += dual_fixup;

                // S-vertices do not keep track of potential delta2 edges.
                delta2_clear_vertex(x);

                // Add new S-vertices to the scan queue.
                scan_queue.push_back(x);
            });
    }

    /**
     * Change an unlabeled top-level blossom into a T-blossom.
     *
     * This function takes time O(log(n)).
     */
    void assign_blossom_label_t(BlossomT* blossom)
    {
        assert(! blossom->parent);
        assert(blossom->label == LABEL_NONE);

        blossom->label = LABEL_T;

        // Labeled blossoms must not be in the delta2 queue.
        delta2_disable_blossom(blossom);

        NonTrivialBlossomT* ntb = blossom->nontrivial();
        if (ntb) {
            // Unlabeled blossoms and T-blossoms use different rules
            // for modified blossom duals. Adjust the modified dual variable
            // to preserve the true blossom dual while switching labels.
            ntb->dual_var += 2 * delta_sum;

            // Top-level T-blossoms are tracked in the delta4 queue.
            assert(! ntb->delta4_node.valid());
            delta4_queue.insert(&ntb->delta4_node, ntb->dual_var, ntb);
        }

        // Unlabeled vertices and T-vertices use different rules for
        // modified vertex duals. Adjust the dual offset to preserve the
        // true vertex duals while switching labels.
        //
        // Unlabeled vertex:
        //   vertex_dual[x] = u(x) - B(x).vertex_dual_offset
        //
        // T-vertex:
        //   vertex_dual[x] = u(x) - delta_sum - B(x).vertex_dual_offset
        //
        blossom->vertex_dual_offset -= delta_sum;
    }

    /**
     * Change a top-level S-blossom into an unlabeled blossom.
     *
     * For a blossom with "j" vertices and "k" incident edges,
     * this function takes time O((j + k) * log(n)).
     *
     * This function is called at most once per blossom per stage.
     * It therefore takes total time O((n + m) * log(n)) per stage.
     */
    void remove_blossom_label_s(BlossomT* blossom)
    {
        assert(! blossom->parent);
        assert(blossom->label == LABEL_S);

        blossom->label = LABEL_NONE;

        // Adjust modified blossom dual to preserve true blossom dual.
        NonTrivialBlossomT* ntb = blossom->nontrivial();
        if (ntb) {
            ntb->dual_var += 2 * delta_sum;
        }

        // Loop over vertices in the blossom.
        assert(blossom->vertex_dual_offset == 0);
        WeightType dual_fixup = -delta_sum;
        for_vertices_in_blossom(blossom,
            [this,blossom,dual_fixup](VertexId x) {

                // Adjust modified vertex dual to preserve true vertex dual.
                vertex_dual[x] += dual_fixup;

                // Scan incident edges.
                for (EdgeId e : graph.adjacent_edges[x]) {
                    const EdgeT& edge = graph.edges[e];
                    VertexId y = (edge.vt.first != x) ? edge.vt.first
                                                      : edge.vt.second;

                    // If this edge is in the delta3 queue, remove it.
                    delta3_remove_edge(e);

                    BlossomT* by = top_level_blossom(y);
                    if (by->label == LABEL_S) {
                        // Edge "e" connects unlabeled vertex "x" to
                        // S-vertex "y". It must be tracked for delta2.
                        delta2_add_edge(e, x, blossom);
                    } else {
                        // Edge "e" connects former S-vertex "x" to
                        // T-vertex or unlabeled vertex "y". This edge was
                        // tracked for delta2 but must now be removed.
                        delta2_remove_edge(e, y, by);
                    }
                }
            });
    }

    /**
     * Change a top-level T-blossom into an unlabeled blossom.
     *
     * This function takes time O(log(n)).
     */
    void remove_blossom_label_t(BlossomT* blossom)
    {
        assert(! blossom->parent);
        assert(blossom->label == LABEL_T);

        blossom->label = LABEL_NONE;

        NonTrivialBlossomT* ntb = blossom->nontrivial();
        if (ntb) {
            // Unlabeled blossoms are not tracked in the delta4 queue.
            assert(ntb->delta4_node.valid());
            delta4_queue.remove(&ntb->delta4_node);

            // Adjust modified blossom dual to preserve true blossom dual.
            ntb->dual_var -= 2 * delta_sum;
        }

        // Adjust dual offset to preserve true vertex duals.
        blossom->vertex_dual_offset += delta_sum;

        // Enable unlabeled top-level blossom for delta2 tracking.
        delta2_enable_blossom(blossom);
    }

    /**
     * Change a top-level S-blossom into an S-subblossom.
     *
     * This function takes time O(1).
     */
    void change_s_blossom_to_subblossom(BlossomT* blossom)
    {
        assert(! blossom->parent);
        assert(blossom->label == LABEL_S);

        blossom->label = LABEL_NONE;

        // The subblossom becomes unlabeled.
        // Adjust its modified blossom dual to preserve the true blossom dual.
        NonTrivialBlossomT* ntb = blossom->nontrivial();
        if (ntb) {
            ntb->dual_var += 2 * delta_sum;
        }
    }

    /** Remove blossom label. */
    void reset_blossom_label(BlossomT* blossom)
    {
        if (! blossom->parent) {
            if (blossom->label == LABEL_S) {
                remove_blossom_label_s(blossom);
            } else if (blossom->label == LABEL_T) {
                remove_blossom_label_t(blossom);
            }
        }
    }

    /**
     * Remove the alternating trees with specified root vertices.
     *
     * All blossoms that are part of these trees become unlabeled.
     * Delta tracking is updated accordingly.
     *
     * This function takes time O((n + m) * log(n)).
     */
    void remove_alternating_tree(VertexId x, VertexId y)
    {
        for (BlossomT& blossom : trivial_blossom) {
            if ((! blossom.parent)
                    && (blossom.label != LABEL_NONE)
                    && (blossom.tree_root == x || blossom.tree_root == y)) {
                reset_blossom_label(&blossom);
            }
        }

        for (BlossomT& blossom : nontrivial_blossom) {
            if ((! blossom.parent)
                    && (blossom.label != LABEL_NONE)
                    && (blossom.tree_root == x || blossom.tree_root == y)) {
                reset_blossom_label(&blossom);
            }
        }
    }

    /* **********  Creating and expanding blossoms:  ********** */

    /**
     * Trace back through the alternating trees from vertices "x" and "y".
     *
     * If both vertices are part of the same alternating tree, this function
     * discovers a new blossom. In this case it returns an alternating path
     * through the blossom that starts and ends in the same sub-blossom.
     *
     * If the vertices are part of different alternating trees, this function
     * discovers an augmenting path. In this case it returns an alternating
     * path that starts and ends in an unmatched vertex.
     *
     * This function takes time O(k * log(n)) to discover a blossom,
     * where "k" is the number of sub-blossoms,
     * or time O(n * log(n)) to discover an augmenting path.
     */
    AlternatingPath trace_alternating_paths(VertexId x, VertexId y)
    {
        // Initialize a path containing only the edge (x, y).
        AlternatingPath path;
        path.edges.emplace_back(x, y);

        // "first_common" is the first common ancestor of "x" and "y"
        // in the alternating tree, or "nullptr" if no common ancestor
        // has been found.
        BlossomT* first_common = nullptr;

        // Alternate between tracing the path from "x" and the path from "y".
        // This ensures that the search time is bounded by the size of any
        // newly found blossom.
        while ((x != NO_VERTEX) || (y != NO_VERTEX)) {

            // Trace path from vertex "x".
            if (x != NO_VERTEX) {

                // Stop if we found a common ancestor.
                BlossomT* bx = top_level_blossom(x);
                if (vertex_marker[bx->base_vertex]) {
                    first_common = bx;
                    break;
                }

                // Mark blossom as potential common ancestor.
                vertex_marker[bx->base_vertex] = true;
                marked_vertex.push_back(bx->base_vertex);

                // Trace back to the parent in the alternating tree.
                x = bx->tree_edge.first;
                if (x != NO_VERTEX) {
                    path.edges.push_front(bx->tree_edge);
                }
            }

            // Trace path from vertex "y".
            if (y != NO_VERTEX) {

                // Stop if we found a common ancestor.
                BlossomT* by = top_level_blossom(y);
                if (vertex_marker[by->base_vertex]) {
                    first_common = by;
                    break;
                }

                // Mark blossom as potential common ancestor.
                vertex_marker[by->base_vertex] = true;
                marked_vertex.push_back(by->base_vertex);

                // Trace back to the parent in the alternating tree.
                y = by->tree_edge.first;
                if (y != NO_VERTEX) {
                    path.edges.emplace_back(by->tree_edge.second, y);
                }
            }
        }

        // Remove all markers we placed.
        for (VertexId k : marked_vertex) {
            vertex_marker[k] = false;
        }
        marked_vertex.clear();

        // If we found a common ancestor, trim the paths so they end there.
        if (first_common) {
            assert(first_common->label == LABEL_S);
            while (top_level_blossom(path.edges.front().first)
                   != first_common) {
                path.edges.pop_front();
            }
            while (top_level_blossom(path.edges.back().second)
                   != first_common) {
                path.edges.pop_back();
            }
        }

        // Any alternating path between S-blossoms must have odd length.
        assert(path.edges.size() % 2 == 1);

        return path;
    }

    /**
     * Create a new blossom from an alternating cycle.
     *
     * Assign label S to the new blossom.
     * Relabel all T-sub-blossoms as S and add their vertices to the queue.
     *
     * This function takes total time O((n + m) * log(n)) per stage.
     */
    void make_blossom(const AlternatingPath& path)
    {
        assert(path.edges.size() % 2 == 1);
        assert(path.edges.size() >= 3);

        // Collect pointers to sub-blossoms.
        std::vector<BlossomT*> subblossoms;
        subblossoms.reserve(path.edges.size());
        for (VertexPair edge : path.edges) {
            subblossoms.push_back(top_level_blossom(edge.first));
        }

        // Check that the path is cyclic.
        VertexId pos = 0;
        for (VertexPair edge : path.edges) {
            pos = (pos + 1) % subblossoms.size();
            assert(top_level_blossom(edge.second) == subblossoms[pos]);
        }

        // Blossom must start and end with an S-blossom.
        assert(subblossoms.front()->label == LABEL_S);

        for (BlossomT* sub : subblossoms) {

            // Mark vertices inside former T-blossoms as S-vertices.
            if (sub->label == LABEL_T) {
                remove_blossom_label_t(sub);
                assign_blossom_label_s(sub);
            }

            // Remove labels from sub-blossoms.
            change_s_blossom_to_subblossom(sub);
        }

        // Create the new blossom object.
        auto blossom_iterator = nontrivial_blossom.emplace(
                                    nontrivial_blossom.end(),
                                    subblossoms,
                                    path.edges);
        NonTrivialBlossomT* blossom = &(*blossom_iterator);
        blossom->this_blossom_iterator = blossom_iterator;

        // Initialize modified blossom dual to set true dual to 0.
        blossom->dual_var = -2 * delta_sum;

        // Link the subblossoms to the their new parent.
        for (BlossomT* sub : subblossoms) {
            sub->parent = blossom;
        }

        // Merge concatenable queues.
        std::vector<VertexQueueT*> subqueues;
        subqueues.reserve(subblossoms.size());
        for (BlossomT* sub : subblossoms) {
            subqueues.push_back(&sub->vertex_queue);
        }
        blossom->vertex_queue.merge(subqueues.begin(), subqueues.end());

        // Assign label S to the new blossom and link to the alternating tree.
        blossom->label = LABEL_S;
        blossom->tree_edge = subblossoms.front()->tree_edge;
        blossom->tree_root = subblossoms.front()->tree_root;
    }

    /** Expand and delete the specified T-blossom. */
    void expand_t_blossom(NonTrivialBlossomT* blossom)
    {
        assert(! blossom->parent);
        assert(blossom->label == LABEL_T);

        // Remove label from blossom.
        remove_blossom_label_t(blossom);

        // Remove blossom from the delta2 queue.
        delta2_disable_blossom(blossom);

        // Split concatenable queue, thus reconstructing the separate
        // concatenable queues of the sub-blossoms.
        blossom->vertex_queue.split();

        // Prepare to push pending delta updates down to the sub-blossoms.
        WeightType vertex_dual_offset = blossom->vertex_dual_offset;
        blossom->vertex_dual_offset = 0;

        // Convert sub-blossoms into top-level blossoms.
        for (const auto& sub : blossom->subblossoms) {
            BlossomT* sub_blossom = sub.blossom;
            assert(sub_blossom->parent == blossom);
            assert(sub_blossom->label == LABEL_NONE);
            sub_blossom->parent = nullptr;

            // Push pending delta updates to sub-blossom.
            assert(sub_blossom->vertex_dual_offset == 0);
            sub_blossom->vertex_dual_offset = vertex_dual_offset;

            // Add unlabeled sub-blossom to the delta2 queue.
            delta2_enable_blossom(sub_blossom);
        }

        // The expanded blossom was part of an alternating tree.
        // We must now reconstruct the part of the alternating tree
        // that ran through this blossom by linking some of the sub-blossoms
        // into the tree.

        // Find the sub-blossom through which the expanding blossom was
        // attached to the alternating tree.
        BlossomT* entry = top_level_blossom(blossom->tree_edge.second);

        // Find the position of this sub-blossom within the expanding blossom.
        auto subblossom_loc = blossom->find_subblossom(entry);
        VertexId entry_pos = subblossom_loc.first;
        auto entry_it = subblossom_loc.second;

        // Get the edge that attaches this blossom to the alternating tree.
        VertexId x, y;
        std::tie(x, y) = blossom->tree_edge;

        // Walk around the expanding blossom from "entry" to its base
        // in an even number of steps.
        auto sub_begin = blossom->subblossoms.begin();
        auto sub_end = blossom->subblossoms.end();
        auto sub_it = entry_it;
        while ((sub_it != sub_begin) && (sub_it != sub_end)) {

            // Assign label T to the current sub-blossom on the path.
            // This also assigns label S to the next sub-blossom.
            extend_tree_s_to_t(x, y);

            if (entry_pos % 2 == 0) {
                // Walk two steps backward to the base.
                --sub_it;
                assert(sub_it != sub_begin);
                --sub_it;
                // Get the edge from S-sub-blossom to next sub-blossom.
                std::tie(y, x) = sub_it->edge;
            } else {
                // Walk two steps forward to the base.
                ++sub_it;
                assert(sub_it != sub_end);
                // Get the edge from S-sub-blossom to next sub-blossom.
                std::tie(x, y) = sub_it->edge;
                ++sub_it;
            }
        }

        // Finally, assign label T to the base sub-blossom.
        BlossomT* base = blossom->subblossoms.front().blossom;
        assign_blossom_label_t(base);
        base->tree_edge = std::make_pair(x, y);
        base->tree_root = blossom->tree_root;

        // Delete the expanded blossom.
        nontrivial_blossom.erase(blossom->this_blossom_iterator);
    }

    /* **********  Augmenting:  ********** */

    /**
     * Augment along an alternating path through the specified blossom,
     * from sub-blossom "entry" to the base vertex of the blossom.
     *
     * Modify the blossom to reflect that sub-blossom "entry" contains
     * the base vertex after augmenting.
     */
    void augment_blossom_rec(
            NonTrivialBlossomT* blossom,
            BlossomT* entry,
            std::stack<std::pair<NonTrivialBlossomT*, BlossomT*>>& rec_stack)
    {
        // Find the position of "entry" within "blossom".
        auto subblossom_loc = blossom->find_subblossom(entry);
        VertexId entry_pos = subblossom_loc.first;
        auto entry_it = subblossom_loc.second;

        // Walk around the blossom from "entry" to the base
        // in an even number of steps.
        auto sub_begin = blossom->subblossoms.begin();
        auto sub_end = blossom->subblossoms.end();
        auto sub_it = entry_it;
        while ((sub_it != sub_begin) && (sub_it != sub_end)) {
            VertexId x, y;
            BlossomT* bx;
            BlossomT* by;

            if (entry_pos % 2 == 0) {
                // Walk backward to the base.
                --sub_it;
                by = sub_it->blossom;
                assert(sub_it != sub_begin);
                --sub_it;
                bx = sub_it->blossom;
                std::tie(x, y) = sub_it->edge;
            } else {
                // Walk forward to the base.
                // We may wrap past the end of the subblossom list.
                ++sub_it;
                assert(sub_it != sub_end);
                std::tie(x, y) = sub_it->edge;
                bx = sub_it->blossom;
                ++sub_it;
                by = (sub_it == sub_end) ?
                    blossom->subblossoms.front().blossom :
                    sub_it->blossom;
            }

            // Pull this edge into the matching.
            vertex_mate[x] = y;
            vertex_mate[y] = x;

            // Augment through any non-trivial subblossoms touching this edge.
            NonTrivialBlossomT* bx_ntb = bx->nontrivial();
            if (bx_ntb) {
                rec_stack.emplace(bx_ntb, &trivial_blossom[x]);
            }

            NonTrivialBlossomT* by_ntb = by->nontrivial();
            if (by_ntb) {
                rec_stack.emplace(by_ntb, &trivial_blossom[y]);
            }
        }

        // Re-orient the blossom.
        if (entry_it != sub_begin) {
            // Rotate the subblossom list.
            blossom->subblossoms.splice(sub_begin,
                                        blossom->subblossoms,
                                        entry_it,
                                        sub_end);
        }

        // Update the base vertex.
        // We can pull the new base vertex from the entry sub-blossom
        // since its augmentation has already finished.
        blossom->base_vertex = entry->base_vertex;
    }

    /**
     * Augment along an alternating path through the specified blossom,
     * from sub-blossom "entry" to the base vertex of the blossom.
     *
     * Recursively handle sub-blossoms as needed.
     */
    void augment_blossom(NonTrivialBlossomT* blossom, BlossomT* entry)
    {
        // Use an explicit stack to avoid deep recursion.
        std::stack<std::pair<NonTrivialBlossomT*, BlossomT*>> rec_stack;
        rec_stack.emplace(blossom, entry);

        while (!rec_stack.empty()) {
            NonTrivialBlossomT* outer_blossom;
            BlossomT* inner_entry;
            std::tie(outer_blossom, inner_entry) = rec_stack.top();

            NonTrivialBlossomT* inner_blossom = inner_entry->parent;
            assert(inner_blossom);

            if (inner_blossom != outer_blossom) {
                // After augmenting "inner_blossom",
                // continue by augmenting its parent.
                rec_stack.top() = std::make_pair(outer_blossom,
                                                 inner_blossom);
            } else {
                // After augmenting "inner_blossom",
                // this entire "outer_blossom" will be finished.
                rec_stack.pop();
            }

            // Augment "inner_blossom".
            augment_blossom_rec(inner_blossom, inner_entry, rec_stack);
        }
    }

    /**
     * Augment the matching through the specified augmenting path.
     *
     * This function takes time O(n * log(n)).
     */
    void augment_matching(const AlternatingPath& path)
    {
        // Check that the path starts and ends in an unmatched blossom.
        assert(path.edges.size() % 2 == 1);
        assert(vertex_mate[
            top_level_blossom(path.edges.front().first)->base_vertex]
            == NO_VERTEX);
        assert(vertex_mate[
            top_level_blossom(path.edges.back().second)->base_vertex]
            == NO_VERTEX);

        // Process the unmatched edges on the augmenting path.
        auto edge_it = path.edges.begin();
        auto edge_end = path.edges.end();
        while (edge_it != edge_end) {
            VertexId x = edge_it->first;
            VertexId y = edge_it->second;

            // Augment any non-trivial blossoms that touch this edge.
            BlossomT* bx = top_level_blossom(x);
            NonTrivialBlossomT* bx_ntb = bx->nontrivial();
            if (bx_ntb) {
                augment_blossom(bx_ntb, &trivial_blossom[x]);
            }

            BlossomT* by = top_level_blossom(y);
            NonTrivialBlossomT* by_ntb = by->nontrivial();
            if (by_ntb) {
                augment_blossom(by_ntb, &trivial_blossom[y]);
            }

            // Pull this edge into the matching.
            vertex_mate[x] = y;
            vertex_mate[y] = x;

            // Move forward through the augmenting path to
            // the next edge to be matched.
            ++edge_it;
            if (edge_it == edge_end) {
                break;
            }
            ++edge_it;
        }
    }

    /* **********  Alternating tree:  ********** */

    /**
     * Assign label T to the unlabeled blossom that contains vertex "y".
     *
     * The newly labeled T-blossom is added to the alternating tree.
     * Directly afterwards, label S is assigned to the blossom that has
     * a matched edge to the base of the newly labeled T-blossom.
     * That newly labeled S-blossom is also added to the alternating tree.
     *
     * @pre "x" is an S-vertex.
     * @pre "y" is an unlabeled, matched vertex.
     * @pre The top-level blossom that contains "y" has a matched base vertex.
     * @pre There is a tight edge between vertices "x" and "y".
     */
    void extend_tree_s_to_t(VertexId x, VertexId y)
    {
        BlossomT* bx = top_level_blossom(x);
        BlossomT* by = top_level_blossom(y);
        assert(bx->label == LABEL_S);

        // Assign label T to the top-level blossom that contains vertex "y".
        assign_blossom_label_t(by);
        by->tree_edge = std::make_pair(x, y);
        by->tree_root = bx->tree_root;

        // Assign label S to the blossom that is mated to the T-blossom.
        VertexId y2 = by->base_vertex;
        VertexId z = vertex_mate[y2];
        assert(z != NO_VERTEX);

        BlossomT* bz = top_level_blossom(z);
        assign_blossom_label_s(bz);
        bz->tree_edge = std::make_pair(y2, z);
        bz->tree_root = by->tree_root;
    }

    /**
     * Add the edge between S-vertices "x" and "y".
     *
     * If the edge connects blossoms that are part of the same alternating
     * tree, this function creates a new S-blossom and returns false.
     *
     * If the edge connects two different alternating trees, an augmenting
     * path has been discovered. In this case the matching is augmented.
     *
     * @pre "x" and "y" are S-vertices in different top-level blossoms.
     * @pre There is a tight edge between vertices "x" and "y".
     *
     * @return True if the matching was augmented; otherwise false.
     */
    bool add_s_to_s_edge(VertexId x, VertexId y)
    {
        BlossomT* bx = top_level_blossom(x);
        BlossomT* by = top_level_blossom(y);

        assert(bx->label == LABEL_S);
        assert(by->label == LABEL_S);
        assert(bx != by);

        // Trace back through the alternating trees from "x" and "y".
        AlternatingPath path = trace_alternating_paths(x, y);

        if (bx->tree_root == by->tree_root) {
            // Both blossoms belong to the same alternating tree.
            // This implies that the alternating path is a cycle.
            // Use it to create a new blossom.
            make_blossom(path);
            return false;

        } else {
            // The blossoms belong to different alternating trees.
            // This implies that the alternating path is an augmenting path.

            // Remove labels from all blossoms in the two alternating trees
            // on the augmenting path.
            remove_alternating_tree(bx->tree_root, by->tree_root);

            // Augment the matching along the alternating path.
            augment_matching(path);
            return true;
        }
    }

    /**
     * Scan the incident edges of newly labeled S-vertices.
     *
     * Edges are added to delta2 tracking or delta3 tracking depending
     * on the state of the vertex on the opposite side of the edge.
     *
     * This function does not yet use the edges to extend the alternating
     * tree or find blossoms or augmenting paths, even if the edges
     * are tight. If there are such tight edges, they will be used later
     * through zero-delta steps.
     *
     * If there are "j" new S-vertices with a total of "k" incident edges,
     * this function takes time O((j + k) * log(n)).
     *
     * Since each vertex can become an S-vertex at most once per stage,
     * this function takes total time O((n + m) * log(n)) per stage.
     */
    void scan_new_s_vertices()
    {
        // Process the queue of S-vertices to be scanned.
        // This loop runs through O(n) iterations per stage.
        for (VertexId x : scan_queue) {

            // Double-check that "x" is an S-vertex.
            BlossomT* bx = top_level_blossom(x);
            assert(bx->label == LABEL_S);

            // Scan the edges that are incident on "x".
            // This loop runs through O(m) iterations per stage.
            for (EdgeId e : graph.adjacent_edges[x]) {
                const EdgeT& edge = graph.edges[e];
                VertexId y = (edge.vt.first != x) ? edge.vt.first
                                                  : edge.vt.second;

                // Ignore edges that are internal to a blossom.
                BlossomT* by = top_level_blossom(y);
                if (bx == by) {
                    continue;
                }

                if (by->label == LABEL_S) {
                    // Edge between S-vertices in different top-level blossoms.
                    // Add to delta3 tracking.
                    delta3_add_edge(e);
                } else {
                    // Edge to T-vertex or unlabeled vertex.
                    // Add to delta2 tracking.
                    delta2_add_edge(e, y, by);
                }
            }
        }

        scan_queue.clear();
    }

    /* **********  Delta steps:  ********** */

    /**
     * Calculate a delta step in the dual LPP problem.
     *
     * This function returns the minimum of the 4 types of delta values,
     * and the type of delta which obtain the minimum, and the edge or
     * blossom that produces the minimum delta, if applicable.
     *
     * This function takes time O((1 + k) * log(n)),
     * where "k" is the number of intra-blossom edges removed from
     * the delta3 queue.
     *
     * At most O(n) delta steps can occur during a stage.
     * Each edge can be inserted into the delta3 queue at most once per stage.
     * Therefore, this function takes total time O((n + m) * log(n))
     * per stage.
     *
     * @return Tuple (delta_type, delta_value, delta_edge, delta_blossom)
     */
    DeltaStep calc_dual_delta_step()
    {
        DeltaStep delta;
        delta.blossom = nullptr;

        // Compute delta1: minimum dual variable of any S-vertex.
        // All unmatched vertices have the same dual value, and this is
        // the minimum value among all S-vertices.
        delta.kind = 1;
        delta.value = init_vertex_dual - delta_sum;

        // Compute delta2: minimum slack of any edge between an S-vertex and
        // an unlabeled vertex.
        EdgeId e;
        WeightType slack;
        std::tie(e, slack) = delta2_get_min_edge();
        if ((e != NO_EDGE) && (slack <= delta.value)) {
            delta.kind = 2;
            delta.value = slack;
            delta.edge = graph.edges[e].vt;
        }

        // Compute delta3: half minimum slack of any edge between two
        // top-level S-blossoms.
        std::tie(e, slack) = delta3_get_min_edge();
        if ((e != NO_EDGE) && (slack / 2 <= delta.value)) {
            delta.kind = 3;
            delta.value = slack / 2;
            delta.edge = graph.edges[e].vt;
        }

        // Compute delta4: half minimum dual of a top-level T-blossom.
        if (! delta4_queue.empty()) {
            NonTrivialBlossomT* blossom = delta4_queue.min_elem();
            assert(! blossom->parent);
            assert(blossom->label == LABEL_T);
            // Calculate true blossom dual variable from modified dual.
            WeightType blossom_dual = blossom->dual_var - 2 * delta_sum;
            if (blossom_dual / 2 <= delta.value) {
                delta.kind = 4;
                delta.value = blossom_dual / 2;
                delta.blossom = blossom;
            }
        }

        return delta;
    }

    /* **********  Main algorithm:  ********** */

    /**
     * Assign label S to all vertices and create a separate alternating
     * tree rooted in each vertex.
     *
     * This function takes time O(n + m).
     * It is called once, at the beginning of the algorithm.
     */
    void start()
    {
        for (VertexId x = 0; x < graph.num_vertex; ++x) {
            assert(vertex_mate[x] == NO_VERTEX);

            // Assign label S.
            BlossomT* bx = top_level_blossom(x);
            assert(bx->base_vertex == x);
            assign_blossom_label_s(bx);

            // Mark blossom as the root of an alternating tree.
            bx->tree_edge = std::make_pair(NO_VERTEX, x);
            bx->tree_root = x;
        }
    }

    /**
     * Run one stage of the matching algorithm.
     *
     * The stage searches a maximum-weight augmenting path.
     * If this path is found, it is used to augment the matching,
     * thereby increasing the number of matched edges by 1.
     * If no such path is found, the matching must already be optimal.
     *
     * This function takes time O((n + m) * log(n)).
     *
     * @return True if the matching was successfully augmented;
     *         false if no further improvement is possible.
     */
    bool run_stage()
    {
        // Each pass through the following loop is a "substage".
        // The substage tries to find an augmenting path.
        // If an augmenting path is found, we augment the matching and end
        // the stage. Otherwise we update the dual LPP problem and enter the
        // next substage, or stop if no further improvement is possible.
        //
        // This loop runs through at most O(n) iterations per stage.
        while (true) {

            // Consider the incident edges of newly labeled S-vertices.
            scan_new_s_vertices();

            // Calculate delta step in the dual LPP problem.
            DeltaStep delta = calc_dual_delta_step();

            // Update the running sum of delta steps. This implicitly updates
            // the dual variables of vertices and blossoms.
            delta_sum += delta.value;

            if (delta.kind == 2) {
                // Use the edge from S-vertex to unlabeled vertex that got
                // unlocked through the delta update.
                VertexId x = delta.edge.first;
                VertexId y = delta.edge.second;
                if (top_level_blossom(x)->label != LABEL_S) {
                    std::swap(x, y);
                }
                extend_tree_s_to_t(x, y);

            } else if (delta.kind == 3) {
                // Use the S-to-S edge that got unlocked by the delta update.
                // This may reveal an augmenting path.
                VertexId x = delta.edge.first;
                VertexId y = delta.edge.second;
                bool augmented = add_s_to_s_edge(x, y);
                if (augmented) {
                    return true;
                }

            } else if (delta.kind == 4) {
                // Expand the T-blossom that reached dual value 0 through
                // the delta update.
                assert(delta.blossom);
                expand_t_blossom(delta.blossom);

            } else {
                // No further improvement possible. End the stage.
                assert(delta.kind == 1);
                return false;
            }
        }
    }

    /**
     * Remove alternating trees and apply lazy updates to dual variables.
     *
     * This function takes time O((n + m) * log(n)).
     * It is called once, at the end of the algorithm.
     */
    void cleanup()
    {
        assert(scan_queue.empty());

        auto cleanup_blossom = [this](BlossomT* blossom) {

            // Remove blossom label.
            if ((! blossom->parent) && (blossom->label != LABEL_NONE)) {
                reset_blossom_label(blossom);
            }

            // Unwind lazy delta updates to vertex dual variables.
            if (blossom->vertex_dual_offset != 0) {
                WeightType dual_fixup = blossom->vertex_dual_offset;
                blossom->vertex_dual_offset = 0;
                for_vertices_in_blossom(blossom,
                    [this,dual_fixup](VertexId x) {
                        vertex_dual[x] += dual_fixup;
                    });
            }
        };

        for (BlossomT& blossom : trivial_blossom) {
            cleanup_blossom(&blossom);
        }
        for (BlossomT& blossom : nontrivial_blossom) {
            cleanup_blossom(&blossom);
        }

        assert(delta2_queue.empty());
        assert(delta3_queue.empty());
        assert(delta4_queue.empty());
    }

    /** Run the matching algorithm. */
    void run()
    {
        // Assign label S to all vertices.
        start();

        // Improve the solution until no further improvement is possible.
        //
        // Each successful pass through this loop increases the number
        // of matched edges by 1.
        //
        // This loop runs through at most (n/2 + 1) iterations.
        // Each iteration takes time O((n + m) * log(n)).
        while (run_stage()) ;

        // Clean up and unwind lazy updates to dual variables.
        cleanup();
    }
};


/* **************************************************
 * **          struct MatchingVerifier             **
 * ************************************************** */

/** Helper class to verify that an optimal solution has been found. */
template <typename WeightType>
class MatchingVerifier
{
public:
    MatchingVerifier(const MatchingContext<WeightType>& ctx)
      : ctx(ctx)
      , graph(ctx.graph)
      , edge_duals(ctx.graph.edges.size())
    { }

    /**
     * Verify that the optimum solution has been found.
     *
     * This function takes time O(n**2).
     *
     * @return True if the solution is optimal; otherwise false.
     */
    bool verify()
    {
        return (verify_vertex_mate()
                && verify_vertex_duals()
                && verify_blossom_duals()
                && verify_blossoms_and_calc_edge_duals()
                && verify_edge_slack());
    }

private:
    using EdgeT = Edge<WeightType>;
    using BlossomT = Blossom<WeightType>;
    using NonTrivialBlossomT = NonTrivialBlossom<WeightType>;

    static bool checked_add(WeightType& result, WeightType a, WeightType b)
    {
        if (a > std::numeric_limits<WeightType>::max() - b) {
            return true;
        } else {
            result = a + b;
            return false;
        }
    }

    /** Convert edge pointer to its index in the vector "edges". */
    std::size_t edge_index(const EdgeT* edge)
    {
        return edge - graph.edges.data();
    }

    /** Check that the array "vertex_mate" is consistent. */
    bool verify_vertex_mate()
    {
        // Count matched vertices and check symmetry of "vertex_mate".
        VertexId num_matched_vertex = 0;
        for (VertexId x = 0; x < graph.num_vertex; ++x) {
            VertexId y = ctx.vertex_mate[x];
            if (y != NO_VERTEX) {
                ++num_matched_vertex;
                if (ctx.vertex_mate[y] != x) {
                    return false;
                }
            }
        }

        // Count matched edges.
        VertexId num_matched_edge = 0;
        for (const EdgeT& edge : graph.edges) {
            if (ctx.vertex_mate[edge.vt.first] == edge.vt.second) {
                ++num_matched_edge;
            }
        }

        // Check that all matched vertices correspond to matched edges.
        return (num_matched_vertex == 2 * num_matched_edge);
    }

    /**
     * Check that vertex dual variables are non-negative,
     * and all unmatched vertices have zero dual.
     */
    bool verify_vertex_duals()
    {
        for (VertexId x = 0; x < graph.num_vertex; ++x) {
            if (ctx.vertex_dual[x] < 0) {
                return false;
            }
            if ((ctx.vertex_mate[x] == NO_VERTEX) && (ctx.vertex_dual[x] != 0)) {
                return false;
            }
        }
        return true;
    }

    /** Check that blossom dual variables are non-negative. */
    bool verify_blossom_duals()
    {
        for (const NonTrivialBlossomT& blossom : ctx.nontrivial_blossom) {
            if (blossom.dual_var < 0) {
                return false;
            }
        }
        return true;
    }

    /**
     * Helper function for verifying the solution.
     *
     * Descend down the blossom tree to find edges that are contained
     * in blossoms.
     *
     * On the way down, keep track of the sum of dual variables of
     * containing blossoms. Add blossom duals to edges that are contained
     * inside blossoms.
     *
     * On the way up, keep track of the total number of matched edges
     * in subblossoms. Check that all blossoms with non-zero dual variables
     * are "full".
     *
     * @return True if successful;
     *         false if a blossom with non-zero dual is not full;
     *         false if blossom dual calculations cause numeric overflow.
     */
    bool check_blossom(const NonTrivialBlossomT* blossom)
    {
        // For each vertex "x",
        // "vertex_depth[x]" is the depth of the smallest blossom on
        // the current descent path that contains "x".
        std::vector<VertexId> vertex_depth(graph.num_vertex);

        // At each depth, keep track of the sum of blossom duals
        // along the current descent path.
        std::vector<WeightType> path_sum_dual = {0};

        // At each depth, keep track of the number of matched edges
        // along the current ascent path.
        std::vector<VertexId> path_num_matched = {0};

        // Use an explicit stack to avoid deep recursion.
        using SubBlossomList = std::list<typename NonTrivialBlossomT::SubBlossom>;
        std::stack<std::pair<const NonTrivialBlossomT*,
                             typename SubBlossomList::const_iterator>> stack;
        stack.emplace(blossom, blossom->subblossoms.begin());

        while (! stack.empty()) {
            VertexId depth = stack.size();
            auto& stack_elem = stack.top();

            blossom = stack_elem.first;
            auto subblossom_it = stack_elem.second;

            if (subblossom_it == blossom->subblossoms.begin()) {
                // We just entered this sub-blossom.
                // Update the depth of all vertices in this sub-blossom.
                for_vertices_in_blossom(blossom,
                    [&vertex_depth,depth](VertexId x) {
                        vertex_depth[x] = depth;
                    });

                // Calculate the sum of blossom duals at the new depth.
                path_sum_dual.push_back(path_sum_dual.back());
                if (checked_add(path_sum_dual.back(),
                                path_sum_dual.back(),
                                blossom->dual_var)) {
                    return false;
                }

                // Initialize the number of matched edges at the new depth.
                path_num_matched.push_back(0);

                if (blossom->subblossoms.size() < 3) {
                    return false;
                }
            }

            if (subblossom_it != blossom->subblossoms.end()) {

                // Update the sub-blossom pointer at the current depth.
                ++stack_elem.second;

                // Examine the current sub-blossom.
                BlossomT* sub = subblossom_it->blossom;
                NonTrivialBlossomT* ntb = sub->nontrivial();
                if (ntb) {
                    // Prepare to descend into this sub-blossom.
                    stack.emplace(std::make_pair(ntb, ntb->subblossoms.begin()));
                } else {
                    // Handle single vertex.
                    // For each incident edge, find the smallest blossom
                    // that contains it.
                    VertexId x = sub->base_vertex;
                    for (EdgeId e : graph.adjacent_edges[x]) {
                        const EdgeT* edge = &graph.edges[e];
                        // Only consider edges pointing out from "x".
                        if (edge->vt.first == x) {
                            VertexId y = edge->vt.second;
                            VertexId edge_depth = vertex_depth[y];
                            if (edge_depth > 0) {
                                // Found the smallest blossom that contains this edge.
                                // Add the duals of the containing blossoms.
                                if (checked_add(edge_duals[edge_index(edge)],
                                                edge_duals[edge_index(edge)],
                                                path_sum_dual[edge_depth])) {
                                    return false;
                                }

                                // Update the number of matched edges in the blossom.
                                if (ctx.vertex_mate[x] == y) {
                                    path_num_matched[edge_depth] += 1;
                                }
                            }
                        }
                    }
                }

            } else {
                // We are leaving the current sub-blossom.

                // Count the number of vertices inside this blossom.
                VertexId blossom_num_vertex = 0;
                for_vertices_in_blossom(blossom,
                    [&blossom_num_vertex](VertexId) {
                        ++blossom_num_vertex;
                    });

                // Check that the blossom is "full".
                // A blossom is full if all except one of its vertices
                // are matched to another vertex within the blossom.
                VertexId blossom_num_matched = path_num_matched[depth];
                if (blossom_num_vertex != 2 * blossom_num_matched + 1) {
                    return false;
                }

                // Update the number of matched edges in the parent blossom.
                path_num_matched[depth - 1] += path_num_matched[depth];

                // Push vertices in this sub-blossom back to the parent.
                for_vertices_in_blossom(blossom,
                    [&vertex_depth,depth](VertexId x) {
                        vertex_depth[x] = depth - 1;
                    });

                // Trim the descending path.
                path_sum_dual.pop_back();
                path_num_matched.pop_back();

                // Remove the current blossom from the stack.
                stack.pop();
            }
        }

        return true;
    }

    /**
     * Check that all blossoms are full.
     * Also calculate the sum of dual variables for every edge.
     */
    bool verify_blossoms_and_calc_edge_duals()
    {
        // For each edge, calculate the sum of its vertex duals.
        for (const EdgeT& edge : graph.edges) {
            if (checked_add(edge_duals[edge_index(&edge)],
                            ctx.vertex_dual[edge.vt.first],
                            ctx.vertex_dual[edge.vt.second])) {
                return false;
            }
        }

        // Descend down each top-level blossom.
        // Check that blossoms are full.
        // Add blossom duals to the edges contained inside the blossoms.
        // This takes total time O(n**2).
        for (const NonTrivialBlossomT& blossom : ctx.nontrivial_blossom) {
            if (! blossom.parent) {
                if (!check_blossom(&blossom)) {
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * Check that all edges have non-negative slack,
     * and check that all matched edges have zero slack.
     *
     * @pre Edge duals must be calculated before calling this function.
     */
    bool verify_edge_slack()
    {
        for (const EdgeT& edge : graph.edges) {
            WeightType duals = edge_duals[edge_index(&edge)];
            WeightType weight = ctx.weight_factor * edge.weight;

            if (weight > duals) {
                return false;
            }
            WeightType slack = duals - weight;

            if (ctx.vertex_mate[edge.vt.first] == edge.vt.second) {
                if (slack != 0) {
                    return false;
                }
            }
        }
        return true;
    }

private:
    /** Reference to the MatchingContext instance. */
    const MatchingContext<WeightType>& ctx;

    /** Reference to the input graph. */
    const Graph<WeightType>& graph;

    /**
     * For each edge, the sum of duals of its incident vertices
     * and duals of all blossoms that contain the edge.
     */
    std::vector<WeightType> edge_duals;
};


} // namespace impl


/* **************************************************
 * **        public functions                      **
 * ************************************************** */

/**
 * Compute a maximum-weighted matching in the general undirected weighted
 * graph given by "edges".
 *
 * The graph is specified as a list of edges, each edge specified as a tuple
 * of its two vertices and the edge weight.
 * There may be at most one edge between any pair of vertices.
 * No vertex may have an edge to itself.
 * The graph may be non-connected (i.e. contain multiple components).
 *
 * Vertices are indexed by consecutive, non-negative integers, such that
 * the first vertex has index 0 and the last vertex has index (n-1).
 * Edge weights may be integers or floating point numbers.
 *
 * Isolated vertices (not incident to any edge) are allowed, but not
 * recommended since such vertices consume time and memory but have
 * no effect on the maximum-weight matching.
 * Edges with negative weight are ignored.
 *
 * This function takes time O(n * (n + m) * log(n)),
 * where "n" is the number of vertices and "m" is the number of edges.
 * This function uses O(n + m) memory.
 *
 * @tparam WeightType   Type used to represent edge weights.
 *                      This must be a signed type, e.g. "long" or "double".
 *
 * @param  edges        Graph defined as a vector of weighted edges.
 *
 * @return Vector of pairs of matched vertex indices.
 *         This is a subset of the edges in the graph.
 *
 * @throw  std::invalid_argument  If the input graph is not valid.
 */
template <typename WeightType>
std::vector<VertexPair> maximum_weight_matching(
    const std::vector<Edge<WeightType>>& edges)
{
    // Check that the input meets all constraints.
    impl::check_input_graph(edges);

    // Run matching algorithm.
    impl::MatchingContext<WeightType> matching(edges);
    matching.run();

    // Verify that the solution is optimal (works only for integer weights).
    if (std::numeric_limits<WeightType>::is_integer) {
        assert(impl::MatchingVerifier<WeightType>(matching).verify());
    }

    // Extract the matched edges.
    std::vector<VertexPair> solution;
    for (const Edge<WeightType>& edge : matching.graph.edges) {
        if (matching.vertex_mate[edge.vt.first] == edge.vt.second) {
            solution.push_back(edge.vt);
        }
    }

    return solution;
}


/**
 * Adjust edge weights to compute a maximum cardinality matching.
 *
 * This function adjusts edge weights in the graph such that the
 * maximum-weight matching of the adjusted graph is a maximum-cardinality
 * matching, equal to a matching in the original graph that has maximum weight
 * out of all matchings with maximum cardinality.
 *
 * The graph is specified as a vector of edges.
 * Negative edge weights are allowed.
 *
 * This function increases all edge weights by an equal amount such that
 * the adjusted weights satisfy the following conditions:
 *   - All edge weights are positive;
 *   - The minimum edge weight is at least "n" times the difference between
 *     maximum and minimum edge weight.
 *
 * This function increases edge weights by an amount that is proportional
 * to the product of the unadjusted weight range and the number of vertices
 * in the graph. This may fail if it would cause adjusted edge weights
 * to exceed the supported numeric range. In case of floating point weights,
 * the weight adjustment may also cause increased rounding errors.
 *
 * This function takes time O(m), where "m" is the number of edges.
 *
 * @tparam WeightType   Type used to represent edge weights.
 * @param  edges        Graph defined as a vector of weighted edges.
 *
 * @return Vector of edges with adjusted weights.
 *         If no adjustments are necessary, this will be a copy of the
 *         input vector.
 *
 * @throw  std::invalid_argument  If the graph is invalid or edge weights
 *                                exceed the supported range.
 */
template <typename WeightType>
std::vector<Edge<WeightType>> adjust_weights_for_maximum_cardinality_matching(
    const std::vector<Edge<WeightType>>& edges_in)
{
    const WeightType min_safe_weight = std::numeric_limits<WeightType>::min() / 6;
    const WeightType max_safe_weight = std::numeric_limits<WeightType>::max() / 6;

    // Copy edges.
    std::vector<Edge<WeightType>> edges(edges_in);

    // Don't worry about empty graphs.
    if (edges.empty()) {
        return edges;
    }

    // Count number of vertices.
    // Determine minimum and maximum edge weight.
    VertexId num_vertex = 0;
    WeightType min_weight = edges.front().weight;
    WeightType max_weight = min_weight;

    const VertexId max_num_vertex = std::numeric_limits<VertexId>::max();
    for (const Edge<WeightType>& edge : edges) {
        VertexId m = std::max(edge.vt.first, edge.vt.second);
        if (m >= max_num_vertex) {
            throw std::invalid_argument("Vertex ID out of range");
        }
        num_vertex = std::max(num_vertex, m + 1);

        if (! std::numeric_limits<WeightType>::is_integer) {
            if (! std::isfinite(edge.weight)) {
                throw std::invalid_argument(
                    "Edge weights must be finite numbers");
            }
        }

        min_weight = std::min(min_weight, edge.weight);
        max_weight = std::max(max_weight, edge.weight);
    }

    // Calculate weight range and required weight adjustment.
    if ((min_weight < min_safe_weight) || (max_weight > max_safe_weight)) {
        throw std::invalid_argument(
            "Edge weight exceeds maximum supported value");
    }

    WeightType weight_range = max_weight - min_weight;

    if (weight_range > max_safe_weight / num_vertex) {
        throw std::invalid_argument(
            "Adjusted edge weight exceeds maximum supported value");
    }

    // Do nothing if the weights already ensure maximum-cardinality.
    if ((min_weight > 0) && (min_weight >= num_vertex * weight_range)) {
        return edges;
    }

    WeightType delta;
    if (weight_range > 0) {
        // Increase weights to make minimum edge weight large enough
        // to improve any non-maximum-cardinality matching.
        delta = num_vertex * weight_range - min_weight;
    } else {
        // All weights are the same. Increase weights to make them positive.
        delta = 1 - min_weight;
    }

    assert(delta >= 0);

    if (delta > max_safe_weight - max_weight) {
        throw std::invalid_argument(
            "Adjusted edge weight exceeds maximum supported value");
    }

    // Increase all edge weights by "delta".
    for (Edge<WeightType>& edge: edges) {
        edge.weight += delta;
    }

    return edges;
}


} // namespace mwmatching

#endif // MWMATCHING_H_
