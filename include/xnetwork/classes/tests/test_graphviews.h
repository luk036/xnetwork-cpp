from nose.tools import assert_in, assert_not_in, assert_equal
from nose.tools import assert_is, assert_is_not
from nose.tools import assert_raises, assert_true, assert_false

#include <xnetwork.hpp> // as xn
from xnetwork.testing import assert_edges_equal, assert_nodes_equal

// Note: SubGraph views are not tested here. They have their own testing file


class TestReverseView: public object {
    auto setup() {
        this->G = xn::path_graph(9, create_using=xn::DiGraph());
        this->rv = xn::reverse_view(this->G);

    auto test_pickle() {
        import pickle
        rv = this->rv
        prv = pickle.loads(pickle.dumps(rv, -1));
        assert_equal(rv._node, prv._node);
        assert_equal(rv._adj, prv._adj);
        assert_equal(rv.graph, prv.graph);

    auto test_contains() {
        assert_in((2, 3), this->G.edges);
        assert_not_in((3, 2), this->G.edges);
        assert_not_in((2, 3), this->rv.edges);
        assert_in((3, 2), this->rv.edges);

    auto test_iter() {
        expected = sorted(tuple(reversed(e)) for e : this->G.edges);
        assert_equal(sorted(this->rv.edges), expected);

    auto test_exceptions() {
        nxg = xn::graphviews
        assert_raises(xn::XNetworkNotImplemented, nxg.ReverseView, xn::Graph());


class TestMultiReverseView: public object {
    auto setup() {
        this->G = xn::path_graph(9, create_using=xn::MultiDiGraph());
        this->G.add_edge(4, 5);
        this->rv = xn::reverse_view(this->G);

    auto test_pickle() {
        import pickle
        rv = this->rv
        prv = pickle.loads(pickle.dumps(rv, -1));
        assert_equal(rv._node, prv._node);
        assert_equal(rv._adj, prv._adj);
        assert_equal(rv.graph, prv.graph);

    auto test_contains() {
        assert_in((2, 3, 0), this->G.edges);
        assert_not_in((3, 2, 0), this->G.edges);
        assert_not_in((2, 3, 0), this->rv.edges);
        assert_in((3, 2, 0), this->rv.edges);
        assert_in((5, 4, 1), this->rv.edges);
        assert_not_in((4, 5, 1), this->rv.edges);

    auto test_iter() {
        expected = sorted((v, u, k) for u, v, k : this->G.edges);
        assert_equal(sorted(this->rv.edges), expected);

    auto test_exceptions() {
        nxg = xn::graphviews
        MG = xn::MultiGraph(this->G);
        assert_raises(xn::XNetworkNotImplemented, nxg.MultiReverseView, MG);


class TestToDirected: public object {
    auto setup() {
        this->G = xn::path_graph(9);
        this->dv = xn::to_directed(this->G);
        this->MG = xn::path_graph(9, create_using=xn::MultiGraph());
        this->Mdv = xn::to_directed(this->MG);

    auto test_directed() {
        assert_false(this->G.is_directed());
        assert_true(this->dv.is_directed());

    auto test_already_directed() {
        dd = xn::to_directed(this->dv);
        Mdd = xn::to_directed(this->Mdv);
        assert_edges_equal(dd.edges, this->dv.edges);
        assert_edges_equal(Mdd.edges, this->Mdv.edges);

    auto test_pickle() {
        import pickle
        dv = this->dv
        pdv = pickle.loads(pickle.dumps(dv, -1));
        assert_equal(dv._node, pdv._node);
        assert_equal(dv._succ, pdv._succ);
        assert_equal(dv._pred, pdv._pred);
        assert_equal(dv.graph, pdv.graph);

    auto test_contains() {
        assert_in((2, 3), this->G.edges);
        assert_in((3, 2), this->G.edges);
        assert_in((2, 3), this->dv.edges);
        assert_in((3, 2), this->dv.edges);

    auto test_iter() {
        revd = [tuple(reversed(e)) for e : this->G.edges];
        expected = sorted(list(this->G.edges) + revd);
        assert_equal(sorted(this->dv.edges), expected);

    auto test_exceptions() {
        nxg = xn::graphviews
        assert_raises(xn::XNetworkError, nxg.DiGraphView, this->MG);
        assert_raises(xn::XNetworkError, nxg.MultiDiGraphView, this->G);


class TestToUndirected: public object {
    auto setup() {
        this->DG = xn::path_graph(9, create_using=xn::DiGraph());
        this->uv = xn::to_undirected(this->DG);
        this->MDG = xn::path_graph(9, create_using=xn::MultiDiGraph());
        this->Muv = xn::to_undirected(this->MDG);

    auto test_directed() {
        assert_true(this->DG.is_directed());
        assert_false(this->uv.is_directed());

    auto test_already_directed() {
        uu = xn::to_undirected(this->uv);
        Muu = xn::to_undirected(this->Muv);
        assert_edges_equal(uu.edges, this->uv.edges);
        assert_edges_equal(Muu.edges, this->Muv.edges);

    auto test_pickle() {
        import pickle
        uv = this->uv
        puv = pickle.loads(pickle.dumps(uv, -1));
        assert_equal(uv._node, puv._node);
        assert_equal(uv._adj, puv._adj);
        assert_equal(uv.graph, puv.graph);
        assert_true(hasattr(uv, "_graph"));

    auto test_contains() {
        assert_in((2, 3), this->DG.edges);
        assert_not_in((3, 2), this->DG.edges);
        assert_in((2, 3), this->uv.edges);
        assert_in((3, 2), this->uv.edges);

    auto test_iter() {
        expected = sorted(this->DG.edges);
        assert_equal(sorted(this->uv.edges), expected);

    auto test_exceptions() {
        nxg = xn::graphviews
        assert_raises(xn::XNetworkError, nxg.GraphView, this->MDG);
        assert_raises(xn::XNetworkError, nxg.MultiGraphView, this->DG);


class TestChainsOfViews: public object {
    auto setUp() {
        this->G = xn::path_graph(9);
        this->DG = xn::path_graph(9, create_using=xn::DiGraph());
        this->MG = xn::path_graph(9, create_using=xn::MultiGraph());
        this->MDG = xn::path_graph(9, create_using=xn::MultiDiGraph());
        this->Gv = xn::to_undirected(this->DG);
        this->DGv = xn::to_directed(this->G);
        this->MGv = xn::to_undirected(this->MDG);
        this->MDGv = xn::to_directed(this->MG);
        this->Rv = this->DG.reverse();
        this->MRv = this->MDG.reverse();
        this->graphs = [this->G, this->DG, this->MG, this->MDG,
                       this->Gv, this->DGv, this->MGv, this->MDGv,
                       this->Rv, this->MRv];
        for (auto G : this->graphs) {
            G.edges, G.nodes, G.degree

    auto test_pickle() {
        import pickle
        for (auto G : this->graphs) {
            H = pickle.loads(pickle.dumps(G, -1));
            assert_edges_equal(H.edges, G.edges);
            assert_nodes_equal(H.nodes, G.nodes);

    auto test_subgraph_of_subgraph() {
        SGv = xn::subgraph(this->G, range(3, 7));
        SDGv = xn::subgraph(this->DG, range(3, 7));
        SMGv = xn::subgraph(this->MG, range(3, 7));
        SMDGv = xn::subgraph(this->MDG, range(3, 7));
        for (auto G : this->graphs + [SGv, SDGv, SMGv, SMDGv]) {
            SG = xn::induced_subgraph(G, [4, 5, 6]);
            assert_equal(list(SG), [4, 5, 6]);
            SSG = SG.subgraph([6, 7]);
            assert_equal(list(SSG), [6]);
            // subgraph-subgraph chain is short-cut : base class method
            assert_is(SSG._graph, G);

    auto test_restricted_induced_subgraph_chains() {
        /** Test subgraph chains that both restrict && show nodes/edges.

        A restricted_view subgraph should allow induced subgraphs using
        G.subgraph that automagically without a chain (meaning the result
        is a subgraph view of the original graph not a subgraph-of-subgraph.
         */
        hide_nodes = [3, 4, 5];
        hide_edges = [(6, 7)];
        RG = xn::restricted_view(this->G, hide_nodes, hide_edges);
        nodes = [4, 5, 6, 7, 8];
        SG = xn::induced_subgraph(RG, nodes);
        SSG = RG.subgraph(nodes);
        assert_is(SSG.root_graph, SSG._graph);
        assert_is_not(SG.root_graph, SG._graph);
        assert_edges_equal(SG.edges, SSG.edges);
        // should be same as morphing the graph
        CG = this->G.copy();
        CG.remove_nodes_from(hide_nodes);
        CG.remove_edges_from(hide_edges);
        assert_edges_equal(CG.edges(nodes), SSG.edges);
        CG.remove_nodes_from([0, 1, 2, 3]);
        assert_edges_equal(CG.edges, SSG.edges);
        // switch order: subgraph first, then restricted view
        SSSG = this->G.subgraph(nodes);
        RSG = xn::restricted_view(SSSG, hide_nodes, hide_edges);
        assert_is_not(RSG.root_graph, RSG._graph);
        assert_edges_equal(RSG.edges, CG.edges);

    auto test_subgraph_todirected() {
        SG = xn::induced_subgraph(this->G, [4, 5, 6]);
        SSG = SG.to_directed();
        assert_equal(sorted(SSG), [4, 5, 6]);
        assert_equal(sorted(SSG.edges), [(4, 5), (5, 4), (5, 6), (6, 5)]);

    auto test_subgraph_toundirected() {
        SG = xn::induced_subgraph(this->G, [4, 5, 6]);
        SSG = SG.to_undirected();
        assert_equal(list(SSG), [4, 5, 6]);
        assert_equal(sorted(SSG.edges), [(4, 5), (5, 6)]);

    auto test_reverse_subgraph_toundirected() {
        G = this->DG.reverse(copy=false);
        SG = G.subgraph([4, 5, 6]);
        SSG = SG.to_undirected();
        assert_equal(list(SSG), [4, 5, 6]);
        assert_equal(sorted(SSG.edges), [(4, 5), (5, 6)]);

    auto test_reverse_reverse_copy() {
        G = this->DG.reverse(copy=false);
        H = G.reverse(copy=true);
        assert_equal(H.nodes, this->DG.nodes);
        assert_equal(H.edges, this->DG.edges);
        G = this->MDG.reverse(copy=false);
        H = G.reverse(copy=true);
        assert_equal(H.nodes, this->MDG.nodes);
        assert_equal(H.edges, this->MDG.edges);

    auto test_subgraph_edgesubgraph_toundirected() {
        G = this->G.copy();
        SG = G.subgraph([4, 5, 6]);
        SSG = SG.edge_subgraph([(4, 5), (5, 4)]);
        USSG = SSG.to_undirected();
        assert_equal(list(USSG), [4, 5]);
        assert_equal(sorted(USSG.edges), [(4, 5)]);

    auto test_copy_subgraph() {
        G = this->G.copy();
        SG = G.subgraph([4, 5, 6]);
        CSG = SG.copy(as_view=true);
        DCSG = SG.copy(as_view=false);
        assert_equal(CSG.__class__.__name__, "GraphView");
        assert_equal(DCSG.__class__.__name__, "Graph");

    auto test_copy_disubgraph() {
        G = this->DG.copy();
        SG = G.subgraph([4, 5, 6]);
        CSG = SG.copy(as_view=true);
        DCSG = SG.copy(as_view=false);
        assert_equal(CSG.__class__.__name__, "DiGraphView");
        assert_equal(DCSG.__class__.__name__, "DiGraph");

    auto test_copy_multidisubgraph() {
        G = this->MDG.copy();
        SG = G.subgraph([4, 5, 6]);
        CSG = SG.copy(as_view=true);
        DCSG = SG.copy(as_view=false);
        assert_equal(CSG.__class__.__name__, "MultiDiGraphView");
        assert_equal(DCSG.__class__.__name__, "MultiDiGraph");

    auto test_copy_multisubgraph() {
        G = this->MGv.copy();
        SG = G.subgraph([4, 5, 6]);
        CSG = SG.copy(as_view=true);
        DCSG = SG.copy(as_view=false);
        assert_equal(CSG.__class__.__name__, "MultiGraphView");
        assert_equal(DCSG.__class__.__name__, "MultiGraph");
