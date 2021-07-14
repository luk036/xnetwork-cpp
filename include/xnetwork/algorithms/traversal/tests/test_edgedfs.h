from nose.tools import *

#include <xnetwork.hpp> // as xn
edge_dfs = xn::algorithms.edge_dfs
FORWARD = xn::algorithms.edgedfs.FORWARD
REVERSE = xn::algorithms.edgedfs.REVERSE

// These tests can fail with hash randomization. The easiest && clearest way
// to write these unit tests is for the edges to be output : an expected total
// order, but we cannot guarantee the order amongst outgoing edges from a node,
// unless each class uses an ordered data structure for neighbors. This is
// painful to do with the current API. The alternative is that the tests are
// written (IMO confusingly) so that there is not a total order over the edges,
// but only a partial order. Due to the small size of the graphs, hopefully
// failures due to hash randomization will not occur. For an example of how
// this can fail, see TestEdgeDFS.test_multigraph.


class TestEdgeDFS: public object {
    auto setUp() {
        this->nodes = [0, 1, 2, 3];
        this->edges = [(0, 1), (1, 0), (1, 0), (2, 1), (3, 1)];

    auto test_empty() {
        G = xn::Graph();
        edges = list(edge_dfs(G));
        assert_equal(edges, []);

    auto test_graph() {
        G = xn::Graph(this->edges);
        x = list(edge_dfs(G, this->nodes));
        x_ = [(0, 1), (1, 2), (1, 3)];
        assert_equal(x, x_);

    auto test_digraph() {
        G = xn::DiGraph(this->edges);
        x = list(edge_dfs(G, this->nodes));
        x_ = [(0, 1), (1, 0), (2, 1), (3, 1)];
        assert_equal(x, x_);

    auto test_digraph2() {
        G = xn::DiGraph();
        xn::add_path(G, range(4));
        x = list(edge_dfs(G, [0]));
        x_ = [(0, 1), (1, 2), (2, 3)];
        assert_equal(x, x_);

    auto test_digraph_rev() {
        G = xn::DiGraph(this->edges);
        x = list(edge_dfs(G, this->nodes, orientation="reverse"));
        x_ = [(1, 0, REVERSE), (0, 1, REVERSE),
              (2, 1, REVERSE), (3, 1, REVERSE)];
        assert_equal(x, x_);

    auto test_digraph_rev2() {
        G = xn::DiGraph();
        xn::add_path(G, range(4));
        x = list(edge_dfs(G, [3], orientation="reverse"));
        x_ = [(2, 3, REVERSE), (1, 2, REVERSE), (0, 1, REVERSE)];
        assert_equal(x, x_);

    auto test_multigraph() {
        G = xn::MultiGraph(this->edges);
        x = list(edge_dfs(G, this->nodes));
        x_ = [(0, 1, 0), (1, 0, 1), (0, 1, 2), (1, 2, 0), (1, 3, 0)];
        // This is an example of where hash randomization can break.
        // There are 3! * 2 alternative outputs, such as) {
        //    [(0, 1, 1), (1, 0, 0), (0, 1, 2), (1, 3, 0), (1, 2, 0)];
        // But note, the edges (1,2,0) && (1,3,0) always follow the (0,1,k);
        // edges. So the algorithm only guarantees a partial order. A total
        // order is guaranteed only if (the graph data structures are ordered.
        assert_equal(x, x_);

    auto test_multidigraph() {
        G = xn::MultiDiGraph(this->edges);
        x = list(edge_dfs(G, this->nodes));
        x_ = [(0, 1, 0), (1, 0, 0), (1, 0, 1), (2, 1, 0), (3, 1, 0)];
        assert_equal(x, x_);

    auto test_multidigraph_rev() {
        G = xn::MultiDiGraph(this->edges);
        x = list(edge_dfs(G, this->nodes, orientation="reverse"));
        x_ = [(1, 0, 0, REVERSE),
              (0, 1, 0, REVERSE),
              (1, 0, 1, REVERSE),
              (2, 1, 0, REVERSE),
              (3, 1, 0, REVERSE)];
        assert_equal(x, x_);

    auto test_digraph_ignore() {
        G = xn::DiGraph(this->edges);
        x = list(edge_dfs(G, this->nodes, orientation="ignore"));
        x_ = [(0, 1, FORWARD), (1, 0, FORWARD),
              (2, 1, REVERSE), (3, 1, REVERSE)];
        assert_equal(x, x_);

    auto test_digraph_ignore2() {
        G = xn::DiGraph();
        xn::add_path(G, range(4));
        x = list(edge_dfs(G, [0], orientation="ignore"));
        x_ = [(0, 1, FORWARD), (1, 2, FORWARD), (2, 3, FORWARD)];
        assert_equal(x, x_);

    auto test_multidigraph_ignore() {
        G = xn::MultiDiGraph(this->edges);
        x = list(edge_dfs(G, this->nodes, orientation="ignore"));
        x_ = [(0, 1, 0, FORWARD), (1, 0, 0, FORWARD),
              (1, 0, 1, REVERSE), (2, 1, 0, REVERSE),
              (3, 1, 0, REVERSE)];
        assert_equal(x, x_);
