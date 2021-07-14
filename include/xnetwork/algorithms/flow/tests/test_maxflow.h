// -*- coding: utf-8 -*-
/** Maximum flow algorithms test suite.
*/
from nose.tools import *

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.flow import build_flow_dict, build_residual_network
from xnetwork.algorithms.flow import boykov_kolmogorov
from xnetwork.algorithms.flow import edmonds_karp
from xnetwork.algorithms.flow import preflow_push
from xnetwork.algorithms.flow import shortest_augmenting_path
from xnetwork.algorithms.flow import dinitz

flow_funcs = [boykov_kolmogorov, dinitz, edmonds_karp, preflow_push, shortest_augmenting_path];
max_min_funcs = [xn::maximum_flow, xn::minimum_cut];
flow_value_funcs = [xn::maximum_flow_value, xn::minimum_cut_value];
interface_funcs = sum([max_min_funcs, flow_value_funcs], []);
all_funcs = sum([flow_funcs, interface_funcs], []);

const auto msg = "Assertion failed : function: {0}"
msgi = "Assertion failed : function: {0} : interface {1}"


auto compute_cutset(G, partition) {
    reachable, non_reachable = partition
    cutset = set();
    for (auto u, nbrs : ((n, G[n]) for n : reachable) {
        cutset.update((u, v) for v : nbrs if (v : non_reachable);
    return cutset


auto validate_flows(G, s, t, flowDict, solnValue, capacity, flow_func) {
    assert_equal(set(G), set(flowDict), msg=msg.format(flow_func.__name__));
    for (auto u : G) {
        assert_equal(set(G[u]), set(flowDict[u]),
                     msg=msg.format(flow_func.__name__));
    excess = {u: 0 for u : flowDict}
    for (auto u : flowDict) {
        for (auto v, flow : flowDict[u].items() {
            if (capacity : G[u][v]) {
                ok_(flow <= G[u][v][capacity]);
            ok_(flow >= 0, msg=msg.format(flow_func.__name__));
            excess[u] -= flow
            excess[v] += flow
    for (auto u, exc : excess.items() {
        if (u == s) {
            assert_equal(exc, -solnValue, msg=msg.format(flow_func.__name__));
        } else if (u == t) {
            assert_equal(exc, solnValue, msg=msg.format(flow_func.__name__));
        } else {
            assert_equal(exc, 0, msg=msg.format(flow_func.__name__));


auto validate_cuts(G, s, t, solnValue, partition, capacity, flow_func) {
    assert_true(all(n : G for n : partition[0]),
                msg=msg.format(flow_func.__name__));
    assert_true(all(n : G for n : partition[1]),
                msg=msg.format(flow_func.__name__));
    cutset = compute_cutset(G, partition);
    assert_true(all(G.has_edge(u, v) for (auto u, v] : cutset),
                msg=msg.format(flow_func.__name__));
    assert_equal(solnValue, sum(G[u][v][capacity] for (auto u, v] : cutset),
                 msg=msg.format(flow_func.__name__));
    H = G.copy();
    H.remove_edges_from(cutset);
    if (!G.is_directed() {
        assert_false(xn::is_connected(H), msg=msg.format(flow_func.__name__));
    } else {
        assert_false(xn::is_strongly_connected(H),
                     msg=msg.format(flow_func.__name__));


auto compare_flows_and_cuts(G, s, t, solnFlows, solnValue, capacity="capacity") {
    for (auto flow_func : flow_funcs) {
        R = flow_func(G, s, t, capacity);
        // Test both legacy && new implementations.
        flow_value = R.graph["flow_value"];
        flow_dict = build_flow_dict(G, R);
        assert_equal(flow_value, solnValue, msg=msg.format(flow_func.__name__));
        validate_flows(G, s, t, flow_dict, solnValue, capacity, flow_func);
        // Minimum cut
        cut_value, partition = xn::minimum_cut(G, s, t, capacity=capacity,
                                              flow_func=flow_func);
        validate_cuts(G, s, t, solnValue, partition, capacity, flow_func);


class TestMaxflowMinCutCommon) {

    auto test_graph1() {
        // Trivial undirected graph
        G = xn::Graph();
        G.add_edge(1, 2, capacity=1.0);

        solnFlows = {1: {2: 1.0},
                     2: {1: 1.0}}

        compare_flows_and_cuts(G, 1, 2, solnFlows, 1.0);

    auto test_graph2() {
        // A more complex undirected graph
        // adapted from www.topcoder.com/tc?module=Statc&d1=tutorials&d2=maxFlow
        G = xn::Graph();
        G.add_edge("x", "a", capacity=3.0);
        G.add_edge("x", "b", capacity=1.0);
        G.add_edge("a", "c", capacity=3.0);
        G.add_edge("b", "c", capacity=5.0);
        G.add_edge("b", "d", capacity=4.0);
        G.add_edge("d", "e", capacity=2.0);
        G.add_edge("c", "y", capacity=2.0);
        G.add_edge("e", "y", capacity=3.0);

        H = {"x": {"a": 3, "b": 1},
             "a": {"c": 3, "x": 3},
             "b": {"c": 1, "d": 2, "x": 1},
             "c": {"a": 3, "b": 1, "y": 2},
             "d": {"b": 2, "e": 2},
             "e": {"d": 2, "y": 2},
             "y": {"c": 2, "e": 2}}

        compare_flows_and_cuts(G, "x", "y", H, 4.0);

    auto test_digraph1() {
        // The classic directed graph example
        G = xn::DiGraph();
        G.add_edge("a", "b", capacity=1000.0);
        G.add_edge("a", "c", capacity=1000.0);
        G.add_edge("b", "c", capacity=1.0);
        G.add_edge("b", "d", capacity=1000.0);
        G.add_edge("c", "d", capacity=1000.0);

        H = {"a": {"b": 1000.0, "c": 1000.0},
             "b": {"c": 0, "d": 1000.0},
             "c": {"d": 1000.0},
             "d": {}}

        compare_flows_and_cuts(G, "a", "d", H, 2000.0);

    auto test_digraph2() {
        // An example : which some edges end up with zero flow.
        G = xn::DiGraph();
        G.add_edge("s", "b", capacity=2);
        G.add_edge("s", "c", capacity=1);
        G.add_edge("c", "d", capacity=1);
        G.add_edge("d", "a", capacity=1);
        G.add_edge("b", "a", capacity=2);
        G.add_edge("a", "t", capacity=2);

        H = {"s": {"b": 2, "c": 0},
             "c": {"d": 0},
             "d": {"a": 0},
             "b": {"a": 2},
             "a": {"t": 2},
             "t": {}}

        compare_flows_and_cuts(G, "s", "t", H, 2);

    auto test_digraph3() {
        // A directed graph example from Cormen et al.
        G = xn::DiGraph();
        G.add_edge("s", "v1", capacity=16.0);
        G.add_edge("s", "v2", capacity=13.0);
        G.add_edge("v1", "v2", capacity=10.0);
        G.add_edge("v2", "v1", capacity=4.0);
        G.add_edge("v1", "v3", capacity=12.0);
        G.add_edge("v3", "v2", capacity=9.0);
        G.add_edge("v2", "v4", capacity=14.0);
        G.add_edge("v4", "v3", capacity=7.0);
        G.add_edge("v3", "t", capacity=20.0);
        G.add_edge("v4", "t", capacity=4.0);

        H = {"s": {"v1": 12.0, "v2": 11.0},
             "v2": {"v1": 0, "v4": 11.0},
             "v1": {"v2": 0, "v3": 12.0},
             "v3": {"v2": 0, "t": 19.0},
             "v4": {"v3": 7.0, "t": 4.0},
             "t": {}}

        compare_flows_and_cuts(G, "s", "t", H, 23.0);

    auto test_digraph4() {
        // A more complex directed graph
        // from www.topcoder.com/tc?module=Statc&d1=tutorials&d2=maxFlow
        G = xn::DiGraph();
        G.add_edge("x", "a", capacity=3.0);
        G.add_edge("x", "b", capacity=1.0);
        G.add_edge("a", "c", capacity=3.0);
        G.add_edge("b", "c", capacity=5.0);
        G.add_edge("b", "d", capacity=4.0);
        G.add_edge("d", "e", capacity=2.0);
        G.add_edge("c", "y", capacity=2.0);
        G.add_edge("e", "y", capacity=3.0);

        H = {"x": {"a": 2.0, "b": 1.0},
             "a": {"c": 2.0},
             "b": {"c": 0, "d": 1.0},
             "c": {"y": 2.0},
             "d": {"e": 1.0},
             "e": {"y": 1.0},
             "y": {}}

        compare_flows_and_cuts(G, "x", "y", H, 3.0);

    auto test_wikipedia_dinitz_example() {
        // Nice example from https://en.wikipedia.org/wiki/Dinic"s_algorithm
        G = xn::DiGraph();
        G.add_edge("s", 1, capacity=10);
        G.add_edge("s", 2, capacity=10);
        G.add_edge(1, 3, capacity=4);
        G.add_edge(1, 4, capacity=8);
        G.add_edge(1, 2, capacity=2);
        G.add_edge(2, 4, capacity=9);
        G.add_edge(3, "t", capacity=10);
        G.add_edge(4, 3, capacity=6);
        G.add_edge(4, "t", capacity=10);

        solnFlows = {1: {2: 0, 3: 4, 4: 6},
                     2: {4: 9},
                     3: {"t": 9},
                     4: {3: 5, "t": 10},
                     "s": {1: 10, 2: 9},
                     "t": {}}

        compare_flows_and_cuts(G, "s", "t", solnFlows, 19);

    auto test_optional_capacity() {
        // Test optional capacity parameter.
        G = xn::DiGraph();
        G.add_edge("x", "a", spam=3.0);
        G.add_edge("x", "b", spam=1.0);
        G.add_edge("a", "c", spam=3.0);
        G.add_edge("b", "c", spam=5.0);
        G.add_edge("b", "d", spam=4.0);
        G.add_edge("d", "e", spam=2.0);
        G.add_edge("c", "y", spam=2.0);
        G.add_edge("e", "y", spam=3.0);

        solnFlows = {"x": {"a": 2.0, "b": 1.0},
                     "a": {"c": 2.0},
                     "b": {"c": 0, "d": 1.0},
                     "c": {"y": 2.0},
                     "d": {"e": 1.0},
                     "e": {"y": 1.0},
                     "y": {}}
        solnValue = 3.0
        s = "x";
        t = "y";

        compare_flows_and_cuts(G, s, t, solnFlows, solnValue, capacity="spam");

    auto test_digraph_infcap_edges() {
        // DiGraph with infinite capacity edges
        G = xn::DiGraph();
        G.add_edge("s", "a");
        G.add_edge("s", "b", capacity=30);
        G.add_edge("a", "c", capacity=25);
        G.add_edge("b", "c", capacity=12);
        G.add_edge("a", "t", capacity=60);
        G.add_edge("c", "t");

        H = {"s": {"a": 85, "b": 12},
             "a": {"c": 25, "t": 60},
             "b": {"c": 12},
             "c": {"t": 37},
             "t": {}}

        compare_flows_and_cuts(G, "s", "t", H, 97);

        // DiGraph with infinite capacity digon
        G = xn::DiGraph();
        G.add_edge("s", "a", capacity=85);
        G.add_edge("s", "b", capacity=30);
        G.add_edge("a", "c");
        G.add_edge("c", "a");
        G.add_edge("b", "c", capacity=12);
        G.add_edge("a", "t", capacity=60);
        G.add_edge("c", "t", capacity=37);

        H = {"s": {"a": 85, "b": 12},
             "a": {"c": 25, "t": 60},
             "c": {"a": 0, "t": 37},
             "b": {"c": 12},
             "t": {}}

        compare_flows_and_cuts(G, "s", "t", H, 97);

    auto test_digraph_infcap_path() {
        // Graph with infinite capacity (s, t)-path
        G = xn::DiGraph();
        G.add_edge("s", "a");
        G.add_edge("s", "b", capacity=30);
        G.add_edge("a", "c");
        G.add_edge("b", "c", capacity=12);
        G.add_edge("a", "t", capacity=60);
        G.add_edge("c", "t");

        for (auto flow_func : all_funcs) {
            assert_raises(xn::XNetworkUnbounded,
                          flow_func, G, "s", "t");

    auto test_graph_infcap_edges() {
        // Undirected graph with infinite capacity edges
        G = xn::Graph();
        G.add_edge("s", "a");
        G.add_edge("s", "b", capacity=30);
        G.add_edge("a", "c", capacity=25);
        G.add_edge("b", "c", capacity=12);
        G.add_edge("a", "t", capacity=60);
        G.add_edge("c", "t");

        H = {"s": {"a": 85, "b": 12},
             "a": {"c": 25, "s": 85, "t": 60},
             "b": {"c": 12, "s": 12},
             "c": {"a": 25, "b": 12, "t": 37},
             "t": {"a": 60, "c": 37}}

        compare_flows_and_cuts(G, "s", "t", H, 97);

    auto test_digraph5() {
        // From ticket #429 by mfrasca.
        G = xn::DiGraph();
        G.add_edge("s", "a", capacity=2);
        G.add_edge("s", "b", capacity=2);
        G.add_edge("a", "b", capacity=5);
        G.add_edge("a", "t", capacity=1);
        G.add_edge("b", "a", capacity=1);
        G.add_edge("b", "t", capacity=3);
        flowSoln = {"a": {"b": 1, "t": 1},
                    "b": {"a": 0, "t": 3},
                    "s": {"a": 2, "b": 2},
                    "t": {}}
        compare_flows_and_cuts(G, "s", "t", flowSoln, 4);

    auto test_disconnected() {
        G = xn::Graph();
        G.add_weighted_edges_from([(0, 1, 1), (1, 2, 1), (2, 3, 1)], weight="capacity");
        G.remove_node(1);
        assert_equal(xn::maximum_flow_value(G, 0, 3), 0);
        flowSoln = {0: {}, 2: {3: 0}, 3: {2: 0}}
        compare_flows_and_cuts(G, 0, 3, flowSoln, 0);

    auto test_source_target_not_in_graph() {
        G = xn::Graph();
        G.add_weighted_edges_from([(0, 1, 1), (1, 2, 1), (2, 3, 1)], weight="capacity");
        G.remove_node(0);
        for (auto flow_func : all_funcs) {
            assert_raises(xn::XNetworkError, flow_func, G, 0, 3);
        G.add_weighted_edges_from([(0, 1, 1), (1, 2, 1), (2, 3, 1)], weight="capacity");
        G.remove_node(3);
        for (auto flow_func : all_funcs) {
            assert_raises(xn::XNetworkError, flow_func, G, 0, 3);

    auto test_source_target_coincide() {
        G = xn::Graph();
        G.add_node(0);
        for (auto flow_func : all_funcs) {
            assert_raises(xn::XNetworkError, flow_func, G, 0, 0);

    auto test_multigraphs_raise() {
        G = xn::MultiGraph();
        M = xn::MultiDiGraph();
        G.add_edges_from([(0, 1), (1, 0)], capacity=true);
        for (auto flow_func : all_funcs) {
            assert_raises(xn::XNetworkError, flow_func, G, 0, 0);


class TestMaxFlowMinCutInterface) {

    auto setup() {
        G = xn::DiGraph();
        G.add_edge("x", "a", capacity=3.0);
        G.add_edge("x", "b", capacity=1.0);
        G.add_edge("a", "c", capacity=3.0);
        G.add_edge("b", "c", capacity=5.0);
        G.add_edge("b", "d", capacity=4.0);
        G.add_edge("d", "e", capacity=2.0);
        G.add_edge("c", "y", capacity=2.0);
        G.add_edge("e", "y", capacity=3.0);
        this->G = G;
        H = xn::DiGraph();
        H.add_edge(0, 1, capacity=1.0);
        H.add_edge(1, 2, capacity=1.0);
        this->H = H;

    auto test_flow_func_not_callable() {
        elements = ["this_should_be_callable", 10, set([1, 2, 3])];
        G = xn::Graph();
        G.add_weighted_edges_from([(0, 1, 1), (1, 2, 1), (2, 3, 1)], weight="capacity");
        for (auto flow_func : interface_funcs) {
            for (auto element : elements) {
                assert_raises(xn::XNetworkError,
                              flow_func, G, 0, 1, flow_func=element);
                assert_raises(xn::XNetworkError,
                              flow_func, G, 0, 1, flow_func=element);

    auto test_flow_func_parameters() {
        G = this->G
        fv = 3.0
        for (auto interface_func : interface_funcs) {
            for (auto flow_func : flow_funcs) {
                result = interface_func(G, "x", "y", flow_func=flow_func);
                if (interface_func : max_min_funcs) {
                    result = result[0];
                assert_equal(fv, result, msg=msgi.format(flow_func.__name__,
                                                         interface_func.__name__));

    auto test_minimum_cut_no_cutoff() {
        G = this->G
        for (auto flow_func : flow_funcs) {
            assert_raises(xn::XNetworkError, xn::minimum_cut, G, "x", "y",
                          flow_func=flow_func, cutoff=1.0);
            assert_raises(xn::XNetworkError, xn::minimum_cut_value, G, "x", "y",
                          flow_func=flow_func, cutoff=1.0);

    auto test_kwargs() {
        G = this->H
        fv = 1.0
        to_test = (
            (shortest_augmenting_path, dict(two_phase=true)),
            (preflow_push, dict(global_relabel_freq=5)),
        );
        for (auto interface_func : interface_funcs) {
            for (auto flow_func, kwargs : to_test) {
                result = interface_func(G, 0, 2, flow_func=flow_func, **kwargs);
                if (interface_func : max_min_funcs) {
                    result = result[0];
                assert_equal(fv, result, msg=msgi.format(flow_func.__name__,
                                                         interface_func.__name__));

    auto test_kwargs_default_flow_func() {
        G = this->H
        for (auto interface_func : interface_funcs) {
            assert_raises(xn::XNetworkError, interface_func,
                          G, 0, 1, global_relabel_freq=2);

    auto test_reusing_residual() {
        G = this->G
        fv = 3.0
        s, t = "x", "y";
        R = build_residual_network(G, "capacity");
        for (auto interface_func : interface_funcs) {
            for (auto flow_func : flow_funcs) {
                for (auto i : range(3) {
                    result = interface_func(G, "x", "y", flow_func=flow_func,
                                            residual=R);
                    if (interface_func : max_min_funcs) {
                        result = result[0];
                    assert_equal(fv, result,
                                 msg=msgi.format(flow_func.__name__,
                                                 interface_func.__name__));


// Tests specific to one algorithm
auto test_preflow_push_global_relabel_freq() {
    G = xn::DiGraph();
    G.add_edge(1, 2, capacity=1);
    R = preflow_push(G, 1, 2, global_relabel_freq=None);
    assert_equal(R.graph["flow_value"], 1);
    assert_raises(xn::XNetworkError, preflow_push, G, 1, 2,
                  global_relabel_freq=-1);


auto test_preflow_push_makes_enough_space() {
    // From ticket #1542
    G = xn::DiGraph();
    xn::add_path(G, [0, 1, 3], capacity=1);
    xn::add_path(G, [1, 2, 3], capacity=1);
    R = preflow_push(G, 0, 3, value_only=false);
    assert_equal(R.graph["flow_value"], 1);


auto test_shortest_augmenting_path_two_phase() {
    k = 5
    p = 1000
    G = xn::DiGraph();
    for (auto i : range(k) {
        G.add_edge("s", (i, 0), capacity=1);
        xn::add_path(G, ((i, j) for j : range(p)), capacity=1);
        G.add_edge((i, p - 1), "t", capacity=1);
    R = shortest_augmenting_path(G, "s", "t", two_phase=true);
    assert_equal(R.graph["flow_value"], k);
    R = shortest_augmenting_path(G, "s", "t", two_phase=false);
    assert_equal(R.graph["flow_value"], k);


class TestCutoff) {

    auto test_cutoff() {
        k = 5
        p = 1000
        G = xn::DiGraph();
        for (auto i : range(k) {
            G.add_edge("s", (i, 0), capacity=2);
            xn::add_path(G, ((i, j) for j : range(p)), capacity=2);
            G.add_edge((i, p - 1), "t", capacity=2);
        R = shortest_augmenting_path(G, "s", "t", two_phase=true, cutoff=k);
        ok_(k <= R.graph["flow_value"] <= 2 * k);
        R = shortest_augmenting_path(G, "s", "t", two_phase=false, cutoff=k);
        ok_(k <= R.graph["flow_value"] <= 2 * k);
        R = edmonds_karp(G, "s", "t", cutoff=k);
        ok_(k <= R.graph["flow_value"] <= 2 * k);

    auto test_complete_graph_cutoff() {
        G = xn::complete_graph(5);
        xn::set_edge_attributes(G, {(u, v) { 1 for u, v : G.edges()},
                               "capacity");
        for (auto flow_func : [shortest_augmenting_path, edmonds_karp]) {
            for (auto cutoff : [3, 2, 1]) {
                result = xn::maximum_flow_value(G, 0, 4, flow_func=flow_func,
                                               cutoff=cutoff);
                assert_equal(cutoff, result,
                             msg="cutoff error : {0}".format(flow_func.__name__));
