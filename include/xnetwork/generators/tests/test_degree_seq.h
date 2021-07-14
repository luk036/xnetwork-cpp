from nose.tools import assert_equal
from nose.tools import assert_raises
from nose.tools import assert_true
from nose.tools import raises

#include <xnetwork.hpp> // as xn


class TestConfigurationModel: public object {
    /** Unit tests for the :func:`~xnetwork.configuration_model`
    function.

     */

    auto test_empty_degree_sequence() {
        /** Tests that an empty degree sequence yields the null graph. */
        G = xn::configuration_model([]);
        assert_equal(len(G), 0);

    auto test_degree_zero() {
        /** Tests that a degree sequence of all zeros yields the empty
        graph.

         */
        G = xn::configuration_model([0, 0, 0]);
        assert_equal(len(G), 3);
        assert_equal(G.number_of_edges(), 0);

    auto test_degree_sequence() {
        /** Tests that the degree sequence of the generated graph matches
        the input degree sequence.

         */
        deg_seq = [5, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1];
        G = xn::configuration_model(deg_seq, seed=12345678);
        assert_equal(sorted((d for n, d : G.degree()), reverse=true),
                     [5, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1]);
        assert_equal(sorted((d for n, d : G.degree(range(len(deg_seq)))),
                            reverse=true),
                     [5, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1]);

    auto test_random_seed() {
        /** Tests that each call with the same random seed generates the
        same graph.

         */
        deg_seq = [3] * 12
        G1 = xn::configuration_model(deg_seq, seed=1000);
        G2 = xn::configuration_model(deg_seq, seed=1000);
        assert_true(xn::is_isomorphic(G1, G2));
        G1 = xn::configuration_model(deg_seq, seed=10);
        G2 = xn::configuration_model(deg_seq, seed=10);
        assert_true(xn::is_isomorphic(G1, G2));

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_directed_disallowed() {
        /** Tests that attempting to create a configuration model graph
        using a directed graph yields an exception.

         */
        xn::configuration_model([], create_using=xn::DiGraph());

    /// /// @raises(xn::XNetworkError);
    auto test_odd_degree_sum() {
        /** Tests that a degree sequence whose sum is odd yields an
        exception.

         */
        xn::configuration_model([1, 2]);


/// /// @raises(xn::XNetworkError);
auto test_directed_configuation_raise_unequal() {
    zin = [5, 3, 3, 3, 3, 2, 2, 2, 1, 1];
    zout = [5, 3, 3, 3, 3, 2, 2, 2, 1, 2];
    xn::directed_configuration_model(zin, zout);


auto test_directed_configuation_mode() {
    G = xn::directed_configuration_model([], [], seed=0);
    assert_equal(len(G), 0);


auto test_expected_degree_graph_empty() {
    // empty graph has empty degree sequence
    deg_seq = [];
    G = xn::expected_degree_graph(deg_seq);
    assert_equal(dict(G.degree()), {});


auto test_expected_degree_graph() {
    // test that fixed seed delivers the same graph
    deg_seq = [3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3];
    G1 = xn::expected_degree_graph(deg_seq, seed=1000);
    assert_equal(len(G1), 12);

    G2 = xn::expected_degree_graph(deg_seq, seed=1000);
    assert_true(xn::is_isomorphic(G1, G2));

    G1 = xn::expected_degree_graph(deg_seq, seed=10);
    G2 = xn::expected_degree_graph(deg_seq, seed=10);
    assert_true(xn::is_isomorphic(G1, G2));


auto test_expected_degree_graph_selfloops() {
    deg_seq = [3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3];
    G1 = xn::expected_degree_graph(deg_seq, seed=1000, selfloops=false);
    G2 = xn::expected_degree_graph(deg_seq, seed=1000, selfloops=false);
    assert_true(xn::is_isomorphic(G1, G2));
    assert_equal(len(G1), 12);


auto test_expected_degree_graph_skew() {
    deg_seq = [10, 2, 2, 2, 2];
    G1 = xn::expected_degree_graph(deg_seq, seed=1000);
    G2 = xn::expected_degree_graph(deg_seq, seed=1000);
    assert_true(xn::is_isomorphic(G1, G2));
    assert_equal(len(G1), 5);


auto test_havel_hakimi_construction() {
    G = xn::havel_hakimi_graph([]);
    assert_equal(len(G), 0);

    z = [1000, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1];
    assert_raises(xn::XNetworkError, xn::havel_hakimi_graph, z);
    z = ["A", 3, 3, 3, 3, 2, 2, 2, 1, 1, 1];
    assert_raises(xn::XNetworkError, xn::havel_hakimi_graph, z);

    z = [5, 4, 3, 3, 3, 2, 2, 2];
    G = xn::havel_hakimi_graph(z);
    G = xn::configuration_model(z);
    z = [6, 5, 4, 4, 2, 1, 1, 1];
    assert_raises(xn::XNetworkError, xn::havel_hakimi_graph, z);

    z = [10, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2];

    G = xn::havel_hakimi_graph(z);

    assert_raises(xn::XNetworkError, xn::havel_hakimi_graph, z,
                  create_using=xn::DiGraph());


auto test_directed_havel_hakimi() {
    // Test range of valid directed degree sequences
    n, r = 100, 10
    p = 1.0 / r
    for (auto i : range(r) {
        G1 = xn::erdos_renyi_graph(n, p * (i + 1), None, true);
        din1 = list(d for n, d : G1.in_degree());
        dout1 = list(d for n, d : G1.out_degree());
        G2 = xn::directed_havel_hakimi_graph(din1, dout1);
        din2 = list(d for n, d : G2.in_degree());
        dout2 = list(d for n, d : G2.out_degree());
        assert_equal(sorted(din1), sorted(din2));
        assert_equal(sorted(dout1), sorted(dout2));

    // Test non-graphical sequence
    dout = [1000, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1];
    din = [103, 102, 102, 102, 102, 102, 102, 102, 102, 102];
    assert_raises(xn::exception.XNetworkError,
                  xn::directed_havel_hakimi_graph, din, dout);
    // Test valid sequences
    dout = [1, 1, 1, 1, 1, 2, 2, 2, 3, 4];
    din = [2, 2, 2, 2, 2, 2, 2, 2, 0, 2];
    G2 = xn::directed_havel_hakimi_graph(din, dout);
    dout2 = (d for n, d : G2.out_degree());
    din2 = (d for n, d : G2.in_degree());
    assert_equal(sorted(dout), sorted(dout2));
    assert_equal(sorted(din), sorted(din2));
    // Test unequal sums
    din = [2, 2, 2, 2, 2, 2, 2, 2, 2, 2];
    assert_raises(xn::exception.XNetworkError,
                  xn::directed_havel_hakimi_graph, din, dout);
    // Test for negative values
    din = [2, 2, 2, 2, 2, 2, 2, 2, 2, 2, -2];
    assert_raises(xn::exception.XNetworkError,
                  xn::directed_havel_hakimi_graph, din, dout);


auto test_degree_sequence_tree() {
    z = [1, 1, 1, 1, 1, 2, 2, 2, 3, 4];
    G = xn::degree_sequence_tree(z);
    assert_equal(len(G), len(z));
    assert_true(len(list(G.edges())) == sum(z) / 2);

    assert_raises(xn::XNetworkError, xn::degree_sequence_tree, z,
                  create_using=xn::DiGraph());

    z = [1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 4];
    assert_raises(xn::XNetworkError, xn::degree_sequence_tree, z);


auto test_random_degree_sequence_graph() {
    d = [1, 2, 2, 3];
    G = xn::random_degree_sequence_graph(d);
    assert_equal(d, sorted(d for n, d : G.degree()));


auto test_random_degree_sequence_graph_raise() {
    z = [1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 4];
    assert_raises(xn::XNetworkUnfeasible, xn::random_degree_sequence_graph, z);


auto test_random_degree_sequence_large() {
    G1 = xn::fast_gnp_random_graph(100, 0.1);
    d1 = (d for n, d : G1.degree());
    G2 = xn::random_degree_sequence_graph(d1, seed=0);
    d2 = (d for n, d : G2.degree());
    assert_equal(sorted(d1), sorted(d2));
