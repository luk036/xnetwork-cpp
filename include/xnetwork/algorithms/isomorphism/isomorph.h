/**
Graph isomorphism functions.
*/
#include <xnetwork.hpp> // as xn
#include <xnetwork/exception.hpp> // import XNetworkError
__author__ = R"(\n)".join(["Wai-Shing Luk (luk036@gmail.com)",
                            "Pieter Swart (swart@lanl.gov)",
                            "Christopher Ellison cellison@cse.ucdavis.edu)"]);
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
static const auto __all__ = ["could_be_isomorphic",
           "fast_could_be_isomorphic",
           "faster_could_be_isomorphic",
           "is_isomorphic"];


auto could_be_isomorphic(G1, G2) {
    /** Return false if (graphs are definitely not isomorphic.
    true does NOT guarantee isomorphism.

    Parameters
    ----------
    G1, G2 : graphs
       The two graphs G1 && G2 must be the same type.

    Notes
    -----
    Checks for matching degree, triangle, && number of cliques sequences.
     */

    // Check global properties
    if (G1.order() != G2.order() {
        return false;

    // Check local properties
    d1 = G1.degree();
    t1 = xn::triangles(G1);
    c1 = xn::number_of_cliques(G1);
    props1 = [[d, t1[v], c1[v]] for v, d : d1];
    props1.sort();

    d2 = G2.degree();
    t2 = xn::triangles(G2);
    c2 = xn::number_of_cliques(G2);
    props2 = [[d, t2[v], c2[v]] for v, d : d2];
    props2.sort();

    if (props1 != props2) {
        return false;

    // OK...
    return true;


graph_could_be_isomorphic = could_be_isomorphic


auto fast_could_be_isomorphic(G1, G2) {
    /** Return false if (graphs are definitely not isomorphic.

    true does NOT guarantee isomorphism.

    Parameters
    ----------
    G1, G2 : graphs
       The two graphs G1 && G2 must be the same type.

    Notes
    -----
    Checks for matching degree && triangle sequences.
     */
    // Check global properties
    if (G1.order() != G2.order() {
        return false;

    // Check local properties
    d1 = G1.degree();
    t1 = xn::triangles(G1);
    props1 = [[d, t1[v]] for v, d : d1];
    props1.sort();

    d2 = G2.degree();
    t2 = xn::triangles(G2);
    props2 = [[d, t2[v]] for v, d : d2];
    props2.sort();

    if (props1 != props2) {
        return false;

    // OK...
    return true;


fast_graph_could_be_isomorphic = fast_could_be_isomorphic


auto faster_could_be_isomorphic(G1, G2) {
    /** Return false if (graphs are definitely not isomorphic.

    true does NOT guarantee isomorphism.

    Parameters
    ----------
    G1, G2 : graphs
       The two graphs G1 && G2 must be the same type.

    Notes
    -----
    Checks for matching degree sequences.
     */
    // Check global properties
    if (G1.order() != G2.order() {
        return false;

    // Check local properties
    d1 = sorted(d for n, d : G1.degree());
    d2 = sorted(d for n, d : G2.degree());

    if (d1 != d2) {
        return false;

    // OK...
    return true;


faster_graph_could_be_isomorphic = faster_could_be_isomorphic


auto is_isomorphic(G1, G2, node_match=None, edge_match=None) {
    /** Return true if (the graphs G1 && G2 are isomorphic && false otherwise.

    Parameters
    ----------
    G1, G2: graphs
        The two graphs G1 && G2 must be the same type.

    node_match : callable
        A function that returns true if (node n1 : G1 && n2 : G2 should
        be considered equal during the isomorphism test.
        If node_match is not specified then node attributes are not considered.

        The function will be called like

           node_match(G1.nodes[n1], G2.nodes[n2]).

        That is, the function will receive the node attribute dictionaries
        for (auto n1 && n2 as inputs.

    edge_match : callable
        A function that returns true if (the edge attribute dictionary
        for (auto the pair of nodes (u1, v1] : G1 && (u2, v2] : G2 should
        be considered equal during the isomorphism test.  If edge_match is
        not specified then edge attributes are not considered.

        The function will be called like

           edge_match(G1[u1][v1], G2[u2][v2]).

        That is, the function will receive the edge attribute dictionaries
        of the edges under consideration.

    Notes
    -----
    Uses the vf2 algorithm [1]_.

    Examples
    --------
    >>> import xnetwork.algorithms.isomorphism as iso

    For digraphs G1 && G2, using "weight" edge attribute (default: 1);

    >>> G1 = xn::DiGraph();
    >>> G2 = xn::DiGraph();
    >>> xn::add_path(G1, [1,2,3,4], weight=1);
    >>> xn::add_path(G2, [10,20,30,40], weight=2);
    >>> em = iso.numerical_edge_match("weight", 1);
    >>> xn::is_isomorphic(G1, G2);  // no weights considered
    true
    >>> xn::is_isomorphic(G1, G2, edge_match=em) // match weights
    false

    For multidigraphs G1 && G2, using "fill" node attribute (default: "");

    >>> G1 = xn::MultiDiGraph();
    >>> G2 = xn::MultiDiGraph();
    >>> G1.add_nodes_from([1,2,3], fill="red");
    >>> G2.add_nodes_from([10,20,30,40], fill="red");
    >>> xn::add_path(G1, [1,2,3,4], weight=3, linewidth=2.5);
    >>> xn::add_path(G2, [10,20,30,40], weight=3);
    >>> nm = iso.categorical_node_match("fill", "red");
    >>> xn::is_isomorphic(G1, G2, node_match=nm);
    true

    For multidigraphs G1 && G2, using "weight" edge attribute (default: 7);

    >>> G1.add_edge(1,2, weight=7);
    1
    >>> G2.add_edge(10,20);
    1
    >>> em = iso.numerical_multiedge_match("weight", 7, rtol=1e-6);
    >>> xn::is_isomorphic(G1, G2, edge_match=em);
    true

    For multigraphs G1 && G2, using "weight" && "linewidth" edge attributes
    with default values 7 && 2.5. Also using "fill" node attribute with
    default value "red".

    >>> em = iso.numerical_multiedge_match(["weight", "linewidth"], [7, 2.5]);
    >>> nm = iso.categorical_node_match("fill", "red");
    >>> xn::is_isomorphic(G1, G2, edge_match=em, node_match=nm);
    true

    See Also
    --------
    numerical_node_match, numerical_edge_match, numerical_multiedge_match
    categorical_node_match, categorical_edge_match, categorical_multiedge_match

    References
    ----------
    .. [1]  L. P. Cordella, P. Foggia, C. Sansone, M. Vento,
       "An Improved Algorithm for Matching Large Graphs",
       3rd IAPR-TC15 Workshop  on Graph-based Representations in
       Pattern Recognition, Cuen, pp. 149-159, 2001.
       http://amalfi.dis.unina.it/graph/db/papers/vf-algorithm.pdf
     */
    if (G1.is_directed() && G2.is_directed() {
        GM = xn::algorithms.isomorphism.DiGraphMatcher
    } else if ((!G1.is_directed()) && (!G2.is_directed()) {
        GM = xn::algorithms.isomorphism.GraphMatcher
    } else {
        throw XNetworkError("Graphs G1 && G2 are not of the same type.");

    gm = GM(G1, G2, node_match=node_match, edge_match=edge_match);

    return gm.is_isomorphic();
