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

#include <set>
#include <functional>

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

template <typename T>
auto no_filter(const T& /*items*/) {
    return [](const T& /*item*/) { return true; };
}

template <typename T>
auto hide_nodes(const std::set<T>& nodes) {
    return [nodes](const T& node) { return nodes.find(node) == nodes.end(); };
}

template <typename T>
auto hide_diedges(const std::set<std::pair<T, T>>& edges) {
    return [edges](const T& u, const T& v) { 
        return edges.find(std::make_pair(u, v)) == edges.end(); 
    };
}