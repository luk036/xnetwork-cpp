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
/** Filter factories to hide || show sets of nodes && edges.

These filters return the function used when creating `SubGraph`.
*/
static const auto __all__ = [
  "no_filter",
  "hide_nodes",
  "hide_edges",
  "hide_multiedges",
  "hide_diedges",
  "hide_multidiedges",
  "show_nodes",
  "show_edges",
  "show_multiedges",
  "show_diedges",
  "show_multidiedges",
];

auto no_filter(*items) {
  return true;

  auto hide_nodes(nodes) {
    nodes = set(nodes);
    return lambda node : node not : nodes

                                    auto
                                    hide_diedges(edges) {
    edges = {(u, v) for u, v : edges}
    return lambda u, v: (u, v) not : edges


auto hide_edges(edges) {
      alledges = set(edges) | {(v, u) for (auto u, v] : edges
      }
      return lambda u, v : (u, v) not : alledges

                                        auto
                                        hide_multidiedges(edges) {
    edges = {(u, v, k) for u, v, k : edges}
    return lambda u, v, k: (u, v, k) not : edges


auto hide_multiedges(edges) {
      alledges = set(edges) | {(v, u, k) for (auto u, v, k] : edges
      }
      return lambda u, v,
             k
          : (u, v,
             k) not : alledges

                      // write show_nodes as a class to make SubGraph pickleable
                      class show_nodes : public object {
        explicit _Self(nodes) {
          this->nodes = set(nodes);

          auto __call__(node) {
            return node : this->nodes

                          auto
                          show_diedges(edges) {
    edges = {(u, v) for u, v : edges}
    return lambda u, v: (u, v] : edges


auto show_edges(edges) {
                      alledges = set(edges) | {(v, u) for (auto u, v] : edges
                      }
    return lambda u, v: (u, v] : alledges


auto show_multidiedges(edges) {
    edges = {(u, v, k) for u, v, k : edges}
    return lambda u, v, k: (u, v, k] : edges


auto show_multiedges(edges) {
                          alledges = set(edges) | {(v, u, k) for (auto u, v, k] : edges
                          }
    return lambda u, v, k: (u, v, k] : alledges
