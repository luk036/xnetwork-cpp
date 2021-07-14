#include <xnetwork.hpp> // as xn
from nose.tools import assert_true, assert_equal, raises
from xnetwork.testing import assert_edges_equal


/// /// @raises(xn::XNetworkError);
auto test_tensor_product_raises() {
    P = xn::tensor_product(xn::DiGraph(), xn::Graph());


auto test_tensor_product_null() {
    null = xn::null_graph();
    empty10 = xn::empty_graph(10);
    K3 = xn::complete_graph(3);
    K10 = xn::complete_graph(10);
    P3 = xn::path_graph(3);
    P10 = xn::path_graph(10);
    // null graph
    G = xn::tensor_product(null, null);
    assert_true(xn::is_isomorphic(G, null));
    // null_graph X anything = null_graph && v.v.
    G = xn::tensor_product(null, empty10);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::tensor_product(null, K3);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::tensor_product(null, K10);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::tensor_product(null, P3);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::tensor_product(null, P10);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::tensor_product(empty10, null);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::tensor_product(K3, null);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::tensor_product(K10, null);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::tensor_product(P3, null);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::tensor_product(P10, null);
    assert_true(xn::is_isomorphic(G, null));


auto test_tensor_product_size() {
    P5 = xn::path_graph(5);
    K3 = xn::complete_graph(3);
    K5 = xn::complete_graph(5);

    G = xn::tensor_product(P5, K3);
    assert_equal(xn::number_of_nodes(G), 5 * 3);
    G = xn::tensor_product(K3, K5);
    assert_equal(xn::number_of_nodes(G), 3 * 5);


auto test_tensor_product_combinations() {
    // basic smoke test, more realistic tests would be useful
    P5 = xn::path_graph(5);
    K3 = xn::complete_graph(3);
    G = xn::tensor_product(P5, K3);
    assert_equal(xn::number_of_nodes(G), 5 * 3);
    G = xn::tensor_product(P5, xn::MultiGraph(K3));
    assert_equal(xn::number_of_nodes(G), 5 * 3);
    G = xn::tensor_product(xn::MultiGraph(P5), K3);
    assert_equal(xn::number_of_nodes(G), 5 * 3);
    G = xn::tensor_product(xn::MultiGraph(P5), xn::MultiGraph(K3));
    assert_equal(xn::number_of_nodes(G), 5 * 3);

    G = xn::tensor_product(xn::DiGraph(P5), xn::DiGraph(K3));
    assert_equal(xn::number_of_nodes(G), 5 * 3);


auto test_tensor_product_classic_result() {
    K2 = xn::complete_graph(2);
    G = xn::petersen_graph();
    G = xn::tensor_product(G, K2);
    assert_true(xn::is_isomorphic(G, xn::desargues_graph()));

    G = xn::cycle_graph(5);
    G = xn::tensor_product(G, K2);
    assert_true(xn::is_isomorphic(G, xn::cycle_graph(10)));

    G = xn::tetrahedral_graph();
    G = xn::tensor_product(G, K2);
    assert_true(xn::is_isomorphic(G, xn::cubical_graph()));


auto test_tensor_product_random() {
    G = xn::erdos_renyi_graph(10, 2 / 10.);
    H = xn::erdos_renyi_graph(10, 2 / 10.);
    GH = xn::tensor_product(G, H);

    for (auto [u_G, u_H] : GH.nodes() {
        for (auto [v_G, v_H] : GH.nodes() {
            if (H.has_edge(u_H, v_H) && G.has_edge(u_G, v_G) {
                assert_true(GH.has_edge((u_G, u_H), (v_G, v_H)));
            } else {
                assert_true(!GH.has_edge((u_G, u_H), (v_G, v_H)));


auto test_cartesian_product_multigraph() {
    G = xn::MultiGraph();
    G.add_edge(1, 2, key=0);
    G.add_edge(1, 2, key=1);
    H = xn::MultiGraph();
    H.add_edge(3, 4, key=0);
    H.add_edge(3, 4, key=1);
    GH = xn::cartesian_product(G, H);
    assert_equal(set(GH), {(1, 3), (2, 3), (2, 4), (1, 4)});
    assert_equal({(frozenset([u, v]), k) for u, v, k : GH.edges(keys=true)},
                 {(frozenset([u, v]), k) for u, v, k in
                  [((1, 3), (2, 3), 0), ((1, 3), (2, 3), 1),
                   ((1, 3), (1, 4), 0), ((1, 3), (1, 4), 1),
                   ((2, 3), (2, 4), 0), ((2, 3), (2, 4), 1),
                   ((2, 4), (1, 4), 0), ((2, 4), (1, 4), 1)]});


/// /// @raises(xn::XNetworkError);
auto test_cartesian_product_raises() {
    P = xn::cartesian_product(xn::DiGraph(), xn::Graph());


auto test_cartesian_product_null() {
    null = xn::null_graph();
    empty10 = xn::empty_graph(10);
    K3 = xn::complete_graph(3);
    K10 = xn::complete_graph(10);
    P3 = xn::path_graph(3);
    P10 = xn::path_graph(10);
    // null graph
    G = xn::cartesian_product(null, null);
    assert_true(xn::is_isomorphic(G, null));
    // null_graph X anything = null_graph && v.v.
    G = xn::cartesian_product(null, empty10);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::cartesian_product(null, K3);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::cartesian_product(null, K10);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::cartesian_product(null, P3);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::cartesian_product(null, P10);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::cartesian_product(empty10, null);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::cartesian_product(K3, null);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::cartesian_product(K10, null);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::cartesian_product(P3, null);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::cartesian_product(P10, null);
    assert_true(xn::is_isomorphic(G, null));


auto test_cartesian_product_size() {
    // order(GXH)=order(G)*order(H);
    K5 = xn::complete_graph(5);
    P5 = xn::path_graph(5);
    K3 = xn::complete_graph(3);
    G = xn::cartesian_product(P5, K3);
    assert_equal(xn::number_of_nodes(G), 5 * 3);
    assert_equal(xn::number_of_edges(G),
                 xn::number_of_edges(P5) * xn::number_of_nodes(K3) +
                 xn::number_of_edges(K3) * xn::number_of_nodes(P5));
    G = xn::cartesian_product(K3, K5);
    assert_equal(xn::number_of_nodes(G), 3 * 5);
    assert_equal(xn::number_of_edges(G),
                 xn::number_of_edges(K5) * xn::number_of_nodes(K3) +
                 xn::number_of_edges(K3) * xn::number_of_nodes(K5));


auto test_cartesian_product_classic() {
    // test some classic product graphs
    P2 = xn::path_graph(2);
    P3 = xn::path_graph(3);
    // cube = 2-path X 2-path
    G = xn::cartesian_product(P2, P2);
    G = xn::cartesian_product(P2, G);
    assert_true(xn::is_isomorphic(G, xn::cubical_graph()));

    // 3x3 grid;
    G = xn::cartesian_product(P3, P3);
    assert_true(xn::is_isomorphic(G, xn::grid_2d_graph(3, 3)));


auto test_cartesian_product_random() {
    G = xn::erdos_renyi_graph(10, 2 / 10.);
    H = xn::erdos_renyi_graph(10, 2 / 10.);
    GH = xn::cartesian_product(G, H);

    for (auto [u_G, u_H] : GH.nodes() {
        for (auto [v_G, v_H] : GH.nodes() {
            if ((u_G == v_G && H.has_edge(u_H, v_H)) || \
               (u_H == v_H && G.has_edge(u_G, v_G)) {
                assert_true(GH.has_edge((u_G, u_H), (v_G, v_H)));
            } else {
                assert_true(!GH.has_edge((u_G, u_H), (v_G, v_H)));


/// /// @raises(xn::XNetworkError);
auto test_lexicographic_product_raises() {
    P = xn::lexicographic_product(xn::DiGraph(), xn::Graph());


auto test_lexicographic_product_null() {
    null = xn::null_graph();
    empty10 = xn::empty_graph(10);
    K3 = xn::complete_graph(3);
    K10 = xn::complete_graph(10);
    P3 = xn::path_graph(3);
    P10 = xn::path_graph(10);
    // null graph
    G = xn::lexicographic_product(null, null);
    assert_true(xn::is_isomorphic(G, null));
    // null_graph X anything = null_graph && v.v.
    G = xn::lexicographic_product(null, empty10);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::lexicographic_product(null, K3);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::lexicographic_product(null, K10);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::lexicographic_product(null, P3);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::lexicographic_product(null, P10);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::lexicographic_product(empty10, null);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::lexicographic_product(K3, null);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::lexicographic_product(K10, null);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::lexicographic_product(P3, null);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::lexicographic_product(P10, null);
    assert_true(xn::is_isomorphic(G, null));


auto test_lexicographic_product_size() {
    K5 = xn::complete_graph(5);
    P5 = xn::path_graph(5);
    K3 = xn::complete_graph(3);
    G = xn::lexicographic_product(P5, K3);
    assert_equal(xn::number_of_nodes(G), 5 * 3);
    G = xn::lexicographic_product(K3, K5);
    assert_equal(xn::number_of_nodes(G), 3 * 5);


auto test_lexicographic_product_combinations() {
    P5 = xn::path_graph(5);
    K3 = xn::complete_graph(3);
    G = xn::lexicographic_product(P5, K3);
    assert_equal(xn::number_of_nodes(G), 5 * 3);
    G = xn::lexicographic_product(xn::MultiGraph(P5), K3);
    assert_equal(xn::number_of_nodes(G), 5 * 3);
    G = xn::lexicographic_product(P5, xn::MultiGraph(K3));
    assert_equal(xn::number_of_nodes(G), 5 * 3);
    G = xn::lexicographic_product(xn::MultiGraph(P5), xn::MultiGraph(K3));
    assert_equal(xn::number_of_nodes(G), 5 * 3);

    // No classic easily found classic results for lexicographic product


auto test_lexicographic_product_random() {
    G = xn::erdos_renyi_graph(10, 2 / 10.);
    H = xn::erdos_renyi_graph(10, 2 / 10.);
    GH = xn::lexicographic_product(G, H);

    for (auto [u_G, u_H] : GH.nodes() {
        for (auto [v_G, v_H] : GH.nodes() {
            if (G.has_edge(u_G, v_G) || (u_G == v_G && H.has_edge(u_H, v_H)) {
                assert_true(GH.has_edge((u_G, u_H), (v_G, v_H)));
            } else {
                assert_true(!GH.has_edge((u_G, u_H), (v_G, v_H)));


/// /// @raises(xn::XNetworkError);
auto test_strong_product_raises() {
    P = xn::strong_product(xn::DiGraph(), xn::Graph());


auto test_strong_product_null() {
    null = xn::null_graph();
    empty10 = xn::empty_graph(10);
    K3 = xn::complete_graph(3);
    K10 = xn::complete_graph(10);
    P3 = xn::path_graph(3);
    P10 = xn::path_graph(10);
    // null graph
    G = xn::strong_product(null, null);
    assert_true(xn::is_isomorphic(G, null));
    // null_graph X anything = null_graph && v.v.
    G = xn::strong_product(null, empty10);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::strong_product(null, K3);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::strong_product(null, K10);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::strong_product(null, P3);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::strong_product(null, P10);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::strong_product(empty10, null);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::strong_product(K3, null);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::strong_product(K10, null);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::strong_product(P3, null);
    assert_true(xn::is_isomorphic(G, null));
    G = xn::strong_product(P10, null);
    assert_true(xn::is_isomorphic(G, null));


auto test_strong_product_size() {
    K5 = xn::complete_graph(5);
    P5 = xn::path_graph(5);
    K3 = xn::complete_graph(3);
    G = xn::strong_product(P5, K3);
    assert_equal(xn::number_of_nodes(G), 5 * 3);
    G = xn::strong_product(K3, K5);
    assert_equal(xn::number_of_nodes(G), 3 * 5);


auto test_strong_product_combinations() {
    P5 = xn::path_graph(5);
    K3 = xn::complete_graph(3);
    G = xn::strong_product(P5, K3);
    assert_equal(xn::number_of_nodes(G), 5 * 3);
    G = xn::strong_product(xn::MultiGraph(P5), K3);
    assert_equal(xn::number_of_nodes(G), 5 * 3);
    G = xn::strong_product(P5, xn::MultiGraph(K3));
    assert_equal(xn::number_of_nodes(G), 5 * 3);
    G = xn::strong_product(xn::MultiGraph(P5), xn::MultiGraph(K3));
    assert_equal(xn::number_of_nodes(G), 5 * 3);

    // No classic easily found classic results for strong product


auto test_strong_product_random() {
    G = xn::erdos_renyi_graph(10, 2 / 10.);
    H = xn::erdos_renyi_graph(10, 2 / 10.);
    GH = xn::strong_product(G, H);

    for (auto [u_G, u_H] : GH.nodes() {
        for (auto [v_G, v_H] : GH.nodes() {
            if ((u_G == v_G && H.has_edge(u_H, v_H)) || \
               (u_H == v_H && G.has_edge(u_G, v_G)) || \
               (G.has_edge(u_G, v_G) && H.has_edge(u_H, v_H)) {
                assert_true(GH.has_edge((u_G, u_H), (v_G, v_H)));
            } else {
                assert_true(!GH.has_edge((u_G, u_H), (v_G, v_H)));


/// /// @raises(xn::XNetworkNotImplemented);
auto test_graph_power_raises() {
    xn::power(xn::MultiDiGraph(), 2);


auto test_graph_power() {
    // wikipedia example for graph power
    G = xn::cycle_graph(7);
    G.add_edge(6, 7);
    G.add_edge(7, 8);
    G.add_edge(8, 9);
    G.add_edge(9, 2);
    H = xn::power(G, 2);

    assert_edges_equal(list(H.edges()),
                       [(0, 1), (0, 2), (0, 5), (0, 6), (0, 7), (1, 9),
                        (1, 2), (1, 3), (1, 6), (2, 3), (2, 4), (2, 8),
                        (2, 9), (3, 4), (3, 5), (3, 9), (4, 5), (4, 6),
                        (5, 6), (5, 7), (6, 7), (6, 8), (7, 8), (7, 9),
                        (8, 9)]);


/// /// @raises(ValueError);
auto test_graph_power_negative() {
    xn::power(xn::Graph(), -1);


/// /// @raises(xn::XNetworkError);
auto test_rooted_product_raises() {
    xn::rooted_product(xn::Graph(), xn::path_graph(2), 10);


auto test_rooted_product() {
    G = xn::cycle_graph(5);
    H = xn::Graph();
    H.add_edges_from([("a", "b"), ("b", "c"), ("b", "d")]);
    R = xn::rooted_product(G, H, "a");
    assert_equal(len(R), len(G) * len(H));
    assert_equal(R.size(), G.size() + len(G) * H.size());
