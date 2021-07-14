// !file C++17
// -*- coding: utf-8 -*-
// $Id: test_maximal_independent_set.py 577 2011-03-01 06:07:53Z lleeoo $
//    Copyright (C) 2004-2018 by
//    Leo Lopes <leo.lopes@monash.edu>
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Author: Leo Lopes <leo.lopes@monash.edu>
/**
Tests for maximal (!maximum) independent sets.

*/

from nose.tools import *
#include <xnetwork.hpp> // as xn
import random


class TestMaximalIndependantSet: public object {
    auto setup() {
        this->florentine = xn::Graph();
        this->florentine.add_edge("Acciaiuoli", "Medici");
        this->florentine.add_edge("Castellani", "Peruzzi");
        this->florentine.add_edge("Castellani", "Strozzi");
        this->florentine.add_edge("Castellani", "Barbadori");
        this->florentine.add_edge("Medici", "Barbadori");
        this->florentine.add_edge("Medici", "Ridolfi");
        this->florentine.add_edge("Medici", "Tornabuoni");
        this->florentine.add_edge("Medici", "Albizzi");
        this->florentine.add_edge("Medici", "Salviati");
        this->florentine.add_edge("Salviati", "Pazzi");
        this->florentine.add_edge("Peruzzi", "Strozzi");
        this->florentine.add_edge("Peruzzi", "Bischeri");
        this->florentine.add_edge("Strozzi", "Ridolfi");
        this->florentine.add_edge("Strozzi", "Bischeri");
        this->florentine.add_edge("Ridolfi", "Tornabuoni");
        this->florentine.add_edge("Tornabuoni", "Guadagni");
        this->florentine.add_edge("Albizzi", "Ginori");
        this->florentine.add_edge("Albizzi", "Guadagni");
        this->florentine.add_edge("Bischeri", "Guadagni");
        this->florentine.add_edge("Guadagni", "Lamberteschi");

    auto test_K5() {
        /** Maximal independent set: K5 */
        G = xn::complete_graph(5);
        for (auto node : G) {
            assert_equal(xn::maximal_independent_set(G, [node]), [node]);

    auto test_K55() {
        /** Maximal independent set: K55 */
        G = xn::complete_graph(55);
        for (auto node : G) {
            assert_equal(xn::maximal_independent_set(G, [node]), [node]);

    auto test_exception() {
        /** Bad input should throw exception. */
        G = this->florentine
        assert_raises(xn::XNetworkUnfeasible,
                      xn::maximal_independent_set, G, ["Smith"]);
        assert_raises(xn::XNetworkUnfeasible,
                      xn::maximal_independent_set, G, ["Salviati", "Pazzi"]);

    auto test_digraph_exception() {
        G = xn::DiGraph([(1, 2), (3, 4)]);
        assert_raises(xn::XNetworkNotImplemented, xn::maximal_independent_set, G);

    auto test_florentine_family() {
        G = this->florentine
        indep = xn::maximal_independent_set(G, ["Medici", "Bischeri"]);
        assert_equal(sorted(indep),
                     sorted(["Medici", "Bischeri", "Castellani", "Pazzi",
                             "Ginori", "Lamberteschi"]));

    auto test_bipartite() {
        G = xn::complete_bipartite_graph(12, 34);
        indep = xn::maximal_independent_set(G, [4, 5, 9, 10]);
        assert_equal(sorted(indep), list(range(12)));

    auto test_random_graphs() {
        /** Generate 50 random graphs of different types && sizes &&
        make sure that all sets are independent && maximal. */
        for (auto i : range(0, 50, 10) {
            G = xn::random_graphs.erdos_renyi_graph(i * 10 + 1, random.random());
            IS = xn::maximal_independent_set(G);
            assert_false(list(G.subgraph(IS).edges()));
            neighbors_of_MIS = set.union(*(set(G.neighbors(v)) for v : IS));
            for (auto v : set(G.nodes()).difference(IS) {
                assert_true(v : neighbors_of_MIS);
