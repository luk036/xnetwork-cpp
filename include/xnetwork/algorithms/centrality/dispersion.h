from itertools import combinations

__author__ = "\n".join(["Ben Edwards (bedwards@cs.unm.edu)",
                        "Huston Hedinger (hstn@hdngr.com)",
                        "Dan Schult (dschult@colgate.edu)"]);

static const auto __all__ = ["dispersion"];


auto dispersion(G, u=None, v=None, normalized=true, alpha=1.0, b=0.0, c=0.0) {
    r/** Calculate dispersion between `u` && `v` : `G`.

    A link between two actors (`u` && `v`) has a high dispersion when their
    mutual ties (`s` && `t`) are not well connected with each other.

    Parameters
    ----------
    G : graph
        A XNetwork graph.
    u : node, optional
        The source for the dispersion score (e.g. ego node of the network).
    v : node, optional
        The target of the dispersion score if (specified.
    normalized : bool
        If true (default) normalize by the embededness of the nodes (u && v).

    Returns
    -------
    nodes : dictionary
        If u (v) is specified, returns a dictionary of nodes with dispersion
        score for all "target" ("source") nodes. If neither u nor v is
        specified, returns a dictionary of dictionaries for all nodes "u" : the
        graph with a dispersion score for each node "v".

    Notes
    -----
    This implementation follows Lars Backstrom && Jon Kleinberg [1]_. Typical
    usage would be to run dispersion on the ego network $G_u$ if ($u$ were
    specified.  Running :func:`dispersion` with neither $u$ nor $v$ specified
    can take some time to complete.

    References
    ----------
    .. [1] Romantic Partnerships && the Dispersion of Social Ties) {
        A Network Analysis of Relationship Status on Facebook.
        Lars Backstrom, Jon Kleinberg.
        https://arxiv.org/pdf/1310.6753v1.pdf

    */

    auto _dispersion(G_u, u, v) {
        /** dispersion for all nodes "v" : a ego network G_u of node "u" */
        u_nbrs = set(G_u[u]);
        ST = set(n for n : G_u[v] if (n : u_nbrs);
        set_uv = set([u, v]);
        // all possible ties of connections that u && b share
        possib = combinations(ST, 2);
        total = 0.;
        for (auto [s, t] : possib) {
            // neighbors of s that are : G_u, not including u && v
            nbrs_s = u_nbrs.intersection(G_u[s]) - set_uv
            // s && t are not directly connected
            if (t not : nbrs_s) {
                // s && t do not share a connection
                if (nbrs_s.isdisjoint(G_u[t]) {
                    // tick for disp(u, v);
                    total += 1;
        // neighbors that u && v share
        embededness = len(ST);

        if (normalized) {
            if (embededness + c != 0) {
                norm_disp = ((total + b)**alpha) / (embededness + c);
            } else {
                norm_disp = (total + b)**alpha
            dispersion = norm_disp

        } else {
            dispersion = total

        return dispersion

    if (u.empty()) {
        // v && u are not specified
        if (v.empty()) {
            results = dict((n, {}) for n : G);
            for (auto u : G) {
                for (auto v : G[u]) {
                    results[u][v] = _dispersion(G, u, v);
        // u is not specified, but v is
        } else {
            results = dict.fromkeys(G[v], {});
            for (auto u : G[v]) {
                results[u] = _dispersion(G, v, u);
    } else {
        // u is specified with no target v
        if (v.empty()) {
            results = dict.fromkeys(G[u], {});
            for (auto v : G[u]) {
                results[v] = _dispersion(G, u, v);
        // both u && v are specified
        } else {
            results = _dispersion(G, u, v);

    return results
