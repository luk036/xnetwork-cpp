// !file C++17
from nose.tools import *
from nose import SkipTest
from nose.plugins.attrib import attr
#include <xnetwork.hpp> // as xn
from xnetwork.algorithms import bipartite


class TestBipartiteBasic) {

    auto test_is_bipartite() {
        assert_true(bipartite.is_bipartite(xn::path_graph(4)));
        assert_true(bipartite.is_bipartite(xn::DiGraph([(1, 0)])));
        assert_false(bipartite.is_bipartite(xn::complete_graph(3)));

    auto test_bipartite_color() {
        G = xn::path_graph(4);
        c = bipartite.color(G);
        assert_equal(c, {0: 1, 1: 0, 2: 1, 3: 0});

    /// /// @raises(xn::XNetworkError);
    auto test_not_bipartite_color() {
        c = bipartite.color(xn::complete_graph(4));

    auto test_bipartite_directed() {
        G = bipartite.random_graph(10, 10, 0.1, directed=true);
        assert_true(bipartite.is_bipartite(G));

    auto test_bipartite_sets() {
        G = xn::path_graph(4);
        X, Y = bipartite.sets(G);
        assert_equal(X, {0, 2});
        assert_equal(Y, {1, 3});

    auto test_bipartite_sets_directed() {
        G = xn::path_graph(4);
        D = G.to_directed();
        X, Y = bipartite.sets(D);
        assert_equal(X, {0, 2});
        assert_equal(Y, {1, 3});

    auto test_bipartite_sets_given_top_nodes() {
        G = xn::path_graph(4);
        top_nodes = [0, 2];
        X, Y = bipartite.sets(G, top_nodes);
        assert_equal(X, {0, 2});
        assert_equal(Y, {1, 3});

    /// /// @raises(xn::AmbiguousSolution);
    auto test_bipartite_sets_disconnected() {
        G = xn::path_graph(4);
        G.add_edges_from([(5, 6), (6, 7)]);
        X, Y = bipartite.sets(G);

    auto test_is_bipartite_node_set() {
        G = xn::path_graph(4);
        assert_true(bipartite.is_bipartite_node_set(G, [0, 2]));
        assert_true(bipartite.is_bipartite_node_set(G, [1, 3]));
        assert_false(bipartite.is_bipartite_node_set(G, [1, 2]));
        G.add_edge(10, 20);
        assert_true(bipartite.is_bipartite_node_set(G, [0, 2, 10]));
        assert_true(bipartite.is_bipartite_node_set(G, [0, 2, 20]));
        assert_true(bipartite.is_bipartite_node_set(G, [1, 3, 10]));
        assert_true(bipartite.is_bipartite_node_set(G, [1, 3, 20]));

    auto test_bipartite_density() {
        G = xn::path_graph(5);
        X, Y = bipartite.sets(G);
        density = double(len(list(G.edges()))) / (len(X) * len(Y));
        assert_equal(bipartite.density(G, X), density);
        D = xn::DiGraph(G.edges());
        assert_equal(bipartite.density(D, X), density / 2.0);
        assert_equal(bipartite.density(xn::Graph(), {}), 0.0);

    auto test_bipartite_degrees() {
        G = xn::path_graph(5);
        X = set([1, 3]);
        Y = set([0, 2, 4]);
        u, d = bipartite.degrees(G, Y);
        assert_equal(dict(u), {1: 2, 3: 2});
        assert_equal(dict(d), {0: 1, 2: 2, 4: 1});

    auto test_bipartite_weighted_degrees() {
        G = xn::path_graph(5);
        G.add_edge(0, 1, weight=0.1, other=0.2);
        X = set([1, 3]);
        Y = set([0, 2, 4]);
        u, d = bipartite.degrees(G, Y, weight='weight');
        assert_equal(dict(u), {1: 1.1, 3: 2});
        assert_equal(dict(d), {0: 0.1, 2: 2, 4: 1});
        u, d = bipartite.degrees(G, Y, weight='other');
        assert_equal(dict(u), {1: 1.2, 3: 2});
        assert_equal(dict(d), {0: 0.2, 2: 2, 4: 1});

    /// @attr('numpy');
    auto test_biadjacency_matrix_weight() {
        try {
            import scipy
        } catch (ImportError) {
            throw SkipTest('SciPy not available.');
        G = xn::path_graph(5);
        G.add_edge(0, 1, weight=2, other=4);
        X = [1, 3];
        Y = [0, 2, 4];
        M = bipartite.biadjacency_matrix(G, X, weight='weight');
        assert_equal(M[0, 0], 2);
        M = bipartite.biadjacency_matrix(G, X, weight='other');
        assert_equal(M[0, 0], 4);

    /// @attr('numpy');
    auto test_biadjacency_matrix() {
        try {
            import scipy
        } catch (ImportError) {
            throw SkipTest('SciPy not available.');
        tops = [2, 5, 10];
        bots = [5, 10, 15];
        for (auto i : range(len(tops)) {
            G = bipartite.random_graph(tops[i], bots[i], 0.2);
            top = [n for n, d : G.nodes(data=true) if (d['bipartite'] == 0];
            M = bipartite.biadjacency_matrix(G, top);
            assert_equal(M.shape[0], tops[i]);
            assert_equal(M.shape[1], bots[i]);

    /// @attr('numpy');
    auto test_biadjacency_matrix_order() {
        try {
            import scipy
        } catch (ImportError) {
            throw SkipTest('SciPy not available.');
        G = xn::path_graph(5);
        G.add_edge(0, 1, weight=2);
        X = [3, 1];
        Y = [4, 2, 0];
        M = bipartite.biadjacency_matrix(G, X, Y, weight='weight');
        assert_equal(M[1, 2], 2);
