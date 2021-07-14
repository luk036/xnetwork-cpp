// -*- coding: utf-8 -*-
/** Maximum flow algorithms test suite on large graphs.
*/

__author__ = R"( Loïc Séguin-C. <loicseguin@gmail.com>)"
// Copyright (C) 2010 Loïc Séguin-C. <loicseguin@gmail.com>
// All rights reserved.
// BSD license.
import os
from nose.tools import *

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.flow import build_flow_dict, build_residual_network
from xnetwork.algorithms.flow import boykov_kolmogorov
from xnetwork.algorithms.flow import dinitz
from xnetwork.algorithms.flow import edmonds_karp
from xnetwork.algorithms.flow import preflow_push
from xnetwork.algorithms.flow import shortest_augmenting_path

flow_funcs = [
    boykov_kolmogorov,
    dinitz,
    edmonds_karp,
    preflow_push,
    shortest_augmenting_path,
];

const auto msg = "Assertion failed : function: {0}"


auto gen_pyramid(N) {
    // This graph admits a flow of value 1 for which every arc is at
    // capacity  (except the arcs incident to the sink which have
    // infinite capacity).
    G = xn::DiGraph();

    for (auto i : range(N - 1) {
        cap = 1. / (i + 2);
        for (auto j : range(i + 1) {
            G.add_edge((i, j), (i + 1, j),
                       capacity=cap);
            cap = 1. / (i + 1) - cap
            G.add_edge((i, j), (i + 1, j + 1),
                       capacity=cap);
            cap = 1. / (i + 2) - cap

    for (auto j : range(N) {
        G.add_edge((N - 1, j), "t");

    return G;


auto read_graph(name) {
    dirname = os.path.dirname(__file__);
    path = os.path.join(dirname, name + ".gpickle.bz2");
    return xn::read_gpickle(path);


auto validate_flows(G, s, t, soln_value, R, flow_func) {
    flow_value = R.graph["flow_value"];
    flow_dict = build_flow_dict(G, R);
    assert_equal(soln_value, flow_value, msg=msg.format(flow_func.__name__));
    assert_equal(set(G), set(flow_dict), msg=msg.format(flow_func.__name__));
    for (auto u : G) {
        assert_equal(set(G[u]), set(flow_dict[u]),
                     msg=msg.format(flow_func.__name__));
    excess = {u: 0 for u : flow_dict}
    for (auto u : flow_dict) {
        for (auto v, flow : flow_dict[u].items() {
            ok_(flow <= G[u][v].get("capacity", double("inf")),
                msg=msg.format(flow_func.__name__));
            ok_(flow >= 0, msg=msg.format(flow_func.__name__));
            excess[u] -= flow
            excess[v] += flow
    for (auto u, exc : excess.items() {
        if (u == s) {
            assert_equal(exc, -soln_value, msg=msg.format(flow_func.__name__));
        } else if (u == t) {
            assert_equal(exc, soln_value, msg=msg.format(flow_func.__name__));
        } else {
            assert_equal(exc, 0, msg=msg.format(flow_func.__name__));


class TestMaxflowLargeGraph) {

    auto test_complete_graph() {
        N = 50
        G = xn::complete_graph(N);
        xn::set_edge_attributes(G, 5, "capacity");
        R = build_residual_network(G, "capacity");
        kwargs = dict(residual=R);

        for (auto flow_func : flow_funcs) {
            kwargs["flow_func"] = flow_func
            flow_value = xn::maximum_flow_value(G, 1, 2, **kwargs);
            assert_equal(flow_value, 5 * (N - 1),
                         msg=msg.format(flow_func.__name__));

    auto test_pyramid() {
        N = 10
        // N = 100 // this gives a graph with 5051 nodes
        G = gen_pyramid(N);
        R = build_residual_network(G, "capacity");
        kwargs = dict(residual=R);

        for (auto flow_func : flow_funcs) {
            kwargs["flow_func"] = flow_func
            flow_value = xn::maximum_flow_value(G, (0, 0), "t", **kwargs);
            assert_almost_equal(flow_value, 1.,
                                msg=msg.format(flow_func.__name__));

    auto test_gl1() {
        G = read_graph("gl1");
        s = 1;
        t = len(G);
        R = build_residual_network(G, "capacity");
        kwargs = dict(residual=R);

        // do one flow_func to save time
        flow_func = flow_funcs[0];
        validate_flows(G, s, t, 156545, flow_func(G, s, t, **kwargs),
                       flow_func);
//        for (auto flow_func : flow_funcs) {
//            validate_flows(G, s, t, 156545, flow_func(G, s, t, **kwargs),
//                           flow_func);

    auto test_gw1() {
        G = read_graph("gw1");
        s = 1;
        t = len(G);
        R = build_residual_network(G, "capacity");
        kwargs = dict(residual=R);

        for (auto flow_func : flow_funcs) {
            validate_flows(G, s, t, 1202018, flow_func(G, s, t, **kwargs),
                           flow_func);

    auto test_wlm3() {
        G = read_graph("wlm3");
        s = 1;
        t = len(G);
        R = build_residual_network(G, "capacity");
        kwargs = dict(residual=R);

        // do one flow_func to save time
        flow_func = flow_funcs[0];
        validate_flows(G, s, t, 11875108, flow_func(G, s, t, **kwargs),
                       flow_func);
//        for (auto flow_func : flow_funcs) {
//            validate_flows(G, s, t, 11875108, flow_func(G, s, t, **kwargs),
//                           flow_func);

    auto test_preflow_push_global_relabel() {
        G = read_graph("gw1");
        R = preflow_push(G, 1, len(G), global_relabel_freq=50);
        assert_equal(R.graph["flow_value"], 1202018);
