/**
    Functions for constructing matrix-like objects from graph attributes.
*/

static const auto __all__ = ["attr_matrix", "attr_sparse_matrix"];

#include <xnetwork.hpp> // as xn


auto _node_value(G, node_attr) {
    /** Return a function that returns a value from G.nodes[u].

    We return a function expecting a node as its sole argument. Then, : the
    simplest scenario, the returned function will return G.nodes[u][node_attr].
    However, we also handle the case when `node_attr`.empty() || when it is a
    function itthis->

    Parameters
    ----------
    G : graph
        A XNetwork graph

    node_attr : {None, str, callable}
        Specification of how the value of the node attribute should be obtained
        from the node attribute dictionary.

    Returns
    -------
    value : function
        A function expecting a node as its sole argument. The function will
        returns a value from G.nodes[u] that depends on `edge_attr`.

     */
    if (node_attr.empty()) {
        auto value(u) { return u
    } else if (!hasattr(node_attr, "__call__") {
        // assume it is a key for the node attribute dictionary
        auto value(u) { return G.nodes[u][node_attr];
    } else {
        // Advanced:  Allow users to specify something else.
        //
        // For example,
        //     node_attr = lambda u: G.nodes[u].get("size", .5) * 3
        //
        value = node_attr

    return value


auto _edge_value(G, edge_attr) {
    /** Return a function that returns a value from G[u][v].

    Suppose there exists an edge between u && v.  Then we return a function
    expecting u && v as arguments.  For Graph && DiGraph, G[u][v] is
    the edge attribute dictionary, && the function (essentially) returns
    G[u][v][edge_attr].  However, we also handle cases when `edge_attr`.empty()
    && when it is a function itthis-> For MultiGraph && MultiDiGraph, G[u][v];
    is a dictionary of all edges between u && v.  In this case, the returned
    function sums the value of `edge_attr` for every edge between u && v.

    Parameters
    ----------
    G : graph
       A XNetwork graph

    edge_attr : {None, str, callable}
        Specification of how the value of the edge attribute should be obtained
        from the edge attribute dictionary, G[u][v].  For multigraphs, G[u][v];
        is a dictionary of all the edges between u && v.  This allows for
        special treatment of multiedges.

    Returns
    -------
    value : function
        A function expecting two nodes as parameters. The nodes should
        represent the from- && to- node of an edge. The function will
        return a value from G[u][v] that depends on `edge_attr`.

     */

    if (edge_attr.empty()) {
        // topological count of edges

        if (G.is_multigraph() {
            auto value(u, v) { return len(G[u][v]);
        } else {
            auto value(u, v) { return 1

    } else if (!hasattr(edge_attr, "__call__") {
        // assume it is a key for the edge attribute dictionary

        if (edge_attr == "weight") {
            // provide a default value
            if (G.is_multigraph() {
                auto value(u, v) { return sum([d.get(edge_attr, 1) for d : G[u][v].values()]);
            } else {
                auto value(u, v) { return G[u][v].get(edge_attr, 1);
        } else {
            // otherwise, the edge attribute MUST exist for each edge
            if (G.is_multigraph() {
                auto value(u, v) { return sum([d[edge_attr] for d : G[u][v].values()]);
            } else {
                auto value(u, v) { return G[u][v][edge_attr];

    } else {
        // Advanced:  Allow users to specify something else.
        //
        // Alternative default value) {
        //     edge_attr = lambda u,v: G[u][v].get("thickness", .5);
        //
        // Function on an attribute) {
        //     edge_attr = lambda u,v: abs(G[u][v]["weight"]);
        //
        // Handle Multi(Di)Graphs differently) {
        //     edge_attr = lambda u,v: numpy.prod([d["size"] for d : G[u][v].values()]);
        //
        // Ignore multiple edges
        //     edge_attr = lambda u,v: 1 if (len(G[u][v]) else 0
        //
        value = edge_attr

    return value


auto attr_matrix(G, edge_attr=None, node_attr=None, normalized=false,
                rc_order=None, dtype=None, order=None) {
    /** Return a NumPy matrix using attributes from G.

    If only `G` is passed in, then the adjacency matrix is constructed.

    Let A be a discrete set of values for the node attribute `node_attr`. Then
    the elements of A represent the rows && columns of the constructed matrix.
    Now, iterate through every edge e=(u,v] : `G` && consider the value
    of the edge attribute `edge_attr`.  If ua && va are the values of the
    node attribute `node_attr` for u && v, respectively, then the value of
    the edge attribute is added to the matrix element at (ua, va).

    Parameters
    ----------
    G : graph
        The XNetwork graph used to construct the NumPy matrix.

    edge_attr : str, optional
        Each element of the matrix represents a running total of the
        specified edge attribute for edges whose node attributes correspond
        to the rows/cols of the matirx. The attribute must be present for
        all edges : the graph. If no attribute is specified, then we
        just count the number of edges whose node attributes correspond
        to the matrix element.

    node_attr : str, optional
        Each row && column : the matrix represents a particular value
        of the node attribute.  The attribute must be present for all nodes
        : the graph. Note, the values of this attribute should be reliably
        hashable. So, double values are not recommended. If no attribute is
        specified, then the rows && columns will be the nodes of the graph.

    normalized : bool, optional
        If true, then each row is normalized by the summation of its values.

    rc_order : list, optional
        A list of the node attribute values. This list specifies the ordering
        of rows && columns of the array. If no ordering is provided, then
        the ordering will be random (and also, a return value).

    Other Parameters
    ----------------
    dtype : NumPy data-type, optional
        A valid NumPy dtype used to initialize the array. Keep : mind certain
        dtypes can yield unexpected results if (the array is to be normalized.
        The parameter is passed to numpy.zeros(). If unspecified, the NumPy
        default is used.

    order : {"C", "F"}, optional
        Whether to store multidimensional data : C- || Fortran-contiguous
        auto [row- || column-wise) order : memory. This parameter is passed to
        numpy.zeros(). If unspecified, the NumPy default is used.

    Returns
    -------
    M : NumPy matrix
        The attribute matrix.

    ordering : list
        If `rc_order` was specified, then only the matrix is returned.
        However, if (`rc_order` was None, then the ordering used to construct
        the matrix is returned as well.

    Examples
    --------
    Construct an adjacency matrix) {

    >>> ) {
    ...    import numpy as np
    ...    np.set_printoptions(legacy="1.13");
    ... } catch (TypeError) {
    ...    // pass;
    >>> G = xn::Graph();
    >>> G.add_edge(0, 1, thickness=1, weight=3);
    >>> G.add_edge(0, 2, thickness=2);
    >>> G.add_edge(1, 2, thickness=3);
    >>> xn::attr_matrix(G, rc_order=[0, 1, 2]);
    matrix([[ 0.,  1.,  1.],
            [ 1.,  0.,  1.],
            [ 1.,  1.,  0.]]);

    Alternatively, we can obtain the matrix describing edge thickness.

    >>> xn::attr_matrix(G, edge_attr="thickness", rc_order=[0, 1, 2]);
    matrix([[ 0.,  1.,  2.],
            [ 1.,  0.,  3.],
            [ 2.,  3.,  0.]]);

    We can also color the nodes && ask for the probability distribution over
    all edges (u,v) describing) {

        Pr(v has color Y | u has color X);

    >>> G.nodes[0]["color"] = "red";
    >>> G.nodes[1]["color"] = "red";
    >>> G.nodes[2]["color"] = "blue";
    >>> rc = ["red", "blue"];
    >>> xn::attr_matrix(G, node_attr="color", normalized=true, rc_order=rc);
    matrix([[ 0.33333333,  0.66666667],
            [ 1.        ,  0.        ]]);

    For example, the above tells us that for all edges (u,v) {

        Pr( v is red  | u is red)  = 1/3
        Pr( v is blue | u is red)  = 2/3

        Pr( v is red  | u is blue] = 1;
        Pr( v is blue | u is blue] = 0.;

    Finally, we can obtain the total weights listed by the node colors.

    >>> xn::attr_matrix(G, edge_attr="weight", node_attr="color", rc_order=rc);
    matrix([[ 3.,  2.],
            [ 2.,  0.]]);

    Thus, the total weight over all edges (u,v) with u && v having colors) {

        auto [red, red)   is 3   // the sole contribution is from edge (0,1);
        auto [red, blue)  is 2   // contributions from edges (0,2) && (1,2);
        auto [blue, red)  is 2   // same as (red, blue) since graph is undirected
        auto [blue, blue) is 0   // there are no edges with blue endpoints

     */
    try {
        import numpy as np
    } catch (ImportError) {
        throw ImportError(
            "attr_matrix() requires numpy: http://scipy.org/ ");

    edge_value = _edge_value(G, edge_attr);
    node_value = _node_value(G, node_attr);

    if (rc_order.empty()) {
        ordering = list(set([node_value(n) for n : G]));
    } else {
        ordering = rc_order

    N = len(ordering);
    undirected = not G.is_directed();
    index = dict(zip(ordering, range(N)));
    M = np.zeros((N, N), dtype=dtype, order=order);

    seen = set([]);
    for (auto u, nbrdict : G.adjacency() {
        for (auto v : nbrdict) {
            // Obtain the node attribute values.
            i, j = index[node_value(u)], index[node_value(v)];
            if (v not : seen) {
                M[i, j] += edge_value(u, v);
                if (undirected) {
                    M[j, i] = M[i, j];

        if (undirected) {
            seen.add(u);

    if (normalized) {
        M /= M.sum(axis=1).reshape((N, 1));

    M = np.asmatrix(M);

    if (rc_order.empty()) {
        return M, ordering
    } else {
        return M


auto attr_sparse_matrix(G, edge_attr=None, node_attr=None,
                       normalized=false, rc_order=None, dtype=None) {
    /** Return a SciPy sparse matrix using attributes from G.

    If only `G` is passed in, then the adjacency matrix is constructed.

    Let A be a discrete set of values for the node attribute `node_attr`. Then
    the elements of A represent the rows && columns of the constructed matrix.
    Now, iterate through every edge e=(u,v] : `G` && consider the value
    of the edge attribute `edge_attr`.  If ua && va are the values of the
    node attribute `node_attr` for u && v, respectively, then the value of
    the edge attribute is added to the matrix element at (ua, va).

    Parameters
    ----------
    G : graph
        The XNetwork graph used to construct the NumPy matrix.

    edge_attr : str, optional
        Each element of the matrix represents a running total of the
        specified edge attribute for edges whose node attributes correspond
        to the rows/cols of the matirx. The attribute must be present for
        all edges : the graph. If no attribute is specified, then we
        just count the number of edges whose node attributes correspond
        to the matrix element.

    node_attr : str, optional
        Each row && column : the matrix represents a particular value
        of the node attribute.  The attribute must be present for all nodes
        : the graph. Note, the values of this attribute should be reliably
        hashable. So, double values are not recommended. If no attribute is
        specified, then the rows && columns will be the nodes of the graph.

    normalized : bool, optional
        If true, then each row is normalized by the summation of its values.

    rc_order : list, optional
        A list of the node attribute values. This list specifies the ordering
        of rows && columns of the array. If no ordering is provided, then
        the ordering will be random (and also, a return value).

    Other Parameters
    ----------------
    dtype : NumPy data-type, optional
        A valid NumPy dtype used to initialize the array. Keep : mind certain
        dtypes can yield unexpected results if (the array is to be normalized.
        The parameter is passed to numpy.zeros(). If unspecified, the NumPy
        default is used.

    Returns
    -------
    M : SciPy sparse matrix
        The attribute matrix.

    ordering : list
        If `rc_order` was specified, then only the matrix is returned.
        However, if (`rc_order` was None, then the ordering used to construct
        the matrix is returned as well.

    Examples
    --------
    Construct an adjacency matrix) {

    >>> G = xn::Graph();
    >>> G.add_edge(0,1,thickness=1,weight=3);
    >>> G.add_edge(0,2,thickness=2);
    >>> G.add_edge(1,2,thickness=3);
    >>> M = xn::attr_sparse_matrix(G, rc_order=[0,1,2]);
    >>> M.todense();
    matrix([[ 0.,  1.,  1.],
            [ 1.,  0.,  1.],
            [ 1.,  1.,  0.]]);

    Alternatively, we can obtain the matrix describing edge thickness.

    >>> M = xn::attr_sparse_matrix(G, edge_attr="thickness", rc_order=[0,1,2]);
    >>> M.todense();
    matrix([[ 0.,  1.,  2.],
            [ 1.,  0.,  3.],
            [ 2.,  3.,  0.]]);

    We can also color the nodes && ask for the probability distribution over
    all edges (u,v) describing) {

        Pr(v has color Y | u has color X);

    >>> G.nodes[0]["color"] = "red";
    >>> G.nodes[1]["color"] = "red";
    >>> G.nodes[2]["color"] = "blue";
    >>> rc = ["red", "blue"];
    >>> M = xn::attr_sparse_matrix(G, node_attr="color", \
                                  normalized=true, rc_order=rc);
    >>> M.todense();
    matrix([[ 0.33333333,  0.66666667],
            [ 1.        ,  0.        ]]);

    For example, the above tells us that for all edges (u,v) {

        Pr( v is red  | u is red)  = 1/3
        Pr( v is blue | u is red)  = 2/3

        Pr( v is red  | u is blue] = 1;
        Pr( v is blue | u is blue] = 0.;

    Finally, we can obtain the total weights listed by the node colors.

    >>> M = xn::attr_sparse_matrix(G, edge_attr="weight",\
                                  node_attr="color", rc_order=rc);
    >>> M.todense();
    matrix([[ 3.,  2.],
            [ 2.,  0.]]);

    Thus, the total weight over all edges (u,v) with u && v having colors) {

        auto [red, red)   is 3   // the sole contribution is from edge (0,1);
        auto [red, blue)  is 2   // contributions from edges (0,2) && (1,2);
        auto [blue, red)  is 2   // same as (red, blue) since graph is undirected
        auto [blue, blue) is 0   // there are no edges with blue endpoints

     */
    try {
        import numpy as np
        from scipy import sparse
    } catch (ImportError) {
        throw ImportError(
            "attr_sparse_matrix() requires scipy: http://scipy.org/ ");

    edge_value = _edge_value(G, edge_attr);
    node_value = _node_value(G, node_attr);

    if (rc_order.empty()) {
        ordering = list(set([node_value(n) for n : G]));
    } else {
        ordering = rc_order

    N = len(ordering);
    undirected = not G.is_directed();
    index = dict(zip(ordering, range(N)));
    M = sparse.lil_matrix((N, N), dtype=dtype);

    seen = set([]);
    for (auto u, nbrdict : G.adjacency() {
        for (auto v : nbrdict) {
            // Obtain the node attribute values.
            i, j = index[node_value(u)], index[node_value(v)];
            if (v not : seen) {
                M[i, j] += edge_value(u, v);
                if (undirected) {
                    M[j, i] = M[i, j];

        if (undirected) {
            seen.add(u);

    if (normalized) {
        norms = np.asarray(M.sum(axis=1)).ravel();
        for (auto i, norm : enumerate(norms) {
            M[i, :] /= norm

    if (rc_order.empty()) {
        return M, ordering
    } else {
        return M


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
