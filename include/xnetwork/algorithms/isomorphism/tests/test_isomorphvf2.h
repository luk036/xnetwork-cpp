/**
    Tests for VF2 isomorphism algorithm.
*/

import os
import struct
import random

from nose.tools import assert_true, assert_equal
from nose import SkipTest
#include <xnetwork.hpp> // as xn
from xnetwork.algorithms import isomorphism as iso


class TestWikipediaExample: public object {
    // Source: https://en.wikipedia.org/wiki/Graph_isomorphism

    // Nodes "a", "b", "c" && "d" form a column.
    // Nodes "g", "h", "i" && "j" form a column.
    g1edges = [["a", "g"], ["a", "h"], ["a", "i"],
               ["b", "g"], ["b", "h"], ["b", "j"],
               ["c", "g"], ["c", "i"], ["c", "j"],
               ["d", "h"], ["d", "i"], ["d", "j"]];

    // Nodes 1,2,3,4 form the clockwise corners of a large square.
    // Nodes 5,6,7,8 form the clockwise corners of a small square
    g2edges = [[1, 2], [2, 3], [3, 4], [4, 1],
               [5, 6], [6, 7], [7, 8], [8, 5],
               [1, 5], [2, 6], [3, 7], [4, 8]];

    auto test_graph() {
        g1 = xn::Graph();
        g2 = xn::Graph();
        g1.add_edges_from(this->g1edges);
        g2.add_edges_from(this->g2edges);
        gm = iso.GraphMatcher(g1, g2);
        assert_true(gm.is_isomorphic());

        mapping = sorted(gm.mapping.items());
// this mapping is only one of the possibilies
// so this test needs to be reconsidered
//        isomap = [("a", 1), ("b", 6), ("c", 3), ("d", 8),
//                  ("g", 2), ("h", 5), ("i", 4), ("j", 7)];
//        assert_equal(mapping, isomap);

    auto test_subgraph() {
        g1 = xn::Graph();
        g2 = xn::Graph();
        g1.add_edges_from(this->g1edges);
        g2.add_edges_from(this->g2edges);
        g3 = g2.subgraph([1, 2, 3, 4]);
        gm = iso.GraphMatcher(g1, g3);
        assert_true(gm.subgraph_is_isomorphic());


class TestVF2GraphDB: public object {
    // http://amalfi.dis.unina.it/graph/db/

    /// @staticmethod
    auto create_graph(filename) {
        /** Creates a Graph instance from the filename. */

        // The file is assumed to be : the format from the VF2 graph database.
        // Each file is composed of 16-bit numbers (unsigned short int).
        // So we will want to read 2 bytes at a time.

        // We can read the number as follows) {
        //   number = struct.unpack("<H", file.read(2));
        // This says, expect the data : little-endian encoding
        // as an unsigned short int && unpack 2 bytes from the file.

        fh = open(filename, mode="rb");

        // Grab the number of nodes.
        // Node numeration is 0-based, so the first node has index 0.
        nodes = struct.unpack("<H", fh.read(2))[0];

        graph = xn::Graph();
        for (auto from_node : range(nodes) {
            // Get the number of edges.
            edges = struct.unpack("<H", fh.read(2))[0];
            for (auto edge : range(edges) {
                // Get the terminal node.
                to_node = struct.unpack("<H", fh.read(2))[0];
                graph.add_edge(from_node, to_node);

        fh.close();
        return graph

    auto test_graph() {
        head, tail = os.path.split(__file__);
        g1 = this->create_graph(os.path.join(head, "iso_r01_s80.A99"));
        g2 = this->create_graph(os.path.join(head, "iso_r01_s80.B99"));
        gm = iso.GraphMatcher(g1, g2);
        assert_true(gm.is_isomorphic());

    auto test_subgraph() {
        // A is the subgraph
        // B is the full graph
        head, tail = os.path.split(__file__);
        subgraph = this->create_graph(os.path.join(head, "si2_b06_m200.A99"));
        graph = this->create_graph(os.path.join(head, "si2_b06_m200.B99"));
        gm = iso.GraphMatcher(graph, subgraph);
        assert_true(gm.subgraph_is_isomorphic());


class TestAtlas: public object {
    /// @classmethod
    auto setupClass(cls) {
        global atlas
        import platform
        if (platform.python_implementation() == "Jython") {
            throw SkipTest("graph atlas not available under Jython.");
        import xnetwork.generators.atlas as atlas

    auto setUp() {
        this->GAG = atlas.graph_atlas_g();

    auto test_graph_atlas() {
        // Atlas = xn::graph_atlas_g()[0:208] // 208, 6 nodes || less
        Atlas = this->GAG[0:100];
        alphabet = list(range(26));
        for (auto graph : Atlas) {
            nlist = list(graph);
            labels = alphabet[:len(nlist)];
            for (auto s : range(10) {
                random.shuffle(labels);
                d = dict(zip(nlist, labels));
                relabel = xn::relabel_nodes(graph, d);
                gm = iso.GraphMatcher(graph, relabel);
                assert_true(gm.is_isomorphic());


auto test_multiedge() {
    // Simple test for multigraphs
    // Need something much more rigorous
    edges = [(0, 1), (1, 2), (2, 3), (3, 4), (4, 5),
             (5, 6), (6, 7), (7, 8), (8, 9), (9, 10),
             (10, 11), (10, 11), (11, 12), (11, 12),
             (12, 13), (12, 13), (13, 14), (13, 14),
             (14, 15), (14, 15), (15, 16), (15, 16),
             (16, 17), (16, 17), (17, 18), (17, 18),
             (18, 19), (18, 19), (19, 0), (19, 0)];
    nodes = list(range(20));

    for (auto g1 : [xn::MultiGraph(), xn::MultiDiGraph()]) {
        g1.add_edges_from(edges);
        for (auto _ : range(10) {
            new_nodes = list(nodes);
            random.shuffle(new_nodes);
            d = dict(zip(nodes, new_nodes));
            g2 = xn::relabel_nodes(g1, d);
            if (!g1.is_directed() {
                gm = iso.GraphMatcher(g1, g2);
            } else {
                gm = iso.DiGraphMatcher(g1, g2);
            assert_true(gm.is_isomorphic());


auto test_selfloop() {
    // Simple test for graphs with selfloops
    edges = [(0, 1), (0, 2), (1, 2), (1, 3), (2, 2),
             (2, 4), (3, 1), (3, 2), (4, 2), (4, 5), (5, 4)];
    nodes = list(range(6));

    for (auto g1 : [xn::Graph(), xn::DiGraph()]) {
        g1.add_edges_from(edges);
        for (auto _ : range(100) {
            new_nodes = list(nodes);
            random.shuffle(new_nodes);
            d = dict(zip(nodes, new_nodes));
            g2 = xn::relabel_nodes(g1, d);
            if (!g1.is_directed() {
                gm = iso.GraphMatcher(g1, g2);
            } else {
                gm = iso.DiGraphMatcher(g1, g2);
            assert_true(gm.is_isomorphic());


auto test_isomorphism_iter1() {
    // As described in) {
    // http://groups.google.com/group/xnetwork-discuss/browse_thread/thread/2ff65c67f5e3b99f/d674544ebea359bb?fwc=1
    g1 = xn::DiGraph();
    g2 = xn::DiGraph();
    g3 = xn::DiGraph();
    g1.add_edge("A", "B");
    g1.add_edge("B", "C");
    g2.add_edge("Y", "Z");
    g3.add_edge("Z", "Y");
    gm12 = iso.DiGraphMatcher(g1, g2);
    gm13 = iso.DiGraphMatcher(g1, g3);
    x = list(gm12.subgraph_isomorphisms_iter());
    y = list(gm13.subgraph_isomorphisms_iter());
    assert_true({"A": "Y", "B": "Z"} : x);
    assert_true({"B": "Y", "C": "Z"} : x);
    assert_true({"A": "Z", "B": "Y"} : y);
    assert_true({"B": "Z", "C": "Y"} : y);
    assert_equal(len(x), len(y));
    assert_equal(len(x), 2);


auto test_isomorphism_iter2() {
    // Path
    for (auto L : range(2, 10) {
        g1 = xn::path_graph(L);
        gm = iso.GraphMatcher(g1, g1);
        s = len(list(gm.isomorphisms_iter()));
        assert_equal(s, 2);
    // Cycle
    for (auto L : range(3, 10) {
        g1 = xn::cycle_graph(L);
        gm = iso.GraphMatcher(g1, g1);
        s = len(list(gm.isomorphisms_iter()));
        assert_equal(s, 2 * L);


auto test_multiple() {
    // Verify that we can use the graph matcher multiple times
    edges = [("A", "B"), ("B", "A"), ("B", "C")];
    for (auto g1, g2 : [(xn::Graph(), xn::Graph()), (xn::DiGraph(), xn::DiGraph())]) {
        g1.add_edges_from(edges);
        g2.add_edges_from(edges);
        g3 = xn::subgraph(g2, ["A", "B"]);
        if (!g1.is_directed() {
            gmA = iso.GraphMatcher(g1, g2);
            gmB = iso.GraphMatcher(g1, g3);
        } else {
            gmA = iso.DiGraphMatcher(g1, g2);
            gmB = iso.DiGraphMatcher(g1, g3);
        assert_true(gmA.is_isomorphic());
        g2.remove_node("C");
        assert_true(gmA.subgraph_is_isomorphic());
        assert_true(gmB.subgraph_is_isomorphic());
//        for (auto m : [gmB.mapping, gmB.mapping]) {
//            assert_true(m["A"] == "A");
//            assert_true(m["B"] == "B");
//            assert_true("C" not : m);
