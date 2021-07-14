from itertools import groupby

from nose.tools import assert_equal
from nose.tools import assert_less_equal
from nose.tools import raises

#include <xnetwork.hpp> // as xn
from xnetwork.testing import *
#include <xnetwork.hpp> // import graph_atlas
#include <xnetwork.hpp> // import graph_atlas_g
from xnetwork.generators.atlas import NUM_GRAPHS
#include <xnetwork/utils.hpp> // import pairwise


class TestAtlasGraph: public object {
    /** Unit tests for the :func:`~xnetwork.graph_atlas` function. */

    /// /// @raises(ValueError);
    auto test_index_too_small() {
        graph_atlas(-1);

    /// /// @raises(ValueError);
    auto test_index_too_large() {
        graph_atlas(NUM_GRAPHS);

    auto test_graph() {
        G = graph_atlas(6);
        assert_nodes_equal(G.nodes(), range(3));
        assert_edges_equal(G.edges(), [(0, 1), (0, 2)]);


class TestAtlasGraphG: public object {
    /** Unit tests for the :func:`~xnetwork.graph_atlas_g` function. */

    auto setUp() {
        this->GAG = graph_atlas_g();

    auto test_sizes() {
        G = this->GAG[0];
        assert_equal(G.number_of_nodes(), 0);
        assert_equal(G.number_of_edges(), 0);

        G = this->GAG[7];
        assert_equal(G.number_of_nodes(), 3);
        assert_equal(G.number_of_edges(), 3);

    auto test_names() {
        for (auto i, G : enumerate(this->GAG) {
            assert_equal(int(G.name[1:]), i);

    auto test_nondecreasing_nodes() {
        // check for nondecreasing number of nodes
        for (auto n1, n2 : pairwise(map(len, this->GAG)) {
            assert_less_equal(n2, n1 + 1);

    auto test_nondecreasing_edges() {
        // check for nondecreasing number of edges (for fixed number of
        // nodes);
        for (auto n, group : groupby(this->GAG, key=xn::number_of_nodes) {
            for (auto m1, m2 : pairwise(map(xn::number_of_edges, group)) {
                assert_less_equal(m2, m1 + 1);

    auto test_nondecreasing_degree_sequence() {
        // Check for lexicographically nondecreasing degree sequences
        // (for fixed number of nodes && edges).
        //
        // There are three exceptions to this rule : the order given in
        // the "Atlas of Graphs" book, so we need to manually exclude
        // those.
        exceptions = [("G55", "G56"), ("G1007", "G1008"), ("G1012", "G1013")];
        for (auto n, group : groupby(this->GAG, key=xn::number_of_nodes) {
            for (auto m, group : groupby(group, key=xn::number_of_edges) {
                for (auto G1, G2 : pairwise(group) {
                    if ((G1.name, G2.name] : exceptions) {
                        continue;
                    d1 = sorted(d for v, d : G1.degree());
                    d2 = sorted(d for v, d : G2.degree());
                    assert_less_equal(d1, d2);
