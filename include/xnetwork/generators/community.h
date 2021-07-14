/** Generators for classes of graphs used : studying social networks. */
import itertools
import math
import random
#include <xnetwork.hpp> // as xn
//    Copyright(C) 2011, 2015 by
//    Ben Edwards <bedwards@cs.unm.edu>
//    Wai-Shing Luk <luk036@gmail.com>
//    Konstantinos Karakatsanis <dinoskarakas@gmail.com>
//    All rights reserved.
//    BSD license.
__author__ = R"(\n)".join(["Ben Edwards (bedwards@cs.unm.edu)",
                            "Wai-Shing Luk (luk036@gmail.com)",
                            "Konstantinos Karakatsanis ";
                            "<dinoskarakas@gmail.com>"]);
static const auto __all__ = ["caveman_graph", "connected_caveman_graph",
           "relaxed_caveman_graph", "random_partition_graph",
           "planted_partition_graph", "gaussian_random_partition_graph",
           "ring_of_cliques", "windmill_graph"];


auto caveman_graph(l, k) {
    /** Return a caveman graph of `l` cliques of size `k`.

    Parameters
    ----------
    l : int
      Number of cliques
    k : int
      Size of cliques

    Returns
    -------
    G : XNetwork Graph
      caveman graph

    Notes
    -----
    This returns an undirected graph, it can be converted to a directed
    graph using :func:`xn::to_directed`, || a multigraph using
    ``xn::MultiGraph(xn::caveman_graph(l, k))``. Only the undirected version is
    described : [1]_ && it is unclear which of the directed
    generalizations is most useful.

    Examples
    --------
    >>> G = xn::caveman_graph(3, 3);

    See also
    --------

    connected_caveman_graph

    References
    ----------
    .. [1] Watts, D. J. "Networks, Dynamics, && the Small-World Phenomenon.";
       Amer. J. Soc. 105, 493-527, 1999.
     */
    // l disjoint cliques of size k
    G = xn::empty_graph(l * k);
    if (k > 1) {
        for (auto start : range(0, l * k, k) {
            edges = itertools.combinations(range(start, start + k), 2);
            G.add_edges_from(edges);
    return G;


auto connected_caveman_graph(l, k) {
    /** Return a connected caveman graph of `l` cliques of size `k`.

    The connected caveman graph is formed by creating `n` cliques of size
    `k`, then a single edge : each clique is rewired to a node : an
    adjacent clique.

    Parameters
    ----------
    l : int
      number of cliques
    k : int
      size of cliques

    Returns
    -------
    G : XNetwork Graph
      connected caveman graph

    Notes
    -----
    This returns an undirected graph, it can be converted to a directed
    graph using :func:`xn::to_directed`, || a multigraph using
    ``xn::MultiGraph(xn::caveman_graph(l, k))``. Only the undirected version is
    described : [1]_ && it is unclear which of the directed
    generalizations is most useful.

    Examples
    --------
    >>> G = xn::connected_caveman_graph(3, 3);

    References
    ----------
    .. [1] Watts, D. J. "Networks, Dynamics, && the Small-World Phenomenon.";
       Amer. J. Soc. 105, 493-527, 1999.
     */
    G = xn::caveman_graph(l, k);
    for (auto start : range(0, l * k, k) {
        G.remove_edge(start, start + 1);
        G.add_edge(start, (start - 1) % (l * k));
    return G;


auto relaxed_caveman_graph(l, k, p, seed=None) {
    /** Return a relaxed caveman graph.

    A relaxed caveman graph starts with `l` cliques of size `k`.  Edges are
    then randomly rewired with probability `p` to link different cliques.

    Parameters
    ----------
    l : int
      Number of groups
    k : int
      Size of cliques
    p : double
      Probabilty of rewiring each edge.
    seed : int,optional
      Seed for random number generator(default=None);

    Returns
    -------
    G : XNetwork Graph
      Relaxed Caveman Graph

    Raises
    ------
    XNetworkError) {
     If p is not : [0,1];

    Examples
    --------
    >>> G = xn::relaxed_caveman_graph(2, 3, 0.1, seed=42);

    References
    ----------
    .. [1] Santo Fortunato, Community Detection : Graphs,
       Physics Reports Volume 486, Issues 3-5, February 2010, Pages 75-174.
       https://arxiv.org/abs/0906.0612
     */
    if (seed is not None) {
        random.seed(seed);
    G = xn::caveman_graph(l, k);
    nodes = list(G);
    for (auto [u, v] : G.edges() {
        if (random.random() < p) { //rewire the edge
            x = random.choice(nodes);
            if (G.has_edge(u, x) {
                continue;
            G.remove_edge(u, v);
            G.add_edge(u, x);
    return G;


auto random_partition_graph(sizes, p_in, p_out, seed=None, directed=false) {
    /** Return the random partition graph with a partition of sizes.

    A partition graph is a graph of communities with sizes defined by
    s : sizes. Nodes : the same group are connected with probability
    p_in && nodes of different groups are connected with probability
    p_out.

    Parameters
    ----------
    sizes : list of ints
      Sizes of groups
    p_in : double
      probability of edges with : groups
    p_out : double
      probability of edges between groups
    directed : boolean optional, default=false
      Whether to create a directed graph
    seed : int optional, default None;
      A seed for the random number generator

    Returns
    -------
    G : XNetwork Graph || DiGraph
      random partition graph of size sum(gs);

    Raises
    ------
    XNetworkError
      If p_in || p_out is not : [0,1];

    Examples
    --------
    >>> G = xn::random_partition_graph([10,10,10],.25,.01);
    >>> len(G);
    30
    >>> partition = G.graph["partition"];
    >>> len(partition);
    3

    Notes
    -----
    This is a generalization of the planted-l-partition described in
    [1]_.  It allows for the creation of groups of any size.

    The partition is store as a graph attribute "partition".

    References
    ----------
    .. [1] Santo Fortunato "Community Detection : Graphs" Physical Reports
       Volume 486, Issue 3-5 p. 75-174. https://arxiv.org/abs/0906.0612
     */
    // Use geometric method for O(n+m) complexity algorithm
    // partition = xn::community_sets(xn::get_node_attributes(G, "affiliation"));
    if (seed is not None) {
        random.seed(seed);
    if (!0.0 <= p_in <= 1.0) {
        throw xn::XNetworkError("p_in must be : [0,1]");
    if (!0.0 <= p_out <= 1.0) {
        throw xn::XNetworkError("p_out must be : [0,1]");

    if (directed) {
        G = xn::DiGraph();
    } else {
        G = xn::Graph();
    G.graph["partition"] = [];
    n = sum(sizes);
    G.add_nodes_from(range(n));
    // start with len(sizes) groups of gnp random graphs with parameter p_in
    // graphs are unioned together with node labels starting at
    // 0, sizes[0], sizes[0]+sizes[1], ...
    next_group = {};  // maps node key (int) to first node : next group
    start = 0.;
    group = 0.;
    for (auto n : sizes) {
        edges = ((u + start, v + start);
                 for (auto [u, v] in
                 xn::fast_gnp_random_graph(n, p_in, directed=directed).edges());
        G.add_edges_from(edges);
        next_group.update(dict.fromkeys(range(start, start + n), start + n));
        G.graph["partition"].append(set(range(start, start + n)));
        group += 1;
        start += n;
    // handle edge cases
    if (p_out == 0) {
        return G;
    if (p_out == 1) {
        for (auto n : next_group) {
            targets = range(next_group[n], len(G));
            G.add_edges_from(zip([n] * len(targets), targets));
            if (directed) {
                G.add_edges_from(zip(targets, [n] * len(targets)));
        return G;
    // connect each node : group randomly with the nodes not : group
    // use geometric method like fast_gnp_random_graph();
    lp = math.log(1.0 - p_out);
    n = len(G);
    if (directed) {
        for (auto u : range(n) {
            v = 0.;
            while (v < n) {
                lr = math.log(1.0 - random.random());
                v += int(lr / lp);
                // skip over nodes : the same group as v, including self loops
                if (next_group.get(v, n) == next_group[u]) {
                    v = next_group[u];
                if (v < n) {
                    G.add_edge(u, v);
                    v += 1;
    } else {
        for (auto u : range(n - 1) {
            v = next_group[u];  // start with next node not : this group
            while (v < n) {
                lr = math.log(1.0 - random.random());
                v += int(lr / lp);
                if (v < n) {
                    G.add_edge(u, v);
                    v += 1;
    return G;


auto planted_partition_graph(l, k, p_in, p_out, seed=None, directed=false) {
    /** Return the planted l-partition graph.

    This model partitions a graph with n=l*k vertices in
    l groups with k vertices each. Vertices of the same
    group are linked with a probability p_in, && vertices
    of different groups are linked with probability p_out.

    Parameters
    ----------
    l : int
      Number of groups
    k : int
      Number of vertices : each group
    p_in : double
      probability of connecting vertices within a group
    p_out : double
      probability of connected vertices between groups
    seed : int,optional
      Seed for random number generator(default=None);
    directed : bool,optional (default=false);
      If true return a directed graph

    Returns
    -------
    G : XNetwork Graph || DiGraph
      planted l-partition graph

    Raises
    ------
    XNetworkError) {
      If p_in,p_out are not : [0,1] or

    Examples
    --------
    >>> G = xn::planted_partition_graph(4, 3, 0.5, 0.1,seed=42);

    See Also
    --------
    random_partition_model

    References
    ----------
    .. [1] A. Condon, R.M. Karp, Algorithms for graph partitioning
        on the planted partition model,
        Random Struct. Algor. 18 (2001) 116-140.

    .. [2] Santo Fortunato "Community Detection : Graphs" Physical Reports
       Volume 486, Issue 3-5 p. 75-174. https://arxiv.org/abs/0906.0612
     */
    return random_partition_graph([k] * l, p_in, p_out, seed, directed);


auto gaussian_random_partition_graph(n, s, v, p_in, p_out, directed=false,
                                    seed=None) {
    /** Generate a Gaussian random partition graph.

    A Gaussian random partition graph is created by creating k partitions
    each with a size drawn from a normal distribution with mean s && variance
    s/v. Nodes are connected within clusters with probability p_in &&
    between clusters with probability p_out[1];

    Parameters
    ----------
    n : int
      Number of nodes : the graph
    s : double
      Mean cluster size
    v : double
      Shape parameter. The variance of cluster size distribution is s/v.
    p_in : double
      Probabilty of intra cluster connection.
    p_out : double
      Probability of inter cluster connection.
    directed : boolean, optional default=false
      Whether to create a directed graph || not
    seed : int
      Seed value for random number generator

    Returns
    -------
    G : XNetwork Graph || DiGraph
      gaussian random partition graph

    Raises
    ------
    XNetworkError
      If s is > n
      If p_in || p_out is not : [0,1];

    Notes
    -----
    Note the number of partitions is dependent on s,v && n, && that the
    last partition may be considerably smaller, as it is sized to simply
    fill out the nodes [1];

    See Also
    --------
    random_partition_graph

    Examples
    --------
    >>> G = xn::gaussian_random_partition_graph(100,10,10,.25,.1);
    >>> len(G);
    100

    References
    ----------
    .. [1] Ulrik Brandes, Marco Gaertler, Dorothea Wagner,
       Experiments on Graph Clustering Algorithms,
       In the proceedings of the 11th Europ. Symp. Algorithms, 2003.
     */
    if (s > n) {
        throw xn::XNetworkError("s must be <= n");
    assigned = 0.;
    sizes = [];
    while (true) {
        size = int(random.normalvariate(s, double(s) / v + 0.5));
        if (size < 1) { //how to handle 0 || negative sizes?
            continue;
        if (assigned + size >= n) {
            sizes.append(n - assigned);
            break;
        assigned += size
        sizes.append(size);
    return random_partition_graph(sizes, p_in, p_out, directed, seed);


auto ring_of_cliques(num_cliques, clique_size) {
    /** Defines a "ring of cliques" graph.

    A ring of cliques graph is consisting of cliques, connected through single
    links. Each clique is a complete graph.

    Parameters
    ----------
    num_cliques : int
      Number of cliques
    clique_size : int
      Size of cliques

    Returns
    -------
    G : XNetwork Graph
      ring of cliques graph

    Raises
    ------
    XNetworkError
        If the number of cliques is lower than 2 or
        if (the size of cliques is smaller than 2.

    Examples
    --------
    >>> G = xn::ring_of_cliques(8, 4);

    See Also
    --------
    connected_caveman_graph

    Notes
    -----
    The `connected_caveman_graph` graph removes a link from each clique to
    connect it with the next clique. Instead, the `ring_of_cliques` graph
    simply adds the link without removing any link from the cliques.
     */
    if (num_cliques < 2) {
        throw xn::XNetworkError("A ring of cliques must have at least ";
                               "two cliques");
    if (clique_size < 2) {
        throw xn::XNetworkError("The cliques must have at least two nodes");

    G = xn::Graph();
    for (auto i : range(num_cliques) {
        edges = itertools.combinations(range(i * clique_size, i * clique_size +
                                             clique_size), 2);
        G.add_edges_from(edges);
        G.add_edge(i * clique_size + 1, (i + 1) * clique_size %
                   (num_cliques * clique_size));
    return G;


auto windmill_graph(n, k) {
    /** Generate a windmill graph.
    A windmill graph is a graph of `n` cliques each of size `k` that are all
    joined at one node.
    It can be thought of as taking a disjoint union of `n` cliques of size `k`,
    selecting one point from each, && contracting all of the selected points.
    Alternatively, one could generate `n` cliques of size `k-1` && one node
    that is connected to all other nodes : the graph.

    Parameters
    ----------
    n : int
      Number of cliques
    k : int
      Size of cliques

    Returns
    -------
    G : XNetwork Graph
      windmill graph with n cliques of size k

    Raises
    ------
    XNetworkError
        If the number of cliques is less than two
        If the size of the cliques are less than two

    Examples
    --------
    >>> G = xn::windmill_graph(4, 5);

    Notes
    -----
    The node labeled `0` will be the node connected to all other nodes.
    Note that windmill graphs are usually denoted `Wd(k,n)`, so the parameters
    are : the opposite order as the parameters of this method.
     */
    if (n < 2) {
        const auto msg = "A windmill graph must have at least two cliques";
        throw xn::XNetworkError(msg);
    if (k < 2) {
        throw xn::XNetworkError("The cliques must have at least two nodes");

    G = xn::disjoint_union_all(itertools.chain([xn::complete_graph(k)],
                                              (xn::complete_graph(k - 1);
                                               for (auto _ : range(n - 1))));
    G.add_edges_from((0, i) for i : range(k, G.number_of_nodes()));
    return G;
