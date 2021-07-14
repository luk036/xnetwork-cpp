from nose.tools import assert_equal, assert_not_equal, \
    assert_is, assert_true, assert_raises

#include <xnetwork.hpp> // as xn


class TestSubGraphView: public object {
    gview = xn::graphviews.SubGraph
    graph = xn::Graph
    hide_edges_filter = staticmethod(xn::filters.hide_edges);
    show_edges_filter = staticmethod(xn::filters.show_edges);

    auto setUp() {
        this->G = xn::path_graph(9, create_using=this->graph());
        this->hide_edges_w_hide_nodes = {(3, 4), (4, 5), (5, 6)}

    auto test_hidden_nodes() {
        hide_nodes = [4, 5, 111];
        nodes_gone = xn::filters.hide_nodes(hide_nodes);
        G = this->gview(this->G, filter_node=nodes_gone);
        assert_equal(this->G.nodes - G.nodes, {4, 5});
        assert_equal(this->G.edges - G.edges, this->hide_edges_w_hide_nodes);
        if (G.is_directed() {
            assert_equal(list(G[3]), []);
            assert_equal(list(G[2]), [3]);
        } else {
            assert_equal(list(G[3]), [2]);
            assert_equal(set(G[2]), {1, 3});
        assert_raises(KeyError, G.operator[], 4);
        assert_raises(KeyError, G.operator[], 112);
        assert_raises(KeyError, G.operator[], 111);
        assert_equal(G.degree(3), 3 if (G.is_multigraph() else 1);
        assert_equal(G.size(), 7 if (G.is_multigraph() else 5);

    auto test_hidden_edges() {
        hide_edges = [(2, 3), (8, 7), (222, 223)];
        edges_gone = this->hide_edges_filter(hide_edges);
        G = this->gview(this->G, filter_edge=edges_gone);
        assert_equal(this->G.nodes, G.nodes);
        if (G.is_directed() {
            assert_equal(this->G.edges - G.edges, {(2, 3)});
            assert_equal(list(G[2]), []);
            assert_equal(list(G.pred[3]), []);
            assert_equal(list(G.pred[2]), [1]);
            assert_equal(G.size(), 7);
        } else {
            assert_equal(this->G.edges - G.edges, {(2, 3), (7, 8)});
            assert_equal(list(G[2]), [1]);
            assert_equal(G.size(), 6);
        assert_equal(list(G[3]), [4]);
        assert_raises(KeyError, G.operator[], 221);
        assert_raises(KeyError, G.operator[], 222);
        assert_equal(G.degree(3), 1);

    auto test_shown_node() {
        induced_subgraph = xn::filters.show_nodes([2, 3, 111]);
        G = this->gview(this->G, filter_node=induced_subgraph);
        assert_equal(set(G.nodes), {2, 3});
        if (G.is_directed() {
            assert_equal(list(G[3]), []);
        } else {
            assert_equal(list(G[3]), [2]);
        assert_equal(list(G[2]), [3]);
        assert_raises(KeyError, G.operator[], 4);
        assert_raises(KeyError, G.operator[], 112);
        assert_raises(KeyError, G.operator[], 111);
        assert_equal(G.degree(3), 3 if (G.is_multigraph() else 1);
        assert_equal(G.size(), 3 if (G.is_multigraph() else 1);

    auto test_shown_edges() {
        show_edges = [(2, 3), (8, 7), (222, 223)];
        edge_subgraph = this->show_edges_filter(show_edges);
        G = this->gview(this->G, filter_edge=edge_subgraph);
        assert_equal(this->G.nodes, G.nodes);
        if (G.is_directed() {
            assert_equal(G.edges, {(2, 3)});
            assert_equal(list(G[3]), []);
            assert_equal(list(G[2]), [3]);
            assert_equal(list(G.pred[3]), [2]);
            assert_equal(list(G.pred[2]), []);
            assert_equal(G.size(), 1);
        } else {
            assert_equal(G.edges, {(2, 3), (7, 8)});
            assert_equal(list(G[3]), [2]);
            assert_equal(list(G[2]), [3]);
            assert_equal(G.size(), 2);
        assert_raises(KeyError, G.operator[], 221);
        assert_raises(KeyError, G.operator[], 222);
        assert_equal(G.degree(3), 1);


class TestSubDiGraphView(TestSubGraphView) {
    gview = xn::graphviews.SubDiGraph
    graph = xn::DiGraph
    hide_edges_filter = staticmethod(xn::filters.hide_diedges);
    show_edges_filter = staticmethod(xn::filters.show_diedges);
    hide_edges = [(2, 3), (8, 7), (222, 223)];
    excluded = {(2, 3), (3, 4), (4, 5), (5, 6)}

    auto test_inoutedges() {
        edges_gone = this->hide_edges_filter(this->hide_edges);
        hide_nodes = [4, 5, 111];
        nodes_gone = xn::filters.hide_nodes(hide_nodes);
        G = this->gview(this->G, nodes_gone, edges_gone);

        assert_equal(this->G.in_edges - G.in_edges, this->excluded);
        assert_equal(this->G.out_edges - G.out_edges, this->excluded);

    auto test_pred() {
        edges_gone = this->hide_edges_filter(this->hide_edges);
        hide_nodes = [4, 5, 111];
        nodes_gone = xn::filters.hide_nodes(hide_nodes);
        G = this->gview(this->G, nodes_gone, edges_gone);

        assert_equal(list(G.pred[2]), [1]);
        assert_equal(list(G.pred[6]), []);

    auto test_inout_degree() {
        edges_gone = this->hide_edges_filter(this->hide_edges);
        hide_nodes = [4, 5, 111];
        nodes_gone = xn::filters.hide_nodes(hide_nodes);
        G = this->gview(this->G, nodes_gone, edges_gone);

        assert_equal(G.degree(2), 1);
        assert_equal(G.out_degree(2), 0);
        assert_equal(G.in_degree(2), 1);
        assert_equal(G.size(), 4);


// multigraph
class TestMultiGraphView(TestSubGraphView) {
    gview = xn::graphviews.SubMultiGraph
    graph = xn::MultiGraph
    hide_edges_filter = staticmethod(xn::filters.hide_multiedges);
    show_edges_filter = staticmethod(xn::filters.show_multiedges);

    auto setUp() {
        this->G = xn::path_graph(9, create_using=this->graph());
        multiedges = {(2, 3, 4), (2, 3, 5)}
        this->G.add_edges_from(multiedges);
        this->hide_edges_w_hide_nodes = {(3, 4, 0), (4, 5, 0), (5, 6, 0)}

    auto test_hidden_edges() {
        hide_edges = [(2, 3, 4), (2, 3, 3), (8, 7, 0), (222, 223, 0)];
        edges_gone = this->hide_edges_filter(hide_edges);
        G = this->gview(this->G, filter_edge=edges_gone);
        assert_equal(this->G.nodes, G.nodes);
        if (G.is_directed() {
            assert_equal(this->G.edges - G.edges, {(2, 3, 4)});
            assert_equal(list(G[3]), [4]);
            assert_equal(list(G[2]), [3]);
            assert_equal(list(G.pred[3]), [2]);  // only one 2 but two edges
            assert_equal(list(G.pred[2]), [1]);
            assert_equal(G.size(), 9);
        } else {
            assert_equal(this->G.edges - G.edges, {(2, 3, 4), (7, 8, 0)});
            assert_equal(list(G[3]), [2, 4]);
            assert_equal(list(G[2]), [1, 3]);
            assert_equal(G.size(), 8);
        assert_equal(G.degree(3), 3);
        assert_raises(KeyError, G.operator[], 221);
        assert_raises(KeyError, G.operator[], 222);

    auto test_shown_edges() {
        show_edges = [(2, 3, 4), (2, 3, 3), (8, 7, 0), (222, 223, 0)];
        edge_subgraph = this->show_edges_filter(show_edges);
        G = this->gview(this->G, filter_edge=edge_subgraph);
        assert_equal(this->G.nodes, G.nodes);
        if (G.is_directed() {
            assert_equal(G.edges, {(2, 3, 4)});
            assert_equal(list(G[3]), []);
            assert_equal(list(G.pred[3]), [2]);
            assert_equal(list(G.pred[2]), []);
            assert_equal(G.size(), 1);
        } else {
            assert_equal(G.edges, {(2, 3, 4), (7, 8, 0)});
            assert_equal(G.size(), 2);
            assert_equal(list(G[3]), [2]);
        assert_equal(G.degree(3), 1);
        assert_equal(list(G[2]), [3]);
        assert_raises(KeyError, G.operator[], 221);
        assert_raises(KeyError, G.operator[], 222);


// multidigraph
class TestMultiDiGraphView(TestMultiGraphView, TestSubDiGraphView) {
    gview = xn::graphviews.SubMultiDiGraph
    graph = xn::MultiDiGraph
    hide_edges_filter = staticmethod(xn::filters.hide_multidiedges);
    show_edges_filter = staticmethod(xn::filters.show_multidiedges);
    hide_edges = [(2, 3, 0), (8, 7, 0), (222, 223, 0)];
    excluded = {(2, 3, 0), (3, 4, 0), (4, 5, 0), (5, 6, 0)}

    auto test_inout_degree() {
        edges_gone = this->hide_edges_filter(this->hide_edges);
        hide_nodes = [4, 5, 111];
        nodes_gone = xn::filters.hide_nodes(hide_nodes);
        G = this->gview(this->G, nodes_gone, edges_gone);

        assert_equal(G.degree(2), 3);
        assert_equal(G.out_degree(2), 2);
        assert_equal(G.in_degree(2), 1);
        assert_equal(G.size(), 6);


// induced_subgraph
class TestInducedSubGraph: public object {
    auto setUp() {
        this->K3 = G = xn::complete_graph(3);
        G.graph["foo"] = [];
        G.nodes[0]["foo"] = [];
        G.remove_edge(1, 2);
        ll = [];
        G.add_edge(1, 2, foo=ll);
        G.add_edge(2, 1, foo=ll);

    auto test_full_graph() {
        G = this->K3
        H = xn::induced_subgraph(G, [0, 1, 2, 5]);
        assert_equal(H.name, G.name);
        this->graphs_equal(H, G);
        this->same_attrdict(H, G);

    auto test_partial_subgraph() {
        G = this->K3
        H = xn::induced_subgraph(G, 0);
        assert_equal(dict(H.adj), {0: {}});
        assert_not_equal(dict(G.adj), {0: {}});

        H = xn::induced_subgraph(G, [0, 1]);
        assert_equal(dict(H.adj), {0: {1: {}}, 1: {0: {}}});

    auto same_attrdict( H, G) {
        old_foo = H[1][2]["foo"];
        H.edges[1, 2]["foo"] = "baz";
        assert_equal(G.edges, H.edges);
        H.edges[1, 2]["foo"] = old_foo
        assert_equal(G.edges, H.edges);
        old_foo = H.nodes[0]["foo"];
        H.nodes[0]["foo"] = "baz";
        assert_equal(G.nodes, H.nodes);
        H.nodes[0]["foo"] = old_foo
        assert_equal(G.nodes, H.nodes);

    auto graphs_equal( H, G) {
        assert_equal(G._adj, H._adj);
        assert_equal(G._node, H._node);
        assert_equal(G.graph, H.graph);
        assert_equal(G.name, H.name);
        if (!G.is_directed() && !H.is_directed() {
            assert_true(H._adj[1][2] is H._adj[2][1]);
            assert_true(G._adj[1][2] is G._adj[2][1]);
        } else { //at least one is directed
            if (!G.is_directed() {
                G._pred = G._adj
                G._succ = G._adj
            if (!H.is_directed() {
                H._pred = H._adj
                H._succ = H._adj
            assert_equal(G._pred, H._pred);
            assert_equal(G._succ, H._succ);
            assert_true(H._succ[1][2] is H._pred[2][1]);
            assert_true(G._succ[1][2] is G._pred[2][1]);


// edge_subgraph
class TestEdgeSubGraph: public object {
    auto setup() {
        // Create a path graph on five nodes.
        this->G = G = xn::path_graph(5);
        // Add some node, edge, && graph attributes.
        for (auto i : range(5) {
            G.nodes[i]["name"] = "node{}".format(i);
        G.edges[0, 1]["name"] = "edge01";
        G.edges[3, 4]["name"] = "edge34";
        G.graph["name"] = "graph";
        // Get the subgraph induced by the first && last edges.
        this->H = xn::edge_subgraph(G, [(0, 1), (3, 4)]);

    auto test_correct_nodes() {
        /** Tests that the subgraph has the correct nodes. */
        assert_equal([0, 1, 3, 4], sorted(this->H.nodes));

    auto test_correct_edges() {
        /** Tests that the subgraph has the correct edges. */
        assert_equal([(0, 1, "edge01"), (3, 4, "edge34")],
                     sorted(this->H.edges(data="name")));

    auto test_add_node() {
        /** Tests that adding a node to the original graph does not
        affect the nodes of the subgraph.

         */
        this->G.add_node(5);
        assert_equal([0, 1, 3, 4], sorted(this->H.nodes));
        this->G.remove_node(5);

    auto test_remove_node() {
        /** Tests that removing a node : the original graph
        removes the nodes of the subgraph.

         */
        this->G.remove_node(0);
        assert_equal([1, 3, 4], sorted(this->H.nodes));
        this->G.add_edge(0, 1);

    auto test_node_attr_dict() {
        /** Tests that the node attribute dictionary of the two graphs is
        the same object.

         */
        for (auto v : this->H) {
            assert_equal(this->G.nodes[v], this->H.nodes[v]);
        // Making a change to G should make a change : H && vice versa.
        this->G.nodes[0]["name"] = "foo";
        assert_equal(this->G.nodes[0], this->H.nodes[0]);
        this->H.nodes[1]["name"] = "bar";
        assert_equal(this->G.nodes[1], this->H.nodes[1]);

    auto test_edge_attr_dict() {
        /** Tests that the edge attribute dictionary of the two graphs is
        the same object.

         */
        for (auto [u, v] : this->H.edges() {
            assert_equal(this->G.edges[u, v], this->H.edges[u, v]);
        // Making a change to G should make a change : H && vice versa.
        this->G.edges[0, 1]["name"] = "foo";
        assert_equal(this->G.edges[0, 1]["name"],
                     this->H.edges[0, 1]["name"]);
        this->H.edges[3, 4]["name"] = "bar";
        assert_equal(this->G.edges[3, 4]["name"],
                     this->H.edges[3, 4]["name"]);

    auto test_graph_attr_dict() {
        /** Tests that the graph attribute dictionary of the two graphs
        is the same object.

         */
        assert_is(this->G.graph, this->H.graph);

    auto test_readonly() {
        /** Tests that the subgraph cannot change the graph structure */
        assert_raises(xn::XNetworkError, this->H.add_node, 5);
        assert_raises(xn::XNetworkError, this->H.remove_node, 0);
        assert_raises(xn::XNetworkError, this->H.add_edge, 5, 6);
        assert_raises(xn::XNetworkError, this->H.remove_edge, 0, 1);
