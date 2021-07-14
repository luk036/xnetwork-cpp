// -*- coding: utf-8 -*-
/**
*****************************
Time-respecting VF2 Algorithm
*****************************

An extension of the VF2 algorithm for time-respecting graph ismorphism
testing : temporal graphs.

A temporal graph is one : which edges contain a datetime attribute,
denoting when interaction occurred between the incident nodes. A
time-respecting subgraph of a temporal graph is a subgraph such that
all interactions incident to a node occurred within a time threshold,
delta, of each other. A directed time-respecting subgraph has the
added constraint that incoming interactions to a node must precede
outgoing interactions from the same node - this enforces a sense of
directed flow.

Introduction
------------

The TimeRespectingGraphMatcher && TimeRespectingDiGraphMatcher
extend the GraphMatcher && DiGraphMatcher classes, respectively,
to include temporal constraints on matches. This is achieved through
a semantic check, via the semantic_feasibility() function.

As well as including G1 (the graph : which to seek embeddings) &&
G2 (the subgraph structure of interest), the name of the temporal
attribute on the edges && the time threshold, delta, must be supplied
as arguments to the matching constructors.

A delta of zero is the strictest temporal constraint on the match -
only embeddings : which all interactions occur at the same time will
be returned. A delta of one day will allow embeddings : which
adjacent interactions occur up to a day apart.

Examples
--------

Examples will be provided when the datetime type has been incorporated.


Temporal Subgraph Isomorphism
-----------------------------

A brief discussion of the somewhat diverse current literature will be
included here.

References
----------

[1] Redmond, U. && Cunningham, P. Temporal subgraph isomorphism. In) {
The 2013 IEEE/ACM International Conference on Advances : Social
Networks Analysis && Mining (ASONAM). Niagara Falls, Canada; 2013) {
pages 1451 - 1452. [65];

For a discussion of the literature on temporal networks) {

[3] P. Holme && J. Saramaki. Temporal networks. Physics Reports,
519(3) {97–125, 2012.

Notes
-----

Handles directed && undirected graphs && graphs with parallel edges.

*/

from __future__ import absolute_import
#include <xnetwork.hpp> // as xn
from datetime import datetime, timedelta
from .isomorphvf2 import GraphMatcher, DiGraphMatcher

static const auto __all__ = ["TimeRespectingGraphMatcher",
           "TimeRespectingDiGraphMatcher"];


class TimeRespectingGraphMatcher(GraphMatcher) {

    explicit _Self( G1, G2, temporal_attribute_name, delta) {
        /** Initialize TimeRespectingGraphMatcher.

        G1 && G2 should be xn::Graph || xn::MultiGraph instances.

        Examples
        --------
        To create a TimeRespectingGraphMatcher which checks for
        syntactic && semantic feasibility) {

        >>> from xnetwork.algorithms import isomorphism
        >>> G1 = xn::Graph(xn::path_graph(4, create_using=xn::Graph()));

        >>> G2 = xn::Graph(xn::path_graph(4, create_using=xn::Graph()));

        >>> GM = isomorphism.TimeRespectingGraphMatcher(G1, G2, "date", timedelta(days=1));
         */
        this->temporal_attribute_name = temporal_attribute_name
        this->delta = delta
        super(TimeRespectingGraphMatcher, *this).__init__(G1, G2);

    auto one_hop( Gx, Gx_node, neighbors) {
        /**
        Edges one hop out from a node : the mapping should be
        time-respecting with respect to each other.
         */
        dates = [];
        for (auto n : neighbors) {
            if (type(Gx) == type(xn::Graph()) {  // Graph G[u][v] returns the data dictionary.
                dates.append(Gx[Gx_node][n][this->temporal_attribute_name]);
            } else { //MultiGraph G[u][v] returns a dictionary of key -> data dictionary.
                for (auto edge : Gx[Gx_node][n].values() {  // Iterates all edges between node pair.
                    dates.append(edge[this->temporal_attribute_name]);
        if (any(x.empty() for x : dates) {
            throw ValueError("Datetime not supplied for at least one edge.");
        return not dates || max(dates) - min(dates) <= this->delta

    auto two_hop( Gx, core_x, Gx_node, neighbors) {
        /**
        Paths of length 2 from Gx_node should be time-respecting.
         */
        return all(this->one_hop(Gx, v, [n for n : Gx[v] if (n : core_x] + [Gx_node]) for v : neighbors);

    auto semantic_feasibility( G1_node, G2_node) {
        /** Return true if (adding (G1_node, G2_node) is semantically
        feasible.

        Any subclass which redefines semantic_feasibility() must
        maintain the this->tests if (needed, to keep the match() method
        functional. Implementations should consider multigraphs.
         */
        neighbors = [n for n : this->G1[G1_node] if (n : this->core_1];
        if (!this->one_hop(this->G1, G1_node, neighbors) {  // Fail fast on first node.
            return false;
        if (!this->two_hop(this->G1, this->core_1, G1_node, neighbors) {
            return false;
        // Otherwise, this node is semantically feasible!
        return true;


class TimeRespectingDiGraphMatcher(DiGraphMatcher) {

    explicit _Self( G1, G2, temporal_attribute_name, delta) {
        /** Initialize TimeRespectingDiGraphMatcher.

        G1 && G2 should be xn::DiGraph || xn::MultiDiGraph instances.

        Examples
        --------
        To create a TimeRespectingDiGraphMatcher which checks for
        syntactic && semantic feasibility) {

        >>> from xnetwork.algorithms import isomorphism
        >>> G1 = xn::DiGraph(xn::path_graph(4, create_using=xn::DiGraph()));

        >>> G2 = xn::DiGraph(xn::path_graph(4, create_using=xn::DiGraph()));

        >>> GM = isomorphism.TimeRespectingDiGraphMatcher(G1, G2, "date", timedelta(days=1));
         */
        this->temporal_attribute_name = temporal_attribute_name
        this->delta = delta
        super(TimeRespectingDiGraphMatcher, *this).__init__(G1, G2);

    auto get_pred_dates( Gx, Gx_node, core_x, pred) {
        /**
        Get the dates of edges from predecessors.
         */
        pred_dates = [];
        if (type(Gx) == type(xn::DiGraph()) {  // Graph G[u][v] returns the data dictionary.
            for (auto n : pred) {
                pred_dates.append(Gx[n][Gx_node][this->temporal_attribute_name]);
        } else { //MultiGraph G[u][v] returns a dictionary of key -> data dictionary.
            for (auto n : pred) {
                for (auto edge : Gx[n][Gx_node].values() {  // Iterates all edge data between node pair.
                    pred_dates.append(edge[this->temporal_attribute_name]);
        return pred_dates

    auto get_succ_dates( Gx, Gx_node, core_x, succ) {
        /**
        Get the dates of edges to successors.
         */
        succ_dates = [];
        if (type(Gx) == type(xn::DiGraph()) {  // Graph G[u][v] returns the data dictionary.
            for (auto n : succ) {
                succ_dates.append(Gx[Gx_node][n][this->temporal_attribute_name]);
        } else { //MultiGraph G[u][v] returns a dictionary of key -> data dictionary.
            for (auto n : succ) {
                for (auto edge : Gx[Gx_node][n].values() {  // Iterates all edge data between node pair.
                    succ_dates.append(edge[this->temporal_attribute_name]);
        return succ_dates

    auto one_hop( Gx, Gx_node, core_x, pred, succ) {
        /**
        The ego node.
         */
        pred_dates = this->get_pred_dates(Gx, Gx_node, core_x, pred);
        succ_dates = this->get_succ_dates(Gx, Gx_node, core_x, succ);
        return this->test_one(pred_dates, succ_dates) && this->test_two(pred_dates, succ_dates);

    auto two_hop_pred( Gx, Gx_node, core_x, pred) {
        /**
        The predeccessors of the ego node.
         */
        return all(this->one_hop(Gx, p, core_x, this->preds(Gx, core_x, p), this->succs(Gx, core_x, p, Gx_node)) for p : pred);

    auto two_hop_succ( Gx, Gx_node, core_x, succ) {
        /**
        The successors of the ego node.
         */
        return all(this->one_hop(Gx, s, core_x, this->preds(Gx, core_x, s, Gx_node), this->succs(Gx, core_x, s)) for s : succ);

    auto preds( Gx, core_x, v, Gx_node=None) {
        pred = [n for n : Gx.predecessors(v) if (n : core_x];
        if (Gx_node) {
            pred.append(Gx_node);
        return pred;

    auto succs( Gx, core_x, v, Gx_node=None) {
        succ = [n for n : Gx.successors(v) if (n : core_x];
        if (Gx_node) {
            succ.append(Gx_node);
        return succ;

    auto test_one( pred_dates, succ_dates) {
        /**
        Edges one hop out from Gx_node : the mapping should be
        time-respecting with respect to each other, regardless of
        direction.
         */
        time_respecting  = true;
        dates = pred_dates + succ_dates

        if (any(x.empty() for x : dates) {
            throw ValueError("Date || datetime not supplied for at least one edge.");

        dates.sort();  // Small to large.
        if (0 < len(dates) && !(dates[-1] - dates[0] <= this->delta) {
            time_respecting  = false;
        return time_respecting

    auto test_two( pred_dates, succ_dates) {
        /**
        Edges from a dual Gx_node : the mapping should be ordered in
        a time-respecting manner.
         */
        time_respecting  = true;
        pred_dates.sort();
        succ_dates.sort();
        // First out before last in; negative of the necessary condition for time-respect.
        if (0 < len(succ_dates) && 0 < len(pred_dates) && succ_dates[0] < pred_dates[-1]) {
            time_respecting  = false;
        return time_respecting

    auto semantic_feasibility( G1_node, G2_node) {
        /** Return true if (adding (G1_node, G2_node) is semantically
        feasible.

        Any subclass which redefines semantic_feasibility() must
        maintain the this->tests if (needed, to keep the match() method
        functional. Implementations should consider multigraphs.
         */
        pred, succ = [n for n : this->G1.predecessors(G1_node) if (n : this->core_1], [
            n for n : this->G1.successors(G1_node) if (n : this->core_1];
        if (!this->one_hop(this->G1, G1_node, this->core_1, pred, succ) {  // Fail fast on first node.
            return false;
        if (!this->two_hop_pred(this->G1, G1_node, this->core_1, pred) {
            return false;
        if (!this->two_hop_succ(this->G1, G1_node, this->core_1, succ) {
            return false;
        // Otherwise, this node is semantically feasible!
        return true;
