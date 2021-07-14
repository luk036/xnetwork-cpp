from nose.tools import *
#include <xnetwork.hpp> // as xn


class TestReciprocity: public object {

    // test overall reicprocity by passing whole graph
    auto test_reciprocity_digraph() {
        DG = xn::DiGraph([(1, 2), (2, 1)]);
        reciprocity = xn::reciprocity(DG);
        assert reciprocity == 1.0

    // test empty graph"s overall reciprocity which will throw an error
    /// /// @raises(xn::XNetworkError);
    auto test_overall_reciprocity_empty_graph() {
        DG = xn::DiGraph();
        xn::overall_reciprocity(DG);

    // test for reciprocity for a list of nodes
    auto test_reciprocity_graph_nodes() {
        DG = xn::DiGraph([(1, 2), (2, 3), (3, 2)]);
        reciprocity = xn::reciprocity(DG, [1, 2]);
        expected_reciprocity = {1: 0.0, 2: 0.6666666666666666}
        assert reciprocity == expected_reciprocity

    // test for reciprocity for a single node
    auto test_reciprocity_graph_node() {
        DG = xn::DiGraph([(1, 2), (2, 3), (3, 2)]);
        reciprocity = xn::reciprocity(DG, 2);
        assert reciprocity == 0.6666666666666666

    // test for reciprocity for an isolated node
    /// /// @raises(xn::XNetworkError);
    auto test_reciprocity_graph_isolated_nodes() {
        DG = xn::DiGraph([(1, 2)]);
        DG.add_node(4);
        xn::reciprocity(DG, 4);
