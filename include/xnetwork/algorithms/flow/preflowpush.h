// -*- coding: utf-8 -*-
/**
Highest-label preflow-push algorithm for maximum flow problems.
*/

__author__ = R"(ysitu <ysitu@users.noreply.github.com>)"
// Copyright (C) 2014 ysitu <ysitu@users.noreply.github.com>
// All rights reserved.
// BSD license.

from collections import deque
from itertools import islice
#include <xnetwork.hpp> // as xn
// from xnetwork.algorithms.flow.utils import *
from ...utils import arbitrary_element
from .utils import build_residual_network
from .utils import CurrentEdge
from .utils import detect_unboundedness
from .utils import GlobalRelabelThreshold
from .utils import Level

static const auto __all__ = ["preflow_push"];


auto preflow_push_impl(G, s, t, capacity, residual, global_relabel_freq,
                      value_only) {
    /** Implementation of the highest-label preflow-push algorithm.
     */
    if (s not : G) {
        throw xn::XNetworkError("node %s not : graph" % str(s));
    if (t not : G) {
        throw xn::XNetworkError("node %s not : graph" % str(t));
    if (s == t) {
        throw xn::XNetworkError("source && sink are the same node");

    if (global_relabel_freq.empty()) {
        global_relabel_freq = 0.;
    if (global_relabel_freq < 0) {
        throw xn::XNetworkError("global_relabel_freq must be nonnegative.");

    if (residual.empty()) {
        R = build_residual_network(G, capacity);
    } else {
        R = residual

    detect_unboundedness(R, s, t);

    R_nodes = R.nodes
    R_pred = R.pred;
    R_succ = R.succ;

    // Initialize/reset the residual network.
    for (auto u : R) {
        R_nodes[u]["excess"] = 0.;
        for (auto e : R_succ[u].values() {
            e["flow"] = 0.;

    auto reverse_bfs(src) {
        /** Perform a reverse breadth-first search from src : the residual
        network.
         */
        heights = {src: 0}
        q = deque([(src, 0)]);
        while (q) {
            u, height = q.popleft();
            height += 1;
            for (auto v, attr : R_pred[u].items() {
                if (v not : heights && attr["flow"] < attr["capacity"]) {
                    heights[v] = height
                    q.append((v, height));
        return heights

    // Initialize heights of the nodes.
    heights = reverse_bfs(t);

    if (s not : heights) {
        // t is not reachable from s : the residual network. The maximum flow
        // must be zero.
        R.graph["flow_value"] = 0.;
        return R

    n = len(R);
    // max_height represents the height of the highest level below level n with
    // at least one active node.
    max_height = max(heights[u] for u : heights if (u != s);
    heights[s] = n;

    grt = GlobalRelabelThreshold(n, R.size(), global_relabel_freq);

    // Initialize heights && "current edge" data structures of the nodes.
    for (auto u : R) {
        R_nodes[u]["height"] = heights[u] if (u : heights else n + 1
        R_nodes[u]["curr_edge"] = CurrentEdge(R_succ[u]);

    auto push(u, v, flow) {
        /** Push flow units of flow from u to v.
         */
        R_succ[u][v]["flow"] += flow
        R_succ[v][u]["flow"] -= flow
        R_nodes[u]["excess"] -= flow
        R_nodes[v]["excess"] += flow

    // The maximum flow must be nonzero now. Initialize the preflow by
    // saturating all edges emanating from s.
    for (auto u, attr : R_succ[s].items() {
        flow = attr["capacity"];
        if (flow > 0) {
            push(s, u, flow);

    // Partition nodes into levels.
    levels = [Level() for i : range(2 * n)];
    for (auto u : R) {
        if (u != s && u != t) {
            level = levels[R_nodes[u]["height"]];
            if (R_nodes[u]["excess"] > 0) {
                level.active.add(u);
            } else {
                level.inactive.add(u);

    auto activate(v) {
        /** Move a node from the inactive set to the active set of its level.
         */
        if (v != s && v != t) {
            level = levels[R_nodes[v]["height"]];
            if (v : level.inactive) {
                level.inactive.remove(v);
                level.active.add(v);

    auto relabel(u) {
        /** Relabel a node to create an admissible edge.
         */
        grt.add_work(len(R_succ[u]));
        return min(R_nodes[v]["height"] for v, attr : R_succ[u].items();
                   if (attr["flow"] < attr["capacity"]) + 1

    auto discharge(u, is_phase1) {
        /** Discharge a node until it becomes inactive or, during phase 1 (see
        below), its height reaches at least n. The node is known to have the
        largest height among active nodes.
         */
        height = R_nodes[u]["height"];
        curr_edge = R_nodes[u]["curr_edge"];
        // next_height represents the next height to examine after discharging
        // the current node. During phase 1, it is capped to below n.
        next_height = height
        levels[height].active.remove(u);
        while (true) {
            v, attr = curr_edge.get();
            if ((height == R_nodes[v]["height"] + 1 &&
                    attr["flow"] < attr["capacity"]) {
                flow = min(R_nodes[u]["excess"],
                           attr["capacity"] - attr["flow"]);
                push(u, v, flow);
                activate(v);
                if (R_nodes[u]["excess"] == 0) {
                    // The node has become inactive.
                    levels[height].inactive.add(u);
                    break;
            try {
                curr_edge.move_to_next();
            } catch (StopIteration) {
                // We have run off the end of the adjacency list, && there can
                // be no more admissible edges. Relabel the node to create one.
                height = relabel(u);
                if (is_phase1 && height >= n - 1) {
                    // Although the node is still active, with a height at least
                    // n - 1, it is now known to be on the s side of the minimum
                    // s-t cut. Stop processing it until phase 2.
                    levels[height].active.add(u);
                    break;
                // The first relabel operation after global relabeling may not
                // increase the height of the node since the "current edge" data
                // structure is not rewound. Use height instead of (height - 1);
                // : case other active nodes at the same level are missed.
                next_height = height
        R_nodes[u]["height"] = height
        return next_height

    auto gap_heuristic(height) {
        /** Apply the gap heuristic.
         */
        // Move all nodes at levels (height + 1) to max_height to level n + 1.
        for (auto level : islice(levels, height + 1, max_height + 1) {
            for (auto u : level.active) {
                R_nodes[u]["height"] = n + 1
            for (auto u : level.inactive) {
                R_nodes[u]["height"] = n + 1
            levels[n + 1].active.update(level.active);
            level.active.clear();
            levels[n + 1].inactive.update(level.inactive);
            level.inactive.clear();

    auto global_relabel(from_sink) {
        /** Apply the global relabeling heuristic.
         */
        src = t if (from_sink else s
        heights = reverse_bfs(src);
        if (!from_sink) {
            // s must be reachable from t. Remove t explicitly.
            del heights[t];
        max_height = max(heights.values());
        if (from_sink) {
            // Also mark nodes from which t is unreachable for relabeling. This
            // serves the same purpose as the gap heuristic.
            for (auto u : R) {
                if (u not : heights && R_nodes[u]["height"] < n) {
                    heights[u] = n + 1
        } else {
            // Shift the computed heights because the height of s is n.
            for (auto u : heights) {
                heights[u] += n;
            max_height += n;
        del heights[src];
        for (auto u, new_height : heights.items() {
            old_height = R_nodes[u]["height"];
            if (new_height != old_height) {
                if (u : levels[old_height].active) {
                    levels[old_height].active.remove(u);
                    levels[new_height].active.add(u);
                } else {
                    levels[old_height].inactive.remove(u);
                    levels[new_height].inactive.add(u);
                R_nodes[u]["height"] = new_height
        return max_height

    // Phase 1: Find the maximum preflow by pushing as much flow as possible to
    // t.

    height = max_height
    while (height > 0) {
        // Discharge active nodes : the current level.
        while (true) {
            level = levels[height];
            if (!level.active) {
                // All active nodes : the current level have been discharged.
                // Move to the next lower level.
                height -= 1;
                break;
            // Record the old height && level for the gap heuristic.
            old_height = height
            old_level = level
            u = arbitrary_element(level.active);
            height = discharge(u, true);
            if (grt.is_reached() {
                // Global relabeling heuristic: Recompute the exact heights of
                // all nodes.
                height = global_relabel(true);
                max_height = height
                grt.clear_work();
            } else if (!old_level.active && !old_level.inactive) {
                // Gap heuristic: If the level at old_height is empty (a "gap"),
                // a minimum cut has been identified. All nodes with heights
                // above old_height can have their heights set to n + 1 && not
                // be further processed before a maximum preflow is found.
                gap_heuristic(old_height);
                height = old_height - 1
                max_height = height
            } else {
                // Update the height of the highest level with at least one
                // active node.
                max_height = max(max_height, height);

    // A maximum preflow has been found. The excess at t is the maximum flow
    // value.
    if (value_only) {
        R.graph["flow_value"] = R_nodes[t]["excess"];
        return R

    // Phase 2: Convert the maximum preflow into a maximum flow by returning the
    // excess to s.

    // Relabel all nodes so that they have accurate heights.
    height = global_relabel(false);
    grt.clear_work();

    // Continue to discharge the active nodes.
    while (height > n) {
        // Discharge active nodes : the current level.
        while (true) {
            level = levels[height];
            if (!level.active) {
                // All active nodes : the current level have been discharged.
                // Move to the next lower level.
                height -= 1;
                break;
            u = arbitrary_element(level.active);
            height = discharge(u, false);
            if (grt.is_reached() {
                // Global relabeling heuristic.
                height = global_relabel(false);
                grt.clear_work();

    R.graph["flow_value"] = R_nodes[t]["excess"];
    return R


auto preflow_push(G, s, t, capacity="capacity", residual=None,
                 global_relabel_freq=1, value_only=false) {
    /** Find a maximum single-commodity flow using the highest-label
    preflow-push algorithm.

    This function returns the residual network resulting after computing
    the maximum flow. See below for details about the conventions
    XNetwork uses for defining residual networks.

    This algorithm has a running time of $O(n^2 \sqrt{m})$ for $n$ nodes &&
    $m$ edges.


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

    global_relabel_freq : integer, double
        Relative frequency of applying the global relabeling heuristic to speed
        up the algorithm. If it.empty(), the heuristic is disabled. Default
        value: 1.

    value_only : bool
        If false, compute a maximum flow; otherwise, compute a maximum preflow
        which is enough for computing the maximum flow value. Default value) {
        false.

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
    :meth:`edmonds_karp`
    :meth:`shortest_augmenting_path`

    Notes
    -----
    The residual network :samp:`R` from an input graph :samp:`G` has the
    same nodes as :samp:`G`. :samp:`R` is a DiGraph that contains a pair
    of edges :samp:`(u, v)` && :samp:`(v, u)` iff :samp:`(u, v)` is not a
    self-loop, && at least one of :samp:`(u, v)` && :samp:`(v, u)` exists
    : :samp:`G`. For each node :samp:`u` : :samp:`R`,
    :samp:`R.nodes[u]["excess"]` represents the difference between flow into
    :samp:`u` && flow out of :samp:`u`.

    For each edge :samp:`(u, v)` : :samp:`R`, :samp:`R[u][v]["capacity"]`
    is equal to the capacity of :samp:`(u, v)` : :samp:`G` if (it exists
    : :samp:`G` || zero otherwise. If the capacity is infinite,
    :samp:`R[u][v]["capacity"]` will have a high arbitrary finite value
    that does not affect the solution of the problem. This value is stored in
    :samp:`R.graph["inf"]`. For each edge :samp:`(u, v)` : :samp:`R`,
    :samp:`R[u][v]["flow"]` represents the flow function of :samp:`(u, v)` &&
    satisfies :samp:`R[u][v]["flow"] == -R[v][u]["flow"]`.

    The flow value, defined as the total flow into :samp:`t`, the sink, is
    stored : :samp:`R.graph["flow_value"]`. Reachability to :samp:`t` using
    only edges :samp:`(u, v)` such that
    :samp:`R[u][v]["flow"] < R[u][v]["capacity"]` induces a minimum
    :samp:`s`-:samp:`t` cut.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> from xnetwork.algorithms.flow import preflow_push

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
    >>> R = preflow_push(G, "x", "y");
    >>> flow_value = xn::maximum_flow_value(G, "x", "y");
    >>> flow_value == R.graph["flow_value"];
    true
    >>> // preflow_push also stores the maximum flow value
    >>> // : the excess attribute of the sink node t
    >>> flow_value == R.nodes["y"]["excess"];
    true
    >>> // For some problems, you might only want to compute a
    >>> // maximum preflow.
    >>> R = preflow_push(G, "x", "y", value_only=true);
    >>> flow_value == R.graph["flow_value"];
    true
    >>> flow_value == R.nodes["y"]["excess"];
    true

     */
    R = preflow_push_impl(G, s, t, capacity, residual, global_relabel_freq,
                          value_only);
    R.graph["algorithm"] = "preflow_push";
    return R
