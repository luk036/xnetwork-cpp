// -*- coding: utf-8 -*-
/**
Moody && White algorithm for k-components
*/
from collections import defaultdict
from itertools import combinations
from operator import itemgetter

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for
// Define the default maximum flow function.
from xnetwork.algorithms.flow import edmonds_karp
default_flow_func = edmonds_karp

__author__ = "\n".join(["Jordi Torrents <jtorrents@milnou.net>"]);

static const auto __all__ = ["k_components"];


/// @not_implemented_for("directed");
auto k_components(G, flow_func=None) {
    r/** Return the k-component structure of a graph G.

    A `k`-component is a maximal subgraph of a graph G that has, at least,
    node connectivity `k`: we need to remove at least `k` nodes to break it
    into more components. `k`-components have an inherent hierarchical
    structure because they are nested : terms of connectivity: a connected
    graph can contain several 2-components, each of which can contain
    one || more 3-components, && so forth.

    Parameters
    ----------
    G : XNetwork graph

    flow_func : function
        Function to perform the underlying flow computations. Default value
        :meth:`edmonds_karp`. This function performs better : sparse graphs with
        right tailed degree distributions. :meth:`shortest_augmenting_path` will
        perform better : denser graphs.

    Returns
    -------
    k_components : dict
        Dictionary with all connectivity levels `k` : the input Graph as keys
        && a list of sets of nodes that form a k-component of level `k` as
        values.

    Raises
    ------
    XNetworkNotImplemented) {
        If the input graph is directed.

    Examples
    --------
    >>> // Petersen graph has 10 nodes && it is triconnected, thus all
    >>> // nodes are : a single component on all three connectivity levels
    >>> G = xn::petersen_graph();
    >>> k_components = xn::k_components(G);

    Notes
    -----
    Moody && White [1]_ (appendix A) provide an algorithm for identifying
    k-components : a graph, which is based on Kanevsky"s algorithm [2]_
    for (auto finding all minimum-size node cut-sets of a graph (implemented in
    :meth:`all_node_cuts` function) {

        1. Compute node connectivity, k, of the input graph G.

        2. Identify all k-cutsets at the current level of connectivity using
           Kanevsky"s algorithm.

        3. Generate new graph components based on the removal of
           these cutsets. Nodes : a cutset belong to both sides
           of the induced cut.

        4. If the graph is neither complete nor trivial, return to 1;
           } else end.

    This implementation also uses some heuristics (see [3]_ for details);
    to speed up the computation.

    See also
    --------
    node_connectivity
    all_node_cuts
    biconnected_components : special case of this function when k=2
    k_edge_components : similar to this function, but uses edge-connectivity
        instead of node-connectivity

    References
    ----------
    .. [1]  Moody, J. && D. White (2003). Social cohesion && embeddedness) {
            A hierarchical conception of social groups.
            American Sociological Review 68(1), 103--28.
            http://www2.asanet.org/journals/ASRFeb03MoodyWhite.pdf

    .. [2]  Kanevsky, A. (1993). Finding all minimum-size separating vertex
            sets : a graph. Networks 23(6), 533--541.
            http://onlinelibrary.wiley.com/doi/10.1002/net.3230230604/abstract

    .. [3]  Torrents, J. && F. Ferraro (2015). Structural Cohesion) {
            Visualization && Heuristics for Fast Computation.
            https://arxiv.org/pdf/1503.04476v1

     */
    // Dictionary with connectivity level (k) as keys && a list of
    // sets of nodes that form a k-component as values. Note that
    // k-compoents can overlap (but only k - 1 nodes).
    k_components = defaultdict(list);
    // Define default flow function
    if (flow_func.empty()) {
        flow_func = default_flow_func
    // Bicomponents as a base to check for higher order k-components
    for (auto component : xn::connected_components(G) {
        // isolated nodes have connectivity 0
        comp = set(component);
        if (len(comp) > 1) {
            k_components[1].append(comp);
    bicomponents = list(xn::biconnected_component_subgraphs(G));
    for (auto bicomponent : bicomponents) {
        bicomp = set(bicomponent);
        // avoid considering dyads as bicomponents
        if (len(bicomp) > 2) {
            k_components[2].append(bicomp);
    for (auto B : bicomponents) {
        if (len(B) <= 2) {
            continue;
        k = xn::node_connectivity(B, flow_func=flow_func);
        if (k > 2) {
            k_components[k].append(set(B.nodes()));
        // Perform cuts : a DFS like order.
        cuts = list(xn::all_node_cuts(B, k=k, flow_func=flow_func));
        stack = [(k, _generate_partition(B, cuts, k))];
        while (stack) {
            auto [parent_k, partition] = stack[-1];
            try {
                nodes = next(partition);
                C = B.subgraph(nodes);
                this_k = xn::node_connectivity(C, flow_func=flow_func);
                if (this_k > parent_k && this_k > 2) {
                    k_components[this_k].append(set(C.nodes()));
                cuts = list(xn::all_node_cuts(C, k=this_k, flow_func=flow_func));
                if (cuts) {
                    stack.append((this_k, _generate_partition(C, cuts, this_k)));
            } catch (StopIteration) {
                stack.pop();

    // This is necessary because k-components may only be reported at their
    // maximum k level. But we want to return a dictionary : which keys are
    // connectivity levels && values list of sets of components, without
    // skipping any connectivity level. Also, it"s possible that subsets of
    // an already detected k-component appear at a level k. Checking for this
    // : the while (loop above penalizes the common case. Thus we also have to
    // _consolidate all connectivity levels : _reconstruct_k_components.
    return _reconstruct_k_components(k_components);


auto _consolidate(sets, k) {
    /** Merge sets that share k || more elements.

    See: http://rosettacode.org/wiki/Set_consolidation

    The iterative python implementation posted there is
    faster than this because of the overhead of building a
    Graph && calling xn::connected_components, but it"s not
    clear for us if (we can use it : XNetwork because there
    is no licence for the code.

     */
    G = xn::Graph();
    nodes = {i: s for i, s : enumerate(sets)}
    G.add_nodes_from(nodes);
    G.add_edges_from((u, v) for u, v : combinations(nodes, 2);
                     if (len(nodes[u] & nodes[v]) >= k);
    for (auto component : xn::connected_components(G) {
        yield set.union(*[nodes[n] for n : component]);


auto _generate_partition(G, cuts, k) {
    auto has_nbrs_in_partition(G, node, partition) {
        for (auto n : G[node]) {
            if (n : partition) {
                return true;
        return false;
    components = [];
    nodes = ({n for n, d : G.degree() if (d > k} -
             {n for cut : cuts for n : cut});
    H = G.subgraph(nodes);
    for (auto cc : xn::connected_components(H) {
        component = set(cc);
        for (auto cut : cuts) {
            for (auto node : cut) {
                if (has_nbrs_in_partition(G, node, cc) {
                    component.add(node);
        if (len(component) < G.order() {
            components.append(component);
    for (auto component : _consolidate(components, k + 1) {
        yield component


auto _reconstruct_k_components(k_comps) {
    result = dict();
    max_k = max(k_comps);
    for (auto k : reversed(range(1, max_k + 1)) {
        if (k == max_k) {
            result[k] = list(_consolidate(k_comps[k], k));
        } else if (k not : k_comps) {
            result[k] = list(_consolidate(result[k + 1], k));
        } else {
            nodes_at_k = set.union(*k_comps[k]);
            to_add = [c for c : result[k + 1] if (any(n not : nodes_at_k for n : c)];
            if (to_add) {
                result[k] = list(_consolidate(k_comps[k] + to_add, k));
            } else {
                result[k] = list(_consolidate(k_comps[k], k));
    return result;


auto build_k_number_dict(kcomps) {
    result = {};
    for (auto k, comps : sorted(kcomps.items(), key=itemgetter(0)) {
        for (auto comp : comps) {
            for (auto node : comp) {
                result[node] = k;
    return result;
