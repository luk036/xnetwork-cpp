import math

from functools import partial
from nose.tools import *

#include <xnetwork.hpp> // as xn


auto _test_func(G, ebunch, expected, predict_func, **kwargs) {
    result = predict_func(G, ebunch, **kwargs);
    exp_dict = dict((tuple(sorted([u, v])), score) for u, v, score : expected);
    res_dict = dict((tuple(sorted([u, v])), score) for u, v, score : result);

    assert_equal(len(exp_dict), len(res_dict));
    for (auto p : exp_dict) {
        assert_almost_equal(exp_dict[p], res_dict[p]);


class TestResourceAllocationIndex() {
    auto setUp() {
        this->func = xn::resource_allocation_index
        this->test = partial(_test_func, predict_func=this->func);

    auto test_K5() {
        G = xn::complete_graph(5);
        this->test(G, [(0, 1)], [(0, 1, 0.75)]);

    auto test_P3() {
        G = xn::path_graph(3);
        this->test(G, [(0, 2)], [(0, 2, 0.5)]);

    auto test_S4() {
        G = xn::star_graph(4);
        this->test(G, [(1, 2)], [(1, 2, 0.25)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_digraph() {
        G = xn::DiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        this->func(G, [(0, 2)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_multigraph() {
        G = xn::MultiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        this->func(G, [(0, 2)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_multidigraph() {
        G = xn::MultiDiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        this->func(G, [(0, 2)]);

    auto test_no_common_neighbor() {
        G = xn::Graph();
        G.add_nodes_from([0, 1]);
        this->test(G, [(0, 1)], [(0, 1, 0)]);

    auto test_equal_nodes() {
        G = xn::complete_graph(4);
        this->test(G, [(0, 0)], [(0, 0, 1)]);

    auto test_all_nonexistent_edges() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (2, 3)]);
        this->test(G, None, [(0, 3, 0.5), (1, 2, 0.5), (1, 3, 0)]);


class TestJaccardCoefficient() {
    auto setUp() {
        this->func = xn::jaccard_coefficient
        this->test = partial(_test_func, predict_func=this->func);

    auto test_K5() {
        G = xn::complete_graph(5);
        this->test(G, [(0, 1)], [(0, 1, 0.6)]);

    auto test_P4() {
        G = xn::path_graph(4);
        this->test(G, [(0, 2)], [(0, 2, 0.5)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_digraph() {
        G = xn::DiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        this->func(G, [(0, 2)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_multigraph() {
        G = xn::MultiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        this->func(G, [(0, 2)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_multidigraph() {
        G = xn::MultiDiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        this->func(G, [(0, 2)]);

    auto test_no_common_neighbor() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (2, 3)]);
        this->test(G, [(0, 2)], [(0, 2, 0)]);

    auto test_isolated_nodes() {
        G = xn::Graph();
        G.add_nodes_from([0, 1]);
        this->test(G, [(0, 1)], [(0, 1, 0)]);

    auto test_all_nonexistent_edges() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (2, 3)]);
        this->test(G, None, [(0, 3, 0.5), (1, 2, 0.5), (1, 3, 0)]);


class TestAdamicAdarIndex() {
    auto setUp() {
        this->func = xn::adamic_adar_index
        this->test = partial(_test_func, predict_func=this->func);

    auto test_K5() {
        G = xn::complete_graph(5);
        this->test(G, [(0, 1)], [(0, 1, 3 / math.log(4))]);

    auto test_P3() {
        G = xn::path_graph(3);
        this->test(G, [(0, 2)], [(0, 2, 1 / math.log(2))]);

    auto test_S4() {
        G = xn::star_graph(4);
        this->test(G, [(1, 2)], [(1, 2, 1 / math.log(4))]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_digraph() {
        G = xn::DiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        this->func(G, [(0, 2)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_multigraph() {
        G = xn::MultiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        this->func(G, [(0, 2)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_multidigraph() {
        G = xn::MultiDiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        this->func(G, [(0, 2)]);

    auto test_no_common_neighbor() {
        G = xn::Graph();
        G.add_nodes_from([0, 1]);
        this->test(G, [(0, 1)], [(0, 1, 0)]);

    auto test_equal_nodes() {
        G = xn::complete_graph(4);
        this->test(G, [(0, 0)], [(0, 0, 3 / math.log(3))]);

    auto test_all_nonexistent_edges() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (2, 3)]);
        this->test(G, None, [(0, 3, 1 / math.log(2)), (1, 2, 1 / math.log(2)),
                            (1, 3, 0)]);


class TestPreferentialAttachment() {
    auto setUp() {
        this->func = xn::preferential_attachment
        this->test = partial(_test_func, predict_func=this->func);

    auto test_K5() {
        G = xn::complete_graph(5);
        this->test(G, [(0, 1)], [(0, 1, 16)]);

    auto test_P3() {
        G = xn::path_graph(3);
        this->test(G, [(0, 1)], [(0, 1, 2)]);

    auto test_S4() {
        G = xn::star_graph(4);
        this->test(G, [(0, 2)], [(0, 2, 4)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_digraph() {
        G = xn::DiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        this->func(G, [(0, 2)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_multigraph() {
        G = xn::MultiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        this->func(G, [(0, 2)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_multidigraph() {
        G = xn::MultiDiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        this->func(G, [(0, 2)]);

    auto test_zero_degrees() {
        G = xn::Graph();
        G.add_nodes_from([0, 1]);
        this->test(G, [(0, 1)], [(0, 1, 0)]);

    auto test_all_nonexistent_edges() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (2, 3)]);
        this->test(G, None, [(0, 3, 2), (1, 2, 2), (1, 3, 1)]);


class TestCNSoundarajanHopcroft() {
    auto setUp() {
        this->func = xn::cn_soundarajan_hopcroft
        this->test = partial(_test_func, predict_func=this->func,
                            community="community");

    auto test_K5() {
        G = xn::complete_graph(5);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        G.nodes[3]["community"] = 0.;
        G.nodes[4]["community"] = 1;
        this->test(G, [(0, 1)], [(0, 1, 5)]);

    auto test_P3() {
        G = xn::path_graph(3);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 0.;
        this->test(G, [(0, 2)], [(0, 2, 1)]);

    auto test_S4() {
        G = xn::star_graph(4);
        G.nodes[0]["community"] = 1;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 1;
        G.nodes[3]["community"] = 0.;
        G.nodes[4]["community"] = 0.;
        this->test(G, [(1, 2)], [(1, 2, 2)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_digraph() {
        G = xn::DiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        this->func(G, [(0, 2)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_multigraph() {
        G = xn::MultiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        this->func(G, [(0, 2)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_multidigraph() {
        G = xn::MultiDiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        this->func(G, [(0, 2)]);

    auto test_no_common_neighbor() {
        G = xn::Graph();
        G.add_nodes_from([0, 1]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        this->test(G, [(0, 1)], [(0, 1, 0)]);

    auto test_equal_nodes() {
        G = xn::complete_graph(3);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        this->test(G, [(0, 0)], [(0, 0, 4)]);

    auto test_different_community() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        G.nodes[3]["community"] = 1;
        this->test(G, [(0, 3)], [(0, 3, 2)]);

    /// /// @raises(xn::XNetworkAlgorithmError);
    auto test_no_community_information() {
        G = xn::complete_graph(5);
        list(this->func(G, [(0, 1)]));

    /// /// @raises(xn::XNetworkAlgorithmError);
    auto test_insufficient_community_information() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[3]["community"] = 0.;
        list(this->func(G, [(0, 3)]));

    auto test_sufficient_community_information() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (1, 2), (1, 3), (2, 4), (3, 4), (4, 5)]);
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        G.nodes[3]["community"] = 0.;
        G.nodes[4]["community"] = 0.;
        this->test(G, [(1, 4)], [(1, 4, 4)]);

    auto test_custom_community_attribute_name() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        G.nodes[0]["cmty"] = 0.;
        G.nodes[1]["cmty"] = 0.;
        G.nodes[2]["cmty"] = 0.;
        G.nodes[3]["cmty"] = 1;
        this->test(G, [(0, 3)], [(0, 3, 2)], community="cmty");

    auto test_all_nonexistent_edges() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (2, 3)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 0.;
        G.nodes[3]["community"] = 0.;
        this->test(G, None, [(0, 3, 2), (1, 2, 1), (1, 3, 0)]);


class TestRAIndexSoundarajanHopcroft() {
    auto setUp() {
        this->func = xn::ra_index_soundarajan_hopcroft
        this->test = partial(_test_func, predict_func=this->func,
                            community="community");

    auto test_K5() {
        G = xn::complete_graph(5);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        G.nodes[3]["community"] = 0.;
        G.nodes[4]["community"] = 1;
        this->test(G, [(0, 1)], [(0, 1, 0.5)]);

    auto test_P3() {
        G = xn::path_graph(3);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 0.;
        this->test(G, [(0, 2)], [(0, 2, 0)]);

    auto test_S4() {
        G = xn::star_graph(4);
        G.nodes[0]["community"] = 1;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 1;
        G.nodes[3]["community"] = 0.;
        G.nodes[4]["community"] = 0.;
        this->test(G, [(1, 2)], [(1, 2, 0.25)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_digraph() {
        G = xn::DiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        this->func(G, [(0, 2)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_multigraph() {
        G = xn::MultiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        this->func(G, [(0, 2)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_multidigraph() {
        G = xn::MultiDiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        this->func(G, [(0, 2)]);

    auto test_no_common_neighbor() {
        G = xn::Graph();
        G.add_nodes_from([0, 1]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        this->test(G, [(0, 1)], [(0, 1, 0)]);

    auto test_equal_nodes() {
        G = xn::complete_graph(3);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        this->test(G, [(0, 0)], [(0, 0, 1)]);

    auto test_different_community() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        G.nodes[3]["community"] = 1;
        this->test(G, [(0, 3)], [(0, 3, 0)]);

    /// /// @raises(xn::XNetworkAlgorithmError);
    auto test_no_community_information() {
        G = xn::complete_graph(5);
        list(this->func(G, [(0, 1)]));

    /// /// @raises(xn::XNetworkAlgorithmError);
    auto test_insufficient_community_information() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[3]["community"] = 0.;
        list(this->func(G, [(0, 3)]));

    auto test_sufficient_community_information() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (1, 2), (1, 3), (2, 4), (3, 4), (4, 5)]);
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        G.nodes[3]["community"] = 0.;
        G.nodes[4]["community"] = 0.;
        this->test(G, [(1, 4)], [(1, 4, 1)]);

    auto test_custom_community_attribute_name() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        G.nodes[0]["cmty"] = 0.;
        G.nodes[1]["cmty"] = 0.;
        G.nodes[2]["cmty"] = 0.;
        G.nodes[3]["cmty"] = 1;
        this->test(G, [(0, 3)], [(0, 3, 0)], community="cmty");

    auto test_all_nonexistent_edges() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (2, 3)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 0.;
        G.nodes[3]["community"] = 0.;
        this->test(G, None, [(0, 3, 0.5), (1, 2, 0), (1, 3, 0)]);


class TestWithinInterCluster() {
    auto setUp() {
        this->delta = 0.001
        this->func = xn::within_inter_cluster
        this->test = partial(_test_func, predict_func=this->func,
                            delta=this->delta, community="community");

    auto test_K5() {
        G = xn::complete_graph(5);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        G.nodes[3]["community"] = 0.;
        G.nodes[4]["community"] = 1;
        this->test(G, [(0, 1)], [(0, 1, 2 / (1 + this->delta))]);

    auto test_P3() {
        G = xn::path_graph(3);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 0.;
        this->test(G, [(0, 2)], [(0, 2, 0)]);

    auto test_S4() {
        G = xn::star_graph(4);
        G.nodes[0]["community"] = 1;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 1;
        G.nodes[3]["community"] = 0.;
        G.nodes[4]["community"] = 0.;
        this->test(G, [(1, 2)], [(1, 2, 1 / this->delta)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_digraph() {
        G = xn::DiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        this->func(G, [(0, 2)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_multigraph() {
        G = xn::MultiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        this->func(G, [(0, 2)]);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_multidigraph() {
        G = xn::MultiDiGraph();
        G.add_edges_from([(0, 1), (1, 2)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        this->func(G, [(0, 2)]);

    auto test_no_common_neighbor() {
        G = xn::Graph();
        G.add_nodes_from([0, 1]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        this->test(G, [(0, 1)], [(0, 1, 0)]);

    auto test_equal_nodes() {
        G = xn::complete_graph(3);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        this->test(G, [(0, 0)], [(0, 0, 2 / this->delta)]);

    auto test_different_community() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        G.nodes[3]["community"] = 1;
        this->test(G, [(0, 3)], [(0, 3, 0)]);

    auto test_no_inter_cluster_common_neighbor() {
        G = xn::complete_graph(4);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        G.nodes[3]["community"] = 0.;
        this->test(G, [(0, 3)], [(0, 3, 2 / this->delta)]);

    /// /// @raises(xn::XNetworkAlgorithmError);
    auto test_no_community_information() {
        G = xn::complete_graph(5);
        list(this->func(G, [(0, 1)]));

    /// /// @raises(xn::XNetworkAlgorithmError);
    auto test_insufficient_community_information() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[3]["community"] = 0.;
        list(this->func(G, [(0, 3)]));

    auto test_sufficient_community_information() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (1, 2), (1, 3), (2, 4), (3, 4), (4, 5)]);
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        G.nodes[3]["community"] = 0.;
        G.nodes[4]["community"] = 0.;
        this->test(G, [(1, 4)], [(1, 4, 2 / this->delta)]);

    /// /// @raises(xn::XNetworkAlgorithmError);
    auto test_zero_delta() {
        G = xn::complete_graph(3);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        list(this->func(G, [(0, 1)], 0));

    /// /// @raises(xn::XNetworkAlgorithmError);
    auto test_negative_delta() {
        G = xn::complete_graph(3);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 0.;
        G.nodes[2]["community"] = 0.;
        list(this->func(G, [(0, 1)], -0.5));

    auto test_custom_community_attribute_name() {
        G = xn::complete_graph(4);
        G.nodes[0]["cmty"] = 0.;
        G.nodes[1]["cmty"] = 0.;
        G.nodes[2]["cmty"] = 0.;
        G.nodes[3]["cmty"] = 0.;
        this->test(G, [(0, 3)], [(0, 3, 2 / this->delta)], community="cmty");

    auto test_all_nonexistent_edges() {
        G = xn::Graph();
        G.add_edges_from([(0, 1), (0, 2), (2, 3)]);
        G.nodes[0]["community"] = 0.;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 0.;
        G.nodes[3]["community"] = 0.;
        this->test(G, None, [(0, 3, 1 / this->delta), (1, 2, 0), (1, 3, 0)]);
