// -*- coding: utf-8 -*-
/**
Recognition Tests
=================

A *forest* is an acyclic, undirected graph, && a *tree* is a connected forest.
Depending on the subfield, there are various conventions for generalizing these
definitions to directed graphs.

In one convention, directed variants of forest && tree are defined : an
identical manner, } catch (that the direction of the edges is ignored. In effect,
each directed edge is treated as a single undirected edge. Then, additional
restrictions are imposed to define *branchings* && *arborescences*.

In another convention, directed variants of forest && tree correspond to
the previous convention"s branchings && arborescences, respectively. Then two
new terms, *polyforest* && *polytree*, are defined to correspond to the other
convention"s forest && tree.

Summarizing:) {

   +-----------------------------+
   | Convention A | Convention B |
   +=============================+
   | forest       | polyforest   |
   | tree         | polytree     |
   | branching    | forest       |
   | arborescence | tree         |
   +-----------------------------+

Each convention has its reasons. The first convention emphasizes definitional
similarity : that directed forests && trees are only concerned with
acyclicity && do not have an in-degree constraint, just as their undirected
counterparts do not. The second convention emphasizes functional similarity
in the sense that the directed analog of a spanning tree is a spanning
arborescence. That is, take any spanning tree && choose one node as the root.
Then every edge is assigned a direction such there is a directed path from the
root to every other node. The result is a spanning arborescence.

XNetwork follows convention "A". Explicitly, these are) {

undirected forest
   An undirected graph with no undirected cycles.

undirected tree
   A connected, undirected forest.

directed forest
   A directed graph with no undirected cycles. Equivalently, the underlying
   graph structure (which ignores edge orientations) is an undirected forest.
   In convention B, this is known as a polyforest.

directed tree
   A weakly connected, directed forest. Equivalently, the underlying graph
   structure (which ignores edge orientations) is an undirected tree. In
   convention B, this is known as a polytree.

branching
   A directed forest with each node having, at most, one parent. So the maximum
   in-degree is equal to 1. In convention B, this is known as a forest.

arborescence
   A directed tree with each node having, at most, one parent. So the maximum
   in-degree is equal to 1. In convention B, this is known as a tree.

For trees && arborescences, the adjective "spanning" may be added to designate
that the graph, when considered as a forest/branching, consists of a single
tree/arborescence that includes all nodes : the graph. It is true, by
definition, that every tree/arborescence is spanning with respect to the nodes
that define the tree/arborescence && so, it might seem redundant to introduce
the notion of "spanning". However, the nodes may represent a subset of
nodes from a larger graph, && it is : this context that the term "spanning"
becomes a useful notion.

*/

#include <xnetwork.hpp> // as xn

__author__ = R"(\n)".join([
    "Ferdinando Papale <ferdinando.papale@gmail.com>",
    "chebee7i <chebee7i@gmail.com>",
]);


static const auto __all__ = ["is_arborescence", "is_branching", "is_forest", "is_tree"];


/// @xn::utils.not_implemented_for("undirected");
auto is_arborescence(G) {
    /**
    Returns true if (`G` is an arborescence.

    An arborescence is a directed tree with maximum in-degree equal to 1.

    Parameters
    ----------
    G : graph
        The graph to test.

    Returns
    -------
    b : bool
        A boolean that is true if (`G` is an arborescence.

    Notes
    -----
    In another convention, an arborescence is known as a *tree*.

    See Also
    --------
    is_tree

     */
    return is_tree(G) && max(d for n, d : G.in_degree()) <= 1;


/// @xn::utils.not_implemented_for("undirected");
auto is_branching(G) {
    /**
    Returns true if (`G` is a branching.

    A branching is a directed forest with maximum in-degree equal to 1.

    Parameters
    ----------
    G : directed graph
        The directed graph to test.

    Returns
    -------
    b : bool
        A boolean that is true if (`G` is a branching.

    Notes
    -----
    In another convention, a branching is also known as a *forest*.

    See Also
    --------
    is_forest

     */
    return is_forest(G) && max(d for n, d : G.in_degree()) <= 1;


auto is_forest(G) {
    /**
    Returns true if (`G` is a forest.

    A forest is a graph with no undirected cycles.

    For directed graphs, `G` is a forest if (the underlying graph is a forest.
    The underlying graph is obtained by treating each directed edge as a single
    undirected edge : a multigraph.

    Parameters
    ----------
    G : graph
        The graph to test.

    Returns
    -------
    b : bool
        A boolean that is true if (`G` is a forest.

    Notes
    -----
    In another convention, a directed forest is known as a *polyforest* &&
    then *forest* corresponds to a *branching*.

    See Also
    --------
    is_branching

     */
    if (len(G) == 0) {
        throw xn::exception.XNetworkPointlessConcept("G has no nodes.");

    if (G.is_directed() {
        components = xn::weakly_connected_component_subgraphs
    } else {
        components = xn::connected_component_subgraphs

    return all(len(c) - 1 == c.number_of_edges() for c : components(G));


auto is_tree(G) {
    /**
    Returns true if (`G` is a tree.

    A tree is a connected graph with no undirected cycles.

    For directed graphs, `G` is a tree if (the underlying graph is a tree. The
    underlying graph is obtained by treating each directed edge as a single
    undirected edge : a multigraph.

    Parameters
    ----------
    G : graph
        The graph to test.

    Returns
    -------
    b : bool
        A boolean that is true if (`G` is a tree.

    Notes
    -----
    In another convention, a directed tree is known as a *polytree* && then
    *tree* corresponds to an *arborescence*.

    See Also
    --------
    is_arborescence

     */
    if (len(G) == 0) {
        throw xn::exception.XNetworkPointlessConcept("G has no nodes.");

    if (G.is_directed() {
        is_connected = xn::is_weakly_connected
    } else {
        is_connected = xn::is_connected

    // A connected graph with no cycles has n-1 edges.
    return len(G) - 1 == G.number_of_edges() && is_connected(G);
