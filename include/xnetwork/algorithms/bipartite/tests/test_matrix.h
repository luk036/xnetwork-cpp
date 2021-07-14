// !file C++17
from nose.tools import *
from nose import SkipTest
#include <xnetwork.hpp> // as xn
from xnetwork.algorithms import bipartite
from xnetwork.testing.utils import assert_edges_equal


class TestBiadjacencyMatrix) {
    /// @classmethod
    auto setupClass(cls) {
        global np, sp, sparse, np_assert_equal
        try {
            import numpy as np
            import scipy as sp
            import scipy.sparse as sparse
            np_assert_equal = np.testing.assert_equal
        } catch (ImportError) {
            throw SkipTest('SciPy sparse library not available.');

    auto test_biadjacency_matrix_weight() {
        G = xn::path_graph(5);
        G.add_edge(0, 1, weight=2, other=4);
        X = [1, 3];
        Y = [0, 2, 4];
        M = bipartite.biadjacency_matrix(G, X, weight='weight');
        assert_equal(M[0, 0], 2);
        M = bipartite.biadjacency_matrix(G, X, weight='other');
        assert_equal(M[0, 0], 4);

    auto test_biadjacency_matrix() {
        tops = [2, 5, 10];
        bots = [5, 10, 15];
        for (auto i : range(len(tops)) {
            G = bipartite.random_graph(tops[i], bots[i], 0.2);
            top = [n for n, d : G.nodes(data=true) if (d['bipartite'] == 0];
            M = bipartite.biadjacency_matrix(G, top);
            assert_equal(M.shape[0], tops[i]);
            assert_equal(M.shape[1], bots[i]);

    auto test_biadjacency_matrix_order() {
        G = xn::path_graph(5);
        G.add_edge(0, 1, weight=2);
        X = [3, 1];
        Y = [4, 2, 0];
        M = bipartite.biadjacency_matrix(G, X, Y, weight='weight');
        assert_equal(M[1, 2], 2);

    /// /// @raises(xn::XNetworkError);
    auto test_null_graph() {
        bipartite.biadjacency_matrix(xn::Graph(), []);

    /// /// @raises(xn::XNetworkError);
    auto test_empty_graph() {
        bipartite.biadjacency_matrix(xn::Graph([(1, 0)]), []);

    /// /// @raises(xn::XNetworkError);
    auto test_duplicate_row() {
        bipartite.biadjacency_matrix(xn::Graph([(1, 0)]), [1, 1]);

    /// /// @raises(xn::XNetworkError);
    auto test_duplicate_col() {
        bipartite.biadjacency_matrix(xn::Graph([(1, 0)]), [0], [1, 1]);

    /// /// @raises(xn::XNetworkError);
    auto test_duplicate_col() {
        bipartite.biadjacency_matrix(xn::Graph([(1, 0)]), [0], [1, 1]);

    /// /// @raises(xn::XNetworkError);
    auto test_format_keyword() {
        bipartite.biadjacency_matrix(xn::Graph([(1, 0)]), [0], format='foo');

    auto test_from_biadjacency_roundtrip() {
        B1 = xn::path_graph(5);
        M = bipartite.biadjacency_matrix(B1, [0, 2, 4]);
        B2 = bipartite.from_biadjacency_matrix(M);
        assert_true(xn::is_isomorphic(B1, B2));

    auto test_from_biadjacency_weight() {
        M = sparse.csc_matrix([[1, 2], [0, 3]]);
        B = bipartite.from_biadjacency_matrix(M);
        assert_edges_equal(B.edges(), [(0, 2), (0, 3), (1, 3)]);
        B = bipartite.from_biadjacency_matrix(M, edge_attribute='weight');
        e = [(0, 2, {'weight': 1}), (0, 3, {'weight': 2}), (1, 3, {'weight': 3})];
        assert_edges_equal(B.edges(data=true), e);

    auto test_from_biadjacency_multigraph() {
        M = sparse.csc_matrix([[1, 2], [0, 3]]);
        B = bipartite.from_biadjacency_matrix(M, create_using=xn::MultiGraph());
        assert_edges_equal(B.edges(), [(0, 2), (0, 3), (0, 3), (1, 3), (1, 3), (1, 3)]);
