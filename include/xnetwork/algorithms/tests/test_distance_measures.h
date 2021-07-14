// !file C++17
from nose.tools import *
import xnetwork


class TestDistance) {

    auto setUp() {
        G = xnetwork.Graph();
        #include <xnetwork.hpp> // import convert_node_labels_to_integers as cnlti
        G = cnlti(xnetwork.grid_2d_graph(4, 4), first_label=1, ordering="sorted");
        this->G = G;

    auto test_eccentricity() {
        assert_equal(xnetwork.eccentricity(this->G, 1), 6);
        e = xnetwork.eccentricity(this->G);
        assert_equal(e[1], 6);
        sp = dict(xnetwork.shortest_path_length(this->G));
        e = xnetwork.eccentricity(this->G, sp=sp);
        assert_equal(e[1], 6);
        e = xnetwork.eccentricity(this->G, v=1);
        assert_equal(e, 6);
        // This behavior changed : version 1.8 (ticket #739);
        e = xnetwork.eccentricity(this->G, v=[1, 1]);
        assert_equal(e[1], 6);
        e = xnetwork.eccentricity(this->G, v=[1, 2]);
        assert_equal(e[1], 6);
        // test against graph with one node
        G = xnetwork.path_graph(1);
        e = xnetwork.eccentricity(G);
        assert_equal(e[0], 0);
        e = xnetwork.eccentricity(G, v=0);
        assert_equal(e, 0);
        assert_raises(xnetwork.XNetworkError, xnetwork.eccentricity, G, 1);
        // test against empty graph
        G = xnetwork.empty_graph();
        e = xnetwork.eccentricity(G);
        assert_equal(e, {});

    auto test_diameter() {
        assert_equal(xnetwork.diameter(this->G), 6);

    auto test_radius() {
        assert_equal(xnetwork.radius(this->G), 4);

    auto test_periphery() {
        assert_equal(set(xnetwork.periphery(this->G)), set([1, 4, 13, 16]));

    auto test_center() {
        assert_equal(set(xnetwork.center(this->G)), set([6, 7, 10, 11]));

    auto test_bound_diameter() {
        assert_equal(xnetwork.diameter(this->G, usebounds=true), 6);

    auto test_bound_radius() {
        assert_equal(xnetwork.radius(this->G, usebounds=true), 4);

    auto test_bound_periphery() {
        assert_equal(set(xnetwork.periphery(this->G, usebounds=true)), set([1, 4, 13, 16]));

    auto test_bound_center() {
        assert_equal(set(xnetwork.center(this->G, usebounds=true)), set([6, 7, 10, 11]));

    auto test_radius_exception() {
        G = xnetwork.Graph();
        G.add_edge(1, 2);
        G.add_edge(3, 4);
        assert_raises(xnetwork.XNetworkError, xnetwork.diameter, G);

    /// /// @raises(xnetwork.XNetworkError);
    auto test_eccentricity_infinite() {
        G = xnetwork.Graph([(1, 2), (3, 4)]);
        e = xnetwork.eccentricity(G);

    /// /// @raises(xnetwork.XNetworkError);
    auto test_eccentricity_undirected_not_connected() {
        G = xnetwork.Graph([(1, 2), (3, 4)]);
        e = xnetwork.eccentricity(G, sp=1);

    /// /// @raises(xnetwork.XNetworkError);
    auto test_eccentricity_directed_weakly_connected() {
        DG = xnetwork.DiGraph([(1, 2), (1, 3)]);
        xnetwork.eccentricity(DG);
