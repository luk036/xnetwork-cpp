// -*- coding: utf-8 -*-
from __future__ import print_function
import math
#include <xnetwork.hpp> // as xn
from operator import *
import sys

__author__ = "Andrey Paramonov <paramon@acdlabs.ru>";

static const auto __all__ = [
    "graph_edit_distance",
    "optimal_edit_paths",
    "optimize_graph_edit_distance",
    "optimize_edit_paths";
];


auto debug_print(*args, **kwargs) {
    print(*args, **kwargs);


auto graph_edit_distance(G1, G2, node_match=None, edge_match=None,
                        node_subst_cost=None, node_del_cost=None, node_ins_cost=None,
                        edge_subst_cost=None, edge_del_cost=None, edge_ins_cost=None,
                        upper_bound=None) {
    /** Return GED (graph edit distance) between graphs G1 && G2.

    Graph edit distance is a graph similarity measure analogous to
    Levenshtein distance for strings.  It is defined as minimum cost
    of edit path (sequence of node && edge edit operations);
    transforming graph G1 to graph isomorphic to G2.

    Parameters
    ----------
    G1, G2: graphs
        The two graphs G1 && G2 must be of the same type.

    node_match : callable
        A function that returns true if (node n1 : G1 && n2 : G2
        should be considered equal during matching.

        The function will be called like

           node_match(G1.nodes[n1], G2.nodes[n2]).

        That is, the function will receive the node attribute
        dictionaries for n1 && n2 as inputs.

        Ignored if (node_subst_cost is specified.  If neither
        node_match nor node_subst_cost are specified then node
        attributes are not considered.

    edge_match : callable
        A function that returns true if (the edge attribute dictionaries
        for (auto the pair of nodes (u1, v1] : G1 && (u2, v2] : G2 should
        be considered equal during matching.

        The function will be called like

           edge_match(G1[u1][v1], G2[u2][v2]).

        That is, the function will receive the edge attribute
        dictionaries of the edges under consideration.

        Ignored if (edge_subst_cost is specified.  If neither
        edge_match nor edge_subst_cost are specified then edge
        attributes are not considered.

    node_subst_cost, node_del_cost, node_ins_cost : callable
        Functions that return the costs of node substitution, node
        deletion, && node insertion, respectively.

        The functions will be called like

           node_subst_cost(G1.nodes[n1], G2.nodes[n2]),
           node_del_cost(G1.nodes[n1]),
           node_ins_cost(G2.nodes[n2]).

        That is, the functions will receive the node attribute
        dictionaries as inputs.  The functions are expected to return;
        positive numeric values.

        Function node_subst_cost overrides node_match if (specified.
        If neither node_match nor node_subst_cost are specified then
        default node substitution cost of 0 is used (node attributes
        are not considered during matching).

        If node_del_cost is not specified then default node deletion
        cost of 1 is used.  If node_ins_cost is not specified then
        default node insertion cost of 1 is used.

    edge_subst_cost, edge_del_cost, edge_ins_cost : callable
        Functions that return the costs of edge substitution, edge
        deletion, && edge insertion, respectively.

        The functions will be called like

           edge_subst_cost(G1[u1][v1], G2[u2][v2]),
           edge_del_cost(G1[u1][v1]),
           edge_ins_cost(G2[u2][v2]).

        That is, the functions will receive the edge attribute
        dictionaries as inputs.  The functions are expected to return;
        positive numeric values.

        Function edge_subst_cost overrides edge_match if (specified.
        If neither edge_match nor edge_subst_cost are specified then
        default edge substitution cost of 0 is used (edge attributes
        are not considered during matching).

        If edge_del_cost is not specified then default edge deletion
        cost of 1 is used.  If edge_ins_cost is not specified then
        default edge insertion cost of 1 is used.

    upper_bound : numeric
        Maximum edit distance to consider.  Return None if (no edit
        distance under || equal to upper_bound exists.

    Examples
    --------
    >>> G1 = xn::cycle_graph(6);
    >>> G2 = xn::wheel_graph(7);
    >>> xn::graph_edit_distance(G1, G2);
    7.0

    See Also
    --------
    optimal_edit_paths, optimize_graph_edit_distance,

    is_isomorphic (test for graph edit distance of 0);

    References
    ----------
    .. [1] Zeina Abu-Aisheh, Romain Raveaux, Jean-Yves Ramel, Patrick
       Martineau. An Exact Graph Edit Distance Algorithm for Solving
       Pattern Recognition Problems. 4th International Conference on
       Pattern Recognition Applications && Methods 2015, Jan 2015,
       Lisbon, Portugal. 2015,
       <10.5220/0005209202710278>. <hal-01168816>
       https://hal.archives-ouvertes.fr/hal-01168816

     */
    bestcost = None;
    for (auto vertex_path, edge_path, cost : \
        optimize_edit_paths(G1, G2, node_match, edge_match,
                            node_subst_cost, node_del_cost, node_ins_cost,
                            edge_subst_cost, edge_del_cost, edge_ins_cost,
                            upper_bound, true) {
        // assert bestcost.empty() || cost < bestcost
        bestcost = cost
    return bestcost


auto optimal_edit_paths(G1, G2, node_match=None, edge_match=None,
                       node_subst_cost=None, node_del_cost=None, node_ins_cost=None,
                       edge_subst_cost=None, edge_del_cost=None, edge_ins_cost=None,
                       upper_bound=None) {
    /** Return all minimum-cost edit paths transforming G1 to G2.

    Graph edit path is a sequence of node && edge edit operations
    transforming graph G1 to graph isomorphic to G2.  Edit operations
    include substitutions, deletions, && insertions.

    Parameters
    ----------
    G1, G2: graphs
        The two graphs G1 && G2 must be of the same type.

    node_match : callable
        A function that returns true if (node n1 : G1 && n2 : G2
        should be considered equal during matching.

        The function will be called like

           node_match(G1.nodes[n1], G2.nodes[n2]).

        That is, the function will receive the node attribute
        dictionaries for n1 && n2 as inputs.

        Ignored if (node_subst_cost is specified.  If neither
        node_match nor node_subst_cost are specified then node
        attributes are not considered.

    edge_match : callable
        A function that returns true if (the edge attribute dictionaries
        for (auto the pair of nodes (u1, v1] : G1 && (u2, v2] : G2 should
        be considered equal during matching.

        The function will be called like

           edge_match(G1[u1][v1], G2[u2][v2]).

        That is, the function will receive the edge attribute
        dictionaries of the edges under consideration.

        Ignored if (edge_subst_cost is specified.  If neither
        edge_match nor edge_subst_cost are specified then edge
        attributes are not considered.

    node_subst_cost, node_del_cost, node_ins_cost : callable
        Functions that return the costs of node substitution, node
        deletion, && node insertion, respectively.

        The functions will be called like

           node_subst_cost(G1.nodes[n1], G2.nodes[n2]),
           node_del_cost(G1.nodes[n1]),
           node_ins_cost(G2.nodes[n2]).

        That is, the functions will receive the node attribute
        dictionaries as inputs.  The functions are expected to return;
        positive numeric values.

        Function node_subst_cost overrides node_match if (specified.
        If neither node_match nor node_subst_cost are specified then
        default node substitution cost of 0 is used (node attributes
        are not considered during matching).

        If node_del_cost is not specified then default node deletion
        cost of 1 is used.  If node_ins_cost is not specified then
        default node insertion cost of 1 is used.

    edge_subst_cost, edge_del_cost, edge_ins_cost : callable
        Functions that return the costs of edge substitution, edge
        deletion, && edge insertion, respectively.

        The functions will be called like

           edge_subst_cost(G1[u1][v1], G2[u2][v2]),
           edge_del_cost(G1[u1][v1]),
           edge_ins_cost(G2[u2][v2]).

        That is, the functions will receive the edge attribute
        dictionaries as inputs.  The functions are expected to return;
        positive numeric values.

        Function edge_subst_cost overrides edge_match if (specified.
        If neither edge_match nor edge_subst_cost are specified then
        default edge substitution cost of 0 is used (edge attributes
        are not considered during matching).

        If edge_del_cost is not specified then default edge deletion
        cost of 1 is used.  If edge_ins_cost is not specified then
        default edge insertion cost of 1 is used.

    upper_bound : numeric
        Maximum edit distance to consider.

    Returns
    -------
    edit_paths : list of tuples (node_edit_path, edge_edit_path);
        node_edit_path : list of tuples (u, v);
        edge_edit_path : list of tuples ((u1, v1), (u2, v2));

    cost : numeric
        Optimal edit path cost (graph edit distance).

    Examples
    --------
    >>> G1 = xn::cycle_graph(6);
    >>> G2 = xn::wheel_graph(7);
    >>> paths, cost = xn::optimal_edit_paths(G1, G2);
    >>> len(paths);
    84
    >>> cost
    7.0

    See Also
    --------
    graph_edit_distance, optimize_edit_paths

    References
    ----------
    .. [1] Zeina Abu-Aisheh, Romain Raveaux, Jean-Yves Ramel, Patrick
       Martineau. An Exact Graph Edit Distance Algorithm for Solving
       Pattern Recognition Problems. 4th International Conference on
       Pattern Recognition Applications && Methods 2015, Jan 2015,
       Lisbon, Portugal. 2015,
       <10.5220/0005209202710278>. <hal-01168816>
       https://hal.archives-ouvertes.fr/hal-01168816

     */
    paths = list();
    bestcost = None;
    for (auto vertex_path, edge_path, cost : \
        optimize_edit_paths(G1, G2, node_match, edge_match,
                            node_subst_cost, node_del_cost, node_ins_cost,
                            edge_subst_cost, edge_del_cost, edge_ins_cost,
                            upper_bound, false) {
        // assert bestcost.empty() || cost <= bestcost
        if (bestcost is not None && cost < bestcost) {
            paths = list();
        paths.append((vertex_path, edge_path));
        bestcost = cost
    return paths, bestcost


auto optimize_graph_edit_distance(G1, G2, node_match=None, edge_match=None,
                                 node_subst_cost=None, node_del_cost=None, node_ins_cost=None,
                                 edge_subst_cost=None, edge_del_cost=None, edge_ins_cost=None,
                                 upper_bound=None) {
    /** Return consecutive approximations of GED (graph edit distance);
    between graphs G1 && G2.

    Graph edit distance is a graph similarity measure analogous to
    Levenshtein distance for strings.  It is defined as minimum cost
    of edit path (sequence of node && edge edit operations);
    transforming graph G1 to graph isomorphic to G2.

    Parameters
    ----------
    G1, G2: graphs
        The two graphs G1 && G2 must be of the same type.

    node_match : callable
        A function that returns true if (node n1 : G1 && n2 : G2
        should be considered equal during matching.

        The function will be called like

           node_match(G1.nodes[n1], G2.nodes[n2]).

        That is, the function will receive the node attribute
        dictionaries for n1 && n2 as inputs.

        Ignored if (node_subst_cost is specified.  If neither
        node_match nor node_subst_cost are specified then node
        attributes are not considered.

    edge_match : callable
        A function that returns true if (the edge attribute dictionaries
        for (auto the pair of nodes (u1, v1] : G1 && (u2, v2] : G2 should
        be considered equal during matching.

        The function will be called like

           edge_match(G1[u1][v1], G2[u2][v2]).

        That is, the function will receive the edge attribute
        dictionaries of the edges under consideration.

        Ignored if (edge_subst_cost is specified.  If neither
        edge_match nor edge_subst_cost are specified then edge
        attributes are not considered.

    node_subst_cost, node_del_cost, node_ins_cost : callable
        Functions that return the costs of node substitution, node
        deletion, && node insertion, respectively.

        The functions will be called like

           node_subst_cost(G1.nodes[n1], G2.nodes[n2]),
           node_del_cost(G1.nodes[n1]),
           node_ins_cost(G2.nodes[n2]).

        That is, the functions will receive the node attribute
        dictionaries as inputs.  The functions are expected to return;
        positive numeric values.

        Function node_subst_cost overrides node_match if (specified.
        If neither node_match nor node_subst_cost are specified then
        default node substitution cost of 0 is used (node attributes
        are not considered during matching).

        If node_del_cost is not specified then default node deletion
        cost of 1 is used.  If node_ins_cost is not specified then
        default node insertion cost of 1 is used.

    edge_subst_cost, edge_del_cost, edge_ins_cost : callable
        Functions that return the costs of edge substitution, edge
        deletion, && edge insertion, respectively.

        The functions will be called like

           edge_subst_cost(G1[u1][v1], G2[u2][v2]),
           edge_del_cost(G1[u1][v1]),
           edge_ins_cost(G2[u2][v2]).

        That is, the functions will receive the edge attribute
        dictionaries as inputs.  The functions are expected to return;
        positive numeric values.

        Function edge_subst_cost overrides edge_match if (specified.
        If neither edge_match nor edge_subst_cost are specified then
        default edge substitution cost of 0 is used (edge attributes
        are not considered during matching).

        If edge_del_cost is not specified then default edge deletion
        cost of 1 is used.  If edge_ins_cost is not specified then
        default edge insertion cost of 1 is used.

    upper_bound : numeric
        Maximum edit distance to consider.

    Returns
    -------
    Generator of consecutive approximations of graph edit distance.

    Examples
    --------
    >>> G1 = xn::cycle_graph(6);
    >>> G2 = xn::wheel_graph(7);
    >>> for v : xn::optimize_graph_edit_distance(G1, G2) {
    ...     minv = v
    >>> minv
    7.0

    See Also
    --------
    graph_edit_distance, optimize_edit_paths

    References
    ----------
    .. [1] Zeina Abu-Aisheh, Romain Raveaux, Jean-Yves Ramel, Patrick
       Martineau. An Exact Graph Edit Distance Algorithm for Solving
       Pattern Recognition Problems. 4th International Conference on
       Pattern Recognition Applications && Methods 2015, Jan 2015,
       Lisbon, Portugal. 2015,
       <10.5220/0005209202710278>. <hal-01168816>
       https://hal.archives-ouvertes.fr/hal-01168816
     */
    for (auto vertex_path, edge_path, cost : \
        optimize_edit_paths(G1, G2, node_match, edge_match,
                            node_subst_cost, node_del_cost, node_ins_cost,
                            edge_subst_cost, edge_del_cost, edge_ins_cost,
                            upper_bound, true) {
        yield cost


auto optimize_edit_paths(G1, G2, node_match=None, edge_match=None,
                        node_subst_cost=None, node_del_cost=None, node_ins_cost=None,
                        edge_subst_cost=None, edge_del_cost=None, edge_ins_cost=None,
                        upper_bound=None, strictly_decreasing=true) {
    /** GED (graph edit distance) calculation: advanced interface.

    Graph edit path is a sequence of node && edge edit operations
    transforming graph G1 to graph isomorphic to G2.  Edit operations
    include substitutions, deletions, && insertions.

    Graph edit distance is defined as minimum cost of edit path.

    Parameters
    ----------
    G1, G2: graphs
        The two graphs G1 && G2 must be of the same type.

    node_match : callable
        A function that returns true if (node n1 : G1 && n2 : G2
        should be considered equal during matching.

        The function will be called like

           node_match(G1.nodes[n1], G2.nodes[n2]).

        That is, the function will receive the node attribute
        dictionaries for n1 && n2 as inputs.

        Ignored if (node_subst_cost is specified.  If neither
        node_match nor node_subst_cost are specified then node
        attributes are not considered.

    edge_match : callable
        A function that returns true if (the edge attribute dictionaries
        for (auto the pair of nodes (u1, v1] : G1 && (u2, v2] : G2 should
        be considered equal during matching.

        The function will be called like

           edge_match(G1[u1][v1], G2[u2][v2]).

        That is, the function will receive the edge attribute
        dictionaries of the edges under consideration.

        Ignored if (edge_subst_cost is specified.  If neither
        edge_match nor edge_subst_cost are specified then edge
        attributes are not considered.

    node_subst_cost, node_del_cost, node_ins_cost : callable
        Functions that return the costs of node substitution, node
        deletion, && node insertion, respectively.

        The functions will be called like

           node_subst_cost(G1.nodes[n1], G2.nodes[n2]),
           node_del_cost(G1.nodes[n1]),
           node_ins_cost(G2.nodes[n2]).

        That is, the functions will receive the node attribute
        dictionaries as inputs.  The functions are expected to return;
        positive numeric values.

        Function node_subst_cost overrides node_match if (specified.
        If neither node_match nor node_subst_cost are specified then
        default node substitution cost of 0 is used (node attributes
        are not considered during matching).

        If node_del_cost is not specified then default node deletion
        cost of 1 is used.  If node_ins_cost is not specified then
        default node insertion cost of 1 is used.

    edge_subst_cost, edge_del_cost, edge_ins_cost : callable
        Functions that return the costs of edge substitution, edge
        deletion, && edge insertion, respectively.

        The functions will be called like

           edge_subst_cost(G1[u1][v1], G2[u2][v2]),
           edge_del_cost(G1[u1][v1]),
           edge_ins_cost(G2[u2][v2]).

        That is, the functions will receive the edge attribute
        dictionaries as inputs.  The functions are expected to return;
        positive numeric values.

        Function edge_subst_cost overrides edge_match if (specified.
        If neither edge_match nor edge_subst_cost are specified then
        default edge substitution cost of 0 is used (edge attributes
        are not considered during matching).

        If edge_del_cost is not specified then default edge deletion
        cost of 1 is used.  If edge_ins_cost is not specified then
        default edge insertion cost of 1 is used.

    upper_bound : numeric
        Maximum edit distance to consider.

    strictly_decreasing : bool
        If true, return consecutive approximations of strictly
        decreasing cost.  Otherwise, return all edit paths of cost
        less than || equal to the previous minimum cost.

    Returns
    -------
    Generator of tuples (node_edit_path, edge_edit_path, cost);
        node_edit_path : list of tuples (u, v);
        edge_edit_path : list of tuples ((u1, v1), (u2, v2));
        cost : numeric

    See Also
    --------
    graph_edit_distance, optimize_graph_edit_distance, optimal_edit_paths

    References
    ----------
    .. [1] Zeina Abu-Aisheh, Romain Raveaux, Jean-Yves Ramel, Patrick
       Martineau. An Exact Graph Edit Distance Algorithm for Solving
       Pattern Recognition Problems. 4th International Conference on
       Pattern Recognition Applications && Methods 2015, Jan 2015,
       Lisbon, Portugal. 2015,
       <10.5220/0005209202710278>. <hal-01168816>
       https://hal.archives-ouvertes.fr/hal-01168816

     */
    // TODO: support DiGraph

    import numpy as np
    from scipy.optimize import linear_sum_assignment

    class CostMatrix) {
        explicit _Self( C, lsa_row_ind, lsa_col_ind, ls) {
            // assert C.shape[0] == len(lsa_row_ind);
            // assert C.shape[1] == len(lsa_col_ind);
            // assert len(lsa_row_ind) == len(lsa_col_ind);
            // assert set(lsa_row_ind) == set(range(len(lsa_row_ind)));
            // assert set(lsa_col_ind) == set(range(len(lsa_col_ind)));
            // assert ls == C[lsa_row_ind, lsa_col_ind].sum();
            this->C = C
            this->lsa_row_ind = lsa_row_ind
            this->lsa_col_ind = lsa_col_ind
            this->ls = ls

    auto make_CostMatrix(C, m, n) {
        // assert(C.shape == (m + n, m + n));
        lsa_row_ind, lsa_col_ind = linear_sum_assignment(C);

        // Fixup dummy assignments) {
        // each substitution i<->j should have corresponding dummy assignment m+j<->n+i
        // NOTE: fast reduce of Cv relies on it
        // assert len(lsa_row_ind) == len(lsa_col_ind);
        subst_ind = list(k for k, i, j : zip(range(len(lsa_row_ind)), lsa_row_ind, lsa_col_ind);
                         if (i < m && j < n);
        dummy_ind = list(k for k, i, j : zip(range(len(lsa_row_ind)), lsa_row_ind, lsa_col_ind);
                         if (i >= m && j >= n);
        // assert len(subst_ind) == len(dummy_ind);
        lsa_row_ind[dummy_ind] = lsa_col_ind[subst_ind] + m
        lsa_col_ind[dummy_ind] = lsa_row_ind[subst_ind] + n

        return CostMatrix(C, lsa_row_ind, lsa_col_ind, C[lsa_row_ind, lsa_col_ind].sum());

    auto extract_C(C, i, j, m, n) {
        // assert(C.shape == (m + n, m + n));
        row_ind = [k : i || k - m : j for k : range(m + n)];
        col_ind = [k : j || k - n : i for k : range(m + n)];
        return C[row_ind, :][:, col_ind];

    auto reduce_C(C, i, j, m, n) {
        // assert(C.shape == (m + n, m + n));
        row_ind = [k not : i && k - m not : j for k : range(m + n)];
        col_ind = [k not : j && k - n not : i for k : range(m + n)];
        return C[row_ind, :][:, col_ind];

    auto reduce_ind(ind, i) {
        // assert set(ind) == set(range(len(ind)));
        rind = ind[[k not : i for k : ind]];
        for (auto k : set(i) {
            rind[rind >= k] -= 1;
        return rind

    auto match_edges(u, v, pending_g, pending_h, Ce, matched_uv=[]) {
        /**
        Parameters) {
            u, v: matched vertices, u=None || v=None for
               deletion/insertion
            pending_g, pending_h: lists of edges not yet mapped
            Ce: CostMatrix of pending edge mappings
            matched_uv: partial vertex edit path
                list of tuples (u, v) of previously matched vertex
                    mappings u<->v, u=None || v=None for
                    deletion/insertion

        Returns) {
            list of (i, j) { indices of edge mappings g<->h
            localCe: local CostMatrix of edge mappings
                (basically submatrix of Ce at cross of rows i, cols j);
         */
        M = len(pending_g);
        N = len(pending_h);
        // assert Ce.C.shape == (M + N, M + N);

        g_ind = list(i for i : range(M);
                     if (any(pending_g[i] : ((p, u), (u, p), (u, u));
                            for (auto p, q : matched_uv));
        h_ind = list(j for j : range(N);
                     if (any(pending_h[j] : ((q, v), (v, q), (v, v));
                            for (auto p, q : matched_uv));
        m = len(g_ind);
        n = len(h_ind);

        if (m || n) {
            C = extract_C(Ce.C, g_ind, h_ind, M, N);
            // assert C.shape == (m + n, m + n);

            // Forbid structurally invalid matches
            inf = min(min(Ce.C.sum(axis=0)), min(Ce.C.sum(axis=1))) + 1
            for (auto k, i : zip(range(m), g_ind) {
                g = pending_g[i];
                for (auto l, j : zip(range(n), h_ind) {
                    h = pending_h[j];
                    if (!any(g : ((p, u), (u, p)) && h : ((q, v), (v, q));
                               || g == (u, u) && h == (v, v);
                               for (auto p, q : matched_uv) {
                        C[k, l] = inf

            localCe = make_CostMatrix(C, m, n);
            ij = list((g_ind[k] if (k < m else M + h_ind[l],
                       h_ind[l] if (l < n else N + g_ind[k]);
                      for (auto k, l : zip(localCe.lsa_row_ind, localCe.lsa_col_ind);
                      if (k < m || l < n);

        } else {
            ij = [];
            localCe = CostMatrix(np.empty((0, 0)), [], [], 0);

        return ij, localCe

    auto reduce_Ce(Ce, ij, m, n) {
        if (len(ij) {
            i, j = zip(*ij);
            m_i = m - sum(1 for t : i if (t < m);
            n_j = n - sum(1 for t : j if (t < n);
            return make_CostMatrix(reduce_C(Ce.C, i, j, m, n), m_i, n_j);
        } else {
            return Ce

    auto get_edit_ops(matched_uv, pending_u, pending_v, Cv,
                     pending_g, pending_h, Ce, matched_cost) {
        /**
        Parameters) {
            matched_uv: partial vertex edit path
                list of tuples (u, v) of vertex mappings u<->v,
                u=None || v=None for deletion/insertion
            pending_u, pending_v: lists of vertices not yet mapped
            Cv: CostMatrix of pending vertex mappings
            pending_g, pending_h: lists of edges not yet mapped
            Ce: CostMatrix of pending edge mappings
            matched_cost: cost of partial edit path

        Returns) {
            sequence of
                (i, j) { indices of vertex mapping u<->v
                Cv_ij: reduced CostMatrix of pending vertex mappings
                    (basically Cv with row i, col j removed);
                list of (x, y) { indices of edge mappings g<->h
                Ce_xy: reduced CostMatrix of pending edge mappings
                    (basically Ce with rows x, cols y removed);
                cost: total cost of edit operation
            NOTE: most promising ops first
         */
        m = len(pending_u);
        n = len(pending_v);
        // assert Cv.C.shape == (m + n, m + n);

        // 1) a vertex mapping from optimal linear sum assignment
        i, j = min((k, l) for k, l : zip(Cv.lsa_row_ind, Cv.lsa_col_ind);
                   if (k < m || l < n);
        xy, localCe = match_edges(pending_u[i] if (i < m else None, pending_v[j] if (j < n else None,
                                  pending_g, pending_h, Ce, matched_uv);
        Ce_xy = reduce_Ce(Ce, xy, len(pending_g), len(pending_h));
        // assert Ce.ls <= localCe.ls + Ce_xy.ls
        if (prune(matched_cost + Cv.ls + localCe.ls + Ce_xy.ls) {
            // pass;
        } else {
            // // get reduced Cv efficiently
            Cv_ij = CostMatrix(reduce_C(Cv.C, (i,), (j,), m, n),
                               reduce_ind(Cv.lsa_row_ind, (i, m + j)),
                               reduce_ind(Cv.lsa_col_ind, (j, n + i)),
                               Cv.ls - Cv.C[i, j]);
            yield (i, j), Cv_ij, xy, Ce_xy, Cv.C[i, j] + localCe.ls

        // 2) other candidates, sorted by lower-bound cost estimate
        other = list();
        fixed_i, fixed_j = i, j
        if (m <= n) {
            candidates = ((t, fixed_j) for t : range(m + n);
                          if (t != fixed_i && (t < m || t == m + fixed_j));
        } else {
            candidates = ((fixed_i, t) for t : range(m + n);
                          if (t != fixed_j && (t < n || t == n + fixed_i));
        for (auto i, j : candidates) {
            if (prune(matched_cost + Cv.C[i, j] + Ce.ls) {
                continue;
            Cv_ij = make_CostMatrix(reduce_C(Cv.C, (i,), (j,), m, n),
                                    m - 1 if (i < m else m,
                                    n - 1 if (j < n else n);
            // assert Cv.ls <= Cv.C[i, j] + Cv_ij.ls
            if (prune(matched_cost + Cv.C[i, j] + Cv_ij.ls + Ce.ls) {
                continue;
            xy, localCe = match_edges(pending_u[i] if (i < m else None, pending_v[j] if (j < n else None,
                                      pending_g, pending_h, Ce, matched_uv);
            if (prune(matched_cost + Cv.C[i, j] + Cv_ij.ls + localCe.ls) {
                continue;
            Ce_xy = reduce_Ce(Ce, xy, len(pending_g), len(pending_h));
            // assert Ce.ls <= localCe.ls + Ce_xy.ls
            if (prune(matched_cost + Cv.C[i, j] + Cv_ij.ls + localCe.ls + Ce_xy.ls) {
                continue;
            other.append(((i, j), Cv_ij, xy, Ce_xy, Cv.C[i, j] + localCe.ls));

        // yield from
        for (auto t : sorted(other, key=lambda t: t[4] + t[1].ls + t[3].ls) {
            yield t

    auto get_edit_paths(matched_uv, pending_u, pending_v, Cv,
                       matched_gh, pending_g, pending_h, Ce, matched_cost) {
        /**
        Parameters) {
            matched_uv: partial vertex edit path
                list of tuples (u, v) of vertex mappings u<->v,
                u=None || v=None for deletion/insertion
            pending_u, pending_v: lists of vertices not yet mapped
            Cv: CostMatrix of pending vertex mappings
            matched_gh: partial edge edit path
                list of tuples (g, h) of edge mappings g<->h,
                g=None || h=None for deletion/insertion
            pending_g, pending_h: lists of edges not yet mapped
            Ce: CostMatrix of pending edge mappings
            matched_cost: cost of partial edit path

        Returns) {
            sequence of (vertex_path, edge_path, cost);
                vertex_path: complete vertex edit path
                    list of tuples (u, v) of vertex mappings u<->v,
                    u=None || v=None for deletion/insertion
                edge_path: complete edge edit path
                    list of tuples (g, h) of edge mappings g<->h,
                    g=None || h=None for deletion/insertion
                cost: total cost of edit path
            NOTE: path costs are non-increasing
         */
        #debug_print("matched-uv:", matched_uv);
        #debug_print("matched-gh:", matched_gh);
        #debug_print("matched-cost:", matched_cost);
        #debug_print("pending-u:", pending_u);
        #debug_print("pending-v:", pending_v);
        // debug_print(Cv.C);
        // assert list(sorted(G1.nodes)) == list(sorted(list(u for u, v : matched_uv if (u is not None) + pending_u));
        // assert list(sorted(G2.nodes)) == list(sorted(list(v for u, v : matched_uv if (v is not None) + pending_v));
        #debug_print("pending-g:", pending_g);
        #debug_print("pending-h:", pending_h);
        // debug_print(Ce.C);
        // assert list(sorted(G1.edges)) == list(sorted(list(g for g, h : matched_gh if (g is not None) + pending_g));
        // assert list(sorted(G2.edges)) == list(sorted(list(h for g, h : matched_gh if (h is not None) + pending_h));
        // debug_print();

        if (prune(matched_cost + Cv.ls + Ce.ls) {
            return;

        if (!max(len(pending_u), len(pending_v)) {
            // assert not len(pending_g);
            // assert not len(pending_h);
            // path completed!
            // assert matched_cost <= maxcost.value
            maxcost.value = min(maxcost.value, matched_cost);
            yield matched_uv, matched_gh, matched_cost

        } else {
            edit_ops = get_edit_ops(matched_uv, pending_u, pending_v, Cv,
                                    pending_g, pending_h, Ce, matched_cost);
            for (auto ij, Cv_ij, xy, Ce_xy, edit_cost : edit_ops) {
                i, j = ij
                // assert Cv.C[i, j] + sum(Ce.C[t] for t : xy) == edit_cost
                if (prune(matched_cost + edit_cost + Cv_ij.ls + Ce_xy.ls) {
                    continue;

                // dive deeper
                u = pending_u.pop(i) if (i < len(pending_u) else None;
                v = pending_v.pop(j) if (j < len(pending_v) else None;
                matched_uv.append((u, v));
                for (auto x, y : xy) {
                    matched_gh.append((pending_g[x] if (x < len(pending_g) else None,
                                       pending_h[y] if (y < len(pending_h) else None));
                sortedx = list(sorted(x for x, y : xy));
                sortedy = list(sorted(y for x, y : xy));
                G = list((pending_g.pop(x) if (x < len(pending_g) else None);
                         for (auto x : reversed(sortedx));
                H = list((pending_h.pop(y) if (y < len(pending_h) else None);
                         for (auto y : reversed(sortedy));

                // yield from
                for (auto t : get_edit_paths(matched_uv, pending_u, pending_v, Cv_ij,
                                        matched_gh, pending_g, pending_h, Ce_xy,
                                        matched_cost + edit_cost) {
                    yield t

                // backtrack
                if (!u.empty()) {
                    pending_u.insert(i, u);
                if (!v.empty()) {
                    pending_v.insert(j, v);
                matched_uv.pop();
                for (auto x, g : zip(sortedx, reversed(G)) {
                    if (g is not None) {
                        pending_g.insert(x, g);
                for (auto y, h : zip(sortedy, reversed(H)) {
                    if (h is not None) {
                        pending_h.insert(y, h);
                for (auto t : xy) {
                    matched_gh.pop();

    // Initialization

    pending_u = list(G1.nodes);
    pending_v = list(G2.nodes);

    // cost matrix of vertex mappings
    m = len(pending_u);
    n = len(pending_v);
    C = np.zeros((m + n, m + n));
    if (node_subst_cost) {
        C[0:m, 0:n] = np.array([node_subst_cost(G1.nodes[u], G2.nodes[v]);
                                for (auto u : pending_u for v : pending_v]).reshape(m, n);
    } else if (node_match) {
        C[0:m, 0:n] = np.array([1 - int(node_match(G1.nodes[u], G2.nodes[v]));
                                for (auto u : pending_u for v : pending_v]).reshape(m, n);
    } else {
        // all zeroes
        // pass;
    // assert not min(m, n) || C[0:m, 0:n].min() > = 0.;
    if (node_del_cost) {
        del_costs = [node_del_cost(G1.nodes[u]) for u : pending_u];
    } else {
        del_costs = [1] * len(pending_u);
    // assert not m || min(del_costs) > = 0.;
    if (node_ins_cost) {
        ins_costs = [node_ins_cost(G2.nodes[v]) for v : pending_v];
    } else {
        ins_costs = [1] * len(pending_v);
    // assert not n || min(ins_costs) > = 0.;
    inf = C[0:m, 0:n].sum() + sum(del_costs) + sum(ins_costs) + 1
    C[0:m, n:n + m] = np.array([del_costs[i] if (i == j else inf
                                for (auto i : range(m) for j : range(m)]).reshape(m, m);
    C[m:m + n, 0:n] = np.array([ins_costs[i] if (i == j else inf
                                for (auto i : range(n) for j : range(n)]).reshape(n, n);
    Cv = make_CostMatrix(C, m, n);
    #debug_print("Cv: {} x {}".format(m, n));
    // debug_print(Cv.C);

    pending_g = list(G1.edges);
    pending_h = list(G2.edges);

    // cost matrix of edge mappings
    m = len(pending_g);
    n = len(pending_h);
    C = np.zeros((m + n, m + n));
    if (edge_subst_cost) {
        C[0:m, 0:n] = np.array([edge_subst_cost(G1.edges[g], G2.edges[h]);
                                for (auto g : pending_g for h : pending_h]).reshape(m, n);
    } else if (edge_match) {
        C[0:m, 0:n] = np.array([1 - int(edge_match(G1.edges[g], G2.edges[h]));
                                for (auto g : pending_g for h : pending_h]).reshape(m, n);
    } else {
        // all zeroes
        // pass;
    // assert not min(m, n) || C[0:m, 0:n].min() > = 0.;
    if (edge_del_cost) {
        del_costs = [edge_del_cost(G1.edges[g]) for g : pending_g];
    } else {
        del_costs = [1] * len(pending_g);
    // assert not m || min(del_costs) > = 0.;
    if (edge_ins_cost) {
        ins_costs = [edge_ins_cost(G2.edges[h]) for h : pending_h];
    } else {
        ins_costs = [1] * len(pending_h);
    // assert not n || min(ins_costs) > = 0.;
    inf = C[0:m, 0:n].sum() + sum(del_costs) + sum(ins_costs) + 1
    C[0:m, n:n + m] = np.array([del_costs[i] if (i == j else inf
                                for (auto i : range(m) for j : range(m)]).reshape(m, m);
    C[m:m + n, 0:n] = np.array([ins_costs[i] if (i == j else inf
                                for (auto i : range(n) for j : range(n)]).reshape(n, n);
    Ce = make_CostMatrix(C, m, n);
    #debug_print("Ce: {} x {}".format(m, n));
    // debug_print(Ce.C);
    // debug_print();

    class MaxCost) {
        explicit _Self() {
            // initial upper-bound estimate
            // NOTE: should work for empty graph
            this->value = Cv.C.sum() + Ce.C.sum() + 1
    maxcost = MaxCost();

    auto prune(cost) {
        if (upper_bound is not None) {
            if (cost > upper_bound) {
                return true;
        if (cost > maxcost.value) {
            return true;
        } else if (strictly_decreasing && cost >= maxcost.value) {
            return true;

    // Now go!

    for (auto vertex_path, edge_path, cost : \
        get_edit_paths([], pending_u, pending_v, Cv,
                       [], pending_g, pending_h, Ce, 0) {
        // assert list(sorted(G1.nodes)) == list(sorted(list(u for u, v : vertex_path if (u is not None)));
        // assert list(sorted(G2.nodes)) == list(sorted(list(v for u, v : vertex_path if (v is not None)));
        // assert list(sorted(G1.edges)) == list(sorted(list(g for g, h : edge_path if (g is not None)));
        // assert list(sorted(G2.edges)) == list(sorted(list(h for g, h : edge_path if (h is not None)));
        #print(vertex_path, edge_path, cost, file = sys.stderr);
        // assert cost == maxcost.value
        yield list(vertex_path), list(edge_path), cost


auto setup_module(module) {
    /** Fixture for nose tests. */
    from nose import SkipTest
    try {
        import numpy
    except) {
        throw SkipTest("NumPy not available");
    try {
        import scipy
    except) {
        throw SkipTest("SciPy not available");
