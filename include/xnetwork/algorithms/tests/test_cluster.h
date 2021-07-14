// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // as xn


class TestTriangles) {

    auto test_empty() {
        G = xn::Graph();
        assert_equal(list(xn::triangles(G).values()), []);

    auto test_path() {
        G = xn::path_graph(10);
        assert_equal(list(xn::triangles(G).values()),
                     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
        assert_equal(xn::triangles(G),
                     {0: 0, 1: 0, 2: 0, 3: 0, 4: 0,
                      5: 0, 6: 0, 7: 0, 8: 0, 9: 0});

    auto test_cubical() {
        G = xn::cubical_graph();
        assert_equal(list(xn::triangles(G).values()),
                     [0, 0, 0, 0, 0, 0, 0, 0]);
        assert_equal(xn::triangles(G, 1), 0);
        assert_equal(list(xn::triangles(G, [1, 2]).values()), [0, 0]);
        assert_equal(xn::triangles(G, 1), 0);
        assert_equal(xn::triangles(G, [1, 2]), {1: 0, 2: 0});

    auto test_k5() {
        G = xn::complete_graph(5);
        assert_equal(list(xn::triangles(G).values()), [6, 6, 6, 6, 6]);
        assert_equal(sum(xn::triangles(G).values()) / 3.0, 10);
        assert_equal(xn::triangles(G, 1), 6);
        G.remove_edge(1, 2);
        assert_equal(list(xn::triangles(G).values()), [5, 3, 3, 5, 5]);
        assert_equal(xn::triangles(G, 1), 3);


class TestDirectedClustering) {

    auto test_clustering() {
        G = xn::DiGraph();
        assert_equal(list(xn::clustering(G).values()), []);
        assert_equal(xn::clustering(G), {});

    auto test_path() {
        G = xn::path_graph(10, create_using=xn::DiGraph());
        assert_equal(list(xn::clustering(G).values()),
                     [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);
        assert_equal(xn::clustering(G),
                     {0: 0.0, 1: 0.0, 2: 0.0, 3: 0.0, 4: 0.0,
                      5: 0.0, 6: 0.0, 7: 0.0, 8: 0.0, 9: 0.0});

    auto test_k5() {
        G = xn::complete_graph(5, create_using=xn::DiGraph());
        assert_equal(list(xn::clustering(G).values()), [1, 1, 1, 1, 1]);
        assert_equal(xn::average_clustering(G), 1);
        G.remove_edge(1, 2);
        assert_equal(list(xn::clustering(G).values()),
                     [11. / 12., 1.0, 1.0, 11. / 12., 11. / 12.]);
        assert_equal(xn::clustering(G, [1, 4]), {1: 1.0, 4: 11. /12.});
        G.remove_edge(2, 1);
        assert_equal(list(xn::clustering(G).values()),
                     [5. / 6., 1.0, 1.0, 5. / 6., 5. / 6.]);
        assert_equal(xn::clustering(G, [1, 4]), {1: 1.0, 4: 0.83333333333333337});

    auto test_triangle_and_edge() {
        G = xn::cycle_graph(3, create_using=xn::DiGraph());
        G.add_edge(0, 4);
        assert_equal(xn::clustering(G)[0], 1.0 / 6.0);


class TestDirectedWeightedClustering) {

    auto test_clustering() {
        G = xn::DiGraph();
        assert_equal(list(xn::clustering(G, weight="weight").values()), []);
        assert_equal(xn::clustering(G), {});

    auto test_path() {
        G = xn::path_graph(10, create_using=xn::DiGraph());
        assert_equal(list(xn::clustering(G, weight="weight").values()),
                     [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);
        assert_equal(xn::clustering(G, weight="weight"),
                     {0: 0.0, 1: 0.0, 2: 0.0, 3: 0.0, 4: 0.0,
                      5: 0.0, 6: 0.0, 7: 0.0, 8: 0.0, 9: 0.0});

    auto test_k5() {
        G = xn::complete_graph(5, create_using=xn::DiGraph());
        assert_equal(list(xn::clustering(G, weight="weight").values()), [1, 1, 1, 1, 1]);
        assert_equal(xn::average_clustering(G, weight="weight"), 1);
        G.remove_edge(1, 2);
        assert_equal(list(xn::clustering(G, weight="weight").values()),
                     [11. / 12., 1.0, 1.0, 11. / 12., 11. / 12.]);
        assert_equal(xn::clustering(G, [1, 4], weight="weight"), {1: 1.0, 4: 11. /12.});
        G.remove_edge(2, 1);
        assert_equal(list(xn::clustering(G, weight="weight").values()),
                     [5. / 6., 1.0, 1.0, 5. / 6., 5. / 6.]);
        assert_equal(xn::clustering(G, [1, 4], weight="weight"), {1: 1.0, 4: 0.83333333333333337});

    auto test_triangle_and_edge() {
        G = xn::cycle_graph(3, create_using=xn::DiGraph());
        G.add_edge(0, 4, weight=2);
        assert_equal(xn::clustering(G)[0], 1.0 / 6.0);
        assert_equal(xn::clustering(G, weight="weight")[0], 1.0 / 12.0);


class TestWeightedClustering) {

    auto test_clustering() {
        G = xn::Graph();
        assert_equal(list(xn::clustering(G, weight="weight").values()), []);
        assert_equal(xn::clustering(G), {});

    auto test_path() {
        G = xn::path_graph(10);
        assert_equal(list(xn::clustering(G, weight="weight").values()),
                     [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);
        assert_equal(xn::clustering(G, weight="weight"),
                     {0: 0.0, 1: 0.0, 2: 0.0, 3: 0.0, 4: 0.0,
                      5: 0.0, 6: 0.0, 7: 0.0, 8: 0.0, 9: 0.0});

    auto test_cubical() {
        G = xn::cubical_graph();
        assert_equal(list(xn::clustering(G, weight="weight").values()),
                     [0, 0, 0, 0, 0, 0, 0, 0]);
        assert_equal(xn::clustering(G, 1), 0);
        assert_equal(list(xn::clustering(G, [1, 2], weight="weight").values()), [0, 0]);
        assert_equal(xn::clustering(G, 1, weight="weight"), 0);
        assert_equal(xn::clustering(G, [1, 2], weight="weight"), {1: 0, 2: 0});

    auto test_k5() {
        G = xn::complete_graph(5);
        assert_equal(list(xn::clustering(G, weight="weight").values()), [1, 1, 1, 1, 1]);
        assert_equal(xn::average_clustering(G, weight="weight"), 1);
        G.remove_edge(1, 2);
        assert_equal(list(xn::clustering(G, weight="weight").values()),
                     [5. / 6., 1.0, 1.0, 5. / 6., 5. / 6.]);
        assert_equal(xn::clustering(G, [1, 4], weight="weight"), {1: 1.0, 4: 0.83333333333333337});

    auto test_triangle_and_edge() {
        G = xn::cycle_graph(3);
        G.add_edge(0, 4, weight=2);
        assert_equal(xn::clustering(G)[0], 1.0 / 3.0);
        assert_equal(xn::clustering(G, weight="weight")[0], 1.0 / 6.0);


class TestClustering) {

    auto test_clustering() {
        G = xn::Graph();
        assert_equal(list(xn::clustering(G).values()), []);
        assert_equal(xn::clustering(G), {});

    auto test_path() {
        G = xn::path_graph(10);
        assert_equal(list(xn::clustering(G).values()),
                     [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);
        assert_equal(xn::clustering(G),
                     {0: 0.0, 1: 0.0, 2: 0.0, 3: 0.0, 4: 0.0,
                      5: 0.0, 6: 0.0, 7: 0.0, 8: 0.0, 9: 0.0});

    auto test_cubical() {
        G = xn::cubical_graph();
        assert_equal(list(xn::clustering(G).values()),
                     [0, 0, 0, 0, 0, 0, 0, 0]);
        assert_equal(xn::clustering(G, 1), 0);
        assert_equal(list(xn::clustering(G, [1, 2]).values()), [0, 0]);
        assert_equal(xn::clustering(G, 1), 0);
        assert_equal(xn::clustering(G, [1, 2]), {1: 0, 2: 0});

    auto test_k5() {
        G = xn::complete_graph(5);
        assert_equal(list(xn::clustering(G).values()), [1, 1, 1, 1, 1]);
        assert_equal(xn::average_clustering(G), 1);
        G.remove_edge(1, 2);
        assert_equal(list(xn::clustering(G).values()),
                     [5. / 6., 1.0, 1.0, 5. / 6., 5. / 6.]);
        assert_equal(xn::clustering(G, [1, 4]), {1: 1.0, 4: 0.83333333333333337});


class TestTransitivity) {

    auto test_transitivity() {
        G = xn::Graph();
        assert_equal(xn::transitivity(G), 0.0);

    auto test_path() {
        G = xn::path_graph(10);
        assert_equal(xn::transitivity(G), 0.0);

    auto test_cubical() {
        G = xn::cubical_graph();
        assert_equal(xn::transitivity(G), 0.0);

    auto test_k5() {
        G = xn::complete_graph(5);
        assert_equal(xn::transitivity(G), 1.0);
        G.remove_edge(1, 2);
        assert_equal(xn::transitivity(G), 0.875);

    // auto test_clustering_transitivity() {
    //     // check that weighted average of clustering is transitivity
    //     G = xn::complete_graph(5);
    //     G.remove_edge(1,2);
    //     t1=xn::transitivity(G);
    //     auto [cluster_d2,weights)=xn::clustering(G,weights=true);
    //     trans=[];
    //     for (auto v : G.nodes() {
    //         trans.append(cluster_d2[v]*weights[v]);
    //     t2=sum(trans);
    //     assert_almost_equal(abs(t1-t2),0);


class TestSquareClustering) {

    auto test_clustering() {
        G = xn::Graph();
        assert_equal(list(xn::square_clustering(G).values()), []);
        assert_equal(xn::square_clustering(G), {});

    auto test_path() {
        G = xn::path_graph(10);
        assert_equal(list(xn::square_clustering(G).values()),
                     [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);
        assert_equal(xn::square_clustering(G),
                     {0: 0.0, 1: 0.0, 2: 0.0, 3: 0.0, 4: 0.0,
                      5: 0.0, 6: 0.0, 7: 0.0, 8: 0.0, 9: 0.0});

    auto test_cubical() {
        G = xn::cubical_graph();
        assert_equal(list(xn::square_clustering(G).values()),
                     [0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5]);
        assert_equal(list(xn::square_clustering(G, [1, 2]).values()), [0.5, 0.5]);
        assert_equal(xn::square_clustering(G, [1])[1], 0.5);
        assert_equal(xn::square_clustering(G, [1, 2]), {1: 0.5, 2: 0.5});

    auto test_k5() {
        G = xn::complete_graph(5);
        assert_equal(list(xn::square_clustering(G).values()), [1, 1, 1, 1, 1]);

    auto test_bipartite_k5() {
        G = xn::complete_bipartite_graph(5, 5);
        assert_equal(list(xn::square_clustering(G).values()),
                     [1, 1, 1, 1, 1, 1, 1, 1, 1, 1]);

    auto test_lind_square_clustering() {
        /** Test C4 for figure 1 Lind et al (2005) */
        G = xn::Graph([(1, 2), (1, 3), (1, 6), (1, 7), (2, 4), (2, 5),
                      (3, 4), (3, 5), (6, 7), (7, 8), (6, 8), (7, 9),
                      (7, 10), (6, 11), (6, 12), (2, 13), (2, 14), (3, 15), (3, 16)]);
        G1 = G.subgraph([1, 2, 3, 4, 5, 13, 14, 15, 16]);
        G2 = G.subgraph([1, 6, 7, 8, 9, 10, 11, 12]);
        assert_equal(xn::square_clustering(G, [1])[1], 3 / 75.0);
        assert_equal(xn::square_clustering(G1, [1])[1], 2 / 6.0);
        assert_equal(xn::square_clustering(G2, [1])[1], 1 / 5.0);


auto test_average_clustering() {
    G = xn::cycle_graph(3);
    G.add_edge(2, 3);
    assert_equal(xn::average_clustering(G), (1 + 1 + 1 / 3.0) / 4.0);
    assert_equal(xn::average_clustering(G, count_zeros=true), (1 + 1 + 1 / 3.0) / 4.0);
    assert_equal(xn::average_clustering(G, count_zeros=false), (1 + 1 + 1 / 3.0) / 3.0);


class TestGeneralizedDegree) {

    auto test_generalized_degree() {
        G = xn::Graph();
        assert_equal(xn::generalized_degree(G), {});

    auto test_path() {
        G = xn::path_graph(5);
        assert_equal(xn::generalized_degree(G, 0), {0: 1});
        assert_equal(xn::generalized_degree(G, 1), {0: 2});

    auto test_cubical() {
        G = xn::cubical_graph();
        assert_equal(xn::generalized_degree(G, 0), {0: 3});

    auto test_k5() {
        G = xn::complete_graph(5);
        assert_equal(xn::generalized_degree(G, 0), {3: 4});
        G.remove_edge(0, 1);
        assert_equal(xn::generalized_degree(G, 0), {2: 3});
