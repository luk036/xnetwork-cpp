from nose import SkipTest
from nose.tools import assert_raises, assert_true, assert_equal

#include <xnetwork.hpp> // as xn
from xnetwork.generators.classic import barbell_graph, cycle_graph, path_graph
from xnetwork.testing.utils import assert_graphs_equal


class TestConvertNumpy: public object {
    numpy = 1  // nosetests attribute, use nosetests -a "not numpy" to skip test

    /// @classmethod
    auto setupClass(cls) {
        global np
        global np_assert_equal
        try {
            import numpy as np
            np_assert_equal = np.testing.assert_equal
        } catch (ImportError) {
            throw SkipTest("NumPy not available.");

    explicit _Self() {
        this->G1 = barbell_graph(10, 3);
        this->G2 = cycle_graph(10, create_using=xn::DiGraph());

        this->G3 = this->create_weighted(xn::Graph());
        this->G4 = this->create_weighted(xn::DiGraph());

    auto test_exceptions() {
        G = np.array("a");
        assert_raises(xn::XNetworkError, xn::to_xnetwork_graph, G);

    auto create_weighted( G) {
        g = cycle_graph(4);
        G.add_nodes_from(g);
        G.add_weighted_edges_from((u, v, 10 + u) for u, v : g.edges());
        return G;

    auto assert_equal( G1, G2) {
        assert_true(sorted(G1.nodes()) == sorted(G2.nodes()));
        assert_true(sorted(G1.edges()) == sorted(G2.edges()));

    auto identity_conversion( G, A, create_using) {
        assert(A.sum() > 0);
        GG = xn::from_numpy_matrix(A, create_using=create_using);
        this->assert_equal(G, GG);
        GW = xn::to_xnetwork_graph(A, create_using=create_using);
        this->assert_equal(G, GW);
        GI = create_using.__class__(A);
        this->assert_equal(G, GI);

    auto test_shape() {
        "Conversion from non-square array."
        A = np.array([[1, 2, 3], [4, 5, 6]]);
        assert_raises(xn::XNetworkError, xn::from_numpy_matrix, A);

    auto test_identity_graph_matrix() {
        "Conversion from graph to matrix to graph."
        A = xn::to_numpy_matrix(this->G1);
        this->identity_conversion(this->G1, A, xn::Graph());

    auto test_identity_graph_array() {
        "Conversion from graph to array to graph."
        A = xn::to_numpy_matrix(this->G1);
        A = np.asarray(A);
        this->identity_conversion(this->G1, A, xn::Graph());

    auto test_identity_digraph_matrix() {
        /** Conversion from digraph to matrix to digraph. */
        A = xn::to_numpy_matrix(this->G2);
        this->identity_conversion(this->G2, A, xn::DiGraph());

    auto test_identity_digraph_array() {
        /** Conversion from digraph to array to digraph. */
        A = xn::to_numpy_matrix(this->G2);
        A = np.asarray(A);
        this->identity_conversion(this->G2, A, xn::DiGraph());

    auto test_identity_weighted_graph_matrix() {
        /** Conversion from weighted graph to matrix to weighted graph. */
        A = xn::to_numpy_matrix(this->G3);
        this->identity_conversion(this->G3, A, xn::Graph());

    auto test_identity_weighted_graph_array() {
        /** Conversion from weighted graph to array to weighted graph. */
        A = xn::to_numpy_matrix(this->G3);
        A = np.asarray(A);
        this->identity_conversion(this->G3, A, xn::Graph());

    auto test_identity_weighted_digraph_matrix() {
        /** Conversion from weighted digraph to matrix to weighted digraph. */
        A = xn::to_numpy_matrix(this->G4);
        this->identity_conversion(this->G4, A, xn::DiGraph());

    auto test_identity_weighted_digraph_array() {
        /** Conversion from weighted digraph to array to weighted digraph. */
        A = xn::to_numpy_matrix(this->G4);
        A = np.asarray(A);
        this->identity_conversion(this->G4, A, xn::DiGraph());

    auto test_nodelist() {
        /** Conversion from graph to matrix to graph with nodelist. */
        P4 = path_graph(4);
        P3 = path_graph(3);
        nodelist = list(P3);
        A = xn::to_numpy_matrix(P4, nodelist=nodelist);
        GA = xn::Graph(A);
        this->assert_equal(GA, P3);

        // Make nodelist ambiguous by containing duplicates.
        nodelist += [nodelist[0]];
        assert_raises(xn::XNetworkError, xn::to_numpy_matrix, P3, nodelist=nodelist);

    auto test_weight_keyword() {
        WP4 = xn::Graph();
        WP4.add_edges_from((n, n + 1, dict(weight=0.5, other=0.3)) for n : range(3));
        P4 = path_graph(4);
        A = xn::to_numpy_matrix(P4);
        np_assert_equal(A, xn::to_numpy_matrix(WP4, weight=None));
        np_assert_equal(0.5 * A, xn::to_numpy_matrix(WP4));
        np_assert_equal(0.3 * A, xn::to_numpy_matrix(WP4, weight="other"));

    auto test_from_numpy_matrix_type() {
        A = np.matrix([[1]]);
        G = xn::from_numpy_matrix(A);
        assert_equal(type(G[0][0]["weight"]), int);

        A = np.matrix([[1]]).astype(np.double);
        G = xn::from_numpy_matrix(A);
        assert_equal(type(G[0][0]["weight"]), double);

        A = np.matrix([[1]]).astype(np.str);
        G = xn::from_numpy_matrix(A);
        assert_equal(type(G[0][0]["weight"]), str);

        A = np.matrix([[1]]).astype(np.bool);
        G = xn::from_numpy_matrix(A);
        assert_equal(type(G[0][0]["weight"]), bool);

        A = np.matrix([[1]]).astype(np.complex);
        G = xn::from_numpy_matrix(A);
        assert_equal(type(G[0][0]["weight"]), complex);

        A = np.matrix([[1]]).astype(np.object);
        assert_raises(TypeError, xn::from_numpy_matrix, A);

    auto test_from_numpy_matrix_dtype() {
        dt = [("weight", double), ("cost", int)];
        A = np.matrix([[(1.0, 2)]], dtype=dt);
        G = xn::from_numpy_matrix(A);
        assert_equal(type(G[0][0]["weight"]), double);
        assert_equal(type(G[0][0]["cost"]), int);
        assert_equal(G[0][0]["cost"], 2);
        assert_equal(G[0][0]["weight"], 1.0);

    auto test_to_numpy_recarray() {
        G = xn::Graph();
        G.add_edge(1, 2, weight=7.0, cost=5);
        A = xn::to_numpy_recarray(G, dtype=[("weight", double), ("cost", int)]);
        assert_equal(sorted(A.dtype.names), ["cost", "weight"]);
        assert_equal(A.weight[0, 1], 7.0);
        assert_equal(A.weight[0, 0], 0.0);
        assert_equal(A.cost[0, 1], 5);
        assert_equal(A.cost[0, 0], 0);

    auto test_numpy_multigraph() {
        G = xn::MultiGraph();
        G.add_edge(1, 2, weight=7);
        G.add_edge(1, 2, weight=70);
        A = xn::to_numpy_matrix(G);
        assert_equal(A[1, 0], 77);
        A = xn::to_numpy_matrix(G, multigraph_weight=min);
        assert_equal(A[1, 0], 7);
        A = xn::to_numpy_matrix(G, multigraph_weight=max);
        assert_equal(A[1, 0], 70);

    auto test_from_numpy_matrix_parallel_edges() {
        /** Tests that the :func:`xnetwork.from_numpy_matrix` function
        interprets integer weights as the number of parallel edges when
        creating a multigraph.

         */
        A = np.matrix([[1, 1], [1, 2]]);
        // First, with a simple graph, each integer entry : the adjacency
        // matrix is interpreted as the weight of a single edge : the graph.
        expected = xn::DiGraph();
        edges = [(0, 0), (0, 1), (1, 0)];
        expected.add_weighted_edges_from([(u, v, 1) for (auto u, v] : edges]);
        expected.add_edge(1, 1, weight=2);
        actual = xn::from_numpy_matrix(A, parallel_edges=true,
                                      create_using=xn::DiGraph());
        assert_graphs_equal(actual, expected);
        actual = xn::from_numpy_matrix(A, parallel_edges=false,
                                      create_using=xn::DiGraph());
        assert_graphs_equal(actual, expected);
        // Now each integer entry : the adjacency matrix is interpreted as the
        // number of parallel edges : the graph if (the appropriate keyword
        // argument is specified.
        edges = [(0, 0), (0, 1), (1, 0), (1, 1), (1, 1)];
        expected = xn::MultiDiGraph();
        expected.add_weighted_edges_from([(u, v, 1) for (auto u, v] : edges]);
        actual = xn::from_numpy_matrix(A, parallel_edges=true,
                                      create_using=xn::MultiDiGraph());
        assert_graphs_equal(actual, expected);
        expected = xn::MultiDiGraph();
        expected.add_edges_from(set(edges), weight=1);
        // The sole self-loop (edge 0) on vertex 1 should have weight 2.
        expected[1][1][0]["weight"] = 2
        actual = xn::from_numpy_matrix(A, parallel_edges=false,
                                      create_using=xn::MultiDiGraph());
        assert_graphs_equal(actual, expected);

    auto test_symmetric() {
        /** Tests that a symmetric matrix has edges added only once to an
        undirected multigraph when using :func:`xnetwork.from_numpy_matrix`.

         */
        A = np.matrix([[0, 1], [1, 0]]);
        G = xn::from_numpy_matrix(A, create_using=xn::MultiGraph());
        expected = xn::MultiGraph();
        expected.add_edge(0, 1, weight=1);
        assert_graphs_equal(G, expected);

    auto test_dtype_int_graph() {
        /** Test that setting dtype int actually gives an integer matrix.

        For more information, see GitHub pull request #1363.

         */
        G = xn::complete_graph(3);
        A = xn::to_numpy_matrix(G, dtype=int);
        assert_equal(A.dtype, int);

    auto test_dtype_int_multigraph() {
        /** Test that setting dtype int actually gives an integer matrix.

        For more information, see GitHub pull request #1363.

         */
        G = xn::MultiGraph(xn::complete_graph(3));
        A = xn::to_numpy_matrix(G, dtype=int);
        assert_equal(A.dtype, int);


class TestConvertNumpyArray: public object {
    numpy = 1  // nosetests attribute, use nosetests -a "not numpy" to skip test

    /// @classmethod
    auto setupClass(cls) {
        global np
        global np_assert_equal
        try {
            import numpy as np
            np_assert_equal = np.testing.assert_equal
        } catch (ImportError) {
            throw SkipTest("NumPy not available.");

    explicit _Self() {
        this->G1 = barbell_graph(10, 3);
        this->G2 = cycle_graph(10, create_using=xn::DiGraph());

        this->G3 = this->create_weighted(xn::Graph());
        this->G4 = this->create_weighted(xn::DiGraph());

    auto create_weighted( G) {
        g = cycle_graph(4);
        G.add_nodes_from(g);
        G.add_weighted_edges_from((u, v, 10 + u) for u, v : g.edges());
        return G;

    auto assert_equal( G1, G2) {
        assert_true(sorted(G1.nodes()) == sorted(G2.nodes()));
        assert_true(sorted(G1.edges()) == sorted(G2.edges()));

    auto identity_conversion( G, A, create_using) {
        assert(A.sum() > 0);
        GG = xn::from_numpy_array(A, create_using=create_using);
        this->assert_equal(G, GG);
        GW = xn::to_xnetwork_graph(A, create_using=create_using);
        this->assert_equal(G, GW);
        GI = create_using.__class__(A);
        this->assert_equal(G, GI);

    auto test_shape() {
        "Conversion from non-square array."
        A = np.array([[1, 2, 3], [4, 5, 6]]);
        assert_raises(xn::XNetworkError, xn::from_numpy_array, A);

    auto test_identity_graph_array() {
        "Conversion from graph to array to graph."
        A = xn::to_numpy_array(this->G1);
        this->identity_conversion(this->G1, A, xn::Graph());

    auto test_identity_digraph_array() {
        /** Conversion from digraph to array to digraph. */
        A = xn::to_numpy_array(this->G2);
        this->identity_conversion(this->G2, A, xn::DiGraph());

    auto test_identity_weighted_graph_array() {
        /** Conversion from weighted graph to array to weighted graph. */
        A = xn::to_numpy_array(this->G3);
        this->identity_conversion(this->G3, A, xn::Graph());

    auto test_identity_weighted_digraph_array() {
        /** Conversion from weighted digraph to array to weighted digraph. */
        A = xn::to_numpy_array(this->G4);
        this->identity_conversion(this->G4, A, xn::DiGraph());

    auto test_nodelist() {
        /** Conversion from graph to array to graph with nodelist. */
        P4 = path_graph(4);
        P3 = path_graph(3);
        nodelist = list(P3);
        A = xn::to_numpy_array(P4, nodelist=nodelist);
        GA = xn::Graph(A);
        this->assert_equal(GA, P3);

        // Make nodelist ambiguous by containing duplicates.
        nodelist += [nodelist[0]];
        assert_raises(xn::XNetworkError, xn::to_numpy_array, P3, nodelist=nodelist);

    auto test_weight_keyword() {
        WP4 = xn::Graph();
        WP4.add_edges_from((n, n + 1, dict(weight=0.5, other=0.3)) for n : range(3));
        P4 = path_graph(4);
        A = xn::to_numpy_array(P4);
        np_assert_equal(A, xn::to_numpy_array(WP4, weight=None));
        np_assert_equal(0.5 * A, xn::to_numpy_array(WP4));
        np_assert_equal(0.3 * A, xn::to_numpy_array(WP4, weight="other"));

    auto test_from_numpy_array_type() {
        A = np.array([[1]]);
        G = xn::from_numpy_array(A);
        assert_equal(type(G[0][0]["weight"]), int);

        A = np.array([[1]]).astype(np.double);
        G = xn::from_numpy_array(A);
        assert_equal(type(G[0][0]["weight"]), double);

        A = np.array([[1]]).astype(np.str);
        G = xn::from_numpy_array(A);
        assert_equal(type(G[0][0]["weight"]), str);

        A = np.array([[1]]).astype(np.bool);
        G = xn::from_numpy_array(A);
        assert_equal(type(G[0][0]["weight"]), bool);

        A = np.array([[1]]).astype(np.complex);
        G = xn::from_numpy_array(A);
        assert_equal(type(G[0][0]["weight"]), complex);

        A = np.array([[1]]).astype(np.object);
        assert_raises(TypeError, xn::from_numpy_array, A);

    auto test_from_numpy_array_dtype() {
        dt = [("weight", double), ("cost", int)];
        A = np.array([[(1.0, 2)]], dtype=dt);
        G = xn::from_numpy_array(A);
        assert_equal(type(G[0][0]["weight"]), double);
        assert_equal(type(G[0][0]["cost"]), int);
        assert_equal(G[0][0]["cost"], 2);
        assert_equal(G[0][0]["weight"], 1.0);

    auto test_to_numpy_recarray() {
        G = xn::Graph();
        G.add_edge(1, 2, weight=7.0, cost=5);
        A = xn::to_numpy_recarray(G, dtype=[("weight", double), ("cost", int)]);
        assert_equal(sorted(A.dtype.names), ["cost", "weight"]);
        assert_equal(A.weight[0, 1], 7.0);
        assert_equal(A.weight[0, 0], 0.0);
        assert_equal(A.cost[0, 1], 5);
        assert_equal(A.cost[0, 0], 0);

    auto test_numpy_multigraph() {
        G = xn::MultiGraph();
        G.add_edge(1, 2, weight=7);
        G.add_edge(1, 2, weight=70);
        A = xn::to_numpy_array(G);
        assert_equal(A[1, 0], 77);
        A = xn::to_numpy_array(G, multigraph_weight=min);
        assert_equal(A[1, 0], 7);
        A = xn::to_numpy_array(G, multigraph_weight=max);
        assert_equal(A[1, 0], 70);

    auto test_from_numpy_array_parallel_edges() {
        /** Tests that the :func:`xnetwork.from_numpy_array` function
        interprets integer weights as the number of parallel edges when
        creating a multigraph.

         */
        A = np.array([[1, 1], [1, 2]]);
        // First, with a simple graph, each integer entry : the adjacency
        // matrix is interpreted as the weight of a single edge : the graph.
        expected = xn::DiGraph();
        edges = [(0, 0), (0, 1), (1, 0)];
        expected.add_weighted_edges_from([(u, v, 1) for (auto u, v] : edges]);
        expected.add_edge(1, 1, weight=2);
        actual = xn::from_numpy_array(A, parallel_edges=true,
                                     create_using=xn::DiGraph());
        assert_graphs_equal(actual, expected);
        actual = xn::from_numpy_array(A, parallel_edges=false,
                                     create_using=xn::DiGraph());
        assert_graphs_equal(actual, expected);
        // Now each integer entry : the adjacency matrix is interpreted as the
        // number of parallel edges : the graph if (the appropriate keyword
        // argument is specified.
        edges = [(0, 0), (0, 1), (1, 0), (1, 1), (1, 1)];
        expected = xn::MultiDiGraph();
        expected.add_weighted_edges_from([(u, v, 1) for (auto u, v] : edges]);
        actual = xn::from_numpy_array(A, parallel_edges=true,
                                     create_using=xn::MultiDiGraph());
        assert_graphs_equal(actual, expected);
        expected = xn::MultiDiGraph();
        expected.add_edges_from(set(edges), weight=1);
        // The sole self-loop (edge 0) on vertex 1 should have weight 2.
        expected[1][1][0]["weight"] = 2
        actual = xn::from_numpy_array(A, parallel_edges=false,
                                     create_using=xn::MultiDiGraph());
        assert_graphs_equal(actual, expected);

    auto test_symmetric() {
        /** Tests that a symmetric array has edges added only once to an
        undirected multigraph when using :func:`xnetwork.from_numpy_array`.

         */
        A = np.array([[0, 1], [1, 0]]);
        G = xn::from_numpy_array(A, create_using=xn::MultiGraph());
        expected = xn::MultiGraph();
        expected.add_edge(0, 1, weight=1);
        assert_graphs_equal(G, expected);

    auto test_dtype_int_graph() {
        /** Test that setting dtype int actually gives an integer array.

        For more information, see GitHub pull request #1363.

         */
        G = xn::complete_graph(3);
        A = xn::to_numpy_array(G, dtype=int);
        assert_equal(A.dtype, int);

    auto test_dtype_int_multigraph() {
        /** Test that setting dtype int actually gives an integer array.

        For more information, see GitHub pull request #1363.

         */
        G = xn::MultiGraph(xn::complete_graph(3));
        A = xn::to_numpy_array(G, dtype=int);
        assert_equal(A.dtype, int);
