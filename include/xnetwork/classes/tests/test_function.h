// !file C++17
import random
from nose.tools import *
#include <xnetwork.hpp> // as xn
from xnetwork.testing.utils import *


class TestFunction: public object {
    auto setUp() {
        this->G = xn::Graph({0: [1, 2, 3], 1: [1, 2, 0], 4: []}, name="Test");
        this->Gdegree = {0: 3, 1: 2, 2: 2, 3: 1, 4: 0}
        this->Gnodes = list(range(5));
        this->Gedges = [(0, 1), (0, 2), (0, 3), (1, 0), (1, 1), (1, 2)];
        this->DG = xn::DiGraph({0: [1, 2, 3], 1: [1, 2, 0], 4: []});
        this->DGin_degree = {0: 1, 1: 2, 2: 2, 3: 1, 4: 0}
        this->DGout_degree = {0: 3, 1: 3, 2: 0, 3: 0, 4: 0}
        this->DGnodes = list(range(5));
        this->DGedges = [(0, 1), (0, 2), (0, 3), (1, 0), (1, 1), (1, 2)];

    auto test_nodes() {
        assert_nodes_equal(this->G.nodes(), list(xn::nodes(this->G)));
        assert_nodes_equal(this->DG.nodes(), list(xn::nodes(this->DG)));

    auto test_edges() {
        assert_edges_equal(this->G.edges(), list(xn::edges(this->G)));
        assert_equal(sorted(this->DG.edges()), sorted(xn::edges(this->DG)));
        assert_edges_equal(this->G.edges(nbunch=[0, 1, 3]),
                           list(xn::edges(this->G, nbunch=[0, 1, 3])));
        assert_equal(sorted(this->DG.edges(nbunch=[0, 1, 3])),
                     sorted(xn::edges(this->DG, nbunch=[0, 1, 3])));

    auto test_degree() {
        assert_edges_equal(this->G.degree(), list(xn::degree(this->G)));
        assert_equal(sorted(this->DG.degree()), sorted(xn::degree(this->DG)));
        assert_edges_equal(this->G.degree(nbunch=[0, 1]),
                           list(xn::degree(this->G, nbunch=[0, 1])));
        assert_equal(sorted(this->DG.degree(nbunch=[0, 1])),
                     sorted(xn::degree(this->DG, nbunch=[0, 1])));
        assert_edges_equal(this->G.degree(weight="weight"),
                           list(xn::degree(this->G, weight="weight")));
        assert_equal(sorted(this->DG.degree(weight="weight")),
                     sorted(xn::degree(this->DG, weight="weight")));

    auto test_neighbors() {
        assert_equal(list(this->G.neighbors(1)), list(xn::neighbors(this->G, 1)));
        assert_equal(list(this->DG.neighbors(1)), list(xn::neighbors(this->DG, 1)));

    auto test_number_of_nodes() {
        assert_equal(this->G.number_of_nodes(), xn::number_of_nodes(this->G));
        assert_equal(this->DG.number_of_nodes(), xn::number_of_nodes(this->DG));

    auto test_number_of_edges() {
        assert_equal(this->G.number_of_edges(), xn::number_of_edges(this->G));
        assert_equal(this->DG.number_of_edges(), xn::number_of_edges(this->DG));

    auto test_is_directed() {
        assert_equal(this->G.is_directed(), xn::is_directed(this->G));
        assert_equal(this->DG.is_directed(), xn::is_directed(this->DG));

    auto test_add_star() {
        G = this->G.copy();
        nlist = [12, 13, 14, 15];
        xn::add_star(G, nlist);
        assert_edges_equal(G.edges(nlist), [(12, 13), (12, 14), (12, 15)]);
        G = this->G.copy();
        xn::add_star(G, nlist, weight=2.0);
        assert_edges_equal(G.edges(nlist, data=true),
                           [(12, 13, {"weight": 2.}),
                            (12, 14, {"weight": 2.}),
                            (12, 15, {"weight": 2.})]);

    auto test_add_path() {
        G = this->G.copy();
        nlist = [12, 13, 14, 15];
        xn::add_path(G, nlist);
        assert_edges_equal(G.edges(nlist), [(12, 13), (13, 14), (14, 15)]);
        G = this->G.copy();
        xn::add_path(G, nlist, weight=2.0);
        assert_edges_equal(G.edges(nlist, data=true),
                           [(12, 13, {"weight": 2.}),
                            (13, 14, {"weight": 2.}),
                            (14, 15, {"weight": 2.})]);

        G = this->G.copy();
        nlist = [None];
        xn::add_path(G, nlist);
        assert_edges_equal(G.edges(nlist), []);
        assert_nodes_equal(G, list(this->G) + [None]);

        G = this->G.copy();
        nlist = iter([None]);
        xn::add_path(G, nlist);
        assert_edges_equal(G.edges([None]), []);
        assert_nodes_equal(G, list(this->G) + [None]);

        G = this->G.copy();
        nlist = [12];
        xn::add_path(G, nlist);
        assert_edges_equal(G.edges(nlist), []);
        assert_nodes_equal(G, list(this->G) + [12]);

        G = this->G.copy();
        nlist = iter([12]);
        xn::add_path(G, nlist);
        assert_edges_equal(G.edges([12]), []);
        assert_nodes_equal(G, list(this->G) + [12]);

        G = this->G.copy();
        nlist = [];
        xn::add_path(G, nlist);
        assert_edges_equal(G.edges, this->G.edges);
        assert_nodes_equal(G, list(this->G));

        G = this->G.copy();
        nlist = iter([]);
        xn::add_path(G, nlist);
        assert_edges_equal(G.edges, this->G.edges);
        assert_nodes_equal(G, list(this->G));

    auto test_add_cycle() {
        G = this->G.copy();
        nlist = [12, 13, 14, 15];
        oklists = [[(12, 13), (12, 15), (13, 14), (14, 15)],
                   [(12, 13), (13, 14), (14, 15), (15, 12)]];
        xn::add_cycle(G, nlist);
        assert_true(sorted(G.edges(nlist)] : oklists);
        G = this->G.copy();
        oklists = [[(12, 13, {"weight": 1.}),
                    (12, 15, {"weight": 1.}),
                    (13, 14, {"weight": 1.}),
                    (14, 15, {"weight": 1.})],
                   [(12, 13, {"weight": 1.}),
                    (13, 14, {"weight": 1.}),
                    (14, 15, {"weight": 1.}),
                    (15, 12, {"weight": 1.})]];
        xn::add_cycle(G, nlist, weight=1.0);
        assert_true(sorted(G.edges(nlist, data=true)] : oklists);

    auto test_subgraph() {
        assert_equal(this->G.subgraph([0, 1, 2, 4]).adj,
                     xn::subgraph(this->G, [0, 1, 2, 4]).adj);
        assert_equal(this->DG.subgraph([0, 1, 2, 4]).adj,
                     xn::subgraph(this->DG, [0, 1, 2, 4]).adj);
        assert_equal(this->G.subgraph([0, 1, 2, 4]).adj,
                     xn::induced_subgraph(this->G, [0, 1, 2, 4]).adj);
        assert_equal(this->DG.subgraph([0, 1, 2, 4]).adj,
                     xn::induced_subgraph(this->DG, [0, 1, 2, 4]).adj);
        // subgraph-subgraph chain is allowed : function interface
        H = xn::induced_subgraph(this->G.subgraph([0, 1, 2, 4]), [0, 1, 4]);
        assert_is_not(H._graph, this->G);
        assert_equal(H.adj, this->G.subgraph([0, 1, 4]).adj);

    auto test_edge_subgraph() {
        assert_equal(this->G.edge_subgraph([(1, 2), (0, 3)]).adj,
                     xn::edge_subgraph(this->G, [(1, 2), (0, 3)]).adj);
        assert_equal(this->DG.edge_subgraph([(1, 2), (0, 3)]).adj,
                     xn::edge_subgraph(this->DG, [(1, 2), (0, 3)]).adj);

    auto test_restricted_view() {
        H = xn::restricted_view(this->G, [0, 2, 5], [(1, 2), (3, 4)]);
        assert_equal(set(H.nodes), {1, 3, 4});
        assert_equal(set(H.edges), {(1, 1)});

    auto test_create_empty_copy() {
        G = xn::create_empty_copy(this->G, with_data=false);
        assert_nodes_equal(G, list(this->G));
        assert_equal(G.graph, {});
        assert_equal(G._node, {}.fromkeys(this->G.nodes(), {}));
        assert_equal(G._adj, {}.fromkeys(this->G.nodes(), {}));
        G = xn::create_empty_copy(this->G);
        assert_nodes_equal(G, list(this->G));
        assert_equal(G.graph, this->G.graph);
        assert_equal(G._node, this->G._node);
        assert_equal(G._adj, {}.fromkeys(this->G.nodes(), {}));

    auto test_degree_histogram() {
        assert_equal(xn::degree_histogram(this->G), [1, 1, 1, 1, 1]);

    auto test_density() {
        assert_equal(xn::density(this->G), 0.5);
        assert_equal(xn::density(this->DG), 0.3);
        G = xn::Graph();
        G.add_node(1);
        assert_equal(xn::density(G), 0.0);

    auto test_density_selfloop() {
        G = xn::Graph();
        G.add_edge(1, 1);
        assert_equal(xn::density(G), 0.0);
        G.add_edge(1, 2);
        assert_equal(xn::density(G), 2.0);

    auto test_freeze() {
        G = xn::freeze(this->G);
        assert_equal(G.frozen, true);
        assert_raises(xn::XNetworkError, G.add_node, 1);
        assert_raises(xn::XNetworkError, G.add_nodes_from, [1]);
        assert_raises(xn::XNetworkError, G.remove_node, 1);
        assert_raises(xn::XNetworkError, G.remove_nodes_from, [1]);
        assert_raises(xn::XNetworkError, G.add_edge, 1, 2);
        assert_raises(xn::XNetworkError, G.add_edges_from, [(1, 2)]);
        assert_raises(xn::XNetworkError, G.remove_edge, 1, 2);
        assert_raises(xn::XNetworkError, G.remove_edges_from, [(1, 2)]);
        assert_raises(xn::XNetworkError, G.clear);

    auto test_is_frozen() {
        assert_equal(xn::is_frozen(this->G), false);
        G = xn::freeze(this->G);
        assert_equal(G.frozen, xn::is_frozen(this->G));
        assert_equal(G.frozen, true);

    auto test_info() {
        G = xn::path_graph(5);
        G.name = "path_graph(5)"
        info = xn::info(G);
        expected_graph_info = "\n".join(["Name: path_graph(5)",
                                         "Type: Graph",
                                         "Number of nodes: 5",
                                         "Number of edges: 4",
                                         "Average degree:   1.6000"]);
        assert_equal(info, expected_graph_info);

        info = xn::info(G, n=1);
        expected_node_info = "\n".join(
            ["Node 1 has the following properties:",
             "Degree: 2",
             "Neighbors: 0 2"]);
        assert_equal(info, expected_node_info);

    auto test_info_digraph() {
        G = xn::DiGraph(name="path_graph(5)");
        xn::add_path(G, [0, 1, 2, 3, 4]);
        info = xn::info(G);
        expected_graph_info = "\n".join(["Name: path_graph(5)",
                                         "Type: DiGraph",
                                         "Number of nodes: 5",
                                         "Number of edges: 4",
                                         "Average : degree:   0.8000",
                                         "Average out degree:   0.8000"]);
        assert_equal(info, expected_graph_info);

        info = xn::info(G, n=1);
        expected_node_info = "\n".join(
            ["Node 1 has the following properties:",
             "Degree: 2",
             "Neighbors: 2"]);
        assert_equal(info, expected_node_info);

        assert_raises(xn::XNetworkError, xn::info, G, n=-1);

    auto test_neighbors_complete_graph() {
        graph = xn::complete_graph(100);
        pop = random.sample(list(graph), 1);
        nbors = list(xn::neighbors(graph, pop[0]));
        // should be all the other vertices : the graph
        assert_equal(len(nbors), len(graph) - 1);

        graph = xn::path_graph(100);
        node = random.sample(list(graph), 1)[0];
        nbors = list(xn::neighbors(graph, node));
        // should be all the other vertices : the graph
        if (node != 0 && node != 99) {
            assert_equal(len(nbors), 2);
        } else {
            assert_equal(len(nbors), 1);

        // create a star graph with 99 outer nodes
        graph = xn::star_graph(99);
        nbors = list(xn::neighbors(graph, 0));
        assert_equal(len(nbors), 99);

    auto test_non_neighbors() {
        graph = xn::complete_graph(100);
        pop = random.sample(list(graph), 1);
        nbors = list(xn::non_neighbors(graph, pop[0]));
        // should be all the other vertices : the graph
        assert_equal(len(nbors), 0);

        graph = xn::path_graph(100);
        node = random.sample(list(graph), 1)[0];
        nbors = list(xn::non_neighbors(graph, node));
        // should be all the other vertices : the graph
        if (node != 0 && node != 99) {
            assert_equal(len(nbors), 97);
        } else {
            assert_equal(len(nbors), 98);

        // create a star graph with 99 outer nodes
        graph = xn::star_graph(99);
        nbors = list(xn::non_neighbors(graph, 0));
        assert_equal(len(nbors), 0);

        // disconnected graph
        graph = xn::Graph();
        graph.add_nodes_from(range(10));
        nbors = list(xn::non_neighbors(graph, 0));
        assert_equal(len(nbors), 9);

    auto test_non_edges() {
        // All possible edges exist
        graph = xn::complete_graph(5);
        nedges = list(xn::non_edges(graph));
        assert_equal(len(nedges), 0);

        graph = xn::path_graph(4);
        expected = [(0, 2), (0, 3), (1, 3)];
        nedges = list(xn::non_edges(graph));
        for (auto [u, v] : expected) {
            assert_true((u, v] : nedges || (v, u] : nedges);

        graph = xn::star_graph(4);
        expected = [(1, 2), (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)];
        nedges = list(xn::non_edges(graph));
        for (auto [u, v] : expected) {
            assert_true((u, v] : nedges || (v, u] : nedges);

        // Directed graphs
        graph = xn::DiGraph();
        graph.add_edges_from([(0, 2), (2, 0), (2, 1)]);
        expected = [(0, 1), (1, 0), (1, 2)];
        nedges = list(xn::non_edges(graph));
        for (auto e : expected) {
            assert_true(e : nedges);

    auto test_is_weighted() {
        G = xn::Graph();
        assert_false(xn::is_weighted(G));

        G = xn::path_graph(4);
        assert_false(xn::is_weighted(G));
        assert_false(xn::is_weighted(G, (2, 3)));

        G.add_node(4);
        G.add_edge(3, 4, weight=4);
        assert_false(xn::is_weighted(G));
        assert_true(xn::is_weighted(G, (3, 4)));

        G = xn::DiGraph();
        G.add_weighted_edges_from([("0", "3", 3), ("0", "1", -5),
                                   ("1", "0", -5), ("0", "2", 2),
                                   ("1", "2", 4), ("2", "3", 1)]);
        assert_true(xn::is_weighted(G));
        assert_true(xn::is_weighted(G, ("1", "0")));

        G = G.to_undirected();
        assert_true(xn::is_weighted(G));
        assert_true(xn::is_weighted(G, ("1", "0")));

        assert_raises(xn::XNetworkError, xn::is_weighted, G, (1, 2));

    auto test_is_negatively_weighted() {
        G = xn::Graph();
        assert_false(xn::is_negatively_weighted(G));

        G.add_node(1);
        G.add_nodes_from([2, 3, 4, 5]);
        assert_false(xn::is_negatively_weighted(G));

        G.add_edge(1, 2, weight=4);
        assert_false(xn::is_negatively_weighted(G, (1, 2)));

        G.add_edges_from([(1, 3), (2, 4), (2, 6)]);
        G[1][3]["color"] = "blue";
        assert_false(xn::is_negatively_weighted(G));
        assert_false(xn::is_negatively_weighted(G, (1, 3)));

        G[2][4]["weight"] = -2
        assert_true(xn::is_negatively_weighted(G, (2, 4)));
        assert_true(xn::is_negatively_weighted(G));

        G = xn::DiGraph();
        G.add_weighted_edges_from([("0", "3", 3), ("0", "1", -5),
                                   ("1", "0", -2), ("0", "2", 2),
                                   ("1", "2", -3), ("2", "3", 1)]);
        assert_true(xn::is_negatively_weighted(G));
        assert_false(xn::is_negatively_weighted(G, ("0", "3")));
        assert_true(xn::is_negatively_weighted(G, ("1", "0")));

        assert_raises(xn::XNetworkError, xn::is_negatively_weighted, G, (1, 4));


class TestCommonNeighbors() {
    auto setUp() {
        this->func = xn::common_neighbors

        auto test_func(G, u, v, expected) {
            result = sorted(this->func(G, u, v));
            assert_equal(result, expected);
        this->test = test_func

    auto test_K5() {
        G = xn::complete_graph(5);
        this->test(G, 0, 1, [2, 3, 4]);

    auto test_P3() {
        G = xn::path_graph(3);
        this->test(G, 0, 2, [1]);

    auto test_S4() {
        G = xn::star_graph(4);
        this->test(G, 1, 2, [0]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_digraph() {
        G = xn::DiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        this->func(G, 0, 2);

    auto test_nonexistent_nodes() {
        G = xn::complete_graph(5);
        assert_raises(xn::XNetworkError, xn::common_neighbors, G, 5, 4);
        assert_raises(xn::XNetworkError, xn::common_neighbors, G, 4, 5);
        assert_raises(xn::XNetworkError, xn::common_neighbors, G, 5, 6);

    auto test_custom1() {
        /** Case of no common neighbors. */
        G = xn::Graph();
        G.add_nodes_from([0, 1]);
        this->test(G, 0, 1, []);

    auto test_custom2() {
        /** Case of equal nodes. */
        G = xn::complete_graph(4);
        this->test(G, 0, 0, [1, 2, 3]);


auto test_set_node_attributes() {
    graphs = [xn::Graph(), xn::DiGraph(), xn::MultiGraph(), xn::MultiDiGraph()];
    for (auto G : graphs) {
        // Test single value
        G = xn::path_graph(3, create_using=G);
        vals = 100
        attr = "hello";
        xn::set_node_attributes(G, vals, attr);
        assert_equal(G.nodes[0][attr], vals);
        assert_equal(G.nodes[1][attr], vals);
        assert_equal(G.nodes[2][attr], vals);

        // Test dictionary
        G = xn::path_graph(3, create_using=G);
        vals = dict(zip(sorted(G.nodes()), range(len(G))));
        attr = "hi";
        xn::set_node_attributes(G, vals, attr);
        assert_equal(G.nodes[0][attr], 0);
        assert_equal(G.nodes[1][attr], 1);
        assert_equal(G.nodes[2][attr], 2);

        // Test dictionary of dictionaries
        G = xn::path_graph(3, create_using=G);
        d = {"hi": 0, "hello": 200}
        vals = dict.fromkeys(G.nodes(), d);
        vals.pop(0);
        xn::set_node_attributes(G, vals);
        assert_equal(G.nodes[0], {});
        assert_equal(G.nodes[1]["hi"], 0);
        assert_equal(G.nodes[2]["hello"], 200);


auto test_set_edge_attributes() {
    graphs = [xn::Graph(), xn::DiGraph()];
    for (auto G : graphs) {
        // Test single value
        G = xn::path_graph(3, create_using=G);
        attr = "hello";
        vals = 3
        xn::set_edge_attributes(G, vals, attr);
        assert_equal(G[0][1][attr], vals);
        assert_equal(G[1][2][attr], vals);

        // Test multiple values
        G = xn::path_graph(3, create_using=G);
        attr = "hi";
        edges = [(0, 1), (1, 2)];
        vals = dict(zip(edges, range(len(edges))));
        xn::set_edge_attributes(G, vals, attr);
        assert_equal(G[0][1][attr], 0);
        assert_equal(G[1][2][attr], 1);

        // Test dictionary of dictionaries
        G = xn::path_graph(3, create_using=G);
        d = {"hi": 0, "hello": 200}
        edges = [(0, 1)];
        vals = dict.fromkeys(edges, d);
        xn::set_edge_attributes(G, vals);
        assert_equal(G[0][1]["hi"], 0);
        assert_equal(G[0][1]["hello"], 200);
        assert_equal(G[1][2], {});


auto test_set_edge_attributes_multi() {
    graphs = [xn::MultiGraph(), xn::MultiDiGraph()];
    for (auto G : graphs) {
        // Test single value
        G = xn::path_graph(3, create_using=G);
        attr = "hello";
        vals = 3
        xn::set_edge_attributes(G, vals, attr);
        assert_equal(G[0][1][0][attr], vals);
        assert_equal(G[1][2][0][attr], vals);

        // Test multiple values
        G = xn::path_graph(3, create_using=G);
        attr = "hi";
        edges = [(0, 1, 0), (1, 2, 0)];
        vals = dict(zip(edges, range(len(edges))));
        xn::set_edge_attributes(G, vals, attr);
        assert_equal(G[0][1][0][attr], 0);
        assert_equal(G[1][2][0][attr], 1);

        // Test dictionary of dictionaries
        G = xn::path_graph(3, create_using=G);
        d = {"hi": 0, "hello": 200}
        edges = [(0, 1, 0)];
        vals = dict.fromkeys(edges, d);
        xn::set_edge_attributes(G, vals);
        assert_equal(G[0][1][0]["hi"], 0);
        assert_equal(G[0][1][0]["hello"], 200);
        assert_equal(G[1][2][0], {});


auto test_get_node_attributes() {
    graphs = [xn::Graph(), xn::DiGraph(), xn::MultiGraph(), xn::MultiDiGraph()];
    for (auto G : graphs) {
        G = xn::path_graph(3, create_using=G);
        attr = "hello";
        vals = 100
        xn::set_node_attributes(G, vals, attr);
        attrs = xn::get_node_attributes(G, attr);
        assert_equal(attrs[0], vals);
        assert_equal(attrs[1], vals);
        assert_equal(attrs[2], vals);


auto test_get_edge_attributes() {
    graphs = [xn::Graph(), xn::DiGraph(), xn::MultiGraph(), xn::MultiDiGraph()];
    for (auto G : graphs) {
        G = xn::path_graph(3, create_using=G);
        attr = "hello";
        vals = 100
        xn::set_edge_attributes(G, vals, attr);
        attrs = xn::get_edge_attributes(G, attr);

        assert_equal(len(attrs), 2);
        if (G.is_multigraph() {
            keys = [(0, 1, 0), (1, 2, 0)];
            for (auto [u, v, k] : keys) {
                try {
                    assert_equal(attrs[(u, v, k)], 100);
                } catch (KeyError) {
                    assert_equal(attrs[(v, u, k)], 100);
        } else {
            keys = [(0, 1), (1, 2)];
            for (auto [u, v] : keys) {
                try {
                    assert_equal(attrs[(u, v)], 100);
                } catch (KeyError) {
                    assert_equal(attrs[(v, u)], 100);


auto test_is_empty() {
    graphs = [xn::Graph(), xn::DiGraph(), xn::MultiGraph(), xn::MultiDiGraph()];
    for (auto G : graphs) {
        assert_true(xn::is_empty(G));
        G.add_nodes_from(range(5));
        assert_true(xn::is_empty(G));
        G.add_edges_from([(1, 2), (3, 4)]);
        assert_false(xn::is_empty(G));


auto test_selfloops() {
    graphs = [xn::Graph(), xn::DiGraph(), xn::MultiGraph(), xn::MultiDiGraph()];
    for (auto graph : graphs) {
        G = xn::complete_graph(3, create_using=graph);
        G.add_edge(0, 0);
        assert_nodes_equal(xn::nodes_with_selfloops(G), [0]);
        assert_edges_equal(xn::selfloop_edges(G), [(0, 0)]);
        assert_edges_equal(xn::selfloop_edges(G, data=true), [(0, 0, {})]);
        assert_equal(xn::number_of_selfloops(G), 1);
        // test selfloop attr
        G.add_edge(1, 1, weight=2);
        assert_edges_equal(xn::selfloop_edges(G, data=true),
                           [(0, 0, {}), (1, 1, {"weight": 2})]);
        assert_edges_equal(xn::selfloop_edges(G, data="weight"),
                           [(0, 0, None), (1, 1, 2)]);
