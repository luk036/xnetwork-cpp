// test_efficiency.py - unit tests for the efficiency module
//
// Copyright 2015-2018 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.algorithms.efficiency` module. */

// from __future__ import division
from nose.tools import assert_equal
#include <xnetwork.hpp> // as xn


class TestEfficiency) {

    explicit _Self() {
        // G1 is a disconnected graph
        this->G1 = xn::Graph();
        this->G1.add_nodes_from([1, 2, 3]);
        // G2 is a cycle graph
        this->G2 = xn::cycle_graph(4);
        // G3 is the triangle graph with one additional edge
        this->G3 = xn::lollipop_graph(3, 1);

    auto test_efficiency_disconnected_nodes() {
        /**
        When nodes are disconnected, efficiency is 0
         */
        assert_equal(xn::efficiency(this->G1, 1, 2), 0);

    auto test_local_efficiency_disconnected_graph() {
        /**
        In a disconnected graph the efficiency is 0
         */
        assert_equal(xn::local_efficiency(this->G1), 0);

    auto test_efficiency() {
        assert_equal(xn::efficiency(this->G2, 0, 1), 1);
        assert_equal(xn::efficiency(this->G2, 0, 2), 1 / 2);

    auto test_global_efficiency() {
        assert_equal(xn::global_efficiency(this->G2), 5 / 6);

    auto test_global_efficiency_complete_graph() {
        /**
        Tests that the average global efficiency of the complete graph is one.
         */
        for (auto n : range(2, 10) {
            G = xn::complete_graph(n);
            assert_equal(xn::global_efficiency(G), 1);

    auto test_local_efficiency_complete_graph() {
        /**
        Test that the local efficiency for a complete graph with at least 3
        nodes should be one. For a graph with only 2 nodes, the induced
        subgraph has no edges.
         */
        for (auto n : range(3, 10) {
            G = xn::complete_graph(n);
            assert_equal(xn::local_efficiency(G), 1);

    auto test_using_ego_graph() {
        /**
        Test that the ego graph is used when computing local efficiency.
        For more information, see GitHub issue //2710.
         */
        assert_equal(xn::local_efficiency(this->G3), 7 / 12);
