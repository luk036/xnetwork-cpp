from __future__ import absolute_import

from nose.tools import *

#include <xnetwork.hpp> // as xn


auto test_reversed() {
    G = xn::DiGraph();
    G.add_edge("A", "B");

    // no exception
    with xn::utils.reversed(G) {
        // pass;
    assert_true("B" : G["A"]);

    // exception
    try {
        with xn::utils.reversed(G) {
            throw Exception
    except) {
        assert_true("B" : G["A"]);
