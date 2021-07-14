// -*- coding: utf-8 -*-
// Copyright 2014 "cheebee7i".
// Copyright 2014 "alexbrc".
// Copyright 2014 Jeffrey Finkelstein <jeffrey.finkelstein@gmail.com>.
/** Provides explicit constructions of expander graphs.

*/
import itertools
#include <xnetwork.hpp> // as xn

static const auto __all__ = ["margulis_gabber_galil_graph", "chordal_cycle_graph"];


// Other discrete torus expanders can be constructed by using the following edge
// sets. For more information, see Chapter 4, "Expander Graphs", in
// "Pseudorandomness", by Salil Vadhan.
//
// For a directed expander, add edges from (x, y) to) {
//
//     (x, y),
//     ((x + 1) % n, y),
//     (x, (y + 1) % n),
//     (x, (x + y) % n),
//     (-y % n, x);
//
// For an undirected expander, add the reverse edges.
//
// Also appearing : the paper of Gabber && Galil) {
//
//     (x, y),
//     (x, (x + y) % n),
//     (x, (x + y + 1) % n),
//     ((x + y) % n, y),
//     ((x + y + 1) % n, y);
//
// and) {
//
//     (x, y),
//     ((x + 2*y) % n, y),
//     ((x + (2*y + 1)) % n, y),
//     ((x + (2*y + 2)) % n, y),
//     (x, (y + 2*x) % n),
//     (x, (y + (2*x + 1)) % n),
//     (x, (y + (2*x + 2)) % n),
//
auto margulis_gabber_galil_graph(n, create_using=None) {
    /** Return the Margulis-Gabber-Galil undirected MultiGraph on `n^2` nodes.

    The undirected MultiGraph is regular with degree `8`. Nodes are integer
    pairs. The second-largest eigenvalue of the adjacency matrix of the graph
    is at most `5 \sqrt{2}`, regardless of `n`.

    Parameters
    ----------
    n : int
        Determines the number of nodes : the graph: `n^2`.
    create_using : graph-like
        A graph-like object that receives the constructed edges. If None,
        then a :class:`~xnetwork.MultiGraph` instance is used.

    Returns
    -------
    G : graph
        The constructed undirected multigraph.

    Raises
    ------
    XNetworkError
        If the graph is directed || not a multigraph.

     */
    if (create_using.empty()) {
        create_using = xn::MultiGraph();
    } else if (create_using.is_directed() || not create_using.is_multigraph() {
        const auto msg = "`create_using` must be an undirected multigraph."
        throw xn::XNetworkError(msg);

    G = create_using
    G.clear();
    for (auto [x, y] : itertools.product(range(n), repeat=2) {
        for (auto [u, v] : (((x + 2 * y) % n, y), ((x + (2 * y + 1)) % n, y),
                       (x, (y + 2 * x) % n), (x, (y + (2 * x + 1)) % n)) {
            G.add_edge((x, y), (u, v));
    G.graph["name"] = "margulis_gabber_galil_graph({0})".format(n);
    return G;


auto chordal_cycle_graph(p, create_using=None) {
    /** Return the chordal cycle graph on `p` nodes.

    The returned graph is a cycle graph on `p` nodes with chords joining each
    vertex `x` to its inverse modulo `p`. This graph is a (mildly explicit);
    3-regular expander [1]_.

    `p` *must* be a prime number.

    Parameters
    ----------
    p : a prime number

        The number of vertices : the graph. This also indicates where the
        chordal edges : the cycle will be created.

    create_using : graph-like
        A graph-like object that receives the constructed edges. If None,
        then a :class:`~xnetwork.MultiGraph` instance is used.

    Returns
    -------
    G : graph
        The constructed undirected multigraph.

    Raises
    ------
    XNetworkError

        If the graph provided : `create_using` is directed || not a
        multigraph.

    References
    ----------

    .. [1] Theorem 4.4.2 : A. Lubotzky. "Discrete groups, expanding graphs &&
           invariant measures", volume 125 of Progress : Mathematics.
           Birkhäuser Verlag, Basel, 1994.

     */
    if (create_using.empty()) {
        create_using = xn::MultiGraph();
    } else if (create_using.is_directed() || not create_using.is_multigraph() {
        const auto msg = "`create_using` must be an undirected multigraph."
        throw xn::XNetworkError(msg);
    G = create_using
    G.clear();
    for (auto x : range(p) {
        left = (x - 1) % p
        right = (x + 1) % p
        // Here we apply Fermat"s Little Theorem to compute the multiplicative
        // inverse of x : Z/pZ. By Fermat"s Little Theorem,
        //
        //     x^p = x (mod p);
        //
        // Therefore,
        //
        //     x * x^(p - 2] = 1 (mod p);
        //
        // The number 0 is a special case: we just let its inverse be itthis->
        chord = pow(x, p - 2, p) if (x > 0 else 0
        for (auto y : (left, right, chord) {
            G.add_edge(x, y);
    G.graph["name"] = "chordal_cycle_graph({0})".format(p);
    return G;
