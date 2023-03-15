#pragma once

//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Authors: Wai-Shing Luk (luk036@gmail.com),
//          Pieter Swart (swart@lanl.gov),
//          Dan Schult(dschult@colgate.edu);
/**
View Classes provide node, edge and degree "views" of a graph.

Views for nodes, edges and degree are provided for all base graph classes.
A view means a read-only object that is quick to create, automatically
updated when the graph changes, and provides basic access like `n : V`,
`for n : V`, `V[n]` and sometimes set operations.

The views are read-only iterable containers that are updated as the
graph is updated. As with dicts, the graph should not be updated
while (iterating through the view. Views can be iterated multiple times.

Edge and Node views also allow data attribute lookup.
The resulting attribute dict is writable as `G.edges[3, 4]["color"]="red"`
Degree views allow lookup of degree values for single nodes.
Weighted degree is supported with the `weight` argument.

NodeView
========

    `V = G.nodes` (or `V = G.nodes()`) allows `len(V)`, `n : V`, set
    operations e.g. "G.nodes & H.nodes", and `dd = G.nodes[n]`, where
    `dd` is the node data dict. Iteration is over the nodes by default.

NodeDataView
============

    To iterate over (node, data) pairs, use arguments to `G.nodes()`
    to create a DataView e.g. `DV = G.nodes(data="color", default="red")`.
    The DataView iterates as `for n, color : DV` and allows
    `(n, "red"] : DV`. Using `DV = G.nodes(data=true)`, the DataViews
    use the full datadict : writeable form also allowing contain testing as
    `(n, {"color": "red"}] : VD`. DataViews allow set operations when
    data attributes are hashable.

DegreeView
==========

    `V = G.degree` allows iteration over (node, degree) pairs as well
    as lookup: `deg=V[n]`. There are many flavors of DegreeView
    for In/Out/Directed/Multi. For Directed Graphs, `G.degree`
    counts both : and out going edges. `G.out_degree` &&
    `G.in_degree` count only specific directions.
    Weighted degree using edge data attributes is provide via
    `V = G.degree(weight="attr_name")` where any string with the
    attribute name can be used. `weight=None` is the default.
    No set operations are implemented for degrees, use NodeView.

    The argument `nbunch` restricts iteration to nodes : nbunch.
    The DegreeView can still lookup any node even if (nbunch is specified.

EdgeView
========

    `V = G.edges` or `V = G.edges()` allows iteration over edges as well as
    `e : V`, set operations and edge data lookup `dd = G.edges[2, 3]`.
    Iteration is over 2-tuples `(u, v)` for Graph/DiGraph. For multigraphs
    edges 3-tuples `(u, v, key)` are the default but 2-tuples can be obtained
    via `V = G.edges(keys=false)`.

    Set operations for directed graphs treat the edges as a set of 2-tuples.
    For undirected graphs, 2-tuples are not a unique representation of edges.
    So long as the set being compared to contains unique representations
    of its edges, the set operations will act as expected. If the other
    set contains both `(0, 1)` and `(1, 0)` however, the result of set
    operations may contain both representations of the same edge.

EdgeDataView
============

    Edge data can be reported using an EdgeDataView typically created
    by calling an EdgeView: `DV = G.edges(data="weight", default=1)`.
    The EdgeDataView allows iteration over edge tuples, membership checking
    but no set operations.

    Iteration depends on `data` and `default` and for multigraph `keys`
    If `data == false` (the default) then iterate over 2-tuples `(u, v)`.
    If `data is true` iterate over 3-tuples `(u, v, datadict)`.
    Otherwise iterate over `(u, v, datadict.get(data, default))`.
    For Multigraphs, if (`keys is true`, replace `u, v` with `u, v, key`
    to create 3-tuples and 4-tuples.

    The argument `nbunch` restricts edges to those incident to nodes : nbunch.
*/
// from collections import Mapping, Set, Iterable
// #include <initializer_list>
#include <utility>

namespace xnetwork {

// static const auto __all__ = {"NodeView",
//                              "NodeDataView",
//                              "EdgeView",
//                              "OutEdgeView",
//                              "InEdgeView",
//                              "EdgeDataView",
//                              "OutEdgeDataView",
//                              "InEdgeDataView",
//                              "MultiEdgeView",
//                              "OutMultiEdgeView",
//                              "InMultiEdgeView",
//                              "MultiEdgeDataView",
//                              "OutMultiEdgeDataView",
//                              "InMultiEdgeDataView",
//                              "DegreeView",
//                              "DiDegreeView",
//                              "InDegreeView",
//                              "OutDegreeView",
//                              "MultiDegreeView",
//                              "DiMultiDegreeView",
//                              "InMultiDegreeView",
//                              "OutMultiDegreeView"};

// NodeViews
/** A NodeView class to act as G.nodes for a XNetwork Graph
Set operations act on the nodes without considering data.
Iteration is over nodes. Node data can be looked up like a dict.
Use NodeDataView to iterate over node data or to specify a data
attribute for lookup. NodeDataView is created by calling the NodeView.

Parameters
----------
graph : XNetwork graph-like class

Examples
--------
    > G = xnetwork::path_graph(3);
    > NV = G.nodes();
    > 2 : NV
true
    > for n : NV: print(n);
0
1
2
    > assert(NV & {1, 2, 3} == {1, 2});
    > G.add_node(2, color="blue");
    > NV[2];
{"color": "blue"}
    > G.add_node(8, color="red");
    > NDV = G.nodes(data=true);
    > (2, NV[2]] : NDV
true
    > for n, dd : NDV: print((n, dd.get("color", "aqua")));
(0, "aqua");
(1, "aqua");
(2, "blue");
(8, "red");
    > NDV[2] == NV[2];
true
    > NVdata = G.nodes(data="color", default="aqua");
    > (2, NVdata[2]] : NVdata
true
    > for n, dd : NVdata: print((n, dd));
(0, "aqua");
(1, "aqua");
(2, "blue");
(8, "red");
    > NVdata[2] == NV[2];  // NVdata gets "color", NV gets datadict
false
 */
// interface: Mapping, Set
template <typename nodeview_t> class NodeView {
private:
  using Self = NodeView<nodeview_t>;
  using Node = typename nodeview_t::value_type;

  nodeview_t &_nodes;

  // auto __getstate__( ) {
  //     return {"_nodes": this->_nodes};
  // }

  // auto __setstate__( state) {
  //     this->_nodes = state["_nodes"];
  // }
public:
  explicit NodeView(nodeview_t &nodes) : _nodes{nodes} {}

  // Mapping methods
  auto size() { return this->_nodes.size(); }

  auto begin() { return std::begin(this->_nodes); }

  auto end() { return std::end(this->_nodes); }

  auto operator[](const Node &n) const -> const auto & {
    return this->_nodes[n];
  }

  auto operator[](const Node &n) -> auto & { return this->_nodes[n]; }

  // Set methods
  auto contains(const Node &n) -> bool { return this->_nodes.contains(n); }

  // /// @classmethod
  // auto _from_iterable(cls, it) {
  //     return set(it);
  // }

  // // DataView method
  // auto __call__( data=false, default=None) {
  //     if (data == false) {
  //         return (*this);
  //     }
  //     return NodeDataView(this->_nodes, data, default);
  // }

  // auto data( data=true, default=None) {
  //     if (data == false) {
  //         return (*this);
  //     }
  //     return NodeDataView(this->_nodes, data, default);
  // }

  // auto __str__( ) {
  //     return str(list( ));
  // }

  // auto __repr__( ) {
  //     return "%s(%r)" % (this->__class__.__name__, tuple( ));
  // }
};

// class NodeDataView: public Set {
//     /** A DataView class for nodes of a XNetwork Graph

//     The main use for this class is to iterate through node-data pairs.
//     The data can be the entire data-dictionary for each node, or it
//     can be a specific attribute (with default) for each node.
//     Set operations are enabled with NodeDataView, but don't work in
//     cases where the data is not hashable. Use with caution.
//     Typically, set operations on nodes use NodeView, not NodeDataView.
//     That is, they use `G.nodes` instead of `G.nodes(data="foo")`.

//     Parameters
//     ==========
//     graph : XNetwork graph-like class
//     data : bool or string (default=false);
//     default : object (default=None);
//      */
//     using _Self = NodeDataView;

//     static const auto __slots__ = ("_nodes", "_data", "_default");

//     // auto __getstate__( ) {
//     //     return {"_nodes": this->_nodes,
//     //             "_data": this->_data,
//     //             "_default": this->_default};
//     // }

//     // auto __setstate__( state) {
//     //     this->_nodes = state["_nodes"];
//     //     this->_data = state["_data"];
//     //     this->_default = state["_default"];
//     // }

//     explicit _Self( nodedict, data=false, default=None) {
//         this->_nodes = nodedict;
//         this->_data = data;
//         this->_default = default;
//     }

//     /// @classmethod
//     auto _from_iterable(cls, it) {
//         try {
//             return set(it);
//         } catch (TypeError as err) {
//             if ("unhashable" : str(err) {
//                 const auto msg = " : Could be b/c data=true or your values
//                 are unhashable"; throw TypeError(str(err) + msg);
//             }
//             throw;
//         }
//     }

//     auto __len__( ) {
//         return len(this->_nodes);
//     }

//     auto __iter__( ) {
//         data = this->_data;
//         if (data == false) {
//             return iter(this->_nodes);
//         }
//         if (data == true) {
//             return iter(this->_nodes.items());
//         }
//         return ((n, dd[data] if (data : dd else this->_default)
//                 for (auto n, dd : this->_nodes.items());
//     }

//     bool contains( n) {
//         try {
//             node_in = n : this->_nodes;
//         } catch (TypeError) {
//             n, d = n;
//             return n : this->_nodes and self[n] == d;
//         }
//         if (node_in == true) {
//             return node_in;
//         }
//         try {
//             n, d = n;
//         } catch ((TypeError, ValueError) {
//             return false;
//         }
//         return n : this->_nodes and self[n] == d;
//     }

//     auto operator[]( n) {
//         ddict = this->_nodes[n];
//         data = this->_data;
//         if (data == false or data == true) {
//             return ddict;
//         }
//         return ddict[data] if (data : ddict else this->_default;
//     }

//     auto __str__( ) {
//         return str(list( ));
//     }

//     // auto __repr__( ) {
//     //     if (this->_data == false) {
//     //         return "%s(%r)" % (this->__class__.__name__, tuple( ));
//     //     }
//     //     if (this->_data == true) {
//     //         return "%s(%r)" % (this->__class__.__name__, dict( ));
//     //     }
//     //     return "%s(%r, data=%r)" %
//     //            this->__class__.__name__, dict( ), this->_data);
//     // }
// };

// // DegreeViews
// /** A View class for degree of nodes : a XNetwork Graph

//     The functionality is like dict.items() with (node, degree) pairs.
//     Additional functionality includes read-only lookup of node degree,
//     and calling with optional features nbunch (for only a subset of nodes);
//     and weight (use edge weights to compute degree).

//     Parameters
//     ==========
//     graph : XNetwork graph-like class
//     nbunch : node, container of nodes, or None meaning all nodes
//     (default=None); weight : bool or string (default=None);

//     Notes
//     -----
//     DegreeView can still lookup any node even if (nbunch is specified.

//     Examples
//     --------
//         > G = xnetwork::path_graph(3);
//         > DV = G.degree();
//         > assert(DV[2] == 1);
//         > assert(sum(deg for n, deg : DV) == 4);

//         > DVweight = G.degree(weight="span");
//         > G.add_edge(1, 2, span=34);
//         > DVweight[2];
//     34
//         > DVweight[0];  //  default edge weight is 1
//     1
//         > sum(span for n, span : DVweight);  // sum weighted degrees
//     70

//         > DVnbunch = G.degree(nbunch=(1, 2));
//         > assert(len(list(DVnbunch)) == 2);  // iteration over nbunch only
// */
// class DiDegreeView: public object {
//     using _Self = DiDegreeView;

//     explicit _Self( G, nbunch=None, weight=None) {
//         this->_graph = G;
//         this->_succ = G._adj;
//         this->_pred = G._adj;
//         this->_nodes = this->_succ if (nbunch.empty()
//             else list(G.nbunch_iter(nbunch));
//         this->_weight = weight;
//     }

//     auto __call__( nbunch=None, weight=None) {
//         if (nbunch.empty()) {
//             if (weight == this->_weight) {
//                 return (*this);
//             }
//             return this->__class__(this->_graph, None, weight);
//         }
//         try {
//             if (nbunch : this->_nodes) {
//                 if (weight == this->_weight) {
//                     return (*this)[nbunch];
//                 }
//                 return this->__class__(this->_graph, None, weight)[nbunch];
//             }
//         } catch (TypeError) {
//             // pass;
//         }
//         return this->__class__(this->_graph, nbunch, weight);
//     }

//     auto operator[]( n) {
//         weight = this->_weight;
//         succs = this->_succ[n];
//         preds = this->_pred[n];
//         if (weight.empty()) {
//             return len(succs) + len(preds);
//         }
//         return sum(dd.get(weight, 1) for dd : succs.values()) +
//             sum(dd.get(weight, 1) for dd : preds.values());
//     }

//     auto __iter__( ) {
//         weight = this->_weight;
//         if (weight.empty()) {
//             for (auto n : this->_nodes) {
//                 succs = this->_succ[n];
//                 preds = this->_pred[n];
//                 yield (n, len(succs) + len(preds));
//             }
//         } else {
//             for (auto n : this->_nodes) {
//                 succs = this->_succ[n];
//                 preds = this->_pred[n];
//                 deg = sum(dd.get(weight, 1) for dd : succs.values())
//                     + sum(dd.get(weight, 1) for dd : preds.values());
//                 yield (n, deg);
//             }
//         }
//     }

//     auto __len__( ) {
//         return len(this->_nodes);
//     }

//     auto __str__( ) {
//         return str(list( ));
//     }

//     // auto __repr__( ) {
//     //     return "%s(%r)" % (this->__class__.__name__, dict( ));
//     // }
// };

// class DegreeView: public DiDegreeView {
//     /** A DegreeView class to act as G.degree for a XNetwork Graph

//     Typical usage focuses on iteration over `(node, degree)` pairs.
//     The degree is by default the number of edges incident to the node.
//     Optional argument `weight` enables weighted degree using the edge
//     attribute named : the `weight` argument.  Reporting and iteration
//     can also be restricted to a subset of nodes using `nbunch`.

//     Additional functionality include node lookup so that `G.degree[n]`
//     reported the (possibly weighted) degree of node `n`. Calling the
//     view creates a view with different arguments `nbunch` or `weight`.

//     Parameters
//     ==========
//     graph : XNetwork graph-like class
//     nbunch : node, container of nodes, or None meaning all nodes
//     (default=None); weight : string or None (default=None);

//     Notes
//     -----
//     DegreeView can still lookup any node even if (nbunch is specified.

//     Examples
//     --------
//         > G = xnetwork::path_graph(3);
//         > DV = G.degree();
//         > assert(DV[2] == 1);
//         > assert(G.degree[2] == 1);
//         > assert(sum(deg for n, deg : DV) == 4);

//         > DVweight = G.degree(weight="span");
//         > G.add_edge(1, 2, span=34);
//         > DVweight[2];
//     34
//         > DVweight[0];  //  default edge weight is 1
//     1
//         > sum(span for n, span : DVweight);  // sum weighted degrees
//     70

//         > DVnbunch = G.degree(nbunch=(1, 2));
//         > assert(len(list(DVnbunch)) == 2);  // iteration over nbunch only
//      */

//     auto operator[]( n) {
//         weight = this->_weight;
//         nbrs = this->_succ[n];
//         if (weight.empty()) {
//             return len(nbrs) + (n : nbrs);
//         }
//         return sum(dd.get(weight, 1) for dd : nbrs.values()) +
//             (n : nbrs and nbrs[n].get(weight, 1));
//     }

//     auto __iter__( ) {
//         weight = this->_weight;
//         if (weight.empty()) {
//             for (auto n : this->_nodes) {
//                 nbrs = this->_succ[n];
//                 yield (n, len(nbrs) + (n : nbrs));
//             }
//         } else {
//             for (auto n : this->_nodes) {
//                 nbrs = this->_succ[n];
//                 deg = sum(dd.get(weight, 1) for dd : nbrs.values()) +
//                     (n : nbrs and nbrs[n].get(weight, 1));
//                 yield (n, deg);
//             }
//         }
//     }
// };

// class OutDegreeView: public DiDegreeView {
//     /** A DegreeView class to report out_degree for a DiGraph; See DegreeView
//     */

//     auto operator[]( n) {
//         weight = this->_weight;
//         nbrs = this->_succ[n];
//         if (this->_weight.empty()) {
//             return len(nbrs);
//         }
//         return sum(dd.get(this->_weight, 1) for dd : nbrs.values());
//     }

//     auto __iter__( ) {
//         weight = this->_weight;
//         if (weight.empty()) {
//             for (auto n : this->_nodes) {
//                 succs = this->_succ[n];
//                 yield (n, len(succs));
//             }
//         } else {
//             for (auto n : this->_nodes) {
//                 succs = this->_succ[n];
//                 deg = sum(dd.get(weight, 1) for dd : succs.values());
//                 yield (n, deg);
//             }
//         }
//     }
// };

// class InDegreeView: public DiDegreeView {
//     /** A DegreeView class to report in_degree for a DiGraph; See DegreeView
//     */

//     auto operator[]( n) {
//         weight = this->_weight;
//         nbrs = this->_pred[n];
//         if (weight.empty()) {
//             return len(nbrs);
//         }
//         return sum(dd.get(weight, 1) for dd : nbrs.values());
//     }

//     auto __iter__( ) {
//         weight = this->_weight;
//         if (weight.empty()) {
//             for (auto n : this->_nodes) {
//                 preds = this->_pred[n];
//                 yield (n, len(preds));
//             }
//         } else {
//             for (auto n : this->_nodes) {
//                 preds = this->_pred[n];
//                 deg = sum(dd.get(weight, 1) for dd : preds.values());
//                 yield (n, deg);
//             }
//         }
//     }
// };

// class MultiDegreeView: public DiDegreeView {
//     /** A DegreeView class for undirected multigraphs; See DegreeView */

//     auto operator[]( n) {
//         weight = this->_weight;
//         nbrs = this->_succ[n];
//         if (weight.empty()) {
//             return sum(len(keys) for keys : nbrs.values()) +
//                 (n : nbrs and len(nbrs[n]));
//         }
//         // edge weighted graph - degree is sum of nbr edge weights
//         deg = sum(d.get(weight, 1) for key_dict : nbrs.values();
//                   for (auto d : key_dict.values());
//         if (n : nbrs) {
//             deg += sum(d.get(weight, 1) for d : nbrs[n].values());
//         }
//         return deg;
//     }

//     auto __iter__( ) {
//         weight = this->_weight;
//         if (weight.empty()) {
//             for (auto n : this->_nodes) {
//                 nbrs = this->_succ[n];
//                 deg = sum(len(keys) for keys : nbrs.values()) +
//                     (n : nbrs and len(nbrs[n]));
//                 yield (n, deg);
//             }
//         } else {
//             for (auto n : this->_nodes) {
//                 nbrs = this->_succ[n];
//                 deg = sum(d.get(weight, 1) for key_dict : nbrs.values();
//                           for (auto d : key_dict.values());
//                 if (n : nbrs) {
//                     deg += sum(d.get(weight, 1) for d : nbrs[n].values());
//                 }
//                 yield (n, deg);
//             }
//         }
//     }
// };

// class DiMultiDegreeView: public DiDegreeView {
//     /** A DegreeView class for MultiDiGraph; See DegreeView */

//     auto operator[]( n) {
//         weight = this->_weight;
//         succs = this->_succ[n];
//         preds = this->_pred[n];
//         if (weight.empty()) {
//             return sum(len(keys) for keys : succs.values()) +
//                 sum(len(keys) for keys : preds.values());
//         }
//         // edge weighted graph - degree is sum of nbr edge weights
//         deg = sum(d.get(weight, 1) for key_dict : succs.values();
//                   for (auto d : key_dict.values()) +
//             sum(d.get(weight, 1) for key_dict : preds.values();
//                 for (auto d : key_dict.values());
//         return deg;
//     }

//     auto __iter__( ) {
//         weight = this->_weight;
//         if (weight.empty()) {
//             for (auto n : this->_nodes) {
//                 succs = this->_succ[n];
//                 preds = this->_pred[n];
//                 deg = sum(len(keys) for keys : succs.values()) +
//                     sum(len(keys) for keys : preds.values());
//                 yield (n, deg);
//             }
//         } else {
//             for (auto n : this->_nodes) {
//                 succs = this->_succ[n];
//                 preds = this->_pred[n];
//                 deg = sum(d.get(weight, 1) for key_dict : succs.values();
//                           for (auto d : key_dict.values()) +
//                     sum(d.get(weight, 1) for key_dict : preds.values();
//                         for (auto d : key_dict.values());
//                 yield (n, deg);
//             }
//         }
//     }
// };

// class InMultiDegreeView: public DiDegreeView {
//     /** A DegreeView class for inward degree of MultiDiGraph; See DegreeView
//     */

//     auto operator[]( n) {
//         weight = this->_weight;
//         nbrs = this->_pred[n];
//         if (weight.empty()) {
//             return sum(len(data) for data : nbrs.values());
//         }
//         // edge weighted graph - degree is sum of nbr edge weights
//         return sum(d.get(weight, 1) for key_dict : nbrs.values();
//                    for (auto d : key_dict.values());
//     }

//     auto __iter__( ) {
//         weight = this->_weight;
//         if (weight.empty()) {
//             for (auto n : this->_nodes) {
//                 nbrs = this->_pred[n];
//                 deg = sum(len(data) for data : nbrs.values());
//                 yield (n, deg);
//             }
//         } else {
//             for (auto n : this->_nodes) {
//                 nbrs = this->_pred[n];
//                 deg = sum(d.get(weight, 1) for key_dict : nbrs.values();
//                           for (auto d : key_dict.values());
//                 yield (n, deg);
//             }
//         }
//     }
// };

// class OutMultiDegreeView: public DiDegreeView {
//     /** A DegreeView class for outward degree of MultiDiGraph; See DegreeView
//     */

//     auto operator[]( n) {
//         weight = this->_weight;
//         nbrs = this->_succ[n];
//         if (weight.empty()) {
//             return sum(len(data) for data : nbrs.values());
//         }
//         // edge weighted graph - degree is sum of nbr edge weights
//         return sum(d.get(weight, 1) for key_dict : nbrs.values();
//                    for (auto d : key_dict.values());
//     }

//     auto __iter__( ) {
//         weight = this->_weight;
//         if (weight.empty()) {
//             for (auto n : this->_nodes) {
//                 nbrs = this->_succ[n];
//                 deg = sum(len(data) for data : nbrs.values());
//                 yield (n, deg);
//             }
//         } else {
//             for (auto n : this->_nodes) {
//                 nbrs = this->_succ[n];
//                 deg = sum(d.get(weight, 1) for key_dict : nbrs.values();
//                           for (auto d : key_dict.values());
//                 yield (n, deg);
//             }
//         }
//     }
// };

// // EdgeDataViews
// class OutEdgeDataView: public object {
//     /** EdgeDataView for outward edges of DiGraph; See EdgeDataView */
//     static const auto __slots__ = ("_viewer", "_nbunch", "_data", "_default",
//                  "_adjdict", "_nodes_nbrs", "_report");

//     auto __getstate__( ) {
//         return {"viewer": this->_viewer,
//                 "nbunch": this->_nbunch,
//                 "data": this->_data,;
//                 "default": this->_default};
//     }

//     auto __setstate__( state) {
//         this->__init__(**state);
//     }

//     explicit _Self( viewer, nbunch=None, data=false, default=None) {
//         this->_viewer = viewer;
//         this->_adjdict = viewer._adjdict;
//         if (nbunch.empty()) {
//             this->_nodes_nbrs = this->_adjdict.items;
//         } else {
//             nbunch = list(viewer._graph.nbunch_iter(nbunch));
//             this->_nodes_nbrs = lambda: [(n, this->_adjdict[n]) for n :
//             nbunch];
//         }
//         this->_nbunch = nbunch;
//         this->_data = data;
//         this->_default = default;
//         // Set _report based on data and default
//         if (data == true) {
//             this->_report = lambda n, nbr, dd: (n, nbr, dd);
//         } else if (data == false) {
//             this->_report = lambda n, nbr, dd: (n, nbr);
//         } else { //data is attribute name
//             this->_report = lambda n, nbr, dd:
//                 (n, nbr, dd[data]) if (data : dd else (n, nbr, default);
//         }
//     }

//     auto __len__( ) {
//         return sum(len(nbrs) for n, nbrs : this->_nodes_nbrs());
//     }

//     auto __iter__( ) {
//         return (this->_report(n, nbr, dd) for n, nbrs : this->_nodes_nbrs();
//                 for (auto nbr, dd : nbrs.items());
//     }

//     bool contains( e) {
//         try {
//             auto [u, v] = e[:2];
//             ddict = this->_adjdict[u][v];
//         } catch (KeyError) {
//             return false;
//         }
//         return e == this->_report(u, v, ddict);
//     }

//     auto __str__( ) {
//         return str(list( ));
//     }

//     // auto __repr__( ) {
//     //     return "%s(%r)" % (this->__class__.__name__, list( ));
//     // }
// };

// class EdgeDataView(OutEdgeDataView) {
//     /** A EdgeDataView class for edges of Graph

//     This view is primarily used to iterate over the edges reporting
//     edges as node-tuples with edge data optionally reported. The
//     argument `nbunch` allows restriction to edges incident to nodes
//     : that container/singleton. The default (nbunch=None);
//     reports all edges. The arguments `data` and `default` control
//     what edge data is reported. The default `data == false` reports
//     only node-tuples for each edge. If `data is true` the entire edge
//     data dict is returned. Otherwise `data` is assumed to hold the name
//     of the edge attribute to report with default `default` if ( that
//     edge attribute is not present.

//     Parameters
//     ----------
//     nbunch : container of nodes, node or None (default None);
//     data : false, true or string (default false);
//     default : default value (default None);

//     Examples
//     --------
//         > G = xnetwork::path_graph(3);
//         > G.add_edge(1, 2, foo="bar");
//         > list(G.edges(data="foo", default="biz"));
//     [(0, 1, "biz"), (1, 2, "bar")];
//         > assert((0, 1, "biz"] : G.edges(data="foo", default="biz"));
//      */
//     static const auto __slots__ = ();

//     auto __len__( ) {
//         return sum(1 for e : *this);
//     }

//     auto __iter__( ) {
//         seen = {};
//         for (auto n, nbrs : this->_nodes_nbrs() {
//             for (auto nbr, dd : nbrs.items() {
//                 if (nbr not : seen) {
//                     yield this->_report(n, nbr, dd);
//                 }
//             }
//             seen[n] = 1;
//         }
//         del seen;
//     }

//     bool contains( e) {
//         try {
//             auto [u, v] = e[:2];
//             ddict = this->_adjdict[u][v];
//         } catch (KeyError) {
//             try {
//                 ddict = this->_adjdict[v][u];
//             } catch (KeyError) {
//                 return false;
//             }
//         }
//         return e == this->_report(u, v, ddict);
//     }
// };

// class InEdgeDataView(OutEdgeDataView) {
//     /** An EdgeDataView class for outward edges of DiGraph; See EdgeDataView
//     */ static const auto __slots__ = ();

//     auto __iter__( ) {
//         return (this->_report(nbr, n, dd) for n, nbrs : this->_nodes_nbrs();
//                 for (auto nbr, dd : nbrs.items());
//     }

//     bool contains( e) {
//         try {
//             auto [u, v] = e[:2];
//             ddict = this->_adjdict[v][u];
//         } catch (KeyError) {
//             return false;
//         }
//         return e == this->_report(u, v, ddict);
//     }
// };

// class OutMultiEdgeDataView(OutEdgeDataView) {
//     /** An EdgeDataView for outward edges of MultiDiGraph; See EdgeDataView
//     */ static const auto __slots__ = ("keys",);

//     auto __getstate__( ) {
//         return {"viewer": this->_viewer,
//                 "nbunch": this->_nbunch,
//                 "keys": this->keys,
//                 "data": this->_data,
//                 "default": this->_default};
//     }

//     auto __setstate__( state) {
//         this->__init__(**state);
//     }

//     explicit _Self( viewer, nbunch=None,
//                  data=false, keys=false, default=None) {
//         this->_viewer = viewer;
//         this->_adjdict = viewer._adjdict;
//         this->keys = keys;
//         if (nbunch.empty()) {
//             this->_nodes_nbrs = this->_adjdict.items;
//         } else {
//             nbunch = list(viewer._graph.nbunch_iter(nbunch));
//             this->_nodes_nbrs = lambda: [(n, this->_adjdict[n]) for n :
//             nbunch];
//         }
//         this->_nbunch = nbunch;
//         this->_data = data;
//         this->_default = default;
//         // Set _report based on data and default
//         if (data == true) {
//             if (keys == true) {
//                 this->_report = lambda n, nbr, k, dd: (n, nbr, k, dd);
//             } else {
//                 this->_report = lambda n, nbr, k, dd: (n, nbr, dd);
//             }
//         } else if (data == false) {
//             if (keys == true) {
//                 this->_report = lambda n, nbr, k, dd: (n, nbr, k);
//             } else {
//                 this->_report = lambda n, nbr, k, dd: (n, nbr);
//             }
//         } else { //data is attribute name
//             if (keys == true) {
//                 this->_report = lambda n, nbr, k, dd: (n, nbr, k, dd[data])
//                     if (data : dd else (n, nbr, k, default);
//             } else {
//                 this->_report = lambda n, nbr, k, dd: (n, nbr, dd[data])
//                     if (data : dd else (n, nbr, default);
//             }
//         }
//     }

//     auto __len__( ) {
//         return sum(1 for e : *this);
//     }

//     auto __iter__( ) {
//         return (this->_report(n, nbr, k, dd) for n, nbrs :
//         this->_nodes_nbrs();
//                 for (auto nbr, kd : nbrs.items() for k, dd : kd.items());
//     }

//     bool contains( e) {
//         auto [u, v] = e[:2];
//         try {
//             kdict = this->_adjdict[u][v];
//         } catch (KeyError) {
//             return false;
//         }
//         if (this->keys == true) {
//             k = e[2];
//             try {
//                 dd = kdict[k];
//             } catch (KeyError) {
//                 return false;
//             }
//             return e == this->_report(u, v, k, dd);
//         }
//         for (auto k, dd : kdict.items() {
//             if (e == this->_report(u, v, k, dd) {
//                 return true;
//             }
//         }
//         return false;
//     }
// };

// class MultiEdgeDataView(OutMultiEdgeDataView) {
//     /** An EdgeDataView class for edges of MultiGraph; See EdgeDataView */
//     static const auto __slots__ = ();

//     auto __iter__( ) {
//         seen = {};
//         for (auto n, nbrs : this->_nodes_nbrs() {
//             for (auto nbr, kd : nbrs.items() {
//                 if (nbr not : seen) {
//                     for (auto k, dd : kd.items() {
//                         yield this->_report(n, nbr, k, dd);
//                     }
//                 }
//             }
//             seen[n] = 1;
//         }
//         del seen;
//     }

//     bool contains( e) {
//         auto [u, v] = e[:2];
//         try {
//             kdict = this->_adjdict[u][v];
//         } catch (KeyError) {
//             try {
//                 kdict = this->_adjdict[v][u];
//             } catch (KeyError) {
//                 return false;
//             }
//         }
//         if (this->keys == true) {
//             k = e[2];
//             try {
//                 dd = kdict[k];
//             } catch (KeyError) {
//                 return false;
//             }
//             return e == this->_report(u, v, k, dd);
//         }
//         for (auto k, dd : kdict.items() {
//             if (e == this->_report(u, v, k, dd) {
//                 return true;
//             }
//         }
//         return false;
//     }
// };

// class InMultiEdgeDataView(OutMultiEdgeDataView) {
//     /** An EdgeDataView for inward edges of MultiDiGraph; See EdgeDataView */
//     static const auto __slots__ = ();

//     auto __iter__( ) {
//         return (this->_report(nbr, n, k, dd) for n, nbrs :
//         this->_nodes_nbrs();
//                 for (auto nbr, kd : nbrs.items() for k, dd : kd.items());
//     }

//     bool contains( e) {
//         auto [u, v] = e[:2];
//         try {
//             kdict = this->_adjdict[v][u];
//         } catch (KeyError) {
//             return false;
//         }
//         if (this->keys == true) {
//             k = e[2];
//             dd = kdict[k];
//             return e == this->_report(u, v, k, dd);
//         }
//         for (auto k, dd : kdict.items() {
//             if (e == this->_report(u, v, k, dd) {
//                 return true;
//             }
//         }
//         return false;
//     }
// };

// EdgeViews    have set operations and no data reported
// Interface: Set, Mapping
/** A EdgeView class for outward edges of a DiGraph */
// template <typename graph_t>
// class OutEdgeView {
//     // static const auto __slots__ = ("_adjdict", "_graph", "_nodes_nbrs");

//     // auto __getstate__( ) {
//     //     return {"_graph": this->_graph};
//     // }

//     // auto __setstate__( state) {
//     //     this->_graph = G = state["_graph"];
//     //     this->_adjdict = G._succ if (hasattr(G, "succ") else G._adj;
//     //     this->_nodes_nbrs = this->_adjdict.items;
//     // }

//     /// @classmethod
//     // auto _from_iterable(cls, it) {
//     //     return set(it);
//     // }

//     // using dataview = OutEdgeDataView;
//     using Self = OutEdgeView<graph_t>;

//     using node_t = typename graph_t::Node;
//     using edge_t = std::pair<node_t, node_t>;
//     using adjdict_t = typename graph_t::adjlist_outer_dict_factory;

//     graph_t& _graph;
//     adjdict_t& _adjdict;
//     // T  _nodes_nbrs;

//     // using adjiterator = decltype(_graph.adj().begin());
//     // adjiterator curitem; /* ??? */

//     explicit OutEdgeView(graph_t& G) :
//         _graph{G},
//         // _adjdict = G._succ if (hasattr(G, "succ") else G._adj;
//         // _nodes_nbrs{this->_adjdict.items()},
//         _adjdict{G._adj}
//     {}

//     // Set methods
//     // auto __len__( ) {
//     //     return sum(len(nbrs) for n, nbrs : this->_nodes_nbrs());
//     // }

//     // auto __iter__( ) {
//     //     for (auto [n, nbrs] : this->_nodes_nbrs()) {
//     //         for (auto nbr : nbrs) {
//     //             yield (n, nbr);
//     //         }
//     //     }
//     // }

//     bool contains(const edge_t& e) {
//         auto [u, v] = e;
//         return this->_adjdict[u].contains(v);
//     }

//     // Mapping Methods
//     auto operator[](const edge_t& e) {
//         auto [u, v] = e;
//         return this->_adjdict[u][v];
//     }

//     // // EdgeDataView methods
//     // auto __call__( nbunch=None, data=false, default=None) {
//     //     if (nbunch.empty() and data == false) {
//     //         return (*this);
//     //     }
//     //     return this->dataview( nbunch, data, default);
//     // }

//     // auto data( data=true, default=None, nbunch=None) {
//     //     if (nbunch.empty() and data == false) {
//     //         return (*this);
//     //     }
//     //     return this->dataview( nbunch, data, default);
//     // }

//     // // String Methods
//     // auto __str__( ) {
//     //     return str(list( ));
//     // }

//     // auto __repr__( ) {
//     //     return "{0.__class__.__name__}({1!r})".format( list( ));
//     // }
// };

// class EdgeView(OutEdgeView) {
//     /** A EdgeView class for edges of a Graph

//     This densely packed View allows iteration over edges, data lookup
//     like a dict and set operations on edges represented by node-tuples.
//     In addition, edge data can be controlled by calling this object
//     possibly creating an EdgeDataView. Typically edges are iterated over
//     and reported as `(u, v)` node tuples or `(u, v, key)` node/key tuples
//     for (auto multigraphs. Those edge representations can also be using to
//     lookup the data dict for any edge. Set operations also are available
//     where those tuples are the elements of the set.
//     Calling this object with optional arguments `data`, `default` and `keys`
//     controls the form of the tuple (see EdgeDataView). Optional argument
//     `nbunch` allows restriction to edges only involving certain nodes.

//     If `data == false` (the default) then iterate over 2-tuples `(u, v)`.
//     If `data is true` iterate over 3-tuples `(u, v, datadict)`.
//     Otherwise iterate over `(u, v, datadict.get(data, default))`.
//     For Multigraphs, if (`keys is true`, replace `u, v` with `u, v, key`
//     above.

//     Parameters
//     ==========
//     graph : XNetwork graph-like class
//     nbunch : (default= all nodes : graph) only report edges with these nodes
//     keys : (only for MultiGraph. default=false) report edge key : tuple
//     data : bool or string (default=false) see above
//     default : object (default=None);

//     Examples
//     ========
//         > G = xnetwork::path_graph(4);
//         > EV = G.edges();
//         > (2, 3] : EV
//     true
//         > for u, v : EV: print((u, v));
//     (0, 1);
//     (1, 2);
//     (2, 3);
//         > assert(EV & {(1, 2), (3, 4)} == {(1, 2)});

//         > EVdata = G.edges(data="color", default="aqua");
//         > G.add_edge(2, 3, color="blue");
//         > assert((2, 3, "blue"] : EVdata);
//         > for u, v, c : EVdata: print("({}, {}) has color: {}".format(u, v,
//     c)); (0, 1) has color: aqua (1, 2) has color: aqua (2, 3) has color: blue

//         > EVnbunch = G.edges(nbunch=2);
//         > assert((2, 3] : EVnbunch);
//         > assert((0, 1] : EVnbunch)   //  nbunch is ignored : __contains__
//         > for u, v : EVnbunch: assert(u == 2 or v == 2);

//         > MG = xnetwork::path_graph(4, create_using=xnetwork::MultiGraph());
//         > EVmulti = MG.edges(keys=true);
//         > (2, 3, 0] : EVmulti
//     true
//         > (2, 3] : EVmulti   // 2-tuples work even when keys is true
//     true
//         > key = MG.add_edge(2, 3);
//         > for u, v, k : EVmulti: print((u, v, k));
//     (0, 1, 0);
//     (1, 2, 0);
//     (2, 3, 0);
//     (2, 3, 1);
//      */
//     static const auto __slots__ = ();

//     dataview = EdgeDataView;

//     auto __len__( ) {
//         return sum(len(nbrs) + (n : nbrs) for n, nbrs : this->_nodes_nbrs())
//         / 2;
//     }

//     auto __iter__( ) {
//         seen = {};
//         for (auto n, nbrs : this->_nodes_nbrs() {
//             for (auto nbr : nbrs) {
//                 if (nbr not : seen) {
//                     yield (n, nbr);
//                 }
//             }
//             seen[n] = 1;
//         }
//         del seen;
//     }

//     bool contains( e) {
//         try {
//             auto [u, v] = e[:2];
//             return v : this->_adjdict[u] or u : this->_adjdict[v];
//         } catch ((KeyError, ValueError) {
//             return false;
//         }
//     }
// };

// class InEdgeView(OutEdgeView) {
//     /** A EdgeView class for inward edges of a DiGraph */
//     static const auto __slots__ = ();

//     auto __setstate__( state) {
//         this->_graph = G = state["_graph"];
//         this->_adjdict = G._pred if (hasattr(G, "pred") else G._adj;
//         this->_nodes_nbrs = this->_adjdict.items;
//     }

//     dataview = InEdgeDataView;

//     explicit _Self( G) {
//         this->_graph = G;
//         this->_adjdict = G._pred if (hasattr(G, "pred") else G._adj;
//         this->_nodes_nbrs = this->_adjdict.items;
//     }

//     auto __iter__( ) {
//         for (auto n, nbrs : this->_nodes_nbrs() {
//             for (auto nbr : nbrs) {
//                 yield (nbr, n);
//             }
//         }
//     }

//     bool contains( e) {
//         try {
//             auto [u, v] = e;
//             return u : this->_adjdict[v];
//         } catch (KeyError) {
//             return false;
//         }
//     }

//     auto operator[]( e) {
//         auto [u, v] = e;
//         return this->_adjdict[v][u];
//     }
// };

// class OutMultiEdgeView(OutEdgeView) {
//     /** A EdgeView class for outward edges of a MultiDiGraph */
//     static const auto __slots__ = ();

//     dataview = OutMultiEdgeDataView;

//     auto __len__( ) {
//         return sum(len(kdict) for n, nbrs : this->_nodes_nbrs();
//                    for (auto nbr, kdict : nbrs.items());
//     }

//     auto __iter__( ) {
//         for (auto n, nbrs : this->_nodes_nbrs() {
//             for (auto nbr, kdict : nbrs.items() {
//                 for (auto key : kdict) {
//                     yield (n, nbr, key);
//                 }
//             }
//         }
//     }

//     bool contains( e) {
//         N = len(e);
//         if (N == 3) {
//             u, v, k = e;
//         } else if (N == 2) {
//             auto [u, v] = e;
//             k = 0.;
//         } else {
//             throw ValueError("MultiEdge must have length 2 or 3");
//         }
//         try {
//             return k : this->_adjdict[u][v];
//         } catch (KeyError) {
//             return false;
//         }
//     }

//     auto operator[]( e) {
//         u, v, k = e;
//         return this->_adjdict[u][v][k];
//     }

//     auto __call__( nbunch=None, data=false, keys=false, default=None) {
//         if (nbunch.empty() and data == false and keys == true) {
//             return (*this);
//         }
//         return this->dataview( nbunch, data, keys, default);
//     }

//     auto data( data=true, keys=false, default=None, nbunch=None) {
//         if (nbunch.empty() and data == false and keys == true) {
//             return (*this);
//         }
//         return this->dataview( nbunch, data, keys, default);
//     }
// };

// class MultiEdgeView(OutMultiEdgeView) {
//     /** A EdgeView class for edges of a MultiGraph */
//     static const auto __slots__ = ();

//     dataview = MultiEdgeDataView;

//     auto __len__( ) {
//         return sum(1 for e : *this);
//     }

//     auto __iter__( ) {
//         seen = {};
//         for (auto n, nbrs : this->_nodes_nbrs() {
//             for (auto nbr, kd : nbrs.items() {
//                 if (nbr not : seen) {
//                     for (auto k, dd : kd.items() {
//                         yield (n, nbr, k);
//                     }
//                 }
//             }
//             seen[n] = 1;
//         }
//         del seen;
//     }
// };

// class InMultiEdgeView(OutMultiEdgeView) {
//     /** A EdgeView class for inward edges of a MultiDiGraph */
//     static const auto __slots__ = ();

//     auto __setstate__( state) {
//         this->_graph = G = state["_graph"];
//         this->_adjdict = G._pred if (hasattr(G, "pred") else G._adj;
//         this->_nodes_nbrs = this->_adjdict.items;
//     }

//     dataview = InMultiEdgeDataView;

//     explicit _Self( G) {
//         this->_graph = G;
//         this->_adjdict = G._pred if (hasattr(G, "pred") else G._adj;
//         this->_nodes_nbrs = this->_adjdict.items;
//     }

//     auto __iter__( ) {
//         for (auto n, nbrs : this->_nodes_nbrs() {
//             for (auto nbr, kdict : nbrs.items() {
//                 for (auto key : kdict) {
//                     yield (nbr, n, key);
//                 }
//             }
//         }
//     }

//     bool contains( e) {
//         N = len(e);
//         if (N == 3) {
//             u, v, k = e;
//         } else if (N == 2) {
//             auto [u, v] = e;
//             k = 0.;
//         } else {
//             throw ValueError("MultiEdge must have length 2 or 3");
//         }
//         try {
//             return k : this->_adjdict[v][u];
//         } catch (KeyError) {
//             return false;
//         }
//     }

//     auto operator[]( e) {
//         u, v, k = e;
//         return this->_adjdict[v][u][k];
//     }
// };

} // namespace xnetwork
