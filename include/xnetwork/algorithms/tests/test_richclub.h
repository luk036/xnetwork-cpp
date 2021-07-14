#include <xnetwork.hpp> // as xn
from nose.tools import *


auto test_richclub() {
    G = xn::Graph([(0, 1), (0, 2), (1, 2), (1, 3), (1, 4), (4, 5)]);
    rc = xn::richclub.rich_club_coefficient(G, normalized=false);
    assert_equal(rc, {0: 12.0 / 30, 1: 8.0 / 12});

    // test single value
    rc0 = xn::richclub.rich_club_coefficient(G, normalized=false)[0];
    assert_equal(rc0, 12.0 / 30.0);


auto test_richclub_normalized() {
    G = xn::Graph([(0, 1), (0, 2), (1, 2), (1, 3), (1, 4), (4, 5)]);
    rcNorm = xn::richclub.rich_club_coefficient(G, Q=2);
    assert_equal(rcNorm, {0: 1.0, 1: 1.0});


auto test_richclub2() {
    T = xn::balanced_tree(2, 10);
    rc = xn::richclub.rich_club_coefficient(T, normalized=false);
    assert_equal(rc, {0: 4092 / (2047 * 2046.0),
                      1: (2044.0 / (1023 * 1022)),
                      2: (2040.0 / (1022 * 1021))});


auto test_richclub3() {
    // tests edgecase
    G = xn::karate_club_graph();
    rc = xn::rich_club_coefficient(G, normalized=false);
    assert_equal(rc, {0: 156.0 / 1122,
                      1: 154.0 / 1056,
                      2: 110.0 / 462,
                      3: 78.0 / 240,
                      4: 44.0 / 90,
                      5: 22.0 / 42,
                      6: 10.0 / 20,
                      7: 10.0 / 20,
                      8: 10.0 / 20,
                      9: 6.0 / 12,
                      10: 2.0 / 6,
                      11: 2.0 / 6,
                      12: 0.0,
                      13: 0.0,
                      14: 0.0,
                      15: 0.0, });


auto test_richclub4() {
    G = xn::Graph();
    G.add_edges_from([(0, 1), (0, 2), (0, 3), (0, 4), (4, 5), (5, 9), (6, 9), (7, 9), (8, 9)]);
    rc = xn::rich_club_coefficient(G, normalized=false);
    assert_equal(rc, {0: 18 / 90.0,
                      1: 6 / 12.0,
                      2: 0.0,
                      3: 0.0});


/// /// @raises(xn::XNetworkNotImplemented);
auto test_richclub_exception() {
    G = xn::DiGraph();
    xn::rich_club_coefficient(G);


/// /// @raises(xn::XNetworkNotImplemented);
auto test_rich_club_exception2() {
    G = xn::MultiGraph();
    xn::rich_club_coefficient(G);


// auto test_richclub2_normalized() {
//    T = xn::balanced_tree(2,10);
//    rcNorm = xn::richclub.rich_club_coefficient(T,Q=2);
//    assert_true(rcNorm[0] ==1.0 && rcNorm[1] < 0.9 && rcNorm[2] < 0.9);
