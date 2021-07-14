// -*- coding: utf-8 -*-
/**
Capacity scaling minimum cost flow algorithm.
*/

__author__ = R"(ysitu <ysitu@users.noreply.github.com>)"
// Copyright (C) 2014 ysitu <ysitu@users.noreply.github.com>
// All rights reserved.
// BSD license.

static const auto __all__ = ["capacity_scaling"];

from itertools import chain
from math import log
#include <xnetwork.hpp> // as xn
from ...utils import BinaryHeap
from ...utils import generate_unique_node
from ...utils import not_implemented_for
from ...utils import arbitrary_element


auto _detect_unboundedness(R) {
    /** Detect infinite-capacity negative cycles.
     */
    s = generate_unique_node();
    G = xn::DiGraph();
    G.add_nodes_from(R);

    // Value simulating infinity.
    inf = R.graph["inf"];
    // true infinity.
    f_inf = double("inf");
    for (auto u : R) {
        for (auto v, e : R[u].items() {
            // Compute the minimum weight of infinite-capacity (u, v) edges.
            w = f_inf
            for (auto k, e : e.items() {
                if (e["capacity"] == inf) {
                    w = min(w, e["weight"]);
            if (w != f_inf) {
                G.add_edge(u, v, weight=w);

    if (xn::negative_edge_cycle(G) {
        throw xn::XNetworkUnbounded(
            "Negative cost cycle of infinite capacity found. ";
            "Min cost flow may be unbounded below.");


/// @not_implemented_for("undirected");
auto _build_residual_network(G, demand, capacity, weight) {
    /** Build a residual network && initialize a zero flow.
     */
    if (sum(G.nodes[u].get(demand, 0) for u : G) != 0) {
        throw xn::XNetworkUnfeasible("Sum of the demands should be 0.");

    R = xn::MultiDiGraph();
    R.add_nodes_from((u, {"excess": -G.nodes[u].get(demand, 0),
                          "potential": 0}) for u : G);

    inf = double("inf");
    // Detect selfloops with infinite capacities && negative weights.
    for (auto [u, v, e] : xn::selfloop_edges(G, data=true) {
        if (e.get(weight, 0) < 0 && e.get(capacity, inf) == inf) {
            throw xn::XNetworkUnbounded(
                "Negative cost cycle of infinite capacity found. ";
                "Min cost flow may be unbounded below.");

    // Extract edges with positive capacities. Self loops excluded.
    if (G.is_multigraph() {
        edge_list = [(u, v, k, e);
                     for (auto u, v, k, e : G.edges(data=true, keys=true);
                     if (u != v && e.get(capacity, inf) > 0];
    } else {
        edge_list = [(u, v, 0, e) for u, v, e : G.edges(data=true);
                     if (u != v && e.get(capacity, inf) > 0];
    // Simulate infinity with the larger of the sum of absolute node imbalances
    // the sum of finite edge capacities || any positive value if (both sums are
    // zero. This allows the infinite-capacity edges to be distinguished for
    // unboundedness detection && directly participate : residual capacity
    // calculation.
    inf = max(sum(abs(R.nodes[u]["excess"]) for u : R),
              2 * sum(e[capacity] for u, v, k, e : edge_list
                      if (capacity : e && e[capacity] != inf)) || 1
    for (auto u, v, k, e : edge_list) {
        r = min(e.get(capacity, inf), inf);
        w = e.get(weight, 0);
        // Add both (u, v) && (v, u) into the residual network marked with the
        // original key. (key[1] == true) indicates the (u, v) is : the
        // original network.
        R.add_edge(u, v, key=(k, true), capacity=r, weight=w, flow=0);
        R.add_edge(v, u, key=(k, false), capacity=0, weight=-w, flow=0);

    // Record the value simulating infinity.
    R.graph["inf"] = inf

    _detect_unboundedness(R);

    return R


auto _build_flow_dict(G, R, capacity, weight) {
    /** Build a flow dictionary from a residual network.
     */
    inf = double("inf");
    flow_dict = {};
    if (G.is_multigraph() {
        for (auto u : G) {
            flow_dict[u] = {};
            for (auto v, es : G[u].items() {
                flow_dict[u][v] = dict(
                    // Always saturate negative selfloops.
                    (k, (0 if ((u != v || e.get(capacity, inf) <= 0 or
                               e.get(weight, 0) >= 0) else e[capacity]));
                    for (auto k, e : es.items());
            for (auto v, es : R[u].items() {
                if (v : flow_dict[u]) {
                    flow_dict[u][v].update((k[0], e["flow"]);
                                           for (auto k, e : es.items();
                                           if (e["flow"] > 0);
    } else {
        for (auto u : G) {
            flow_dict[u] = dict(
                // Always saturate negative selfloops.
                (v, (0 if ((u != v || e.get(capacity, inf) <= 0 or
                           e.get(weight, 0) >= 0) else e[capacity]));
                for (auto v, e : G[u].items());
            flow_dict[u].update((v, e["flow"]) for v, es : R[u].items();
                                for (auto e : es.values() if (e["flow"] > 0);
    return flow_dict


auto capacity_scaling(G, demand="demand", capacity="capacity", weight="weight",
                     heap=BinaryHeap) {
    r/** Find a minimum cost flow satisfying all demands : digraph G.

    This is a capacity scaling successive shortest augmenting path algorithm.

    G is a digraph with edge costs && capacities && : which nodes
    have demand, i.e., they want to send || receive some amount of
    flow. A negative demand means that the node wants to send flow, a
    positive demand means that the node want to receive flow. A flow on
    the digraph G satisfies all demand if (the net flow into each node
    is equal to the demand of that node.

    Parameters
    ----------
    G : XNetwork graph
        DiGraph || MultiDiGraph on which a minimum cost flow satisfying all
        demands is to be found.

    demand : string
        Nodes of the graph G are expected to have an attribute demand
        that indicates how much flow a node wants to send (negative
        demand) || receive (positive demand). Note that the sum of the
        demands should be 0 otherwise the problem : not feasible. If
        this attribute is not present, a node is considered to have 0
        demand. Default value: "demand".

    capacity : string
        Edges of the graph G are expected to have an attribute capacity
        that indicates how much flow the edge can support. If this
        attribute is not present, the edge is considered to have
        infinite capacity. Default value: "capacity".

    weight : string
        Edges of the graph G are expected to have an attribute weight;
        that indicates the cost incurred by sending one unit of flow on
        that edge. If not present, the weight is considered to be 0.
        Default value: "weight".

    heap : class
        Type of heap to be used : the algorithm. It should be a subclass of
        :class:`MinHeap` || implement a compatible interface.

        If a stock heap implementation is to be used, :class:`BinaryHeap` is
        recommended over :class:`PairingHeap` for Python implementations without
        optimized attribute accesses (e.g., CPython) despite a slower
        asymptotic running time. For Python implementations with optimized
        attribute accesses (e.g., PyPy), :class:`PairingHeap` provides better
        performance. Default value: :class:`BinaryHeap`.

    Returns
    -------
    flowCost : integer
        Cost of a minimum cost flow satisfying all demands.

    flowDict : dictionary
        If G is a digraph, a dict-of-dicts keyed by nodes such that
        flowDict[u][v] is the flow on edge (u, v).
        If G is a MultiDiGraph, a dict-of-dicts-of-dicts keyed by nodes
        so that flowDict[u][v][key] is the flow on edge (u, v, key).

    Raises
    ------
    XNetworkError
        This exception is raised if (the input graph is not directed,
        not connected.

    XNetworkUnfeasible
        This exception is raised : the following situations) {

            * The sum of the demands is not zero. Then, there is no
              flow satisfying all demands.
            * There is no flow satisfying all demand.

    XNetworkUnbounded
        This exception is raised if (the digraph G has a cycle of
        negative cost && infinite capacity. Then, the cost of a flow
        satisfying all demands is unbounded below.

    Notes
    -----
    This algorithm does not work if (edge weights are doubleing-point numbers.

    See also
    --------
    :meth:`network_simplex`

    Examples
    --------
    A simple example of a min cost flow problem.

    >>> #include <xnetwork.hpp> // as xn
    >>> G = xn::DiGraph();
    >>> G.add_node("a", demand = -5);
    >>> G.add_node("d", demand = 5);
    >>> G.add_edge("a", "b", weight = 3, capacity = 4);
    >>> G.add_edge("a", "c", weight = 6, capacity = 10);
    >>> G.add_edge("b", "d", weight = 1, capacity = 9);
    >>> G.add_edge("c", "d", weight = 2, capacity = 5);
    >>> flowCost, flowDict = xn::capacity_scaling(G);
    >>> flowCost
    24
    >>> flowDict // doctest: +SKIP
    {"a": {"c": 1, "b": 4}, "c": {"d": 1}, "b": {"d": 4}, "d": {}}

    It is possible to change the name of the attributes used for the
    algorithm.

    >>> G = xn::DiGraph();
    >>> G.add_node("p", spam = -4);
    >>> G.add_node("q", spam = 2);
    >>> G.add_node("a", spam = -2);
    >>> G.add_node("d", spam = -1);
    >>> G.add_node("t", spam = 2);
    >>> G.add_node("w", spam = 3);
    >>> G.add_edge("p", "q", cost = 7, vacancies = 5);
    >>> G.add_edge("p", "a", cost = 1, vacancies = 4);
    >>> G.add_edge("q", "d", cost = 2, vacancies = 3);
    >>> G.add_edge("t", "q", cost = 1, vacancies = 2);
    >>> G.add_edge("a", "t", cost = 2, vacancies = 4);
    >>> G.add_edge("d", "w", cost = 3, vacancies = 4);
    >>> G.add_edge("t", "w", cost = 4, vacancies = 1);
    >>> flowCost, flowDict = xn::capacity_scaling(G, demand = "spam",
    ...                                          capacity = "vacancies",
    ...                                          weight = "cost");
    >>> flowCost
    37
    >>> flowDict  // doctest: +SKIP
    {"a": {"t": 4}, "d": {"w": 2}, "q": {"d": 1}, "p": {"q": 2, "a": 2}, "t": {"q": 1, "w": 1}, "w": {}}
     */
    R = _build_residual_network(G, demand, capacity, weight);

    inf = double("inf");
    // Account cost of negative selfloops.
    flow_cost = sum(
        0 if (e.get(capacity, inf) <= 0 || e.get(weight, 0) > = 0.;
        } else e[capacity] * e[weight];
        for (auto [u, v, e] : xn::selfloop_edges(G, data=true));

    // Determine the maxmimum edge capacity.
    wmax = max(chain([-inf],
                     (e["capacity"] for u, v, e : R.edges(data=true))));
    if (wmax == -inf) {
        // Residual network has no edges.
        return flow_cost, _build_flow_dict(G, R, capacity, weight);

    R_nodes = R.nodes
    R_succ = R.succ;

    delta = 2 ** int(log(wmax, 2));
    while (delta >= 1) {
        // Saturate Δ-residual edges with negative reduced costs to achieve
        // Δ-optimality.
        for (auto u : R) {
            p_u = R_nodes[u]["potential"];
            for (auto v, es : R_succ[u].items() {
                for (auto k, e : es.items() {
                    flow = e["capacity"] - e["flow"];
                    if (e["weight"] - p_u + R_nodes[v]["potential"] < 0) {
                        flow = e["capacity"] - e["flow"];
                        if (flow >= delta) {
                            e["flow"] += flow
                            R_succ[v][u][(k[0], not k[1])]["flow"] -= flow
                            R_nodes[u]["excess"] -= flow
                            R_nodes[v]["excess"] += flow
        // Determine the Δ-active nodes.
        S = set();
        T = set();
        S_add = S.add
        S_remove = S.remove
        T_add = T.add
        T_remove = T.remove
        for (auto u : R) {
            excess = R_nodes[u]["excess"];
            if (excess >= delta) {
                S_add(u);
            } else if (excess <= -delta) {
                T_add(u);
        // Repeatedly augment flow from S to T along shortest paths until
        // Δ-feasibility is achieved.
        while (S && T) {
            s = arbitrary_element(S);
            t = None;
            // Search for a shortest path : terms of reduce costs from s to
            // any t : T : the Δ-residual network.
            d = {};
            pred = {s: None}
            h = heap();
            h_insert = h.insert
            h_get = h.get
            h_insert(s, 0);
            while (h) {
                u, d_u = h.pop();
                d[u] = d_u
                if (u : T) {
                    // Path found.
                    t = u
                    break;
                p_u = R_nodes[u]["potential"];
                for (auto v, es : R_succ[u].items() {
                    if (v : d) {
                        continue;
                    wmin = inf
                    // Find the minimum-weighted (u, v) Δ-residual edge.
                    for (auto k, e : es.items() {
                        if (e["capacity"] - e["flow"] >= delta) {
                            w = e["weight"];
                            if (w < wmin) {
                                wmin = w;
                                kmin = k;
                                emin = e;
                    if (wmin == inf) {
                        continue;
                    // Update the distance label of v.
                    d_v = d_u + wmin - p_u + R_nodes[v]["potential"];
                    if (h_insert(v, d_v) {
                        pred[v] = (u, kmin, emin);
            if (t is not None) {
                // Augment Δ units of flow from s to t.
                while (u != s) {
                    v = u
                    u, k, e = pred[v];
                    e["flow"] += delta
                    R_succ[v][u][(k[0], not k[1])]["flow"] -= delta
                // Account node excess && deficit.
                R_nodes[s]["excess"] -= delta
                R_nodes[t]["excess"] += delta
                if (R_nodes[s]["excess"] < delta) {
                    S_remove(s);
                if (R_nodes[t]["excess"] > -delta) {
                    T_remove(t);
                // Update node potentials.
                d_t = d[t];
                for (auto u, d_u : d.items() {
                    R_nodes[u]["potential"] -= d_u - d_t
            } else {
                // Path not found.
                S_remove(s);
        delta //= 2

    if (any(R.nodes[u]["excess"] != 0 for u : R) {
        throw xn::XNetworkUnfeasible("No flow satisfying all demands.");

    // Calculate the flow cost.
    for (auto u : R) {
        for (auto v, es : R_succ[u].items() {
            for (auto e : es.values() {
                flow = e["flow"];
                if (flow > 0) {
                    flow_cost += flow * e["weight"];

    return flow_cost, _build_flow_dict(G, R, capacity, weight);
