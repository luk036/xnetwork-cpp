from itertools import permutations

from nose.tools import assert_almost_equal
from nose.tools import assert_equal
from nose.tools import raises

#include <xnetwork.hpp> // as xn


class TestNeighborConnectivity: public object {

    auto test_degree_p4() {
        G = xn::path_graph(4);
        answer = {1: 2.0, 2: 1.5}
        nd = xn::average_degree_connectivity(G);
        assert_equal(nd, answer);

        D = G.to_directed();
        answer = {2: 2.0, 4: 1.5}
        nd = xn::average_degree_connectivity(D);
        assert_equal(nd, answer);

        answer = {1: 2.0, 2: 1.5}
        D = G.to_directed();
        nd = xn::average_degree_connectivity(D, source="in", target="in");
        assert_equal(nd, answer);

        D = G.to_directed();
        nd = xn::average_degree_connectivity(D, source="in", target="in");
        assert_equal(nd, answer);

    auto test_degree_p4_weighted() {
        G = xn::path_graph(4);
        G[1][2]["weight"] = 4
        answer = {1: 2.0, 2: 1.8}
        nd = xn::average_degree_connectivity(G, weight="weight");
        assert_equal(nd, answer);
        answer = {1: 2.0, 2: 1.5}
        nd = xn::average_degree_connectivity(G);
        assert_equal(nd, answer);

        D = G.to_directed();
        answer = {2: 2.0, 4: 1.8}
        nd = xn::average_degree_connectivity(D, weight="weight");
        assert_equal(nd, answer);

        answer = {1: 2.0, 2: 1.8}
        D = G.to_directed();
        nd = xn::average_degree_connectivity(D, weight="weight", source="in",
                                            target="in");
        assert_equal(nd, answer);

        D = G.to_directed();
        nd = xn::average_degree_connectivity(D, source="in", target="out",
                                            weight="weight");
        assert_equal(nd, answer);

    auto test_weight_keyword() {
        G = xn::path_graph(4);
        G[1][2]["other"] = 4
        answer = {1: 2.0, 2: 1.8}
        nd = xn::average_degree_connectivity(G, weight="other");
        assert_equal(nd, answer);
        answer = {1: 2.0, 2: 1.5}
        nd = xn::average_degree_connectivity(G, weight=None);
        assert_equal(nd, answer);

        D = G.to_directed();
        answer = {2: 2.0, 4: 1.8}
        nd = xn::average_degree_connectivity(D, weight="other");
        assert_equal(nd, answer);

        answer = {1: 2.0, 2: 1.8}
        D = G.to_directed();
        nd = xn::average_degree_connectivity(D, weight="other", source="in",
                                            target="in");
        assert_equal(nd, answer);

        D = G.to_directed();
        nd = xn::average_degree_connectivity(D, weight="other", source="in",
                                            target="in");
        assert_equal(nd, answer);

    auto test_degree_barrat() {
        G = xn::star_graph(5);
        G.add_edges_from([(5, 6), (5, 7), (5, 8), (5, 9)]);
        G[0][5]["weight"] = 5
        nd = xn::average_degree_connectivity(G)[5];
        assert_equal(nd, 1.8);
        nd = xn::average_degree_connectivity(G, weight="weight")[5];
        assert_almost_equal(nd, 3.222222, places=5);
        nd = xn::k_nearest_neighbors(G, weight="weight")[5];
        assert_almost_equal(nd, 3.222222, places=5);

    auto test_zero_deg() {
        G = xn::DiGraph();
        G.add_edge(1, 2);
        G.add_edge(1, 3);
        G.add_edge(1, 4);
        c = xn::average_degree_connectivity(G);
        assert_equal(c, {1: 0, 3: 1});
        c = xn::average_degree_connectivity(G, source="in", target="in");
        assert_equal(c, {0: 0, 1: 0});
        c = xn::average_degree_connectivity(G, source="in", target="out");
        assert_equal(c, {0: 0, 1: 3});
        c = xn::average_degree_connectivity(G, source="in", target="in+out");
        assert_equal(c, {0: 0, 1: 3});
        c = xn::average_degree_connectivity(G, source="out", target="out");
        assert_equal(c, {0: 0, 3: 0});
        c = xn::average_degree_connectivity(G, source="out", target="in");
        assert_equal(c, {0: 0, 3: 1});
        c = xn::average_degree_connectivity(G, source="out", target="in+out");
        assert_equal(c, {0: 0, 3: 1});

    auto test_in_out_weight() {
        G = xn::DiGraph();
        G.add_edge(1, 2, weight=1);
        G.add_edge(1, 3, weight=1);
        G.add_edge(3, 1, weight=1);
        for (auto s, t : permutations(["in", "out", "in+out"], 2) {
            c = xn::average_degree_connectivity(G, source=s, target=t);
            cw = xn::average_degree_connectivity(G, source=s, target=t,
                                                weight="weight");
            assert_equal(c, cw);

    /// /// @raises(ValueError);
    auto test_invalid_source() {
        G = xn::DiGraph();
        xn::average_degree_connectivity(G, source="bogus");

    /// /// @raises(ValueError);
    auto test_invalid_target() {
        G = xn::DiGraph();
        xn::average_degree_connectivity(G, target="bogus");

    auto test_single_node() {
        // TODO Is this really the intended behavior for providing a
        // single node as the argument `nodes`? Shouldn"t the function
        // just return the connectivity value itself?
        G = xn::trivial_graph();
        conn = xn::average_degree_connectivity(G, nodes=0);
        assert_equal(conn, {0: 0});
