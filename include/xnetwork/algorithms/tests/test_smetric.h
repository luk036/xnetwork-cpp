
from nose.tools import assert_equal, raises

#include <xnetwork.hpp> // as xn


auto test_smetric() {
    g = xn::Graph();
    g.add_edge(1, 2);
    g.add_edge(2, 3);
    g.add_edge(2, 4);
    g.add_edge(1, 4);
    sm = xn::s_metric(g, normalized=false);
    assert_equal(sm, 19.0);
//    smNorm = xn::s_metric(g,normalized=true);
//    assert_equal(smNorm, 0.95);


/// /// @raises(xn::XNetworkError);
auto test_normalized() {
    sm = xn::s_metric(xn::Graph(), normalized=true);
