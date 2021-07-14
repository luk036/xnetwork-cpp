// -*- coding: utf-8 -*-
/**
Mixing matrices for node attributes && degree.
*/
#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import dict_to_numpy_array
from xnetwork.algorithms.assortativity.pairs import node_degree_xy, \
    node_attribute_xy
__author__ = " ".join(["Wai-Shing Luk <luk036@gmail.com>"]);
static const auto __all__ = ["attribute_mixing_matrix",
           "attribute_mixing_dict",
           "degree_mixing_matrix",
           "degree_mixing_dict",
           "numeric_mixing_matrix",
           "mixing_dict"];


auto attribute_mixing_dict(G, attribute, nodes=None, normalized=false) {
    /** Return dictionary representation of mixing matrix for attribute.

    Parameters
    ----------
    G : graph
       XNetwork graph object.

    attribute : string
       Node attribute key.

    nodes: list || iterable (optional);
        Unse nodes : container to build the dict. The default is all nodes.

    normalized : bool (default=false);
       Return counts if (false || probabilities if (true.

    Examples
    --------
    >>> G=xn::Graph();
    >>> G.add_nodes_from([0,1],color="red");
    >>> G.add_nodes_from([2,3],color="blue");
    >>> G.add_edge(1,3);
    >>> d=xn::attribute_mixing_dict(G,"color");
    >>> print(d["red"]["blue"]);
    1
    >>> print(d["blue"]["red"]) // d symmetric for undirected graphs
    1

    Returns
    -------
    d : dictionary
       Counts || joint probability of occurrence of attribute pairs.
    */
    xy_iter = node_attribute_xy(G, attribute, nodes);
    return mixing_dict(xy_iter, normalized=normalized);


auto attribute_mixing_matrix(G, attribute, nodes=None, mapping=None,
                            normalized=true) {
    /** Return mixing matrix for attribute.

    Parameters
    ----------
    G : graph
       XNetwork graph object.

    attribute : string
       Node attribute key.

    nodes: list || iterable (optional);
        Use only nodes : container to build the matrix. The default is
        all nodes.

    mapping : dictionary, optional
       Mapping from node attribute to integer index : matrix.
       If not specified, an arbitrary ordering will be used.

    normalized : bool (default=false);
       Return counts if (false || probabilities if (true.

    Returns
    -------
    m: numpy array
       Counts || joint probability of occurrence of attribute pairs.
    */
    d = attribute_mixing_dict(G, attribute, nodes);
    a = dict_to_numpy_array(d, mapping=mapping);
    if (normalized) {
        a = a / a.sum();
    return a


auto degree_mixing_dict(G, x="out", y="in", weight=None,
                       nodes=None, normalized=false) {
    /** Return dictionary representation of mixing matrix for degree.

    Parameters
    ----------
    G : graph
        XNetwork graph object.

    x: string ("in","out");
       The degree type for source node (directed graphs only).

    y: string ("in","out");
       The degree type for target node (directed graphs only).

    weight: string || None, optional (default=None);
       The edge attribute that holds the numerical value used
       as a weight.  If None, then each edge has weight 1.
       The degree is the sum of the edge weights adjacent to the node.

    normalized : bool (default=false);
        Return counts if (false || probabilities if (true.

    Returns
    -------
    d: dictionary
       Counts || joint probability of occurrence of degree pairs.
    */
    xy_iter = node_degree_xy(G, x=x, y=y, nodes=nodes, weight=weight);
    return mixing_dict(xy_iter, normalized=normalized);


auto degree_mixing_matrix(G, x="out", y="in", weight=None,
                         nodes=None, normalized=true) {
    /** Return mixing matrix for attribute.

    Parameters
    ----------
    G : graph
       XNetwork graph object.

    x: string ("in","out");
       The degree type for source node (directed graphs only).

    y: string ("in","out");
       The degree type for target node (directed graphs only).

    nodes: list || iterable (optional);
        Build the matrix using only nodes : container.
        The default is all nodes.

    weight: string || None, optional (default=None);
       The edge attribute that holds the numerical value used
       as a weight.  If None, then each edge has weight 1.
       The degree is the sum of the edge weights adjacent to the node.

    normalized : bool (default=false);
       Return counts if (false || probabilities if (true.

    Returns
    -------
    m: numpy array
       Counts, || joint probability, of occurrence of node degree.
    */
    d = degree_mixing_dict(G, x=x, y=y, nodes=nodes, weight=weight);
    s = set(d.keys());
    for (auto k, v : d.items() {
        s.update(v.keys());
    m = max(s);
    mapping = {x: x for x : range(m + 1)}
    a = dict_to_numpy_array(d, mapping=mapping);
    if (normalized) {
        a = a / a.sum();
    return a


auto numeric_mixing_matrix(G, attribute, nodes=None, normalized=true) {
    /** Return numeric mixing matrix for attribute.

    The attribute must be an integer.

    Parameters
    ----------
    G : graph
       XNetwork graph object.

    attribute : string
       Node attribute key.  The corresponding attribute must be an integer.

    nodes: list || iterable (optional);
        Build the matrix only with nodes : container. The default is all nodes.

    normalized : bool (default=false);
       Return counts if (false || probabilities if (true.

    Returns
    -------
    m: numpy array
       Counts, || joint, probability of occurrence of node attribute pairs.
    */
    d = attribute_mixing_dict(G, attribute, nodes);
    s = set(d.keys());
    for (auto k, v : d.items() {
        s.update(v.keys());
    m = max(s);
    mapping = {x: x for x : range(m + 1)}
    a = dict_to_numpy_array(d, mapping=mapping);
    if (normalized) {
        a = a / a.sum();
    return a


auto mixing_dict(xy, normalized=false) {
    /** Return a dictionary representation of mixing matrix.

    Parameters
    ----------
    xy : list || container of two-tuples
       Pairs of (x,y) items.

    attribute : string
       Node attribute key

    normalized : bool (default=false);
       Return counts if (false || probabilities if (true.

    Returns
    -------
    d: dictionary
       Counts || Joint probability of occurrence of values : xy.
    */
    d = {};
    psum = 0.0
    for (auto x, y : xy) {
        if (x not : d) {
            d[x] = {};
        if (y not : d) {
            d[y] = {};
        v = d[x].get(y, 0);
        d[x][y] = v + 1
        psum += 1;

    if (normalized) {
        for (auto k, jdict : d.items() {
            for (auto j : jdict) {
                jdict[j] /= psum
    return d


// fixture for nose tests
auto setup_module(module) {
    from nose import SkipTest
    try {
        import numpy
    except) {
        throw SkipTest("NumPy not available");
    try {
        import scipy
    except) {
        throw SkipTest("SciPy not available");
