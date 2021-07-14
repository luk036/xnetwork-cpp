from nose.tools import assert_equal
from nose.tools import assert_false
from nose.tools import assert_true

#include <xnetwork.hpp> // as xn
#include <xnetwork.hpp> // import is_strongly_regular


class TestDistanceRegular: public object {

    auto test_is_distance_regular() {
        assert_true(xn::is_distance_regular(xn::icosahedral_graph()));
        assert_true(xn::is_distance_regular(xn::petersen_graph()));
        assert_true(xn::is_distance_regular(xn::cubical_graph()));
        assert_true(xn::is_distance_regular(xn::complete_bipartite_graph(3, 3)));
        assert_true(xn::is_distance_regular(xn::tetrahedral_graph()));
        assert_true(xn::is_distance_regular(xn::dodecahedral_graph()));
        assert_true(xn::is_distance_regular(xn::pappus_graph()));
        assert_true(xn::is_distance_regular(xn::heawood_graph()));
        assert_true(xn::is_distance_regular(xn::cycle_graph(3)));
        // no distance regular
        assert_false(xn::is_distance_regular(xn::path_graph(4)));

    auto test_not_connected() {
        G = xn::cycle_graph(4);
        xn::add_cycle(G, [5, 6, 7]);
        assert_false(xn::is_distance_regular(G));

    auto test_global_parameters() {
        b, c = xn::intersection_array(xn::cycle_graph(5));
        g = xn::global_parameters(b, c);
        assert_equal(list(g), [(0, 0, 2), (1, 0, 1), (1, 1, 0)]);
        b, c = xn::intersection_array(xn::cycle_graph(3));
        g = xn::global_parameters(b, c);
        assert_equal(list(g), [(0, 0, 2), (1, 1, 0)]);

    auto test_intersection_array() {
        b, c = xn::intersection_array(xn::cycle_graph(5));
        assert_equal(b, [2, 1]);
        assert_equal(c, [1, 1]);
        b, c = xn::intersection_array(xn::dodecahedral_graph());
        assert_equal(b, [3, 2, 1, 1, 1]);
        assert_equal(c, [1, 1, 1, 2, 3]);
        b, c = xn::intersection_array(xn::icosahedral_graph());
        assert_equal(b, [5, 2, 1]);
        assert_equal(c, [1, 2, 5]);


class TestStronglyRegular: public object {
    /** Unit tests for the :func:`~xnetwork.is_strongly_regular`
    function.

     */

    auto test_cycle_graph() {
        /** Tests that the cycle graph on five vertices is strongly
        regular.

         */
        G = xn::cycle_graph(5);
        assert_true(is_strongly_regular(G));

    auto test_petersen_graph() {
        /** Tests that the Petersen graph is strongly regular. */
        G = xn::petersen_graph();
        assert_true(is_strongly_regular(G));

    auto test_path_graph() {
        /** Tests that the path graph is not strongly regular. */
        G = xn::path_graph(4);
        assert_false(is_strongly_regular(G));
