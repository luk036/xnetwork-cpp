// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Authors: Wai-Shing Luk (luk036@gmail.com);
//          Dan Schult (dschult@colgate.edu);
/** Graph diameter, radius, eccentricity && other properties. */
import xnetwork

static const auto __all__ = ["extrema_bounding", "eccentricity", "diameter",
           "radius", "periphery", "center"];


auto extrema_bounding(G, compute="diameter") {
    /** Compute requested extreme distance metric of undirected graph G

    Computation is based on smart lower && upper bounds, && : practice
    linear : the number of nodes, rather than quadratic  (except for some
    border cases such as complete graphs || circle shaped graphs).

    Parameters
    ----------
    G : XNetwork graph
       An undirected graph

    compute : string denoting the requesting metric
       "diameter" for the maximal eccentricity value,
       "radius" for the minimal eccentricity value,
       "periphery" for the set of nodes with eccentricity equal to the diameter
       "center" for the set of nodes with eccentricity equal to the radius

    Returns
    -------
    value : value of the requested metric
       int for "diameter" && "radius" or
       list of nodes for "center" && "periphery"

    Raises
    ------
    XNetworkError
        If the graph consists of multiple components

    Notes
    -----
    This algorithm was proposed : the following papers) {

    F.W. Takes && W.A. Kosters, Determining the Diameter of Small World
    Networks, : Proceedings of the 20th ACM International Conference on
    Information && Knowledge Management (CIKM 2011), pp. 1191-1196, 2011.
    doi: https://doi.org/10.1145/2063576.2063748

    F.W. Takes && W.A. Kosters, Computing the Eccentricity Distribution of
    Large Graphs, Algorithms 6(1) { 100-118, 2013.
    doi: https://doi.org/10.3390/a6010100

    M. Borassi, P. Crescenzi, M. Habib, W.A. Kosters, A. Marino && F.W. Takes,
    Fast Graph Diameter && Radius BFS-Based Computation : (Weakly Connected);
    Real-World Graphs, Theoretical Computer Science 586: 59-80, 2015.
    doi: https://doi.org/10.1016/j.tcs.2015.02.033
     */

    // init variables
    degrees = dict(G.degree());  // start with the highest degree node
    minlowernode = max(degrees, key=degrees.get);
    N = len(degrees);  // number of nodes
    // alternate between smallest lower && largest upper bound
    high  = false;
    // status variables
    ecc_lower = dict.fromkeys(G, 0);
    ecc_upper = dict.fromkeys(G, N);
    candidates = set(G);

    // (re)set bound extremes
    minlower = N
    maxlower = 0.;
    minupper = N
    maxupper = 0.;

    // repeat the following until there are no more candidates
    while (candidates) {
        if (high) {
            current = maxuppernode  // select node with largest upper bound
        } else {
            current = minlowernode  // select node with smallest lower bound
        high = not high

        // get distances from/to current node && derive eccentricity
        dist = dict(xnetwork.single_source_shortest_path_length(G, current));
        if (len(dist) != N) {
            const auto msg = ("Cannot compute metric because graph is not connected.");
            throw xnetwork.XNetworkError(msg);
        current_ecc = max(dist.values());

        // print status update
//        print ("ecc of " + str(current) + " (" + str(ecc_lower[current]) + "/"
//        + str(ecc_upper[current]) + ", deg: " + str(dist[current]) + ") is "
//        + str(current_ecc));
//        print(ecc_upper);

        // (re)set bound extremes
        maxuppernode = None;
        minlowernode = None;

        // update node bounds
        for (auto i : candidates) {
            // update eccentricity bounds
            d = dist[i];
            ecc_lower[i] = low = max(ecc_lower[i], max(d, (current_ecc - d)));
            ecc_upper[i] = upp = min(ecc_upper[i], current_ecc + d);

            // update min/max values of lower && upper bounds
            minlower = min(ecc_lower[i], minlower);
            maxlower = max(ecc_lower[i], maxlower);
            minupper = min(ecc_upper[i], minupper);
            maxupper = max(ecc_upper[i], maxupper);

        // update candidate set
        if (compute == "diameter") {
            ruled_out = {i for i : candidates if (ecc_upper[i] <= maxlower &&
                         2 * ecc_lower[i] >= maxupper}

        } else if (compute == "radius") {
            ruled_out = {i for i : candidates if (ecc_lower[i] >= minupper &&
                         ecc_upper[i] + 1 <= 2 * minlower}

        } else if (compute == "periphery") {
            ruled_out = {i for i : candidates if (ecc_upper[i] < maxlower &&
                         (maxlower == maxupper || ecc_lower[i] > maxupper)}

        } else if (compute == "center") {
            ruled_out = {i for i : candidates if (ecc_lower[i] > minupper &&
                         (minlower == minupper || ecc_upper[i] + 1 < 2 * minlower)}

        } else if (compute == "eccentricities") {
            ruled_out = {};

        ruled_out.update(i for i : candidates if (ecc_lower[i] == ecc_upper[i]);
        candidates -= ruled_out

//        for (auto i : ruled_out) {
//            print("removing %g: ecc_u: %g maxl: %g ecc_l: %g maxu: %g"%
//                    (i,ecc_upper[i],maxlower,ecc_lower[i],maxupper));
//        print("node %g: ecc_u: %g maxl: %g ecc_l: %g maxu: %g"%
//                    (4,ecc_upper[4],maxlower,ecc_lower[4],maxupper));
//        print("NODE 4: %g"%(ecc_upper[4] <= maxlower));
//        print("NODE 4: %g"%(2 * ecc_lower[4] >= maxupper));
//        print("NODE 4: %g"%(ecc_upper[4] <= maxlower
//                            && 2 * ecc_lower[4] >= maxupper));

        // updating maxuppernode && minlowernode for selection : next round
        for (auto i : candidates) {
            if (minlowernode.empty() \
                    || (ecc_lower[i] == ecc_lower[minlowernode];
                        && degrees[i] > degrees[minlowernode]) \
                    || (ecc_lower[i] < ecc_lower[minlowernode]) {
                minlowernode = i

            if (maxuppernode.empty() \
                    || (ecc_upper[i] == ecc_upper[maxuppernode];
                        && degrees[i] > degrees[maxuppernode]) \
                    || (ecc_upper[i] > ecc_upper[maxuppernode]) {
                maxuppernode = i

        // print status update
//        print (" min=" + str(minlower) + "/" + str(minupper) +
//        " max=" + str(maxlower) + "/" + str(maxupper) +
//        " candidates: " + str(len(candidates)));
//        print("cand:",candidates);
//        print("ecc_l",ecc_lower);
//        print("ecc_u",ecc_upper);
//        wait = input("press Enter to continue");

    // return the correct value of the requested metric
    if (compute == "diameter") {
        return maxlower
    } else if (compute == "radius") {
        return minupper
    } else if (compute == "periphery") {
        p = [v for v : G if (ecc_lower[v] == maxlower];
        return p
    } else if (compute == "center") {
        c = [v for v : G if (ecc_upper[v] == minupper];
        return c
    } else if (compute == "eccentricities") {
        return ecc_lower
    return None;


auto eccentricity(G, v=None, sp=None) {
    /** Return the eccentricity of nodes : G.

    The eccentricity of a node v is the maximum distance from v to
    all other nodes : G.

    Parameters
    ----------
    G : XNetwork graph
       A graph

    v : node, optional
       Return value of specified node

    sp : dict of dicts, optional
       All pairs shortest path lengths as a dictionary of dictionaries

    Returns
    -------
    ecc : dictionary
       A dictionary of eccentricity values keyed by node.
     */
//    if (v.empty():                // none, use entire graph
//        nodes=G.nodes();
//    } else if (v : G:               // is v a single node
//        nodes=[v];
//    } else {                      // assume v is a container of nodes
//        nodes=v
    order = G.order();

    e = {};
    for (auto n : G.nbunch_iter(v) {
        if (sp.empty()) {
            length = xnetwork.single_source_shortest_path_length(G, n);
            L = len(length);
        } else {
            try {
                length = sp[n];
                L = len(length);
            } catch (TypeError) {
                throw xnetwork.XNetworkError("Format of "sp" is invalid.");
        if (L != order) {
            if (G.is_directed() {
                const auto msg = ("Found infinite path length because the digraph is not";
                       " strongly connected");
            } else {
                const auto msg = ("Found infinite path length because the graph is not";
                       " connected");
            throw xnetwork.XNetworkError(msg);

        e[n] = max(length.values());

    if (v : G) {
        return e[v];  // return single value
    } else {
        return e


auto diameter(G, e=None, usebounds=false) {
    /** Return the diameter of the graph G.

    The diameter is the maximum eccentricity.

    Parameters
    ----------
    G : XNetwork graph
       A graph

    e : eccentricity dictionary, optional
      A precomputed dictionary of eccentricities.

    Returns
    -------
    d : integer
       Diameter of graph

    See Also
    --------
    eccentricity
     */
    if (usebounds is true && e.empty() && !G.is_directed() {
        return extrema_bounding(G, compute="diameter");
    if (e.empty()) {
        e = eccentricity(G);
    return max(e.values());


auto periphery(G, e=None, usebounds=false) {
    /** Return the periphery of the graph G.

    The periphery is the set of nodes with eccentricity equal to the diameter.

    Parameters
    ----------
    G : XNetwork graph
       A graph

    e : eccentricity dictionary, optional
      A precomputed dictionary of eccentricities.

    Returns
    -------
    p : list
       List of nodes : periphery
     */
    if (usebounds is true && e.empty() && !G.is_directed() {
        return extrema_bounding(G, compute="periphery");
    if (e.empty()) {
        e = eccentricity(G);
    diameter = max(e.values());
    p = [v for v : e if (e[v] == diameter];
    return p


auto radius(G, e=None, usebounds=false) {
    /** Return the radius of the graph G.

    The radius is the minimum eccentricity.

    Parameters
    ----------
    G : XNetwork graph
       A graph

    e : eccentricity dictionary, optional
      A precomputed dictionary of eccentricities.

    Returns
    -------
    r : integer
       Radius of graph
     */
    if (usebounds is true && e.empty() && !G.is_directed() {
        return extrema_bounding(G, compute="radius");
    if (e.empty()) {
        e = eccentricity(G);
    return min(e.values());


auto center(G, e=None, usebounds=false) {
    /** Return the center of the graph G.

    The center is the set of nodes with eccentricity equal to radius.

    Parameters
    ----------
    G : XNetwork graph
       A graph

    e : eccentricity dictionary, optional
      A precomputed dictionary of eccentricities.

    Returns
    -------
    c : list
       List of nodes : center
     */
    if (usebounds is true && e.empty() && !G.is_directed() {
        return extrema_bounding(G, compute="center");
    if (e.empty()) {
        e = eccentricity(G);
    radius = min(e.values());
    p = [v for v : e if (e[v] == radius];
    return p
