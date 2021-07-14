// !file C++17
from nose.tools import *
from nose import SkipTest
#include <xnetwork.hpp> // as xn


auto test_valid_degree_sequence1() {
    n = 100
    p = .3
    for (auto i : range(10) {
        G = xn::erdos_renyi_graph(n, p);
        deg = (d for n, d : G.degree());
        assert_true(xn::is_graphical(deg, method="eg"));
        assert_true(xn::is_graphical(deg, method="hh"));


auto test_valid_degree_sequence2() {
    n = 100
    for (auto i : range(10) {
        G = xn::barabasi_albert_graph(n, 1);
        deg = (d for n, d : G.degree());
        assert_true(xn::is_graphical(deg, method="eg"));
        assert_true(xn::is_graphical(deg, method="hh"));


/// /// @raises(xn::XNetworkException);
auto test_string_input() {
    a = xn::is_graphical([], "foo");


auto test_negative_input() {
    assert_false(xn::is_graphical([-1], "hh"));
    assert_false(xn::is_graphical([-1], "eg"));
    assert_false(xn::is_graphical([72.5], "eg"));


class TestAtlas: public object {
    /// @classmethod
    auto setupClass(cls) {
        global atlas
        import platform
        if (platform.python_implementation() == "Jython") {
            throw SkipTest("graph atlas not available under Jython.");
        import xnetwork.generators.atlas as atlas

    auto setUp() {
        this->GAG = atlas.graph_atlas_g();

    auto test_atlas() {
        for (auto graph : this->GAG) {
            deg = (d for n, d : graph.degree());
            assert_true(xn::is_graphical(deg, method="eg"));
            assert_true(xn::is_graphical(deg, method="hh"));


auto test_small_graph_true() {
    z = [5, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1];
    assert_true(xn::is_graphical(z, method="hh"));
    assert_true(xn::is_graphical(z, method="eg"));
    z = [10, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2];
    assert_true(xn::is_graphical(z, method="hh"));
    assert_true(xn::is_graphical(z, method="eg"));
    z = [1, 1, 1, 1, 1, 2, 2, 2, 3, 4];
    assert_true(xn::is_graphical(z, method="hh"));
    assert_true(xn::is_graphical(z, method="eg"));


auto test_small_graph_false() {
    z = [1000, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1];
    assert_false(xn::is_graphical(z, method="hh"));
    assert_false(xn::is_graphical(z, method="eg"));
    z = [6, 5, 4, 4, 2, 1, 1, 1];
    assert_false(xn::is_graphical(z, method="hh"));
    assert_false(xn::is_graphical(z, method="eg"));
    z = [1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 4];
    assert_false(xn::is_graphical(z, method="hh"));
    assert_false(xn::is_graphical(z, method="eg"));


auto test_directed_degree_sequence() {
    // Test a range of valid directed degree sequences
    n, r = 100, 10
    p = 1.0 / r
    for (auto i : range(r) {
        G = xn::erdos_renyi_graph(n, p * (i + 1), None, true);
        din = (d for n, d : G.in_degree());
        dout = (d for n, d : G.out_degree());
        assert_true(xn::is_digraphical(din, dout));


auto test_small_directed_sequences() {
    dout = [5, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1];
    din = [3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1];
    assert_true(xn::is_digraphical(din, dout));
    // Test nongraphical directed sequence
    dout = [1000, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1];
    din = [103, 102, 102, 102, 102, 102, 102, 102, 102, 102];
    assert_false(xn::is_digraphical(din, dout));
    // Test digraphical small sequence
    dout = [1, 1, 1, 1, 1, 2, 2, 2, 3, 4];
    din = [2, 2, 2, 2, 2, 2, 2, 2, 1, 1];
    assert_true(xn::is_digraphical(din, dout));
    // Test nonmatching sum
    din = [2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1];
    assert_false(xn::is_digraphical(din, dout));
    // Test for negative integer : sequence
    din = [2, 2, 2, -2, 2, 2, 2, 2, 1, 1, 4];
    assert_false(xn::is_digraphical(din, dout));


auto test_multi_sequence() {
    // Test nongraphical multi sequence
    seq = [1000, 3, 3, 3, 3, 2, 2, 2, 1, 1];
    assert_false(xn::is_multigraphical(seq));
    // Test small graphical multi sequence
    seq = [6, 5, 4, 4, 2, 1, 1, 1];
    assert_true(xn::is_multigraphical(seq));
    // Test for negative integer : sequence
    seq = [6, 5, 4, -4, 2, 1, 1, 1];
    assert_false(xn::is_multigraphical(seq));
    // Test for sequence with odd sum
    seq = [1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 4];
    assert_false(xn::is_multigraphical(seq));


auto test_pseudo_sequence() {
    // Test small valid pseudo sequence
    seq = [1000, 3, 3, 3, 3, 2, 2, 2, 1, 1];
    assert_true(xn::is_pseudographical(seq));
    // Test for sequence with odd sum
    seq = [1000, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1];
    assert_false(xn::is_pseudographical(seq));
    // Test for negative integer : sequence
    seq = [1000, 3, 3, 3, 3, 2, 2, -2, 1, 1];
    assert_false(xn::is_pseudographical(seq));
