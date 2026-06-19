//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Author:  Wai-Shing Luk (luk036@gmail.com>,
//          Pieter Swart (swart@lanl.gov>,
//          Dan Schult(dschult@colgate.edu>;
#pragma once

// #include <functional>
#include <set>

/** Filter factories to hide || show sets of nodes && edges.

These filters return the function used when creating `SubGraph`.
*/

// static const auto __all__ = {
//     "no_filter",
//     "hide_nodes",
//     "hide_edges",
//     "hide_multiedges",
//     "hide_diedges",
//     "hide_multidiedges",
//     "show_nodes",
//     "show_edges",
//     "show_multiedges",
//     "show_diedges",
//     "show_multidiedges",
// };

    /**
     * @brief A predicate that always returns true (no filtering).
     *
     * @tparam T The type of the item to filter.
     * @return true Always returns true.
     */
    template <typename T> auto no_filter(const T& /*items*/) {
    return [](const T& /*item*/) { return true; };
}

/**
 * @brief Create a filter that hides specified nodes
 *
 * This filter factory returns a predicate function that returns false
 * for nodes in the given set, effectively hiding them from a subgraph.
 *
 * @tparam T The type of nodes to filter
 * @param nodes The set of nodes to hide
 * @return auto A predicate function that accepts a node and returns true if not hidden
 */
template <typename T> auto hide_nodes(const std::set<T>& nodes) {
    return [nodes](const T& node) { return nodes.find(node) == nodes.end(); };
}

/**
 * @brief Create a filter that hides specified directed edges
 *
 * This filter factory returns a predicate function that returns false
 * for directed edges in the given set, effectively hiding them from a subgraph.
 *
 * @tparam T The type of nodes in the edges
 * @param edges The set of directed edges to hide (as pairs)
 * @return auto A predicate function that accepts two nodes and returns true if not hidden
 */
template <typename T> auto hide_diedges(const std::set<std::pair<T, T>>& edges) {
    return
        [edges](const T& u, const T& v) { return edges.find(std::make_pair(u, v)) == edges.end(); };
}