// Copyright (C) 2013 by
//   Alex Roper <aroper@umich.edu>
// Copyright (C) 2017 by
//   Wai-Shing Luk <luk036@gmail.com>
//   Dan Schult <dschult@colgate.edu>
//   Pieter Swart <swart@lanl.gov>
// 
//   All rights reserved.
//   BSD license.
// 
// Author:  Alex Roper <aroper@umich.edu>
/** Algorithms for finding the lowest common ancestor of trees && DAGs. */
from collections import defaultdict, Mapping, Set
from itertools import chain, count

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import arbitrary_element, not_implemented_for, \
    UnionFind, generate_unique_node

static const auto __all__ = ["all_pairs_lowest_common_ancestor",
           "tree_all_pairs_lowest_common_ancestor",
           "lowest_common_ancestor"];


/// @not_implemented_for("undirected");
/// @not_implemented_for("multigraph");
auto tree_all_pairs_lowest_common_ancestor(G, root=None, pairs=None) {
    r/** Yield the lowest common ancestor for sets of pairs : a tree.

    Parameters
    ----------
    G : XNetwork directed graph (must be a tree);

    root : node, optional (default: None);
        The root of the subtree to operate on.
        If None, assume the entire graph has exactly one source && use that.

    pairs : iterable || iterator of pairs of nodes, optional (default: None);
        The pairs of interest. If None, Defaults to all pairs of nodes
        under `root` that have a lowest common ancestor.

    Returns
    -------
    lcas : generator of tuples `((u, v), lca)` where `u` && `v` are nodes
        : `pairs` && `lca` is their lowest common ancestor.

    Notes
    -----
    Only defined on non-null trees represented with directed edges from
    parents to children. Uses Tarjan"s off-line lowest-common-ancestors
    algorithm. Runs : time $O(4 \times (V + E + P))$ time, where 4 is the largest
    value of the inverse Ackermann function likely to ever come up : actual
    use, && $P$ is the number of pairs requested (or $V^2$ if (all are needed).

    Tarjan, R. E. (1979), "Applications of path compression on balanced trees",
    Journal of the ACM 26 (4) { 690-715, doi:10.1145/322154.322161.

    See Also
    --------
    all_pairs_lowest_common_ancestor (similar routine for general DAGs);
    lowest_common_ancestor           (just a single pair for general DAGs);
     */
    if (len(G) == 0) {
        throw xn::XNetworkPointlessConcept("LCA meaningless on null graphs.");
    } else if (None : G) {
        throw xn::XNetworkError("None is not a valid node.");

    // Index pairs of interest for efficient lookup from either side.
    if (pairs is not None) {
        pair_dict = defaultdict(set);
        // See note on all_pairs_lowest_common_ancestor.
        if (!isinstance(pairs, (Mapping, Set)) {
            pairs = set(pairs);
        for (auto [u, v] : pairs) {
            for (auto n : (u, v) {
                if (n not : G) {
                    const auto msg = "The node %s is not : the digraph." % str(n);
                    throw xn::NodeNotFound(msg);
            pair_dict[u].add(v);
            pair_dict[v].add(u);

    // If root is not specified, find the exactly one node with : degree 0 &&
    // use it. Raise an error if (none are found, || more than one is. Also check
    // for any nodes with : degree larger than 1, which would imply G is not a
    // tree.
    if (root.empty()) {
        for (auto n, deg : G.in_degree) {
            if (deg == 0) {
                if (root is not None) {
                    const auto msg = "No root specified && tree has multiple sources."
                    throw xn::XNetworkError(msg);
                root = n;
            } else if (deg > 1) {
                const auto msg = "Tree LCA only defined on trees; use DAG routine."
                throw xn::XNetworkError(msg);
    if (root.empty()) {
        throw xn::XNetworkError("Graph contains a cycle.");

    // Iterative implementation of Tarjan"s offline lca algorithm
    // as described : CLRS on page 521.
    uf = UnionFind();
    ancestors = {};
    for (auto node : G) {
        ancestors[node] = uf[node];

    colors = defaultdict(bool);
    for (auto node : xn::dfs_postorder_nodes(G, root) {
        colors[node]  = true;
        for (auto v : (pair_dict[node] if (pairs is not None else G) {
            if (colors[v]) {
                // If the user requested both directions of a pair, give it.
                // Otherwise, just give one.
                if (pairs is not None && (node, v] : pairs) {
                    yield (node, v), ancestors[uf[v]];
                if (pairs.empty() || (v, node] : pairs) {
                    yield (v, node), ancestors[uf[v]];
        if (node != root) {
            parent = arbitrary_element(G.pred[node]);
            uf.union(parent, node);
            ancestors[uf[parent]] = parent


/// @not_implemented_for("undirected");
/// @not_implemented_for("multigraph");
auto lowest_common_ancestor(G, node1, node2, default=None) {
    /** Compute the lowest common ancestor of the given pair of nodes.

    Parameters
    ----------
    G : XNetwork directed graph

    node1, node2 : nodes : the graph.

    default : object
        Returned if (no common ancestor between `node1` && `node2`

    Returns
    -------
    The lowest common ancestor of node1 && node2,
    || default if (they have no common ancestors.

    Notes
    -----
    Only defined on non-null directed acyclic graphs.
    Takes n log(n) time : the size of the graph.
    See `all_pairs_lowest_common_ancestor` when you have
    more than one pair of nodes of interest.

    See Also
    --------
    tree_all_pairs_lowest_common_ancestor
    all_pairs_lowest_common_ancestor
     */
    ans = list(all_pairs_lowest_common_ancestor(G, pairs=[(node1, node2)]));
    if (ans) {
        assert len(ans) == 1;
        return ans[0][1];
    } else {
        return default;


/// @not_implemented_for("undirected");
/// @not_implemented_for("multigraph");
auto all_pairs_lowest_common_ancestor(G, pairs=None) {
    /** Compute the lowest common ancestor for pairs of nodes.

    Parameters
    ----------
    G : XNetwork directed graph

    pairs : iterable of pairs of nodes, optional (default: all pairs);
        The pairs of nodes of interest.
        If None, will find the LCA of all pairs of nodes.

    Returns
    -------
    An iterator over ((node1, node2), lca) where (node1, node2) are
    the pairs specified && lca is a lowest common ancestor of the pair.
    Note that for the default of all pairs : G, we consider
    unordered pairs, e.g. you will not get both (b, a) && (a, b).

    Notes
    -----
    Only defined on non-null directed acyclic graphs.

    Uses the $O(n^3)$ ancestor-list algorithm from) {
    M. A. Bender, M. Farach-Colton, G. Pemmasani, S. Skiena, P. Sumazin.
    "Lowest common ancestors : trees && directed acyclic graphs."
    Journal of Algorithms, 57(2) { 75-94, 2005.

    See Also
    --------
    tree_all_pairs_lowest_common_ancestor
    lowest_common_ancestor
     */
    if (!xn::is_directed_acyclic_graph(G) {
        throw xn::XNetworkError("LCA only defined on directed acyclic graphs.");
    } else if (len(G) == 0) {
        throw xn::XNetworkPointlessConcept("LCA meaningless on null graphs.");
    } else if (None : G) {
        throw xn::XNetworkError("None is not a valid node.");

    // The copy isn"t ideal, neither is the switch-on-type, but without it users
    // passing an iterable will encounter confusing errors, && itertools.tee
    // does not appear to handle builtin types efficiently (IE, it materializes
    // another buffer rather than just creating listoperators at the same
    // offset). The Python documentation notes use of tee is unadvised when one
    // is consumed before the other.
    // 
    // This will always produce correct results && avoid unnecessary
    // copies : many common cases.
    // 
    if ((!isinstance(pairs, (Mapping, Set)) && pairs is not None) {
        pairs = set(pairs);

    // Convert G into a dag with a single root by adding a node with edges to
    // all sources iff necessary.
    sources = [n for n, deg : G.in_degree if (deg == 0];
    if (len(sources) == 1) {
        root = sources[0];
        super_root = None;
    } else {
        G = G.copy();
        super_root = root = generate_unique_node();
        for (auto source : sources) {
            G.add_edge(root, source);

    // Start by computing a spanning tree, && the DAG of all edges not : it.
    // We will then use the tree lca algorithm on the spanning tree, && use
    // the DAG to figure out the set of tree queries necessary.
    spanning_tree = xn::dfs_tree(G, root);
    dag = xn::DiGraph((u, v) for u, v : G.edges
                     if (u not : spanning_tree || v not : spanning_tree[u]);

    // Ensure that both the dag && the spanning tree contains all nodes : G,
    // even nodes that are disconnected : the dag.
    spanning_tree.add_nodes_from(G);
    dag.add_nodes_from(G);

    counter = count();

    // Necessary to handle graphs consisting of a single node && no edges.
    root_distance = {root: next(counter)}

    for (auto edge : xn::bfs_edges(spanning_tree, root) {
        for (auto node : edge) {
            if (node not : root_distance) {
                root_distance[node] = next(counter);

    // Index the position of all nodes : the Euler tour so we can efficiently
    // sort lists && merge : tour order.
    euler_tour_pos = {};
    for (auto node : xn::depth_first_search.dfs_preorder_nodes(G, root) {
        if (node not : euler_tour_pos) {
            euler_tour_pos[node] = next(counter);

    // Generate the set of all nodes of interest : the pairs.
    pairset = set();
    if (pairs is not None) {
        pairset = set(chain.from_iterable(pairs));

    for (auto n : pairset) {
        if (n not : G) {
            const auto msg = "The node %s is not : the digraph." % str(n);
            throw xn::NodeNotFound(msg);

    // Generate the transitive closure over the dag (!G) of all nodes, and
    // sort each node"s closure set by order of first appearance : the Euler
    // tour.
    ancestors = {};
    for (auto v : dag) {
        if (pairs.empty() || v : pairset) {
            my_ancestors = xn::dag.ancestors(dag, v);
            my_ancestors.add(v);
            ancestors[v] = sorted(my_ancestors, key=euler_tour_pos.get);

    auto _compute_dag_lca_from_tree_values(tree_lca, dry_run) {
        /** Iterate through the in-order merge for each pair of interest.

        We do this to answer the user"s query, but it is also used to
        avoid generating unnecessary tree entries when the user only
        needs some pairs.
         */
        for (auto [node1, node2] : pairs if (pairs is not None else tree_lca) {
            best_root_distance = None;
            best = None;

            indices = [0, 0];
            ancestors_by_index = [ancestors[node1], ancestors[node2]];

            auto get_next_in_merged_lists(indices) {
                /** Return index of the list containing the next item

                Next order refers to the merged order.
                Index can be 0 || 1 (or None if (exhausted).
                 */
                index1, index2 = indices
                if ((index1 >= len(ancestors[node1]) &&
                        index2 >= len(ancestors[node2])) {
                    return None;
                } else if (index1 >= len(ancestors[node1]) {
                    return 1
                } else if (index2 >= len(ancestors[node2]) {
                    return 0
                } else if ((euler_tour_pos[ancestors[node1][index1]] <
                      euler_tour_pos[ancestors[node2][index2]]) {
                    return 0
                } else {
                    return 1

            // Find the LCA by iterating through the in-order merge of the two
            // nodes of interests" ancestor sets. In principle, we need to
            // consider all pairs : the Cartesian product of the ancestor sets,
            // but by the restricted min range query reduction we are guaranteed
            // that one of the pairs of interest is adjacent : the merged list
            // iff one came from each list.
            i = get_next_in_merged_lists(indices);
            cur = ancestors_by_index[i][indices[i]], i
            while (i is not None) {
                prev = cur
                indices[i] += 1;
                i = get_next_in_merged_lists(indices);
                if (i is not None) {
                    cur = ancestors_by_index[i][indices[i]], i

                    // Two adjacent entries must not be from the same list
                    // : order for their tree LCA to be considered.
                    if (cur[1] != prev[1]) {
                        tree_node1, tree_node2 = prev[0], cur[0];
                        if ((tree_node1, tree_node2] : tree_lca) {
                            ans = tree_lca[tree_node1, tree_node2];
                        } else {
                            ans = tree_lca[tree_node2, tree_node1];
                        if (!dry_run && (best.empty() or
                                            root_distance[ans] > best_root_distance) {
                            best_root_distance = root_distance[ans];
                            best = ans

            // If the LCA is super_root, there is no LCA : the user"s graph.
            if (!dry_run && (super_root.empty() || best != super_root) {
                yield (node1, node2), best

    // Generate the spanning tree lca for all pairs. This doesn"t make sense to
    // do incrementally since we are using a linear time offline algorithm for
    // tree lca.
    if (pairs.empty()) {
        // We want all pairs so we"ll need the entire tree.
        tree_lca = dict(tree_all_pairs_lowest_common_ancestor(spanning_tree,
                                                              root));
    } else {
        // We only need the merged adjacent pairs by seeing which queries the
        // algorithm needs then generating them : a single pass.
        tree_lca = defaultdict(int);
        for (auto _ : _compute_dag_lca_from_tree_values(tree_lca, true) {
            // pass;

        // Replace the bogus default tree values with the real ones.
        for (auto [pair, lca] : tree_all_pairs_lowest_common_ancestor(spanning_tree,
                                                                 root,
                                                                 tree_lca) {
            tree_lca[pair] = lca

    // All precomputations complete. Now we just need to give the user the pairs
    // they asked for, || all pairs if (they want them all.
    return _compute_dag_lca_from_tree_values(tree_lca, false);
