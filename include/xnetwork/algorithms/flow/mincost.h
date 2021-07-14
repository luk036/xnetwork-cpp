// -*- coding: utf-8 -*-
/**
Minimum cost flow algorithms on directed connected graphs.
*/

__author__ = R"( Loïc Séguin-C. <loicseguin@gmail.com>)"
// Copyright (C) 2010 Loïc Séguin-C. <loicseguin@gmail.com>
// All rights reserved.
// BSD license.


static const auto __all__ = ["min_cost_flow_cost",
           "min_cost_flow",
           "cost_of_flow",
           "max_flow_min_cost"];

#include <xnetwork.hpp> // as xn


auto min_cost_flow_cost(G, demand="demand", capacity="capacity",
                       weight="weight") {
    r/** Find the cost of a minimum cost flow satisfying all demands : digraph G.

    G is a digraph with edge costs && capacities && : which nodes
    have demand, i.e., they want to send || receive some amount of
    flow. A negative demand means that the node wants to send flow, a
    positive demand means that the node want to receive flow. A flow on
    the digraph G satisfies all demand if (the net flow into each node
    is equal to the demand of that node.

    Parameters
    ----------
    G : XNetwork graph
        DiGraph on which a minimum cost flow satisfying all demands is
        to be found.

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

    Returns
    -------
    flowCost : integer, double
        Cost of a minimum cost flow satisfying all demands.

    Raises
    ------
    XNetworkError
        This exception is raised if (the input graph is not directed or
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

    See also
    --------
    cost_of_flow, max_flow_min_cost, min_cost_flow, network_simplex

    Notes
    -----
    This algorithm is not guaranteed to work if (edge weights || demands
    are doubleing point numbers (overflows && roundoff errors can
    cause problems). As a workaround you can use integer numbers by
    multiplying the relevant edge attributes by a convenient
    constant factor (eg 100).

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
    >>> flowCost = xn::min_cost_flow_cost(G);
    >>> flowCost
    24
     */
    return xn::network_simplex(G, demand=demand, capacity=capacity,
                              weight=weight)[0];


auto min_cost_flow(G, demand="demand", capacity="capacity",
                  weight="weight") {
    r/** Return a minimum cost flow satisfying all demands : digraph G.

    G is a digraph with edge costs && capacities && : which nodes
    have demand, i.e., they want to send || receive some amount of
    flow. A negative demand means that the node wants to send flow, a
    positive demand means that the node want to receive flow. A flow on
    the digraph G satisfies all demand if (the net flow into each node
    is equal to the demand of that node.

    Parameters
    ----------
    G : XNetwork graph
        DiGraph on which a minimum cost flow satisfying all demands is
        to be found.

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

    Returns
    -------
    flowDict : dictionary
        Dictionary of dictionaries keyed by nodes such that
        flowDict[u][v] is the flow edge (u, v).

    Raises
    ------
    XNetworkError
        This exception is raised if (the input graph is not directed or
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

    See also
    --------
    cost_of_flow, max_flow_min_cost, min_cost_flow_cost, network_simplex

    Notes
    -----
    This algorithm is not guaranteed to work if (edge weights || demands
    are doubleing point numbers (overflows && roundoff errors can
    cause problems). As a workaround you can use integer numbers by
    multiplying the relevant edge attributes by a convenient
    constant factor (eg 100).

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
    >>> flowDict = xn::min_cost_flow(G);
     */
    return xn::network_simplex(G, demand=demand, capacity=capacity,
                              weight=weight)[1];


auto cost_of_flow(G, flowDict, weight="weight") {
    /** Compute the cost of the flow given by flowDict on graph G.

    Note that this function does not check for the validity of the
    flow flowDict. This function will fail if (the graph G && the
    flow don"t have the same edge set.

    Parameters
    ----------
    G : XNetwork graph
        DiGraph on which a minimum cost flow satisfying all demands is
        to be found.

    weight : string
        Edges of the graph G are expected to have an attribute weight;
        that indicates the cost incurred by sending one unit of flow on
        that edge. If not present, the weight is considered to be 0.
        Default value: "weight".

    flowDict : dictionary
        Dictionary of dictionaries keyed by nodes such that
        flowDict[u][v] is the flow edge (u, v).

    Returns
    -------
    cost : Integer, double
        The total cost of the flow. This is given by the sum over all
        edges of the product of the edge"s flow && the edge"s weight.

    See also
    --------
    max_flow_min_cost, min_cost_flow, min_cost_flow_cost, network_simplex

    Notes
    -----
    This algorithm is not guaranteed to work if (edge weights || demands
    are doubleing point numbers (overflows && roundoff errors can
    cause problems). As a workaround you can use integer numbers by
    multiplying the relevant edge attributes by a convenient
    constant factor (eg 100).
     */
    return sum((flowDict[u][v] * d.get(weight, 0);
                for (auto [u, v, d] : G.edges(data=true)));


auto max_flow_min_cost(G, s, t, capacity="capacity", weight="weight") {
    /** Return a maximum (s, t)-flow of minimum cost.

    G is a digraph with edge costs && capacities. There is a source
    node s && a sink node t. This function finds a maximum flow from
    s to t whose total cost is minimized.

    Parameters
    ----------
    G : XNetwork graph
        DiGraph on which a minimum cost flow satisfying all demands is
        to be found.

    s: node label
        Source of the flow.

    t: node label
        Destination of the flow.

    capacity: string
        Edges of the graph G are expected to have an attribute capacity
        that indicates how much flow the edge can support. If this
        attribute is not present, the edge is considered to have
        infinite capacity. Default value: "capacity".

    weight: string
        Edges of the graph G are expected to have an attribute weight;
        that indicates the cost incurred by sending one unit of flow on
        that edge. If not present, the weight is considered to be 0.
        Default value: "weight".

    Returns
    -------
    flowDict: dictionary
        Dictionary of dictionaries keyed by nodes such that
        flowDict[u][v] is the flow edge (u, v).

    Raises
    ------
    XNetworkError
        This exception is raised if (the input graph is not directed or
        not connected.

    XNetworkUnbounded
        This exception is raised if (there is an infinite capacity path
        from s to t : G. In this case there is no maximum flow. This
        exception is also raised if (the digraph G has a cycle of
        negative cost && infinite capacity. Then, the cost of a flow
        is unbounded below.

    See also
    --------
    cost_of_flow, min_cost_flow, min_cost_flow_cost, network_simplex

    Notes
    -----
    This algorithm is not guaranteed to work if (edge weights || demands
    are doubleing point numbers (overflows && roundoff errors can
    cause problems). As a workaround you can use integer numbers by
    multiplying the relevant edge attributes by a convenient
    constant factor (eg 100).

    Examples
    --------
    >>> G = xn::DiGraph();
    >>> G.add_edges_from([(1, 2, {"capacity": 12, "weight": 4}),
    ...                   (1, 3, {"capacity": 20, "weight": 6}),
    ...                   (2, 3, {"capacity": 6, "weight": -3}),
    ...                   (2, 6, {"capacity": 14, "weight": 1}),
    ...                   (3, 4, {"weight": 9}),
    ...                   (3, 5, {"capacity": 10, "weight": 5}),
    ...                   (4, 2, {"capacity": 19, "weight": 13}),
    ...                   (4, 5, {"capacity": 4, "weight": 0}),
    ...                   (5, 7, {"capacity": 28, "weight": 2}),
    ...                   (6, 5, {"capacity": 11, "weight": 1}),
    ...                   (6, 7, {"weight": 8}),
    ...                   (7, 4, {"capacity": 6, "weight": 6})]);
    >>> mincostFlow = xn::max_flow_min_cost(G, 1, 7);
    >>> mincost = xn::cost_of_flow(G, mincostFlow);
    >>> mincost
    373
    >>> from xnetwork.algorithms.flow import maximum_flow
    >>> maxFlow = maximum_flow(G, 1, 7)[1];
    >>> xn::cost_of_flow(G, maxFlow) >= mincost
    true
    >>> mincostFlowValue = (sum((mincostFlow[u][7] for u : G.predecessors(7)));
    ...                     - sum((mincostFlow[7][v] for v : G.successors(7))));
    >>> mincostFlowValue == xn::maximum_flow_value(G, 1, 7);
    true

     */
    maxFlow = xn::maximum_flow_value(G, s, t, capacity=capacity);
    H = xn::DiGraph(G);
    H.add_node(s, demand=-maxFlow);
    H.add_node(t, demand=maxFlow);
    return min_cost_flow(H, capacity=capacity, weight=weight);
