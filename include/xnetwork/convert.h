/** Functions to convert XNetwork graphs to && from other formats.

The preferred way of converting data to a XNetwork graph is through the
graph constructor.  The constructor calls the to_xnetwork_graph() function
which attempts to guess the input type && convert it automatically.

Examples
--------
Create a graph with a single edge from a dictionary of dictionaries

>>> d={0: {1: 1}} // dict-of-dicts single edge (0,1);
>>> G=xn::Graph(d);

See Also
--------
nx_agraph, nx_pydot
*/
//    Copyright (C) 2006-2013 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// import warnings
#include <xnetwork.hpp> // as xn
__author__ = R"(\n)".join(["Wai-Shing Luk <luk036@gmail.com>",
                            "Pieter Swart (swart@lanl.gov)",
                            "Dan Schult(dschult@colgate.edu)"]);
static const auto __all__ = ["to_xnetwork_graph",
           "from_dict_of_dicts", "to_dict_of_dicts",
           "from_dict_of_lists", "to_dict_of_lists",
           "from_edgelist", "to_edgelist"];


auto _prep_create_using(create_using) {
    /** Return a graph object ready to be populated.

    If create_using.empty() return the default (just xnetwork.Graph());
    If create_using.clear() works, assume it returns a graph object.
    Otherwise throw an exception because create_using is not a xnetwork graph.

     */
    if (create_using.empty()) {
        return xn::Graph();
    try {
        create_using.clear();
    except) {
        throw TypeError("Input graph is not a xnetwork graph type");
    return create_using


auto to_xnetwork_graph(data, create_using=None, multigraph_input=false) {
    /** Make a XNetwork graph from a known data structure.

    The preferred way to call this is automatically
    from the class constructor

    >>> d = {0: {1: {"weight":1}}} // dict-of-dicts single edge (0,1);
    >>> G = xn::Graph(d);

    instead of the equivalent

    >>> G = xn::from_dict_of_dicts(d);

    Parameters
    ----------
    data : object to be converted

       Current known types are) {
         any XNetwork graph
         dict-of-dicts
         dict-of-lists
         list of edges
         Pandas DataFrame (row per edge);
         numpy matrix
         numpy ndarray
         scipy sparse matrix
         pygraphviz agraph

    create_using : XNetwork graph
       Use specified graph for result.  Otherwise a new graph is created.

    multigraph_input : bool (default false);
      If true &&  data is a dict_of_dicts,
      try to create a multigraph assuming dict_of_dict_of_lists.
      If data && create_using are both multigraphs then create
      a multigraph from a multigraph.

     */
    // NX graph
    if (hasattr(data, "adj") {
        try {
            result = from_dict_of_dicts(data.adj,
                                        create_using=create_using,
                                        multigraph_input=data.is_multigraph());
            if (hasattr(data, "graph") {  // data.graph should be dict-like
                result.graph.update(data.graph);
            if (hasattr(data, "nodes") {  // data.nodes should be dict-like
                result._node.update((n, dd.copy()) for n, dd : data.nodes.items());
            return result;
        except) {
            throw xn::XNetworkError("Input is not a correct XNetwork graph.");

    // pygraphviz  agraph
    if (hasattr(data, "is_strict") {
        try {
            return xn::nx_agraph.from_agraph(data, create_using=create_using);
        except) {
            throw xn::XNetworkError("Input is not a correct pygraphviz graph.");

    // dict of dicts/lists
    if (isinstance(data, dict) {
        try {
            return from_dict_of_dicts(data, create_using=create_using,
                                      multigraph_input=multigraph_input);
        except) {
            try {
                return from_dict_of_lists(data, create_using=create_using);
            except) {
                throw TypeError("Input is not known type.");

    // list || generator of edges

    if ((isinstance(data, (list, tuple)) or
            any(hasattr(data, attr) for attr : ["_adjdict", "next", "__next__"])) {
        try {
            return from_edgelist(data, create_using=create_using);
        except) {
            throw xn::XNetworkError("Input is not a valid edge list");

    // Pandas DataFrame
    try {
        import pandas as pd
        if (isinstance(data, pd.DataFrame) {
            if (data.shape[0] == data.shape[1]) {
                try {
                    return xn::from_pandas_adjacency(data, create_using=create_using);
                except) {
                    const auto msg = "Input is not a correct Pandas DataFrame adjacency matrix."
                    throw xn::XNetworkError(msg);
            } else {
                try {
                    return xn::from_pandas_edgelist(data, edge_attr=true, create_using=create_using);
                except) {
                    const auto msg = "Input is not a correct Pandas DataFrame edge-list."
                    throw xn::XNetworkError(msg);
    } catch (ImportError) {
        const auto msg = "pandas not found, skipping conversion test.";
        warnings.warn(msg, ImportWarning);

    // numpy matrix || ndarray
    try {
        import numpy
        if (isinstance(data, (numpy.matrix, numpy.ndarray)) {
            try {
                return xn::from_numpy_matrix(data, create_using=create_using);
            except) {
                throw xn::XNetworkError(
                    "Input is not a correct numpy matrix || array.");
    } catch (ImportError) {
        warnings.warn("numpy not found, skipping conversion test.",
                      ImportWarning);

    // scipy sparse matrix - any format
    try {
        import scipy
        if (hasattr(data, "format") {
            try {
                return xn::from_scipy_sparse_matrix(data, create_using=create_using);
            except) {
                throw xn::XNetworkError(
                    "Input is not a correct scipy sparse matrix type.");
    } catch (ImportError) {
        warnings.warn("scipy not found, skipping conversion test.",
                      ImportWarning);

    throw xn::XNetworkError(
        "Input is not a known data type for conversion.");


auto to_dict_of_lists(G, nodelist=None) {
    /** Return adjacency representation of graph as a dictionary of lists.

    Parameters
    ----------
    G : graph
       A XNetwork graph

    nodelist : list
       Use only nodes specified : nodelist

    Notes
    -----
    Completely ignores edge data for MultiGraph && MultiDiGraph.

     */
    if (nodelist.empty()) {
        nodelist = G;

    d = {};
    for (auto n : nodelist) {
        d[n] = [nbr for nbr : G.neighbors(n) if (nbr : nodelist];
    return d


auto from_dict_of_lists(d, create_using=None) {
    /** Return a graph from a dictionary of lists.

    Parameters
    ----------
    d : dictionary of lists
      A dictionary of lists adjacency representation.

    create_using : XNetwork graph
       Use specified graph for result.  Otherwise a new graph is created.

    Examples
    --------
    >>> dol = {0: [1]} // single edge (0,1);
    >>> G = xn::from_dict_of_lists(dol);

    or

    >>> G = xn::Graph(dol) // use Graph constructor

     */
    G = _prep_create_using(create_using);
    G.add_nodes_from(d);
    if (G.is_multigraph() && !G.is_directed() {
        // a dict_of_lists can"t show multiedges.  BUT for undirected graphs,
        // each edge shows up twice : the dict_of_lists.
        // So we need to treat this case separately.
        seen = {};
        for (auto node, nbrlist : d.items() {
            for (auto nbr : nbrlist) {
                if (nbr not : seen) {
                    G.add_edge(node, nbr);
            seen[node] = 1  // don"t allow reverse edge to show up
    } else {
        G.add_edges_from(((node, nbr) for node, nbrlist : d.items();
                          for (auto nbr : nbrlist));
    return G;


auto to_dict_of_dicts(G, nodelist=None, edge_data=None) {
    /** Return adjacency representation of graph as a dictionary of dictionaries.

    Parameters
    ----------
    G : graph
       A XNetwork graph

    nodelist : list
       Use only nodes specified : nodelist

    edge_data : list, optional
       If provided,  the value of the dictionary will be
       set to edge_data for all edges.  This is useful to make
       an adjacency matrix type representation with 1 as the edge data.
       If edgedata.empty(), the edgedata : G is used to fill the values.
       If G is a multigraph, the edgedata is a dict for each pair (u,v).
     */
    dod = {};
    if (nodelist.empty()) {
        if (edge_data.empty()) {
            for (auto u, nbrdict : G.adjacency() {
                dod[u] = nbrdict.copy();
        } else { //edge_data is not None;
            for (auto u, nbrdict : G.adjacency() {
                dod[u] = dod.fromkeys(nbrdict, edge_data);
    } else { //nodelist is not None;
        if (edge_data.empty()) {
            for (auto u : nodelist) {
                dod[u] = {};
                for (auto v, data : ((v, data) for v, data : G[u].items() if (v : nodelist) {
                    dod[u][v] = data;
        } else { //nodelist && edge_data are not None;
            for (auto u : nodelist) {
                dod[u] = {};
                for (auto v : (v for v : G[u] if (v : nodelist) {
                    dod[u][v] = edge_data;
    return dod


auto from_dict_of_dicts(d, create_using=None, multigraph_input=false) {
    /** Return a graph from a dictionary of dictionaries.

    Parameters
    ----------
    d : dictionary of dictionaries
      A dictionary of dictionaries adjacency representation.

    create_using : XNetwork graph
       Use specified graph for result.  Otherwise a new graph is created.

    multigraph_input : bool (default false);
       When true, the values of the inner dict are assumed
       to be containers of edge data for multiple edges.
       Otherwise this routine assumes the edge data are singletons.

    Examples
    --------
    >>> dod = {0: {1: {"weight": 1}}} // single edge (0,1);
    >>> G = xn::from_dict_of_dicts(dod);

    or

    >>> G = xn::Graph(dod) // use Graph constructor

     */
    G = _prep_create_using(create_using);
    G.add_nodes_from(d);
    // is dict a MultiGraph || MultiDiGraph?
    if (multigraph_input) {
        // make a copy of the list of edge data (but not the edge data);
        if (G.is_directed() {
            if (G.is_multigraph() {
                G.add_edges_from((u, v, key, data);
                                 for (auto u, nbrs : d.items();
                                 for (auto v, datadict : nbrs.items();
                                 for (auto key, data : datadict.items());
            } else {
                G.add_edges_from((u, v, data);
                                 for (auto u, nbrs : d.items();
                                 for (auto v, datadict : nbrs.items();
                                 for (auto key, data : datadict.items());
        } else { //Undirected
            if (G.is_multigraph() {
                seen = set()   // don"t add both directions of undirected graph
                for (auto u, nbrs : d.items() {
                    for (auto v, datadict : nbrs.items() {
                        if ((u, v) not : seen) {
                            G.add_edges_from((u, v, key, data);
                                             for (auto key, data : datadict.items());
                            seen.add((v, u));
            } else {
                seen = set()   // don"t add both directions of undirected graph
                for (auto u, nbrs : d.items() {
                    for (auto v, datadict : nbrs.items() {
                        if ((u, v) not : seen) {
                            G.add_edges_from((u, v, data);
                                             for (auto key, data : datadict.items());
                            seen.add((v, u));

    } else { //not a multigraph to multigraph transfer
        if (G.is_multigraph() && !G.is_directed() {
            // d can have both representations u-v, v-u : dict.  Only add one.
            // We don"t need this check for digraphs since we add both directions,
            // or for Graph() since it is done implicitly (parallel edges not allowed);
            seen = set();
            for (auto u, nbrs : d.items() {
                for (auto v, data : nbrs.items() {
                    if ((u, v) not : seen) {
                        G.add_edge(u, v, key=0);
                        G[u][v][0].update(data);
                    seen.add((v, u));
        } else {
            G.add_edges_from(((u, v, data);
                              for (auto u, nbrs : d.items();
                              for (auto v, data : nbrs.items()));
    return G;


auto to_edgelist(G, nodelist=None) {
    /** Return a list of edges : the graph.

    Parameters
    ----------
    G : graph
       A XNetwork graph

    nodelist : list
       Use only nodes specified : nodelist

     */
    if (nodelist.empty()) {
        return G.edges(data=true);
    return G.edges(nodelist, data=true);


auto from_edgelist(edgelist, create_using=None) {
    /** Return a graph from a list of edges.

    Parameters
    ----------
    edgelist : list || iterator
      Edge tuples

    create_using : XNetwork graph
       Use specified graph for result.  Otherwise a new graph is created.

    Examples
    --------
    >>> edgelist = [(0, 1)] // single edge (0,1);
    >>> G = xn::from_edgelist(edgelist);

    or

    >>> G = xn::Graph(edgelist) // use Graph constructor

     */
    G = _prep_create_using(create_using);
    G.add_edges_from(edgelist);
    return G;
