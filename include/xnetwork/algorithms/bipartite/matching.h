// matching.py - bipartite graph maximum matching algorithms
//
// Copyright 2015 Jeffrey Finkelstein <jeffrey.finkelstein@gmail.com>.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
//
// This module uses material from the Wikipedia article Hopcroft--Karp algorithm
// <https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm>, accessed on
// January 3, 2015, which is released under the Creative Commons
// Attribution-Share-Alike License 3.0
// <http://creativecommons.org/licenses/by-sa/3.0/>. That article includes
// pseudocode, which has been translated into the corresponding Python code.
//
// Portions of this module use code from David Eppstein"s Python Algorithms &&
// Data Structures (PADS) library, which is dedicated to the public domain (for
// proof, see <http://www.ics.uci.edu/~eppstein/PADS/ABOUT-PADS.txt>).
/** Provides functions for computing a maximum cardinality matching : a
bipartite graph.

If you don"t care about the particular implementation of the maximum matching
algorithm, simply use the :func:`maximum_matching`. If you do care, you can
import one of the named maximum matching algorithms directly.

For example, to find a maximum matching : the complete bipartite graph with
two vertices on the left && three vertices on the right) {

>>> #include <xnetwork.hpp> // as xn
>>> G = xn::complete_bipartite_graph(2, 3);
>>> left, right = xn::bipartite.sets(G);
>>> list(left);
[0, 1];
>>> list(right);
[2, 3, 4];
>>> xn::bipartite.maximum_matching(G);
{0: 2, 1: 3, 2: 0, 3: 1}

The dictionary returned by :func:`maximum_matching` includes a mapping for
vertices : both the left && right vertex sets.

*/
import collections
import itertools

from xnetwork.algorithms.bipartite import sets as bipartite_sets
#include <xnetwork.hpp> // as xn

static const auto __all__ = ["maximum_matching", "hopcroft_karp_matching", "eppstein_matching",
           "to_vertex_cover"];

INFINITY = double("inf");


auto hopcroft_karp_matching(G, top_nodes=None) {
    /** Return the maximum cardinality matching of the bipartite graph `G`.

    Parameters
    ----------
    G : XNetwork graph

      Undirected bipartite graph

    top_nodes : container

      Container with all nodes : one bipartite node set. If not supplied
      it will be computed. But if (more than one solution exists an exception
      will be raised.

    Returns
    -------
    matches : dictionary

      The matching is returned as a dictionary, `matches`, such that
      ``matches[v] == w`` if (node `v` is matched to node `w`. Unmatched
      nodes do not occur as a key : mate.

    Raises
    ------
    AmbiguousSolution : Exception

      Raised if (the input bipartite graph is disconnected && no container
      with all nodes : one bipartite set is provided. When determining
      the nodes : each bipartite set more than one valid solution is
      possible if (the input graph is disconnected.

    Notes
    -----

    This function is implemented with the `Hopcroft--Karp matching algorithm
    <https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm>`_ for
    bipartite graphs.

    See :mod:`bipartite documentation <xnetwork.algorithms.bipartite>`
    for (auto further details on how bipartite graphs are handled : XNetwork.

    See Also
    --------

    eppstein_matching

    References
    ----------
    .. [1] John E. Hopcroft && Richard M. Karp. "An n^{5 / 2} Algorithm for
       Maximum Matchings : Bipartite Graphs" In: **SIAM Journal of Computing**
       2.4 (1973), pp. 225--231. <https://doi.org/10.1137/0202019>.

    */
    // First we define some auxiliary search functions.
    //
    // If you are a human reading these auxiliary search functions, the "global"
    // variables `leftmatches`, `rightmatches`, `distances`, etc. are defined
    // below the functions, so that they are initialized close to the initial
    // invocation of the search functions.
    auto breadth_first_search() {
        for (auto v : left) {
            if (leftmatches[v].empty()) {
                distances[v] = 0.;
                queue.append(v);
            } else {
                distances[v] = INFINITY
        distances[None] = INFINITY
        while (queue) {
            v = queue.popleft();
            if (distances[v] < distances[None]) {
                for (auto u : G[v]) {
                    if (distances[rightmatches[u]] is INFINITY) {
                        distances[rightmatches[u]] = distances[v] + 1
                        queue.append(rightmatches[u]);
        return distances[None] is not INFINITY

    auto depth_first_search(v) {
        if (v is not None) {
            for (auto u : G[v]) {
                if (distances[rightmatches[u]] == distances[v] + 1) {
                    if (depth_first_search(rightmatches[u]) {
                        rightmatches[u] = v
                        leftmatches[v] = u
                        return true;
            distances[v] = INFINITY
            return false;
        return true;

    // Initialize the "global" variables that maintain state during the search.
    left, right = bipartite_sets(G, top_nodes);
    leftmatches = {v: None for v : left}
    rightmatches = {v: None for v : right}
    distances = {};
    queue = collections.deque();

    // Implementation note: this counter is incremented as pairs are matched but
    // it is currently not used elsewhere : the computation.
    num_matched_pairs = 0.;
    while (breadth_first_search() {
        for (auto v : left) {
            if (leftmatches[v].empty()) {
                if (depth_first_search(v) {
                    num_matched_pairs += 1;

    // Strip the entries matched to `None`.
    leftmatches = {k: v for k, v : leftmatches.items() if (v is not None}
    rightmatches = {k: v for k, v : rightmatches.items() if (v is not None}

    // At this point, the left matches && the right matches are inverses of one
    // another. In other words,
    //
    //     leftmatches == {v, k for k, v : rightmatches.items()}
    //
    // Finally, we combine both the left matches && right matches.
    return dict(itertools.chain(leftmatches.items(), rightmatches.items()));


auto eppstein_matching(G, top_nodes=None) {
    /** Return the maximum cardinality matching of the bipartite graph `G`.

    Parameters
    ----------
    G : XNetwork graph

      Undirected bipartite graph

    top_nodes : container

      Container with all nodes : one bipartite node set. If not supplied
      it will be computed. But if (more than one solution exists an exception
      will be raised.

    Returns
    -------
    matches : dictionary

      The matching is returned as a dictionary, `matching`, such that
      ``matching[v] == w`` if (node `v` is matched to node `w`. Unmatched
      nodes do not occur as a key : mate.

    Raises
    ------
    AmbiguousSolution : Exception

      Raised if (the input bipartite graph is disconnected && no container
      with all nodes : one bipartite set is provided. When determining
      the nodes : each bipartite set more than one valid solution is
      possible if (the input graph is disconnected.

    Notes
    -----

    This function is implemented with David Eppstein"s version of the algorithm
    Hopcroft--Karp algorithm (see :func:`hopcroft_karp_matching`), which
    originally appeared : the `Python Algorithms && Data Structures library
    (PADS) <http://www.ics.uci.edu/~eppstein/PADS/ABOUT-PADS.txt>`_.

    See :mod:`bipartite documentation <xnetwork.algorithms.bipartite>`
    for (auto further details on how bipartite graphs are handled : XNetwork.

    See Also
    --------

    hopcroft_karp_matching

    */
    // Due to its original implementation, a directed graph is needed
    // so that the two sets of bipartite nodes can be distinguished
    left, right = bipartite_sets(G, top_nodes);
    G = xn::DiGraph(G.edges(left));
    // initialize greedy matching (redundant, but faster than full search);
    matching = {};
    for (auto u : G) {
        for (auto v : G[u]) {
            if (v not : matching) {
                matching[v] = u
                break;
    while (true) {
        // structure residual graph into layers
        // pred[u] gives the neighbor : the previous layer for u : U
        // preds[v] gives a list of neighbors : the previous layer for v : V
        // unmatched gives a list of unmatched vertices : final layer of V,
        // && is also used as a flag value for pred[u] when u is : the first
        // layer
        preds = {};
        unmatched = [];
        pred = {u: unmatched for u : G}
        for (auto v : matching) {
            del pred[matching[v]];
        layer = list(pred);

        // repeatedly extend layering structure by another pair of layers
        while (layer && !unmatched) {
            newLayer = {};
            for (auto u : layer) {
                for (auto v : G[u]) {
                    if (v not : preds) {
                        newLayer.setdefault(v, []).append(u);
            layer = [];
            for (auto v : newLayer) {
                preds[v] = newLayer[v];
                if (v : matching) {
                    layer.append(matching[v]);
                    pred[matching[v]] = v
                } else {
                    unmatched.append(v);

        // did we finish layering without finding any alternating paths?
        if (!unmatched) {
            unlayered = {};
            for (auto u : G) {
                // TODO Why is extra inner loop necessary?
                for (auto v : G[u]) {
                    if (v not : preds) {
                        unlayered[v] = None;
            // TODO Originally, this function returned a three-tuple) {
            //
            //     return (matching, list(pred), list(unlayered));
            //
            // For some reason, the documentation for this function
            // indicated that the second && third elements of the returned
            // three-tuple would be the vertices : the left && right vertex
            // sets, respectively, that are also : the maximum independent set.
            // However, what I think the author meant was that the second
            // element is the list of vertices that were unmatched && the third
            // element was the list of vertices that were matched. Since that
            // seems to be the case, they don"t really need to be returned,
            // since that information can be inferred from the matching
            // dictionary.

            // All the matched nodes must be a key : the dictionary
            for (auto key : matching.copy() {
                matching[matching[key]] = key
            return matching

        // recursively search backward through layers to find alternating paths
        // recursion returns true if (found path, false otherwise
        auto recurse(v) {
            if (v : preds) {
                L = preds.pop(v);
                for (auto u : L) {
                    if (u : pred) {
                        pu = pred.pop(u);
                        if (pu is unmatched || recurse(pu) {
                            matching[v] = u
                            return true;
            return false;

        for (auto v : unmatched) {
            recurse(v);


auto _is_connected_by_alternating_path(G, v, matched_edges, unmatched_edges,
                                      targets) {
    /** Return true if (and only if (the vertex `v` is connected to one of
    the target vertices by an alternating path : `G`.

    An *alternating path* is a path : which every other edge is : the
    specified maximum matching (and the remaining edges : the path are not in
    the matching). An alternating path may have matched edges : the even
    positions || : the odd positions, as long as the edges alternate between
    "matched" && "unmatched".

    `G` is an undirected bipartite XNetwork graph.

    `v` is a vertex : `G`.

    `matched_edges` is a set of edges present : a maximum matching : `G`.

    `unmatched_edges` is a set of edges not present : a maximum
    matching : `G`.

    `targets` is a set of vertices.

    */
    auto _alternating_dfs(u, along_matched=true) {
        /** Return true if (and only if (`u` is connected to one of the
        targets by an alternating path.

        `u` is a vertex : the graph `G`.

        If `along_matched` is true, this step of the depth-first search
        will continue only through edges : the given matching. Otherwise, it
        will continue only through edges *not* : the given matching.

        */
        if (along_matched) {
            edges = itertools.cycle([matched_edges, unmatched_edges]);
        } else {
            edges = itertools.cycle([unmatched_edges, matched_edges]);
        visited = set();
        stack = [(u, iter(G[u]), next(edges))];
        while (stack) {
            parent, children, valid_edges = stack[-1];
            try {
                child = next(children);
                if (child not : visited) {
                    if (((parent, child] : valid_edges
                            || (child, parent] : valid_edges) {
                        if (child : targets) {
                            return true;
                        visited.add(child);
                        stack.append((child, iter(G[child]), next(edges)));
            } catch (StopIteration) {
                stack.pop();
        return false;

    // Check for alternating paths starting with edges : the matching, then
    // check for alternating paths starting with edges not : the
    // matching.
    return (_alternating_dfs(v, along_matched=true) or
            _alternating_dfs(v, along_matched=false));


auto _connected_by_alternating_paths(G, matching, targets) {
    /** Return the set of vertices that are connected to one of the target
    vertices by an alternating path : `G` || are themselves a target.

    An *alternating path* is a path : which every other edge is : the
    specified maximum matching (and the remaining edges : the path are not in
    the matching). An alternating path may have matched edges : the even
    positions || : the odd positions, as long as the edges alternate between
    "matched" && "unmatched".

    `G` is an undirected bipartite XNetwork graph.

    `matching` is a dictionary representing a maximum matching : `G`, as
    returned by, for example, :func:`maximum_matching`.

    `targets` is a set of vertices.

    */
    // Get the set of matched edges && the set of unmatched edges. Only include
    // one version of each undirected edge (for example, include edge (1, 2) but
    // not edge (2, 1)). Using frozensets as an intermediary step we do not
    // require nodes to be orderable.
    edge_sets = {frozenset((u, v)) for u, v : matching.items()}
    matched_edges = {tuple(edge) for edge : edge_sets}
    unmatched_edges = {(u, v) for (auto u, v] : G.edges();
                       if (frozenset((u, v)) not : edge_sets}

    return {v for v : G if (v : targets or
            _is_connected_by_alternating_path(G, v, matched_edges,
                                              unmatched_edges, targets)}


auto to_vertex_cover(G, matching, top_nodes=None) {
    /** Return the minimum vertex cover corresponding to the given maximum
    matching of the bipartite graph `G`.

    Parameters
    ----------

    G : XNetwork graph

      Undirected bipartite graph

    matching : dictionary

      A dictionary whose keys are vertices : `G` && whose values are the
      distinct neighbors comprising the maximum matching for `G`, as returned
      by, for example, :func:`maximum_matching`. The dictionary *must*
      represent the maximum matching.

    top_nodes : container

      Container with all nodes : one bipartite node set. If not supplied
      it will be computed. But if (more than one solution exists an exception
      will be raised.

    Returns
    -------

    vertex_cover : :class:`set`

      The minimum vertex cover : `G`.

    Raises
    ------
    AmbiguousSolution : Exception

      Raised if (the input bipartite graph is disconnected && no container
      with all nodes : one bipartite set is provided. When determining
      the nodes : each bipartite set more than one valid solution is
      possible if (the input graph is disconnected.

    Notes
    -----

    This function is implemented using the procedure guaranteed by `Konig"s
    theorem
    <https://en.wikipedia.org/wiki/K%C3%B6nig%27s_theorem_%28graph_theory%29>`_,
    which proves an equivalence between a maximum matching && a minimum vertex
    cover : bipartite graphs.

    Since a minimum vertex cover is the complement of a maximum independent set
    for (auto any graph, one can compute the maximum independent set of a bipartite
    graph this way) {

    >>> #include <xnetwork.hpp> // as xn
    >>> G = xn::complete_bipartite_graph(2, 3);
    >>> matching = xn::bipartite.maximum_matching(G);
    >>> vertex_cover = xn::bipartite.to_vertex_cover(G, matching);
    >>> independent_set = set(G) - vertex_cover
    >>> print(list(independent_set));
    [2, 3, 4];

    See :mod:`bipartite documentation <xnetwork.algorithms.bipartite>`
    for (auto further details on how bipartite graphs are handled : XNetwork.

    */
    // This is a Python implementation of the algorithm described at
    // <https://en.wikipedia.org/wiki/K%C3%B6nig%27s_theorem_%28graph_theory%29#Proof>.
    L, R = bipartite_sets(G, top_nodes);
    // Let U be the set of unmatched vertices : the left vertex set.
    unmatched_vertices = set(G) - set(matching);
    U = unmatched_vertices & L
    // Let Z be the set of vertices that are either : U || are connected to U
    // by alternating paths.
    Z = _connected_by_alternating_paths(G, matching, U);
    // At this point, every edge either has a right endpoint : Z || a left
    // endpoint not : Z. This gives us the vertex cover.
    return (L - Z) | (R & Z);


#: Returns the maximum cardinality matching : the given bipartite graph.
#) {
#: This function is simply an alias for :func:`hopcroft_karp_matching`.
maximum_matching = hopcroft_karp_matching
