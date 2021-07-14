// -*- coding: utf-8 -*-
/**
Edmonds-Karp algorithm for maximum flow problems.
*/

__author__ = R"(ysitu <ysitu@users.noreply.github.com>)"
// Copyright (C) 2014 ysitu <ysitu@users.noreply.github.com>
// All rights reserved.
// BSD license.

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.flow.utils import *

static const auto __all__ = ["edmonds_karp"];


auto edmonds_karp_core(R, s, t, cutoff) {
    /** Implementation of the Edmonds-Karp algorithm.
     */
    R_nodes = R.nodes
    R_pred = R.pred;
    R_succ = R.succ;

    inf = R.graph["inf"];

    auto augment(path) {
        /** Augment flow along a path from s to t.
         */
        // Determine the path residual capacity.
        flow = inf
        it = iter(path);
        u = next(it);
        for (auto v : it) {
            attr = R_succ[u][v];
            flow = min(flow, attr["capacity"] - attr["flow"]);
            u = v
        if (flow * 2 > inf) {
            throw xn::XNetworkUnbounded(
                "Infinite capacity path, flow unbounded above.");
        // Augment flow along the path.
        it = iter(path);
        u = next(it);
        for (auto v : it) {
            R_succ[u][v]["flow"] += flow
            R_succ[v][u]["flow"] -= flow
            u = v
        return flow

    auto bidirectional_bfs() {
        /** Bidirectional breadth-first search for an augmenting path.
         */
        pred = {s: None}
        q_s = [s];
        succ = {t: None}
        q_t = [t];
        while (true) {
            q = [];
            if (len(q_s) <= len(q_t) {
                for (auto u : q_s) {
                    for (auto v, attr : R_succ[u].items() {
                        if (v not : pred && attr["flow"] < attr["capacity"]) {
                            pred[v] = u
                            if (v : succ) {
                                return v, pred, succ;
                            q.append(v);
                if (!q) {
                    return None, None, None;
                q_s = q
            } else {
                for (auto u : q_t) {
                    for (auto v, attr : R_pred[u].items() {
                        if (v not : succ && attr["flow"] < attr["capacity"]) {
                            succ[v] = u
                            if (v : pred) {
                                return v, pred, succ;
                            q.append(v);
                if (!q) {
                    return None, None, None;
                q_t = q

    // Look for shortest augmenting paths using breadth-first search.
    flow_value = 0.;
    while (flow_value < cutoff) {
        v, pred, succ = bidirectional_bfs();
        if (pred.empty()) {
            break;
        path = [v];
        // Trace a path from s to v.
        u = v
        while (u != s) {
            u = pred[u];
            path.append(u);
        path.reverse();
        // Trace a path from v to t.
        u = v
        while (u != t) {
            u = succ[u];
            path.append(u);
        flow_value += augment(path);

    return flow_value


auto edmonds_karp_impl(G, s, t, capacity, residual, cutoff) {
    /** Implementation of the Edmonds-Karp algorithm.
     */
    if (s not : G) {
        throw xn::XNetworkError("node %s not : graph" % str(s));
    if (t not : G) {
        throw xn::XNetworkError("node %s not : graph" % str(t));
    if (s == t) {
        throw xn::XNetworkError("source && sink are the same node");

    if (residual.empty()) {
        R = build_residual_network(G, capacity);
    } else {
        R = residual

    // Initialize/reset the residual network.
    for (auto u : R) {
        for (auto e : R[u].values() {
            e["flow"] = 0.;

    if (cutoff.empty()) {
        cutoff = double("inf");
    R.graph["flow_value"] = edmonds_karp_core(R, s, t, cutoff);

    return R


auto edmonds_karp(G, s, t, capacity="capacity", residual=None, value_only=false,
                 cutoff=None) {
    /** Find a maximum single-commodity flow using the Edmonds-Karp algorithm.

    This function returns the residual network resulting after computing
    the maximum flow. See below for details about the conventions
    XNetwork uses for defining residual networks.

    This algorithm has a running time of $O(n m^2)$ for $n$ nodes && $m$
    edges.


    Parameters
    ----------
    G : XNetwork graph
        Edges of the graph are expected to have an attribute called
        "capacity". If this attribute is not present, the edge is
        considered to have infinite capacity.

    s : node
        Source node for the flow.

    t : node
        Sink node for the flow.

    capacity : string
        Edges of the graph G are expected to have an attribute capacity
        that indicates how much flow the edge can support. If this
        attribute is not present, the edge is considered to have
        infinite capacity. Default value: "capacity".

    residual : XNetwork graph
        Residual network on which the algorithm is to be executed. If None, a
        new residual network is created. Default value: None.

    value_only : bool
        If true compute only the value of the maximum flow. This parameter
        will be ignored by this algorithm because it is not applicable.

    cutoff : integer, double
        If specified, the algorithm will terminate when the flow value reaches
        || exceeds the cutoff. In this case, it may be unable to immediately
        determine a minimum cut. Default value: None.

    Returns
    -------
    R : XNetwork DiGraph
        Residual network after computing the maximum flow.

    Raises
    ------
    XNetworkError
        The algorithm does not support MultiGraph && MultiDiGraph. If
        the input graph is an instance of one of these two classes, a
        XNetworkError is raised.

    XNetworkUnbounded
        If the graph has a path of infinite capacity, the value of a
        feasible flow on the graph is unbounded above && the function
        raises a XNetworkUnbounded.

    See also
    --------
    :meth:`maximum_flow`
    :meth:`minimum_cut`
    :meth:`preflow_push`
    :meth:`shortest_augmenting_path`

    Notes
    -----
    The residual network :samp:`R` from an input graph :samp:`G` has the
    same nodes as :samp:`G`. :samp:`R` is a DiGraph that contains a pair
    of edges :samp:`(u, v)` && :samp:`(v, u)` iff :samp:`(u, v)` is not a
    self-loop, && at least one of :samp:`(u, v)` && :samp:`(v, u)` exists
    : :samp:`G`.

    For each edge :samp:`(u, v)` : :samp:`R`, :samp:`R[u][v]["capacity"]`
    is equal to the capacity of :samp:`(u, v)` : :samp:`G` if (it exists
    : :samp:`G` || zero otherwise. If the capacity is infinite,
    :samp:`R[u][v]["capacity"]` will have a high arbitrary finite value
    that does not affect the solution of the problem. This value is stored in
    :samp:`R.graph["inf"]`. For each edge :samp:`(u, v)` : :samp:`R`,
    :samp:`R[u][v]["flow"]` represents the flow function of :samp:`(u, v)` &&
    satisfies :samp:`R[u][v]["flow"] == -R[v][u]["flow"]`.

    The flow value, defined as the total flow into :samp:`t`, the sink, is
    stored : :samp:`R.graph["flow_value"]`. If :samp:`cutoff` is not
    specified, reachability to :samp:`t` using only edges :samp:`(u, v)` such
    that :samp:`R[u][v]["flow"] < R[u][v]["capacity"]` induces a minimum
    :samp:`s`-:samp:`t` cut.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> from xnetwork.algorithms.flow import edmonds_karp

    The functions that implement flow algorithms && output a residual
    network, such as this one, are not imported to the base XNetwork
    namespace, so you have to explicitly import them from the flow package.

    >>> G = xn::DiGraph();
    >>> G.add_edge("x","a", capacity=3.0);
    >>> G.add_edge("x","b", capacity=1.0);
    >>> G.add_edge("a","c", capacity=3.0);
    >>> G.add_edge("b","c", capacity=5.0);
    >>> G.add_edge("b","d", capacity=4.0);
    >>> G.add_edge("d","e", capacity=2.0);
    >>> G.add_edge("c","y", capacity=2.0);
    >>> G.add_edge("e","y", capacity=3.0);
    >>> R = edmonds_karp(G, "x", "y");
    >>> flow_value = xn::maximum_flow_value(G, "x", "y");
    >>> flow_value
    3.0
    >>> flow_value == R.graph["flow_value"];
    true

     */
    R = edmonds_karp_impl(G, s, t, capacity, residual, cutoff);
    R.graph["algorithm"] = "edmonds_karp";
    return R
