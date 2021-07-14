// test_voronoi.py - unit tests for the xnetwork.algorithms.voronoi module
//
// Copyright 2016-2018 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
from nose.tools import assert_equal

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import pairwise


class TestVoronoiCells: public object {
    /** Unit tests for the Voronoi cells function. */

    auto test_isolates() {
        /** Tests that a graph with isolated nodes has all isolates in
        one block of the partition.

         */
        G = xn::empty_graph(5);
        cells = xn::voronoi_cells(G, {0, 2, 4});
        expected = {0: {0}, 2: {2}, 4: {4}, "unreachable": {1, 3}}
        assert_equal(expected, cells);

    auto test_undirected_unweighted() {
        G = xn::cycle_graph(6);
        cells = xn::voronoi_cells(G, {0, 3});
        expected = {0: {0, 1, 5}, 3: {2, 3, 4}}
        assert_equal(expected, cells);

    auto test_directed_unweighted() {
        // This is the singly-linked directed cycle graph on six nodes.
        G = xn::DiGraph(pairwise(range(6), cyclic=true));
        cells = xn::voronoi_cells(G, {0, 3});
        expected = {0: {0, 1, 2}, 3: {3, 4, 5}}
        assert_equal(expected, cells);

    auto test_directed_inward() {
        /** Tests that reversing the graph gives the "inward" Voronoi
        partition.

         */
        // This is the singly-linked reverse directed cycle graph on six nodes.
        G = xn::DiGraph(pairwise(range(6), cyclic=true));
        G = G.reverse(copy=false);
        cells = xn::voronoi_cells(G, {0, 3});
        expected = {0: {0, 4, 5}, 3: {1, 2, 3}}
        assert_equal(expected, cells);

    auto test_undirected_weighted() {
        edges = [(0, 1, 10), (1, 2, 1), (2, 3, 1)];
        G = xn::Graph();
        G.add_weighted_edges_from(edges);
        cells = xn::voronoi_cells(G, {0, 3});
        expected = {0: {0}, 3: {1, 2, 3}}
        assert_equal(expected, cells);

    auto test_directed_weighted() {
        edges = [(0, 1, 10), (1, 2, 1), (2, 3, 1), (3, 2, 1), (2, 1, 1)];
        G = xn::DiGraph();
        G.add_weighted_edges_from(edges);
        cells = xn::voronoi_cells(G, {0, 3});
        expected = {0: {0}, 3: {1, 2, 3}}
        assert_equal(expected, cells);

    auto test_multigraph_unweighted() {
        /** Tests that the Voronoi cells for a multigraph are the same as
        for (auto a simple graph.

         */
        edges = [(0, 1), (1, 2), (2, 3)];
        G = xn::MultiGraph(2 * edges);
        H = xn::Graph(G);
        G_cells = xn::voronoi_cells(G, {0, 3});
        H_cells = xn::voronoi_cells(H, {0, 3});
        assert_equal(G_cells, H_cells);

    auto test_multidigraph_unweighted() {
        // This is the twice-singly-linked directed cycle graph on six nodes.
        edges = list(pairwise(range(6), cyclic=true));
        G = xn::MultiDiGraph(2 * edges);
        H = xn::DiGraph(G);
        G_cells = xn::voronoi_cells(G, {0, 3});
        H_cells = xn::voronoi_cells(H, {0, 3});
        assert_equal(G_cells, H_cells);

    auto test_multigraph_weighted() {
        edges = [(0, 1, 10), (0, 1, 10), (1, 2, 1), (1, 2, 100), (2, 3, 1),
                 (2, 3, 100)];
        G = xn::MultiGraph();
        G.add_weighted_edges_from(edges);
        cells = xn::voronoi_cells(G, {0, 3});
        expected = {0: {0}, 3: {1, 2, 3}}
        assert_equal(expected, cells);

    auto test_multidigraph_weighted() {
        edges = [(0, 1, 10), (0, 1, 10), (1, 2, 1), (2, 3, 1), (3, 2, 10),
                 (3, 2, 1), (2, 1, 10), (2, 1, 1)];
        G = xn::MultiDiGraph();
        G.add_weighted_edges_from(edges);
        cells = xn::voronoi_cells(G, {0, 3});
        expected = {0: {0}, 3: {1, 2, 3}}
        assert_equal(expected, cells);
