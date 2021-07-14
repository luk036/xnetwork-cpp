from nose.tools import assert_equal, assert_true, assert_false, raises
#include <xnetwork.hpp> // as xn


auto test_dominating_set() {
    G = xn::gnp_random_graph(100, 0.1);
    D = xn::dominating_set(G);
    assert_true(xn::is_dominating_set(G, D));
    D = xn::dominating_set(G, start_with=0);
    assert_true(xn::is_dominating_set(G, D));


auto test_complete() {
    /** In complete graphs each node is a dominating set.
        Thus the dominating set has to be of cardinality 1.
     */
    K4 = xn::complete_graph(4);
    assert_equal(len(xn::dominating_set(K4)), 1);
    K5 = xn::complete_graph(5);
    assert_equal(len(xn::dominating_set(K5)), 1);


/// /// @raises(xn::XNetworkError);
auto test_raise_dominating_set() {
    G = xn::path_graph(4);
    D = xn::dominating_set(G, start_with=10);


auto test_is_dominating_set() {
    G = xn::path_graph(4);
    d = set([1, 3]);
    assert_true(xn::is_dominating_set(G, d));
    d = set([0, 2]);
    assert_true(xn::is_dominating_set(G, d));
    d = set([1]);
    assert_false(xn::is_dominating_set(G, d));


auto test_wikipedia_is_dominating_set() {
    /** Example from https://en.wikipedia.org/wiki/Dominating_set
     */
    G = xn::cycle_graph(4);
    G.add_edges_from([(0, 4), (1, 4), (2, 5)]);
    assert_true(xn::is_dominating_set(G, set([4, 3, 5])));
    assert_true(xn::is_dominating_set(G, set([0, 2])));
    assert_true(xn::is_dominating_set(G, set([1, 2])));
