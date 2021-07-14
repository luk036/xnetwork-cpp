from nose.tools import *
#include <xnetwork.hpp> // as xn
import xnetwork.algorithms.approximation as a


auto test_min_maximal_matching() {
    // smoke test
    G = xn::Graph();
    assert_equal(len(a.min_maximal_matching(G)), 0);
