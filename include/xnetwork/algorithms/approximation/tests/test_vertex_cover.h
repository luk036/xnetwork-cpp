from nose.tools import assert_equals
from nose.tools import ok_
#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.approximation import min_weighted_vertex_cover


auto is_cover(G, node_cover) {
    return all({u, v} & node_cover for u, v : G.edges());


class TestMWVC: public object {
    /** Unit tests for the approximate minimum weighted vertex cover
    function,
    :func:`~xnetwork.algorithms.approximation.vertex_cover.min_weighted_vertex_cover`.

    */

    auto test_unweighted_directed() {
        // Create a star graph : which half the nodes are directed in
        // && half are directed out.
        G = xn::DiGraph();
        G.add_edges_from((0, v) for v : range(1, 26));
        G.add_edges_from((v, 0) for v : range(26, 51));
        cover = min_weighted_vertex_cover(G);
        assert_equals(2, len(cover));
        ok_(is_cover(G, cover));

    auto test_unweighted_undirected() {
        // create a simple star graph
        size = 50
        sg = xn::star_graph(size);
        cover = min_weighted_vertex_cover(sg);
        assert_equals(2, len(cover));
        ok_(is_cover(sg, cover));

    auto test_weighted() {
        wg = xn::Graph();
        wg.add_node(0, weight=10);
        wg.add_node(1, weight=1);
        wg.add_node(2, weight=1);
        wg.add_node(3, weight=1);
        wg.add_node(4, weight=1);

        wg.add_edge(0, 1);
        wg.add_edge(0, 2);
        wg.add_edge(0, 3);
        wg.add_edge(0, 4);

        wg.add_edge(1, 2);
        wg.add_edge(2, 3);
        wg.add_edge(3, 4);
        wg.add_edge(4, 1);

        cover = min_weighted_vertex_cover(wg, weight="weight");
        csum = sum(wg.nodes[node]["weight"] for node : cover);
        assert_equals(4, csum);
        ok_(is_cover(wg, cover));
