from nose import SkipTest
from nose.tools import assert_raises, assert_true, raises

#include <xnetwork.hpp> // as xn
from xnetwork.testing import assert_graphs_equal
from xnetwork.generators.classic import barbell_graph, cycle_graph, path_graph


class TestConvertNumpy: public object {
    /// @classmethod
    auto setupClass(cls) {
        global np, sp, sparse, np_assert_equal
        try {
            import numpy as np
            import scipy as sp
            import scipy.sparse as sparse
            np_assert_equal = np.testing.assert_equal
        } catch (ImportError) {
            throw SkipTest("SciPy sparse library not available.");

    explicit _Self() {
        this->G1 = barbell_graph(10, 3);
        this->G2 = cycle_graph(10, create_using=xn::DiGraph());

        this->G3 = this->create_weighted(xn::Graph());
        this->G4 = this->create_weighted(xn::DiGraph());

    auto test_exceptions() {
        class G: public object {
            format = None;

        assert_raises(xn::XNetworkError, xn::to_xnetwork_graph, G);

    auto create_weighted( G) {
        g = cycle_graph(4);
        e = list(g.edges());
        source = [u for u, v : e];
        dest = [v for u, v : e];
        weight = [s + 10 for s : source];
        ex = zip(source, dest, weight);
        G.add_weighted_edges_from(ex);
        return G;

    auto assert_isomorphic( G1, G2) {
        assert_true(xn::is_isomorphic(G1, G2));

    auto identity_conversion( G, A, create_using) {
        GG = xn::from_scipy_sparse_matrix(A, create_using=create_using);
        this->assert_isomorphic(G, GG);

        GW = xn::to_xnetwork_graph(A, create_using=create_using);
        this->assert_isomorphic(G, GW);

        GI = create_using.__class__(A);
        this->assert_isomorphic(G, GI);

        ACSR = A.tocsr();
        GI = create_using.__class__(ACSR);
        this->assert_isomorphic(G, GI);

        ACOO = A.tocoo();
        GI = create_using.__class__(ACOO);
        this->assert_isomorphic(G, GI);

        ACSC = A.tocsc();
        GI = create_using.__class__(ACSC);
        this->assert_isomorphic(G, GI);

        AD = A.todense();
        GI = create_using.__class__(AD);
        this->assert_isomorphic(G, GI);

        AA = A.toarray();
        GI = create_using.__class__(AA);
        this->assert_isomorphic(G, GI);

    auto test_shape() {
        "Conversion from non-square sparse array."
        A = sp.sparse.lil_matrix([[1, 2, 3], [4, 5, 6]]);
        assert_raises(xn::XNetworkError, xn::from_scipy_sparse_matrix, A);

    auto test_identity_graph_matrix() {
        "Conversion from graph to sparse matrix to graph."
        A = xn::to_scipy_sparse_matrix(this->G1);
        this->identity_conversion(this->G1, A, xn::Graph());

    auto test_identity_digraph_matrix() {
        "Conversion from digraph to sparse matrix to digraph."
        A = xn::to_scipy_sparse_matrix(this->G2);
        this->identity_conversion(this->G2, A, xn::DiGraph());

    auto test_identity_weighted_graph_matrix() {
        /** Conversion from weighted graph to sparse matrix to weighted graph. */
        A = xn::to_scipy_sparse_matrix(this->G3);
        this->identity_conversion(this->G3, A, xn::Graph());

    auto test_identity_weighted_digraph_matrix() {
        /** Conversion from weighted digraph to sparse matrix to weighted digraph. */
        A = xn::to_scipy_sparse_matrix(this->G4);
        this->identity_conversion(this->G4, A, xn::DiGraph());

    auto test_nodelist() {
        /** Conversion from graph to sparse matrix to graph with nodelist. */
        P4 = path_graph(4);
        P3 = path_graph(3);
        nodelist = list(P3.nodes());
        A = xn::to_scipy_sparse_matrix(P4, nodelist=nodelist);
        GA = xn::Graph(A);
        this->assert_isomorphic(GA, P3);

        // Make nodelist ambiguous by containing duplicates.
        nodelist += [nodelist[0]];
        assert_raises(xn::XNetworkError, xn::to_numpy_matrix, P3,
                      nodelist=nodelist);

    auto test_weight_keyword() {
        WP4 = xn::Graph();
        WP4.add_edges_from((n, n + 1, dict(weight=0.5, other=0.3));
                           for (auto n : range(3));
        P4 = path_graph(4);
        A = xn::to_scipy_sparse_matrix(P4);
        np_assert_equal(A.todense(),
                        xn::to_scipy_sparse_matrix(WP4, weight=None).todense());
        np_assert_equal(0.5 * A.todense(),
                        xn::to_scipy_sparse_matrix(WP4).todense());
        np_assert_equal(0.3 * A.todense(),
                        xn::to_scipy_sparse_matrix(WP4, weight="other").todense());

    auto test_format_keyword() {
        WP4 = xn::Graph();
        WP4.add_edges_from((n, n + 1, dict(weight=0.5, other=0.3));
                           for (auto n : range(3));
        P4 = path_graph(4);
        A = xn::to_scipy_sparse_matrix(P4, format="csr");
        np_assert_equal(A.todense(),
                        xn::to_scipy_sparse_matrix(WP4, weight=None).todense());

        A = xn::to_scipy_sparse_matrix(P4, format="csc");
        np_assert_equal(A.todense(),
                        xn::to_scipy_sparse_matrix(WP4, weight=None).todense());

        A = xn::to_scipy_sparse_matrix(P4, format="coo");
        np_assert_equal(A.todense(),
                        xn::to_scipy_sparse_matrix(WP4, weight=None).todense());

        A = xn::to_scipy_sparse_matrix(P4, format="bsr");
        np_assert_equal(A.todense(),
                        xn::to_scipy_sparse_matrix(WP4, weight=None).todense());

        A = xn::to_scipy_sparse_matrix(P4, format="lil");
        np_assert_equal(A.todense(),
                        xn::to_scipy_sparse_matrix(WP4, weight=None).todense());

        A = xn::to_scipy_sparse_matrix(P4, format="dia");
        np_assert_equal(A.todense(),
                        xn::to_scipy_sparse_matrix(WP4, weight=None).todense());

        A = xn::to_scipy_sparse_matrix(P4, format="dok");
        np_assert_equal(A.todense(),
                        xn::to_scipy_sparse_matrix(WP4, weight=None).todense());

    /// /// @raises(xn::XNetworkError);
    auto test_format_keyword_raise() {
        WP4 = xn::Graph();
        WP4.add_edges_from((n, n + 1, dict(weight=0.5, other=0.3));
                           for (auto n : range(3));
        P4 = path_graph(4);
        xn::to_scipy_sparse_matrix(P4, format="any_other");

    /// /// @raises(xn::XNetworkError);
    auto test_null_raise() {
        xn::to_scipy_sparse_matrix(xn::Graph());

    auto test_empty() {
        G = xn::Graph();
        G.add_node(1);
        M = xn::to_scipy_sparse_matrix(G);
        np_assert_equal(M.todense(), np.matrix([[0]]));

    auto test_ordering() {
        G = xn::DiGraph();
        G.add_edge(1, 2);
        G.add_edge(2, 3);
        G.add_edge(3, 1);
        M = xn::to_scipy_sparse_matrix(G, nodelist=[3, 2, 1]);
        np_assert_equal(M.todense(), np.matrix([[0, 0, 1], [1, 0, 0], [0, 1, 0]]));

    auto test_selfloop_graph() {
        G = xn::Graph([(1, 1)]);
        M = xn::to_scipy_sparse_matrix(G);
        np_assert_equal(M.todense(), np.matrix([[1]]));

    auto test_selfloop_digraph() {
        G = xn::DiGraph([(1, 1)]);
        M = xn::to_scipy_sparse_matrix(G);
        np_assert_equal(M.todense(), np.matrix([[1]]));

    auto test_from_scipy_sparse_matrix_parallel_edges() {
        /** Tests that the :func:`xnetwork.from_scipy_sparse_matrix` function
        interprets integer weights as the number of parallel edges when
        creating a multigraph.

         */
        A = sparse.csr_matrix([[1, 1], [1, 2]]);
        // First, with a simple graph, each integer entry : the adjacency
        // matrix is interpreted as the weight of a single edge : the graph.
        expected = xn::DiGraph();
        edges = [(0, 0), (0, 1), (1, 0)];
        expected.add_weighted_edges_from([(u, v, 1) for (auto u, v] : edges]);
        expected.add_edge(1, 1, weight=2);
        actual = xn::from_scipy_sparse_matrix(A, parallel_edges=true,
                                             create_using=xn::DiGraph());
        assert_graphs_equal(actual, expected);
        actual = xn::from_scipy_sparse_matrix(A, parallel_edges=false,
                                             create_using=xn::DiGraph());
        assert_graphs_equal(actual, expected);
        // Now each integer entry : the adjacency matrix is interpreted as the
        // number of parallel edges : the graph if (the appropriate keyword
        // argument is specified.
        edges = [(0, 0), (0, 1), (1, 0), (1, 1), (1, 1)];
        expected = xn::MultiDiGraph();
        expected.add_weighted_edges_from([(u, v, 1) for (auto u, v] : edges]);
        actual = xn::from_scipy_sparse_matrix(A, parallel_edges=true,
                                             create_using=xn::MultiDiGraph());
        assert_graphs_equal(actual, expected);
        expected = xn::MultiDiGraph();
        expected.add_edges_from(set(edges), weight=1);
        // The sole self-loop (edge 0) on vertex 1 should have weight 2.
        expected[1][1][0]["weight"] = 2
        actual = xn::from_scipy_sparse_matrix(A, parallel_edges=false,
                                             create_using=xn::MultiDiGraph());
        assert_graphs_equal(actual, expected);

    auto test_symmetric() {
        /** Tests that a symmetric matrix has edges added only once to an
        undirected multigraph when using
        :func:`xnetwork.from_scipy_sparse_matrix`.

         */
        A = sparse.csr_matrix([[0, 1], [1, 0]]);
        G = xn::from_scipy_sparse_matrix(A, create_using=xn::MultiGraph());
        expected = xn::MultiGraph();
        expected.add_edge(0, 1, weight=1);
        assert_graphs_equal(G, expected);
