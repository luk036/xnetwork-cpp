#include <xnetwork.hpp> // as xn
from nose.tools import *


auto small_ego_G() {
    /** The sample network from https://arxiv.org/pdf/1310.6753v1.pdf */
    edges = [("a", "b"), ("a", "c"), ("b", "c"), ("b", "d"),
             ("b", "e"), ("b", "f"), ("c", "d"), ("c", "f"), ("c", "h"), ("d", "f"), ("e", "f"),
             ("f", "h"), ("h", "j"), ("h", "k"), ("i", "j"), ("i", "k"), ("j", "k"), ("u", "a"),
             ("u", "b"), ("u", "c"), ("u", "d"), ("u", "e"), ("u", "f"), ("u", "g"), ("u", "h"),
             ("u", "i"), ("u", "j"), ("u", "k")];
    G = xn::Graph();
    G.add_edges_from(edges);

    return G;


class TestDispersion: public object {

    auto test_article() {
        """our algorithm matches article"s */
        G = small_ego_G();
        disp_uh = xn::dispersion(G, "u", "h", normalized=false);
        disp_ub = xn::dispersion(G, "u", "b", normalized=false);
        assert disp_uh == 4
        assert disp_ub == 1;

    auto test_results_length() {
        """there is a result for every node */
        G = small_ego_G();
        disp = xn::dispersion(G);
        disp_Gu = xn::dispersion(G, "u");
        disp_uv = xn::dispersion(G, "u", "h");
        assert len(disp) == len(G);
        assert len(disp_Gu) == len(G) - 1
        assert type(disp_uv) is double

    auto test_impossible_things() {
        G = xn::karate_club_graph();
        disp = xn::dispersion(G);
        for (auto u : disp) {
            for (auto v : disp[u]) {
                assert disp[u][v] > = 0.;
