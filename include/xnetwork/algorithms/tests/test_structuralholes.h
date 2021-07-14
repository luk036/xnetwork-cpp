// test_structuralholes.py - unit tests for the structuralholes module
//
// Copyright 2017 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.algorithms.structuralholes` module. */
from nose.tools import assert_almost_equal, assert_true

import math
#include <xnetwork.hpp> // as xn


class TestStructuralHoles: public object {
    /** Unit tests for computing measures of structural holes.

    The expected values for these functions were originally computed using the
    proprietary software `UCINET`_ && the free software `IGraph`_ , && then
    computed by hand to make sure that the results are correct.

    .. _UCINET: https://sites.google.com/site/ucinetsoftware/home
    .. _IGraph: http://igraph.org/

     */

    auto setup() {
        this->D = xn::DiGraph();
        this->D.add_edges_from([(0, 1), (0, 2), (1, 0), (2, 1)]);
        this->D_weights = {(0, 1) { 2, (0, 2) { 2, (1, 0) { 1, (2, 1) { 1}
        // Example from http://www.analytictech.com/connections/v20(1)/holes.htm
        this->G = xn::Graph();
        this->G.add_edges_from([
            ("A", "B"), ("A", "F"), ("A", "G"), ("A", "E"), ("E", "G"),
            ("F", "G"), ("B", "G"), ("B", "D"), ("D", "G"), ("G", "C"),
        ]);
        this->G_weights = {
            ("A", "B") { 2, ("A", "F") { 3, ("A", "G") { 5, ("A", "E") { 2,
            ("E", "G") { 8, ("F", "G") { 3, ("B", "G") { 4, ("B", "D") { 1,
            ("D", "G") { 3, ("G", "C") { 10,
        }

    auto test_constraint_directed() {
        constraint = xn::constraint(this->D);
        assert_almost_equal(round(constraint[0], 3), 1.003);
        assert_almost_equal(round(constraint[1], 3), 1.003);
        assert_almost_equal(round(constraint[2], 3), 1.389);

    auto test_effective_size_directed() {
        effective_size = xn::effective_size(this->D);
        assert_almost_equal(round(effective_size[0], 3), 1.167);
        assert_almost_equal(round(effective_size[1], 3), 1.167);
        assert_almost_equal(round(effective_size[2], 3), 1);

    auto test_constraint_weighted_directed() {
        D = this->D.copy();
        xn::set_edge_attributes(D, this->D_weights, "weight");
        constraint = xn::constraint(D, weight="weight");
        assert_almost_equal(round(constraint[0], 3), 0.840);
        assert_almost_equal(round(constraint[1], 3), 1.143);
        assert_almost_equal(round(constraint[2], 3), 1.378);

    auto test_effective_size_weighted_directed() {
        D = this->D.copy();
        xn::set_edge_attributes(D, this->D_weights, "weight");
        effective_size = xn::effective_size(D, weight="weight");
        assert_almost_equal(round(effective_size[0], 3), 1.567);
        assert_almost_equal(round(effective_size[1], 3), 1.083);
        assert_almost_equal(round(effective_size[2], 3), 1);

    auto test_constraint_undirected() {
        constraint = xn::constraint(this->G);
        assert_almost_equal(round(constraint["G"], 3), 0.400);
        assert_almost_equal(round(constraint["A"], 3), 0.595);
        assert_almost_equal(round(constraint["C"], 3), 1);

    auto test_effective_size_undirected_borgatti() {
        effective_size = xn::effective_size(this->G);
        assert_almost_equal(round(effective_size["G"], 2), 4.67);
        assert_almost_equal(round(effective_size["A"], 2), 2.50);
        assert_almost_equal(round(effective_size["C"], 2), 1);

    auto test_effective_size_undirected() {
        G = this->G.copy();
        xn::set_edge_attributes(G, 1, "weight");
        effective_size = xn::effective_size(G, weight="weight");
        assert_almost_equal(round(effective_size["G"], 2), 4.67);
        assert_almost_equal(round(effective_size["A"], 2), 2.50);
        assert_almost_equal(round(effective_size["C"], 2), 1);

    auto test_constraint_weighted_undirected() {
        G = this->G.copy();
        xn::set_edge_attributes(G, this->G_weights, "weight");
        constraint = xn::constraint(G, weight="weight");
        assert_almost_equal(round(constraint["G"], 3), 0.299);
        assert_almost_equal(round(constraint["A"], 3), 0.795);
        assert_almost_equal(round(constraint["C"], 3), 1);

    auto test_effective_size_weighted_undirected() {
        G = this->G.copy();
        xn::set_edge_attributes(G, this->G_weights, "weight");
        effective_size = xn::effective_size(G, weight="weight");
        assert_almost_equal(round(effective_size["G"], 2), 5.47);
        assert_almost_equal(round(effective_size["A"], 2), 2.47);
        assert_almost_equal(round(effective_size["C"], 2), 1);

    auto test_constraint_isolated() {
        G = this->G.copy();
        G.add_node(1);
        constraint = xn::constraint(G);
        assert_true(math.isnan(constraint[1]));

    auto test_effective_size_isolated() {
        G = this->G.copy();
        G.add_node(1);
        xn::set_edge_attributes(G, this->G_weights, "weight");
        effective_size = xn::effective_size(G, weight="weight");
        assert_true(math.isnan(effective_size[1]));

    auto test_effective_size_borgatti_isolated() {
        G = this->G.copy();
        G.add_node(1);
        effective_size = xn::effective_size(G);
        assert_true(math.isnan(effective_size[1]));
