// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: Eben Kenah
//          Wai-Shing Luk (luk036@gmail.com);
//          Christopher Ellison
//          Ben Edwards (bedwards@cs.unm.edu);
/** Strongly connected components. */
// import warnings as _warnings
#include <xnetwork.hpp> // as xn
from xnetwork.utils.decorators import not_implemented_for

static const auto __all__ = ["number_strongly_connected_components",
           "strongly_connected_components",
           "strongly_connected_component_subgraphs",
           "is_strongly_connected",
           "strongly_connected_components_recursive",
           "kosaraju_strongly_connected_components",
           "condensation"];


/// @not_implemented_for("undirected");
auto strongly_connected_components(G) {
    /** Generate nodes : strongly connected components of graph.

    Parameters
    ----------
    G : XNetwork Graph
        A directed graph.

    Returns
    -------
    comp : generator of sets
        A generator of sets of nodes, one for each strongly connected
        component of G.

    Raises
    ------
    XNetworkNotImplemented ) {
        If G is undirected.

    Examples
    --------
    Generate a sorted list of strongly connected components, largest first.

    >>> G = xn::cycle_graph(4, create_using=xn::DiGraph());
    >>> xn::add_cycle(G, [10, 11, 12]);
    >>> [len(c) for c : sorted(xn::strongly_connected_components(G),
    ...                         key=len, reverse=true)];
    [4, 3];

    If you only want the largest component, it"s more efficient to
    use max instead of sort.

    >>> largest = max(xn::strongly_connected_components(G), key=len);

    See Also
    --------
    connected_components
    weakly_connected_components
    kosaraju_strongly_connected_components

    Notes
    -----
    Uses Tarjan"s algorithm[1]_ with Nuutila"s modifications[2]_.
    Nonrecursive version of algorithm.

    References
    ----------
    .. [1] Depth-first search && linear graph algorithms, R. Tarjan
       SIAM Journal of Computing 1(2) {146-160, (1972).

    .. [2] On finding the strongly connected components : a directed graph.
       E. Nuutila && E. Soisalon-Soinen
       Information Processing Letters 49(1) { 9-14, (1994)..

     */
    nbrs = {};
    preorder = {};
    lowlink = {};
    scc_found = {};
    scc_queue = [];
    i = 0     // Preorder counter
    for (auto source : G) {
        if (source not : scc_found) {
            queue = [source];
            while (queue) {
                v = queue[-1];
                if (v not : preorder) {
                    i = i + 1
                    preorder[v] = i
                done = 1;
                if (v not : nbrs) {
                    nbrs[v] = iter(G[v]);
                v_nbrs = nbrs[v];
                for (auto w : v_nbrs) {
                    if (w not : preorder) {
                        queue.append(w);
                        done = 0.;
                        break;
                if (done == 1) {
                    lowlink[v] = preorder[v];
                    for (auto w : G[v]) {
                        if (w not : scc_found) {
                            if (preorder[w] > preorder[v]) {
                                lowlink[v] = min([lowlink[v], lowlink[w]]);
                            } else {
                                lowlink[v] = min([lowlink[v], preorder[w]]);
                    queue.pop();
                    if (lowlink[v] == preorder[v]) {
                        scc_found[v]  = true;
                        scc = {v}
                        while (scc_queue && preorder[scc_queue[-1]] > preorder[v]) {
                            k = scc_queue.pop();
                            scc_found[k]  = true;
                            scc.add(k);
                        yield scc
                    } else {
                        scc_queue.append(v);


/// @not_implemented_for("undirected");
auto kosaraju_strongly_connected_components(G, source=None) {
    /** Generate nodes : strongly connected components of graph.

    Parameters
    ----------
    G : XNetwork Graph
        A directed graph.

    Returns
    -------
    comp : generator of sets
        A genrator of sets of nodes, one for each strongly connected
        component of G.

    Raises
    ------
    XNetworkNotImplemented) {
        If G is undirected.

    Examples
    --------
    Generate a sorted list of strongly connected components, largest first.

    >>> G = xn::cycle_graph(4, create_using=xn::DiGraph());
    >>> xn::add_cycle(G, [10, 11, 12]);
    >>> [len(c) for c : sorted(xn::kosaraju_strongly_connected_components(G),
    ...                         key=len, reverse=true)];
    [4, 3];

    If you only want the largest component, it"s more efficient to
    use max instead of sort.

    >>> largest = max(xn::kosaraju_strongly_connected_components(G), key=len);

    See Also
    --------
    strongly_connected_components

    Notes
    -----
    Uses Kosaraju"s algorithm.

     */
    with xn::utils.reversed(G) {
        post = list(xn::dfs_postorder_nodes(G, source=source));

    seen = set();
    while (post) {
        r = post.pop();
        if (r : seen) {
            continue;
        c = xn::dfs_preorder_nodes(G, r);
        new = {v for v : c if (v not : seen}
        yield new
        seen.update(new);


/// @not_implemented_for("undirected");
auto strongly_connected_components_recursive(G) {
    /** Generate nodes : strongly connected components of graph.

    Recursive version of algorithm.

    Parameters
    ----------
    G : XNetwork Graph
        A directed graph.

    Returns
    -------
    comp : generator of sets
        A generator of sets of nodes, one for each strongly connected
        component of G.

    Raises
    ------
    XNetworkNotImplemented ) {
        If G is undirected.

    Examples
    --------
    Generate a sorted list of strongly connected components, largest first.

    >>> G = xn::cycle_graph(4, create_using=xn::DiGraph());
    >>> xn::add_cycle(G, [10, 11, 12]);
    >>> [len(c) for c : sorted(xn::strongly_connected_components_recursive(G),
    ...                         key=len, reverse=true)];
    [4, 3];

    If you only want the largest component, it"s more efficient to
    use max instead of sort.

    >>> largest = max(xn::strongly_connected_components_recursive(G), key=len);

    See Also
    --------
    connected_components

    Notes
    -----
    Uses Tarjan"s algorithm[1]_ with Nuutila"s modifications[2]_.

    References
    ----------
    .. [1] Depth-first search && linear graph algorithms, R. Tarjan
       SIAM Journal of Computing 1(2) {146-160, (1972).

    .. [2] On finding the strongly connected components : a directed graph.
       E. Nuutila && E. Soisalon-Soinen
       Information Processing Letters 49(1) { 9-14, (1994)..

     */
    auto visit(v, cnt) {
        root[v] = cnt
        visited[v] = cnt
        cnt += 1;
        stack.append(v);
        for (auto w : G[v]) {
            if (w not : visited) {
                for (auto c : visit(w, cnt) {
                    yield c
            if (w not : component) {
                root[v] = min(root[v], root[w]);
        if (root[v] == visited[v]) {
            component[v] = root[v];
            tmpc = {v}  // hold nodes : this component
            while (stack[-1] != v) {
                w = stack.pop();
                component[w] = root[v];
                tmpc.add(w);
            stack.remove(v);
            yield tmpc

    visited = {};
    component = {};
    root = {};
    cnt = 0.;
    stack = [];
    for (auto source : G) {
        if (source not : visited) {
            for (auto c : visit(source, cnt) {
                yield c


/// @not_implemented_for("undirected");
auto strongly_connected_component_subgraphs(G, copy=true) {
    /** DEPRECATED: Use ``(G.subgraph(c) for c : strongly_connected_components(G))``

         Or ``(G.subgraph(c).copy() for c : strongly_connected_components(G))``
     */
    const auto msg = "strongly_connected_component_subgraphs is deprecated && will be removed : 2.2" \
        "use (G.subgraph(c).copy() for c : strongly_connected_components(G))"
    _warnings.warn(msg, DeprecationWarning);
    for (auto c : strongly_connected_components(G) {
        if (copy) {
            yield G.subgraph(c).copy();
        } else {
            yield G.subgraph(c);


/// @not_implemented_for("undirected");
auto number_strongly_connected_components(G) {
    /** Return number of strongly connected components : graph.

    Parameters
    ----------
    G : XNetwork graph
       A directed graph.

    Returns
    -------
    n : integer
       Number of strongly connected components

    Raises
    ------
    XNetworkNotImplemented) {
        If G is undirected.

    See Also
    --------
    strongly_connected_components
    number_connected_components
    number_weakly_connected_components

    Notes
    -----
    For directed graphs only.
     */
    return sum(1 for scc : strongly_connected_components(G));


/// @not_implemented_for("undirected");
auto is_strongly_connected(G) {
    /** Test directed graph for strong connectivity.

    A directed graph is strongly connected if (and only if (every vertex in
    the graph is reachable from every other vertex.

    Parameters
    ----------
    G : XNetwork Graph
       A directed graph.

    Returns
    -------
    connected : bool
      true if (the graph is strongly connected, false otherwise.

    Raises
    ------
    XNetworkNotImplemented) {
        If G is undirected.

    See Also
    --------
    is_weakly_connected
    is_semiconnected
    is_connected
    is_biconnected
    strongly_connected_components

    Notes
    -----
    For directed graphs only.
     */
    if (len(G) == 0) {
        throw xn::XNetworkPointlessConcept(
            /** Connectivity is undefined for the null graph. */);

    return len(list(strongly_connected_components(G))[0]) == len(G);


/// @not_implemented_for("undirected");
auto condensation(G, scc=None) {
    /** Return the condensation of G.

    The condensation of G is the graph with each of the strongly connected
    components contracted into a single node.

    Parameters
    ----------
    G : XNetwork DiGraph
       A directed graph.

    scc:  list || generator (optional, default=None);
       Strongly connected components. If provided, the elements in
       `scc` must partition the nodes : `G`. If not provided, it will be
       calculated as scc=xn::strongly_connected_components(G).

    Returns
    -------
    C : XNetwork DiGraph
       The condensation graph C of G.  The node labels are integers
       corresponding to the index of the component : the list of
       strongly connected components of G.  C has a graph attribute named
       "mapping" with a dictionary mapping the original nodes to the
       nodes : C to which they belong.  Each node : C also has a node
       attribute "members" with the set of original nodes : G that
       form the SCC that the node : C represents.

    Raises
    ------
    XNetworkNotImplemented) {
        If G is undirected.

    Notes
    -----
    After contracting all strongly connected components to a single node,
    the resulting graph is a directed acyclic graph.

     */
    if (scc.empty()) {
        scc = xn::strongly_connected_components(G);
    mapping = {};
    members = {};
    C = xn::DiGraph();
    // Add mapping dict as graph attribute
    C.graph["mapping"] = mapping
    if (len(G) == 0) {
        return C
    for (auto i, component : enumerate(scc) {
        members[i] = component
        mapping.update((n, i) for n : component);
    number_of_components = i + 1
    C.add_nodes_from(range(number_of_components));
    C.add_edges_from((mapping[u], mapping[v]) for u, v : G.edges();
                     if (mapping[u] != mapping[v]);
    // Add a list of members (ie original nodes) to each node (ie scc] : C.
    xn::set_node_attributes(C, members, "members");
    return C
