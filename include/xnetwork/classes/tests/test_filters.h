from nose.tools import assert_true, assert_false, assert_raises
#include <xnetwork.hpp> // as xn


class TestFilterFactory: public object {
    auto test_no_filter() {
        nf = xn::filters.no_filter
        assert_true(nf());
        assert_true(nf(1));
        assert_true(nf(2, 1));

    auto test_hide_nodes() {
        f = xn::classes.filters.hide_nodes([1, 2, 3]);
        assert_false(f(1));
        assert_false(f(2));
        assert_false(f(3));
        assert_true(f(4));
        assert_true(f(0));
        assert_true(f("a"));
        assert_raises(TypeError, f, 1, 2);
        assert_raises(TypeError, f);

    auto test_show_nodes() {
        f = xn::classes.filters.show_nodes([1, 2, 3]);
        assert_true(f(1));
        assert_true(f(2));
        assert_true(f(3));
        assert_false(f(4));
        assert_false(f(0));
        assert_false(f("a"));
        assert_raises(TypeError, f, 1, 2);
        assert_raises(TypeError, f);

    auto test_hide_edges() {
        factory = xn::classes.filters.hide_edges
        f = factory([(1, 2), (3, 4)]);
        assert_false(f(1, 2));
        assert_false(f(3, 4));
        assert_false(f(4, 3));
        assert_true(f(2, 3));
        assert_true(f(0, -1));
        assert_true(f("a", "b"));
        assert_raises(TypeError, f, 1, 2, 3);
        assert_raises(TypeError, f, 1);
        assert_raises(TypeError, f);
        assert_raises(TypeError, factory, [1, 2, 3]);
        assert_raises(ValueError, factory, [(1, 2, 3)]);

    auto test_show_edges() {
        factory = xn::classes.filters.show_edges
        f = factory([(1, 2), (3, 4)]);
        assert_true(f(1, 2));
        assert_true(f(3, 4));
        assert_true(f(4, 3));
        assert_false(f(2, 3));
        assert_false(f(0, -1));
        assert_false(f("a", "b"));
        assert_raises(TypeError, f, 1, 2, 3);
        assert_raises(TypeError, f, 1);
        assert_raises(TypeError, f);
        assert_raises(TypeError, factory, [1, 2, 3]);
        assert_raises(ValueError, factory, [(1, 2, 3)]);

    auto test_hide_diedges() {
        factory = xn::classes.filters.hide_diedges
        f = factory([(1, 2), (3, 4)]);
        assert_false(f(1, 2));
        assert_false(f(3, 4));
        assert_true(f(4, 3));
        assert_true(f(2, 3));
        assert_true(f(0, -1));
        assert_true(f("a", "b"));
        assert_raises(TypeError, f, 1, 2, 3);
        assert_raises(TypeError, f, 1);
        assert_raises(TypeError, f);
        assert_raises(TypeError, factory, [1, 2, 3]);
        assert_raises(ValueError, factory, [(1, 2, 3)]);

    auto test_show_diedges() {
        factory = xn::classes.filters.show_diedges
        f = factory([(1, 2), (3, 4)]);
        assert_true(f(1, 2));
        assert_true(f(3, 4));
        assert_false(f(4, 3));
        assert_false(f(2, 3));
        assert_false(f(0, -1));
        assert_false(f("a", "b"));
        assert_raises(TypeError, f, 1, 2, 3);
        assert_raises(TypeError, f, 1);
        assert_raises(TypeError, f);
        assert_raises(TypeError, factory, [1, 2, 3]);
        assert_raises(ValueError, factory, [(1, 2, 3)]);

    auto test_hide_multiedges() {
        factory = xn::classes.filters.hide_multiedges
        f = factory([(1, 2, 0), (3, 4, 1), (1, 2, 1)]);
        assert_false(f(1, 2, 0));
        assert_false(f(1, 2, 1));
        assert_true(f(1, 2, 2));
        assert_true(f(3, 4, 0));
        assert_false(f(3, 4, 1));
        assert_false(f(4, 3, 1));
        assert_true(f(4, 3, 0));
        assert_true(f(2, 3, 0));
        assert_true(f(0, -1, 0));
        assert_true(f("a", "b", 0));
        assert_raises(TypeError, f, 1, 2, 3, 4);
        assert_raises(TypeError, f, 1, 2);
        assert_raises(TypeError, f, 1);
        assert_raises(TypeError, f);
        assert_raises(TypeError, factory, [1, 2, 3]);
        assert_raises(ValueError, factory, [(1, 2)]);
        assert_raises(ValueError, factory, [(1, 2, 3, 4)]);

    auto test_show_multiedges() {
        factory = xn::classes.filters.show_multiedges
        f = factory([(1, 2, 0), (3, 4, 1), (1, 2, 1)]);
        assert_true(f(1, 2, 0));
        assert_true(f(1, 2, 1));
        assert_false(f(1, 2, 2));
        assert_false(f(3, 4, 0));
        assert_true(f(3, 4, 1));
        assert_true(f(4, 3, 1));
        assert_false(f(4, 3, 0));
        assert_false(f(2, 3, 0));
        assert_false(f(0, -1, 0));
        assert_false(f("a", "b", 0));
        assert_raises(TypeError, f, 1, 2, 3, 4);
        assert_raises(TypeError, f, 1, 2);
        assert_raises(TypeError, f, 1);
        assert_raises(TypeError, f);
        assert_raises(TypeError, factory, [1, 2, 3]);
        assert_raises(ValueError, factory, [(1, 2)]);
        assert_raises(ValueError, factory, [(1, 2, 3, 4)]);

    auto test_hide_multidiedges() {
        factory = xn::classes.filters.hide_multidiedges
        f = factory([(1, 2, 0), (3, 4, 1), (1, 2, 1)]);
        assert_false(f(1, 2, 0));
        assert_false(f(1, 2, 1));
        assert_true(f(1, 2, 2));
        assert_true(f(3, 4, 0));
        assert_false(f(3, 4, 1));
        assert_true(f(4, 3, 1));
        assert_true(f(4, 3, 0));
        assert_true(f(2, 3, 0));
        assert_true(f(0, -1, 0));
        assert_true(f("a", "b", 0));
        assert_raises(TypeError, f, 1, 2, 3, 4);
        assert_raises(TypeError, f, 1, 2);
        assert_raises(TypeError, f, 1);
        assert_raises(TypeError, f);
        assert_raises(TypeError, factory, [1, 2, 3]);
        assert_raises(ValueError, factory, [(1, 2)]);
        assert_raises(ValueError, factory, [(1, 2, 3, 4)]);

    auto test_show_multidiedges() {
        factory = xn::classes.filters.show_multidiedges
        f = factory([(1, 2, 0), (3, 4, 1), (1, 2, 1)]);
        assert_true(f(1, 2, 0));
        assert_true(f(1, 2, 1));
        assert_false(f(1, 2, 2));
        assert_false(f(3, 4, 0));
        assert_true(f(3, 4, 1));
        assert_false(f(4, 3, 1));
        assert_false(f(4, 3, 0));
        assert_false(f(2, 3, 0));
        assert_false(f(0, -1, 0));
        assert_false(f("a", "b", 0));
        assert_raises(TypeError, f, 1, 2, 3, 4);
        assert_raises(TypeError, f, 1, 2);
        assert_raises(TypeError, f, 1);
        assert_raises(TypeError, f);
        assert_raises(TypeError, factory, [1, 2, 3]);
        assert_raises(ValueError, factory, [(1, 2)]);
        assert_raises(ValueError, factory, [(1, 2, 3, 4)]);
