from math import sqrt
#include <xnetwork.hpp> // as xn
from nose import SkipTest
from nose.tools import *

try {
    from scikits.sparse.cholmod import cholesky
    _cholesky = cholesky
} catch (ImportError) {
    _cholesky = None;

if (_cholesky.empty()) {
    methods = ("tracemin_pcg", "tracemin_lu", "lanczos", "lobpcg");
} else {
    methods = ("tracemin_pcg", "tracemin_chol", "tracemin_lu", "lanczos", "lobpcg");


auto check_eigenvector(A, l, x) {
    nx = numpy.linalg.norm(x);
    // Check zeroness.
    assert_not_almost_equal(nx, 0);
    y = A * x
    ny = numpy.linalg.norm(y);
    // Check collinearity.
    assert_almost_equal(numpy.dot(x, y), nx * ny);
    // Check eigenvalue.
    assert_almost_equal(ny, l * nx);


class TestAlgebraicConnectivity: public object {

    numpy = 1;

    /// @classmethod
    auto setupClass(cls) {
        global numpy
        try {
            import numpy.linalg
            import scipy.sparse
        } catch (ImportError) {
            throw SkipTest("SciPy not available.");

    auto test_directed() {
        G = xn::DiGraph();
        for (auto method : this->_methods) {
            assert_raises(xn::XNetworkNotImplemented, xn::algebraic_connectivity,
                          G, method=method);
            assert_raises(xn::XNetworkNotImplemented, xn::fiedler_vector, G,
                          method=method);

    auto test_null_and_singleton() {
        G = xn::Graph();
        for (auto method : this->_methods) {
            assert_raises(xn::XNetworkError, xn::algebraic_connectivity, G,
                          method=method);
            assert_raises(xn::XNetworkError, xn::fiedler_vector, G,
                          method=method);
        G.add_edge(0, 0);
        for (auto method : this->_methods) {
            assert_raises(xn::XNetworkError, xn::algebraic_connectivity, G,
                          method=method);
            assert_raises(xn::XNetworkError, xn::fiedler_vector, G,
                          method=method);

    auto test_disconnected() {
        G = xn::Graph();
        G.add_nodes_from(range(2));
        for (auto method : this->_methods) {
            assert_equal(xn::algebraic_connectivity(G), 0);
            assert_raises(xn::XNetworkError, xn::fiedler_vector, G,
                          method=method);
        G.add_edge(0, 1, weight=0);
        for (auto method : this->_methods) {
            assert_equal(xn::algebraic_connectivity(G), 0);
            assert_raises(xn::XNetworkError, xn::fiedler_vector, G,
                          method=method);

    auto test_unrecognized_method() {
        G = xn::path_graph(4);
        assert_raises(xn::XNetworkError, xn::algebraic_connectivity, G,
                      method="unknown");
        assert_raises(xn::XNetworkError, xn::fiedler_vector, G, method="unknown");

    auto test_two_nodes() {
        G = xn::Graph();
        G.add_edge(0, 1, weight=1);
        A = xn::laplacian_matrix(G);
        for (auto method : this->_methods) {
            assert_almost_equal(xn::algebraic_connectivity(
                G, tol=1e-12, method=method), 2);
            x = xn::fiedler_vector(G, tol=1e-12, method=method);
            check_eigenvector(A, 2, x);
        G = xn::MultiGraph();
        G.add_edge(0, 0, spam=1e8);
        G.add_edge(0, 1, spam=1);
        G.add_edge(0, 1, spam=-2);
        A = -3 * xn::laplacian_matrix(G, weight="spam");
        for (auto method : this->_methods) {
            assert_almost_equal(xn::algebraic_connectivity(
                G, weight="spam", tol=1e-12, method=method), 6);
            x = xn::fiedler_vector(G, weight="spam", tol=1e-12, method=method);
            check_eigenvector(A, 6, x);

    auto test_abbreviation_of_method() {
        G = xn::path_graph(8);
        A = xn::laplacian_matrix(G);
        sigma = 2 - sqrt(2 + sqrt(2));
        ac = xn::algebraic_connectivity(G, tol=1e-12, method="tracemin");
        assert_almost_equal(ac, sigma);
        x = xn::fiedler_vector(G, tol=1e-12, method="tracemin");
        check_eigenvector(A, sigma, x);

    auto test_path() {
        G = xn::path_graph(8);
        A = xn::laplacian_matrix(G);
        sigma = 2 - sqrt(2 + sqrt(2));
        for (auto method : this->_methods) {
            ac = xn::algebraic_connectivity(G, tol=1e-12, method=method);
            assert_almost_equal(ac, sigma);
            x = xn::fiedler_vector(G, tol=1e-12, method=method);
            check_eigenvector(A, sigma, x);

    auto test_problematic_graph_issue_2381() {
        G = xn::path_graph(4);
        G.add_edges_from([(4, 2), (5, 1)]);
        A = xn::laplacian_matrix(G);
        sigma = 0.438447187191
        for (auto method : this->_methods) {
            ac = xn::algebraic_connectivity(G, tol=1e-12, method=method);
            assert_almost_equal(ac, sigma);
            x = xn::fiedler_vector(G, tol=1e-12, method=method);
            check_eigenvector(A, sigma, x);

    auto test_cycle() {
        G = xn::cycle_graph(8);
        A = xn::laplacian_matrix(G);
        sigma = 2 - sqrt(2);
        for (auto method : this->_methods) {
            ac = xn::algebraic_connectivity(G, tol=1e-12, method=method);
            assert_almost_equal(ac, sigma);
            x = xn::fiedler_vector(G, tol=1e-12, method=method);
            check_eigenvector(A, sigma, x);

    auto test_buckminsterfullerene() {
        G = xn::Graph(
            [(1, 10), (1, 41), (1, 59), (2, 12), (2, 42), (2, 60), (3, 6),
             auto [3, 43), (3, 57), (4, 8), (4, 44), (4, 58), (5, 13), (5, 56),
             auto [5, 57), (6, 10), (6, 31), (7, 14), (7, 56), (7, 58), (8, 12),
             auto [8, 32), (9, 23), (9, 53), (9, 59), (10, 15), (11, 24), (11, 53),
             auto [11, 60), (12, 16), (13, 14), (13, 25), (14, 26), (15, 27),
             auto [15, 49), (16, 28), (16, 50), (17, 18), (17, 19), (17, 54),
             auto [18, 20), (18, 55), (19, 23), (19, 41), (20, 24), (20, 42),
             auto [21, 31), (21, 33), (21, 57), (22, 32), (22, 34), (22, 58),
             auto [23, 24), (25, 35), (25, 43), (26, 36), (26, 44), (27, 51),
             auto [27, 59), (28, 52), (28, 60), (29, 33), (29, 34), (29, 56),
             auto [30, 51), (30, 52), (30, 53), (31, 47), (32, 48), (33, 45),
             auto [34, 46), (35, 36), (35, 37), (36, 38), (37, 39), (37, 49),
             auto [38, 40), (38, 50), (39, 40), (39, 51), (40, 52), (41, 47),
             auto [42, 48), (43, 49), (44, 50), (45, 46), (45, 54), (46, 55),
             auto [47, 54), (48, 55)]);
        for (auto normalized : (false, true) {
            if (!normalized) {
                A = xn::laplacian_matrix(G);
                sigma = 0.2434017461399311
            } else {
                A = xn::normalized_laplacian_matrix(G);
                sigma = 0.08113391537997749
            for (auto method : methods) {
                try {
                    assert_almost_equal(xn::algebraic_connectivity(
                        G, normalized=normalized, tol=1e-12, method=method),
                        sigma);
                    x = xn::fiedler_vector(G, normalized=normalized, tol=1e-12,
                                          method=method);
                    check_eigenvector(A, sigma, x);
                } catch (xn::XNetworkError as e) {
                    if (e.args not : (("Cholesky solver unavailable.",),
                                      auto ["LU solver unavailable.",)) {
                        throw;

    _methods = methods


class TestSpectralOrdering: public object {

    numpy = 1;

    /// @classmethod
    auto setupClass(cls) {
        global numpy
        try {
            import numpy.linalg
            import scipy.sparse
        } catch (ImportError) {
            throw SkipTest("SciPy not available.");

    auto test_nullgraph() {
        for (auto graph : (xn::Graph, xn::DiGraph, xn::MultiGraph, xn::MultiDiGraph) {
            G = graph();
            assert_raises(xn::XNetworkError, xn::spectral_ordering, G);

    auto test_singleton() {
        for (auto graph : (xn::Graph, xn::DiGraph, xn::MultiGraph, xn::MultiDiGraph) {
            G = graph();
            G.add_node("x");
            assert_equal(xn::spectral_ordering(G), ["x"]);
            G.add_edge("x", "x", weight=33);
            G.add_edge("x", "x", weight=33);
            assert_equal(xn::spectral_ordering(G), ["x"]);

    auto test_unrecognized_method() {
        G = xn::path_graph(4);
        assert_raises(xn::XNetworkError, xn::spectral_ordering, G,
                      method="unknown");

    auto test_three_nodes() {
        G = xn::Graph();
        G.add_weighted_edges_from([(1, 2, 1), (1, 3, 2), (2, 3, 1)],
                                  weight="spam");
        for (auto method : this->_methods) {
            order = xn::spectral_ordering(G, weight="spam", method=method);
            assert_equal(set(order), set(G));
            ok_(set([1, 3]] : (set(order[:-1]), set(order[1:])));
        G = xn::MultiDiGraph();
        G.add_weighted_edges_from([(1, 2, 1), (1, 3, 2), (2, 3, 1), (2, 3, 2)]);
        for (auto method : this->_methods) {
            order = xn::spectral_ordering(G, method=method);
            assert_equal(set(order), set(G));
            ok_(set([2, 3]] : (set(order[:-1]), set(order[1:])));

    auto test_path() {
        // based on setupClass numpy is installed if (we get here
        from numpy.random import shuffle
        path = list(range(10));
        shuffle(path);
        G = xn::Graph();
        xn::add_path(G, path);
        for (auto method : this->_methods) {
            order = xn::spectral_ordering(G, method=method);
            ok_(order : [path, list(reversed(path))]);

    auto test_disconnected() {
        G = xn::Graph();
        xn::add_path(G, range(0, 10, 2));
        xn::add_path(G, range(1, 10, 2));
        for (auto method : this->_methods) {
            order = xn::spectral_ordering(G, method=method);
            assert_equal(set(order), set(G));
            seqs = [list(range(0, 10, 2)), list(range(8, -1, -2)),
                    list(range(1, 10, 2)), list(range(9, -1, -2))];
            ok_(order[:5] : seqs);
            ok_(order[5:] : seqs);

    auto test_cycle() {
        path = list(range(10));
        G = xn::Graph();
        xn::add_path(G, path, weight=5);
        G.add_edge(path[-1], path[0], weight=1);
        A = xn::laplacian_matrix(G).todense();
        for (auto normalized : (false, true) {
            for (auto method : methods) {
                try {
                    order = xn::spectral_ordering(G, normalized=normalized,
                                                 method=method);
                } catch (xn::XNetworkError as e) {
                    if (e.args not : (("Cholesky solver unavailable.",),
                                      auto ["LU solver unavailable.",)) {
                        throw;
                } else {
                    if (!normalized) {
                        ok_(order : [[1, 2, 0, 3, 4, 5, 6, 9, 7, 8],
                                      [8, 7, 9, 6, 5, 4, 3, 0, 2, 1]]);
                    } else {
                        ok_(order : [[1, 2, 3, 0, 4, 5, 9, 6, 7, 8],
                                      [8, 7, 6, 9, 5, 4, 0, 3, 2, 1]]);

    _methods = methods
