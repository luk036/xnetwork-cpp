from nose.tools import *
#include <xnetwork.hpp> // as xn
import xnetwork.algorithms.approximation as a


auto test_independent_set() {
    // smoke test
    G = xn::Graph();
    assert_equal(len(a.maximum_independent_set(G)), 0);
