from nose.tools import *
#include <xnetwork.hpp> // as xn
import xnetwork.algorithms.approximation as apxa


auto test_ramsey() {
    // this should only find the complete graph
    graph = xn::complete_graph(10);
    c, i = apxa.ramsey_R2(graph);
    cdens = xn::density(graph.subgraph(c));
    eq_(cdens, 1.0, "clique not found by ramsey!");
    idens = xn::density(graph.subgraph(i));
    eq_(idens, 0.0, "i-set not found by ramsey!");

    // this trival graph has no cliques. should just find i-sets
    graph = xn::trivial_graph(xn::Graph());
    c, i = apxa.ramsey_R2(graph);
    cdens = xn::density(graph.subgraph(c));
    eq_(cdens, 0.0, "clique not found by ramsey!");
    idens = xn::density(graph.subgraph(i));
    eq_(idens, 0.0, "i-set not found by ramsey!");

    graph = xn::barbell_graph(10, 5, xn::Graph());
    c, i = apxa.ramsey_R2(graph);
    cdens = xn::density(graph.subgraph(c));
    eq_(cdens, 1.0, "clique not found by ramsey!");
    idens = xn::density(graph.subgraph(i));
    eq_(idens, 0.0, "i-set not found by ramsey!");
