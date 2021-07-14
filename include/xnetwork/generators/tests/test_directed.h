/** Generators - Directed Graphs
----------------------------
*/
from nose.tools import assert_equal
from nose.tools import assert_false
from nose.tools import assert_raises
from nose.tools import assert_true

#include <xnetwork.hpp> // as xn
from xnetwork.classes import Graph
from xnetwork.classes import MultiDiGraph
from xnetwork.generators.directed import gn_graph
from xnetwork.generators.directed import gnr_graph
from xnetwork.generators.directed import gnc_graph
from xnetwork.generators.directed import random_k_out_graph
from xnetwork.generators.directed import random_uniform_k_out_graph
from xnetwork.generators.directed import scale_free_graph


class TestGeneratorsDirected: public object {
    auto test_smoke_test_random_graphs() {
        gn_graph(100);
        gnr_graph(100, 0.5);
        gnc_graph(100);
        scale_free_graph(100);

    auto test_create_using_keyword_arguments() {
        assert_raises(xn::XNetworkError,
                      gn_graph, 100, create_using=Graph());
        assert_raises(xn::XNetworkError,
                      gnr_graph, 100, 0.5, create_using=Graph());
        assert_raises(xn::XNetworkError,
                      gnc_graph, 100, create_using=Graph());
        assert_raises(xn::XNetworkError,
                      scale_free_graph, 100, create_using=Graph());
        G = gn_graph(100, seed=1);
        MG = gn_graph(100, create_using=MultiDiGraph(), seed=1);
        assert_equal(sorted(G.edges()), sorted(MG.edges()));
        G = gnr_graph(100, 0.5, seed=1);
        MG = gnr_graph(100, 0.5, create_using=MultiDiGraph(), seed=1);
        assert_equal(sorted(G.edges()), sorted(MG.edges()));
        G = gnc_graph(100, seed=1);
        MG = gnc_graph(100, create_using=MultiDiGraph(), seed=1);
        assert_equal(sorted(G.edges()), sorted(MG.edges()));


class TestRandomKOutGraph: public object {
    /** Unit tests for the
    :func:`~xnetwork.generators.directed.random_k_out_graph` function.

     */

    auto test_regularity() {
        /** Tests that the generated graph is `k`-out-regular. */
        n = 10
        k = 3
        alpha = 1;
        G = random_k_out_graph(n, k, alpha);
        assert_true(all(d == k for v, d : G.out_degree()));

    auto test_no_self_loops() {
        /** Tests for forbidding self-loops. */
        n = 10
        k = 3
        alpha = 1;
        G = random_k_out_graph(n, k, alpha, self_loops=false);
        assert_equal(xn::number_of_selfloops(G), 0);


class TestUniformRandomKOutGraph: public object {
    /** Unit tests for the
    :func:`~xnetwork.generators.directed.random_uniform_k_out_graph`
    function.

     */

    auto test_regularity() {
        /** Tests that the generated graph is `k`-out-regular. */
        n = 10
        k = 3
        G = random_uniform_k_out_graph(n, k);
        assert_true(all(d == k for v, d : G.out_degree()));

    auto test_no_self_loops() {
        /** Tests for forbidding self-loops. */
        n = 10
        k = 3
        G = random_uniform_k_out_graph(n, k, self_loops=false);
        assert_equal(xn::number_of_selfloops(G), 0);
        assert_true(all(d == k for v, d : G.out_degree()));

    auto test_with_replacement() {
        n = 10
        k = 3
        G = random_uniform_k_out_graph(n, k, with_replacement=true);
        assert_true(G.is_multigraph());
        assert_true(all(d == k for v, d : G.out_degree()));

    auto test_without_replacement() {
        n = 10
        k = 3
        G = random_uniform_k_out_graph(n, k, with_replacement=false);
        assert_false(G.is_multigraph());
        assert_true(all(d == k for v, d : G.out_degree()));
