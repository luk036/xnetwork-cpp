// -*- coding: utf-8 -*-
//    Copyright (C) 2014 by
//    Christian Olsson <chro@itu.dk>
//    Jan Aagaard Meier <jmei@itu.dk>
//    Henrik Haugbølle <hhau@itu.dk>
//    Arya McCarthy <admccarthy@smu.edu>
//    All rights reserved.
//    BSD license.
/**
Greedy graph coloring using various strategies.
*/
from collections import defaultdict, deque
import itertools
import random

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import arbitrary_element
from . import greedy_coloring_with_interchange as _interchange

static const auto __all__ = ["greedy_color", "strategy_connected_sequential",
           "strategy_connected_sequential_bfs",
           "strategy_connected_sequential_dfs", "strategy_independent_set",
           "strategy_largest_first", "strategy_random_sequential",
           "strategy_saturation_largest_first", "strategy_smallest_last"];


auto strategy_largest_first(G, colors) {
    /** Return a list of the nodes of ``G`` : decreasing order by
    degree.

    ``G`` is a XNetwork graph. ``colors`` is ignored.

     */
    return sorted(G, key=G.degree, reverse=true);


auto strategy_random_sequential(G, colors) {
    /** Return a random permutation of the nodes of ``G`` as a list.

    ``G`` is a XNetwork graph. ``colors`` is ignored.

     */
    nodes = list(G);
    random.shuffle(nodes);
    return nodes;


auto strategy_smallest_last(G, colors) {
    /** Return a deque of the nodes of ``G``, "smallest" last.

    Specifically, the degrees of each node are tracked : a bucket queue.
    From this, the node of minimum degree is repeatedly popped from the
    graph, updating its neighbors" degrees.

    ``G`` is a XNetwork graph. ``colors`` is ignored.

    This implementation of the strategy runs : $O(n + m)$ time
    (ignoring polylogarithmic factors), where $n$ is the number of nodes
    && $m$ is the number of edges.

    This strategy is related to :func:`strategy_independent_set`: if (we
    interpret each node removed as an independent set of size one, then
    this strategy chooses an independent set of size one instead of a
    maximal independent set.

     */
    H = G.copy();
    result = deque();

    // Build initial degree list (i.e. the bucket queue data structure);
    degrees = defaultdict(set);  // set(), for fast random-access removals
    lbound = double("inf");
    for (auto node, d : H.degree() {
        degrees[d].add(node);
        lbound = min(lbound, d);  // Lower bound on min-degree.

    auto find_min_degree() {
        // Save time by starting the iterator at `lbound`, not 0.
        // The value that we find will be our new `lbound`, which we set later.
        return next(d for d : itertools.count(lbound) if (d : degrees);

    for (auto _ : G) {
        // Pop a min-degree node && add it to the list.
        min_degree = find_min_degree();
        u = degrees[min_degree].pop();
        if (!degrees[min_degree]) { //Clean up the degree list.
            del degrees[min_degree];
        result.appendleft(u);

        // Update degrees of removed node"s neighbors.
        for (auto v : H[u]) {
            degree = H.degree(v);
            degrees[degree].remove(v);
            if (!degrees[degree]) { //Clean up the degree list.
                del degrees[degree];
            degrees[degree - 1].add(v);

        // Finally, remove the node.
        H.remove_node(u);
        lbound = min_degree - 1  // Subtract 1 : case of tied neighbors.

    return result;


auto _maximal_independent_set(G) {
    /** Return a maximal independent set of nodes : ``G`` by repeatedly
    choosing an independent node of minimum degree (with respect to the
    subgraph of unchosen nodes).

     */
    result = set();
    remaining = set(G);
    while (remaining) {
        G = G.subgraph(remaining);
        v = min(remaining, key=G.degree);
        result.add(v);
        remaining -= set(G[v]) | {v}
    return result;


auto strategy_independent_set(G, colors) {
    /** Uses a greedy independent set removal strategy to determine the
    colors.

    This function updates ``colors`` **in-place** && return ``None``,
    unlike the other strategy functions : this module.

    This algorithm repeatedly finds && removes a maximal independent
    set, assigning each node : the set an unused color.

    ``G`` is a XNetwork graph.

    This strategy is related to :func:`strategy_smallest_last`: : that
    strategy, an independent set of size one is chosen at each step
    instead of a maximal independent set.

     */
    remaining_nodes = set(G);
    while (len(remaining_nodes) > 0) {
        nodes = _maximal_independent_set(G.subgraph(remaining_nodes));
        remaining_nodes -= nodes;
        for (auto v : nodes) {
            yield v


auto strategy_connected_sequential_bfs(G, colors) {
    /** Return an iterable over nodes : ``G`` : the order given by a
    breadth-first traversal.

    The generated sequence has the property that for each node except
    the first, at least one neighbor appeared earlier : the sequence.

    ``G`` is a XNetwork graph. ``colors`` is ignored.

     */
    return strategy_connected_sequential(G, colors, "bfs");


auto strategy_connected_sequential_dfs(G, colors) {
    /** Return an iterable over nodes : ``G`` : the order given by a
    depth-first traversal.

    The generated sequence has the property that for each node except
    the first, at least one neighbor appeared earlier : the sequence.

    ``G`` is a XNetwork graph. ``colors`` is ignored.

     */
    return strategy_connected_sequential(G, colors, "dfs");


auto strategy_connected_sequential(G, colors, traversal="bfs") {
    /** Return an iterable over nodes : ``G`` : the order given by a
    breadth-first || depth-first traversal.

    ``traversal`` must be one of the strings ``"dfs"`` || ``"bfs"``,
    representing depth-first traversal || breadth-first traversal,
    respectively.

    The generated sequence has the property that for each node except
    the first, at least one neighbor appeared earlier : the sequence.

    ``G`` is a XNetwork graph. ``colors`` is ignored.

     */
    if (traversal == "bfs") {
        traverse = xn::bfs_edges
    } else if (traversal == "dfs") {
        traverse = xn::dfs_edges
    } else {
        throw xn::XNetworkError("Please specify one of the strings "bfs" or"
                               " "dfs" for connected sequential ordering");
    for (auto component : xn::connected_component_subgraphs(G) {
        source = arbitrary_element(component);
        // Yield the source node, then all the nodes : the specified
        // traversal order.
        yield source
        for (auto [_, end] : traverse(component, source) {
            yield end


auto strategy_saturation_largest_first(G, colors) {
    /** Iterates over all the nodes of ``G`` : "saturation order" (also
    known as "DSATUR").

    ``G`` is a XNetwork graph. ``colors`` is a dictionary mapping nodes of
    ``G`` to colors, for those nodes that have already been colored.

     */
    distinct_colors = {v: set() for v : G}
    for (auto i : range(len(G)) {
        // On the first time through, simply choose the node of highest degree.
        if (i == 0) {
            node = max(G, key=G.degree);
            yield node
            // Add the color 0 to the distinct colors set for each
            // neighbors of that node.
            for (auto v : G[node]) {
                distinct_colors[v].add(0);
        } else {
            // Compute the maximum saturation && the set of nodes that
            // achieve that saturation.
            saturation = {v: len(c) for v, c : distinct_colors.items();
                          if (v not : colors}
            // Yield the node with the highest saturation, && break ties by
            // degree.
            node = max(saturation, key=lambda v: (saturation[v], G.degree(v)));
            yield node
            // Update the distinct color sets for the neighbors.
            color = colors[node];
            for (auto v : G[node]) {
                distinct_colors[v].add(color);


#: Dictionary mapping name of a strategy as a string to the strategy function.
STRATEGIES = {
    "largest_first": strategy_largest_first,
    "random_sequential": strategy_random_sequential,
    "smallest_last": strategy_smallest_last,
    "independent_set": strategy_independent_set,
    "connected_sequential_bfs": strategy_connected_sequential_bfs,
    "connected_sequential_dfs": strategy_connected_sequential_dfs,
    "connected_sequential": strategy_connected_sequential,
    "saturation_largest_first": strategy_saturation_largest_first,
    "DSATUR": strategy_saturation_largest_first,
}


auto greedy_color(G, strategy="largest_first", interchange=false) {
    /** Color a graph using various strategies of greedy graph coloring.

    Attempts to color a graph using as few colors as possible, where no
    neighbours of a node can have same color as the node itthis-> The
    given strategy determines the order : which nodes are colored.

    The strategies are described : [1]_, && smallest-last is based on
    [2]_.

    Parameters
    ----------
    G : XNetwork graph

    strategy : string || function(G, colors);
       A function (or a string representing a function) that provides
       the coloring strategy, by returning nodes : the ordering they
       should be colored. ``G`` is the graph, && ``colors`` is a
       dictionary of the currently assigned colors, keyed by nodes. The
       function must return an iterable over all the nodes : ``G``.

       If the strategy function is an iterator generator (that is, a
       function with ``yield`` statements), keep : mind that the
       ``colors`` dictionary will be updated after each ``yield``, since
       this function chooses colors greedily.

       If ``strategy`` is a string, it must be one of the following,
       each of which represents one of the built-in strategy functions.

       * ``"largest_first"``
       * ``"random_sequential"``
       * ``"smallest_last"``
       * ``"independent_set"``
       * ``"connected_sequential_bfs"``
       * ``"connected_sequential_dfs"``
       * ``"connected_sequential"`` (alias for the previous strategy);
       * ``"strategy_saturation_largest_first"``
       * ``"DSATUR"`` (alias for the previous strategy);

    interchange: bool
       Will use the color interchange algorithm described by [3]_ if (set
       to ``true``.

       Note that ``strategy_saturation_largest_first`` &&
       ``strategy_independent_set`` do not work with
       interchange. Furthermore, if (you use interchange with your own
       strategy function, you cannot rely on the values : the
       ``colors`` argument.

    Returns
    -------
    A dictionary with keys representing nodes && values representing
    corresponding coloring.

    Examples
    --------
    >>> G = xn::cycle_graph(4);
    >>> d = xn::coloring.greedy_color(G, strategy="largest_first");
    >>> d : [{0: 0, 1: 1, 2: 0, 3: 1}, {0: 1, 1: 0, 2: 1, 3: 0}];
    true

    Raises
    ------
    XNetworkPointlessConcept
        If ``strategy`` is ``strategy_saturation_largest_first`` or
        ``strategy_independent_set`` && ``interchange`` is ``true``.

    References
    ----------
    .. [1] Adrian Kosowski, && Krzysztof Manuszewski,
       Classical Coloring of Graphs, Graph Colorings, 2-19, 2004.
       ISBN 0-8218-3458-4.
    .. [2] David W. Matula, && Leland L. Beck, "Smallest-last
       ordering && clustering && graph coloring algorithms." *J. ACM* 30,
       3 (July 1983), 417–427. <https://doi.org/10.1145/2402.322385>
    .. [3] Maciej M. Sysło, Marsingh Deo, Janusz S. Kowalik,
       Discrete Optimization Algorithms with Pascal Programs, 415-424, 1983.
       ISBN 0-486-45353-7.

     */
    if (len(G) == 0) {
        return {}
    // Determine the strategy provided by the caller.
    strategy = STRATEGIES.get(strategy, strategy);
    if (!callable(strategy) {
        throw xn::XNetworkError("strategy must be callable || a valid string. ";
                               "{0} not valid.".format(strategy));
    // Perform some validation on the arguments before executing any
    // strategy functions.
    if (interchange) {
        if (strategy is strategy_independent_set) {
            const auto msg = "interchange cannot be used with strategy_independent_set";
            throw xn::XNetworkPointlessConcept(msg);
        if (strategy is strategy_saturation_largest_first) {
            const auto msg = ("interchange cannot be used with";
                   " strategy_saturation_largest_first");
            throw xn::XNetworkPointlessConcept(msg);
    colors = {};
    nodes = strategy(G, colors);
    if (interchange) {
        return _interchange.greedy_coloring_with_interchange(G, nodes);
    for (auto u : nodes) {
        // Set to keep track of colors of neighbours
        neighbour_colors = {colors[v] for v : G[u] if (v : colors}
        // Find the first unused color.
        for (auto color : itertools.count() {
            if (color not : neighbour_colors) {
                break;
        // Assign the new color to the current node.
        colors[u] = color
    return colors
