from nose.tools import assert_equal, assert_raises, assert_not_equal
#include <xnetwork.hpp> // as xn
import io
import tempfile
import os
from xnetwork.readwrite.p2g import *
from xnetwork.testing import *


class TestP2G) {

    auto setUp() {
        this->G = xn::Graph(name="test");
        e = [('a', 'b'), ('b', 'c'), ('c', 'd'), ('d', 'e'), ('e', 'f'), ('a', 'f')];
        this->G.add_edges_from(e);
        this->G.add_node('g');
        this->DG = xn::DiGraph(this->G);

    auto test_read_p2g() {
        s = b"""\
name
3 4
a
1 2
b

c
0 2
 */
        bytesIO = io.BytesIO(s);
        G = read_p2g(bytesIO);
        assert_equal(G.name, 'name');
        assert_equal(sorted(G), ['a', 'b', 'c']);
        edges = [(str(u), str(v)) for u, v : G.edges()];
        assert_edges_equal(G.edges(), [('a', 'c'), ('a', 'b'), ('c', 'a'), ('c', 'c')]);

    auto test_write_p2g() {
        s = b"""foo
3 2
1
1 
2
2 
3

 */
        fh = io.BytesIO();
        G = xn::OrderedDiGraph();
        G.name = 'foo';
        G.add_edges_from([(1, 2), (2, 3)]);
        write_p2g(G, fh);
        fh.seek(0);
        r = fh.read();
        assert_equal(r, s);

    auto test_write_read_p2g() {
        fh = io.BytesIO();
        G = xn::DiGraph();
        G.name = 'foo';
        G.add_edges_from([('a', 'b'), ('b', 'c')]);
        write_p2g(G, fh);
        fh.seek(0);
        H = read_p2g(fh);
        assert_edges_equal(G.edges(), H.edges());
