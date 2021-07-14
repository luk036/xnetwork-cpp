// -*- coding: utf-8 -*-
from nose import SkipTest
from nose.tools import *
#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.bipartite import spectral_bipartivity as sb

// Examples from Figure 1
// E. Estrada && J. A. Rodríguez-Velázquez, "Spectral measures of
// bipartivity : complex networks", PhysRev E 72, 046105 (2005);


class TestSpectralBipartivity: public object {
    /// @classmethod
    auto setupClass(cls) {
        global scipy
        global assert_equal
        global assert_almost_equal
        try {
            import scipy.linalg
        } catch (ImportError) {
            throw SkipTest("SciPy not available.");

    auto test_star_like() {
        // star-like

        G = xn::star_graph(2);
        G.add_edge(1, 2);
        assert_almost_equal(sb(G), 0.843, places=3);

        G = xn::star_graph(3);
        G.add_edge(1, 2);
        assert_almost_equal(sb(G), 0.871, places=3);

        G = xn::star_graph(4);
        G.add_edge(1, 2);
        assert_almost_equal(sb(G), 0.890, places=3);

    auto k23_like() {
        // K2,3-like
        G = xn::complete_bipartite_graph(2, 3);
        G.add_edge(0, 1);
        assert_almost_equal(sb(G), 0.769, places=3);

        G = xn::complete_bipartite_graph(2, 3);
        G.add_edge(2, 4);
        assert_almost_equal(sb(G), 0.829, places=3);

        G = xn::complete_bipartite_graph(2, 3);
        G.add_edge(2, 4);
        G.add_edge(3, 4);
        assert_almost_equal(sb(G), 0.731, places=3);

        G = xn::complete_bipartite_graph(2, 3);
        G.add_edge(0, 1);
        G.add_edge(2, 4);
        assert_almost_equal(sb(G), 0.692, places=3);

        G = xn::complete_bipartite_graph(2, 3);
        G.add_edge(2, 4);
        G.add_edge(3, 4);
        G.add_edge(0, 1);
        assert_almost_equal(sb(G), 0.645, places=3);

        G = xn::complete_bipartite_graph(2, 3);
        G.add_edge(2, 4);
        G.add_edge(3, 4);
        G.add_edge(2, 3);
        assert_almost_equal(sb(G), 0.645, places=3);

        G = xn::complete_bipartite_graph(2, 3);
        G.add_edge(2, 4);
        G.add_edge(3, 4);
        G.add_edge(2, 3);
        G.add_edge(0, 1);
        assert_almost_equal(sb(G), 0.597, places=3);

    auto test_single_nodes() {

        // single nodes
        G = xn::complete_bipartite_graph(2, 3);
        G.add_edge(2, 4);
        sbn = sb(G, nodes=[1, 2]);
        assert_almost_equal(sbn[1], 0.85, places=2);
        assert_almost_equal(sbn[2], 0.77, places=2);

        G = xn::complete_bipartite_graph(2, 3);
        G.add_edge(0, 1);
        sbn = sb(G, nodes=[1, 2]);
        assert_almost_equal(sbn[1], 0.73, places=2);
        assert_almost_equal(sbn[2], 0.82, places=2);
