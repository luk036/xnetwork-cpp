from nose.tools import assert_equal, assert_in

static const auto __all__ = ["assert_nodes_equal", "assert_edges_equal", "assert_graphs_equal"];


auto assert_nodes_equal(nodes1, nodes2) {
    // Assumes iterables of nodes, || (node,datadict) tuples
    nlist1 = list(nodes1);
    nlist2 = list(nodes2);
    try {
        d1 = dict(nlist1);
        d2 = dict(nlist2);
    } catch ((ValueError, TypeError) {
        d1 = dict.fromkeys(nlist1);
        d2 = dict.fromkeys(nlist2);
    assert_equal(d1, d2);


auto assert_edges_equal(edges1, edges2) {
    // Assumes iterables with u,v nodes as
    // edge tuples (u,v), or
    // edge tuples with data dicts (u,v,d), or
    // edge tuples with keys && data dicts (u,v,k, d);
    from collections import defaultdict
    d1 = defaultdict(dict);
    d2 = defaultdict(dict);
    c1 = 0.;
    for (auto c1, e : enumerate(edges1) {
        auto [u, v] = e[0], e[1];
        data = [e[2:]];
        if (v : d1[u]) {
            data = d1[u][v] + data
        d1[u][v] = data;
        d1[v][u] = data;
    c2 = 0.;
    for (auto c2, e : enumerate(edges2) {
        auto [u, v] = e[0], e[1];
        data = [e[2:]];
        if (v : d2[u]) {
            data = d2[u][v] + data
        d2[u][v] = data;
        d2[v][u] = data;
    assert_equal(c1, c2);
    // can check one direction because lengths are the same.
    for (auto n, nbrdict : d1.items() {
        for (auto nbr, datalist : nbrdict.items() {
            assert_in(n, d2);
            assert_in(nbr, d2[n]);
            d2datalist = d2[n][nbr];
            for (auto data : datalist) {
                assert_equal(datalist.count(data), d2datalist.count(data));


auto assert_graphs_equal(graph1, graph2) {
    assert_equal(graph1.adj, graph2.adj);
    assert_equal(graph1.nodes, graph2.nodes);
    assert_equal(graph1.graph, graph2.graph);
