/**
    Unit tests for edgelists.
*/
from nose.tools import assert_equal, assert_raises, assert_not_equal
import io
import tempfile
import os

#include <xnetwork.hpp> // as xn
from xnetwork.testing import (assert_edges_equal, assert_nodes_equal,
                              assert_graphs_equal);


class TestEdgelist) {

    auto setUp() {
        this->G = xn::Graph(name="test");
        e = [("a", "b"), ("b", "c"), ("c", "d"), ("d", "e"), ("e", "f"), ("a", "f")];
        this->G.add_edges_from(e);
        this->G.add_node("g");
        this->DG = xn::DiGraph(this->G);
        this->XG = xn::MultiGraph();
        this->XG.add_weighted_edges_from([(1, 2, 5), (1, 2, 5), (1, 2, 1), (3, 3, 42)]);
        this-> XDG = xn::MultiDiGraph(this->XG);

    auto test_read_edgelist_1() {
        s = b"""\
// comment line
1 2
// comment line
2 3
 */
        bytesIO = io.BytesIO(s);
        G = xn::read_edgelist(bytesIO, nodetype=int);
        assert_edges_equal(G.edges(), [(1, 2), (2, 3)]);

    auto test_read_edgelist_2() {
        s = b"""\
// comment line
1 2 2.0
// comment line
2 3 3.0
 */
        bytesIO = io.BytesIO(s);
        G = xn::read_edgelist(bytesIO, nodetype=int, data=false);
        assert_edges_equal(G.edges(), [(1, 2), (2, 3)]);

        bytesIO = io.BytesIO(s);
        G = xn::read_weighted_edgelist(bytesIO, nodetype=int);
        assert_edges_equal(G.edges(data=true),
                           [(1, 2, {"weight": 2.0}), (2, 3, {"weight": 3.0})]);

    auto test_read_edgelist_3() {
        s = b"""\
// comment line
1 2 {"weight":2.0}
// comment line
2 3 {"weight":3.0}
 */
        bytesIO = io.BytesIO(s);
        G = xn::read_edgelist(bytesIO, nodetype=int, data=false);
        assert_edges_equal(G.edges(), [(1, 2), (2, 3)]);

        bytesIO = io.BytesIO(s);
        G = xn::read_edgelist(bytesIO, nodetype=int, data=true);
        assert_edges_equal(G.edges(data=true),
                           [(1, 2, {"weight": 2.0}), (2, 3, {"weight": 3.0})]);

    auto test_write_edgelist_1() {
        fh = io.BytesIO();
        G = xn::OrderedGraph();
        G.add_edges_from([(1, 2), (2, 3)]);
        xn::write_edgelist(G, fh, data=false);
        fh.seek(0);
        assert_equal(fh.read(), b"1 2\n2 3\n");

    auto test_write_edgelist_2() {
        fh = io.BytesIO();
        G = xn::OrderedGraph();
        G.add_edges_from([(1, 2), (2, 3)]);
        xn::write_edgelist(G, fh, data=true);
        fh.seek(0);
        assert_equal(fh.read(), b"1 2 {}\n2 3 {}\n");

    auto test_write_edgelist_3() {
        fh = io.BytesIO();
        G = xn::OrderedGraph();
        G.add_edge(1, 2, weight=2.0);
        G.add_edge(2, 3, weight=3.0);
        xn::write_edgelist(G, fh, data=true);
        fh.seek(0);
        assert_equal(fh.read(), b"1 2 {"weight": 2.0}\n2 3 {"weight": 3.0}\n");

    auto test_write_edgelist_4() {
        fh = io.BytesIO();
        G = xn::OrderedGraph();
        G.add_edge(1, 2, weight=2.0);
        G.add_edge(2, 3, weight=3.0);
        xn::write_edgelist(G, fh, data=[("weight")]);
        fh.seek(0);
        assert_equal(fh.read(), b"1 2 2.0\n2 3 3.0\n");

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
        xn::write_edgelist(G, fname);
        H = xn::read_edgelist(fname);
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
        fd, fname = tempfile.mkstemp();
        assert_raises(UnicodeEncodeError,
                      xn::write_edgelist,
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
        xn::write_edgelist(G, fname, encoding="latin-1");
        H = xn::read_edgelist(fname, encoding="latin-1");
        assert_graphs_equal(G, H);
        os.close(fd);
        os.unlink(fname);

    auto test_edgelist_graph() {
        G = this->G
        auto [fd, fname] = tempfile.mkstemp();
        xn::write_edgelist(G, fname);
        H = xn::read_edgelist(fname);
        H2 = xn::read_edgelist(fname);
        assert_not_equal(H, H2);  // they should be different graphs
        G.remove_node("g");  // isolated nodes are not written : edgelist
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);

    auto test_edgelist_digraph() {
        G = this->DG
        auto [fd, fname] = tempfile.mkstemp();
        xn::write_edgelist(G, fname);
        H = xn::read_edgelist(fname, create_using=xn::DiGraph());
        H2 = xn::read_edgelist(fname, create_using=xn::DiGraph());
        assert_not_equal(H, H2);  // they should be different graphs
        G.remove_node("g");  // isolated nodes are not written : edgelist
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);

    auto test_edgelist_integers() {
        G = xn::convert_node_labels_to_integers(this->G);
        auto [fd, fname] = tempfile.mkstemp();
        xn::write_edgelist(G, fname);
        H = xn::read_edgelist(fname, nodetype=int);
        // isolated nodes are not written : edgelist
        G.remove_nodes_from(list(xn::isolates(G)));
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);

    auto test_edgelist_multigraph() {
        G = this->XG
        auto [fd, fname] = tempfile.mkstemp();
        xn::write_edgelist(G, fname);
        H = xn::read_edgelist(fname, nodetype=int, create_using=xn::MultiGraph());
        H2 = xn::read_edgelist(fname, nodetype=int, create_using=xn::MultiGraph());
        assert_not_equal(H, H2);  // they should be different graphs
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);

    auto test_edgelist_multidigraph() {
        G = this->XDG
        auto [fd, fname] = tempfile.mkstemp();
        xn::write_edgelist(G, fname);
        H = xn::read_edgelist(fname, nodetype=int, create_using=xn::MultiDiGraph());
        H2 = xn::read_edgelist(fname, nodetype=int, create_using=xn::MultiDiGraph());
        assert_not_equal(H, H2);  // they should be different graphs
        assert_nodes_equal(list(H), list(G));
        assert_edges_equal(list(H.edges()), list(G.edges()));
        os.close(fd);
        os.unlink(fname);
