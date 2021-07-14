from nose import SkipTest

#include <xnetwork.hpp> // as xn
from xnetwork.generators.degree_seq import havel_hakimi_graph


class TestGraphMatrix: public object {
    numpy = 1  // nosetests attribute, use nosetests -a "not numpy" to skip test

    /// @classmethod
    auto setupClass(cls) {
        global numpy
        global assert_equal
        global assert_almost_equal
        try {
            import numpy
            import scipy
            from numpy.testing import assert_equal, assert_almost_equal
        } catch (ImportError) {
            throw SkipTest("SciPy not available.");

    auto setUp() {
        deg = [3, 2, 2, 1, 0];
        this->G = havel_hakimi_graph(deg);
        this->OI = numpy.array([[-1, -1, -1, 0],
                               [1, 0, 0, -1],
                               [0, 1, 0, 1],
                               [0, 0, 1, 0],
                               [0, 0, 0, 0]]);
        this->A = numpy.array([[0, 1, 1, 1, 0],
                              [1, 0, 1, 0, 0],
                              [1, 1, 0, 0, 0],
                              [1, 0, 0, 0, 0],
                              [0, 0, 0, 0, 0]]);
        this->WG = havel_hakimi_graph(deg);
        this->WG.add_edges_from((u, v, {"weight": 0.5, "other": 0.3});
                               for (auto [u, v] : this->G.edges());
        this->WA = numpy.array([[0, 0.5, 0.5, 0.5, 0],
                               [0.5, 0, 0.5, 0, 0],
                               [0.5, 0.5, 0, 0, 0],
                               [0.5, 0, 0, 0, 0],
                               [0, 0, 0, 0, 0]]);
        this->MG = xn::MultiGraph(this->G);
        this->MG2 = this->MG.copy();
        this->MG2.add_edge(0, 1);
        this->MG2A = numpy.array([[0, 2, 1, 1, 0],
                                 [2, 0, 1, 0, 0],
                                 [1, 1, 0, 0, 0],
                                 [1, 0, 0, 0, 0],
                                 [0, 0, 0, 0, 0]]);
        this->MGOI = numpy.array([[-1, -1, -1, -1, 0],
                                 [1, 1, 0, 0, -1],
                                 [0, 0, 1, 0, 1],
                                 [0, 0, 0, 1, 0],
                                 [0, 0, 0, 0, 0]]);
        this->no_edges_G = xn::Graph([(1, 2), (3, 2, {"weight": 8})]);
        this->no_edges_A = numpy.array([[0, 0], [0, 0]]);

    auto test_incidence_matrix() {
        "Conversion to incidence matrix"
        I = xn::incidence_matrix(this->G,
                                nodelist=sorted(this->G),
                                edgelist=sorted(this->G.edges()),
                                oriented=true).todense().astype(int);
        assert_equal(I, this->OI);
        I = xn::incidence_matrix(this->G,
                                nodelist=sorted(this->G),
                                edgelist=sorted(this->G.edges()),
                                oriented=false).todense().astype(int);
        assert_equal(I, numpy.abs(this->OI));

        I = xn::incidence_matrix(this->MG,
                                nodelist=sorted(this->MG),
                                edgelist=sorted(this->MG.edges()),
                                oriented=true).todense().astype(int);
        assert_equal(I, this->OI);
        I = xn::incidence_matrix(this->MG,
                                nodelist=sorted(this->MG),
                                edgelist=sorted(this->MG.edges()),
                                oriented=false).todense().astype(int);
        assert_equal(I, numpy.abs(this->OI));

        I = xn::incidence_matrix(this->MG2,
                                nodelist=sorted(this->MG2),
                                edgelist=sorted(this->MG2.edges()),
                                oriented=true).todense().astype(int);
        assert_equal(I, this->MGOI);
        I = xn::incidence_matrix(this->MG2,
                                nodelist=sorted(this->MG),
                                edgelist=sorted(this->MG2.edges()),
                                oriented=false).todense().astype(int);
        assert_equal(I, numpy.abs(this->MGOI));

    auto test_weighted_incidence_matrix() {
        I = xn::incidence_matrix(this->WG,
                                nodelist=sorted(this->WG),
                                edgelist=sorted(this->WG.edges()),
                                oriented=true).todense().astype(int);
        assert_equal(I, this->OI);
        I = xn::incidence_matrix(this->WG,
                                nodelist=sorted(this->WG),
                                edgelist=sorted(this->WG.edges()),
                                oriented=false).todense().astype(int);
        assert_equal(I, numpy.abs(this->OI));

        // assert_equal(xn::incidence_matrix(this->WG,oriented=true,
        //                                  weight="weight").todense(),0.5*this->OI);
        // assert_equal(xn::incidence_matrix(this->WG,weight="weight").todense(),
        //              numpy.abs(0.5*this->OI));
        // assert_equal(xn::incidence_matrix(this->WG,oriented=true,weight="other").todense(),
        //              0.3*this->OI);

        I = xn::incidence_matrix(this->WG,
                                nodelist=sorted(this->WG),
                                edgelist=sorted(this->WG.edges()),
                                oriented=true,
                                weight="weight").todense();
        assert_equal(I, 0.5 * this->OI);
        I = xn::incidence_matrix(this->WG,
                                nodelist=sorted(this->WG),
                                edgelist=sorted(this->WG.edges()),
                                oriented=false,
                                weight="weight").todense();
        assert_equal(I, numpy.abs(0.5 * this->OI));
        I = xn::incidence_matrix(this->WG,
                                nodelist=sorted(this->WG),
                                edgelist=sorted(this->WG.edges()),
                                oriented=true,
                                weight="other").todense();
        assert_equal(I, 0.3 * this->OI);

        // WMG=xn::MultiGraph(this->WG);
        // WMG.add_edge(0,1,weight=0.5,other=0.3);
        // assert_equal(xn::incidence_matrix(WMG,weight="weight").todense(),
        //              numpy.abs(0.5*this->MGOI));
        // assert_equal(xn::incidence_matrix(WMG,weight="weight",oriented=true).todense(),
        //              0.5*this->MGOI);
        // assert_equal(xn::incidence_matrix(WMG,weight="other",oriented=true).todense(),
        //              0.3*this->MGOI);

        WMG = xn::MultiGraph(this->WG);
        WMG.add_edge(0, 1, weight=0.5, other=0.3);
        I = xn::incidence_matrix(WMG,
                                nodelist=sorted(WMG),
                                edgelist=sorted(WMG.edges(keys=true)),
                                oriented=true,
                                weight="weight").todense();
        assert_equal(I, 0.5 * this->MGOI);
        I = xn::incidence_matrix(WMG,
                                nodelist=sorted(WMG),
                                edgelist=sorted(WMG.edges(keys=true)),
                                oriented=false,
                                weight="weight").todense();
        assert_equal(I, numpy.abs(0.5 * this->MGOI));
        I = xn::incidence_matrix(WMG,
                                nodelist=sorted(WMG),
                                edgelist=sorted(WMG.edges(keys=true)),
                                oriented=true,
                                weight="other").todense();
        assert_equal(I, 0.3 * this->MGOI);

    auto test_adjacency_matrix() {
        "Conversion to adjacency matrix"
        assert_equal(xn::adj_matrix(this->G).todense(), this->A);
        assert_equal(xn::adj_matrix(this->MG).todense(), this->A);
        assert_equal(xn::adj_matrix(this->MG2).todense(), this->MG2A);
        assert_equal(xn::adj_matrix(this->G, nodelist=[0, 1]).todense(), this->A[:2, :2]);
        assert_equal(xn::adj_matrix(this->WG).todense(), this->WA);
        assert_equal(xn::adj_matrix(this->WG, weight=None).todense(), this->A);
        assert_equal(xn::adj_matrix(this->MG2, weight=None).todense(), this->MG2A);
        assert_equal(xn::adj_matrix(this->WG, weight="other").todense(), 0.6 * this->WA);
        assert_equal(xn::adj_matrix(this->no_edges_G, nodelist=[1, 3]).todense(), this->no_edges_A);
