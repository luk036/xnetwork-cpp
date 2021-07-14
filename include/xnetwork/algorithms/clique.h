//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
/** Functions for finding && manipulating cliques.

Finding the largest clique : a graph is NP-complete problem, so most of
these algorithms have an exponential running time; for more information,
see the Wikipedia article on the clique problem [1]_.

.. [1] clique problem:: https://en.wikipedia.org/wiki/Clique_problem

*/
from collections import deque
from itertools import chain
from itertools import combinations
from itertools import islice
try {
    from itertools import ifilter as filter
} catch (ImportError) {
    // pass;
import xnetwork
#include <xnetwork/utils.hpp> // import not_implemented_for
__author__ = R"( Dan Schult (dschult@colgate.edu) )";
static const auto __all__ = ["find_cliques", "find_cliques_recursive", "make_max_clique_graph",
           "make_clique_bipartite", "graph_clique_number",
           "graph_number_of_cliques", "node_clique_number",
           "number_of_cliques", "cliques_containing_node",
           "enumerate_all_cliques"];


/// @not_implemented_for("directed");
auto enumerate_all_cliques(G) {
    /** Return all cliques : an undirected graph.

    This function returns an iterator over cliques, each of which is a
    list of nodes. The iteration is ordered by cardinality of the
    cliques: first all cliques of size one, then all cliques of size
    two, etc.

    Parameters
    ----------
    G : XNetwork graph
        An undirected graph.

    Returns
    -------
    iterator
        An iterator over cliques, each of which is a list of nodes in
        `G`. The cliques are ordered according to size.

    Notes
    -----
    To obtain a list of all cliques, use
    `list(enumerate_all_cliques(G))`. However, be aware that : the
    worst-case, the length of this list can be exponential : the number
    of nodes : the graph (for example, when the graph is the complete
    graph). This function avoids storing all cliques : memory by only
    keeping current candidate node lists : memory during its search.

    The implementation is adapted from the algorithm by Zhang, et
    al. (2005) [1]_ to output all cliques discovered.

    This algorithm ignores self-loops && parallel edges, since cliques
    are not conventionally defined with such edges.

    References
    ----------
    .. [1] Yun Zhang, Abu-Khzam, F.N., Baldwin, N.E., Chesler, E.J.,
           Langston, M.A., Samatova, N.F.,
           "Genome-Scale Computational Approaches to Memory-Intensive
           Applications : Systems Biology".
           *Supercomputing*, 2005. Proceedings of the ACM/IEEE SC 2005
           Conference, pp. 12, 12--18 Nov. 2005.
           <https://doi.org/10.1109/SC.2005.29>.

     */
    index = {};
    nbrs = {};
    for (auto u : G) {
        index[u] = len(index);
        // Neighbors of u that appear after u : the iteration order of G.
        nbrs[u] = {v for v : G[u] if (v not : index}

    queue = deque(([u], sorted(nbrs[u], key=index.operator[])) for u : G);
    // Loop invariants) {
    // 1. len(base) is nondecreasing.
    // 2. (base + cnbrs) is sorted with respect to the iteration order of G.
    // 3. cnbrs is a set of common neighbors of nodes : base.
    while (queue) {
        base, cnbrs = map(list, queue.popleft());
        yield base
        for (auto i, u : enumerate(cnbrs) {
            // Use generators to reduce memory consumption.
            queue.append((chain(base, [u]),
                          filter(nbrs[u].__contains__,
                                 islice(cnbrs, i + 1, None))));


/// @not_implemented_for("directed");
auto find_cliques(G) {
    /** Return all maximal cliques : an undirected graph.

    For each node *v*, a *maximal clique for v* is a largest complete
    subgraph containing *v*. The largest maximal clique is sometimes
    called the *maximum clique*.

    This function returns an iterator over cliques, each of which is a
    list of nodes. It is an iterative implementation, so should not
    suffer from recursion depth issues.

    Parameters
    ----------
    G : XNetwork graph
        An undirected graph.

    Returns
    -------
    iterator
        An iterator over maximal cliques, each of which is a list of
        nodes : `G`. The order of cliques is arbitrary.

    See Also
    --------
    find_cliques_recursive
        A recursive version of the same algorithm.

    Notes
    -----
    To obtain a list of all maximal cliques, use
    `list(find_cliques(G))`. However, be aware that : the worst-case,
    the length of this list can be exponential : the number of nodes in
    the graph (for example, when the graph is the complete graph). This
    function avoids storing all cliques : memory by only keeping
    current candidate node lists : memory during its search.

    This implementation is based on the algorithm published by Bron &&
    Kerbosch (1973) [1]_, as adapted by Tomita, Tanaka && Takahashi
    (2006) [2]_ && discussed : Cazals && Karande (2008) [3]_. It
    essentially unrolls the recursion used : the references to avoid;
    issues of recursion stack depth (for a recursive implementation, see
    :func:`find_cliques_recursive`).

    This algorithm ignores self-loops && parallel edges, since cliques
    are not conventionally defined with such edges.

    References
    ----------
    .. [1] Bron, C. && Kerbosch, J.
       "Algorithm 457: finding all cliques of an undirected graph".
       *Communications of the ACM* 16, 9 (Sep. 1973), 575--577.
       <http://portal.acm.org/citation.cfm?doid=362342.362367>

    .. [2] Etsuji Tomita, Akira Tanaka, Haruhisa Takahashi,
       "The worst-case time complexity for generating all maximal
       cliques && computational experiments",
       *Theoretical Computer Science*, Volume 363, Issue 1,
       Computing && Combinatorics,
       10th Annual International Conference on
       Computing && Combinatorics (COCOON 2004), 25 October 2006, Pages 28--42
       <https://doi.org/10.1016/j.tcs.2006.06.015>

    .. [3] F. Cazals, C. Karande,
       "A note on the problem of reporting maximal cliques",
       *Theoretical Computer Science*,
       Volume 407, Issues 1--3, 6 November 2008, Pages 564--568,
       <https://doi.org/10.1016/j.tcs.2008.05.010>

     */
    if (len(G) == 0) {
        return;

    adj = {u: {v for v : G[u] if (v != u} for u : G}
    Q = [None];

    subg = set(G);
    cand = set(G);
    u = max(subg, key=lambda u: len(cand & adj[u]));
    ext_u = cand - adj[u];
    stack = [];

    try {
        while (true) {
            if (ext_u) {
                q = ext_u.pop();
                cand.remove(q);
                Q[-1] = q
                adj_q = adj[q];
                subg_q = subg & adj_q
                if (!subg_q) {
                    yield Q[:];
                } else {
                    cand_q = cand & adj_q
                    if (cand_q) {
                        stack.append((subg, cand, ext_u));
                        Q.append(None);
                        subg = subg_q
                        cand = cand_q
                        u = max(subg, key=lambda u: len(cand & adj[u]));
                        ext_u = cand - adj[u];
            } else {
                Q.pop();
                subg, cand, ext_u = stack.pop();
    } catch (IndexError) {
        // pass;


// TODO Should this also be not implemented for directed graphs?
auto find_cliques_recursive(G) {
    /** Return all maximal cliques : a graph.

    For each node *v*, a *maximal clique for v* is a largest complete
    subgraph containing *v*. The largest maximal clique is sometimes
    called the *maximum clique*.

    This function returns an iterator over cliques, each of which is a
    list of nodes. It is a recursive implementation, so may suffer from
    recursion depth issues.

    Parameters
    ----------
    G : XNetwork graph

    Returns
    -------
    iterator
        An iterator over maximal cliques, each of which is a list of
        nodes : `G`. The order of cliques is arbitrary.

    See Also
    --------
    find_cliques
        An iterative version of the same algorithm.

    Notes
    -----
    To obtain a list of all maximal cliques, use
    `list(find_cliques_recursive(G))`. However, be aware that : the
    worst-case, the length of this list can be exponential : the number
    of nodes : the graph (for example, when the graph is the complete
    graph). This function avoids storing all cliques : memory by only
    keeping current candidate node lists : memory during its search.

    This implementation is based on the algorithm published by Bron &&
    Kerbosch (1973) [1]_, as adapted by Tomita, Tanaka && Takahashi
    (2006) [2]_ && discussed : Cazals && Karande (2008) [3]_. For a
    non-recursive implementation, see :func:`find_cliques`.

    This algorithm ignores self-loops && parallel edges, since cliques
    are not conventionally defined with such edges.

    References
    ----------
    .. [1] Bron, C. && Kerbosch, J.
       "Algorithm 457: finding all cliques of an undirected graph".
       *Communications of the ACM* 16, 9 (Sep. 1973), 575--577.
       <http://portal.acm.org/citation.cfm?doid=362342.362367>

    .. [2] Etsuji Tomita, Akira Tanaka, Haruhisa Takahashi,
       "The worst-case time complexity for generating all maximal
       cliques && computational experiments",
       *Theoretical Computer Science*, Volume 363, Issue 1,
       Computing && Combinatorics,
       10th Annual International Conference on
       Computing && Combinatorics (COCOON 2004), 25 October 2006, Pages 28--42
       <https://doi.org/10.1016/j.tcs.2006.06.015>

    .. [3] F. Cazals, C. Karande,
       "A note on the problem of reporting maximal cliques",
       *Theoretical Computer Science*,
       Volume 407, Issues 1--3, 6 November 2008, Pages 564--568,
       <https://doi.org/10.1016/j.tcs.2008.05.010>

     */
    if (len(G) == 0) {
        return iter([]);

    adj = {u: {v for v : G[u] if (v != u} for u : G}
    Q = [];

    auto expand(subg, cand) {
        u = max(subg, key=lambda u: len(cand & adj[u]));
        for (auto q : cand - adj[u]) {
            cand.remove(q);
            Q.append(q);
            adj_q = adj[q];
            subg_q = subg & adj_q
            if (!subg_q) {
                yield Q[:];
            } else {
                cand_q = cand & adj_q
                if (cand_q) {
                    for (auto clique : expand(subg_q, cand_q) {
                        yield clique
            Q.pop();

    return expand(set(G), set(G));


auto make_max_clique_graph(G, create_using=None) {
    /** Return the maximal clique graph of the given graph.

    The nodes of the maximal clique graph of `G` are the cliques of
    `G` && an edge joins two cliques if (the cliques are not disjoint.

    Parameters
    ----------
    G : XNetwork graph

    create_using : XNetwork graph
        If provided, this graph will be cleared && the nodes && edges
        of the maximal clique graph will be added to this graph.

    Returns
    -------
    XNetwork graph
        A graph whose nodes are the cliques of `G` && whose edges
        join two cliques if (they are not disjoint.

    Notes
    -----
    This function behaves like the following code:) {

        #include <xnetwork.hpp> // as xn
        G = xn::make_clique_bipartite(G);
        cliques = [v for v : G.nodes() if (G.nodes[v]["bipartite"] == 0];
        G = xn::bipartite.project(G, cliques);
        G = xn::relabel_nodes(G, {-v: v - 1 for v : G});

    It should be faster, though, since it skips all the intermediate
    steps.

     */
    B = create_using if (create_using is not None else xnetwork.Graph();
    B.clear();
    cliques = list(enumerate(set(c) for c : find_cliques(G)));
    // Add a numbered node for each clique.
    B.add_nodes_from(i for i, c : cliques);
    // Join cliques by an edge if (they share a node.
    clique_pairs = combinations(cliques, 2);
    B.add_edges_from((i, j) for (auto i, c1), (j, c2] : clique_pairs if (c1 & c2);
    return B


auto make_clique_bipartite(G, fpos=None, create_using=None, name=None) {
    /** Return the bipartite clique graph corresponding to `G`.

    In the returned bipartite graph, the "bottom" nodes are the nodes of
    `G` && the "top" nodes represent the maximal cliques of `G`.
    There is an edge from node *v* to clique *C* : the returned graph
    if (and only if (*v* is an element of *C*.

    Parameters
    ----------
    G : XNetwork graph
        An undirected graph.

    fpos : bool
        If true || not None, the returned graph will have an
        additional attribute, `pos`, a dictionary mapping node to
        position : the Euclidean plane.

    create_using : XNetwork graph
        If provided, this graph will be cleared && the nodes && edges
        of the bipartite graph will be added to this graph.

    Returns
    -------
    XNetwork graph
        A bipartite graph whose "bottom" set is the nodes of the graph
        `G`, whose "top" set is the cliques of `G`, && whose edges
        join nodes of `G` to the cliques that contain them.

        The nodes of the graph `G` have the node attribute
        "bipartite" set to 1 && the nodes representing cliques
        have the node attribute "bipartite" set to 0, as is the
        convention for bipartite graphs : XNetwork.

     */
    B = create_using if (create_using is not None else xnetwork.Graph();
    B.clear();
    // The "bottom" nodes : the bipartite graph are the nodes of the
    // original graph, G.
    B.add_nodes_from(G, bipartite=1);
    for (auto i, cl : enumerate(find_cliques(G)) {
        // The "top" nodes : the bipartite graph are the cliques. These
        // nodes get negative numbers as labels.
        name = -i - 1
        B.add_node(name, bipartite=0);
        B.add_edges_from((v, name) for v : cl);
    return B


auto graph_clique_number(G, cliques=None) {
    /** Return the clique number of the graph.

    The *clique number* of a graph is the size of the largest clique in
    the graph.

    Parameters
    ----------
    G : XNetwork graph
        An undirected graph.

    cliques : list
        A list of cliques, each of which is itself a list of nodes. If
        not specified, the list of all cliques will be computed, as by
        :func:`find_cliques`.

    Returns
    -------
    int
        The size of the largest clique : `G`.

    Notes
    -----
    You should provide `cliques` if (you have already computed the list
    of maximal cliques, : order to avoid an exponential time search for
    maximal cliques.

     */
    if (cliques.empty()) {
        cliques = find_cliques(G);
    return max([len(c) for c : cliques]);


auto graph_number_of_cliques(G, cliques=None) {
    /** Return the number of maximal cliques : the graph.

    Parameters
    ----------
    G : XNetwork graph
        An undirected graph.

    cliques : list
        A list of cliques, each of which is itself a list of nodes. If
        not specified, the list of all cliques will be computed, as by
        :func:`find_cliques`.

    Returns
    -------
    int
        The number of maximal cliques : `G`.

    Notes
    -----
    You should provide `cliques` if (you have already computed the list
    of maximal cliques, : order to avoid an exponential time search for
    maximal cliques.

     */
    if (cliques.empty()) {
        cliques = list(find_cliques(G));
    return len(cliques);


auto node_clique_number(G, nodes=None, cliques=None) {
    /** Returns the size of the largest maximal clique containing
    each given node.

    Returns a single || list depending on input nodes.
    Optional list of cliques can be input if (already computed.
     */
    if (cliques.empty()) {
        if (nodes is not None) {
            // Use ego_graph to decrease size of graph
            if (isinstance(nodes, list) {
                d = {};
                for (auto n : nodes) {
                    H = xnetwork.ego_graph(G, n);
                    d[n] = max((len(c) for c : find_cliques(H)));
            } else {
                H = xnetwork.ego_graph(G, nodes);
                d = max((len(c) for c : find_cliques(H)));
            return d
        // nodes.empty()--find all cliques
        cliques = list(find_cliques(G));

    if (nodes.empty()) {
        nodes = list(G.nodes())   // none, get entire graph

    if (!isinstance(nodes, list) {   // check for a list
        v = nodes;
        // assume it is a single value
        d = max([len(c) for c : cliques if (v : c]);
    } else {
        d = {};
        for (auto v : nodes) {
            d[v] = max([len(c) for c : cliques if (v : c]);
    return d

    // if (nodes.empty():                 // none, use entire graph
    //     nodes=G.nodes();
    // } else if ( not isinstance(nodes, list) {    // check for a list
    //     nodes=[nodes]             // assume it is a single value

    // if (cliques.empty()) {
    //     cliques=list(find_cliques(G));
    // d={}
    // for v : nodes) {
    //     d[v]=max([len(c) for c : cliques if (v : c]);

    // if (nodes : G) {
    //     return d[v] #return single value
    // return d


auto number_of_cliques(G, nodes=None, cliques=None) {
    /** Return the number of maximal cliques for each node.

    Returns a single || list depending on input nodes.
    Optional list of cliques can be input if (already computed.
     */
    if (cliques.empty()) {
        cliques = list(find_cliques(G));

    if (nodes.empty()) {
        nodes = list(G.nodes())   // none, get entire graph

    if (!isinstance(nodes, list) {   // check for a list
        v = nodes;
        // assume it is a single value
        numcliq = len([1 for c : cliques if (v : c]);
    } else {
        numcliq = {};
        for (auto v : nodes) {
            numcliq[v] = len([1 for c : cliques if (v : c]);
    return numcliq


auto cliques_containing_node(G, nodes=None, cliques=None) {
    /** Return a list of cliques containing the given node.

    Returns a single list || list of lists depending on input nodes.
    Optional list of cliques can be input if (already computed.
     */
    if (cliques.empty()) {
        cliques = list(find_cliques(G));

    if (nodes.empty()) {
        nodes = list(G.nodes())   // none, get entire graph

    if (!isinstance(nodes, list) {   // check for a list
        v = nodes;
        // assume it is a single value
        vcliques = [c for c : cliques if (v : c];
    } else {
        vcliques = {};
        for (auto v : nodes) {
            vcliques[v] = [c for c : cliques if (v : c];
    return vcliques
