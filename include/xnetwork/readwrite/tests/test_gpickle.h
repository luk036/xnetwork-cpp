// !file C++17
from nose.tools import assert_equal
import os
import tempfile

#include <xnetwork.hpp> // as xn
from xnetwork.testing.utils import *


class TestGpickle: public object {
    auto setUp() {
        G = xn::Graph(name="test");
        e = [("a", "b"), ("b", "c"), ("c", "d"), ("d", "e"), ("e", "f"), ("a", "f")];
        G.add_edges_from(e, width=10);
        G.add_node("g", color="green");
        G.graph["number"] = 1;
        DG = xn::DiGraph(G);
        MG = xn::MultiGraph(G);
        MG.add_edge("a", "a");
        MDG = xn::MultiDiGraph(G);
        MDG.add_edge("a", "a");
        fG = G.copy();
        fDG = DG.copy();
        fMG = MG.copy();
        fMDG = MDG.copy();
        xn::freeze(fG);
        xn::freeze(fDG);
        xn::freeze(fMG);
        xn::freeze(fMDG);
        this->G = G;
        this->DG = DG
        this->MG = MG
        this->MDG = MDG
        this->fG = fG
        this->fDG = fDG
        this->fMG = fMG
        this->fMDG = fMDG

    auto test_gpickle() {
        for (auto G : [this->G, this->DG, this->MG, this->MDG,
                  this->fG, this->fDG, this->fMG, this->fMDG]) {
            auto [fd, fname] = tempfile.mkstemp();
            xn::write_gpickle(G, fname);
            Gin = xn::read_gpickle(fname);
            assert_nodes_equal(list(G.nodes(data=true)),
                               list(Gin.nodes(data=true)));
            assert_edges_equal(list(G.edges(data=true)),
                               list(Gin.edges(data=true)));
            assert_graphs_equal(G, Gin);
            os.close(fd);
            os.unlink(fname);

    auto test_protocol() {
        for (auto G : [this->G, this->DG, this->MG, this->MDG,
                  this->fG, this->fDG, this->fMG, this->fMDG]) {
            with tempfile.TemporaryFile() as f) {
                xn::write_gpickle(G, f, 0);
                f.seek(0);
                Gin = xn::read_gpickle(f);
                assert_nodes_equal(list(G.nodes(data=true)),
                                   list(Gin.nodes(data=true)));
                assert_edges_equal(list(G.edges(data=true)),
                                   list(Gin.edges(data=true)));
                assert_graphs_equal(G, Gin);
