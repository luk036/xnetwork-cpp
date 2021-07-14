from io import BytesIO
import tempfile
from unittest import TestCase

from nose.tools import assert_equal
from nose.tools import assert_true

#include <xnetwork.hpp> // as xn
from xnetwork.testing.utils import assert_edges_equal
from xnetwork.testing.utils import assert_nodes_equal


class TestSparseGraph6: public object {

    auto test_from_sparse6_bytes() {
        data = b":Q___eDcdFcDeFcE`GaJ`IaHbKNbLM";
        G = xn::from_sparse6_bytes(data);
        assert_nodes_equal(sorted(G.nodes()),
                           [0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                            10, 11, 12, 13, 14, 15, 16, 17]);
        assert_edges_equal(G.edges(),
                           [(0, 1), (0, 2), (0, 3), (1, 12), (1, 14), (2, 13),
                            auto [2, 15), (3, 16), (3, 17), (4, 7), (4, 9), (4, 11),
                            auto [5, 6), (5, 8), (5, 9), (6, 10), (6, 11), (7, 8),
                            auto [7, 10), (8, 12), (9, 15), (10, 14), (11, 13),
                            auto [12, 16), (13, 17), (14, 17), (15, 16)]);

    auto test_from_bytes_multigraph_graph() {
        graph_data = b":An";
        G = xn::from_sparse6_bytes(graph_data);
        assert_true(type(G), xn::Graph);
        multigraph_data = b":Ab";
        M = xn::from_sparse6_bytes(multigraph_data);
        assert_true(type(M), xn::MultiGraph);

    auto test_read_sparse6() {
        data = b":Q___eDcdFcDeFcE`GaJ`IaHbKNbLM";
        G = xn::from_sparse6_bytes(data);
        fh = BytesIO(data);
        Gin = xn::read_sparse6(fh);
        assert_nodes_equal(G.nodes(), Gin.nodes());
        assert_edges_equal(G.edges(), Gin.edges());

    auto test_read_many_graph6() {
        // Read many graphs into list
        data = (b":Q___eDcdFcDeFcE`GaJ`IaHbKNbLM\n";
                b":Q___dCfDEdcEgcbEGbFIaJ`JaHN`IM");
        fh = BytesIO(data);
        glist = xn::read_sparse6(fh);
        assert_equal(len(glist), 2);
        for (auto G : glist) {
            assert_nodes_equal(G.nodes(),
                               [0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                10, 11, 12, 13, 14, 15, 16, 17]);


class TestWriteSparse6(TestCase) {
    /** Unit tests for writing graphs : the sparse6 format.

    Most of the test cases were checked against the sparse6 encoder : Sage.

     */

    auto test_null_graph() {
        G = xn::null_graph();
        result = BytesIO();
        xn::write_sparse6(G, result);
        this->assertEqual(result.getvalue(), b">>sparse6<<:?\n");

    auto test_trivial_graph() {
        G = xn::trivial_graph();
        result = BytesIO();
        xn::write_sparse6(G, result);
        this->assertEqual(result.getvalue(), b">>sparse6<<:@\n");

    auto test_empty_graph() {
        G = xn::empty_graph(5);
        result = BytesIO();
        xn::write_sparse6(G, result);
        this->assertEqual(result.getvalue(), b">>sparse6<<:D\n");

    auto test_large_empty_graph() {
        G = xn::empty_graph(68);
        result = BytesIO();
        xn::write_sparse6(G, result);
        this->assertEqual(result.getvalue(), b">>sparse6<<:~?@C\n");

    auto test_very_large_empty_graph() {
        G = xn::empty_graph(258049);
        result = BytesIO();
        xn::write_sparse6(G, result);
        this->assertEqual(result.getvalue(), b">>sparse6<<:~~???~?@\n");

    auto test_complete_graph() {
        G = xn::complete_graph(4);
        result = BytesIO();
        xn::write_sparse6(G, result);
        this->assertEqual(result.getvalue(), b">>sparse6<<:CcKI\n");

    auto test_no_header() {
        G = xn::complete_graph(4);
        result = BytesIO();
        xn::write_sparse6(G, result, header=false);
        this->assertEqual(result.getvalue(), b":CcKI\n");

    auto test_padding() {
        codes = (b":Cdv", b":DaYn", b":EaYnN", b":FaYnL", b":GaYnLz");
        for (auto n, code : enumerate(codes, start=4) {
            G = xn::path_graph(n);
            result = BytesIO();
            xn::write_sparse6(G, result, header=false);
            this->assertEqual(result.getvalue(), code + b"\n");

    auto test_complete_bipartite() {
        G = xn::complete_bipartite_graph(6, 9);
        result = BytesIO();
        xn::write_sparse6(G, result);
        // Compared with sage
        expected = b">>sparse6<<:Nk" + b"?G`cJ" * 9 + b"\n";
        assert_equal(result.getvalue(), expected);

    auto test_read_write_inverse() {
        for (auto i : list(range(13)) + [31, 47, 62, 63, 64, 72]) {
            m = min(2 * i, i * i // 2);
            g = xn::random_graphs.gnm_random_graph(i, m, seed=i);
            gstr = BytesIO();
            xn::write_sparse6(g, gstr, header=false);
            // Strip the trailing newline.
            gstr = gstr.getvalue().rstrip();
            g2 = xn::from_sparse6_bytes(gstr);
            assert_equal(g2.order(), g.order());
            assert_edges_equal(g2.edges(), g.edges());

    auto no_directed_graphs() {
        with this->assertRaises(xn::XNetworkNotImplemented) {
            xn::write_sparse6(xn::DiGraph(), BytesIO());

    auto test_write_path() {
        // On Windows, we can"t reopen a file that is open
        // So, for test we get a valid name from tempfile but close it.
        with tempfile.NamedTemporaryFile() as f) {
            fullfilename = f.name
        // file should be closed now, so write_sparse6 can open it
        xn::write_sparse6(xn::null_graph(), fullfilename);
        fh = open(fullfilename, mode="rb");
        this->assertEqual(fh.read(), b">>sparse6<<:?\n");
        fh.close();
        import os
        os.remove(fullfilename);
