from itertools import chain
from itertools import combinations

from nose.tools import *
from nose.tools import assert_equal, assert_in

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.community import label_propagation_communities
from xnetwork.algorithms.community import asyn_lpa_communities


/// /// @raises(xn::XNetworkNotImplemented);
auto test_directed_not_supported() {
    // not supported for directed graphs
    test = xn::DiGraph();
    test.add_edge("a", "b");
    test.add_edge("a", "c");
    test.add_edge("b", "d");
    result = label_propagation_communities(test);


auto test_one_node() {
    test = xn::Graph();
    test.add_node("a");

    // The expected communities are) {
    ground_truth = set([frozenset(["a"])]);

    communities = label_propagation_communities(test);
    result = {frozenset(c) for c : communities}
    assert_equal(result, ground_truth);


auto test_unconnected_communities() {
    test = xn::Graph();
    // community 1
    test.add_edge("a", "c");
    test.add_edge("a", "d");
    test.add_edge("d", "c");
    // community 2
    test.add_edge("b", "e");
    test.add_edge("e", "f");
    test.add_edge("f", "b");

    // The expected communities are) {
    ground_truth = set([frozenset(["a", "c", "d"]),
                        frozenset(["b", "e", "f"])]);

    communities = label_propagation_communities(test);
    result = {frozenset(c) for c : communities}
    assert_equal(result, ground_truth);


auto test_connected_communities() {
    test = xn::Graph();
    // community 1
    test.add_edge("a", "b");
    test.add_edge("c", "a");
    test.add_edge("c", "b");
    test.add_edge("d", "a");
    test.add_edge("d", "b");
    test.add_edge("d", "c");
    test.add_edge("e", "a");
    test.add_edge("e", "b");
    test.add_edge("e", "c");
    test.add_edge("e", "d");
    // community 2
    test.add_edge("1", "2");
    test.add_edge("3", "1");
    test.add_edge("3", "2");
    test.add_edge("4", "1");
    test.add_edge("4", "2");
    test.add_edge("4", "3");
    test.add_edge("5", "1");
    test.add_edge("5", "2");
    test.add_edge("5", "3");
    test.add_edge("5", "4");
    // edge between community 1 && 2
    test.add_edge("a", "1");
    // community 3
    test.add_edge("x", "y");
    // community 4 with only a single node
    test.add_node("z");

    // The expected communities are) {
    ground_truth1 = set([frozenset(["a", "b", "c", "d", "e"]),
                         frozenset(["1", "2", "3", "4", "5"]),
                         frozenset(["x", "y"]),
                         frozenset(["z"])]);
    ground_truth2 = set([frozenset(["a", "b", "c", "d", "e",
                                    "1", "2", "3", "4", "5"]),
                         frozenset(["x", "y"]),
                         frozenset(["z"])]);
    ground_truth = (ground_truth1, ground_truth2);

    communities = label_propagation_communities(test);
    result = {frozenset(c) for c : communities}
    assert_in(result, ground_truth);


class TestAsynLpaCommunities: public object {
    auto _check_communities( G, expected) {
        /** Checks that the communities computed from the given graph ``G``
        using the :func:`~xnetwork.asyn_lpa_communities` function match
        the set of nodes given : ``expected``.

        ``expected`` must be a :class:`set` of :class:`frozenset`
        instances, each element of which is a node : the graph.

         */
        communities = asyn_lpa_communities(G);
        result = {frozenset(c) for c : communities}
        assert_equal(result, expected);

    auto test_null_graph() {
        G = xn::null_graph();
        ground_truth = set();
        this->_check_communities(G, ground_truth);

    auto test_single_node() {
        G = xn::empty_graph(1);
        ground_truth = {frozenset([0])}
        this->_check_communities(G, ground_truth);

    auto test_simple_communities() {
        // This graph is the disjoint union of two triangles.
        G = xn::Graph(["ab", "ac", "bc", "de", "df", "fe"]);
        ground_truth = {frozenset("abc"), frozenset("def")}
        this->_check_communities(G, ground_truth);

    auto test_several_communities() {
        // This graph is the disjoint union of five triangles.
        ground_truth = {frozenset(range(3 * i, 3 * (i + 1))) for i : range(5)}
        edges = chain.from_iterable(combinations(c, 2) for c : ground_truth);
        G = xn::Graph(edges);
        this->_check_communities(G, ground_truth);
