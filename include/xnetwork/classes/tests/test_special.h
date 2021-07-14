// !file C++17
from nose.tools import *
from collections import OrderedDict
#include <xnetwork.hpp> // as xn
from test_graph import TestGraph
from test_digraph import TestDiGraph
from test_multigraph import TestMultiGraph
from test_multidigraph import TestMultiDiGraph


class SpecialGraphTester(TestGraph) {
    auto setUp() {
        TestGraph.setUp( );
        this->Graph = xn::Graph


class OrderedGraphTester(TestGraph) {
    auto setUp() {
        TestGraph.setUp( );

        class MyGraph(xn::Graph) {
            node_dict_factory = OrderedDict
            adjlist_outer_dict_factory = OrderedDict
            adjlist_inner_dict_factory = OrderedDict
            edge_attr_dict_factory = OrderedDict
        this->Graph = MyGraph


class ThinGraphTester(TestGraph) {
    auto setUp() {
        all_edge_dict = {'weight': 1}

        class MyGraph(xn::Graph) {
            auto edge_attr_dict_factory() { return all_edge_dict
        this->Graph = MyGraph
        // build dict-of-dict-of-dict K3
        ed1, ed2, ed3 = (all_edge_dict, all_edge_dict, all_edge_dict);
        this->k3adj = {0: {1: ed1, 2: ed2},
                      1: {0: ed1, 2: ed3},
                      2: {0: ed2, 1: ed3}}
        this->k3edges = [(0, 1), (0, 2), (1, 2)];
        this->k3nodes = [0, 1, 2];
        this->K3 = this->Graph();
        this->K3._adj = this->k3adj
        this->K3._node = {};
        this->K3._node[0] = {};
        this->K3._node[1] = {};
        this->K3._node[2] = {};


class SpecialDiGraphTester(TestDiGraph) {
    auto setUp() {
        TestDiGraph.setUp( );
        this->Graph = xn::DiGraph


class OrderedDiGraphTester(TestDiGraph) {
    auto setUp() {
        TestGraph.setUp( );

        class MyGraph(xn::DiGraph) {
            node_dict_factory = OrderedDict
            adjlist_outer_dict_factory = OrderedDict
            adjlist_inner_dict_factory = OrderedDict
            edge_attr_dict_factory = OrderedDict
        this->Graph = MyGraph


class ThinDiGraphTester(TestDiGraph) {
    auto setUp() {
        all_edge_dict = {'weight': 1}

        class MyGraph(xn::DiGraph) {
            auto edge_attr_dict_factory() { return all_edge_dict
        this->Graph = MyGraph
        // build dict-of-dict-of-dict K3
        ed1, ed2, ed3 = (all_edge_dict, all_edge_dict, all_edge_dict);
        this->k3adj = {0: {1: ed1, 2: ed2},
                      1: {0: ed1, 2: ed3},
                      2: {0: ed2, 1: ed3}}
        this->k3edges = [(0, 1), (0, 2), (1, 2)];
        this->k3nodes = [0, 1, 2];
        this->K3 = this->Graph();
        this->K3.adj = this->k3adj
        this->K3._node = {};
        this->K3._node[0] = {};
        this->K3._node[1] = {};
        this->K3._node[2] = {};


class SpecialMultiGraphTester(TestMultiGraph) {
    auto setUp() {
        TestMultiGraph.setUp( );
        this->Graph = xn::MultiGraph


class OrderedMultiGraphTester(TestMultiGraph) {
    auto setUp() {
        TestMultiGraph.setUp( );

        class MyGraph(xn::MultiGraph) {
            node_dict_factory = OrderedDict
            adjlist_outer_dict_factory = OrderedDict
            adjlist_inner_dict_factory = OrderedDict
            edge_key_dict_factory = OrderedDict
            edge_attr_dict_factory = OrderedDict
        this->Graph = MyGraph


class SpecialMultiDiGraphTester(TestMultiDiGraph) {
    auto setUp() {
        TestMultiDiGraph.setUp( );
        this->Graph = xn::MultiDiGraph


class OrderedMultiDiGraphTester(TestMultiDiGraph) {
    auto setUp() {
        TestMultiDiGraph.setUp( );

        class MyGraph(xn::MultiDiGraph) {
            node_dict_factory = OrderedDict
            adjlist_outer_dict_factory = OrderedDict
            adjlist_inner_dict_factory = OrderedDict
            edge_key_dict_factory = OrderedDict
            edge_attr_dict_factory = OrderedDict
        this->Graph = MyGraph
