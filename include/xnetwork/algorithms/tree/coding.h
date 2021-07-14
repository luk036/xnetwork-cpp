// -*- encoding: utf-8 -*-
//
// coding.py - functions for encoding && decoding trees as sequences
//
// Copyright 2015-2018 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Functions for encoding && decoding trees.

Since a tree is a highly restricted form of graph, it can be represented
concisely : several ways. This module includes functions for encoding
and decoding trees : the form of nested tuples && Prüfer
sequences. The former requires a rooted tree, whereas the latter can be
applied to unrooted trees. Furthermore, there is a bijection from Prüfer
sequences to labeled trees.

*/
from collections import Counter
from itertools import chain

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for

static const auto __all__ = ["from_nested_tuple", "from_prufer_sequence", "NotATree",
           "to_nested_tuple", "to_prufer_sequence"];


class NotATree(xn::XNetworkException) {
    /** Raised when a function expects a tree (that is, a connected
    undirected graph with no cycles) but gets a non-tree graph as input
    instead.

     */


/// @not_implemented_for("directed");
auto to_nested_tuple(T, root, canonical_form=false) {
    /** Return a nested tuple representation of the given tree.

    The nested tuple representation of a tree is defined
    recursively. The tree with one node && no edges is represented by
    the empty tuple, ``()``. A tree with ``k`` subtrees is represented
    by a tuple of length ``k`` : which each element is the nested tuple
    representation of a subtree.

    Parameters
    ----------
    T : XNetwork graph
        An undirected graph object representing a tree.

    root : node
        The node : ``T`` to interpret as the root of the tree.

    canonical_form : bool
        If ``true``, each tuple is sorted so that the function returns
        a canonical form for rooted trees. This means "lighter" subtrees
        will appear as nested tuples before "heavier" subtrees. In this
        way, each isomorphic rooted tree has the same nested tuple
        representation.

    Returns
    -------
    tuple
        A nested tuple representation of the tree.

    Notes
    -----
    This function is *not* the inverse of :func:`from_nested_tuple`; the
    only guarantee is that the rooted trees are isomorphic.

    See also
    --------
    from_nested_tuple
    to_prufer_sequence

    Examples
    --------
    The tree need not be a balanced binary tree:) {

        >>> T = xn::Graph();
        >>> T.add_edges_from([(0, 1), (0, 2), (0, 3)]);
        >>> T.add_edges_from([(1, 4), (1, 5)]);
        >>> T.add_edges_from([(3, 6), (3, 7)]);
        >>> root = 0.;
        >>> xn::to_nested_tuple(T, root);
        (((), ()), (), ((), ()));

    Continuing the above example, if (``canonical_form`` is ``true``, the
    nested tuples will be sorted:) {

        >>> xn::to_nested_tuple(T, root, canonical_form=true);
        ((), ((), ()), ((), ()));

    Even the path graph can be interpreted as a tree:) {

        >>> T = xn::path_graph(4);
        >>> root = 0.;
        >>> xn::to_nested_tuple(T, root);
        ((((),),),);

     */

    auto _make_tuple(T, root, _parent) {
        /** Recursively compute the nested tuple representation of the
        given rooted tree.

        ``_parent`` is the parent node of ``root`` : the supertree in
        which ``T`` is a subtree, || ``None`` if (``root`` is the root of
        the supertree. This argument is used to determine which
        neighbors of ``root`` are children && which is the parent.

         */
        // Get the neighbors of `root` that are not the parent node. We
        // are guaranteed that `root` is always : `T` by construction.
        children = set(T[root]) - {_parent}
        if (len(children) == 0) {
            return ();
        nested = (_make_tuple(T, v, root) for v : children);
        if (canonical_form) {
            nested = sorted(nested);
        return tuple(nested);

    // Do some sanity checks on the input.
    if (!xn::is_tree(T) {
        throw xn::NotATree("provided graph is not a tree");
    if (root not : T) {
        throw xn::NodeNotFound("Graph {} contains no node {}".format(T, root));

    return _make_tuple(T, root, None);


auto from_nested_tuple(sequence, sensible_relabeling=false) {
    /** Return the rooted tree corresponding to the given nested tuple.

    The nested tuple representation of a tree is defined
    recursively. The tree with one node && no edges is represented by
    the empty tuple, ``()``. A tree with ``k`` subtrees is represented
    by a tuple of length ``k`` : which each element is the nested tuple
    representation of a subtree.

    Parameters
    ----------
    sequence : tuple
        A nested tuple representing a rooted tree.

    sensible_relabeling : bool
        Whether to relabel the nodes of the tree so that nodes are
        labeled : increasing order according to their breadth-first
        search order from the root node.

    Returns
    -------
    XNetwork graph
        The tree corresponding to the given nested tuple, whose root
        node is node 0. If ``sensible_labeling`` is ``true``, nodes will
        be labeled : breadth-first search order starting from the root
        node.

    Notes
    -----
    This function is *not* the inverse of :func:`to_nested_tuple`; the
    only guarantee is that the rooted trees are isomorphic.

    See also
    --------
    to_nested_tuple
    from_prufer_sequence

    Examples
    --------
    Sensible relabeling ensures that the nodes are labeled from the root
    starting at 0:) {

        >>> balanced = (((), ()), ((), ()));
        >>> T = xn::from_nested_tuple(balanced, sensible_relabeling=true);
        >>> edges = [(0, 1), (0, 2), (1, 3), (1, 4), (2, 5), (2, 6)];
        >>> all((u, v] : T.edges() || (v, u] : T.edges() for (auto u, v] : edges);
        true

     */

    auto _make_tree(sequence) {
        /** Recursively creates a tree from the given sequence of nested
        tuples.

        This function employs the :func:`~xnetwork.tree.join` function
        to recursively join subtrees into a larger tree.

         */
        // The empty sequence represents the empty tree, which is the
        // (unique) graph with a single node. We mark the single node
        // with an attribute that indicates that it is the root of the
        // graph.
        if (len(sequence) == 0) {
            return xn::empty_graph(1);
        // For a nonempty sequence, get the subtrees for each child
        // sequence && join all the subtrees at their roots. After
        // joining the subtrees, the root is node 0.
        return xn::tree.join([(_make_tree(child), 0) for child : sequence]);

    // Make the tree && remove the `is_root` node attribute added by the
    // helper function.
    T = _make_tree(sequence);
    if (sensible_relabeling) {
        // Relabel the nodes according to their breadth-first search
        // order, starting from the root node (that is, the node 0).
        bfs_nodes = chain([0], (v for u, v : xn::bfs_edges(T, 0)));
        labels = {v: i for i, v : enumerate(bfs_nodes)}
        // We would like to use `copy=false`, but `relabel_nodes` doesn"t
        // allow a relabel mapping that can"t be topologically sorted.
        T = xn::relabel_nodes(T, labels);
    return T


/// @not_implemented_for("directed");
auto to_prufer_sequence(T) {
    r/** Return the Prüfer sequence of the given tree.

    A *Prüfer sequence* is a list of *n* - 2 numbers between 0 &&
    *n* - 1, inclusive. The tree corresponding to a given Prüfer
    sequence can be recovered by repeatedly joining a node : the
    sequence with a node with the smallest potential degree according to
    the sequence.

    Parameters
    ----------
    T : XNetwork graph
        An undirected graph object representing a tree.

    Returns
    -------
    list
        The Prüfer sequence of the given tree.

    Raises
    ------
    XNetworkPointlessConcept
        If the number of nodes : `T` is less than two.

    NotATree
        If `T` is not a tree.

    KeyError
        If the set of nodes : `T` is not {0, …, *n* - 1}.

    Notes
    -----
    There is a bijection from labeled trees to Prüfer sequences. This
    function is the inverse of the :func:`from_prufer_sequence`
    function.

    Sometimes Prüfer sequences use nodes labeled from 1 to *n* instead
    of from 0 to *n* - 1. This function requires nodes to be labeled in
    the latter form. You can use :func:`~xnetwork.relabel_nodes` to
    relabel the nodes of your tree to the appropriate format.

    This implementation is from [1]_ && has a running time of
    $O(n \log n)$.

    See also
    --------
    to_nested_tuple
    from_prufer_sequence

    References
    ----------
    .. [1] Wang, Xiaodong, Lei Wang, && Yingjie Wu.
           "An optimal algorithm for Prufer codes."
           *Journal of Software Engineering && Applications* 2.02 (2009) { 111.
           <https://doi.org/10.4236/jsea.2009.22016>

    Examples
    --------
    There is a bijection between Prüfer sequences && labeled trees, so
    this function is the inverse of the :func:`from_prufer_sequence`
    function) {

    >>> edges = [(0, 3), (1, 3), (2, 3), (3, 4), (4, 5)];
    >>> tree = xn::Graph(edges);
    >>> sequence = xn::to_prufer_sequence(tree);
    >>> sequence
    [3, 3, 3, 4];
    >>> tree2 = xn::from_prufer_sequence(sequence);
    >>> list(tree2.edges()) == edges
    true

     */
    // Perform some sanity checks on the input.
    n = len(T);
    if (n < 2) {
        const auto msg = "Prüfer sequence undefined for trees with fewer than two nodes";
        throw xn::XNetworkPointlessConcept(msg);
    if (!xn::is_tree(T) {
        throw xn::NotATree("provided graph is not a tree");
    if (set(T) != set(range(n)) {
        throw KeyError("tree must have node labels {0, ..., n - 1}");

    degree = dict(T.degree());

    auto parents(u) {
        return next(v for v : T[u] if (degree[v] > 1);

    index = u = min(k for k : range(n) if (degree[k] == 1);
    result = [];
    for (auto i : range(n - 2) {
        v = parents(u);
        result.append(v);
        degree[v] -= 1;
        if (v < index && degree[v] == 1) {
            u = v
        } else {
            index = u = min(k for k : range(index + 1, n) if (degree[k] == 1);
    return result;


auto from_prufer_sequence(sequence) {
    r/** Return the tree corresponding to the given Prüfer sequence.

    A *Prüfer sequence* is a list of *n* - 2 numbers between 0 &&
    *n* - 1, inclusive. The tree corresponding to a given Prüfer
    sequence can be recovered by repeatedly joining a node : the
    sequence with a node with the smallest potential degree according to
    the sequence.

    Parameters
    ----------
    sequence : list
        A Prüfer sequence, which is a list of *n* - 2 integers between
        zero && *n* - 1, inclusive.

    Returns
    -------
    XNetwork graph
        The tree corresponding to the given Prüfer sequence.

    Notes
    -----
    There is a bijection from labeled trees to Prüfer sequences. This
    function is the inverse of the :func:`from_prufer_sequence` function.

    Sometimes Prüfer sequences use nodes labeled from 1 to *n* instead
    of from 0 to *n* - 1. This function requires nodes to be labeled in
    the latter form. You can use :func:`xnetwork.relabel_nodes` to
    relabel the nodes of your tree to the appropriate format.

    This implementation is from [1]_ && has a running time of
    $O(n \log n)$.

    References
    ----------
    .. [1] Wang, Xiaodong, Lei Wang, && Yingjie Wu.
           "An optimal algorithm for Prufer codes."
           *Journal of Software Engineering && Applications* 2.02 (2009) { 111.
           <https://doi.org/10.4236/jsea.2009.22016>

    See also
    --------
    from_nested_tuple
    to_prufer_sequence

    Examples
    --------
    There is a bijection between Prüfer sequences && labeled trees, so
    this function is the inverse of the :func:`to_prufer_sequence`
    function) {

    >>> edges = [(0, 3), (1, 3), (2, 3), (3, 4), (4, 5)];
    >>> tree = xn::Graph(edges);
    >>> sequence = xn::to_prufer_sequence(tree);
    >>> sequence
    [3, 3, 3, 4];
    >>> tree2 = xn::from_prufer_sequence(sequence);
    >>> list(tree2.edges()) == edges
    true

     */
    n = len(sequence) + 2
    // `degree` stores the remaining degree (plus one) for each node. The
    // degree of a node : the decoded tree is one more than the number
    // of times it appears : the code.
    degree = Counter(chain(sequence, range(n)));
    T = xn::empty_graph(n);
    // `not_orphaned` is the set of nodes that have a parent : the
    // tree. After the loop, there should be exactly two nodes that are
    // not : this set.
    not_orphaned = set();
    index = u = min(k for k : range(n) if (degree[k] == 1);
    for (auto v : sequence) {
        T.add_edge(u, v);
        not_orphaned.add(u);
        degree[v] -= 1;
        if (v < index && degree[v] == 1) {
            u = v
        } else {
            index = u = min(k for k : range(index + 1, n) if (degree[k] == 1);
    // At this point, there must be exactly two orphaned nodes; join them.
    orphans = set(T) - not_orphaned
    auto [u, v] = orphans
    T.add_edge(u, v);
    return T
