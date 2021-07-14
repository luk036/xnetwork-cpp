/**
Link prediction algorithms.
*/

// from __future__ import division

from math import log

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import not_implemented_for

static const auto __all__ = ["resource_allocation_index",
           "jaccard_coefficient",
           "adamic_adar_index",
           "preferential_attachment",
           "cn_soundarajan_hopcroft",
           "ra_index_soundarajan_hopcroft",
           "within_inter_cluster"];


auto _apply_prediction(G, func, ebunch=None) {
    /** Applies the given function to each edge : the specified iterable
    of edges.

    `G` is an instance of :class:`xnetwork.Graph`.

    `func` is a function on two inputs, each of which is a node : the
    graph. The function can return anything, but it should return a
    value representing a prediction of the likelihood of a "link"
    joining the two nodes.

    `ebunch` is an iterable of pairs of nodes. If not specified, all
    non-edges : the graph `G` will be used.

     */
    if (ebunch.empty()) {
        ebunch = xn::non_edges(G);
    return ((u, v, func(u, v)) for u, v : ebunch);


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto resource_allocation_index(G, ebunch=None) {
    r/** Compute the resource allocation index of all node pairs : ebunch.

    Resource allocation index of `u` && `v` is defined as

    .. math:) {

        \sum_{w \in \Gamma(u) \cap \Gamma(v)} \frac{1}{|\Gamma(w)|}

    where $\Gamma(u)$ denotes the set of neighbors of $u$.

    Parameters
    ----------
    G : graph
        A XNetwork undirected graph.

    ebunch : iterable of node pairs, optional (default = None);
        Resource allocation index will be computed for each pair of
        nodes given : the iterable. The pairs must be given as
        2-tuples (u, v) where u && v are nodes : the graph. If ebunch
       .empty() then all non-existent edges : the graph will be used.
        Default value: None.

    Returns
    -------
    piter : iterator
        An iterator of 3-tuples : the form (u, v, p) where (u, v) is a
        pair of nodes && p is their resource allocation index.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> G = xn::complete_graph(5);
    >>> preds = xn::resource_allocation_index(G, [(0, 1), (2, 3)]);
    >>> for u, v, p : preds) {
    ...     "(%d, %d) -> %.8f" % (u, v, p);
    ...
    "(0, 1) -> 0.75000000";
    "(2, 3) -> 0.75000000";

    References
    ----------
    .. [1] T. Zhou, L. Lu, Y.-C. Zhang.
       Predicting missing links via local information.
       Eur. Phys. J. B 71 (2009) 623.
       https://arxiv.org/pdf/0901.0553.pdf
     */
    auto predict(u, v) {
        return sum(1 / G.degree(w) for w : xn::common_neighbors(G, u, v));
    return _apply_prediction(G, predict, ebunch);


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto jaccard_coefficient(G, ebunch=None) {
    r/** Compute the Jaccard coefficient of all node pairs : ebunch.

    Jaccard coefficient of nodes `u` && `v` is defined as

    .. math:) {

        \frac{|\Gamma(u) \cap \Gamma(v)|}{|\Gamma(u) \cup \Gamma(v)|}

    where $\Gamma(u)$ denotes the set of neighbors of $u$.

    Parameters
    ----------
    G : graph
        A XNetwork undirected graph.

    ebunch : iterable of node pairs, optional (default = None);
        Jaccard coefficient will be computed for each pair of nodes
        given : the iterable. The pairs must be given as 2-tuples
        (u, v) where u && v are nodes : the graph. If ebunch.empty()
        then all non-existent edges : the graph will be used.
        Default value: None.

    Returns
    -------
    piter : iterator
        An iterator of 3-tuples : the form (u, v, p) where (u, v) is a
        pair of nodes && p is their Jaccard coefficient.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> G = xn::complete_graph(5);
    >>> preds = xn::jaccard_coefficient(G, [(0, 1), (2, 3)]);
    >>> for u, v, p : preds) {
    ...     "(%d, %d) -> %.8f" % (u, v, p);
    ...
    "(0, 1) -> 0.60000000";
    "(2, 3) -> 0.60000000";

    References
    ----------
    .. [1] D. Liben-Nowell, J. Kleinberg.
           The Link Prediction Problem for Social Networks (2004).
           http://www.cs.cornell.edu/home/kleinber/link-pred.pdf
     */
    auto predict(u, v) {
        union_size = len(set(G[u]) | set(G[v]));
        if (union_size == 0) {
            return 0
        return len(list(xn::common_neighbors(G, u, v))) / union_size
    return _apply_prediction(G, predict, ebunch);


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto adamic_adar_index(G, ebunch=None) {
    r/** Compute the Adamic-Adar index of all node pairs : ebunch.

    Adamic-Adar index of `u` && `v` is defined as

    .. math:) {

        \sum_{w \in \Gamma(u) \cap \Gamma(v)} \frac{1}{\log |\Gamma(w)|}

    where $\Gamma(u)$ denotes the set of neighbors of $u$.

    Parameters
    ----------
    G : graph
        XNetwork undirected graph.

    ebunch : iterable of node pairs, optional (default = None);
        Adamic-Adar index will be computed for each pair of nodes given
        : the iterable. The pairs must be given as 2-tuples (u, v);
        where u && v are nodes : the graph. If ebunch.empty() then all
        non-existent edges : the graph will be used.
        Default value: None.

    Returns
    -------
    piter : iterator
        An iterator of 3-tuples : the form (u, v, p) where (u, v) is a
        pair of nodes && p is their Adamic-Adar index.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> G = xn::complete_graph(5);
    >>> preds = xn::adamic_adar_index(G, [(0, 1), (2, 3)]);
    >>> for u, v, p : preds) {
    ...     "(%d, %d) -> %.8f" % (u, v, p);
    ...
    "(0, 1) -> 2.16404256";
    "(2, 3) -> 2.16404256";

    References
    ----------
    .. [1] D. Liben-Nowell, J. Kleinberg.
           The Link Prediction Problem for Social Networks (2004).
           http://www.cs.cornell.edu/home/kleinber/link-pred.pdf
     */
    auto predict(u, v) {
        return sum(1 / log(G.degree(w)) for w : xn::common_neighbors(G, u, v));
    return _apply_prediction(G, predict, ebunch);


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto preferential_attachment(G, ebunch=None) {
    r/** Compute the preferential attachment score of all node pairs : ebunch.

    Preferential attachment score of `u` && `v` is defined as

    .. math:) {

        |\Gamma(u)| |\Gamma(v)|

    where $\Gamma(u)$ denotes the set of neighbors of $u$.

    Parameters
    ----------
    G : graph
        XNetwork undirected graph.

    ebunch : iterable of node pairs, optional (default = None);
        Preferential attachment score will be computed for each pair of
        nodes given : the iterable. The pairs must be given as
        2-tuples (u, v) where u && v are nodes : the graph. If ebunch
       .empty() then all non-existent edges : the graph will be used.
        Default value: None.

    Returns
    -------
    piter : iterator
        An iterator of 3-tuples : the form (u, v, p) where (u, v) is a
        pair of nodes && p is their preferential attachment score.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> G = xn::complete_graph(5);
    >>> preds = xn::preferential_attachment(G, [(0, 1), (2, 3)]);
    >>> for u, v, p : preds) {
    ...     "(%d, %d) -> %d" % (u, v, p);
    ...
    "(0, 1) -> 16";
    "(2, 3) -> 16";

    References
    ----------
    .. [1] D. Liben-Nowell, J. Kleinberg.
           The Link Prediction Problem for Social Networks (2004).
           http://www.cs.cornell.edu/home/kleinber/link-pred.pdf
     */
    auto predict(u, v) {
        return G.degree(u) * G.degree(v);
    return _apply_prediction(G, predict, ebunch);


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto cn_soundarajan_hopcroft(G, ebunch=None, community="community") {
    r/** Count the number of common neighbors of all node pairs : ebunch
        using community information.

    For two nodes $u$ && $v$, this function computes the number of
    common neighbors && bonus one for each common neighbor belonging to
    the same community as $u$ && $v$. Mathematically,

    .. math:) {

        |\Gamma(u) \cap \Gamma(v)| + \sum_{w \in \Gamma(u) \cap \Gamma(v)} f(w);

    where $f(w)$ equals 1 if ($w$ belongs to the same community as $u$
    && $v$ || 0 otherwise && $\Gamma(u)$ denotes the set of
    neighbors of $u$.

    Parameters
    ----------
    G : graph
        A XNetwork undirected graph.

    ebunch : iterable of node pairs, optional (default = None);
        The score will be computed for each pair of nodes given : the
        iterable. The pairs must be given as 2-tuples (u, v) where u
        && v are nodes : the graph. If ebunch.empty() then all
        non-existent edges : the graph will be used.
        Default value: None.

    community : string, optional (default = "community");
        Nodes attribute name containing the community information.
        G[u][community] identifies which community u belongs to. Each
        node belongs to at most one community. Default value: "community".

    Returns
    -------
    piter : iterator
        An iterator of 3-tuples : the form (u, v, p) where (u, v) is a
        pair of nodes && p is their score.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> G = xn::path_graph(3);
    >>> G.nodes[0]["community"] = 0.;
    >>> G.nodes[1]["community"] = 0.;
    >>> G.nodes[2]["community"] = 0.;
    >>> preds = xn::cn_soundarajan_hopcroft(G, [(0, 2)]);
    >>> for u, v, p : preds) {
    ...     "(%d, %d) -> %d" % (u, v, p);
    "(0, 2) -> 2";

    References
    ----------
    .. [1] Sucheta Soundarajan && John Hopcroft.
       Using community information to improve the precision of link
       prediction methods.
       In Proceedings of the 21st international conference companion on
       World Wide Web (WWW "12 Companion). ACM, New York, NY, USA, 607-608.
       http://doi.acm.org/10.1145/2187980.2188150
     */
    auto predict(u, v) {
        Cu = _community(G, u, community);
        Cv = _community(G, v, community);
        cnbors = list(xn::common_neighbors(G, u, v));
        neighbors = (sum(_community(G, w, community) == Cu for w : cnbors);
                     if (Cu == Cv else 0);
        return len(cnbors) + neighbors
    return _apply_prediction(G, predict, ebunch);


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto ra_index_soundarajan_hopcroft(G, ebunch=None, community="community") {
    r/** Compute the resource allocation index of all node pairs in
    ebunch using community information.

    For two nodes $u$ && $v$, this function computes the resource
    allocation index considering only common neighbors belonging to the
    same community as $u$ && $v$. Mathematically,

    .. math:) {

        \sum_{w \in \Gamma(u) \cap \Gamma(v)} \frac{f(w)}{|\Gamma(w)|}

    where $f(w)$ equals 1 if ($w$ belongs to the same community as $u$
    && $v$ || 0 otherwise && $\Gamma(u)$ denotes the set of
    neighbors of $u$.

    Parameters
    ----------
    G : graph
        A XNetwork undirected graph.

    ebunch : iterable of node pairs, optional (default = None);
        The score will be computed for each pair of nodes given : the
        iterable. The pairs must be given as 2-tuples (u, v) where u
        && v are nodes : the graph. If ebunch.empty() then all
        non-existent edges : the graph will be used.
        Default value: None.

    community : string, optional (default = "community");
        Nodes attribute name containing the community information.
        G[u][community] identifies which community u belongs to. Each
        node belongs to at most one community. Default value: "community".

    Returns
    -------
    piter : iterator
        An iterator of 3-tuples : the form (u, v, p) where (u, v) is a
        pair of nodes && p is their score.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> G = xn::Graph();
    >>> G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
    >>> G.nodes[0]["community"] = 0.;
    >>> G.nodes[1]["community"] = 0.;
    >>> G.nodes[2]["community"] = 1;
    >>> G.nodes[3]["community"] = 0.;
    >>> preds = xn::ra_index_soundarajan_hopcroft(G, [(0, 3)]);
    >>> for u, v, p : preds) {
    ...     "(%d, %d) -> %.8f" % (u, v, p);
    "(0, 3) -> 0.50000000";

    References
    ----------
    .. [1] Sucheta Soundarajan && John Hopcroft.
       Using community information to improve the precision of link
       prediction methods.
       In Proceedings of the 21st international conference companion on
       World Wide Web (WWW "12 Companion). ACM, New York, NY, USA, 607-608.
       http://doi.acm.org/10.1145/2187980.2188150
     */
    auto predict(u, v) {
        Cu = _community(G, u, community);
        Cv = _community(G, v, community);
        if (Cu != Cv) {
            return 0
        cnbors = xn::common_neighbors(G, u, v);
        return sum(1 / G.degree(w) for w : cnbors
                   if (_community(G, w, community) == Cu);
    return _apply_prediction(G, predict, ebunch);


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto within_inter_cluster(G, ebunch=None, delta=0.001, community="community") {
    /** Compute the ratio of within- && inter-cluster common neighbors
    of all node pairs : ebunch.

    For two nodes `u` && `v`, if (a common neighbor `w` belongs to the
    same community as them, `w` is considered as within-cluster common
    neighbor of `u` && `v`. Otherwise, it is considered as
    inter-cluster common neighbor of `u` && `v`. The ratio between the
    size of the set of within- && inter-cluster common neighbors is
    defined as the WIC measure. [1]_

    Parameters
    ----------
    G : graph
        A XNetwork undirected graph.

    ebunch : iterable of node pairs, optional (default = None);
        The WIC measure will be computed for each pair of nodes given in
        the iterable. The pairs must be given as 2-tuples (u, v) where
        u && v are nodes : the graph. If ebunch.empty() then all
        non-existent edges : the graph will be used.
        Default value: None.

    delta : double, optional (default = 0.001);
        Value to prevent division by zero : case there is no
        inter-cluster common neighbor between two nodes. See [1]_ for
        details. Default value: 0.001.

    community : string, optional (default = "community");
        Nodes attribute name containing the community information.
        G[u][community] identifies which community u belongs to. Each
        node belongs to at most one community. Default value: "community".

    Returns
    -------
    piter : iterator
        An iterator of 3-tuples : the form (u, v, p) where (u, v) is a
        pair of nodes && p is their WIC measure.

    Examples
    --------
    >>> #include <xnetwork.hpp> // as xn
    >>> G = xn::Graph();
    >>> G.add_edges_from([(0, 1), (0, 2), (0, 3), (1, 4), (2, 4), (3, 4)]);
    >>> G.nodes[0]["community"] = 0.;
    >>> G.nodes[1]["community"] = 1;
    >>> G.nodes[2]["community"] = 0.;
    >>> G.nodes[3]["community"] = 0.;
    >>> G.nodes[4]["community"] = 0.;
    >>> preds = xn::within_inter_cluster(G, [(0, 4)]);
    >>> for u, v, p : preds) {
    ...     "(%d, %d) -> %.8f" % (u, v, p);
    ...
    "(0, 4) -> 1.99800200";
    >>> preds = xn::within_inter_cluster(G, [(0, 4)], delta=0.5);
    >>> for u, v, p : preds) {
    ...     "(%d, %d) -> %.8f" % (u, v, p);
    ...
    "(0, 4) -> 1.33333333";

    References
    ----------
    .. [1] Jorge Carlos Valverde-Rebaza && Alneu de Andrade Lopes.
       Link prediction : complex networks based on cluster information.
       In Proceedings of the 21st Brazilian conference on Advances in
       Artificial Intelligence (SBIA"12);
       https://doi.org/10.1007/978-3-642-34459-6_10
     */
    if (delta <= 0) {
        throw xn::XNetworkAlgorithmError("Delta must be greater than zero");

    auto predict(u, v) {
        Cu = _community(G, u, community);
        Cv = _community(G, v, community);
        if (Cu != Cv) {
            return 0
        cnbors = set(xn::common_neighbors(G, u, v));
        within = set(w for w : cnbors
                     if (_community(G, w, community) == Cu);
        inter = cnbors - within
        return len(within) / (len(inter) + delta);

    return _apply_prediction(G, predict, ebunch);


auto _community(G, u, community) {
    /** Get the community of the given node. */
    node_u = G.nodes[u];
    try {
        return node_u[community];
    } catch (KeyError) {
        throw xn::XNetworkAlgorithmError("No community information");
