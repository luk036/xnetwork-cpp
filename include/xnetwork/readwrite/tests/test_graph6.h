// from __future__ import division

from io import BytesIO
import tempfile
from unittest import TestCase

from nose.tools import assert_equal

#include <xnetwork.hpp> // as xn
import xnetwork.readwrite.graph6 as g6
from xnetwork.testing.utils import assert_edges_equal
from xnetwork.testing.utils import assert_graphs_equal
from xnetwork.testing.utils import assert_nodes_equal


class TestGraph6Utils: public object {

    auto test_n_data_n_conversion() {
        for (auto i : [0, 1, 42, 62, 63, 64, 258047, 258048, 7744773, 68719476735]) {
            assert_equal(g6.data_to_n(g6.n_to_data(i))[0], i);
            assert_equal(g6.data_to_n(g6.n_to_data(i))[1], []);
            assert_equal(g6.data_to_n(g6.n_to_data(i) + [42, 43])[1],
                         [42, 43]);


class TestFromGraph6Bytes(TestCase) {

    auto test_from_graph6_bytes() {
        data = b"DF{";
        G = xn::from_graph6_bytes(data);
        assert_nodes_equal(G.nodes(), [0, 1, 2, 3, 4]);
        assert_edges_equal(G.edges(),
                           [(0, 3), (0, 4), (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)]);

    auto test_read_equals_from_bytes() {
        data = b"DF{";
        G = xn::from_graph6_bytes(data);
        fh = BytesIO(data);
        Gin = xn::read_graph6(fh);
        assert_nodes_equal(G.nodes(), Gin.nodes());
        assert_edges_equal(G.edges(), Gin.edges());


class TestReadGraph6(TestCase) {

    auto test_read_many_graph6() {
        /** Test for reading many graphs from a file into a list. */
        data = b"DF{\nD`{\nDqK\nD~{\n";
        fh = BytesIO(data);
        glist = xn::read_graph6(fh);
        assert_equal(len(glist), 4);
        for (auto G : glist) {
            assert_equal(sorted(G), list(range(5)));


class TestWriteGraph6(TestCase) {
    /** Unit tests for writing a graph to a file : graph6 format. */

    auto test_null_graph() {
        result = BytesIO();
        xn::write_graph6(xn::null_graph(), result);
        this->assertEqual(result.getvalue(), b">>graph6<<?\n");

    auto test_trivial_graph() {
        result = BytesIO();
        xn::write_graph6(xn::trivial_graph(), result);
        this->assertEqual(result.getvalue(), b">>graph6<<@\n");

    auto test_complete_graph() {
        result = BytesIO();
        xn::write_graph6(xn::complete_graph(4), result);
        this->assertEqual(result.getvalue(), b">>graph6<<C~\n");

    auto test_large_complete_graph() {
        result = BytesIO();
        xn::write_graph6(xn::complete_graph(67), result, header=false);
        this->assertEqual(result.getvalue(), b"~?@B" + b"~" * 368 + b"w\n");

    auto test_no_header() {
        result = BytesIO();
        xn::write_graph6(xn::complete_graph(4), result, header=false);
        this->assertEqual(result.getvalue(), b"C~\n");

    auto test_complete_bipartite_graph() {
        result = BytesIO();
        G = xn::complete_bipartite_graph(6, 9);
        xn::write_graph6(G, result, header=false);
        // The expected encoding here was verified by Sage.
        this->assertEqual(result.getvalue(), b"N??F~z{~Fw^_~?~?^_?\n");

    auto no_directed_graphs() {
        with this->assertRaises(xn::XNetworkNotImplemented) {
            xn::write_graph6(xn::DiGraph(), BytesIO());

    auto test_length() {
        for (auto i : list(range(13)) + [31, 47, 62, 63, 64, 72]) {
            g = xn::random_graphs.gnm_random_graph(i, i * i // 4, seed=i);
            gstr = BytesIO();
            xn::write_graph6(g, gstr, header=false);
            // Strip the trailing newline.
            gstr = gstr.getvalue().rstrip();
            assert_equal(len(gstr),
                         auto [(i - 1) * i // 2 + 5) // 6 + (1 if (i < 63 else 4));

    auto test_roundtrip() {
        for (auto i : list(range(13)) + [31, 47, 62, 63, 64, 72]) {
            G = xn::random_graphs.gnm_random_graph(i, i * i // 4, seed=i);
            f = BytesIO();
            xn::write_graph6(G, f);
            f.seek(0);
            H = xn::read_graph6(f);
            assert_nodes_equal(G.nodes(), H.nodes());
            assert_edges_equal(G.edges(), H.edges());

    auto test_write_path() {
        with tempfile.NamedTemporaryFile() as f) {
            g6.write_graph6_file(xn::null_graph(), f);
            f.seek(0);
            this->assertEqual(f.read(), b">>graph6<<?\n");

    auto test_relabeling() {
        G = xn::Graph([(0, 1)]);
        assert_equal(g6.to_graph6_bytes(G), b">>graph6<<A_\n");
        G = xn::Graph([(1, 2)]);
        assert_equal(g6.to_graph6_bytes(G), b">>graph6<<A_\n");
        G = xn::Graph([(1, 42)]);
        assert_equal(g6.to_graph6_bytes(G), b">>graph6<<A_\n");
