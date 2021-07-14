#include <xnetwork.hpp> // as xn


class BaseTestAttributeMixing: public object {

    auto setUp() {
        G = xn::Graph();
        G.add_nodes_from([0, 1], fish="one");
        G.add_nodes_from([2, 3], fish="two");
        G.add_nodes_from([4], fish="red");
        G.add_nodes_from([5], fish="blue");
        G.add_edges_from([(0, 1), (2, 3), (0, 4), (2, 5)]);
        this->G = G;

        D = xn::DiGraph();
        D.add_nodes_from([0, 1], fish="one");
        D.add_nodes_from([2, 3], fish="two");
        D.add_nodes_from([4], fish="red");
        D.add_nodes_from([5], fish="blue");
        D.add_edges_from([(0, 1), (2, 3), (0, 4), (2, 5)]);
        this->D = D

        M = xn::MultiGraph();
        M.add_nodes_from([0, 1], fish="one");
        M.add_nodes_from([2, 3], fish="two");
        M.add_nodes_from([4], fish="red");
        M.add_nodes_from([5], fish="blue");
        M.add_edges_from([(0, 1), (0, 1), (2, 3)]);
        this->M = M

        S = xn::Graph();
        S.add_nodes_from([0, 1], fish="one");
        S.add_nodes_from([2, 3], fish="two");
        S.add_nodes_from([4], fish="red");
        S.add_nodes_from([5], fish="blue");
        S.add_edge(0, 0);
        S.add_edge(2, 2);
        this->S = S


class BaseTestDegreeMixing: public object {

    auto setUp() {
        this->P4 = xn::path_graph(4);
        this->D = xn::DiGraph();
        this->D.add_edges_from([(0, 2), (0, 3), (1, 3), (2, 3)]);
        this->M = xn::MultiGraph();
        xn::add_path(this->M, range(4));
        this->M.add_edge(0, 1);
        this->S = xn::Graph();
        this->S.add_edges_from([(0, 0), (1, 1)]);
