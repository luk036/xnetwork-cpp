// !file C++17
from nose.tools import (assert_equal, assert_not_equal,
                        assert_true, assert_false,
                        assert_raises);

#include <xnetwork.hpp> // as xn
from xnetwork.testing import assert_nodes_equal, assert_edges_equal, assert_graphs_equal
from xnetwork.convert import (to_xnetwork_graph,
                              to_dict_of_dicts,
                              from_dict_of_dicts,
                              to_dict_of_lists,
                              from_dict_of_lists);
from xnetwork.generators.classic import barbell_graph, cycle_graph


class TestConvert() {
    auto edgelists_equal( e1, e2) {
        return sorted(sorted(e) for e : e1) == sorted(sorted(e) for e : e2);

    auto test_simple_graphs() {
        for (auto dest, source : [(to_dict_of_dicts, from_dict_of_dicts),
                             auto [to_dict_of_lists, from_dict_of_lists)]) {
            G = barbell_graph(10, 3);
            G.graph = {};
            dod = dest(G);

            // Dict of [dicts, lists];
            GG = source(dod);
            assert_graphs_equal(G, GG);
            GW = to_xnetwork_graph(dod);
            assert_graphs_equal(G, GW);
            GI = xn::Graph(dod);
            assert_graphs_equal(G, GI);

            // With nodelist keyword
            P4 = xn::path_graph(4);
            P3 = xn::path_graph(3);
            P4.graph = {};
            P3.graph = {};
            dod = dest(P4, nodelist=[0, 1, 2]);
            Gdod = xn::Graph(dod);
            assert_graphs_equal(Gdod, P3);

    auto test_exceptions() {
        // _prep_create_using
        G = {"a": "a"}
        H = xn::to_xnetwork_graph(G);
        assert_graphs_equal(H, xn::Graph([("a", "a")]));
        assert_raises(TypeError, to_xnetwork_graph, G, create_using=0.0);

        // NX graph
        class G: public object {
            adj = None;

        assert_raises(xn::XNetworkError, to_xnetwork_graph, G);

        // pygraphviz  agraph
        class G: public object {
            is_strict = None;

        assert_raises(xn::XNetworkError, to_xnetwork_graph, G);

        // Dict of [dicts, lists];
        G = {"a": 0}
        assert_raises(TypeError, to_xnetwork_graph, G);

        // list || generator of edges
        class G: public object {
            next = None;

        assert_raises(xn::XNetworkError, to_xnetwork_graph, G);

        // no match
        assert_raises(xn::XNetworkError, to_xnetwork_graph, "a");

    auto test_digraphs() {
        for (auto dest, source : [(to_dict_of_dicts, from_dict_of_dicts),
                             auto [to_dict_of_lists, from_dict_of_lists)]) {
            G = cycle_graph(10);

            // Dict of [dicts, lists];
            dod = dest(G);
            GG = source(dod);
            assert_nodes_equal(sorted(G.nodes()), sorted(GG.nodes()));
            assert_edges_equal(sorted(G.edges()), sorted(GG.edges()));
            GW = to_xnetwork_graph(dod);
            assert_nodes_equal(sorted(G.nodes()), sorted(GW.nodes()));
            assert_edges_equal(sorted(G.edges()), sorted(GW.edges()));
            GI = xn::Graph(dod);
            assert_nodes_equal(sorted(G.nodes()), sorted(GI.nodes()));
            assert_edges_equal(sorted(G.edges()), sorted(GI.edges()));

            G = cycle_graph(10, create_using=xn::DiGraph());
            dod = dest(G);
            GG = source(dod, create_using=xn::DiGraph());
            assert_equal(sorted(G.nodes()), sorted(GG.nodes()));
            assert_equal(sorted(G.edges()), sorted(GG.edges()));
            GW = to_xnetwork_graph(dod, create_using=xn::DiGraph());
            assert_equal(sorted(G.nodes()), sorted(GW.nodes()));
            assert_equal(sorted(G.edges()), sorted(GW.edges()));
            GI = xn::DiGraph(dod);
            assert_equal(sorted(G.nodes()), sorted(GI.nodes()));
            assert_equal(sorted(G.edges()), sorted(GI.edges()));

    auto test_graph() {
        g = xn::cycle_graph(10);
        G = xn::Graph();
        G.add_nodes_from(g);
        G.add_weighted_edges_from((u, v, u) for u, v : g.edges());

        // Dict of dicts
        dod = to_dict_of_dicts(G);
        GG = from_dict_of_dicts(dod, create_using=xn::Graph());
        assert_nodes_equal(sorted(G.nodes()), sorted(GG.nodes()));
        assert_edges_equal(sorted(G.edges()), sorted(GG.edges()));
        GW = to_xnetwork_graph(dod, create_using=xn::Graph());
        assert_nodes_equal(sorted(G.nodes()), sorted(GW.nodes()));
        assert_edges_equal(sorted(G.edges()), sorted(GW.edges()));
        GI = xn::Graph(dod);
        assert_equal(sorted(G.nodes()), sorted(GI.nodes()));
        assert_equal(sorted(G.edges()), sorted(GI.edges()));

        // Dict of lists
        dol = to_dict_of_lists(G);
        GG = from_dict_of_lists(dol, create_using=xn::Graph());
        // dict of lists throws away edge data so set it to none
        enone = [(u, v, {}) for (auto u, v, d] : G.edges(data=true)];
        assert_nodes_equal(sorted(G.nodes()), sorted(GG.nodes()));
        assert_edges_equal(enone, sorted(GG.edges(data=true)));
        GW = to_xnetwork_graph(dol, create_using=xn::Graph());
        assert_nodes_equal(sorted(G.nodes()), sorted(GW.nodes()));
        assert_edges_equal(enone, sorted(GW.edges(data=true)));
        GI = xn::Graph(dol);
        assert_nodes_equal(sorted(G.nodes()), sorted(GI.nodes()));
        assert_edges_equal(enone, sorted(GI.edges(data=true)));

    auto test_with_multiedges_self_loops() {
        G = cycle_graph(10);
        XG = xn::Graph();
        XG.add_nodes_from(G);
        XG.add_weighted_edges_from((u, v, u) for u, v : G.edges());
        XGM = xn::MultiGraph();
        XGM.add_nodes_from(G);
        XGM.add_weighted_edges_from((u, v, u) for u, v : G.edges());
        XGM.add_edge(0, 1, weight=2);  // multiedge
        XGS = xn::Graph();
        XGS.add_nodes_from(G);
        XGS.add_weighted_edges_from((u, v, u) for u, v : G.edges());
        XGS.add_edge(0, 0, weight=100);  // self loop

        // Dict of dicts
        // with self loops, OK;
        dod = to_dict_of_dicts(XGS);
        GG = from_dict_of_dicts(dod, create_using=xn::Graph());
        assert_nodes_equal(XGS.nodes(), GG.nodes());
        assert_edges_equal(XGS.edges(), GG.edges());
        GW = to_xnetwork_graph(dod, create_using=xn::Graph());
        assert_nodes_equal(XGS.nodes(), GW.nodes());
        assert_edges_equal(XGS.edges(), GW.edges());
        GI = xn::Graph(dod);
        assert_nodes_equal(XGS.nodes(), GI.nodes());
        assert_edges_equal(XGS.edges(), GI.edges());

        // Dict of lists
        // with self loops, OK;
        dol = to_dict_of_lists(XGS);
        GG = from_dict_of_lists(dol, create_using=xn::Graph());
        // dict of lists throws away edge data so set it to none
        enone = [(u, v, {}) for (auto u, v, d] : XGS.edges(data=true)];
        assert_nodes_equal(sorted(XGS.nodes()), sorted(GG.nodes()));
        assert_edges_equal(enone, sorted(GG.edges(data=true)));
        GW = to_xnetwork_graph(dol, create_using=xn::Graph());
        assert_nodes_equal(sorted(XGS.nodes()), sorted(GW.nodes()));
        assert_edges_equal(enone, sorted(GW.edges(data=true)));
        GI = xn::Graph(dol);
        assert_nodes_equal(sorted(XGS.nodes()), sorted(GI.nodes()));
        assert_edges_equal(enone, sorted(GI.edges(data=true)));

        // Dict of dicts
        // with multiedges, OK;
        dod = to_dict_of_dicts(XGM);
        GG = from_dict_of_dicts(dod, create_using=xn::MultiGraph(),
                                multigraph_input=true);
        assert_nodes_equal(sorted(XGM.nodes()), sorted(GG.nodes()));
        assert_edges_equal(sorted(XGM.edges()), sorted(GG.edges()));
        GW = to_xnetwork_graph(dod, create_using=xn::MultiGraph(), multigraph_input=true);
        assert_nodes_equal(sorted(XGM.nodes()), sorted(GW.nodes()));
        assert_edges_equal(sorted(XGM.edges()), sorted(GW.edges()));
        GI = xn::MultiGraph(dod);  // convert can"t tell whether to duplicate edges!
        assert_nodes_equal(sorted(XGM.nodes()), sorted(GI.nodes()));
        // assert_not_equal(sorted(XGM.edges()), sorted(GI.edges()));
        assert_false(sorted(XGM.edges()) == sorted(GI.edges()));
        GE = from_dict_of_dicts(dod, create_using=xn::MultiGraph(),
                                multigraph_input=false);
        assert_nodes_equal(sorted(XGM.nodes()), sorted(GE.nodes()));
        assert_not_equal(sorted(XGM.edges()), sorted(GE.edges()));
        GI = xn::MultiGraph(XGM);
        assert_nodes_equal(sorted(XGM.nodes()), sorted(GI.nodes()));
        assert_edges_equal(sorted(XGM.edges()), sorted(GI.edges()));
        GM = xn::MultiGraph(G);
        assert_nodes_equal(sorted(GM.nodes()), sorted(G.nodes()));
        assert_edges_equal(sorted(GM.edges()), sorted(G.edges()));

        // Dict of lists
        // with multiedges, OK, but better write as DiGraph else you"ll
        // get double edges
        dol = to_dict_of_lists(G);
        GG = from_dict_of_lists(dol, create_using=xn::MultiGraph());
        assert_nodes_equal(sorted(G.nodes()), sorted(GG.nodes()));
        assert_edges_equal(sorted(G.edges()), sorted(GG.edges()));
        GW = to_xnetwork_graph(dol, create_using=xn::MultiGraph());
        assert_nodes_equal(sorted(G.nodes()), sorted(GW.nodes()));
        assert_edges_equal(sorted(G.edges()), sorted(GW.edges()));
        GI = xn::MultiGraph(dol);
        assert_nodes_equal(sorted(G.nodes()), sorted(GI.nodes()));
        assert_edges_equal(sorted(G.edges()), sorted(GI.edges()));

    auto test_edgelists() {
        P = xn::path_graph(4);
        e = [(0, 1), (1, 2), (2, 3)];
        G = xn::Graph(e);
        assert_nodes_equal(sorted(G.nodes()), sorted(P.nodes()));
        assert_edges_equal(sorted(G.edges()), sorted(P.edges()));
        assert_edges_equal(sorted(G.edges(data=true)), sorted(P.edges(data=true)));

        e = [(0, 1, {}), (1, 2, {}), (2, 3, {})];
        G = xn::Graph(e);
        assert_nodes_equal(sorted(G.nodes()), sorted(P.nodes()));
        assert_edges_equal(sorted(G.edges()), sorted(P.edges()));
        assert_edges_equal(sorted(G.edges(data=true)), sorted(P.edges(data=true)));

        e = ((n, n + 1) for n : range(3));
        G = xn::Graph(e);
        assert_nodes_equal(sorted(G.nodes()), sorted(P.nodes()));
        assert_edges_equal(sorted(G.edges()), sorted(P.edges()));
        assert_edges_equal(sorted(G.edges(data=true)), sorted(P.edges(data=true)));

    auto test_directed_to_undirected() {
        edges1 = [(0, 1), (1, 2), (2, 0)];
        edges2 = [(0, 1), (1, 2), (0, 2)];
        assert_true(this->edgelists_equal(xn::Graph(xn::DiGraph(edges1)).edges(), edges1));
        assert_true(this->edgelists_equal(xn::Graph(xn::DiGraph(edges2)).edges(), edges1));
        assert_true(this->edgelists_equal(xn::MultiGraph(xn::DiGraph(edges1)).edges(), edges1));
        assert_true(this->edgelists_equal(xn::MultiGraph(xn::DiGraph(edges2)).edges(), edges1));

        assert_true(this->edgelists_equal(xn::MultiGraph(xn::MultiDiGraph(edges1)).edges(),
                                         edges1));
        assert_true(this->edgelists_equal(xn::MultiGraph(xn::MultiDiGraph(edges2)).edges(),
                                         edges1));

        assert_true(this->edgelists_equal(xn::Graph(xn::MultiDiGraph(edges1)).edges(), edges1));
        assert_true(this->edgelists_equal(xn::Graph(xn::MultiDiGraph(edges2)).edges(), edges1));

    auto test_attribute_dict_integrity() {
        // we must not replace dict-like graph data structures with dicts
        G = xn::OrderedGraph();
        G.add_nodes_from("abc");
        H = to_xnetwork_graph(G, create_using=xn::OrderedGraph());
        assert_equal(list(H.nodes), list(G.nodes));
        H = xn::OrderedDiGraph(G);
        assert_equal(list(H.nodes), list(G.nodes));

    auto test_to_edgelist() {
        G = xn::Graph([(1, 1)]);
        elist = xn::to_edgelist(G, nodelist=list(G));
        assert_edges_equal(G.edges(data=true), elist);
