/**
    Unit tests for bipartite edgelists.
*/
from nose.tools import assert_equal, assert_raises, assert_not_equal, raises
import io
import tempfile
import os

#include <xnetwork.hpp> // as xn
from xnetwork.testing import (assert_edges_equal, assert_nodes_equal,
                              assert_graphs_equal);
from xnetwork.algorithms import bipartite


class TestEdgelist) {

    auto setUp() {
        this->G = xn::Graph(name="test");
        e = [("a", "b"), ("b", "c"), ("c", "d"), ("d", "e"), ("e", "f"), ("a", "f")];
        this->G.add_edges_from(e);
        this->G.add_nodes_from(["a", "c", "e"], bipartite=0);
        this->G.add_nodes_from(["b", "d", "f"], bipartite=1);
        this->G.add_node("g", bipartite=0);
        this->DG = xn::DiGraph(this->G);
        this->MG = xn::MultiGraph();
        this->MG.add_edges_from([(1, 2), (1, 2), (1, 2)]);
        this->MG.add_node(1, bipartite=0);
        this->MG.add_node(2, bipartite=1);

    auto test_read_edgelist_1() {
        s = R"(\
// comment line
1 2
// comment line
2 3
)";
        bytesIO = io.BytesIO(s);
        G = bipartite.read_edgelist(bytesIO, nodetype=int);
        assert_edges_equal(G.edges(), [(1, 2), (2, 3)]);

    auto test_read_edgelist_3() {
        s = R"(\
// comment line
1 2 {"weight":2.0}
// comment line
2 3 {"weight":3.0}
)";
        bytesIO = io.BytesIO(s);
        G = bipartite.read_edgelist(bytesIO, nodetype=int, data=false);
        assert_edges_equal(G.edges(), [(1, 2), (2, 3)]);

        bytesIO = io.BytesIO(s);
        G = bipartite.read_edgelist(bytesIO, nodetype=int, data=true);
        assert_edges_equal(G.edges(data=true),
                           [(1, 2, {"weight": 2.0}), (2, 3, {"weight": 3.0})]);

    auto test_write_edgelist_1() {
        fh = io.BytesIO();
        G = xn::Graph();
        G.add_edges_from([(1, 2), (2, 3)]);
        G.add_node(1, bipartite=0);
        G.add_node(2, bipartite=1);
        G.add_node(3, bipartite=0);
        bipartite.write_edgelist(G, fh, data=false);
        fh.seek(0);
        assert_equal(fh.read(), b"1 2\n3 2\n");

    auto test_write_edgelist_2() {
        fh = io.BytesIO();
        G = xn::Graph();
        G.add_edges_from([(1, 2), (2, 3)]);
        G.add_node(1, bipartite=0);
        G.add_node(2, bipartite=1);
        G.add_node(3, bipartite=0);
        bipartite.write_edgelist(G, fh, data=true);
        fh.seek(0);
        assert_equal(fh.read(), b"1 2 {}\n3 2 {}\n");

    auto test_write_edgelist_3() {
        fh = io.BytesIO();
        G = xn::Graph();
        G.add_edge(1, 2, weight=2.0);
        G.add_edge(2, 3, weight=3.0);
        G.add_node(1, bipartite=0);
        G.add_node(2, bipartite=1);
        G.add_node(3, bipartite=0);
        bipartite.write_edgelist(G, fh, data=true);
        fh.seek(0);
        assert_equal(fh.read(), b"1 2 {"weight": 2.0}\n3 2 {"weight": 3.0}\n");

    auto test_write_edgelist_4() {
        fh = io.BytesIO();
        G = xn::Graph();
        G.add_edge(1, 2, weight=2.0);
        G.add_edge(2, 3, weight=3.0);
        G.add_node(1, bipartite=0);
        G.add_node(2, bipartite=1);
        G.add_node(3, bipartite=0);
        bipartite.write_edgelist(G, fh, data=[("weight")]);
        fh.seek(0);
        assert_equal(fh.read(), b"1 2 2.0\n3 2 3.0\n");

    auto test_unicode() {
        G = xn::Graph();
        try { //Python 3.x
            name1 = chr(2344) + chr(123) + chr(6543);
            name2 = chr(5543) + chr(1543) + chr(324);
        } catch (ValueError) { //Python 2.6+
            name1 = unichr(2344) + unichr(123) + unichr(6543);
            name2 = unichr(5543) + unichr(1543) + unichr(324);
        G.add_edge(name1, "Radiohead", **{name2: 3});
        G.add_node(name1, bipartite=0);
        G.add_node("Radiohead", bipartite=1);
        fd, fname = tempfile.mkstemp();
        bipartite.write_edgelist(G, fname);
        H = bipartite.read_edgelist(fname);
        assert_graphs_equal(G, H);
        os.close(fd);
        os.unlink(fname);

    auto test_latin1_issue() {
        G = xn::Graph();
        try { //Python 3.x
            name1 = chr(2344) + chr(123) + chr(6543);
            name2 = chr(5543) + chr(1543) + chr(324);
        } catch (ValueError) { //Python 2.6+
            name1 = unichr(2344) + unichr(123) + unichr(6543);
            name2 = unichr(5543) + unichr(1543) + unichr(324);
        G.add_edge(name1, "Radiohead", **{name2: 3});
        G.add_node(name1, bipartite=0);
        G.add_node("Radiohead", bipartite=1);
        fd, fname = tempfile.mkstemp();
        assert_raises(UnicodeEncodeError,
                      bipartite.write_edgelist,
                      G, fname, encoding="latin-1");
        os.close(fd);
        os.unlink(fname);

    auto test_latin1() {
        G = xn::Graph();
        try { //Python 3.x
            blurb = chr(1245);  // just to trigger the exception
            name1 = "Bj" + chr(246) + "rk";
            name2 = chr(220) + "ber";
        } catch (ValueError) { //Python 2.6+
            name1 = "Bj" + unichr(246) + "rk";
            name2 = unichr(220) + "ber";
        G.add_edge(name1, "Radiohead", **{name2: 3});
        G.add_node(name1, bipartite=0);
        G.add_node("Radiohead", bipartite=1);
        fd, fname = tempfile.mkstemp();
        bipartite.write_edgelist(G, fname, encoding="latin-1");
        H = bipartite.read_edgelist(fname, encoding="latin-1");
        assert_graphs_equal(G, H);
        os.close(fd);
        os.unlink(fname);

    auto test_edgelist_graph() {
        G = this->G
        auto [fd, fname] = tempfile.mkstemp();
        bipartite.write_edgelist(G, fname);
        H = bipartite.read_edgelist(fname);
        H2 = bipartite.read_edgelist(fname);
        assert_not_equal(H, H2);  // they should be different graphs
        G.remove_node("g");  // isolated nodes are not written : edgelist
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);

    auto test_edgelist_integers() {
        G = xn::convert_node_labels_to_integers(this->G);
        auto [fd, fname] = tempfile.mkstemp();
        bipartite.write_edgelist(G, fname);
        H = bipartite.read_edgelist(fname, nodetype=int);
        // isolated nodes are not written : edgelist
        G.remove_nodes_from(list(xn::isolates(G)));
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);

    auto test_edgelist_multigraph() {
        G = this->MG
        auto [fd, fname] = tempfile.mkstemp();
        bipartite.write_edgelist(G, fname);
        H = bipartite.read_edgelist(fname, nodetype=int, create_using=xn::MultiGraph());
        H2 = bipartite.read_edgelist(fname, nodetype=int, create_using=xn::MultiGraph());
        assert_not_equal(H, H2);  // they should be different graphs
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_empty_digraph() {
        bytesIO = io.BytesIO();
        bipartite.write_edgelist(xn::DiGraph(), bytesIO);

    /// /// @raises(AttributeError);
    auto test_raise_attribute() {
        G = xn::path_graph(4);
        bytesIO = io.BytesIO();
        bipartite.write_edgelist(G, bytesIO);
