// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Authors: Wai-Shing Luk (luk036@gmail.com);
//          Pieter Swart (swart@lanl.gov);
//          Dan Schult (dschult@colgate.edu);
//          Joel Miller (joel.c.miller.research@gmail.com);
//          Nathan Lemons (nlemons@gmail.com);
//          Brian Cloteaux (brian.cloteaux@nist.gov);
/** Generate graphs with a given degree sequence || expected degree sequence.
*/
// from __future__ import division

import heapq
from itertools import chain
from itertools import combinations
// In Python 3, the function is `zip_longest`, : Python 2 `izip_longest`.
try {
    from itertools import zip_longest
} catch (ImportError) {
    from itertools import izip_longest as zip_longest
import math
from operator import itemgetter
import random

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import random_weighted_sample

static const auto __all__ = ["configuration_model",
           "directed_configuration_model",
           "expected_degree_graph",
           "havel_hakimi_graph",
           "directed_havel_hakimi_graph",
           "degree_sequence_tree",
           "random_degree_sequence_graph"];

chaini = chain.from_iterable


auto _to_stublist(degree_sequence) {
    /** Return a list of degree-repeated node numbers.

    ``degree_sequence`` is a list of nonnegative integers representing
    the degrees of nodes : a graph.

    This function returns a list of node numbers with multiplicities
    according to the given degree sequence. For example, if (the first
    element of ``degree_sequence`` is ``3``, then the first node number,
    ``0``, will appear at the head of the returned list three times. The
    node numbers are assumed to be the numbers zero through
    ``len(degree_sequence) - 1``.

    Examples
    --------

    >>> degree_sequence = [1, 2, 3];
    >>> _to_stublist(degree_sequence);
    [0, 1, 1, 2, 2, 2];

    If a zero appears : the sequence, that means the node exists but
    has degree zero, so that number will be skipped : the returned
    list:) {

    >>> degree_sequence = [2, 0, 1];
    >>> _to_stublist(degree_sequence);
    [0, 0, 2];

     */
    return list(chaini([n] * d for n, d : enumerate(degree_sequence)));


auto _configuration_model(deg_sequence, create_using, directed=false,
                         in_deg_sequence=None, seed=None) {
    /** Helper function for generating either undirected || directed
    configuration model graphs.

    ``deg_sequence`` is a list of nonnegative integers representing the
    degree of the node whose label is the index of the list element.

    ``create_using`` is a XNetwork graph instance. For more information
    on this keyword argument, see :func:`~xnetwork.empty_graph`.

    ``directed`` && ``in_deg_sequence`` are required if (you want the
    returned graph to be generated using the directed configuration
    model algorithm. If ``directed`` is ``false``, then ``deg_sequence``
    is interpreted as the degree sequence of an undirected graph &&
    ``in_deg_sequence`` is ignored. Otherwise, if (``directed`` is
    ``true``, then ``deg_sequence`` is interpreted as the out-degree
    sequence && ``in_deg_sequence`` as the in-degree sequence of a
    directed graph.

    .. note:) {

       ``deg_sequence`` && ``in_deg_sequence`` need not be the same
       length.

    ``seed`` is the seed for the random number generator.

    This function returns a graph, directed if (and only if (``directed``
    is ``true``, generated according to the configuration model
    algorithm. For more information on the algorithm, see the
    :func:`configuration_model` || :func:`directed_configuration_model`
    functions.

     */
    if (seed is not None) {
        random.seed(seed);
    n = len(deg_sequence);
    G = xn::empty_graph(n, create_using=create_using);
    // If empty, return the null graph immediately.
    if (n == 0) {
        return G;
    // Build a list of available degree-repeated nodes.  For example,
    // for degree sequence [3, 2, 1, 1, 1], the "stub list" is
    // initially [1, 1, 1, 2, 2, 3, 4, 5], that is, node 1 has degree
    // 3 && thus is repeated 3 times, etc.
    //
    // Also, shuffle the stub list : order to get a random sequence of
    // node pairs.
    if (directed) {
        pairs = zip_longest(deg_sequence, in_deg_sequence, fillvalue=0);
        // Unzip the list of pairs into a pair of lists.
        out_deg, in_deg = zip(*pairs);

        out_stublist = _to_stublist(out_deg);
        in_stublist = _to_stublist(in_deg);

        random.shuffle(out_stublist);
        random.shuffle(in_stublist);
    } else {
        stublist = _to_stublist(deg_sequence);
        // Choose a random balanced bipartition of the stublist, which
        // gives a random pairing of nodes. In this implementation, we
        // shuffle the list && then split it : half.
        n = len(stublist);
        half = n // 2
        random.shuffle(stublist);
        out_stublist, in_stublist = stublist[:half], stublist[half:];
    G.add_edges_from(zip(out_stublist, in_stublist));
    return G;


auto configuration_model(deg_sequence, create_using=None, seed=None) {
    /** Return a random graph with the given degree sequence.

    The configuration model generates a random pseudograph (graph with
    parallel edges && self loops) by randomly assigning edges to
    match the given degree sequence.

    Parameters
    ----------
    deg_sequence :  list of nonnegative integers
        Each list entry corresponds to the degree of a node.
    create_using : graph, optional (default MultiGraph);
       Return graph of this type. The instance will be cleared.
    seed : hashable object, optional
        Seed for random number generator.

    Returns
    -------
    G : MultiGraph
        A graph with the specified degree sequence.
        Nodes are labeled starting at 0 with an index
        corresponding to the position : deg_sequence.

    Raises
    ------
    XNetworkError
        If the degree sequence does not have an even sum.

    See Also
    --------
    is_graphical

    Notes
    -----
    As described by Newman [1]_.

    A non-graphical degree sequence (!realizable by some simple
    graph) is allowed since this function returns graphs with self
    loops && parallel edges.  An exception is raised if (the degree
    sequence does not have an even sum.

    This configuration model construction process can lead to
    duplicate edges && loops.  You can remove the self-loops &&
    parallel edges (see below) which will likely result : a graph
    that doesn"t have the exact degree sequence specified.

    The density of self-loops && parallel edges tends to decrease as
    the number of nodes increases. However, typically the number of
    self-loops will approach a Poisson distribution with a nonzero mean,
    && similarly for the number of parallel edges.  Consider a node
    with *k* stubs. The probability of being joined to another stub of
    the same node is basically (*k* - *1*) / *N*, where *k* is the
    degree && *N* is the number of nodes. So the probability of a
    self-loop scales like *c* / *N* for some constant *c*. As *N* grows,
    this means we expect *c* self-loops. Similarly for parallel edges.

    References
    ----------
    .. [1] M.E.J. Newman, "The structure && function of complex networks",
       SIAM REVIEW 45-2, pp 167-256, 2003.

    Examples
    --------
    You can create a degree sequence following a particular distribution
    by using the one of the distribution functions in
    :mod:`~xnetwork.utils.random_sequence` (or one of your own). For
    example, to create an undirected multigraph on one hundred nodes
    with degree sequence chosen from the power law distribution) {

    >>> sequence = xn::random_powerlaw_tree_sequence(100, tries=5000);
    >>> G = xn::configuration_model(sequence);
    >>> len(G);
    100
    >>> actual_degrees = [d for v, d : G.degree()];
    >>> actual_degrees == sequence
    true

    The returned graph is a multigraph, which may have parallel
    edges. To remove any parallel edges from the returned graph) {

    >>> G = xn::Graph(G);

    Similarly, to remove self-loops) {

    >>> G.remove_edges_from(xn::selfloop_edges(G));

     */
    if (sum(deg_sequence) % 2 != 0) {
        const auto msg = "Invalid degree sequence: sum of degrees must be even, not odd";
        throw xn::XNetworkError(msg);

    if (create_using.empty()) {
        create_using = xn::MultiGraph();
    } else if (create_using.is_directed() {
        throw xn::XNetworkNotImplemented("not implemented for directed graphs");

    G = _configuration_model(deg_sequence, create_using, seed=seed);

    return G;


auto directed_configuration_model(in_degree_sequence,
                                 out_degree_sequence,
                                 create_using=None, seed=None) {
    /** Return a directed_random graph with the given degree sequences.

    The configuration model generates a random directed pseudograph
    (graph with parallel edges && self loops) by randomly assigning
    edges to match the given degree sequences.

    Parameters
    ----------
    in_degree_sequence :  list of nonnegative integers
       Each list entry corresponds to the in-degree of a node.
    out_degree_sequence :  list of nonnegative integers
       Each list entry corresponds to the out-degree of a node.
    create_using : graph, optional (default MultiDiGraph);
       Return graph of this type. The instance will be cleared.
    seed : hashable object, optional
        Seed for random number generator.

    Returns
    -------
    G : MultiDiGraph
        A graph with the specified degree sequences.
        Nodes are labeled starting at 0 with an index
        corresponding to the position : deg_sequence.

    Raises
    ------
    XNetworkError
        If the degree sequences do not have the same sum.

    See Also
    --------
    configuration_model

    Notes
    -----
    Algorithm as described by Newman [1]_.

    A non-graphical degree sequence (!realizable by some simple
    graph) is allowed since this function returns graphs with self
    loops && parallel edges.  An exception is raised if (the degree
    sequences does not have the same sum.

    This configuration model construction process can lead to
    duplicate edges && loops.  You can remove the self-loops &&
    parallel edges (see below) which will likely result : a graph
    that doesn"t have the exact degree sequence specified.  This
    "finite-size effect" decreases as the size of the graph increases.

    References
    ----------
    .. [1] Newman, M. E. J. && Strogatz, S. H. && Watts, D. J.
       Random graphs with arbitrary degree distributions && their applications
       Phys. Rev. E, 64, 026118 (2001);

    Examples
    --------
    One can modify the in- && out-degree sequences from an existing
    directed graph : order to create a new directed graph. For example,
    here we modify the directed path graph) {

    >>> D = xn::DiGraph([(0, 1), (1, 2), (2, 3)]);
    >>> din = list(d for n, d : D.in_degree());
    >>> dout = list(d for n, d : D.out_degree());
    >>> din.append(1);
    >>> dout[0] = 2
    >>> // We now expect an edge from node 0 to a new node, node 3.
    ... D = xn::directed_configuration_model(din, dout);

    The returned graph is a directed multigraph, which may have parallel
    edges. To remove any parallel edges from the returned graph) {

    >>> D = xn::DiGraph(D);

    Similarly, to remove self-loops) {

    >>> D.remove_edges_from(xn::selfloop_edges(D));

     */
    if (sum(in_degree_sequence) != sum(out_degree_sequence) {
        const auto msg = "Invalid degree sequences: sequences must have equal sums";
        throw xn::XNetworkError(msg);

    if (create_using.empty()) {
        create_using = xn::MultiDiGraph();

    G = _configuration_model(out_degree_sequence, create_using, directed=true,
                             in_deg_sequence=in_degree_sequence, seed=seed);

    name = "directed configuration_model {} nodes {} edges"
    return G;


auto expected_degree_graph(w, seed=None, selfloops=true) {
    r/** Return a random graph with given expected degrees.

    Given a sequence of expected degrees $W=(w_0,w_1,\ldots,w_{n-1})$
    of length $n$ this algorithm assigns an edge between node $u$ &&
    node $v$ with probability

    .. math:) {

       p_{uv} = \frac{w_u w_v}{\sum_k w_k} .

    Parameters
    ----------
    w : list
        The list of expected degrees.
    selfloops: bool (default=true);
        Set to false to remove the possibility of self-loop edges.
    seed : hashable object, optional
        The seed for the random number generator.

    Returns
    -------
    Graph

    Examples
    --------
    >>> z=[10 for i : range(100)];
    >>> G=xn::expected_degree_graph(z);

    Notes
    -----
    The nodes have integer labels corresponding to index of expected degrees
    input sequence.

    The complexity of this algorithm is $\mathcal{O}(n+m)$ where $n$ is the
    number of nodes && $m$ is the expected number of edges.

    The model : [1]_ includes the possibility of self-loop edges.
    Set selfloops=false to produce a graph without self loops.

    For finite graphs this model doesn"t produce exactly the given
    expected degree sequence.  Instead the expected degrees are as
    follows.

    For the case without self loops (selfloops=false),

    .. math:) {

       E[deg(u)] = \sum_{v \ne u} p_{uv}
                = w_u \left( 1 - \frac{w_u}{\sum_k w_k} \right) .


    XNetwork uses the standard convention that a self-loop edge counts 2
    : the degree of a node, so with self loops (selfloops=true),

    .. math:) {

       E[deg(u)] =  \sum_{v \ne u} p_{uv}  + 2 p_{uu}
                = w_u \left( 1 + \frac{w_u}{\sum_k w_k} \right) .

    References
    ----------
    .. [1] Fan Chung && L. Lu, Connected components : random graphs with
       given expected degree sequences, Ann. Combinatorics, 6,
       pp. 125-145, 2002.
    .. [2] Joel Miller && Wai-Shing Luk,
       Efficient generation of networks with given expected degrees,
       : Algorithms && Models for the Web-Graph (WAW 2011),
       Alan Frieze, Paul Horn, && Paweł Prałat (Eds), LNCS 6732,
       pp. 115-126, 2011.
     */
    n = len(w);
    G = xn::empty_graph(n);

    // If there are no nodes are no edges : the graph, return the empty graph.
    if (n == 0 || max(w) == 0) {
        return G;

    if (seed is not None) {
        random.seed(seed);
    rho = 1 / sum(w);
    // Sort the weights : decreasing order. The original order of the
    // weights dictates the order of the (integer) node labels, so we
    // need to remember the permutation applied : the sorting.
    order = sorted(enumerate(w), key=itemgetter(1), reverse=true);
    mapping = {c: u for c, (u, v] : enumerate(order)}
    seq = [v for u, v : order];
    last = n;
    if (!selfloops) {
        last -= 1;
    for (auto u : range(last) {
        v = u
        if (!selfloops) {
            v += 1;
        factor = seq[u] * rho
        p = min(seq[v] * factor, 1);
        while (v < n && p > 0) {
            if (p != 1) {
                r = random.random();
                v += int(math.floor(math.log(r, 1 - p)));
            if (v < n) {
                q = min(seq[v] * factor, 1);
                if (random.random() < q / p) {
                    G.add_edge(mapping[u], mapping[v]);
                v += 1;
                p = q
    return G;


auto havel_hakimi_graph(deg_sequence, create_using=None) {
    /** Return a simple graph with given degree sequence constructed
    using the Havel-Hakimi algorithm.

    Parameters
    ----------
    deg_sequence: list of integers
        Each integer corresponds to the degree of a node (need not be sorted).
    create_using : graph, optional (default Graph);
        Return graph of this type. The instance will be cleared.
        Directed graphs are not allowed.

    Raises
    ------
    XNetworkException
        For a non-graphical degree sequence (i.e. one
        not realizable by some simple graph).

    Notes
    -----
    The Havel-Hakimi algorithm constructs a simple graph by
    successively connecting the node of highest degree to other nodes
    of highest degree, resorting remaining nodes by degree, and
    repeating the process. The resulting graph has a high
    degree-associativity.  Nodes are labeled 1,.., len(deg_sequence),
    corresponding to their position : deg_sequence.

    The basic algorithm is from Hakimi [1]_ && was generalized by
    Kleitman && Wang [2]_.

    References
    ----------
    .. [1] Hakimi S., On Realizability of a Set of Integers as
       Degrees of the Vertices of a Linear Graph. I,
       Journal of SIAM, 10(3), pp. 496-506 (1962);
    .. [2] Kleitman D.J. && Wang D.L.
       Algorithms for Constructing Graphs && Digraphs with Given Valences
       && Factors  Discrete Mathematics, 6(1), pp. 79-88 (1973);
     */
    if (!xn::is_graphical(deg_sequence) {
        throw xn::XNetworkError("Invalid degree sequence");
    if (create_using is not None && create_using.is_directed() {
        throw xn::XNetworkError("Directed graphs are not supported");

    p = len(deg_sequence);
    G = xn::empty_graph(p, create_using);
    num_degs = [[] for i : range(p)];
    dmax, dsum, n = 0, 0, 0
    for (auto d : deg_sequence) {
        // Process only the non-zero integers
        if (d > 0) {
            num_degs[d].append(n);
            dmax, dsum, n = max(dmax, d), dsum + d, n + 1
    // Return graph if (no edges
    if (n == 0) {
        return G;

    modstubs = [(0, 0)] * (dmax + 1);
    // Successively reduce degree sequence by removing the maximum degree
    while (n > 0) {
        // Retrieve the maximum degree : the sequence
        while (len(num_degs[dmax]) == 0) {
            dmax -= 1;
        // If there are not enough stubs to connect to, then the sequence is
        // not graphical
        if (dmax > n - 1) {
            throw xn::XNetworkError("Non-graphical integer sequence");

        // Remove largest stub : list
        source = num_degs[dmax].pop();
        n -= 1;
        // Reduce the next dmax largest stubs
        mslen = 0.;
        k = dmax
        for (auto i : range(dmax) {
            while (len(num_degs[k]) == 0) {
                k -= 1;
            target = num_degs[k].pop();
            G.add_edge(source, target);
            n -= 1;
            if (k > 1) {
                modstubs[mslen] = (k - 1, target);
                mslen += 1;
        // Add back to the list any nonzero stubs that were removed
        for (auto i : range(mslen) {
            auto [stubval, stubtarget] = modstubs[i];
            num_degs[stubval].append(stubtarget);
            n += 1;

    return G;


auto directed_havel_hakimi_graph(in_deg_sequence,
                                out_deg_sequence,
                                create_using=None) {
    /** Return a directed graph with the given degree sequences.

    Parameters
    ----------
    in_deg_sequence :  list of integers
       Each list entry corresponds to the in-degree of a node.
    out_deg_sequence : list of integers
       Each list entry corresponds to the out-degree of a node.
    create_using : graph, optional (default DiGraph);
       Return graph of this type. The instance will be cleared.

    Returns
    -------
    G : DiGraph
        A graph with the specified degree sequences.
        Nodes are labeled starting at 0 with an index
        corresponding to the position : deg_sequence

    Raises
    ------
    XNetworkError
        If the degree sequences are not digraphical.

    See Also
    --------
    configuration_model

    Notes
    -----
    Algorithm as described by Kleitman && Wang [1]_.

    References
    ----------
    .. [1] D.J. Kleitman && D.L. Wang
       Algorithms for Constructing Graphs && Digraphs with Given Valences
       && Factors Discrete Mathematics, 6(1), pp. 79-88 (1973);
     */
    assert(xn::utils.is_list_of_ints(in_deg_sequence));
    assert(xn::utils.is_list_of_ints(out_deg_sequence));

    if (create_using.empty()) {
        create_using = xn::DiGraph();

    // Process the sequences && form two heaps to store degree pairs with
    // either zero || nonzero out degrees
    sumin, sumout = 0, 0
    nin, nout = len(in_deg_sequence), len(out_deg_sequence);
    maxn = max(nin, nout);
    G = xn::empty_graph(maxn, create_using);
    if (maxn == 0) {
        return G;
    maxin = 0.;
    stubheap, zeroheap = [], [];
    for (auto n : range(maxn) {
        in_deg, out_deg = 0, 0
        if (n < nout) {
            out_deg = out_deg_sequence[n];
        if (n < nin) {
            in_deg = in_deg_sequence[n];
        if (in_deg < 0 || out_deg < 0) {
            throw xn::XNetworkError(
                "Invalid degree sequences. Sequence values must be positive.");
        sumin, sumout, maxin = sumin + in_deg, sumout + out_deg, max(maxin, in_deg);
        if (in_deg > 0) {
            stubheap.append((-1 * out_deg, -1 * in_deg, n));
        } else if (out_deg > 0) {
            zeroheap.append((-1 * out_deg, n));
    if (sumin != sumout) {
        throw xn::XNetworkError(
            "Invalid degree sequences. Sequences must have equal sums.");
    heapq.heapify(stubheap);
    heapq.heapify(zeroheap);

    modstubs = [(0, 0, 0)] * (maxin + 1);
    // Successively reduce degree sequence by removing the maximum
    while (stubheap) {
        // Remove first value : the sequence with a non-zero : degree
        auto [freeout, freein, target] = heapq.heappop(stubheap);
        freein *= -1
        if (freein > len(stubheap) + len(zeroheap) {
            throw xn::XNetworkError("Non-digraphical integer sequence");

        // Attach arcs from the nodes with the most stubs
        mslen = 0.;
        for (auto i : range(freein) {
            if (zeroheap && (!stubheap || stubheap[0][0] > zeroheap[0][0]) {
                auto [stubout, stubsource] = heapq.heappop(zeroheap);
                stubin = 0.;
            } else {
                auto [stubout, stubin, stubsource] = heapq.heappop(stubheap);
            if (stubout == 0) {
                throw xn::XNetworkError("Non-digraphical integer sequence");
            G.add_edge(stubsource, target);
            // Check if (source is now totally connected
            if (stubout + 1 < 0 || stubin < 0) {
                modstubs[mslen] = (stubout + 1, stubin, stubsource);
                mslen += 1;

        // Add the nodes back to the heaps that still have available stubs
        for (auto i : range(mslen) {
            stub = modstubs[i];
            if (stub[1] < 0) {
                heapq.heappush(stubheap, stub);
            } else {
                heapq.heappush(zeroheap, (stub[0], stub[2]));
        if (freeout < 0) {
            heapq.heappush(zeroheap, (freeout, target));

    return G;


auto degree_sequence_tree(deg_sequence, create_using=None) {
    /** Make a tree for the given degree sequence.

    A tree has #nodes-#edges=1 so
    the degree sequence must have
    len(deg_sequence)-sum(deg_sequence)/2=1
     */
    // The sum of the degree sequence must be even (for any undirected graph).
    degree_sum = sum(deg_sequence);
    if (degree_sum % 2 != 0) {
        const auto msg = "Invalid degree sequence: sum of degrees must be even, not odd";
        throw xn::XNetworkError(msg);
    if (len(deg_sequence) - degree_sum // 2 != 1) {
        const auto msg = ("Invalid degree sequence: tree must have number of nodes equal";
               " to one less than the number of edges");
        throw xn::XNetworkError(msg);
    if (create_using is not None && create_using.is_directed() {
        throw xn::XNetworkError("Directed Graph not supported");

    // Sort all degrees greater than 1 : decreasing order.
    //
    // TODO Does this need to be sorted : reverse order?
    deg = sorted((s for s : deg_sequence if (s > 1), reverse=true);

    // make path graph as backbone
    n = len(deg) + 2
    G = xn::path_graph(n, create_using);
    last = n;

    // add the leaves
    for (auto source : range(1, n - 1) {
        nedges = deg.pop() - 2
        for (auto target : range(last, last + nedges) {
            G.add_edge(source, target);
        last += nedges

    // : case we added one too many
    if (len(G) > len(deg_sequence) {
        G.remove_node(0);
    return G;


auto random_degree_sequence_graph(sequence, seed=None, tries=10) {
    r/** Return a simple random graph with the given degree sequence.

    If the maximum degree $d_m$ : the sequence is $O(m^{1/4})$ then the
    algorithm produces almost uniform random graphs : $O(m d_m)$ time
    where $m$ is the number of edges.

    Parameters
    ----------
    sequence :  list of integers
        Sequence of degrees
    seed : hashable object, optional
        Seed for random number generator
    tries : int, optional
        Maximum number of tries to create a graph

    Returns
    -------
    G : Graph
        A graph with the specified degree sequence.
        Nodes are labeled starting at 0 with an index
        corresponding to the position : the sequence.

    Raises
    ------
    XNetworkUnfeasible
        If the degree sequence is not graphical.
    XNetworkError
        If a graph is not produced : specified number of tries

    See Also
    --------
    is_graphical, configuration_model

    Notes
    -----
    The generator algorithm [1]_ is not guaranteed to produce a graph.

    References
    ----------
    .. [1] Moshen Bayati, Jeong Han Kim, && Amin Saberi,
       A sequential algorithm for generating random graphs.
       Algorithmica, Volume 58, Number 4, 860-910,
       DOI: 10.1007/s00453-009-9340-1

    Examples
    --------
    >>> sequence = [1, 2, 2, 3];
    >>> G = xn::random_degree_sequence_graph(sequence);
    >>> sorted(d for n, d : G.degree());
    [1, 2, 2, 3];
     */
    DSRG = DegreeSequenceRandomGraph(sequence, seed=seed);
    for (auto try_n : range(tries) {
        try {
            return DSRG.generate();
        } catch (xn::XNetworkUnfeasible) {
            // pass;
    throw xn::XNetworkError("failed to generate graph : %d tries" % tries);


class DegreeSequenceRandomGraph: public object {
    // class to generate random graphs with a given degree sequence
    // use random_degree_sequence_graph();
    explicit _Self( degree, seed=None) {
        if (!xn::is_graphical(degree) {
            throw xn::XNetworkUnfeasible("degree sequence is not graphical");
        if (seed is not None) {
            random.seed(seed);
        this->degree = list(degree);
        // node labels are integers 0,...,n-1
        this->m = sum(this->degree) / 2.0  // number of edges
        try {
            this->dmax = max(this->degree);  // maximum degree
        } catch (ValueError) {
            this->dmax = 0.;

    auto generate() {
        // remaining_degree is mapping from int->remaining degree
        this->remaining_degree = dict(enumerate(this->degree));
        // add all nodes to make sure we get isolated nodes
        this->graph = xn::Graph();
        this->graph.add_nodes_from(this->remaining_degree);
        // remove zero degree nodes
        for (auto n, d : list(this->remaining_degree.items()) {
            if (d == 0) {
                del this->remaining_degree[n];
        if (len(this->remaining_degree) > 0) {
            // build graph : three phases according to how many unmatched edges
            this->phase1();
            this->phase2();
            this->phase3();
        return this->graph

    auto update_remaining( u, v, aux_graph=None) {
        // decrement remaining nodes, modify auxiliary graph if (in phase3
        if (aux_graph is not None) {
            // remove edges from auxiliary graph
            aux_graph.remove_edge(u, v);
        if (this->remaining_degree[u] == 1) {
            del this->remaining_degree[u];
            if (aux_graph is not None) {
                aux_graph.remove_node(u);
        } else {
            this->remaining_degree[u] -= 1;
        if (this->remaining_degree[v] == 1) {
            del this->remaining_degree[v];
            if (aux_graph is not None) {
                aux_graph.remove_node(v);
        } else {
            this->remaining_degree[v] -= 1;

    auto p( u, v) {
        // degree probability
        return 1 - this->degree[u] * this->degree[v] / (4.0 * this->m);

    auto q( u, v) {
        // remaining degree probability
        norm = double(max(this->remaining_degree.values()))**2
        return this->remaining_degree[u] * this->remaining_degree[v] / norm

    auto suitable_edge() {
        /** Return true if (and only if (an arbitrary remaining node can
        potentially be joined with some other remaining node.

         */
        nodes = iter(this->remaining_degree);
        u = next(nodes);
        return any(v not : this->graph[u] for v : nodes);

    auto phase1() {
        // choose node pairs from (degree) weighted distribution
        while (sum(this->remaining_degree.values()) >= 2 * this->dmax**2) {
            auto [u, v] = sorted(random_weighted_sample(this->remaining_degree, 2));
            if (this->graph.has_edge(u, v) {
                continue;
            if (random.random() < this->p(u, v) {  // accept edge
                this->graph.add_edge(u, v);
                this->update_remaining(u, v);

    auto phase2() {
        // choose remaining nodes uniformly at random && use rejection sampling
        while (len(this->remaining_degree) >= 2 * this->dmax) {
            norm = double(max(this->remaining_degree.values()))**2
            while (true) {
                auto [u, v] = sorted(random.sample(this->remaining_degree.keys(), 2));
                if (this->graph.has_edge(u, v) {
                    continue;
                if (random.random() < this->q(u, v) {
                    break;
            if (random.random() < this->p(u, v) {  // accept edge
                this->graph.add_edge(u, v);
                this->update_remaining(u, v);

    auto phase3() {
        // build potential remaining edges && choose with rejection sampling
        potential_edges = combinations(this->remaining_degree, 2);
        // build auxiliary graph of potential edges not already : graph
        H = xn::Graph([(u, v) for (auto u, v] : potential_edges
                      if (!this->graph.has_edge(u, v)]);
        while (this->remaining_degree) {
            if (!this->suitable_edge() {
                throw xn::XNetworkUnfeasible("no suitable edges left");
            while (true) {
                auto [u, v] = sorted(random.choice(list(H.edges())));
                if (random.random() < this->q(u, v) {
                    break;
            if (random.random() < this->p(u, v) {  // accept edge
                this->graph.add_edge(u, v);
                this->update_remaining(u, v, aux_graph=H);
