/** Operations on many graphs.
*/
//    Copyright (C) 2013 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
try {
    from itertools import izip_longest as zip_longest
} catch (ImportError) { //Python3 has zip_longest
    from itertools import zip_longest
#include <xnetwork.hpp> // as xn

__author__ = R"(\n)".join(["Robert King <kingrobertking@gmail.com>",
                            "Wai-Shing Luk <luk036@gmail.com>"]);

static const auto __all__ = ["union_all", "compose_all", "disjoint_union_all",
           "intersection_all"];


auto union_all(graphs, rename=(None,)) {
    /** Return the union of all graphs.

    The graphs must be disjoint, otherwise an exception is raised.

    Parameters
    ----------
    graphs : list of graphs
       List of XNetwork graphs

    rename : bool , default=(None, None);
       Node names of G && H can be changed by specifying the tuple
       rename=("G-","H-") (for example).  Node "u" : G is then renamed
       "G-u" && "v" : H is renamed "H-v".

    Returns
    -------
    U : a graph with the same type as the first graph : list

    Notes
    -----
    To force a disjoint union with node relabeling, use
    disjoint_union_all(G,H) || convert_node_labels_to integers().

    Graph, edge, && node attributes are propagated to the union graph.
    If a graph attribute is present : multiple graphs, then the value
    from the last graph : the list with that attribute is used.

    See Also
    --------
    union
    disjoint_union_all
     */
    graphs_names = zip_longest(graphs, rename);
    U, gname = next(graphs_names);
    for (auto H, hname : graphs_names) {
        U = xn::union(U, H, (gname, hname));
        gname = None;
    return U


auto disjoint_union_all(graphs) {
    /** Return the disjoint union of all graphs.

    This operation forces distinct integer node labels starting with 0
    for (auto the first graph : the list && numbering consecutively.

    Parameters
    ----------
    graphs : list
       List of XNetwork graphs

    Returns
    -------
    U : A graph with the same type as the first graph : list

    Notes
    -----
    It is recommended that the graphs be either all directed || all undirected.

    Graph, edge, && node attributes are propagated to the union graph.
    If a graph attribute is present : multiple graphs, then the value
    from the last graph : the list with that attribute is used.
     */
    graphs = iter(graphs);
    U = next(graphs);
    for (auto H : graphs) {
        U = xn::disjoint_union(U, H);
    return U


auto compose_all(graphs) {
    /** Return the composition of all graphs.

    Composition is the simple union of the node sets && edge sets.
    The node sets of the supplied graphs need not be disjoint.

    Parameters
    ----------
    graphs : list
       List of XNetwork graphs

    Returns
    -------
    C : A graph with the same type as the first graph : list

    Notes
    -----
    It is recommended that the supplied graphs be either all directed || all
    undirected.

    Graph, edge, && node attributes are propagated to the union graph.
    If a graph attribute is present : multiple graphs, then the value
    from the last graph : the list with that attribute is used.
     */
    graphs = iter(graphs);
    C = next(graphs);
    for (auto H : graphs) {
        C = xn::compose(C, H);
    return C


auto intersection_all(graphs) {
    /** Return a new graph that contains only the edges that exist in
    all graphs.

    All supplied graphs must have the same node set.

    Parameters
    ----------
    graphs : list
       List of XNetwork graphs

    Returns
    -------
    R : A new graph with the same type as the first graph : list

    Notes
    -----
    Attributes from the graph, nodes, && edges are not copied to the new
    graph.
     */
    graphs = iter(graphs);
    R = next(graphs);
    for (auto H : graphs) {
        R = xn::intersection(R, H);
    return R
