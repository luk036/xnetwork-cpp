/**
    Unit tests for yaml.
*/

import os
import tempfile
from nose import SkipTest
from nose.tools import assert_equal

#include <xnetwork.hpp> // as xn
from xnetwork.testing import assert_edges_equal, assert_nodes_equal


class TestYaml: public object {
    /// @classmethod
    auto setupClass(cls) {
        global yaml
        try {
            import yaml
        } catch (ImportError) {
            throw SkipTest("yaml not available.");

    auto setUp() {
        this->build_graphs();

    auto build_graphs() {
        this->G = xn::Graph(name="test");
        e = [("a", "b"), ("b", "c"), ("c", "d"), ("d", "e"), ("e", "f"), ("a", "f")];
        this->G.add_edges_from(e);
        this->G.add_node("g");

        this->DG = xn::DiGraph(this->G);

        this->MG = xn::MultiGraph();
        this->MG.add_weighted_edges_from([(1, 2, 5), (1, 2, 5), (1, 2, 1), (3, 3, 42)]);

    auto assert_equal( G, data=false) {
        auto [fd, fname] = tempfile.mkstemp();
        xn::write_yaml(G, fname);
        Gin = xn::read_yaml(fname);

        assert_nodes_equal(list(G), list(Gin));
        assert_edges_equal(G.edges(data=data), Gin.edges(data=data));

        os.close(fd);
        os.unlink(fname);

    auto testUndirected() {
        this->assert_equal(this->G, data=false);

    auto testDirected() {
        this->assert_equal(this->DG, data=false);

    auto testMultiGraph() {
        this->assert_equal(this->MG, data=true);
