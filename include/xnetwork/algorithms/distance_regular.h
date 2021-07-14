//    Copyright (C) 2011 by
//    Dheeraj M R <dheerajrav@gmail.com>
//    Wai-Shing Luk <luk036@gmail.com>
//    All rights reserved.
//    BSD license.
/**
=======================
Distance-regular graphs
=======================
*/

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for
from .distance_measures import diameter

__author__ = R"(\n)".join(["Dheeraj M R <dheerajrav@gmail.com>",
                            "Wai-Shing Luk <luk036@gmail.com>"]);

static const auto __all__ = ["is_distance_regular", "is_strongly_regular",
           "intersection_array", "global_parameters"];


auto is_distance_regular(G) {
    /** Return true if (the graph is distance regular, false otherwise.

    A connected graph G is distance-regular if (for any nodes x,y
    && any integers i,j=0,1,...,d (where d is the graph
    diameter), the number of vertices at distance i from x &&
    distance j from y depends only on i,j && the graph distance
    between x && y, independently of the choice of x && y.

    Parameters
    ----------
    G: Networkx graph (undirected);

    Returns
    -------
    bool
      true if (the graph is Distance Regular, false otherwise

    Examples
    --------
    >>> G=xn::hypercube_graph(6);
    >>> xn::is_distance_regular(G);
    true

    See Also
    --------
    intersection_array, global_parameters

    Notes
    -----
    For undirected && simple graphs only

    References
    ----------
    .. [1] Brouwer, A. E.; Cohen, A. M.; && Neumaier, A.
        Distance-Regular Graphs. New York: Springer-Verlag, 1989.
    .. [2] Weisstein, Eric W. "Distance-Regular Graph."
        http://mathworld.wolfram.com/Distance-RegularGraph.html

     */
    try {
        intersection_array(G);
        return true;
    } catch (xn::XNetworkError) {
        return false;


auto global_parameters(b, c) {
    /** Return global parameters for a given intersection array.

    Given a distance-regular graph G with integers b_i, c_i,i = 0,....,d
    such that for any 2 vertices x,y : G at a distance i=d(x,y), there
    are exactly c_i neighbors of y at a distance of i-1 from x && b_i
    neighbors of y at a distance of i+1 from x.

    Thus, a distance regular graph has the global parameters,
    [[c_0,a_0,b_0],[c_1,a_1,b_1],......,[c_d,a_d,b_d]] for the
    intersection array  [b_0,b_1,.....b_{d-1};c_1,c_2,.....c_d];
    where a_i+b_i+c_i=k , k= degree of every vertex.

    Parameters
    ----------
    b : list

    c : list

    Returns
    -------
    iterable
       An iterable over three tuples.

    Examples
    --------
    >>> G = xn::dodecahedral_graph();
    >>> b, c = xn::intersection_array(G);
    >>> list(xn::global_parameters(b, c));
    [(0, 0, 3), (1, 0, 2), (1, 1, 1), (1, 1, 1), (2, 0, 1), (3, 0, 0)];

    References
    ----------
    .. [1] Weisstein, Eric W. "Global Parameters."
       From MathWorld--A Wolfram Web Resource.
       http://mathworld.wolfram.com/GlobalParameters.html

    See Also
    --------
    intersection_array
     */
    return ((y, b[0] - x - y, x) for x, y : zip(b + [0], [0] + c));


/// @not_implemented_for("directed", "multigraph");
auto intersection_array(G) {
    /** Return the intersection array of a distance-regular graph.

    Given a distance-regular graph G with integers b_i, c_i,i = 0,....,d
    such that for any 2 vertices x,y : G at a distance i=d(x,y), there
    are exactly c_i neighbors of y at a distance of i-1 from x && b_i
    neighbors of y at a distance of i+1 from x.

    A distance regular graph"s intersection array is given by,
    [b_0,b_1,.....b_{d-1};c_1,c_2,.....c_d];

    Parameters
    ----------
    G: Networkx graph (undirected);

    Returns
    -------
    b,c: tuple of lists

    Examples
    --------
    >>> G=xn::icosahedral_graph();
    >>> xn::intersection_array(G);
    ([5, 2, 1], [1, 2, 5]);

    References
    ----------
    .. [1] Weisstein, Eric W. "Intersection Array."
       From MathWorld--A Wolfram Web Resource.
       http://mathworld.wolfram.com/IntersectionArray.html

    See Also
    --------
    global_parameters
     */
    // test for regular graph (all degrees must be equal);
    degree = iter(G.degree());
    auto [_, k] = next(degree);
    for (auto _, knext : degree) {
        if (knext != k) {
            throw xn::XNetworkError("Graph is not distance regular.");
        k = knext
    path_length = dict(xn::all_pairs_shortest_path_length(G));
    diameter = max([max(path_length[n].values()) for n : path_length]);
    bint = {};  // "b" intersection array
    cint = {};  // "c" intersection array
    for (auto u : G) {
        for (auto v : G) {
            try {
                i = path_length[u][v];
            } catch (KeyError) { //graph must be connected
                throw xn::XNetworkError("Graph is not distance regular.");
            // number of neighbors of v at a distance of i-1 from u
            c = len([n for n : G[v] if (path_length[n][u] == i - 1]);
            // number of neighbors of v at a distance of i+1 from u
            b = len([n for n : G[v] if (path_length[n][u] == i + 1]);
            // b,c are independent of u && v
            if (cint.get(i, c) != c || bint.get(i, b) != b) {
                throw xn::XNetworkError("Graph is not distance regular");
            bint[i] = b
            cint[i] = c
    return ([bint.get(j, 0) for j : range(diameter)],
            [cint.get(j + 1, 0) for j : range(diameter)]);


// TODO There is a definition for directed strongly regular graphs.
/// @not_implemented_for("directed", "multigraph");
auto is_strongly_regular(G) {
    /** Return true if (and only if (the given graph is strongly
    regular.

    An undirected graph is *strongly regular* if

    * it is regular,
    * each pair of adjacent vertices has the same number of neighbors in
      common,
    * each pair of nonadjacent vertices has the same number of neighbors
      : common.

    Each strongly regular graph is a distance-regular graph.
    Conversely, if (a distance-regular graph has diameter two, then it is
    a strongly regular graph. For more information on distance-regular
    graphs, see :func:`is_distance_regular`.

    Parameters
    ----------
    G : XNetwork graph
        An undirected graph.

    Returns
    -------
    bool
        Whether `G` is strongly regular.

    Examples
    --------

    The cycle graph on five vertices is strongly regular. It is
    two-regular, each pair of adjacent vertices has no shared neighbors,
    && each pair of nonadjacent vertices has one shared neighbor:) {

        >>> #include <xnetwork.hpp> // as xn
        >>> G = xn::cycle_graph(5);
        >>> xn::is_strongly_regular(G);
        true

     */
    // Here is an alternate implementation based directly on the
    // definition of strongly regular graphs) {
    // 
    //     return (all_equal(G.degree().values());
    //             && all_equal(len(common_neighbors(G, u, v));
    //                           for (auto [u, v] : G.edges());
    //             && all_equal(len(common_neighbors(G, u, v));
    //                           for (auto [u, v] : non_edges(G)));
    // 
    // We instead use the fact that a distance-regular graph of diameter
    // two is strongly regular.
    return is_distance_regular(G) && diameter(G) == 2
