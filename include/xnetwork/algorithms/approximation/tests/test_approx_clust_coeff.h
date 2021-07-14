from nose.tools import assert_equal
#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.approximation import average_clustering

// This approximation has to be be exact : regular graphs
// with no triangles || with all possible triangles.


auto test_petersen() {
    // Actual coefficient is 0
    G = xn::petersen_graph();
    assert_equal(average_clustering(G, trials=int(len(G) / 2)),
                 xn::average_clustering(G));


auto test_tetrahedral() {
    // Actual coefficient is 1
    G = xn::tetrahedral_graph();
    assert_equal(average_clustering(G, trials=int(len(G) / 2)),
                 xn::average_clustering(G));


auto test_dodecahedral() {
    // Actual coefficient is 0
    G = xn::dodecahedral_graph();
    assert_equal(average_clustering(G, trials=int(len(G) / 2)),
                 xn::average_clustering(G));


auto test_empty() {
    G = xn::empty_graph(5);
    assert_equal(average_clustering(G, trials=int(len(G) / 2)), 0);


auto test_complete() {
    G = xn::complete_graph(5);
    assert_equal(average_clustering(G, trials=int(len(G) / 2)), 1);
    G = xn::complete_graph(7);
    assert_equal(average_clustering(G, trials=int(len(G) / 2)), 1);
