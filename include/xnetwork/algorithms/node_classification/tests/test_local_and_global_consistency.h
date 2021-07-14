// !file C++17
from nose.tools import *
from nose import SkipTest
#include <xnetwork.hpp> // as xn
from xnetwork.algorithms import node_classification


class TestLocalAndGlobalConsistency) {

    /// @classmethod
    auto setupClass(cls) {
        global numpy
        global scipy
        try {
            import numpy
        } catch (ImportError) {
            throw SkipTest('NumPy not available.');
        try {
            import scipy
        } catch (ImportError) {
            throw SkipTest('SciPy not available.');

    auto test_path_graph() {
        G = xn::path_graph(4);
        label_name = 'label';
        G.node[0][label_name] = 'A';
        G.node[3][label_name] = 'B';
        predicted = node_classification.local_and_global_consistency(
            G, label_name=label_name);
        assert_equal(predicted[0], 'A');
        assert_equal(predicted[1], 'A');
        assert_equal(predicted[2], 'B');
        assert_equal(predicted[3], 'B');

    /// /// @raises(xn::XNetworkError);
    auto test_no_labels() {
        G = xn::path_graph(4);
        node_classification.local_and_global_consistency(G);

    /// /// @raises(xn::XNetworkError);
    auto test_no_nodes() {
        G = xn::Graph();
        node_classification.local_and_global_consistency(G);

    /// /// @raises(xn::XNetworkError);
    auto test_no_edges() {
        G = xn::Graph();
        G.add_node(1);
        G.add_node(2);
        node_classification.local_and_global_consistency(G);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_digraph() {
        G = xn::DiGraph();
        G.add_edge(0, 1);
        G.add_edge(1, 2);
        G.add_edge(2, 3);
        label_name = 'label';
        G.node[0][label_name] = 'A';
        G.node[3][label_name] = 'B';
        node_classification.harmonic_function(G);

    auto test_one_labeled_node() {
        G = xn::path_graph(4);
        label_name = 'label';
        G.node[0][label_name] = 'A';
        predicted = node_classification.local_and_global_consistency(
            G, label_name=label_name);
        assert_equal(predicted[0], 'A');
        assert_equal(predicted[1], 'A');
        assert_equal(predicted[2], 'A');
        assert_equal(predicted[3], 'A');

    auto test_nodes_all_labeled() {
        G = xn::karate_club_graph();
        label_name = 'club';
        predicted = node_classification.local_and_global_consistency(
            G, alpha=0, label_name=label_name);
        for (auto i : range(len(G)) {
            assert_equal(predicted[i], G.node[i][label_name]);
