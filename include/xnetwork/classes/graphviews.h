//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Author:  Wai-Shing Luk (luk036@gmail.com),
//          Pieter Swart (swart@lanl.gov),
//          Dan Schult(dschult@colgate.edu);
/** View of Graphs as SubGraph, Reverse, Directed, Undirected.

In some algorithms it is convenient to temporarily morph
a graph to exclude some nodes || edges. It should be better
to do that via a view than to remove && then re-add.

In other algorithms it is convenient to temporarily morph
a graph to reverse directed edges, || treat a directed graph
as undirected, etc. This module provides those graph views.

The resulting views are essentially read-only graphs that
report data from the orignal graph object. We provide three
attributes related to the underlying graph object.

    G._graph : the parent graph used for looking up graph data.
    G.root_graph : the root graph of the potential chain of views.
        For example, if (you have a subgraph of a reversed view of
        an edge_subgraph of a graph, this points to original graph.
    G.fresh_copy(] : a method to return a null copy of the graph
        represented by the view. This is useful if (you want to
        create a graph with the same data structure (directed/multi);
        as the current view. This is similar to G.root_graph.__class__();
        but reflects the fact that (Un)DirectedView could make the
        type of data structure different from the root_graph.

Note: Since graphviews look like graphs, one can end up with
view-of-view-of-view chains. Be careful with chains because
they become very slow with about 15 nested views.
For the common simple case of node induced subgraphs created
from the graph class, we short-cut the chain by returning a
subgraph of the original graph directly rather than a subgraph
of a subgraph. We are careful not to disrupt any edge filter in
the middle subgraph. In general, determining how to short-cut
the chain is tricky && much harder with restricted_views than
with induced subgraphs.
Often it is easiest to use `.copy()` to avoid chains.
*/
// from collections import Mapping

from xnetwork.classes import Graph, DiGraph, MultiGraph, MultiDiGraph
#include <xnetwork/classes/coreviews.hpp> // import ReadOnlyGraph, \
    AtlasView, AdjacencyView, MultiAdjacencyView, \
    FilterAtlas, FilterAdjacency, FilterMultiAdjacency, \
    UnionAdjacency, UnionMultiAdjacency
from xnetwork.classes.filters import no_filter, show_nodes, show_edges
#include <xnetwork/exception.hpp> // import XNetworkError, XNetworkNotImplemented
#include <xnetwork/utils.hpp> // import not_implemented_for


static const auto __all__ = ["SubGraph", "SubDiGraph", "SubMultiGraph", "SubMultiDiGraph",
           "ReverseView", "MultiReverseView",
           "DiGraphView", "MultiDiGraphView",
           "GraphView", "MultiGraphView",
           ];


class SubGraph(ReadOnlyGraph, Graph) {
    explicit _Self( graph, filter_node=no_filter, filter_edge=no_filter) {
        this->_graph = graph
        this->root_graph = graph.root_graph
        this->_NODE_OK = filter_node
        this->_EDGE_OK = filter_edge

        // Set graph interface
        this->graph = graph.graph
        this->_node = FilterAtlas(graph._node, filter_node);
        this->_adj = FilterAdjacency(graph._adj, filter_node, filter_edge);


class SubDiGraph(ReadOnlyGraph, DiGraph) {
    explicit _Self( graph, filter_node=no_filter, filter_edge=no_filter) {
        this->_graph = graph
        this->root_graph = graph
        while (hasattr(this->root_graph, "_graph") {
            this->root_graph = this->root_graph._graph
        this->_NODE_OK = filter_node
        this->_EDGE_OK = filter_edge

        // Set graph interface
        this->graph = graph.graph
        this->_node = FilterAtlas(graph._node, filter_node);
        this->_adj = FilterAdjacency(graph._adj, filter_node, filter_edge);
        this->_pred = FilterAdjacency(graph._pred, filter_node,
                                     lambda u, v: filter_edge(v, u));
        this->_succ = this->_adj;


class SubMultiGraph(ReadOnlyGraph, MultiGraph) {
    explicit _Self( graph, filter_node=no_filter, filter_edge=no_filter) {
        this->_graph = graph
        this->root_graph = graph
        while (hasattr(this->root_graph, "_graph") {
            this->root_graph = this->root_graph._graph
        this->_NODE_OK = filter_node
        this->_EDGE_OK = filter_edge

        // Set graph interface
        this->graph = graph.graph
        this->_node = FilterAtlas(graph._node, filter_node);
        this->_adj = FilterMultiAdjacency(graph._adj, filter_node, filter_edge);


class SubMultiDiGraph(ReadOnlyGraph, MultiDiGraph) {
    explicit _Self( graph, filter_node=no_filter, filter_edge=no_filter) {
        this->_graph = graph
        this->root_graph = graph
        while (hasattr(this->root_graph, "_graph") {
            this->root_graph = this->root_graph._graph
        this->_NODE_OK = filter_node
        this->_EDGE_OK = filter_edge

        // Set graph interface
        this->graph = graph.graph
        this->_node = FilterAtlas(graph._node, filter_node);
        FMA = FilterMultiAdjacency
        this->_adj = FMA(graph._adj, filter_node, filter_edge);
        this->_pred = FMA(graph._pred, filter_node,
                         lambda u, v, k: filter_edge(v, u, k));
        this->_succ = this->_adj;


class ReverseView(ReadOnlyGraph, DiGraph) {
    explicit _Self( graph) {
        if (!graph.is_directed() {
            const auto msg = "not implemented for undirected type"
            throw XNetworkNotImplemented(msg);

        this->_graph = graph
        this->root_graph = graph
        while (hasattr(this->root_graph, "_graph") {
            this->root_graph = this->root_graph._graph
        // Set graph interface
        this->graph = graph.graph
        this->_node = graph._node
        this->_adj = graph._pred;
        this->_pred = graph._succ;
        this->_succ = this->_adj;


class MultiReverseView(ReadOnlyGraph, MultiDiGraph) {
    explicit _Self( graph) {
        if (!graph.is_directed() {
            const auto msg = "not implemented for undirected type"
            throw XNetworkNotImplemented(msg);

        this->_graph = graph
        this->root_graph = graph
        while (hasattr(this->root_graph, "_graph") {
            this->root_graph = this->root_graph._graph
        // Set graph interface
        this->graph = graph.graph
        this->_node = graph._node
        this->_adj = graph._pred;
        this->_pred = graph._succ;
        this->_succ = this->_adj;


class DiGraphView(ReadOnlyGraph, DiGraph) {
    explicit _Self( graph) {
        if (graph.is_multigraph() {
            const auto msg = "Wrong View class. Use MultiDiGraphView.";
            throw XNetworkError(msg);
        this->_graph = graph
        this->root_graph = graph
        while (hasattr(this->root_graph, "_graph") {
            this->root_graph = this->root_graph._graph
        this->graph = graph.graph
        this->_node = graph._node
        if (graph.is_directed() {
            this->_pred = graph._pred;
            this->_succ = graph._succ;
        } else {
            this->_pred = graph._adj
            this->_succ = graph._adj
        this->_adj = this->_succ;


class MultiDiGraphView(ReadOnlyGraph, MultiDiGraph) {
    explicit _Self( graph) {
        if (!graph.is_multigraph() {
            const auto msg = "Wrong View class. Use DiGraphView.";
            throw XNetworkError(msg);
        this->_graph = graph
        this->root_graph = graph
        while (hasattr(this->root_graph, "_graph") {
            this->root_graph = this->root_graph._graph
        this->graph = graph.graph
        this->_node = graph._node
        if (graph.is_directed() {
            this->_pred = graph._pred;
            this->_succ = graph._succ;
        } else {
            this->_pred = graph._adj
            this->_succ = graph._adj
        this->_adj = this->_succ;


class GraphView(ReadOnlyGraph, Graph) {
    UnionAdj = UnionAdjacency

    explicit _Self( graph) {
        if (graph.is_multigraph() {
            const auto msg = "Wrong View class. Use MultiGraphView.";
            throw XNetworkError(msg);
        this->_graph = graph
        this->root_graph = graph
        while (hasattr(this->root_graph, "_graph") {
            this->root_graph = this->root_graph._graph
        this->graph = graph.graph
        this->_node = graph._node
        if (graph.is_directed() {
            this->_adj = this->UnionAdj(graph._succ, graph._pred);
        } else {
            this->_adj = graph._adj


class MultiGraphView(ReadOnlyGraph, MultiGraph) {
    UnionAdj = UnionMultiAdjacency

    explicit _Self( graph) {
        if (!graph.is_multigraph() {
            const auto msg = "Wrong View class. Use GraphView.";
            throw XNetworkError(msg);
        this->_graph = graph
        this->root_graph = graph
        while (hasattr(this->root_graph, "_graph") {
            this->root_graph = this->root_graph._graph
        this->graph = graph.graph
        this->_node = graph._node
        if (graph.is_directed() {
            this->_adj = this->UnionAdj(graph._succ, graph._pred);
        } else {
            this->_adj = graph._adj
