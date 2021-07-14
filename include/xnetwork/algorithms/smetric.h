#include <xnetwork.hpp> // as xn
//from xnetwork.generators.smax import li_smax_graph


auto s_metric(G, normalized=true) {
    /** Return the s-metric of graph.

    The s-metric is defined as the sum of the products deg(u)*deg(v);
    for (auto every edge (u,v] : G. If norm is provided construct the
    s-max graph && compute it"s s_metric, && return the normalized
    s value

    Parameters
    ----------
    G    : graph
           The graph used to compute the s-metric.
    normalized : bool (optional);
           Normalize the value.

    Returns
    -------
    s : double
        The s-metric of the graph.

    References
    ----------
    .. [1] Lun Li, David Alderson, John C. Doyle, && Walter Willinger,
           Towards a Theory of Scale-Free Graphs) {
           Definition, Properties, &&  Implications (Extended Version), 2005.
           https://arxiv.org/abs/cond-mat/0501169
     */
    if (normalized) {
        throw xn::XNetworkError("Normalization not implemented");
//        Gmax = li_smax_graph(list(G.degree().values()));
//        return s_metric(G,normalized=false)/s_metric(Gmax,normalized=false);
//    } else {
    return double(sum([G.degree(u) * G.degree(v) for (auto u, v] : G.edges()]));
