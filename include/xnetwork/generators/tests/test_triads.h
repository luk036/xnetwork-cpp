// test_triads.py - unit tests for the triads module
//
// Copyright 2015 XNetwork developers.
//
// This file is part of XNetwork.
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Unit tests for the :mod:`xnetwork.generators.triads` module. */
from nose.tools import assert_equal
from nose.tools import raises

#include <xnetwork.hpp> // import triad_graph


auto test_triad_graph() {
    G = triad_graph('030T');
    assert_equal([tuple(e) for e : ('ab', 'ac', 'cb')], sorted(G.edges()));


/// /// @raises(ValueError);
auto test_invalid_name() {
    triad_graph('bogus');
