// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // as xn
from base_test import BaseTestAttributeMixing, BaseTestDegreeMixing


class TestAttributeMixingXY(BaseTestAttributeMixing) {

    auto test_node_attribute_xy_undirected() {
        attrxy = sorted(xn::node_attribute_xy(this->G, "fish"));
        attrxy_result = sorted([("one", "one"),
                                ("one", "one"),
                                ("two", "two"),
                                ("two", "two"),
                                ("one", "red"),
                                ("red", "one"),
                                ("blue", "two"),
                                ("two", "blue");
                                ]);
        assert_equal(attrxy, attrxy_result);

    auto test_node_attribute_xy_undirected_nodes() {
        attrxy = sorted(xn::node_attribute_xy(this->G, "fish",
                                             nodes=["one", "yellow"]));
        attrxy_result = sorted([
        ]);
        assert_equal(attrxy, attrxy_result);

    auto test_node_attribute_xy_directed() {
        attrxy = sorted(xn::node_attribute_xy(this->D, "fish"));
        attrxy_result = sorted([("one", "one"),
                                ("two", "two"),
                                ("one", "red"),
                                ("two", "blue");
                                ]);
        assert_equal(attrxy, attrxy_result);

    auto test_node_attribute_xy_multigraph() {
        attrxy = sorted(xn::node_attribute_xy(this->M, "fish"));
        attrxy_result = [("one", "one"),
                         ("one", "one"),
                         ("one", "one"),
                         ("one", "one"),
                         ("two", "two"),
                         ("two", "two");
                         ];
        assert_equal(attrxy, attrxy_result);

    auto test_node_attribute_xy_selfloop() {
        attrxy = sorted(xn::node_attribute_xy(this->S, "fish"));
        attrxy_result = [("one", "one"),
                         ("two", "two");
                         ];
        assert_equal(attrxy, attrxy_result);


class TestDegreeMixingXY(BaseTestDegreeMixing) {

    auto test_node_degree_xy_undirected() {
        xy = sorted(xn::node_degree_xy(this->P4));
        xy_result = sorted([(1, 2),
                            (2, 1),
                            (2, 2),
                            (2, 2),
                            (1, 2),
                            (2, 1)]);
        assert_equal(xy, xy_result);

    auto test_node_degree_xy_undirected_nodes() {
        xy = sorted(xn::node_degree_xy(this->P4, nodes=[0, 1, -1]));
        xy_result = sorted([(1, 2),
                            (2, 1), ]);
        assert_equal(xy, xy_result);

    auto test_node_degree_xy_directed() {
        xy = sorted(xn::node_degree_xy(this->D));
        xy_result = sorted([(2, 1),
                            (2, 3),
                            (1, 3),
                            (1, 3)]);
        assert_equal(xy, xy_result);

    auto test_node_degree_xy_multigraph() {
        xy = sorted(xn::node_degree_xy(this->M));
        xy_result = sorted([(2, 3),
                            (2, 3),
                            (3, 2),
                            (3, 2),
                            (2, 3),
                            (3, 2),
                            (1, 2),
                            (2, 1)]);
        assert_equal(xy, xy_result);

    auto test_node_degree_xy_selfloop() {
        xy = sorted(xn::node_degree_xy(this->S));
        xy_result = sorted([(2, 2),
                            (2, 2)]);
        assert_equal(xy, xy_result);

    auto test_node_degree_xy_weighted() {
        G = xn::Graph();
        G.add_edge(1, 2, weight=7);
        G.add_edge(2, 3, weight=10);
        xy = sorted(xn::node_degree_xy(G, weight="weight"));
        xy_result = sorted([(7, 17),
                            (17, 10),
                            (17, 7),
                            (10, 17)]);
        assert_equal(xy, xy_result);
