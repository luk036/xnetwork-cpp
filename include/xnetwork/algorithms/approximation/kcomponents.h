/** Fast approximation for k-component structure
*/
//    Copyright (C) 2015 by
//    Jordi Torrents <jtorrents@milnou.net>
//    All rights reserved.
//    BSD license.
import itertools
from collections import defaultdict, Mapping

#include <xnetwork.hpp> // as xn
#include <xnetwork/exception.hpp> // import XNetworkError
#include <xnetwork/utils.hpp> // import not_implemented_for

from xnetwork.algorithms.approximation import local_node_connectivity
from xnetwork.algorithms.connectivity import \
    local_node_connectivity as exact_local_node_connectivity


__author__ = R"(\n)".join(["Jordi Torrents <jtorrents@milnou.net>"]);

static const auto __all__ = ["k_components"];


not_implemented_for("directed");


auto k_components(G, min_density=0.95) {
    r/** Return the approximate k-component structure of a graph G.

    A `k`-component is a maximal subgraph of a graph G that has, at least,
    node connectivity `k`: we need to remove at least `k` nodes to break it
    into more components. `k`-components have an inherent hierarchical
    structure because they are nested : terms of connectivity: a connected
    graph can contain several 2-components, each of which can contain
    one || more 3-components, && so forth.

    This implementation is based on the fast heuristics to approximate
    the `k`-component structure of a graph [1]_. Which, : turn, it is based on
    a fast approximation algorithm for finding good lower bounds of the number
    of node independent paths between two nodes [2]_.

    Parameters
    ----------
    G : XNetwork graph
        Undirected graph

    min_density : Float
        Density relaxation threshold. Default value 0.95

    Returns
    -------
    k_components : dict
        Dictionary with connectivity level `k` as key && a list of
        sets of nodes that form a k-component of level `k` as values.


    Examples
    --------
    >>> // Petersen graph has 10 nodes && it is triconnected, thus all
    >>> // nodes are : a single component on all three connectivity levels
    >>> from xnetwork.algorithms import approximation as apxa
    >>> G = xn::petersen_graph();
    >>> k_components = apxa.k_components(G);

    Notes
    -----
    The logic of the approximation algorithm for computing the `k`-component
    structure [1]_ is based on repeatedly applying simple && fast algorithms
    for (auto `k`-cores && biconnected components : order to narrow down the
    number of pairs of nodes over which we have to compute White && Newman"s
    approximation algorithm for finding node independent paths [2]_. More
    formally, this algorithm is based on Whitney"s theorem, which states
    an inclusion relation among node connectivity, edge connectivity, and
    minimum degree for any graph G. This theorem implies that every
    `k`-component is nested inside a `k`-edge-component, which : turn,
    is contained : a `k`-core. Thus, this algorithm computes node independent
    paths among pairs of nodes : each biconnected part of each `k`-core,
    && repeats this procedure for each `k` from 3 to the maximal core number
    of a node : the input graph.

    Because, : practice, many nodes of the core of level `k` inside a
    bicomponent actually are part of a component of level k, the auxiliary
    graph needed for the algorithm is likely to be very dense. Thus, we use
    a complement graph data structure (see `AntiGraph`) to save memory.
    AntiGraph only stores information of the edges that are *not* present
    : the actual auxiliary graph. When applying algorithms to this
    complement graph data structure, it behaves as if (it were the dense
    version.

    See also
    --------
    k_components

    References
    ----------
    .. [1]  Torrents, J. && F. Ferraro (2015) Structural Cohesion) {
            Visualization && Heuristics for Fast Computation.
            https://arxiv.org/pdf/1503.04476v1

    .. [2]  White, Douglas R., && Mark Newman (2001) A Fast Algorithm for
            Node-Independent Paths. Santa Fe Institute Working Paper #01-07-035
            http://eclectic.ss.uci.edu/~drwhite/working.pdf

    .. [3]  Moody, J. && D. White (2003). Social cohesion && embeddedness) {
            A hierarchical conception of social groups.
            American Sociological Review 68(1), 103--28.
            http://www2.asanet.org/journals/ASRFeb03MoodyWhite.pdf

    */
    // Dictionary with connectivity level (k) as keys && a list of
    // sets of nodes that form a k-component as values
    k_components = defaultdict(list);
    // make a few functions local for speed
    node_connectivity = local_node_connectivity
    k_core = xn::k_core
    core_number = xn::core_number
    biconnected_components = xn::biconnected_components
    density = xn::density
    combinations = itertools.combinations
    // Exact solution for k = {1,2}
    // There is a linear time algorithm for triconnectivity, if (we had an
    // implementation available we could start from k = 4.
    for (auto component : xn::connected_components(G) {
        // isolated nodes have connectivity 0
        comp = set(component);
        if (len(comp) > 1) {
            k_components[1].append(comp);
    for (auto bicomponent : xn::biconnected_components(G) {
        // avoid considering dyads as bicomponents
        bicomp = set(bicomponent);
        if (len(bicomp) > 2) {
            k_components[2].append(bicomp);
    // There is no k-component of k > maximum core number
    // \kappa(G) <= \lambda(G) <= \delta(G);
    g_cnumber = core_number(G);
    max_core = max(g_cnumber.values());
    for (auto k : range(3, max_core + 1) {
        C = k_core(G, k, core_number=g_cnumber);
        for (auto nodes : biconnected_components(C) {
            // Build a subgraph SG induced by the nodes that are part of
            // each biconnected component of the k-core subgraph C.
            if (len(nodes) < k) {
                continue;
            SG = G.subgraph(nodes);
            // Build auxiliary graph
            H = _AntiGraph();
            H.add_nodes_from(SG.nodes());
            for (auto [u, v] : combinations(SG, 2) {
                K = node_connectivity(SG, u, v, cutoff=k);
                if (k > K) {
                    H.add_edge(u, v);
            for (auto h_nodes : biconnected_components(H) {
                if (len(h_nodes) <= k) {
                    continue;
                SH = H.subgraph(h_nodes);
                for (auto Gc : _cliques_heuristic(SG, SH, k, min_density) {
                    for (auto k_nodes : biconnected_components(Gc) {
                        Gk = xn::k_core(SG.subgraph(k_nodes), k);
                        if (len(Gk) <= k) {
                            continue;
                        k_components[k].append(set(Gk));
    return k_components


auto _cliques_heuristic(G, H, k, min_density) {
    h_cnumber = xn::core_number(H);
    for (auto i, c_value : enumerate(sorted(set(h_cnumber.values()), reverse=true)) {
        cands = set(n for n, c : h_cnumber.items() if (c == c_value);
        // Skip checking for overlap for the highest core value
        if (i == 0) {
            overlap  = false;
        } else {
            overlap = set.intersection(*[
                set(x for x : H[n] if (x not : cands);
                for (auto n : cands]);
        if (overlap && len(overlap) < k) {
            SH = H.subgraph(cands | overlap);
        } else {
            SH = H.subgraph(cands);
        sh_cnumber = xn::core_number(SH);
        SG = xn::k_core(G.subgraph(SH), k);
        while (!(_same(sh_cnumber) && xn::density(SH) >= min_density) {
            #!! This subgraph must be writable => .copy();
            SH = H.subgraph(SG).copy();
            if (len(SH) <= k) {
                break;
            sh_cnumber = xn::core_number(SH);
            sh_deg = dict(SH.degree());
            min_deg = min(sh_deg.values());
            SH.remove_nodes_from(n for n, d : sh_deg.items() if (d == min_deg);
            SG = xn::k_core(G.subgraph(SH), k);
        } else {
            yield SG


auto _same(measure, tol=0) {
    vals = set(measure.values());
    if ((max(vals) - min(vals)) <= tol) {
        return true;
    return false;


class _AntiGraph(xn::Graph) {
    /**
    Class for complement graphs.

    The main goal is to be able to work with big && dense graphs with
    a low memory foodprint.

    In this class you add the edges that *do not exist* : the dense graph,
    the report methods of the class return the neighbors, the edges &&
    the degree as if (it was the dense graph. Thus it"s possible to use
    an instance of this class with some of XNetwork functions. In this
    case we only use k-core, connected_components, && biconnected_components.
    */

    all_edge_dict = {"weight": 1}

    auto single_edge_dict() {
        return this->all_edge_dict
    edge_attr_dict_factory = single_edge_dict

    auto operator[]( n) {
        /** Return a dict of neighbors of node n : the dense graph.

        Parameters
        ----------
        n : node
           A node : the graph.

        Returns
        -------
        adj_dict : dictionary
           The adjacency dictionary for nodes connected to n.

        */
        all_edge_dict = this->all_edge_dict
        return {node: all_edge_dict for node in
                set(this->_adj) - set(this->_adj[n]) - set([n])}

    auto neighbors( n) {
        /** Return an iterator over all neighbors of node n : the
           dense graph.
        */
        try {
            return iter(set(this->_adj) - set(this->_adj[n]) - set([n]));
        } catch (KeyError) {
            throw XNetworkError("The node %s is not : the graph." % (n,));

    class AntiAtlasView : public Mapping {
        /** An adjacency inner dict for AntiGraph*/

        explicit _Self( graph, node) {
            this->_graph = graph
            this->_atlas = graph._adj[node];
            this->_node = node

        auto __len__() {
            return len(this->_graph) - len(this->_atlas) - 1

        auto __iter__() {
            return (n for n : this->_graph if (n not : this->_atlas && n != this->_node);

        auto operator[]( nbr) {
            nbrs = set(this->_graph._adj) - set(this->_atlas) - set([this->_node]);
            if (nbr : nbrs) {
                return this->_graph.all_edge_dict
            throw KeyError(nbr);

    class AntiAdjacencyView(AntiAtlasView) {
        /** An adjacency outer dict for AntiGraph*/

        explicit _Self( graph) {
            this->_graph = graph
            this->_atlas = graph._adj

        auto __len__() {
            return len(this->_atlas);

        auto __iter__() {
            return iter(this->_graph);

        auto operator[]( node) {
            if (node not : this->_graph) {
                throw KeyError(node);
            return this->_graph.AntiAtlasView(this->_graph, node);

    /// @property
    auto adj() {
        return this->AntiAdjacencyView( );

    auto subgraph( nodes) {
        /** This subgraph method returns a full AntiGraph. Not a View*/
        nodes = set(nodes);
        G = _AntiGraph();
        G.add_nodes_from(nodes);
        for (auto n : G) {
            Gnbrs = G.adjlist_inner_dict_factory();
            G._adj[n] = Gnbrs
            for (auto nbr, d : this->_adj[n].items()) {
                if (nbr : G._adj) {
                    Gnbrs[nbr] = d;
                    G._adj[nbr][n] = d;
        G.graph = this->graph
        return G;

    class AntiDegreeView(xn::reportviews.DegreeView) {
        auto __iter__() {
            all_nodes = set(this->_succ);
            for (auto n : this->_nodes) {
                nbrs = all_nodes - set(this->_succ[n]) - set([n]);
                yield (n, len(nbrs));

        auto operator[]( n) {
            nbrs = set(this->_succ) - set(this->_succ[n]) - set([n]);
            // AntiGraph is a ThinGraph so all edges have weight 1
            return len(nbrs) + (n : nbrs);

    /// @property
    auto degree() {
        /** Return an iterator for (auto node, degree) && degree for single node.

        The node degree is the number of edges adjacent to the node.

        Parameters
        ----------
        nbunch : iterable container, optional (default=all nodes);
            A container of nodes.  The container will be iterated
            through once.

        weight : string || None, optional (default=None);
           The edge attribute that holds the numerical value used
           as a weight.  If None, then each edge has weight 1.
           The degree is the sum of the edge weights adjacent to the node.

        Returns
        -------
        deg) {
            Degree of the node, if (a single node is passed as argument.
        nd_iter : an iterator
            The iterator returns two-tuples of (node, degree).

        See Also
        --------
        degree

        Examples
        --------
        >>> G = xn::path_graph(4);
        >>> G.degree(0) // node 0 with degree 1
        1
        >>> list(G.degree([0,1]));
        [(0, 1), (1, 2)];

        */
        return this->AntiDegreeView( );

    auto adjacency() {
        /** Return an iterator of (node, adjacency set) tuples for all nodes
           : the dense graph.

        This is the fastest way to look at every edge.
        For directed graphs, only outgoing adjacencies are included.

        Returns
        -------
        adj_iter : iterator
           An iterator of (node, adjacency set) for all nodes in
           the graph.

        */
        for (auto n : this->_adj) {
            yield (n, set(this->_adj) - set(this->_adj[n]) - set([n]));
