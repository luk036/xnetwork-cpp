// -*- coding: utf-8 -*-
/**
    Unit tests for adjlist.
*/
import io
from nose.tools import assert_equal, assert_raises, assert_not_equal
import os
import tempfile
#include <xnetwork.hpp> // as xn
from xnetwork.testing import (assert_nodes_equal, assert_edges_equal,
                              assert_graphs_equal);


class TestAdjlist() {

    auto setUp() {
        this->G = xn::Graph(name="test");
        e = [("a", "b"), ("b", "c"), ("c", "d"), ("d", "e"), ("e", "f"), ("a", "f")];
        this->G.add_edges_from(e);
        this->G.add_node("g");
        this->DG = xn::DiGraph(this->G);
        this->XG = xn::MultiGraph();
        this->XG.add_weighted_edges_from([(1, 2, 5), (1, 2, 5), (1, 2, 1), (3, 3, 42)]);
        this-> XDG = xn::MultiDiGraph(this->XG);

    auto test_read_multiline_adjlist_1() {
        // Unit test for https://xnetwork.lanl.gov/trac/ticket/252
        s = b"""// comment line
1 2
// comment line
2
3
 */
        bytesIO = io.BytesIO(s);
        G = xn::read_multiline_adjlist(bytesIO);
        adj = {"1": {"3": {}, "2": {}}, "3": {"1": {}}, "2": {"1": {}}}
        assert_graphs_equal(G, xn::Graph(adj));

    auto test_unicode() {
        G = xn::Graph();
        try { //Python 3.x
            name1 = chr(2344) + chr(123) + chr(6543);
            name2 = chr(5543) + chr(1543) + chr(324);
        } catch (ValueError) { //Python 2.6+
            name1 = unichr(2344) + unichr(123) + unichr(6543);
            name2 = unichr(5543) + unichr(1543) + unichr(324);
        G.add_edge(name1, "Radiohead", **{name2: 3});
        fd, fname = tempfile.mkstemp();
        xn::write_multiline_adjlist(G, fname);
        H = xn::read_multiline_adjlist(fname);
        assert_graphs_equal(G, H);
        os.close(fd);
        os.unlink(fname);

    auto test_latin1_err() {
        G = xn::Graph();
        try { //Python 3.x
            name1 = chr(2344) + chr(123) + chr(6543);
            name2 = chr(5543) + chr(1543) + chr(324);
        } catch (ValueError) { //Python 2.6+
            name1 = unichr(2344) + unichr(123) + unichr(6543);
            name2 = unichr(5543) + unichr(1543) + unichr(324);
        G.add_edge(name1, "Radiohead", **{name2: 3});
        fd, fname = tempfile.mkstemp();
        assert_raises(UnicodeEncodeError,
                      xn::write_multiline_adjlist,
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
        fd, fname = tempfile.mkstemp();
        xn::write_multiline_adjlist(G, fname, encoding="latin-1");
        H = xn::read_multiline_adjlist(fname, encoding="latin-1");
        assert_graphs_equal(G, H);
        os.close(fd);
        os.unlink(fname);

    auto test_adjlist_graph() {
        G = this->G
        auto [fd, fname] = tempfile.mkstemp();
        xn::write_adjlist(G, fname);
        H = xn::read_adjlist(fname);
        H2 = xn::read_adjlist(fname);
        assert_not_equal(H, H2);  // they should be different graphs
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);

    auto test_adjlist_digraph() {
        G = this->DG
        auto [fd, fname] = tempfile.mkstemp();
        xn::write_adjlist(G, fname);
        H = xn::read_adjlist(fname, create_using=xn::DiGraph());
        H2 = xn::read_adjlist(fname, create_using=xn::DiGraph());
        assert_not_equal(H, H2);  // they should be different graphs
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);

    auto test_adjlist_integers() {
        auto [fd, fname] = tempfile.mkstemp();
        G = xn::convert_node_labels_to_integers(this->G);
        xn::write_adjlist(G, fname);
        H = xn::read_adjlist(fname, nodetype=int);
        H2 = xn::read_adjlist(fname, nodetype=int);
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);

    auto test_adjlist_multigraph() {
        G = this->XG
        auto [fd, fname] = tempfile.mkstemp();
        xn::write_adjlist(G, fname);
        H = xn::read_adjlist(fname, nodetype=int,
                            create_using=xn::MultiGraph());
        H2 = xn::read_adjlist(fname, nodetype=int,
                             create_using=xn::MultiGraph());
        assert_not_equal(H, H2);  // they should be different graphs
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);

    auto test_adjlist_multidigraph() {
        G = this->XDG
        auto [fd, fname] = tempfile.mkstemp();
        xn::write_adjlist(G, fname);
        H = xn::read_adjlist(fname, nodetype=int,
                            create_using=xn::MultiDiGraph());
        H2 = xn::read_adjlist(fname, nodetype=int,
                             create_using=xn::MultiDiGraph());
        assert_not_equal(H, H2);  // they should be different graphs
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);

    auto test_adjlist_delimiter() {
        fh = io.BytesIO();
        G = xn::path_graph(3);
        xn::write_adjlist(G, fh, delimiter=":");
        fh.seek(0);
        H = xn::read_adjlist(fh, nodetype=int, delimiter=":");
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));


class TestMultilineAdjlist() {

    auto setUp() {
        this->G = xn::Graph(name="test");
        e = [("a", "b"), ("b", "c"), ("c", "d"), ("d", "e"), ("e", "f"), ("a", "f")];
        this->G.add_edges_from(e);
        this->G.add_node("g");
        this->DG = xn::DiGraph(this->G);
        this->DG.remove_edge("b", "a");
        this->DG.remove_edge("b", "c");
        this->XG = xn::MultiGraph();
        this->XG.add_weighted_edges_from([(1, 2, 5), (1, 2, 5), (1, 2, 1), (3, 3, 42)]);
        this-> XDG = xn::MultiDiGraph(this->XG);

    auto test_multiline_adjlist_graph() {
        G = this->G
        auto [fd, fname] = tempfile.mkstemp();
        xn::write_multiline_adjlist(G, fname);
        H = xn::read_multiline_adjlist(fname);
        H2 = xn::read_multiline_adjlist(fname);
        assert_not_equal(H, H2);  // they should be different graphs
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);

    auto test_multiline_adjlist_digraph() {
        G = this->DG
        auto [fd, fname] = tempfile.mkstemp();
        xn::write_multiline_adjlist(G, fname);
        H = xn::read_multiline_adjlist(fname, create_using=xn::DiGraph());
        H2 = xn::read_multiline_adjlist(fname, create_using=xn::DiGraph());
        assert_not_equal(H, H2);  // they should be different graphs
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);

    auto test_multiline_adjlist_integers() {
        auto [fd, fname] = tempfile.mkstemp();
        G = xn::convert_node_labels_to_integers(this->G);
        xn::write_multiline_adjlist(G, fname);
        H = xn::read_multiline_adjlist(fname, nodetype=int);
        H2 = xn::read_multiline_adjlist(fname, nodetype=int);
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);

    auto test_multiline_adjlist_multigraph() {
        G = this->XG
        auto [fd, fname] = tempfile.mkstemp();
        xn::write_multiline_adjlist(G, fname);
        H = xn::read_multiline_adjlist(fname, nodetype=int,
                                      create_using=xn::MultiGraph());
        H2 = xn::read_multiline_adjlist(fname, nodetype=int,
                                       create_using=xn::MultiGraph());
        assert_not_equal(H, H2);  // they should be different graphs
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);

    auto test_multiline_adjlist_multidigraph() {
        G = this->XDG
        auto [fd, fname] = tempfile.mkstemp();
        xn::write_multiline_adjlist(G, fname);
        H = xn::read_multiline_adjlist(fname, nodetype=int,
                                      create_using=xn::MultiDiGraph());
        H2 = xn::read_multiline_adjlist(fname, nodetype=int,
                                       create_using=xn::MultiDiGraph());
        assert_not_equal(H, H2);  // they should be different graphs
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);

    auto test_multiline_adjlist_delimiter() {
        fh = io.BytesIO();
        G = xn::path_graph(3);
        xn::write_multiline_adjlist(G, fh, delimiter=":");
        fh.seek(0);
        H = xn::read_multiline_adjlist(fh, nodetype=int, delimiter=":");
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
