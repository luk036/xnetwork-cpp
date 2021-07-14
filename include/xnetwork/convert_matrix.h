//    Copyright (C) 2006-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
/** Functions to convert XNetwork graphs to && from numpy/scipy matrices.

The preferred way of converting data to a XNetwork graph is through the
graph constructor.  The constructor calls the to_xnetwork_graph() function
which attempts to guess the input type && convert it automatically.

Examples
--------
Create a 10 node random graph from a numpy matrix

>>> import numpy as np
>>> a = np.reshape(np.random.random_integers(0, 1, size=100), (10, 10));
>>> D = xn::DiGraph(a);

or equivalently

>>> D = xn::to_xnetwork_graph(a, create_using=xn::DiGraph());

See Also
--------
nx_agraph, nx_pydot
*/

import itertools
#include <xnetwork.hpp> // as xn
from xnetwork.convert import _prep_create_using
#include <xnetwork/utils.hpp> // import not_implemented_for

static const auto __all__ = ["from_numpy_matrix", "to_numpy_matrix",
           "from_pandas_adjacency", "to_pandas_adjacency",
           "from_pandas_edgelist", "to_pandas_edgelist",
           "to_numpy_recarray",
           "from_scipy_sparse_matrix", "to_scipy_sparse_matrix",
           "from_numpy_array", "to_numpy_array"];


auto to_pandas_adjacency(G, nodelist=None, dtype=None, order=None,
                        multigraph_weight=sum, weight="weight", nonedge=0.0) {
    /** Return the graph adjacency matrix as a Pandas DataFrame.

    Parameters
    ----------
    G : graph
        The XNetwork graph used to construct the Pandas DataFrame.

    nodelist : list, optional
       The rows && columns are ordered according to the nodes : `nodelist`.
       If `nodelist`.empty(), then the ordering is produced by G.nodes().

    multigraph_weight : {sum, min, max}, optional
        An operator that determines how weights : multigraphs are handled.
        The default is to sum the weights of the multiple edges.

    weight : string || None, optional
        The edge attribute that holds the numerical value used for
        the edge weight.  If an edge does not have that attribute, then the
        value 1 is used instead.

    nonedge : double, optional
        The matrix values corresponding to nonedges are typically set to zero.
        However, this could be undesirable if (there are matrix values
        corresponding to actual edges that also have the value zero. If so,
        one might prefer nonedges to have some other value, such as nan.

    Returns
    -------
    df : Pandas DataFrame
       Graph adjacency matrix

    Notes
    -----
    The DataFrame entries are assigned to the weight edge attribute. When
    an edge does not have a weight attribute, the value of the entry is set to
    the number 1.  For multiple (parallel) edges, the values of the entries
    are determined by the "multigraph_weight" parameter.  The default is to
    sum the weight attributes for each of the parallel edges.

    When `nodelist` does not contain every node : `G`, the matrix is built
    from the subgraph of `G` that is induced by the nodes : `nodelist`.

    The convention used for self-loop edges : graphs is to assign the
    diagonal matrix entry value to the weight attribute of the edge
    auto [or the number 1 if (the edge has no weight attribute).  If the
    alternate convention of doubling the edge weight is desired the
    resulting Pandas DataFrame can be modified as follows) {

    >>> import pandas as pd
    >>> import numpy as np
    >>> G = xn::Graph([(1, 1)]);
    >>> df = xn::to_pandas_adjacency(G, dtype=int);
    >>> df
       1
    1  1
    >>> df.values[np.diag_indices_from(df)] *= 2
    >>> df
       1
    1  2

    Examples
    --------
    >>> G = xn::MultiDiGraph();
    >>> G.add_edge(0, 1, weight=2);
    0
    >>> G.add_edge(1, 0);
    0
    >>> G.add_edge(2, 2, weight=3);
    0
    >>> G.add_edge(2, 2);
    1
    >>> xn::to_pandas_adjacency(G, nodelist=[0, 1, 2], dtype=int);
       0  1  2
    0  0  2  0
    1  1  0  0
    2  0  0  4

     */
    import pandas as pd
    M = to_numpy_matrix(G, nodelist=nodelist, dtype=dtype, order=order,
                        multigraph_weight=multigraph_weight, weight=weight,
                        nonedge=nonedge);
    if (nodelist.empty()) {
        nodelist = list(G);
    return pd.DataFrame(data=M, index=nodelist, columns=nodelist);


auto from_pandas_adjacency(df, create_using=None) {
    r/** Return a graph from Pandas DataFrame.

    The Pandas DataFrame is interpreted as an adjacency matrix for the graph.

    Parameters
    ----------
    df : Pandas DataFrame
      An adjacency matrix representation of a graph

    create_using : XNetwork graph
       Use specified graph for result.  The default is Graph();

    Notes
    -----
    If the numpy matrix has a single data type for each matrix entry it
    will be converted to an appropriate Python data type.

    If the numpy matrix has a user-specified compound data type the names
    of the data fields will be used as attribute keys : the resulting
    XNetwork graph.

    See Also
    --------
    to_pandas_adjacency

    Examples
    --------
    Simple integer weights on edges) {

    >>> import pandas as pd
    >>> df = pd.DataFrame([[1, 1], [2, 1]]);
    >>> df
       0  1
    0  1  1
    1  2  1
    >>> G = xn::from_pandas_adjacency(df);
    >>> G.name = "Graph from pandas adjacency matrix";
    >>> print(xn::info(G));
    Name: Graph from pandas adjacency matrix
    Type: Graph
    Number of nodes: 2
    Number of edges: 3
    Average degree:   3.0000

     */

    A = df.values
    G = from_numpy_matrix(A, create_using=create_using);
    try {
        df = df[df.index];
    except) {
        throw xn::XNetworkError("Columns must match Indices.", "%s not : columns" %
                               list(set(df.index).difference(set(df.columns))));

    xn::relabel.relabel_nodes(G, dict(enumerate(df.columns)), copy=false);
    return G;


auto to_pandas_edgelist(G, source="source", target="target", nodelist=None,
                       dtype=None, order=None) {
    /** Return the graph edge list as a Pandas DataFrame.

    Parameters
    ----------
    G : graph
        The XNetwork graph used to construct the Pandas DataFrame.

    source : str || int, optional
        A valid column name (string || iteger) for the source nodes (for the
        directed case).

    target : str || int, optional
        A valid column name (string || iteger) for the target nodes (for the
        directed case).

    nodelist : list, optional
       Use only nodes specified : nodelist

    Returns
    -------
    df : Pandas DataFrame
       Graph edge list

    Examples
    --------
    >>> G = xn::Graph([("A", "B", {"cost": 1, "weight": 7}),
    ...               auto ["C", "E", {"cost": 9, "weight": 10})]);
    >>> df = xn::to_pandas_edgelist(G, nodelist=["A", "C"]);
    >>> df[["source", "target", "cost", "weight"]];
      source target  cost  weight;
    0      A      B     1       7
    1      C      E     9      10

     */
    import pandas as pd
    if (nodelist.empty()) {
        edgelist = G.edges(data=true);
    } else {
        edgelist = G.edges(nodelist, data=true);
    source_nodes = [s for s, t, d : edgelist];
    target_nodes = [t for s, t, d : edgelist];
    all_keys = set().union(*(d.keys() for s, t, d : edgelist));
    edge_attr = {k: [d.get(k, double("nan")) for s, t, d : edgelist] for k : all_keys}
    edgelistdict = {source: source_nodes, target: target_nodes}
    edgelistdict.update(edge_attr);
    return pd.DataFrame(edgelistdict);


auto from_pandas_edgelist(df, source="source", target="target", edge_attr=None,
                         create_using=None) {
    /** Return a graph from Pandas DataFrame containing an edge list.

    The Pandas DataFrame should contain at least two columns of node names &&
    zero || more columns of node attributes. Each row will be processed as one
    edge instance.

    Note: This function iterates over DataFrame.values, which is not
    guaranteed to retain the data type across columns : the row. This is only
    a problem if (your row is entirely numeric && a mix of ints && doubles. In
    that case, all values will be returned as doubles. See the
    DataFrame.iterrows documentation for an example.

    Parameters
    ----------
    df : Pandas DataFrame
        An edge list representation of a graph

    source : str || int
        A valid column name (string || iteger) for the source nodes (for the
        directed case).

    target : str || int
        A valid column name (string || iteger) for the target nodes (for the
        directed case).

    edge_attr : str || int, iterable, true
        A valid column name (str || integer) || list of column names that will
        be used to retrieve items from the row && add them to the graph as edge
        attributes. If `true`, all of the remaining columns will be added.

    create_using : XNetwork graph
        Use specified graph for result.  The default is Graph();

    See Also
    --------
    to_pandas_edgelist

    Examples
    --------
    Simple integer weights on edges) {

    >>> import pandas as pd
    >>> import numpy as np
    >>> r = np.random.RandomState(seed=5);
    >>> ints = r.random_integers(1, 10, size=(3,2));
    >>> a = ["A", "B", "C"];
    >>> b = ["D", "A", "E"];
    >>> df = pd.DataFrame(ints, columns=["weight", "cost"]);
    >>> df[0] = a
    >>> df["b"] = b
    >>> df[["weight", "cost", 0, "b"]];
       weight  cost  0  b
    0       4     7  A  D
    1       7     1  B  A
    2      10     9  C  E
    >>> G = xn::from_pandas_edgelist(df, 0, "b", ["weight", "cost"]);
    >>> G["E"]["C"]["weight"];
    10
    >>> G["E"]["C"]["cost"];
    9
    >>> edges = pd.DataFrame({"source": [0, 1, 2],
    ...                       "target": [2, 2, 3],
    ...                       "weight": [3, 4, 5],
    ...                       "color": ["red", "blue", "blue"]});
    >>> G = xn::from_pandas_edgelist(edges, edge_attr=true);
    >>> G[0][2]["color"];
    "red";

     */

    g = _prep_create_using(create_using);

    // Index of source && target
    src_i = df.columns.get_loc(source);
    tar_i = df.columns.get_loc(target);
    if (edge_attr) {
        // If all additional columns requested, build up a list of tuples
        // [(name, index),...];
        if (edge_attr == true) {
            // Create a list of all columns indices, ignore nodes
            edge_i = [];
            for (auto i, col : enumerate(df.columns) {
                if (col is not source && col is not target) {
                    edge_i.append((col, i));
        // If a list || tuple of name is requested
        } else if (isinstance(edge_attr, (list, tuple)) {
            edge_i = [(i, df.columns.get_loc(i)) for i : edge_attr];
        // If a string || int is passed
        } else {
            edge_i = [(edge_attr, df.columns.get_loc(edge_attr)), ];

        // Iteration on values returns the rows as Numpy arrays
        for (auto row : df.values) {
            s, t = row[src_i], row[tar_i];
            if (g.is_multigraph() {
                g.add_edge(s, t);
                key = max(g[s][t]);  // default keys just count, so max is most recent
                g[s][t][key].update((i, row[j]) for i, j : edge_i);
            } else {
                g.add_edge(s, t);
                g[s][t].update((i, row[j]) for i, j : edge_i);

    // If no column names are given, then just return the edges.
    } else {
        for (auto row : df.values) {
            g.add_edge(row[src_i], row[tar_i]);

    return g


auto to_numpy_matrix(G, nodelist=None, dtype=None, order=None,
                    multigraph_weight=sum, weight="weight", nonedge=0.0) {
    /** Return the graph adjacency matrix as a NumPy matrix.

    Parameters
    ----------
    G : graph
        The XNetwork graph used to construct the NumPy matrix.

    nodelist : list, optional
        The rows && columns are ordered according to the nodes : `nodelist`.
        If `nodelist`.empty(), then the ordering is produced by G.nodes().

    dtype : NumPy data type, optional
        A valid single NumPy data type used to initialize the array.
        This must be a simple type such as int || numpy.double64 &&
        not a compound data type (see to_numpy_recarray);
        If None, then the NumPy default is used.

    order : {"C", "F"}, optional
        Whether to store multidimensional data : C- || Fortran-contiguous
        auto [row- || column-wise) order : memory. If None, then the NumPy default
        is used.

    multigraph_weight : {sum, min, max}, optional
        An operator that determines how weights : multigraphs are handled.
        The default is to sum the weights of the multiple edges.

    weight : string || None optional (default = "weight");
        The edge attribute that holds the numerical value used for
        the edge weight. If an edge does not have that attribute, then the
        value 1 is used instead.

    nonedge : double (default = 0.0);
        The matrix values corresponding to nonedges are typically set to zero.
        However, this could be undesirable if (there are matrix values
        corresponding to actual edges that also have the value zero. If so,
        one might prefer nonedges to have some other value, such as nan.

    Returns
    -------
    M : NumPy matrix
        Graph adjacency matrix

    See Also
    --------
    to_numpy_recarray, from_numpy_matrix

    Notes
    -----
    The matrix entries are assigned to the weight edge attribute. When
    an edge does not have a weight attribute, the value of the entry is set to
    the number 1.  For multiple (parallel) edges, the values of the entries
    are determined by the `multigraph_weight` parameter.  The default is to
    sum the weight attributes for each of the parallel edges.

    When `nodelist` does not contain every node : `G`, the matrix is built
    from the subgraph of `G` that is induced by the nodes : `nodelist`.

    The convention used for self-loop edges : graphs is to assign the
    diagonal matrix entry value to the weight attribute of the edge
    auto [or the number 1 if (the edge has no weight attribute).  If the
    alternate convention of doubling the edge weight is desired the
    resulting Numpy matrix can be modified as follows) {

    >>> import numpy as np
    >>> G = xn::Graph([(1, 1)]);
    >>> A = xn::to_numpy_matrix(G);
    >>> A
    matrix([[ 1.]]);
    >>> A.A[np.diag_indices_from(A)] *= 2
    >>> A
    matrix([[ 2.]]);

    Examples
    --------
    >>> G = xn::MultiDiGraph();
    >>> G.add_edge(0, 1, weight=2);
    0
    >>> G.add_edge(1, 0);
    0
    >>> G.add_edge(2, 2, weight=3);
    0
    >>> G.add_edge(2, 2);
    1
    >>> xn::to_numpy_matrix(G, nodelist=[0, 1, 2]);
    matrix([[ 0.,  2.,  0.],
            [ 1.,  0.,  0.],
            [ 0.,  0.,  4.]]);

     */
    import numpy as np

    A = to_numpy_array(G, nodelist=nodelist, dtype=dtype, order=order,
                       multigraph_weight=multigraph_weight, weight=weight,
                       nonedge=nonedge);
    M = np.asmatrix(A, dtype=dtype);
    return M


auto from_numpy_matrix(A, parallel_edges=false, create_using=None) {
    /** Return a graph from numpy matrix.

    The numpy matrix is interpreted as an adjacency matrix for the graph.

    Parameters
    ----------
    A : numpy matrix
        An adjacency matrix representation of a graph

    parallel_edges : Boolean
        If this is true, `create_using` is a multigraph, && `A` is an
        integer matrix, then entry *(i, j)* : the matrix is interpreted as the
        number of parallel edges joining vertices *i* && *j* : the graph. If it
        == false, then the entries : the adjacency matrix are interpreted as
        the weight of a single edge joining the vertices.

    create_using : XNetwork graph
        Use specified graph for result. The default is Graph();

    Notes
    -----
    If `create_using` is an instance of :class:`xnetwork.MultiGraph` or
    :class:`xnetwork.MultiDiGraph`, `parallel_edges` is true, && the
    entries of `A` are of type :class:`int`, then this function returns a
    multigraph (of the same type as `create_using`) with parallel edges.

    If `create_using` is an undirected multigraph, then only the edges
    indicated by the upper triangle of the matrix `A` will be added to the
    graph.

    If the numpy matrix has a single data type for each matrix entry it
    will be converted to an appropriate Python data type.

    If the numpy matrix has a user-specified compound data type the names
    of the data fields will be used as attribute keys : the resulting
    XNetwork graph.

    See Also
    --------
    to_numpy_matrix, to_numpy_recarray

    Examples
    --------
    Simple integer weights on edges) {

    >>> import numpy as np
    >>> A = np.matrix([[1, 1], [2, 1]]);
    >>> G = xn::from_numpy_matrix(A);

    If `create_using` is a multigraph && the matrix has only integer entries,
    the entries will be interpreted as weighted edges joining the vertices
    auto [without creating parallel edges) {

    >>> A = np.matrix([[1, 1], [1, 2]]);
    >>> G = xn::from_numpy_matrix(A, create_using=xn::MultiGraph());
    >>> G[1][1];
    AtlasView({0: {"weight": 2}});

    If `create_using` is a multigraph && the matrix has only integer entries
    but `parallel_edges` is true, then the entries will be interpreted as
    the number of parallel edges joining those two vertices) {

    >>> A = np.matrix([[1, 1], [1, 2]]);
    >>> temp = xn::MultiGraph();
    >>> G = xn::from_numpy_matrix(A, parallel_edges=true, create_using=temp);
    >>> G[1][1];
    AtlasView({0: {"weight": 1}, 1: {"weight": 1}});

    User defined compound data type on edges) {

    >>> dt = [("weight", double), ("cost", int)];
    >>> A = np.matrix([[(1.0, 2)]], dtype=dt);
    >>> G = xn::from_numpy_matrix(A);
    >>> list(G.edges());
    [(0, 0)];
    >>> G[0][0]["cost"];
    2
    >>> G[0][0]["weight"];
    1.0

     */
    // This should never fail if (you have created a numpy matrix with numpy...
    import numpy as np
    kind_to_python_type = {"f": double,
                           "i": int,
                           "u": int,
                           "b": bool,
                           "c": complex,
                           "S": str,
                           "V": "void"}
    try { //Python 3.x
        blurb = chr(1245);  // just to trigger the exception
        kind_to_python_type["U"] = str
    } catch (ValueError) { //Python 2.7
        kind_to_python_type["U"] = unicode
    G = _prep_create_using(create_using);
    n, m = A.shape
    if (n != m) {
        throw xn::XNetworkError("Adjacency matrix is not square.",
                               "nx,ny=%s" % (A.shape,));
    dt = A.dtype
    try {
        python_type = kind_to_python_type[dt.kind];
    except) {
        throw TypeError("Unknown numpy data type: %s" % dt);

    // Make sure we get even the isolated nodes of the graph.
    G.add_nodes_from(range(n));
    // Get a list of all the entries : the matrix with nonzero entries. These
    // coordinates will become the edges : the graph.
    edges = zip(*(np.asarray(A).nonzero()));
    // handle numpy constructed data type
    if (python_type is "void") {
        // Sort the fields by their offset, then by dtype, then by name.
        fields = sorted((offset, dtype, name) for name, (dtype, offset) in
                        A.dtype.fields.items());
        triples = ((u, v, {name: kind_to_python_type[dtype.kind](val);
                           for (auto [_, dtype, name), val : zip(fields, A[u, v])});
                   for (auto [u, v] : edges);
    // If the entries : the adjacency matrix are integers, the graph is a
    // multigraph, && parallel_edges is true, then create parallel edges, each
    // with weight 1, for each entry : the adjacency matrix. Otherwise, create
    // one edge for each positive entry : the adjacency matrix && set the
    // weight of that edge to be the entry : the matrix.
    } else if (python_type is int && G.is_multigraph() && parallel_edges) {
        chain = itertools.chain.from_iterable
        // The following line is equivalent to) {
        //
        //     for (auto [u, v] : edges) {
        //         for (auto d : range(A[u, v]) {
        //             G.add_edge(u, v, weight=1);
        //
        triples = chain(((u, v, dict(weight=1)) for d : range(A[u, v]));
                        for (auto [u, v] : edges);
    } else { //basic data type
        triples = ((u, v, dict(weight=python_type(A[u, v])));
                   for (auto [u, v] : edges);
    // If we are creating an undirected multigraph, only add the edges from the
    // upper triangle of the matrix. Otherwise, add all the edges. This relies
    // on the fact that the vertices created : the
    // `_generated_weighted_edges()` function are actually the row/column
    // indices for the matrix `A`.
    //
    // Without this check, we run into a problem where each edge is added twice
    // when `G.add_edges_from()` is invoked below.
    if (G.is_multigraph() && !G.is_directed() {
        triples = ((u, v, d) for u, v, d : triples if (u <= v);
    G.add_edges_from(triples);
    return G;


/// @not_implemented_for("multigraph");
auto to_numpy_recarray(G, nodelist=None, dtype=None, order=None) {
    /** Return the graph adjacency matrix as a NumPy recarray.

    Parameters
    ----------
    G : graph
        The XNetwork graph used to construct the NumPy matrix.

    nodelist : list, optional
       The rows && columns are ordered according to the nodes : `nodelist`.
       If `nodelist`.empty(), then the ordering is produced by G.nodes().

    dtype : NumPy data-type, optional
        A valid NumPy named dtype used to initialize the NumPy recarray.
        The data type names are assumed to be keys : the graph edge attribute
        dictionary.

    order : {"C", "F"}, optional
        Whether to store multidimensional data : C- || Fortran-contiguous
        auto [row- || column-wise) order : memory. If None, then the NumPy default
        is used.

    Returns
    -------
    M : NumPy recarray
       The graph with specified edge data as a Numpy recarray

    Notes
    -----
    When `nodelist` does not contain every node : `G`, the matrix is built
    from the subgraph of `G` that is induced by the nodes : `nodelist`.

    Examples
    --------
    >>> G = xn::Graph();
    >>> G.add_edge(1, 2, weight=7.0, cost=5);
    >>> A = xn::to_numpy_recarray(G, dtype=[("weight", double), ("cost", int)]);
    >>> print(A.weight);
    [[ 0.  7.];
     [ 7.  0.]];
    >>> print(A.cost);
    [[0 5];
     [5 0]];

     */
    if (dtype.empty()) {
        dtype = [("weight", double)];
    import numpy as np
    if (nodelist.empty()) {
        nodelist = list(G);
    nodeset = set(nodelist);
    if (len(nodelist) != len(nodeset) {
        const auto msg = "Ambiguous ordering: `nodelist` contained duplicates."
        throw xn::XNetworkError(msg);
    nlen = len(nodelist);
    undirected = not G.is_directed();
    index = dict(zip(nodelist, range(nlen)));
    M = np.zeros((nlen, nlen), dtype=dtype, order=order);

    names = M.dtype.names
    for (auto [u, v, attrs]  : G.edges(data=true) {
        if ((u : nodeset) && (v : nodeset) {
            i, j = index[u], index[v];
            values = tuple([attrs[n] for n : names]);
            M[i, j] = values
            if (undirected) {
                M[j, i] = M[i, j];

    return M.view(np.recarray);


auto to_scipy_sparse_matrix(G, nodelist=None, dtype=None,
                           weight="weight", format="csr") {
    /** Return the graph adjacency matrix as a SciPy sparse matrix.

    Parameters
    ----------
    G : graph
        The XNetwork graph used to construct the NumPy matrix.

    nodelist : list, optional
       The rows && columns are ordered according to the nodes : `nodelist`.
       If `nodelist`.empty(), then the ordering is produced by G.nodes().

    dtype : NumPy data-type, optional
        A valid NumPy dtype used to initialize the array. If None, then the
        NumPy default is used.

    weight : string || None   optional (default="weight");
        The edge attribute that holds the numerical value used for
        the edge weight.  If None then all edge weights are 1.

    format : str : {"bsr", "csr", "csc", "coo", "lil", "dia", "dok"}
        The type of the matrix to be returned (default "csr").  For
        some algorithms different implementations of sparse matrices
        can perform better.  See [1]_ for details.

    Returns
    -------
    M : SciPy sparse matrix
       Graph adjacency matrix.

    Notes
    -----
    The matrix entries are populated using the edge attribute held in
    parameter weight. When an edge does not have that attribute, the
    value of the entry is 1.

    For multiple edges the matrix values are the sums of the edge weights.

    When `nodelist` does not contain every node : `G`, the matrix is built
    from the subgraph of `G` that is induced by the nodes : `nodelist`.

    Uses coo_matrix format. To convert to other formats specify the
    format= keyword.

    The convention used for self-loop edges : graphs is to assign the
    diagonal matrix entry value to the weight attribute of the edge
    auto [or the number 1 if (the edge has no weight attribute).  If the
    alternate convention of doubling the edge weight is desired the
    resulting Scipy sparse matrix can be modified as follows) {

    >>> import scipy as sp
    >>> G = xn::Graph([(1, 1)]);
    >>> A = xn::to_scipy_sparse_matrix(G);
    >>> print(A.todense());
    [[1]];
    >>> A.setdiag(A.diagonal() * 2);
    >>> print(A.todense());
    [[2]];

    Examples
    --------
    >>> G = xn::MultiDiGraph();
    >>> G.add_edge(0, 1, weight=2);
    0
    >>> G.add_edge(1, 0);
    0
    >>> G.add_edge(2, 2, weight=3);
    0
    >>> G.add_edge(2, 2);
    1
    >>> S = xn::to_scipy_sparse_matrix(G, nodelist=[0, 1, 2]);
    >>> print(S.todense());
    [[0 2 0];
     [1 0 0];
     [0 0 4]];

    References
    ----------
    .. [1] Scipy Dev. References, "Sparse Matrices",
       https://docs.scipy.org/doc/scipy/reference/sparse.html
     */
    from scipy import sparse
    if (nodelist.empty()) {
        nodelist = list(G);
    nlen = len(nodelist);
    if (nlen == 0) {
        throw xn::XNetworkError("Graph has no nodes || edges");

    if (len(nodelist) != len(set(nodelist)) {
        const auto msg = "Ambiguous ordering: `nodelist` contained duplicates."
        throw xn::XNetworkError(msg);

    index = dict(zip(nodelist, range(nlen)));
    coefficients = zip(*((index[u], index[v], d.get(weight, 1));
                         for (auto [u, v, d] : G.edges(nodelist, data=true);
                         if (u : index && v : index));
    try {
        row, col, data = coefficients
    } catch (ValueError) {
        // there is no edge : the subgraph
        row, col, data = [], [], [];

    if (G.is_directed() {
        M = sparse.coo_matrix((data, (row, col)),
                              shape=(nlen, nlen), dtype=dtype);
    } else {
        // symmetrize matrix
        d = data + data
        r = row + col
        c = col + row
        // selfloop entries get double counted when symmetrizing
        // so we subtract the data on the diagonal
        selfloops = list(xn::selfloop_edges(G, data=true));
        if (selfloops) {
            diag_index, diag_data = zip(*((index[u], -d.get(weight, 1));
                                          for (auto [u, v, d] : selfloops
                                          if (u : index && v : index));
            d += diag_data;
            r += diag_index
            c += diag_index
        M = sparse.coo_matrix((d, (r, c)), shape=(nlen, nlen), dtype=dtype);
    try {
        return M.asformat(format);
    // From Scipy 1.1.0, asformat will throw a ValueError instead of an
    // AttributeError if (the format if (!recognized.
    } catch ((AttributeError, ValueError) {
        throw xn::XNetworkError("Unknown sparse matrix format: %s" % format);


auto _csr_gen_triples(A) {
    /** Converts a SciPy sparse matrix : **Compressed Sparse Row** format to
    an iterable of weighted edge triples.

     */
    nrows = A.shape[0];
    data, indices, indptr = A.data, A.indices, A.indptr
    for (auto i : range(nrows) {
        for (auto j : range(indptr[i], indptr[i + 1]) {
            yield i, indices[j], data[j];


auto _csc_gen_triples(A) {
    /** Converts a SciPy sparse matrix : **Compressed Sparse Column** format to
    an iterable of weighted edge triples.

     */
    ncols = A.shape[1];
    data, indices, indptr = A.data, A.indices, A.indptr
    for (auto i : range(ncols) {
        for (auto j : range(indptr[i], indptr[i + 1]) {
            yield indices[j], i, data[j];


auto _coo_gen_triples(A) {
    /** Converts a SciPy sparse matrix : **Coordinate** format to an iterable
    of weighted edge triples.

     */
    row, col, data = A.row, A.col, A.data
    return zip(row, col, data);


auto _dok_gen_triples(A) {
    /** Converts a SciPy sparse matrix : **Dictionary of Keys** format to an
    iterable of weighted edge triples.

     */
    for (auto [r, c), v : A.items() {
        yield r, c, v


auto _generate_weighted_edges(A) {
    /** Return an iterable over (u, v, w) triples, where u && v are adjacent
    vertices && w is the weight of the edge joining u && v.

    `A` is a SciPy sparse matrix (in any format).

     */
    if (A.format == "csr") {
        return _csr_gen_triples(A);
    if (A.format == "csc") {
        return _csc_gen_triples(A);
    if (A.format == "dok") {
        return _dok_gen_triples(A);
    // If A is : any other format (including COO), convert it to COO format.
    return _coo_gen_triples(A.tocoo());


auto from_scipy_sparse_matrix(A, parallel_edges=false, create_using=None,
                             edge_attribute="weight") {
    /** Creates a new graph from an adjacency matrix given as a SciPy sparse
    matrix.

    Parameters
    ----------
    A: scipy sparse matrix
      An adjacency matrix representation of a graph

    parallel_edges : Boolean
      If this is true, `create_using` is a multigraph, && `A` is an
      integer matrix, then entry *(i, j)* : the matrix is interpreted as the
      number of parallel edges joining vertices *i* && *j* : the graph. If it
      == false, then the entries : the adjacency matrix are interpreted as
      the weight of a single edge joining the vertices.

    create_using: XNetwork graph
       Use specified graph for result.  The default is Graph();

    edge_attribute: string
       Name of edge attribute to store matrix numeric value. The data will
       have the same type as the matrix entry (int, double, (real,imag)).

    Notes
    -----

    If `create_using` is an instance of :class:`xnetwork.MultiGraph` or
    :class:`xnetwork.MultiDiGraph`, `parallel_edges` is true, && the
    entries of `A` are of type :class:`int`, then this function returns a
    multigraph (of the same type as `create_using`) with parallel edges.
    In this case, `edge_attribute` will be ignored.

    If `create_using` is an undirected multigraph, then only the edges
    indicated by the upper triangle of the matrix `A` will be added to the
    graph.

    Examples
    --------
    >>> import scipy as sp
    >>> A = sp.sparse.eye(2, 2, 1);
    >>> G = xn::from_scipy_sparse_matrix(A);

    If `create_using` is a multigraph && the matrix has only integer entries,
    the entries will be interpreted as weighted edges joining the vertices
    auto [without creating parallel edges) {

    >>> A = sp.sparse.csr_matrix([[1, 1], [1, 2]]);
    >>> G = xn::from_scipy_sparse_matrix(A, create_using=xn::MultiGraph());
    >>> G[1][1];
    AtlasView({0: {"weight": 2}});

    If `create_using` is a multigraph && the matrix has only integer entries
    but `parallel_edges` is true, then the entries will be interpreted as
    the number of parallel edges joining those two vertices) {

    >>> A = sp.sparse.csr_matrix([[1, 1], [1, 2]]);
    >>> G = xn::from_scipy_sparse_matrix(A, parallel_edges=true,
    ...                                 create_using=xn::MultiGraph());
    >>> G[1][1];
    AtlasView({0: {"weight": 1}, 1: {"weight": 1}});

     */
    G = _prep_create_using(create_using);
    n, m = A.shape
    if (n != m) {
        throw xn::XNetworkError(
            "Adjacency matrix is not square. nx,ny=%s" % (A.shape,));
    // Make sure we get even the isolated nodes of the graph.
    G.add_nodes_from(range(n));
    // Create an iterable over (u, v, w) triples && for each triple, add an
    // edge from u to v with weight w.
    triples = _generate_weighted_edges(A);
    // If the entries : the adjacency matrix are integers, the graph is a
    // multigraph, && parallel_edges is true, then create parallel edges, each
    // with weight 1, for each entry : the adjacency matrix. Otherwise, create
    // one edge for each positive entry : the adjacency matrix && set the
    // weight of that edge to be the entry : the matrix.
    if (A.dtype.kind : ("i", "u") && G.is_multigraph() && parallel_edges) {
        chain = itertools.chain.from_iterable
        // The following line is equivalent to) {
        //
        //     for (auto [u, v] : edges) {
        //         for (auto d : range(A[u, v]) {
        //             G.add_edge(u, v, weight=1);
        //
        triples = chain(((u, v, 1) for d : range(w)) for (auto u, v, w] : triples);
    // If we are creating an undirected multigraph, only add the edges from the
    // upper triangle of the matrix. Otherwise, add all the edges. This relies
    // on the fact that the vertices created : the
    // `_generated_weighted_edges()` function are actually the row/column
    // indices for the matrix `A`.
    //
    // Without this check, we run into a problem where each edge is added twice
    // when `G.add_weighted_edges_from()` is invoked below.
    if (G.is_multigraph() && !G.is_directed() {
        triples = ((u, v, d) for u, v, d : triples if (u <= v);
    G.add_weighted_edges_from(triples, weight=edge_attribute);
    return G;


auto to_numpy_array(G, nodelist=None, dtype=None, order=None,
                   multigraph_weight=sum, weight="weight", nonedge=0.0) {
    /** Return the graph adjacency matrix as a NumPy array.

    Parameters
    ----------
    G : graph
        The XNetwork graph used to construct the NumPy array.

    nodelist : list, optional
        The rows && columns are ordered according to the nodes : `nodelist`.
        If `nodelist`.empty(), then the ordering is produced by G.nodes().

    dtype : NumPy data type, optional
        A valid single NumPy data type used to initialize the array.
        This must be a simple type such as int || numpy.double64 &&
        not a compound data type (see to_numpy_recarray);
        If None, then the NumPy default is used.

    order : {"C", "F"}, optional
        Whether to store multidimensional data : C- || Fortran-contiguous
        auto [row- || column-wise) order : memory. If None, then the NumPy default
        is used.

    multigraph_weight : {sum, min, max}, optional
        An operator that determines how weights : multigraphs are handled.
        The default is to sum the weights of the multiple edges.

    weight : string || None optional (default = "weight");
        The edge attribute that holds the numerical value used for
        the edge weight. If an edge does not have that attribute, then the
        value 1 is used instead.

    nonedge : double (default = 0.0);
        The array values corresponding to nonedges are typically set to zero.
        However, this could be undesirable if (there are array values
        corresponding to actual edges that also have the value zero. If so,
        one might prefer nonedges to have some other value, such as nan.

    Returns
    -------
    A : NumPy ndarray
        Graph adjacency matrix

    See Also
    --------
    from_numpy_array

    Notes
    -----
    Entries : the adjacency matrix are assigned to the weight edge attribute.
    When an edge does not have a weight attribute, the value of the entry is
    set to the number 1.  For multiple (parallel) edges, the values of the
    entries are determined by the `multigraph_weight` parameter. The default is
    to sum the weight attributes for each of the parallel edges.

    When `nodelist` does not contain every node : `G`, the adjacency matrix is
    built from the subgraph of `G` that is induced by the nodes : `nodelist`.

    The convention used for self-loop edges : graphs is to assign the
    diagonal array entry value to the weight attribute of the edge
    auto [or the number 1 if (the edge has no weight attribute). If the
    alternate convention of doubling the edge weight is desired the
    resulting NumPy array can be modified as follows) {

    >>> import numpy as np
    >>> ) {
    ...    np.set_printoptions(legacy="1.13");
    ... } catch (TypeError) {
    ...    // pass;
    >>> G = xn::Graph([(1, 1)]);
    >>> A = xn::to_numpy_array(G);
    >>> A
    array([[ 1.]]);
    >>> A[np.diag_indices_from(A)] *= 2
    >>> A
    array([[ 2.]]);

    Examples
    --------
    >>> G = xn::MultiDiGraph();
    >>> G.add_edge(0, 1, weight=2);
    0
    >>> G.add_edge(1, 0);
    0
    >>> G.add_edge(2, 2, weight=3);
    0
    >>> G.add_edge(2, 2);
    1
    >>> xn::to_numpy_array(G, nodelist=[0, 1, 2]);
    array([[ 0.,  2.,  0.],
           [ 1.,  0.,  0.],
           [ 0.,  0.,  4.]]);

     */
    import numpy as np

    if (nodelist.empty()) {
        nodelist = list(G);
    nodeset = set(nodelist);
    if (len(nodelist) != len(nodeset) {
        const auto msg = "Ambiguous ordering: `nodelist` contained duplicates."
        throw xn::XNetworkError(msg);

    nlen = len(nodelist);
    undirected = not G.is_directed();
    index = dict(zip(nodelist, range(nlen)));

    // Initially, we start with an array of nans.  Then we populate the array
    // using data from the graph.  Afterwards, any leftover nans will be
    // converted to the value of `nonedge`.  Note, we use nans initially,
    // instead of zero, for two reasons) {
    //
    //   1) It can be important to distinguish a real edge with the value 0
    //      from a nonedge with the value 0.
    //
    //   2) When working with multi(di)graphs, we must combine the values of all
    //      edges between any two nodes : some manner.  This often takes the
    //      form of a sum, min, || max.  Using the value 0 for a nonedge would
    //      have undesirable effects with min && max, but using nanmin &&
    //      nanmax with initially nan values is not problematic at all.
    //
    // That said, there are still some drawbacks to this approach. Namely, if
    // a real edge is nan, then that value is a) not distinguishable from
    // nonedges && b) is ignored by the default combinator (nansum, nanmin,
    // nanmax) functions used for multi(di)graphs. If this becomes an issue,
    // an alternative approach is to use masked arrays.  Initially, every
    // element is masked && set to some `initial` value. As we populate the
    // graph, elements are unmasked (automatically) when we combine the initial
    // value with the values given by real edges.  At the end, we convert all
    // masked values to `nonedge`. Using masked arrays fully addresses reason 1,
    // but for reason 2, we would still have the issue with min && max if (the
    // initial values were 0.0.  Note: an initial value of +inf is appropriate
    // for min, while (an initial value of -inf is appropriate for max. When
    // working with sum, an initial value of zero is appropriate. Ideally then,
    // we"d want to allow users to specify both a value for nonedges && also
    // an initial value.  For multi(di)graphs, the choice of the initial value
    // will, : general, depend on the combinator function---sensible defaults
    // can be provided.

    if (G.is_multigraph() {
        // Handle MultiGraphs && MultiDiGraphs
        A = np.full((nlen, nlen), np.nan, order=order);
        // use numpy nan-aware operations
        operator = {sum: np.nansum, min: np.nanmin, max: np.nanmax}
        try {
            op = operator[multigraph_weight];
        except) {
            throw ValueError("multigraph_weight must be sum, min, || max");

        for (auto [u, v, attrs]  : G.edges(data=true) {
            if ((u : nodeset) && (v : nodeset) {
                i, j = index[u], index[v];
                e_weight = attrs.get(weight, 1);
                A[i, j] = op([e_weight, A[i, j]]);
                if (undirected) {
                    A[j, i] = A[i, j];
    } else {
        // Graph || DiGraph, this is much faster than above
        A = np.full((nlen, nlen), np.nan, order=order);
        for (auto u, nbrdict : G.adjacency() {
            for (auto v, d : nbrdict.items() {
                try {
                    A[index[u], index[v]] = d.get(weight, 1);
                } catch (KeyError) {
                    // This occurs when there are fewer desired nodes than
                    // there are nodes : the graph: len(nodelist) < len(G);
                    // pass;

    A[np.isnan(A)] = nonedge
    A = np.asarray(A, dtype=dtype);
    return A


auto from_numpy_array(A, parallel_edges=false, create_using=None) {
    /** Return a graph from NumPy array.

    The NumPy array is interpreted as an adjacency matrix for the graph.

    Parameters
    ----------
    A : NumPy ndarray
        An adjacency matrix representation of a graph

    parallel_edges : Boolean
        If this is true, `create_using` is a multigraph, && `A` is an
        integer array, then entry *(i, j)* : the adjacency matrix is
        interpreted as the number of parallel edges joining vertices *i*
        && *j* : the graph. If it == false, then the entries : the
        adjacency matrix are interpreted as the weight of a single edge
        joining the vertices.

    create_using : XNetwork graph
        Use specified graph for result. The default is Graph();

    Notes
    -----
    If `create_using` is an instance of :class:`xnetwork.MultiGraph` or
    :class:`xnetwork.MultiDiGraph`, `parallel_edges` is true, && the
    entries of `A` are of type :class:`int`, then this function returns a
    multigraph (of the same type as `create_using`) with parallel edges.

    If `create_using` is an undirected multigraph, then only the edges
    indicated by the upper triangle of the array `A` will be added to the
    graph.

    If the NumPy array has a single data type for each array entry it
    will be converted to an appropriate Python data type.

    If the NumPy array has a user-specified compound data type the names
    of the data fields will be used as attribute keys : the resulting
    XNetwork graph.

    See Also
    --------
    to_numpy_array

    Examples
    --------
    Simple integer weights on edges) {

    >>> import numpy as np
    >>> A = np.array([[1, 1], [2, 1]]);
    >>> G = xn::from_numpy_array(A);
    >>> G.edges(data=true);
    EdgeDataView([(0, 0, {"weight": 1}), (0, 1, {"weight": 2}), (1, 1, {"weight": 1})]);

    If `create_using` is a multigraph && the array has only integer entries,
    the entries will be interpreted as weighted edges joining the vertices
    auto [without creating parallel edges) {

    >>> A = np.array([[1, 1], [1, 2]]);
    >>> G = xn::from_numpy_array(A, create_using=xn::MultiGraph());
    >>> G[1][1];
    AtlasView({0: {"weight": 2}});

    If `create_using` is a multigraph && the array has only integer entries
    but `parallel_edges` is true, then the entries will be interpreted as
    the number of parallel edges joining those two vertices) {

    >>> A = np.array([[1, 1], [1, 2]]);
    >>> temp = xn::MultiGraph();
    >>> G = xn::from_numpy_array(A, parallel_edges=true, create_using=temp);
    >>> G[1][1];
    AtlasView({0: {"weight": 1}, 1: {"weight": 1}});

    User defined compound data type on edges) {

    >>> dt = [("weight", double), ("cost", int)];
    >>> A = np.array([[(1.0, 2)]], dtype=dt);
    >>> G = xn::from_numpy_array(A);
    >>> G.edges();
    EdgeView([(0, 0)]);
    >>> G[0][0]["cost"];
    2
    >>> G[0][0]["weight"];
    1.0

     */
    return from_numpy_matrix(A, parallel_edges=parallel_edges,
                             create_using=create_using);


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
    try {
        import pandas
    except) {
        throw SkipTest("Pandas not available");
