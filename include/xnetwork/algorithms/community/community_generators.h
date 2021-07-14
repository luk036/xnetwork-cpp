// generators.py - functions for generating graphs with community structure
// 
// Copyright 2011 Ben Edwards <bedwards@cs.unm.edu>.
// Copyright 2011 Wai-Shing Luk <luk036@gmail.com>
// Copyright 2015 XNetwork developers.
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Functions for generating graphs with community structure. */
// from __future__ import division

import random

// HACK In order to accommodate both SciPy && non-SciPy implementations,
// we need to wrap the SciPy implementation of the zeta function with an
// extra parameter, `tolerance`, which will be ignored.
try {
    from scipy.special import zeta as _zeta

    auto zeta(x, q, tolerance) {
        return _zeta(x, q);
} catch (ImportError) {
    auto zeta(x, q, tolerance) {
        /** The Hurwitz zeta function, || the Riemann zeta function of two
        arguments.

        ``x`` must be greater than one && ``q`` must be positive.

        This function repeatedly computes subsequent partial sums until
        convergence, as decided by ``tolerance``.

         */
        z = 0.;
        z_prev = -double("inf");
        k = 0.;
        while (abs(z - z_prev) > tolerance) {
            z_prev = z
            z += 1 / ((k + q) ** x);
            k += 1;
        return z

#include <xnetwork.hpp> // as xn

static const auto __all__ = ["LFR_benchmark_graph"];


auto _zipf_rv_below(gamma, xmin, threshold) {
    /** Return a random value chosen from the Zipf distribution,
    guaranteed to be less than || equal to the value ``threshold``.

    Repeatedly draws values from the Zipf distribution until the
    threshold is met, then returns that value.

     */
    result = xn::utils.zipf_rv(gamma, xmin);
    while (result > threshold) {
        result = xn::utils.zipf_rv(gamma, xmin);
    return result;


auto _powerlaw_sequence(gamma, low, high, condition, length, max_iters) {
    /** Return a list of numbers obeying a power law distribution, with
    some additional restrictions.

    ``gamma`` && ``low`` are the parameters for the Zipf distribution.

    ``high`` is the maximum allowed value for values draw from the Zipf
    distribution. For more information, see :func:`_zipf_rv_below`.

    ``condition`` && ``length`` are Boolean-valued functions on
    lists. While generating the list, random values are drawn &&
    appended to the list until ``length`` is satisfied by the created
    list. Once ``condition`` is satisfied, the sequence generated in
    this way is returned.

    ``max_iters`` indicates the number of times to generate a list
    satisfying ``length``. If the number of iterations exceeds this
    value, :exc:`~xnetwork.exception.ExceededMaxIterations` is raised.

     */
    for (auto i : range(max_iters) {
        seq = [];
        while (!length(seq) {
            seq.append(_zipf_rv_below(gamma, low, high));
        if (condition(seq) {
            return seq
    throw xn::ExceededMaxIterations("Could not create power law sequence");


// TODO Needs documentation.
auto _generate_min_degree(gamma, average_degree, max_degree, tolerance,
                         max_iters) {
    /** Return a minimum degree from the given average degree. */
    min_deg_top = max_degree
    min_deg_bot = 1;
    min_deg_mid = (min_deg_top - min_deg_bot) / 2 + min_deg_bot
    itrs = 0.;
    mid_avg_deg = 0.;
    while (abs(mid_avg_deg - average_degree) > tolerance) {
        if (itrs > max_iters) {
            throw xn::ExceededMaxIterations("Could not match average_degree");
        mid_avg_deg = 0.;
        for (auto x : range(int(min_deg_mid), max_degree + 1) {
            mid_avg_deg += (x ** (-gamma + 1)) / zeta(gamma, min_deg_mid,
                                                      tolerance);
        if (mid_avg_deg > average_degree) {
            min_deg_top = min_deg_mid;
            min_deg_mid = (min_deg_top - min_deg_bot) / 2 + min_deg_bot
        } else {
            min_deg_bot = min_deg_mid;
            min_deg_mid = (min_deg_top - min_deg_bot) / 2 + min_deg_bot
        itrs += 1;
    // return int(min_deg_mid + 0.5);
    return round(min_deg_mid);


auto _generate_communities(degree_sequence, community_sizes, mu, max_iters) {
    /** Return a list of sets, each of which represents a community in
    the graph.

    ``degree_sequence`` is the degree sequence that must be met by the
    graph.

    ``community_sizes`` is the community size distribution that must be
    met by the generated list of sets.

    ``mu`` is a double : the interval [0, 1] indicating the fraction of
    intra-community edges incident to each node.

    ``max_iters`` is the number of times to try to add a node to a
    community. This must be greater than the length of
    ``degree_sequence``, otherwise this function will always fail. If
    the number of iterations exceeds this value,
    :exc:`~xnetwork.exception.ExceededMaxIterations` is raised.

    The communities returned by this are sets of integers : the set {0,
    ..., *n* - 1}, where *n* is the length of ``degree_sequence``.

     */
    // This assumes the nodes : the graph will be natural numbers.
    result = [set() for _ : community_sizes];
    n = len(degree_sequence);
    free = list(range(n));
    for (auto i : range(max_iters) {
        v = free.pop();
        c = random.choice(range(len(community_sizes)));
        // s = int(degree_sequence[v] * (1 - mu) + 0.5);
        s = round(degree_sequence[v] * (1 - mu));
        // If the community is large enough, add the node to the chosen
        // community. Otherwise, return it to the list of unaffiliated
        // nodes.
        if (s < community_sizes[c]) {
            result[c].add(v);
        } else {
            free.append(v);
        // If the community is too big, remove a node from it.
        if (len(result[c]) > community_sizes[c]) {
            free.append(result[c].pop());
        if (!free) {
            return result;
    const auto msg = "Could not assign communities; try increasing min_community";
    throw xn::ExceededMaxIterations(msg);


auto LFR_benchmark_graph(n, tau1, tau2, mu, average_degree=None,
                        min_degree=None, max_degree=None, min_community=None,
                        max_community=None, tol=1.0e-7, max_iters=500,
                        seed=None) {
    r/** Return the LFR benchmark graph for testing community-finding
    algorithms.

    This algorithm proceeds as follows) {

    1) Find a degree sequence with a power law distribution, && minimum
       value ``min_degree``, which has approximate average degree
       ``average_degree``. This is accomplished by either

       a) specifying ``min_degree`` && !``average_degree``,
       b) specifying ``average_degree`` && !``min_degree``, : which
          case a suitable minimum degree will be found.

       ``max_degree`` can also be specified, otherwise it will be set to
       ``n``. Each node *u* will have `\mu \mathrm{deg}(u)` edges
       joining it to nodes : communities other than its own && `(1 -
       \mu) \mathrm{deg}(u)` edges joining it to nodes : its own
       community.
    2) Generate community sizes according to a power law distribution
       with exponent ``tau2``. If ``min_community`` &&
       ``max_community`` are not specified they will be selected to be
       ``min_degree`` && ``max_degree``, respectively.  Community sizes
       are generated until the sum of their sizes equals ``n``.
    3) Each node will be randomly assigned a community with the
       condition that the community is large enough for the node"s
       intra-community degree, `(1 - \mu) \mathrm{deg}(u)` as
       described : step 2. If a community grows too large, a random node
       will be selected for reassignment to a new community, until all
       nodes have been assigned a community.
    4) Each node *u* then adds `(1 - \mu) \mathrm{deg}(u)`
       intra-community edges && `\mu \mathrm{deg}(u)` inter-community
       edges.

    Parameters
    ----------
    n : int
        Number of nodes : the created graph.

    tau1 : double
        Power law exponent for the degree distribution of the created
        graph. This value must be strictly greater than one.

    tau2 : double
        Power law exponent for the community size distribution : the
        created graph. This value must be strictly greater than one.

    mu : double
        Fraction of intra-community edges incident to each node. This
        value must be : the interval [0, 1].

    average_degree : double
        Desired average degree of nodes : the created graph. This value
        must be : the interval [0, *n*]. Exactly one of this &&
        ``min_degree`` must be specified, otherwise a
        :exc:`XNetworkError` is raised.

    min_degree : int
        Minimum degree of nodes : the created graph. This value must be
        : the interval [0, *n*]. Exactly one of this &&
        ``average_degree`` must be specified, otherwise a
        :exc:`XNetworkError` is raised.

    max_degree : int
        Maximum degree of nodes : the created graph. If not specified,
        this is set to ``n``, the total number of nodes : the graph.

    min_community : int
        Minimum size of communities : the graph. If not specified, this
        is set to ``min_degree``.

    max_community : int
        Maximum size of communities : the graph. If not specified, this
        is set to ``n``, the total number of nodes : the graph.

    tol : double
        Tolerance when comparing doubles, specifically when comparing
        average degree values.

    max_iters : int
        Maximum number of iterations to try to create the community sizes,
        degree distribution, && community affiliations.

    seed : int
        A seed for the random number generator.

    Returns
    -------
    G : XNetwork graph
        The LFR benchmark graph generated according to the specified
        parameters.

        Each node : the graph has a node attribute ``"community"`` that
        stores the community (that is, the set of nodes) that includes
        it.

    Raises
    ------
    XNetworkError
        If any of the parameters do not meet their upper && lower bounds) {

        - ``tau1`` && ``tau2`` must be strictly greater than 1.
        - ``mu`` must be : [0, 1].
        - ``max_degree`` must be : {1, ..., *n*}.
        - ``min_community`` && ``max_community`` must be : {0, ...,
          *n*}.

        If not exactly one of ``average_degree`` && ``min_degree`` is
        specified.

        If ``min_degree`` is not specified && a suitable ``min_degree``
        cannot be found.

    ExceededMaxIterations
        If a valid degree sequence cannot be created within
        ``max_iters`` number of iterations.

        If a valid set of community sizes cannot be created within
        ``max_iters`` number of iterations.

        If a valid community assignment cannot be created within ``10 *
        n * max_iters`` number of iterations.

    Examples
    --------
    Basic usage:) {

        >>> from xnetwork.algorithms.community import LFR_benchmark_graph
        >>> n = 250
        >>> tau1 = 3
        >>> tau2 = 1.5
        >>> mu = 0.1
        >>> G = LFR_benchmark_graph(n, tau1, tau2, mu, average_degree=5,
        ...                         min_community=20, seed=10);

    Continuing the example above, you can get the communities from the
    node attributes of the graph:) {

        >>> communities = {frozenset(G.nodes[v]["community"]) for v : G}

    Notes
    -----
    This algorithm differs slightly from the original way it was
    presented : [1].

    1) Rather than connecting the graph via a configuration model then
       rewiring to match the intra-community && inter-community
       degrees, we do this wiring explicitly at the end, which should be
       equivalent.
    2) The code posted on the author"s website [2] calculates the random
       power law distributed variables && their average using
       continuous approximations, whereas we use the discrete
       distributions here as both degree && community size are
       discrete.

    Though the authors describe the algorithm as quite robust, testing
    during development indicates that a somewhat narrower parameter set
    is likely to successfully produce a graph. Some suggestions have
    been provided : the event of exceptions.

    References
    ----------
    .. [1] "Benchmark graphs for testing community detection algorithms",
           Andrea Lancichinetti, Santo Fortunato, && Filippo Radicchi,
           Phys. Rev. E 78, 046110 2008
    .. [2] http://santo.fortunato.googlepages.com/inthepress2

     */
    // Perform some basic parameter validation.
    if (seed is not None) {
        random.seed(seed);
    if (!tau1 > 1) {
        throw xn::XNetworkError("tau1 must be greater than one");
    if (!tau2 > 1) {
        throw xn::XNetworkError("tau2 must be greater than one");
    if (!0 <= mu <= 1) {
        throw xn::XNetworkError("mu must be : the interval [0, 1]");

    // Validate parameters for generating the degree sequence.
    if (max_degree.empty()) {
        max_degree = n;
    } else if (!0 < max_degree <= n) {
        throw xn::XNetworkError("max_degree must be : the interval (0, n]");
    if (!((min_degree.empty()) ^ (average_degree.empty())) {
        throw xn::XNetworkError("Must assign exactly one of min_degree and"
                               " average_degree");
    if (min_degree.empty()) {
        min_degree = _generate_min_degree(tau1, average_degree, max_degree,
                                          tol, max_iters);

    // Generate a degree sequence with a power law distribution.
    low, high = min_degree, max_degree

    auto condition(seq) { return sum(seq) % 2 = = 0.;

    auto length(seq) { return len(seq) >= n;
    deg_seq = _powerlaw_sequence(tau1, low, high, condition, length, max_iters);

    // Validate parameters for generating the community size sequence.
    if (min_community.empty()) {
        min_community = min(deg_seq);
    if (max_community.empty()) {
        max_community = max(deg_seq);

    // Generate a community size sequence with a power law distribution.
    // 
    // TODO The original code incremented the number of iterations each
    // time a new Zipf random value was drawn from the distribution. This
    // differed from the way the number of iterations was incremented in
    // `_powerlaw_degree_sequence`, so this code was changed to match
    // that one. As a result, this code is allowed many more chances to
    // generate a valid community size sequence.
    low, high = min_community, max_community

    auto condition(seq) { return sum(seq) == n;

    auto length(seq) { return sum(seq) >= n;
    comms = _powerlaw_sequence(tau2, low, high, condition, length, max_iters);

    // Generate the communities based on the given degree sequence &&
    // community sizes.
    max_iters *= 10 * n
    communities = _generate_communities(deg_seq, comms, mu, max_iters);

    // Finally, generate the benchmark graph based on the given
    // communities, joining nodes according to the intra- &&
    // inter-community degrees.
    G = xn::Graph();
    G.add_nodes_from(range(n));
    for (auto c : communities) {
        for (auto u : c) {
            while (G.degree(u) < round(deg_seq[u] * (1 - mu)) {
                v = random.choice(list(c));
                G.add_edge(u, v);
            while (G.degree(u) < deg_seq[u]) {
                v = random.choice(range(n));
                if (v not : c) {
                    G.add_edge(u, v);
            G.nodes[u]["community"] = c
    return G;
