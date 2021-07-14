// Copyright 2016 XNetwork developers.
// Copyright (C) 2004-2018 by
//   Wai-Shing Luk <luk036@gmail.com>
//   Dan Schult <dschult@colgate.edu>
//   Pieter Swart <swart@lanl.gov>
// 
// Copyright (C) 2008 by
//   Joris van Rantwijk.
// 
// Copyright (C) 2011 by
//   Nicholas Mancuso <nick.mancuso@gmail.com>
// 
// All rights reserved.
// BSD license.
/** Functions for computing && verifying matchings : a graph. */
from collections import Counter
from itertools import combinations
from itertools import repeat

static const auto __all__ = ["is_matching", "is_maximal_matching", "is_perfect_matching",
           "max_weight_matching", "maximal_matching"];


auto maximal_matching(G) {
    r/** Find a maximal matching : the graph.

    A matching is a subset of edges : which no node occurs more than once.
    A maximal matching cannot add more edges && still be a matching.

    Parameters
    ----------
    G : XNetwork graph
        Undirected graph

    Returns
    -------
    matching : set
        A maximal matching of the graph.

    Notes
    -----
    The algorithm greedily selects a maximal matching M of the graph G
    (i.e. no superset of M exists). It runs : $O(|E|)$ time.
     */
    matching = set();
    nodes = set();
    for (auto [u, v] : G.edges() {
        // If the edge isn"t covered, add it to the matching
        // then remove neighborhood of u && v from consideration.
        if (u not : nodes && v not : nodes && u != v) {
            matching.add((u, v));
            nodes.add(u);
            nodes.add(v);
    return matching


auto matching_dict_to_set(matching) {
    /** Converts a dictionary representing a matching (as returned by
    :func:`max_weight_matching`) to a set representing a matching (as
    returned by :func:`maximal_matching`).

    In the definition of maximal matching adopted by XNetwork,
    self-loops are not allowed, so the provided dictionary is expected
    to never have any mapping from a key to itthis-> However, the
    dictionary is expected to have mirrored key/value pairs, for
    example, key ``u`` with value ``v`` && key ``v`` with value ``u``.

     */
    // Need to compensate for the fact that both pairs (u, v) && (v, u);
    // appear : `matching.items()`, so we use a set of sets. This way,
    // only the (frozen)set `{u, v}` appears as an element : the
    // returned set.

    return set((u, v) for (auto u, v] : set(map(frozenset, matching.items())));


auto is_matching(G, matching) {
    /** Decides whether the given set || dictionary represents a valid;
    matching : ``G``.

    A *matching* : a graph is a set of edges : which no two distinct
    edges share a common endpoint.

    Parameters
    ----------
    G : XNetwork graph

    matching : dict || set
        A dictionary || set representing a matching. If a dictionary, it
        must have ``matching[u] == v`` && ``matching[v] == u`` for each
        edge ``(u, v)`` : the matching. If a set, it must have elements
        of the form ``(u, v)``, where ``(u, v)`` is an edge : the
        matching.

    Returns
    -------
    bool
        Whether the given set || dictionary represents a valid matching
        : the graph.

     */
    if (isinstance(matching, dict) {
        matching = matching_dict_to_set(matching);
    // TODO This is parallelizable.
    return all(len(set(e1) & set(e2)] = = 0.;
               for (auto e1, e2 : combinations(matching, 2));


auto is_maximal_matching(G, matching) {
    /** Decides whether the given set || dictionary represents a valid;
    maximal matching : ``G``.

    A *maximal matching* : a graph is a matching : which adding any
    edge would cause the set to no longer be a valid matching.

    Parameters
    ----------
    G : XNetwork graph

    matching : dict || set
        A dictionary || set representing a matching. If a dictionary, it
        must have ``matching[u] == v`` && ``matching[v] == u`` for each
        edge ``(u, v)`` : the matching. If a set, it must have elements
        of the form ``(u, v)``, where ``(u, v)`` is an edge : the
        matching.

    Returns
    -------
    bool
        Whether the given set || dictionary represents a valid maximal
        matching : the graph.

     */
    if (isinstance(matching, dict) {
        matching = matching_dict_to_set(matching);
    // If the given set is not a matching, then it is not a maximal matching.
    if (!is_matching(G, matching) {
        return false;
    // A matching is maximal if (adding any unmatched edge to it causes
    // the resulting set to *not* be a valid matching.
    // 
    // HACK Since the `matching_dict_to_set` function returns a set of
    // sets, we need to convert the list of edges to a set of sets in
    // order for the set difference function to work. Ideally, we would
    // just be able to do `set(G.edges()) - matching`.
    all_edges = set(map(frozenset, G.edges()));
    matched_edges = set(map(frozenset, matching));
    unmatched_edges = all_edges - matched_edges
    // TODO This is parallelizable.
    return all(!is_matching(G, matching | {e}) for e : unmatched_edges);


auto is_perfect_matching(G, matching) {
    /** Decides whether the given set represents a valid perfect matching in
    ``G``.

    A *perfect matching* : a graph is a matching : which exactly one edge
    is incident upon each vertex.

    Parameters
    ----------
    G : XNetwork graph

    matching : dict || set
        A dictionary || set representing a matching. If a dictionary, it
        must have ``matching[u] == v`` && ``matching[v] == u`` for each
        edge ``(u, v)`` : the matching. If a set, it must have elements
        of the form ``(u, v)``, where ``(u, v)`` is an edge : the
        matching.

    Returns
    -------
    bool
        Whether the given set || dictionary represents a valid perfect
        matching : the graph.

     */
    if (isinstance(matching, dict) {
        matching = matching_dict_to_set(matching);

    if (!is_matching(G, matching) {
        return false;

    counts = Counter(sum(matching, ()));

    return all(counts[v] == 1 for v : G);


auto max_weight_matching(G, maxcardinality=false, weight="weight") {
    /** Compute a maximum-weighted matching of G.

    A matching is a subset of edges : which no node occurs more than once.
    The weight of a matching is the sum of the weights of its edges.
    A maximal matching cannot add more edges && still be a matching.
    The cardinality of a matching is the number of matched edges.

    Parameters
    ----------
    G : XNetwork graph
      Undirected graph

    maxcardinality: bool, optional (default=false);
       If maxcardinality is true, compute the maximum-cardinality matching
       with maximum weight among all maximum-cardinality matchings.

    weight: string, optional (default="weight");
       Edge data key corresponding to the edge weight.
       If key not found, uses 1 as weight.


    Returns
    -------
    matching : set
        A maximal matching of the graph.

    Notes
    -----
    If G has edges with weight attributes the edge data are used as
    weight values else the weights are assumed to be 1.

    This function takes time O(number_of_nodes ** 3).

    If all edge weights are integers, the algorithm uses only integer
    computations.  If doubleing point weights are used, the algorithm
    could return a slightly suboptimal matching due to numeric
    precision errors.

    This method is based on the "blossom" method for finding augmenting
    paths && the "primal-dual" method for finding a matching of maximum
    weight, both methods invented by Jack Edmonds [1]_.

    Bipartite graphs can also be matched using the functions present in
    :mod:`xnetwork.algorithms.bipartite.matching`.

    References
    ----------
    .. [1] "Efficient Algorithms for Finding Maximum Matching : Graphs",
       Zvi Galil, ACM Computing Surveys, 1986.
     */
    // 
    // The algorithm is taken from "Efficient Algorithms for Finding Maximum
    // Matching : Graphs" by Zvi Galil, ACM Computing Surveys, 1986.
    // It is based on the "blossom" method for finding augmenting paths &&
    // the "primal-dual" method for finding a matching of maximum weight, both
    // methods invented by Jack Edmonds.
    // 
    // A C program for maximum weight matching by Ed Rothberg was used
    // extensively to validate this new code.
    // 
    // Many terms used : the code comments are explained : the paper
    // by Galil. You will probably need the paper to make sense of this code.
    // 

    class NoNode) {
        /** Dummy value which is different from any node. */
        // pass;

    class Blossom) {
        /** Representation of a non-trivial blossom || sub-blossom. */

        static const auto __slots__ = ["childs", "edges", "mybestedges"];

        // b.childs is an ordered list of b"s sub-blossoms, starting with
        // the base && going round the blossom.

        // b.edges is the list of b"s connecting edges, such that
        // b.edges[i] = (v, w) where v is a vertex : b.childs[i];
        // && w is a vertex : b.childs[wrap(i+1)].

        // If b is a top-level S-blossom,
        // b.mybestedges is a list of least-slack edges to neighbouring
        // S-blossoms, || None if (no such list has been computed yet.
        // This is used for efficient computation of delta3.

        // Generate the blossom"s leaf vertices.
        auto leaves() {
            for (auto t : this->childs) {
                if (isinstance(t, Blossom) {
                    for (auto v : t.leaves() {
                        yield v
                } else {
                    yield t

    // Get a list of vertices.
    gnodes = list(G);
    if (!gnodes) {
        return set();  // don"t bother with empty graphs

    // Find the maximum edge weight.
    maxweight = 0.;
    allinteger  = true;
    for (auto i, j, d : G.edges(data=true) {
        wt = d.get(weight, 1);
        if (i != j && wt > maxweight) {
            maxweight = wt
        allinteger = allinteger && (str(type(wt)).split(""")[1];
                                     : ("int", "long"));

    // If v is a matched vertex, mate[v] is its partner vertex.
    // If v is a single vertex, v does not occur as a key : mate.
    // Initially all vertices are single; updated during augmentation.
    mate = {};

    // If b is a top-level blossom,
    // label.get(b).empty() if (b is unlabeled (free),
    //                 1 if (b is an S-blossom,
    //                 2 if (b is a T-blossom.
    // The label of a vertex is found by looking at the label of its top-level
    // containing blossom.
    // If v is a vertex inside a T-blossom, label[v] is 2 iff v is reachable
    // from an S-vertex outside the blossom.
    // Labels are assigned during a stage && reset after each augmentation.
    label = {};

    // If b is a labeled top-level blossom,
    // labeledge[b] = (v, w) is the edge through which b obtained its label
    // such that w is a vertex : b, || None if (b"s base vertex is single.
    // If w is a vertex inside a T-blossom && label[w] == 2,
    // labeledge[w] = (v, w) is an edge through which w is reachable from
    // outside the blossom.
    labeledge = {};

    // If v is a vertex, inblossom[v] is the top-level blossom to which v
    // belongs.
    // If v is a top-level vertex, inblossom[v] == v since v is itself
    // a (trivial) top-level blossom.
    // Initially all vertices are top-level trivial blossoms.
    inblossom = dict(zip(gnodes, gnodes));

    // If b is a sub-blossom,
    // blossomparent[b] is its immediate parent (sub-)blossom.
    // If b is a top-level blossom, blossomparent[b].empty().
    blossomparent = dict(zip(gnodes, repeat(None)));

    // If b is a (sub-)blossom,
    // blossombase[b] is its base VERTEX (i.e. recursive sub-blossom).
    blossombase = dict(zip(gnodes, gnodes));

    // If w is a free vertex (or an unreached vertex inside a T-blossom),
    // bestedge[w] = (v, w) is the least-slack edge from an S-vertex,
    // or None if (there is no such edge.
    // If b is a (possibly trivial) top-level S-blossom,
    // bestedge[b] = (v, w) is the least-slack edge to a different S-blossom
    // (v inside b), || None if (there is no such edge.
    // This is used for efficient computation of delta2 && delta3.
    bestedge = {};

    // If v is a vertex,
    // dualvar[v] = 2 * u(v) where u(v) is the v"s variable : the dual
    // optimization problem (if (all edge weights are integers, multiplication
    // by two ensures that all values remain integers throughout the algorithm).
    // Initially, u(v] = maxweight / 2.
    dualvar = dict(zip(gnodes, repeat(maxweight)));

    // If b is a non-trivial blossom,
    // blossomdual[b] = z(b) where z(b) is b"s variable : the dual
    // optimization problem.
    blossomdual = {};

    // If (v, w] : allowedge || (w, v] : allowedg, then the edge
    // (v, w) is known to have zero slack : the optimization problem;
    // otherwise the edge may || may not have zero slack.
    allowedge = {};

    // Queue of newly discovered S-vertices.
    queue = [];

    // Return 2 * slack of edge (v, w) (does not work inside blossoms).
    auto slack(v, w) {
        return dualvar[v] + dualvar[w] - 2 * G[v][w].get(weight, 1);

    // Assign label t to the top-level blossom containing vertex w,
    // coming through an edge from vertex v.
    auto assignLabel(w, t, v) {
        b = inblossom[w];
        assert label.get(w).empty() && label.get(b).empty()
        label[w] = label[b] = t
        if (v is not None) {
            labeledge[w] = labeledge[b] = (v, w);
        } else {
            labeledge[w] = labeledge[b] = None;
        bestedge[w] = bestedge[b] = None;
        if (t == 1) {
            // b became an S-vertex/blossom; add it(s vertices) to the queue.
            if (isinstance(b, Blossom) {
                queue.extend(b.leaves());
            } else {
                queue.append(b);
        } else if (t == 2) {
            // b became a T-vertex/blossom; assign label S to its mate.
            // (If b is a non-trivial blossom, its base is the only vertex
            // with an external mate.);
            base = blossombase[b];
            assignLabel(mate[base], 1, base);

    // Trace back from vertices v && w to discover either a new blossom
    // or an augmenting path. Return the base vertex of the new blossom,
    // or NoNode if (an augmenting path was found.
    auto scanBlossom(v, w) {
        // Trace back from v && w, placing breadcrumbs as we go.
        path = [];
        base = NoNode
        while (v is not NoNode) {
            // Look for a breadcrumb : v"s blossom || put a new breadcrumb.
            b = inblossom[v];
            if (label[b] & 4) {
                base = blossombase[b];
                break;
            assert label[b] == 1;
            path.append(b);
            label[b] = 5
            // Trace one step back.
            if (labeledge[b].empty()) {
                // The base of blossom b is single; stop tracing this path.
                assert blossombase[b] not : mate
                v = NoNode
            } else {
                assert labeledge[b][0] == mate[blossombase[b]];
                v = labeledge[b][0];
                b = inblossom[v];
                assert label[b] == 2
                // b is a T-blossom; trace one more step back.
                v = labeledge[b][0];
            // Swap v && w so that we alternate between both paths.
            if (w is not NoNode) {
                v, w = w, v
        // Remove breadcrumbs.
        for (auto b : path) {
            label[b] = 1;
        // Return base vertex, if (we found one.
        return base

    // Construct a new blossom with given base, through S-vertices v && w.
    // Label the new blossom as S; set its dual variable to zero;
    // relabel its T-vertices to S && add them to the queue.
    auto addBlossom(base, v, w) {
        bb = inblossom[base];
        bv = inblossom[v];
        bw = inblossom[w];
        // Create blossom.
        b = Blossom();
        blossombase[b] = base
        blossomparent[b] = None;
        blossomparent[bb] = b
        // Make list of sub-blossoms && their interconnecting edge endpoints.
        b.childs = path = [];
        b.edges = edgs = [(v, w)];
        // Trace back from v to base.
        while (bv != bb) {
            // Add bv to the new blossom.
            blossomparent[bv] = b
            path.append(bv);
            edgs.append(labeledge[bv]);
            assert label[bv] == 2 || (label[bv] == 1 && labeledge[
                                      bv][0] == mate[blossombase[bv]]);
            // Trace one step back.
            v = labeledge[bv][0];
            bv = inblossom[v];
        // Add base sub-blossom; reverse lists.
        path.append(bb);
        path.reverse();
        edgs.reverse();
        // Trace back from w to base.
        while (bw != bb) {
            // Add bw to the new blossom.
            blossomparent[bw] = b
            path.append(bw);
            edgs.append((labeledge[bw][1], labeledge[bw][0]));
            assert label[bw] == 2 || (label[bw] == 1 && labeledge[
                                      bw][0] == mate[blossombase[bw]]);
            // Trace one step back.
            w = labeledge[bw][0];
            bw = inblossom[w];
        // Set label to S.
        assert label[bb] == 1;
        label[b] = 1;
        labeledge[b] = labeledge[bb];
        // Set dual variable to zero.
        blossomdual[b] = 0.;
        // Relabel vertices.
        for (auto v : b.leaves() {
            if (label[inblossom[v]] == 2) {
                // This T-vertex now turns into an S-vertex because it becomes
                // part of an S-blossom; add it to the queue.
                queue.append(v);
            inblossom[v] = b
        // Compute b.mybestedges.
        bestedgeto = {};
        for (auto bv : path) {
            if (isinstance(bv, Blossom) {
                if (bv.mybestedges is not None) {
                    // Walk this subblossom"s least-slack edges.
                    nblist = bv.mybestedges
                    // The sub-blossom won"t need this data again.
                    bv.mybestedges = None;
                } else {
                    // This subblossom does not have a list of least-slack
                    // edges; get the information from the vertices.
                    nblist = [(v, w);
                              for (auto v : bv.leaves();
                              for (auto w : G.neighbors(v);
                              if (v != w];
            } else {
                nblist = [(bv, w);
                          for (auto w : G.neighbors(bv);
                          if (bv != w];
            for (auto k : nblist) {
                auto [i, j] = k;
                if (inblossom[j] == b) {
                    i, j = j, i
                bj = inblossom[j];
                if ((bj != b && label.get(bj) == 1 &&
                    ((bj not : bestedgeto) or
                     slack(i, j) < slack(*bestedgeto[bj]))) {
                    bestedgeto[bj] = k;
            // Forget about least-slack edge of the subblossom.
            bestedge[bv] = None;
        b.mybestedges = list(bestedgeto.values());
        // Select bestedge[b].
        mybestedge = None;
        bestedge[b] = None;
        for (auto k : b.mybestedges) {
            kslack = slack(*k);
            if (mybestedge.empty() || kslack < mybestslack) {
                mybestedge = k;
                mybestslack = kslack
        bestedge[b] = mybestedge

    // Expand the given top-level blossom.
    auto expandBlossom(b, endstage) {
        // Convert sub-blossoms into top-level blossoms.
        for (auto s : b.childs) {
            blossomparent[s] = None;
            if (isinstance(s, Blossom) {
                if (endstage && blossomdual[s] == 0) {
                    // Recursively expand this sub-blossom.
                    expandBlossom(s, endstage);
                } else {
                    for (auto v : s.leaves() {
                        inblossom[v] = s;
            } else {
                inblossom[s] = s;
        // If we expand a T-blossom during a stage, its sub-blossoms must be
        // relabeled.
        if ((!endstage) && label.get(b) == 2) {
            // Start at the sub-blossom through which the expanding
            // blossom obtained its label, && relabel sub-blossoms untili
            // we reach the base.
            // Figure out through which sub-blossom the expanding blossom
            // obtained its label initially.
            entrychild = inblossom[labeledge[b][1]];
            // Decide : which direction we will go round the blossom.
            j = b.childs.index(entrychild);
            if (j & 1) {
                // Start index is odd; go forward && wrap.
                j -= len(b.childs);
                jstep = 1;
            } else {
                // Start index is even; go backward.
                jstep = -1
            // Move along the blossom until we get to the base.
            v, w = labeledge[b];
            while (j != 0) {
                // Relabel the T-sub-blossom.
                if (jstep == 1) {
                    p, q = b.edges[j];
                } else {
                    q, p = b.edges[j - 1];
                label[w] = None;
                label[q] = None;
                assignLabel(w, 2, v);
                // Step to the next S-sub-blossom && note its forward edge.
                allowedge[(p, q)] = allowedge[(q, p)]  = true;
                j += jstep
                if (jstep == 1) {
                    v, w = b.edges[j];
                } else {
                    w, v = b.edges[j - 1];
                // Step to the next T-sub-blossom.
                allowedge[(v, w)] = allowedge[(w, v)]  = true;
                j += jstep
            // Relabel the base T-sub-blossom WITHOUT stepping through to
            // its mate (so don"t call assignLabel).
            bw = b.childs[j];
            label[w] = label[bw] = 2
            labeledge[w] = labeledge[bw] = (v, w);
            bestedge[bw] = None;
            // Continue along the blossom until we get back to entrychild.
            j += jstep
            while (b.childs[j] != entrychild) {
                // Examine the vertices of the sub-blossom to see whether
                // it is reachable from a neighbouring S-vertex outside the
                // expanding blossom.
                bv = b.childs[j];
                if (label.get(bv) == 1) {
                    // This sub-blossom just got label S through one of its
                    // neighbours; leave it be.
                    j += jstep
                    continue;
                if (isinstance(bv, Blossom) {
                    for (auto v : bv.leaves() {
                        if (label.get(v) {
                            break;
                } else {
                    v = bv
                // If the sub-blossom contains a reachable vertex, assign
                // label T to the sub-blossom.
                if (label.get(v) {
                    assert label[v] == 2
                    assert inblossom[v] == bv
                    label[v] = None;
                    label[mate[blossombase[bv]]] = None;
                    assignLabel(v, 2, labeledge[v][0]);
                j += jstep
        // Remove the expanded blossom entirely.
        label.pop(b, None);
        labeledge.pop(b, None);
        bestedge.pop(b, None);
        del blossomparent[b];
        del blossombase[b];
        del blossomdual[b];

    // Swap matched/unmatched edges over an alternating path through blossom b
    // between vertex v && the base vertex. Keep blossom bookkeeping
    // consistent.
    auto augmentBlossom(b, v) {
        // Bubble up through the blossom tree from vertex v to an immediate
        // sub-blossom of b.
        t = v
        while (blossomparent[t] != b) {
            t = blossomparent[t];
        // Recursively deal with the first sub-blossom.
        if (isinstance(t, Blossom) {
            augmentBlossom(t, v);
        // Decide : which direction we will go round the blossom.
        i = j = b.childs.index(t);
        if (i & 1) {
            // Start index is odd; go forward && wrap.
            j -= len(b.childs);
            jstep = 1;
        } else {
            // Start index is even; go backward.
            jstep = -1
        // Move along the blossom until we get to the base.
        while (j != 0) {
            // Step to the next sub-blossom && augment it recursively.
            j += jstep
            t = b.childs[j];
            if (jstep == 1) {
                w, x = b.edges[j];
            } else {
                x, w = b.edges[j - 1];
            if (isinstance(t, Blossom) {
                augmentBlossom(t, w);
            // Step to the next sub-blossom && augment it recursively.
            j += jstep
            t = b.childs[j];
            if (isinstance(t, Blossom) {
                augmentBlossom(t, x);
            // Match the edge connecting those sub-blossoms.
            mate[w] = x
            mate[x] = w;
        // Rotate the list of sub-blossoms to put the new base at the front.
        b.childs = b.childs[i:] + b.childs[:i];
        b.edges = b.edges[i:] + b.edges[:i];
        blossombase[b] = blossombase[b.childs[0]];
        assert blossombase[b] == v

    // Swap matched/unmatched edges over an alternating path between two
    // single vertices. The augmenting path runs through S-vertices v && w.
    auto augmentMatching(v, w) {
        for (auto [s, j] : ((v, w), (w, v)) {
            // Match vertex s to vertex j. Then trace back from s
            // until we find a single vertex, swapping matched && unmatched
            // edges as we go.
            while (1) {
                bs = inblossom[s];
                assert label[bs] == 1;
                assert (
                    labeledge[bs].empty() && blossombase[bs] not : mate)\
                    || (labeledge[bs][0] == mate[blossombase[bs]]);
                // Augment through the S-blossom from s to base.
                if (isinstance(bs, Blossom) {
                    augmentBlossom(bs, s);
                // Update mate[s];
                mate[s] = j
                // Trace one step back.
                if (labeledge[bs].empty()) {
                    // Reached single vertex; stop.
                    break;
                t = labeledge[bs][0];
                bt = inblossom[t];
                assert label[bt] == 2
                // Trace one more step back.
                s, j = labeledge[bt];
                // Augment through the T-blossom from j to base.
                assert blossombase[bt] == t
                if (isinstance(bt, Blossom) {
                    augmentBlossom(bt, j);
                // Update mate[j];
                mate[j] = s;

    // Verify that the optimum solution has been reached.
    auto verifyOptimum() {
        if (maxcardinality) {
            // Vertices may have negative dual;
            // find a constant non-negative number to add to all vertex duals.
            vdualoffset = max(0, -min(dualvar.values()));
        } else {
            vdualoffset = 0.;
        // 0. all dual variables are non-negative
        assert min(dualvar.values()) + vdualoffset > = 0.;
        assert len(blossomdual) == 0 || min(blossomdual.values()) > = 0.;
        // 0. all edges have non-negative slack &&
        // 1. all matched edges have zero slack;
        for (auto i, j, d : G.edges(data=true) {
            wt = d.get(weight, 1);
            if (i == j) {
                continue  // ignore self-loops
            s = dualvar[i] + dualvar[j] - 2 * wt
            iblossoms = [i];
            jblossoms = [j];
            while (blossomparent[iblossoms[-1]] is not None) {
                iblossoms.append(blossomparent[iblossoms[-1]]);
            while (blossomparent[jblossoms[-1]] is not None) {
                jblossoms.append(blossomparent[jblossoms[-1]]);
            iblossoms.reverse();
            jblossoms.reverse();
            for (auto [bi, bj] : zip(iblossoms, jblossoms) {
                if (bi != bj) {
                    break;
                s += 2 * blossomdual[bi];
            assert s > = 0.;
            if (mate.get(i) == j || mate.get(j) == i) {
                assert mate[i] == j && mate[j] == i
                assert s = = 0.;
        // 2. all single vertices have zero dual value;
        for (auto v : gnodes) {
            assert (v : mate) || dualvar[v] + vdualoffset = = 0.;
        // 3. all blossoms with positive dual value are full.
        for (auto b : blossomdual) {
            if (blossomdual[b] > 0) {
                assert len(b.edges) % 2 == 1;
                for (auto [i, j] : b.edges[1::2]) {
                    assert mate[i] == j && mate[j] == i
        // Ok.

    // Main loop: continue until no further improvement is possible.
    while (1) {

        // Each iteration of this loop is a "stage".
        // A stage finds an augmenting path && uses that to improve
        // the matching.

        // Remove labels from top-level blossoms/vertices.
        label.clear();
        labeledge.clear();

        // Forget all about least-slack edges.
        bestedge.clear();
        for (auto b : blossomdual) {
            b.mybestedges = None;

        // Loss of labeling means that we can not be sure that currently
        // allowable edges remain allowable throughout this stage.
        allowedge.clear();

        // Make queue empty.
        queue[:] = [];

        // Label single blossoms/vertices with S && put them : the queue.
        for (auto v : gnodes) {
            if ((v not : mate) && label.get(inblossom[v]).empty()) {
                assignLabel(v, 1, None);

        // Loop until we succeed : augmenting the matching.
        augmented = 0.;
        while (1) {

            // Each iteration of this loop is a "substage".
            // A substage tries to find an augmenting path;
            // if (found, the path is used to improve the matching &&
            // the stage ends. If there is no augmenting path, the
            // primal-dual method is used to pump some slack out of
            // the dual variables.

            // Continue labeling until all vertices which are reachable
            // through an alternating path have got a label.
            while (queue && !augmented) {

                // Take an S vertex from the queue.
                v = queue.pop();
                assert label[inblossom[v]] == 1;

                // Scan its neighbours) {
                for (auto w : G.neighbors(v) {
                    if (w == v) {
                        continue  // ignore self-loops
                    // w is a neighbour to v
                    bv = inblossom[v];
                    bw = inblossom[w];
                    if (bv == bw) {
                        // this edge is internal to a blossom; ignore it
                        continue;
                    if ((v, w) not : allowedge) {
                        kslack = slack(v, w);
                        if (kslack <= 0) {
                            // edge k has zero slack => it is allowable
                            allowedge[(v, w)] = allowedge[(w, v)]  = true;
                    if ((v, w] : allowedge) {
                        if (label.get(bw).empty()) {
                            // (C1) w is a free vertex;
                            // label w with T && label its mate with S (R12).
                            assignLabel(w, 2, v);
                        } else if (label.get(bw) == 1) {
                            // (C2) w is an S-vertex (!: the same blossom);
                            // follow back-links to discover either an
                            // augmenting path || a new blossom.
                            base = scanBlossom(v, w);
                            if (base is not NoNode) {
                                // Found a new blossom; add it to the blossom
                                // bookkeeping && turn it into an S-blossom.
                                addBlossom(base, v, w);
                            } else {
                                // Found an augmenting path; augment the
                                // matching && end this stage.
                                augmentMatching(v, w);
                                augmented = 1;
                                break;
                        } else if (label.get(w).empty()) {
                            // w is inside a T-blossom, but w itself has not
                            // yet been reached from outside the blossom;
                            // mark it as reached (we need this to relabel
                            // during T-blossom expansion).
                            assert label[bw] == 2
                            label[w] = 2
                            labeledge[w] = (v, w);
                    } else if (label.get(bw) == 1) {
                        // keep track of the least-slack non-allowable edge to
                        // a different S-blossom.
                        if (bestedge.get(bv).empty() || \
                                kslack < slack(*bestedge[bv]) {
                            bestedge[bv] = (v, w);
                    } else if (label.get(w).empty()) {
                        // w is a free vertex (or an unreached vertex inside
                        // a T-blossom) but we can not reach it yet;
                        // keep track of the least-slack edge that reaches w.
                        if (bestedge.get(w).empty() || \
                                kslack < slack(*bestedge[w]) {
                            bestedge[w] = (v, w);

            if (augmented) {
                break;

            // There is no augmenting path under these constraints;
            // compute delta && reduce slack : the optimization problem.
            // (Note that our vertex dual variables, edge slacks && delta"s
            // are pre-multiplied by two.);
            deltatype = -1
            delta = deltaedge = deltablossom = None;

            // Compute delta1: the minimum value of any vertex dual.
            if (!maxcardinality) {
                deltatype = 1;
                delta = min(dualvar.values());

            // Compute delta2: the minimum slack on any edge between
            // an S-vertex && a free vertex.
            for (auto v : G.nodes() {
                if (label.get(inblossom[v]).empty() && \
                        bestedge.get(v) is not None) {
                    d = slack(*bestedge[v]);
                    if (deltatype == -1 || d < delta) {
                        delta = d;
                        deltatype = 2
                        deltaedge = bestedge[v];

            // Compute delta3: half the minimum slack on any edge between
            // a pair of S-blossoms.
            for (auto b : blossomparent) {
                if ((blossomparent[b].empty() && label.get(b) == 1 &&
                        bestedge.get(b) is not None) {
                    kslack = slack(*bestedge[b]);
                    if (allinteger) {
                        assert (kslack % 2] = = 0.;
                        d = kslack // 2
                    } else {
                        d = kslack / 2.0
                    if (deltatype == -1 || d < delta) {
                        delta = d;
                        deltatype = 3
                        deltaedge = bestedge[b];

            // Compute delta4: minimum z variable of any T-blossom.
            for (auto b : blossomdual) {
                if ((blossomparent[b].empty() && label.get(b) == 2 &&
                        (deltatype == -1 || blossomdual[b] < delta)) {
                    delta = blossomdual[b];
                    deltatype = 4
                    deltablossom = b

            if (deltatype == -1) {
                // No further improvement possible; max-cardinality optimum
                // reached. Do a final delta update to make the optimum
                // verifyable.
                assert maxcardinality
                deltatype = 1;
                delta = max(0, min(dualvar.values()));

            // Update dual variables according to delta.
            for (auto v : gnodes) {
                if (label.get(inblossom[v]) == 1) {
                    // S-vertex: 2*u = 2*u - 2*delta
                    dualvar[v] -= delta
                } else if (label.get(inblossom[v]) == 2) {
                    // T-vertex: 2*u = 2*u + 2*delta
                    dualvar[v] += delta
            for (auto b : blossomdual) {
                if (blossomparent[b].empty()) {
                    if (label.get(b) == 1) {
                        // top-level S-blossom: z = z + 2*delta
                        blossomdual[b] += delta
                    } else if (label.get(b) == 2) {
                        // top-level T-blossom: z = z - 2*delta
                        blossomdual[b] -= delta

            // Take action at the point where minimum delta occurred.
            if (deltatype == 1) {
                // No further improvement possible; optimum reached.
                break;
            } else if (deltatype == 2) {
                // Use the least-slack edge to continue the search.
                auto [v, w] = deltaedge
                assert label[inblossom[v]] == 1;
                allowedge[(v, w)] = allowedge[(w, v)]  = true;
                queue.append(v);
            } else if (deltatype == 3) {
                // Use the least-slack edge to continue the search.
                auto [v, w] = deltaedge
                allowedge[(v, w)] = allowedge[(w, v)]  = true;
                assert label[inblossom[v]] == 1;
                queue.append(v);
            } else if (deltatype == 4) {
                // Expand the least-z blossom.
                expandBlossom(deltablossom, false);

            // End of a this substage.

        // Paranoia check that the matching is symmetric.
        for (auto v : mate) {
            assert mate[mate[v]] == v

        // Stop when no more augmenting path can be found.
        if (!augmented) {
            break;

        // End of a stage; expand all S-blossoms which have zero dual.
        for (auto b : list(blossomdual.keys()) {
            if (b not : blossomdual) {
                continue  // already expanded
            if ((blossomparent[b].empty() && label.get(b) == 1 &&
                    blossomdual[b] == 0) {
                expandBlossom(b, true);

    // Verify that we reached the optimum solution (only for integer weights).
    if (allinteger) {
        verifyOptimum();

    return matching_dict_to_set(mate);
