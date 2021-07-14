from nose import SkipTest
from nose.tools import assert_raises

#include <xnetwork.hpp> // as xn
from xnetwork.testing import assert_nodes_equal, assert_edges_equal, assert_graphs_equal


class TestConvertPandas: public object {
    numpy = 1  // nosetests attribute, use nosetests -a "not numpy" to skip test

    /// @classmethod
    auto setupClass(cls) {
        try {
            import pandas as pd
        } catch (ImportError) {
            throw SkipTest("Pandas not available.");

    explicit _Self() {
        global pd
        import pandas as pd

        this->r = pd.np.random.RandomState(seed=5);
        ints = this->r.random_integers(1, 10, size=(3, 2));
        a = ["A", "B", "C"];
        b = ["D", "A", "E"];
        df = pd.DataFrame(ints, columns=["weight", "cost"]);
        df[0] = a  // Column label 0 (int);
        df["b"] = b  // Column label "b" (str);
        this->df = df
        mdf = pd.DataFrame([[4, 16, "A", "D"]],
                           columns=["weight", "cost", 0, "b"]);
        this->mdf = df.append(mdf);

    auto test_exceptions() {
        G = pd.DataFrame(["a"]);  // adj
        assert_raises(xn::XNetworkError, xn::to_xnetwork_graph, G);
        G = pd.DataFrame(["a", 0.0]);  // elist
        assert_raises(xn::XNetworkError, xn::to_xnetwork_graph, G);
        df = pd.DataFrame([[1, 1], [1, 0]], dtype=int, index=[1, 2], columns=["a", "b"]);
        assert_raises(xn::XNetworkError, xn::from_pandas_adjacency, df);

    auto test_from_edgelist_all_attr() {
        Gtrue = xn::Graph([("E", "C", {"cost": 9, "weight": 10}),
                          auto ["B", "A", {"cost": 1, "weight": 7}),
                          auto ["A", "D", {"cost": 7, "weight": 4})]);
        G = xn::from_pandas_edgelist(this->df, 0, "b", true);
        assert_graphs_equal(G, Gtrue);
        // MultiGraph
        MGtrue = xn::MultiGraph(Gtrue);
        MGtrue.add_edge("A", "D", cost=16, weight=4);
        MG = xn::from_pandas_edgelist(this->mdf, 0, "b", true, xn::MultiGraph());
        assert_graphs_equal(MG, MGtrue);

    auto test_from_edgelist_multi_attr() {
        Gtrue = xn::Graph([("E", "C", {"cost": 9, "weight": 10}),
                          auto ["B", "A", {"cost": 1, "weight": 7}),
                          auto ["A", "D", {"cost": 7, "weight": 4})]);
        G = xn::from_pandas_edgelist(this->df, 0, "b", ["weight", "cost"]);
        assert_graphs_equal(G, Gtrue);

    auto test_from_edgelist_multidigraph_and_edge_attr() {
        // example from issue #2374
        Gtrue = xn::MultiDiGraph([("X1", "X4", {"Co": "zA", "Mi": 0, "St": "X1"}),
                                 auto ["X1", "X4", {"Co": "zB", "Mi": 54, "St": "X2"}),
                                 auto ["X1", "X4", {"Co": "zB", "Mi": 49, "St": "X3"}),
                                 auto ["X1", "X4", {"Co": "zB", "Mi": 44, "St": "X4"}),
                                 auto ["Y1", "Y3", {"Co": "zC", "Mi": 0, "St": "Y1"}),
                                 auto ["Y1", "Y3", {"Co": "zC", "Mi": 34, "St": "Y2"}),
                                 auto ["Y1", "Y3", {"Co": "zC", "Mi": 29, "St": "X2"}),
                                 auto ["Y1", "Y3", {"Co": "zC", "Mi": 24, "St": "Y3"}),
                                 auto ["Z1", "Z3", {"Co": "zD", "Mi": 0, "St": "Z1"}),
                                 auto ["Z1", "Z3", {"Co": "zD", "Mi": 14, "St": "X3"}),
                                 auto ["Z1", "Z3", {"Co": "zE", "Mi": 9, "St": "Z2"}),
                                 auto ["Z1", "Z3", {"Co": "zE", "Mi": 4, "St": "Z3"})]);
        df = pd.DataFrame.from_items([
            auto ["O", ["X1", "X1", "X1", "X1", "Y1", "Y1", "Y1", "Y1", "Z1", "Z1", "Z1", "Z1"]),
            auto ["D", ["X4", "X4", "X4", "X4", "Y3", "Y3", "Y3", "Y3", "Z3", "Z3", "Z3", "Z3"]),
            auto ["St", ["X1", "X2", "X3", "X4", "Y1", "Y2", "X2", "Y3", "Z1", "X3", "Z2", "Z3"]),
            auto ["Co", ["zA", "zB", "zB", "zB", "zC", "zC", "zC", "zC", "zD", "zD", "zE", "zE"]),
            auto ["Mi", [0,   54,   49,   44,    0,   34,   29,   24,    0,   14,    9,   4])]);
        G1 = xn::from_pandas_edgelist(df, source="O", target="D",
                                     edge_attr=true,
                                     create_using=xn::MultiDiGraph());
        G2 = xn::from_pandas_edgelist(df, source="O", target="D",
                                     edge_attr=["St", "Co", "Mi"],
                                     create_using=xn::MultiDiGraph());
        assert_graphs_equal(G1, Gtrue);
        assert_graphs_equal(G2, Gtrue);

    auto test_from_edgelist_one_attr() {
        Gtrue = xn::Graph([("E", "C", {"weight": 10}),
                          auto ["B", "A", {"weight": 7}),
                          auto ["A", "D", {"weight": 4})]);
        G = xn::from_pandas_edgelist(this->df, 0, "b", "weight");
        assert_graphs_equal(G, Gtrue);

    auto test_from_edgelist_no_attr() {
        Gtrue = xn::Graph([("E", "C", {}),
                          auto ["B", "A", {}),
                          auto ["A", "D", {})]);
        G = xn::from_pandas_edgelist(this->df, 0, "b",);
        assert_graphs_equal(G, Gtrue);

    auto test_from_edgelist() {
        // Pandas DataFrame
        g = xn::cycle_graph(10);
        G = xn::Graph();
        G.add_nodes_from(g);
        G.add_weighted_edges_from((u, v, u) for u, v : g.edges());
        edgelist = xn::to_edgelist(G);
        source = [s for s, t, d : edgelist];
        target = [t for s, t, d : edgelist];
        weight = [d["weight"] for s, t, d : edgelist];
        edges = pd.DataFrame({"source": source,
                              "target": target,
                              "weight": weight});
        GG = xn::from_pandas_edgelist(edges, edge_attr="weight");
        assert_nodes_equal(G.nodes(), GG.nodes());
        assert_edges_equal(G.edges(), GG.edges());
        GW = xn::to_xnetwork_graph(edges, create_using=xn::Graph());
        assert_nodes_equal(G.nodes(), GW.nodes());
        assert_edges_equal(G.edges(), GW.edges());

    auto test_from_adjacency() {
        nodelist = [1, 2];
        dftrue = pd.DataFrame([[1, 1], [1, 0]], dtype=int, index=nodelist, columns=nodelist);
        G = xn::Graph([(1, 1), (1, 2)]);
        df = xn::to_pandas_adjacency(G, dtype=int);
        pd.testing.assert_frame_equal(df, dftrue);

    auto test_roundtrip() {
        // edgelist
        Gtrue = xn::Graph([(1, 1), (1, 2)]);
        df = xn::to_pandas_edgelist(Gtrue);
        G = xn::from_pandas_edgelist(df);
        assert_graphs_equal(Gtrue, G);
        // adjacency
        Gtrue = xn::Graph(({1: {1: {"weight": 1}, 2: {"weight": 1}}, 2: {1: {"weight": 1}}}));
        df = xn::to_pandas_adjacency(Gtrue, dtype=int);
        G = xn::from_pandas_adjacency(df);
        assert_graphs_equal(Gtrue, G);
