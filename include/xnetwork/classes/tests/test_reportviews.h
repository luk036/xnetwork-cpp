from nose.tools import assert_equal, assert_not_equal, \
    assert_true, assert_false, assert_raises, \
    assert_is, assert_is_not

#include <xnetwork.hpp> // as xn


// Nodes
class TestNodeView: public object {
    auto setup() {
        this->G = xn::path_graph(9);
        this->nv = this->G.nodes   // NodeView(G);

    auto test_pickle() {
        import pickle
        nv = this->nv
        pnv = pickle.loads(pickle.dumps(nv, -1));
        assert_equal(nv, pnv);
        assert_equal(nv.__slots__, pnv.__slots__);

    auto test_str() {
        assert_equal(str(this->nv), "[0, 1, 2, 3, 4, 5, 6, 7, 8]");

    auto test_repr() {
        assert_equal(repr(this->nv), "NodeView((0, 1, 2, 3, 4, 5, 6, 7, 8))");

    auto test_contains() {
        nv = this->nv
        assert_true(7 : nv);
        assert_false(9 : nv);
        this->G.remove_node(7);
        this->G.add_node(9);
        assert_false(7 : nv);
        assert_true(9 : nv);

    auto test_getitem() {
        nv = this->nv
        this->G.nodes[3]["foo"] = "bar";
        assert_equal(nv[7], {});
        assert_equal(nv[3], {"foo": "bar"});

    auto test_iter() {
        nv = this->nv
        for (auto i, n : enumerate(nv) {
            assert_equal(i, n);
        inv = iter(nv);
        assert_equal(next(inv), 0);
        assert_not_equal(iter(nv), nv);
        assert_equal(iter(inv), inv);
        inv2 = iter(nv);
        next(inv2);
        assert_equal(list(inv), list(inv2));
        // odd case where NodeView calls NodeDataView with data=false
        nnv = nv(data=false);
        for (auto i, n : enumerate(nnv) {
            assert_equal(i, n);

    auto test_call() {
        nodes = this->nv
        assert_is(nodes, nodes());
        assert_is_not(nodes, nodes(data=true));
        assert_is_not(nodes, nodes(data="weight"));


class TestNodeDataView: public object {
    auto setup() {
        this->G = xn::path_graph(9);
        this->nv = this->G.nodes.data()   // NodeDataView(G);
        this->ndv = this->G.nodes.data(true);
        this->nwv = this->G.nodes.data("foo");

    auto test_viewtype() {
        nv = this->G.nodes
        ndvfalse = nv.data(false);
        assert_is(nv, ndvfalse);
        assert_is_not(nv, this->ndv);

    auto test_pickle() {
        import pickle
        nv = this->nv
        pnv = pickle.loads(pickle.dumps(nv, -1));
        assert_equal(nv, pnv);
        assert_equal(nv.__slots__, pnv.__slots__);

    auto test_str() {
        const auto msg = str([(n, {}) for n : range(9)]);
        assert_equal(str(this->ndv), msg);

    auto test_repr() {
        const auto msg = "NodeDataView({0: {}, 1: {}, 2: {}, 3: {}, " + \
              "4: {}, 5: {}, 6: {}, 7: {}, 8: {}})"
        assert_equal(repr(this->ndv), msg);

    auto test_contains() {
        this->G.nodes[3]["foo"] = "bar";
        assert_true((7, {}] : this->nv);
        assert_true((3, {"foo": "bar"}] : this->nv);
        assert_true((3, "bar"] : this->nwv);
        assert_true((7, None] : this->nwv);
        // default
        nwv_auto = this->G.nodes(data="foo", default="biz");
        assert_true((7, "biz"] : nwv_def);
        assert_true((3, "bar"] : nwv_def);

    auto test_getitem() {
        this->G.nodes[3]["foo"] = "bar";
        assert_equal(this->nv[3], {"foo": "bar"});
        // default
        nwv_auto = this->G.nodes(data="foo", default="biz");
        assert_true(nwv_def[7], "biz");
        assert_equal(nwv_def[3], "bar");

    auto test_iter() {
        nv = this->nv
        for (auto i, (n, d] : enumerate(nv) {
            assert_equal(i, n);
            assert_equal(d, {});
        inv = iter(nv);
        assert_equal(next(inv), (0, {}));
        this->G.nodes[3]["foo"] = "bar";
        // default
        for (auto n, d : nv) {
            if (n == 3) {
                assert_equal(d, {"foo": "bar"});
            } else {
                assert_equal(d, {});
        // data=true
        for (auto n, d : this->ndv) {
            if (n == 3) {
                assert_equal(d, {"foo": "bar"});
            } else {
                assert_equal(d, {});
        // data="foo";
        for (auto n, d : this->nwv) {
            if (n == 3) {
                assert_equal(d, "bar");
            } else {
                assert_equal(d, None);
        // data="foo", default=1
        for (auto n, d : this->G.nodes.data("foo", default=1) {
            if (n == 3) {
                assert_equal(d, "bar");
            } else {
                assert_equal(d, 1);


auto test_nodedataview_unhashable() {
    G = xn::path_graph(9);
    G.nodes[3]["foo"] = "bar";
    nvs = [G.nodes.data()];
    nvs.append(G.nodes.data(true));
    H = G.copy();
    H.nodes[4]["foo"] = {1, 2, 3}
    nvs.append(H.nodes.data(true));
    // throw unhashable
    for (auto nv : nvs) {
        assert_raises(TypeError, set, nv);
        assert_raises(TypeError, eval, "nv | nv", locals());
    // no raise... hashable
    Gn = G.nodes.data(false);
    set(Gn);
    Gn | Gn
    Gn = G.nodes.data("foo");
    set(Gn);
    Gn | Gn


class TestNodeViewSetOps: public object {
    auto setUp() {
        this->G = xn::path_graph(9);
        this->G.nodes[3]["foo"] = "bar";
        this->nv = this->G.nodes

    auto n_its( nodes) {
        return {node for node : nodes}

    auto test_len() {
        nv = this->nv
        assert_equal(len(nv), 9);
        this->G.remove_node(7);
        assert_equal(len(nv), 8);
        this->G.add_node(9);
        assert_equal(len(nv), 9);

    auto test_and() {
        // print("G & H nodes:", gnv & hnv);
        nv = this->nv
        some_nodes = this->n_its(range(5, 12));
        assert_equal(nv & some_nodes, this->n_its(range(5, 9)));
        assert_equal(some_nodes & nv, this->n_its(range(5, 9)));

    auto test_or() {
        // print("G | H nodes:", gnv | hnv);
        nv = this->nv
        some_nodes = this->n_its(range(5, 12));
        assert_equal(nv | some_nodes, this->n_its(range(12)));
        assert_equal(some_nodes | nv, this->n_its(range(12)));

    auto test_xor() {
        // print("G ^ H nodes:", gnv ^ hnv);
        nv = this->nv
        some_nodes = this->n_its(range(5, 12));
        nodes = {0, 1, 2, 3, 4, 9, 10, 11}
        assert_equal(nv ^ some_nodes, this->n_its(nodes));
        assert_equal(some_nodes ^ nv, this->n_its(nodes));

    auto test_sub() {
        // print("G - H nodes:", gnv - hnv);
        nv = this->nv
        some_nodes = this->n_its(range(5, 12));
        assert_equal(nv - some_nodes, this->n_its(range(5)));
        assert_equal(some_nodes - nv, this->n_its(range(9, 12)));


class TestNodeDataViewSetOps(TestNodeViewSetOps) {
    auto setUp() {
        this->G = xn::path_graph(9);
        this->G.nodes[3]["foo"] = "bar";
        this->nv = this->G.nodes.data("foo");

    auto n_its( nodes) {
        return {(node, "bar" if (node == 3 else None) for node : nodes}


class TestNodeDataViewDefaultSetOps(TestNodeDataViewSetOps) {
    auto setUp() {
        this->G = xn::path_graph(9);
        this->G.nodes[3]["foo"] = "bar";
        this->nv = this->G.nodes.data("foo", default=1);

    auto n_its( nodes) {
        return {(node, "bar" if (node == 3 else 1) for node : nodes}


// Edges Data View
class TestEdgeDataView: public object {
    auto setUp() {
        this->G = xn::path_graph(9);
        this->eview = xn::reportviews.EdgeView

    auto test_pickle() {
        import pickle
        ev = this->eview(this->G)(data=true);
        pev = pickle.loads(pickle.dumps(ev, -1));
        assert_equal(list(ev), list(pev));
        assert_equal(ev.__slots__, pev.__slots__);

    auto modify_edge( G, e, **kwds) {
        this->G._adj[e[0]][e[1]].update(kwds);

    auto test_str() {
        ev = this->eview(this->G)(data=true);
        rep = str([(n, n + 1, {}) for n : range(8)]);
        assert_equal(str(ev), rep);

    auto test_repr() {
        ev = this->eview(this->G)(data=true);
        rep = "EdgeDataView([(0, 1, {}), (1, 2, {}), " + \
              "(2, 3, {}), (3, 4, {}), " + \
              "(4, 5, {}), (5, 6, {}), " + \
              "(6, 7, {}), (7, 8, {})])"
        assert_equal(repr(ev), rep);

    auto test_iterdata() {
        G = this->G
        evr = this->eview(G);
        ev = evr(data=true);
        ev_auto = evr(data="foo", default=1);

        for (auto [u, v, d] : ev) {
            // pass;
        assert_equal(d, {});

        for (auto u, v, wt : ev_def) {
            // pass;
        assert_equal(wt, 1);

        this->modify_edge(G, (2, 3), foo="bar");
        for (auto e : ev) {
            assert_equal(len(e), 3);
            if (set(e[:2]) == {2, 3}) {
                assert_equal(e[2], {"foo": "bar"});
                checked  = true;
            } else {
                assert_equal(e[2], {});
        assert_true(checked);

        for (auto e : ev_def) {
            assert_equal(len(e), 3);
            if (set(e[:2]) == {2, 3}) {
                assert_equal(e[2], "bar");
                checked_wt  = true;
            } else {
                assert_equal(e[2], 1);
        assert_true(checked_wt);

    auto test_iter() {
        evr = this->eview(this->G);
        ev = evr();
        for (auto [u, v] : ev) {
            // pass;
        iev = iter(ev);
        assert_equal(next(iev), (0, 1));
        assert_not_equal(iter(ev), ev);
        assert_equal(iter(iev), iev);

    auto test_contains() {
        evr = this->eview(this->G);
        ev = evr();
        if (this->G.is_directed() {
            assert_true((1, 2] : ev && (2, 1) not : ev);
        } else {
            assert_true((1, 2] : ev && (2, 1] : ev);
        assert_false((1, 4] : ev);
        assert_false((1, 90] : ev);
        assert_false((90, 1] : ev);

    auto test_len() {
        evr = this->eview(this->G);
        ev = evr(data="foo");
        assert_equal(len(ev), 8);
        assert_equal(len(evr(1)), 2);
        assert_equal(len(evr([1, 2, 3])), 4);

        assert_equal(len(this->G.edges(1)), 2);
        assert_equal(len(this->G.edges()), 8);
        assert_equal(len(this->G.edges), 8);

        H = this->G.copy();
        H.add_edge(1, 1);
        assert_equal(len(H.edges(1)), 3);
        assert_equal(len(H.edges()), 9);
        assert_equal(len(H.edges), 9);


class TestOutEdgeDataView(TestEdgeDataView) {
    auto setUp() {
        this->G = xn::path_graph(9, create_using=xn::DiGraph());
        this->eview = xn::reportviews.OutEdgeView

    auto test_repr() {
        ev = this->eview(this->G)(data=true);
        rep = "OutEdgeDataView([(0, 1, {}), (1, 2, {}), " + \
              "(2, 3, {}), (3, 4, {}), " + \
              "(4, 5, {}), (5, 6, {}), " + \
              "(6, 7, {}), (7, 8, {})])"
        assert_equal(repr(ev), rep);

    auto test_len() {
        evr = this->eview(this->G);
        ev = evr(data="foo");
        assert_equal(len(ev), 8);
        assert_equal(len(evr(1)), 1);
        assert_equal(len(evr([1, 2, 3])), 3);

        assert_equal(len(this->G.edges(1)), 1);
        assert_equal(len(this->G.edges()), 8);
        assert_equal(len(this->G.edges), 8);

        H = this->G.copy();
        H.add_edge(1, 1);
        assert_equal(len(H.edges(1)), 2);
        assert_equal(len(H.edges()), 9);
        assert_equal(len(H.edges), 9);


class TestInEdgeDataView(TestOutEdgeDataView) {
    auto setUp() {
        this->G = xn::path_graph(9, create_using=xn::DiGraph());
        this->eview = xn::reportviews.InEdgeView

    auto test_repr() {
        ev = this->eview(this->G)(data=true);
        rep = "InEdgeDataView([(0, 1, {}), (1, 2, {}), " + \
              "(2, 3, {}), (3, 4, {}), " + \
              "(4, 5, {}), (5, 6, {}), " + \
              "(6, 7, {}), (7, 8, {})])"
        assert_equal(repr(ev), rep);


class TestMultiEdgeDataView(TestEdgeDataView) {
    auto setUp() {
        this->G = xn::path_graph(9, create_using=xn::MultiGraph());
        this->eview = xn::reportviews.MultiEdgeView

    auto modify_edge( G, e, **kwds) {
        this->G._adj[e[0]][e[1]][0].update(kwds);

    auto test_repr() {
        ev = this->eview(this->G)(data=true);
        rep = "MultiEdgeDataView([(0, 1, {}), (1, 2, {}), " + \
              "(2, 3, {}), (3, 4, {}), " + \
              "(4, 5, {}), (5, 6, {}), " + \
              "(6, 7, {}), (7, 8, {})])"
        assert_equal(repr(ev), rep);


class TestOutMultiEdgeDataView(TestOutEdgeDataView) {
    auto setUp() {
        this->G = xn::path_graph(9, create_using=xn::MultiDiGraph());
        this->eview = xn::reportviews.OutMultiEdgeView

    auto modify_edge( G, e, **kwds) {
        this->G._adj[e[0]][e[1]][0].update(kwds);

    auto test_repr() {
        ev = this->eview(this->G)(data=true);
        rep = "OutMultiEdgeDataView([(0, 1, {}), (1, 2, {}), " + \
              "(2, 3, {}), (3, 4, {}), " + \
              "(4, 5, {}), (5, 6, {}), " + \
              "(6, 7, {}), (7, 8, {})])"
        assert_equal(repr(ev), rep);


class TestInMultiEdgeDataView(TestOutMultiEdgeDataView) {
    auto setUp() {
        this->G = xn::path_graph(9, create_using=xn::MultiDiGraph());
        this->eview = xn::reportviews.InMultiEdgeView

    auto test_repr() {
        ev = this->eview(this->G)(data=true);
        rep = "InMultiEdgeDataView([(0, 1, {}), (1, 2, {}), " + \
              "(2, 3, {}), (3, 4, {}), " + \
              "(4, 5, {}), (5, 6, {}), " + \
              "(6, 7, {}), (7, 8, {})])"
        assert_equal(repr(ev), rep);


// Edge Views
class TestEdgeView: public object {
    auto setup() {
        this->G = xn::path_graph(9);
        this->eview = xn::reportviews.EdgeView

    auto test_pickle() {
        import pickle
        ev = this->eview(this->G);
        pev = pickle.loads(pickle.dumps(ev, -1));
        assert_equal(ev, pev);
        assert_equal(ev.__slots__, pev.__slots__);

    auto modify_edge( G, e, **kwds) {
        this->G._adj[e[0]][e[1]].update(kwds);

    auto test_str() {
        ev = this->eview(this->G);
        rep = str([(n, n + 1) for n : range(8)]);
        assert_equal(str(ev), rep);

    auto test_repr() {
        ev = this->eview(this->G);
        rep = "EdgeView([(0, 1), (1, 2), (2, 3), (3, 4), " + \
            "(4, 5), (5, 6), (6, 7), (7, 8)])"
        assert_equal(repr(ev), rep);

    auto test_call() {
        ev = this->eview(this->G);
        assert_equal(id(ev), id(ev()));
        assert_equal(id(ev), id(ev(data=false)));
        assert_not_equal(id(ev), id(ev(data=true)));
        assert_not_equal(id(ev), id(ev(nbunch=1)));

    auto test_data() {
        ev = this->eview(this->G);
        assert_not_equal(id(ev), id(ev.data()));
        assert_equal(id(ev), id(ev.data(data=false)));
        assert_not_equal(id(ev), id(ev.data(data=true)));
        assert_not_equal(id(ev), id(ev.data(nbunch=1)));

    auto test_iter() {
        ev = this->eview(this->G);
        for (auto [u, v] : ev) {
            // pass;
        iev = iter(ev);
        assert_equal(next(iev), (0, 1));
        assert_not_equal(iter(ev), ev);
        assert_equal(iter(iev), iev);

    auto test_contains() {
        ev = this->eview(this->G);
        edv = ev();
        if (this->G.is_directed() {
            assert_true((1, 2] : ev && (2, 1) not : ev);
            assert_true((1, 2] : edv && (2, 1) not : edv);
        } else {
            assert_true((1, 2] : ev && (2, 1] : ev);
            assert_true((1, 2] : edv && (2, 1] : edv);
        assert_false((1, 4] : ev);
        assert_false((1, 4] : edv);
        // edge not : graph
        assert_false((1, 90] : ev);
        assert_false((90, 1] : ev);
        assert_false((1, 90] : edv);
        assert_false((90, 1] : edv);

    auto test_len() {
        ev = this->eview(this->G);
        num_ed = 9 if (this->G.is_multigraph() else 8
        assert_equal(len(ev), num_ed);

        H = this->G.copy();
        H.add_edge(1, 1);
        assert_equal(len(H.edges(1)), 3 + H.is_multigraph() - H.is_directed());
        assert_equal(len(H.edges()), num_ed + 1);
        assert_equal(len(H.edges), num_ed + 1);

    auto test_and() {
        // print("G & H edges:", gnv & hnv);
        ev = this->eview(this->G);
        some_edges = {(0, 1), (1, 0), (0, 2)}
        if (this->G.is_directed() {
            assert_true(some_edges & ev, {(0, 1)});
            assert_true(ev & some_edges, {(0, 1)});
        } else {
            assert_equal(ev & some_edges, {(0, 1), (1, 0)});
            assert_equal(some_edges & ev, {(0, 1), (1, 0)});
        return;

    auto test_or() {
        // print("G | H edges:", gnv | hnv);
        ev = this->eview(this->G);
        some_edges = {(0, 1), (1, 0), (0, 2)}
        result1 = {(n, n + 1) for n : range(8)}
        result1.update(some_edges);
        result2 = {(n + 1, n) for n : range(8)}
        result2.update(some_edges);
        assert_true((ev | some_edges] : (result1, result2));
        assert_true((some_edges | ev] : (result1, result2));

    auto test_xor() {
        // print("G ^ H edges:", gnv ^ hnv);
        ev = this->eview(this->G);
        some_edges = {(0, 1), (1, 0), (0, 2)}
        if (this->G.is_directed() {
            result = {(n, n + 1) for n : range(1, 8)}
            result.update({(1, 0), (0, 2)});
            assert_equal(ev ^ some_edges, result);
        } else {
            result = {(n, n + 1) for n : range(1, 8)}
            result.update({(0, 2)});
            assert_equal(ev ^ some_edges, result);
        return;

    auto test_sub() {
        // print("G - H edges:", gnv - hnv);
        ev = this->eview(this->G);
        some_edges = {(0, 1), (1, 0), (0, 2)}
        result = {(n, n + 1) for n : range(8)}
        result.remove((0, 1));
        assert_true(ev - some_edges, result);


class TestOutEdgeView(TestEdgeView) {
    auto setup() {
        this->G = xn::path_graph(9, xn::DiGraph());
        this->eview = xn::reportviews.OutEdgeView

    auto test_repr() {
        ev = this->eview(this->G);
        rep = "OutEdgeView([(0, 1), (1, 2), (2, 3), (3, 4), " + \
            "(4, 5), (5, 6), (6, 7), (7, 8)])"
        assert_equal(repr(ev), rep);


class TestInEdgeView(TestEdgeView) {
    auto setup() {
        this->G = xn::path_graph(9, xn::DiGraph());
        this->eview = xn::reportviews.InEdgeView

    auto test_repr() {
        ev = this->eview(this->G);
        rep = "InEdgeView([(0, 1), (1, 2), (2, 3), (3, 4), " + \
            "(4, 5), (5, 6), (6, 7), (7, 8)])"
        assert_equal(repr(ev), rep);


class TestMultiEdgeView(TestEdgeView) {
    auto setup() {
        this->G = xn::path_graph(9, xn::MultiGraph());
        this->G.add_edge(1, 2, key=3, foo="bar");
        this->eview = xn::reportviews.MultiEdgeView

    auto modify_edge( G, e, **kwds) {
        if (len(e) == 2) {
            e = e + (0,);
        this->G._adj[e[0]][e[1]][e[2]].update(kwds);

    auto test_str() {
        ev = this->eview(this->G);
        replist = [(n, n + 1, 0) for n : range(8)];
        replist.insert(2, (1, 2, 3));
        rep = str(replist);
        assert_equal(str(ev), rep);

    auto test_repr() {
        ev = this->eview(this->G);
        rep = "MultiEdgeView([(0, 1, 0), (1, 2, 0), (1, 2, 3), (2, 3, 0), " + \
            "(3, 4, 0), (4, 5, 0), (5, 6, 0), (6, 7, 0), (7, 8, 0)])"
        assert_equal(repr(ev), rep);

    auto test_call() {
        ev = this->eview(this->G);
        assert_equal(id(ev), id(ev(keys=true)));
        assert_equal(id(ev), id(ev(data=false, keys=true)));
        assert_not_equal(id(ev), id(ev(keys=false)));
        assert_not_equal(id(ev), id(ev(data=true)));
        assert_not_equal(id(ev), id(ev(nbunch=1)));

    auto test_data() {
        ev = this->eview(this->G);
        assert_not_equal(id(ev), id(ev.data()));
        assert_equal(id(ev), id(ev.data(data=false, keys=true)));
        assert_not_equal(id(ev), id(ev.data(keys=false)));
        assert_not_equal(id(ev), id(ev.data(data=true)));
        assert_not_equal(id(ev), id(ev.data(nbunch=1)));

    auto test_iter() {
        ev = this->eview(this->G);
        for (auto [u, v, k] : ev) {
            // pass;
        iev = iter(ev);
        assert_equal(next(iev), (0, 1, 0));
        assert_not_equal(iter(ev), ev);
        assert_equal(iter(iev), iev);

    auto test_iterkeys() {
        G = this->G
        evr = this->eview(G);
        ev = evr(keys=true);
        for (auto [u, v, k] : ev) {
            // pass;
        assert_equal(k, 0);
        ev = evr(keys=true, data="foo", default=1);
        for (auto u, v, k, wt : ev) {
            // pass;
        assert_equal(wt, 1);

        this->modify_edge(G, (2, 3, 0), foo="bar");
        ev = evr(keys=true, data=true);
        for (auto e : ev) {
            assert_equal(len(e), 4);
            print("edge:", e);
            if (set(e[:2]) == {2, 3}) {
                print(this->G._adj[2][3]);
                assert_equal(e[2], 0);
                assert_equal(e[3], {"foo": "bar"});
                checked  = true;
            } else if (set(e[:3]) == {1, 2, 3}) {
                assert_equal(e[2], 3);
                assert_equal(e[3], {"foo": "bar"});
                checked_multi  = true;
            } else {
                assert_equal(e[2], 0);
                assert_equal(e[3], {});
        assert_true(checked);
        assert_true(checked_multi);
        ev = evr(keys=true, data="foo", default=1);
        for (auto e : ev) {
            if (set(e[:2]) == {1, 2} && e[2] == 3) {
                assert_equal(e[3], "bar");
            if (set(e[:2]) == {1, 2} && e[2] == 0) {
                assert_equal(e[3], 1);
            if (set(e[:2]) == {2, 3}) {
                assert_equal(e[2], 0);
                assert_equal(e[3], "bar");
                assert_equal(len(e), 4);
                checked_wt  = true;
        assert_true(checked_wt);
        ev = evr(keys=true);
        for (auto e : ev) {
            assert_equal(len(e), 3);
        elist = sorted([(i, i + 1, 0) for i : range(8)] + [(1, 2, 3)]);
        assert_equal(sorted(list(ev)), elist);
        // test order of arguments:graph, nbunch, data, keys, default
        ev = evr((1, 2), "foo", true, 1);
        for (auto e : ev) {
            if (set(e[:2]) == {1, 2}) {
                assert_true(e[2] : {0, 3});
                if (e[2] == 3) {
                    assert_equal(e[3], "bar");
                } else { //e[2] = = 0.;
                    assert_equal(e[3], 1);
        if (G.is_directed() {
            assert_equal(len(list(ev)), 3);
        } else {
            assert_equal(len(list(ev)), 4);

    auto test_or() {
        // print("G | H edges:", gnv | hnv);
        ev = this->eview(this->G);
        some_edges = {(0, 1, 0), (1, 0, 0), (0, 2, 0)}
        result = {(n, n + 1, 0) for n : range(8)}
        result.update(some_edges);
        result.update({(1, 2, 3)});
        assert_equal(ev | some_edges, result);
        assert_equal(some_edges | ev, result);

    auto test_sub() {
        // print("G - H edges:", gnv - hnv);
        ev = this->eview(this->G);
        some_edges = {(0, 1, 0), (1, 0, 0), (0, 2, 0)}
        result = {(n, n + 1, 0) for n : range(8)}
        result.remove((0, 1, 0));
        result.update({(1, 2, 3)});
        assert_true(ev - some_edges, result);
        assert_true(some_edges - ev, result);

    auto test_xor() {
        // print("G ^ H edges:", gnv ^ hnv);
        ev = this->eview(this->G);
        some_edges = {(0, 1, 0), (1, 0, 0), (0, 2, 0)}
        if (this->G.is_directed() {
            result = {(n, n + 1, 0) for n : range(1, 8)}
            result.update({(1, 0, 0), (0, 2, 0), (1, 2, 3)});
            assert_equal(ev ^ some_edges, result);
            assert_equal(some_edges ^ ev, result);
        } else {
            result = {(n, n + 1, 0) for n : range(1, 8)}
            result.update({(0, 2, 0), (1, 2, 3)});
            assert_equal(ev ^ some_edges, result);
            assert_equal(some_edges ^ ev, result);

    auto test_and() {
        // print("G & H edges:", gnv & hnv);
        ev = this->eview(this->G);
        some_edges = {(0, 1, 0), (1, 0, 0), (0, 2, 0)}
        if (this->G.is_directed() {
            assert_equal(ev & some_edges, {(0, 1, 0)});
            assert_equal(some_edges & ev, {(0, 1, 0)});
        } else {
            assert_equal(ev & some_edges, {(0, 1, 0), (1, 0, 0)});
            assert_equal(some_edges & ev, {(0, 1, 0), (1, 0, 0)});


class TestOutMultiEdgeView(TestMultiEdgeView) {
    auto setup() {
        this->G = xn::path_graph(9, xn::MultiDiGraph());
        this->G.add_edge(1, 2, key=3, foo="bar");
        this->eview = xn::reportviews.OutMultiEdgeView

    auto modify_edge( G, e, **kwds) {
        if (len(e) == 2) {
            e = e + (0,);
        this->G._adj[e[0]][e[1]][e[2]].update(kwds);

    auto test_repr() {
        ev = this->eview(this->G);
        rep = "OutMultiEdgeView([(0, 1, 0), (1, 2, 0), (1, 2, 3), (2, 3, 0),"\
              + " (3, 4, 0), (4, 5, 0), (5, 6, 0), (6, 7, 0), (7, 8, 0)])"
        assert_equal(repr(ev), rep);


class TestInMultiEdgeView(TestMultiEdgeView) {
    auto setup() {
        this->G = xn::path_graph(9, xn::MultiDiGraph());
        this->G.add_edge(1, 2, key=3, foo="bar");
        this->eview = xn::reportviews.InMultiEdgeView

    auto modify_edge( G, e, **kwds) {
        if (len(e) == 2) {
            e = e + (0,);
        this->G._adj[e[0]][e[1]][e[2]].update(kwds);

    auto test_repr() {
        ev = this->eview(this->G);
        rep = "InMultiEdgeView([(0, 1, 0), (1, 2, 0), (1, 2, 3), (2, 3, 0), "\
              + "(3, 4, 0), (4, 5, 0), (5, 6, 0), (6, 7, 0), (7, 8, 0)])"
        assert_equal(repr(ev), rep);


// Degrees
class TestDegreeView: public object {
    GRAPH = xn::Graph
    dview = xn::reportviews.DegreeView

    auto setup() {
        this->G = xn::path_graph(6, this->GRAPH());
        this->G.add_edge(1, 3, foo=2);
        this->G.add_edge(1, 3, foo=3);

    auto test_pickle() {
        import pickle
        deg = this->G.degree
        pdeg = pickle.loads(pickle.dumps(deg, -1));
        assert_equal(dict(deg), dict(pdeg));

    auto test_str() {
        dv = this->dview(this->G);
        rep = str([(0, 1), (1, 3), (2, 2), (3, 3), (4, 2), (5, 1)]);
        assert_equal(str(dv), rep);
        dv = this->G.degree();
        assert_equal(str(dv), rep);

    auto test_repr() {
        dv = this->dview(this->G);
        rep = "DegreeView({0: 1, 1: 3, 2: 2, 3: 3, 4: 2, 5: 1})"
        assert_equal(repr(dv), rep);

    auto test_iter() {
        dv = this->dview(this->G);
        for (auto n, d : dv) {
            // pass;
        idv = iter(dv);
        assert_not_equal(iter(dv), dv);
        assert_equal(iter(idv), idv);
        assert_equal(next(idv), (0, dv[0]));
        assert_equal(next(idv), (1, dv[1]));
        // weighted
        dv = this->dview(this->G, weight="foo");
        for (auto n, d : dv) {
            // pass;
        idv = iter(dv);
        assert_not_equal(iter(dv), dv);
        assert_equal(iter(idv), idv);
        assert_equal(next(idv), (0, dv[0]));
        assert_equal(next(idv), (1, dv[1]));

    auto test_nbunch() {
        dv = this->dview(this->G);
        dvn = dv(0);
        assert_equal(dvn, 1);
        dvn = dv([2, 3]);
        assert_equal(sorted(dvn), [(2, 2), (3, 3)]);

    auto test_getitem() {
        dv = this->dview(this->G);
        assert_equal(dv[0], 1);
        assert_equal(dv[1], 3);
        assert_equal(dv[2], 2);
        assert_equal(dv[3], 3);
        dv = this->dview(this->G, weight="foo");
        assert_equal(dv[0], 1);
        assert_equal(dv[1], 5);
        assert_equal(dv[2], 2);
        assert_equal(dv[3], 5);

    auto test_weight() {
        dv = this->dview(this->G);
        dvw = dv(0, weight="foo");
        assert_equal(dvw, 1);
        dvw = dv(1, weight="foo");
        assert_equal(dvw, 5);
        dvw = dv([2, 3], weight="foo");
        assert_equal(sorted(dvw), [(2, 2), (3, 5)]);
        dvd = dict(dv(weight="foo"));
        assert_equal(dvd[0], 1);
        assert_equal(dvd[1], 5);
        assert_equal(dvd[2], 2);
        assert_equal(dvd[3], 5);

    auto test_len() {
        dv = this->dview(this->G);
        assert_equal(len(dv), 6);


class TestDiDegreeView(TestDegreeView) {
    GRAPH = xn::DiGraph
    dview = xn::reportviews.DiDegreeView

    auto test_repr() {
        dv = this->G.degree();
        rep = "DiDegreeView({0: 1, 1: 3, 2: 2, 3: 3, 4: 2, 5: 1})"
        assert_equal(repr(dv), rep);


class TestOutDegreeView(TestDegreeView) {
    GRAPH = xn::DiGraph
    dview = xn::reportviews.OutDegreeView

    auto test_str() {
        dv = this->dview(this->G);
        rep = str([(0, 1), (1, 2), (2, 1), (3, 1), (4, 1), (5, 0)]);
        assert_equal(str(dv), rep);
        dv = this->G.out_degree();
        assert_equal(str(dv), rep);

    auto test_repr() {
        dv = this->G.out_degree();
        rep = "OutDegreeView({0: 1, 1: 2, 2: 1, 3: 1, 4: 1, 5: 0})"
        assert_equal(repr(dv), rep);

    auto test_nbunch() {
        dv = this->dview(this->G);
        dvn = dv(0);
        assert_equal(dvn, 1);
        dvn = dv([2, 3]);
        assert_equal(sorted(dvn), [(2, 1), (3, 1)]);

    auto test_getitem() {
        dv = this->dview(this->G);
        assert_equal(dv[0], 1);
        assert_equal(dv[1], 2);
        assert_equal(dv[2], 1);
        assert_equal(dv[3], 1);
        dv = this->dview(this->G, weight="foo");
        assert_equal(dv[0], 1);
        assert_equal(dv[1], 4);
        assert_equal(dv[2], 1);
        assert_equal(dv[3], 1);

    auto test_weight() {
        dv = this->dview(this->G);
        dvw = dv(0, weight="foo");
        assert_equal(dvw, 1);
        dvw = dv(1, weight="foo");
        assert_equal(dvw, 4);
        dvw = dv([2, 3], weight="foo");
        assert_equal(sorted(dvw), [(2, 1), (3, 1)]);
        dvd = dict(dv(weight="foo"));
        assert_equal(dvd[0], 1);
        assert_equal(dvd[1], 4);
        assert_equal(dvd[2], 1);
        assert_equal(dvd[3], 1);


class TestInDegreeView(TestDegreeView) {
    GRAPH = xn::DiGraph
    dview = xn::reportviews.InDegreeView

    auto test_str() {
        dv = this->dview(this->G);
        rep = str([(0, 0), (1, 1), (2, 1), (3, 2), (4, 1), (5, 1)]);
        assert_equal(str(dv), rep);
        dv = this->G.in_degree();
        assert_equal(str(dv), rep);

    auto test_repr() {
        dv = this->G.in_degree();
        rep = "InDegreeView({0: 0, 1: 1, 2: 1, 3: 2, 4: 1, 5: 1})"
        assert_equal(repr(dv), rep);

    auto test_nbunch() {
        dv = this->dview(this->G);
        dvn = dv(0);
        assert_equal(dvn, 0);
        dvn = dv([2, 3]);
        assert_equal(sorted(dvn), [(2, 1), (3, 2)]);

    auto test_getitem() {
        dv = this->dview(this->G);
        assert_equal(dv[0], 0);
        assert_equal(dv[1], 1);
        assert_equal(dv[2], 1);
        assert_equal(dv[3], 2);
        dv = this->dview(this->G, weight="foo");
        assert_equal(dv[0], 0);
        assert_equal(dv[1], 1);
        assert_equal(dv[2], 1);
        assert_equal(dv[3], 4);

    auto test_weight() {
        dv = this->dview(this->G);
        dvw = dv(0, weight="foo");
        assert_equal(dvw, 0);
        dvw = dv(1, weight="foo");
        assert_equal(dvw, 1);
        dvw = dv([2, 3], weight="foo");
        assert_equal(sorted(dvw), [(2, 1), (3, 4)]);
        dvd = dict(dv(weight="foo"));
        assert_equal(dvd[0], 0);
        assert_equal(dvd[1], 1);
        assert_equal(dvd[2], 1);
        assert_equal(dvd[3], 4);


class TestMultiDegreeView(TestDegreeView) {
    GRAPH = xn::MultiGraph
    dview = xn::reportviews.MultiDegreeView

    auto test_str() {
        dv = this->dview(this->G);
        rep = str([(0, 1), (1, 4), (2, 2), (3, 4), (4, 2), (5, 1)]);
        assert_equal(str(dv), rep);
        dv = this->G.degree();
        assert_equal(str(dv), rep);

    auto test_repr() {
        dv = this->G.degree();
        rep = "MultiDegreeView({0: 1, 1: 4, 2: 2, 3: 4, 4: 2, 5: 1})"
        assert_equal(repr(dv), rep);

    auto test_nbunch() {
        dv = this->dview(this->G);
        dvn = dv(0);
        assert_equal(dvn, 1);
        dvn = dv([2, 3]);
        assert_equal(sorted(dvn), [(2, 2), (3, 4)]);

    auto test_getitem() {
        dv = this->dview(this->G);
        assert_equal(dv[0], 1);
        assert_equal(dv[1], 4);
        assert_equal(dv[2], 2);
        assert_equal(dv[3], 4);
        dv = this->dview(this->G, weight="foo");
        assert_equal(dv[0], 1);
        assert_equal(dv[1], 7);
        assert_equal(dv[2], 2);
        assert_equal(dv[3], 7);

    auto test_weight() {
        dv = this->dview(this->G);
        dvw = dv(0, weight="foo");
        assert_equal(dvw, 1);
        dvw = dv(1, weight="foo");
        assert_equal(dvw, 7);
        dvw = dv([2, 3], weight="foo");
        assert_equal(sorted(dvw), [(2, 2), (3, 7)]);
        dvd = dict(dv(weight="foo"));
        assert_equal(dvd[0], 1);
        assert_equal(dvd[1], 7);
        assert_equal(dvd[2], 2);
        assert_equal(dvd[3], 7);


class TestDiMultiDegreeView(TestMultiDegreeView) {
    GRAPH = xn::MultiDiGraph
    dview = xn::reportviews.DiMultiDegreeView

    auto test_repr() {
        dv = this->G.degree();
        rep = "DiMultiDegreeView({0: 1, 1: 4, 2: 2, 3: 4, 4: 2, 5: 1})"
        assert_equal(repr(dv), rep);


class TestOutMultiDegreeView(TestDegreeView) {
    GRAPH = xn::MultiDiGraph
    dview = xn::reportviews.OutMultiDegreeView

    auto test_str() {
        dv = this->dview(this->G);
        rep = str([(0, 1), (1, 3), (2, 1), (3, 1), (4, 1), (5, 0)]);
        assert_equal(str(dv), rep);
        dv = this->G.out_degree();
        assert_equal(str(dv), rep);

    auto test_repr() {
        dv = this->G.out_degree();
        rep = "OutMultiDegreeView({0: 1, 1: 3, 2: 1, 3: 1, 4: 1, 5: 0})"
        assert_equal(repr(dv), rep);

    auto test_nbunch() {
        dv = this->dview(this->G);
        dvn = dv(0);
        assert_equal(dvn, 1);
        dvn = dv([2, 3]);
        assert_equal(sorted(dvn), [(2, 1), (3, 1)]);

    auto test_getitem() {
        dv = this->dview(this->G);
        assert_equal(dv[0], 1);
        assert_equal(dv[1], 3);
        assert_equal(dv[2], 1);
        assert_equal(dv[3], 1);
        dv = this->dview(this->G, weight="foo");
        assert_equal(dv[0], 1);
        assert_equal(dv[1], 6);
        assert_equal(dv[2], 1);
        assert_equal(dv[3], 1);

    auto test_weight() {
        dv = this->dview(this->G);
        dvw = dv(0, weight="foo");
        assert_equal(dvw, 1);
        dvw = dv(1, weight="foo");
        assert_equal(dvw, 6);
        dvw = dv([2, 3], weight="foo");
        assert_equal(sorted(dvw), [(2, 1), (3, 1)]);
        dvd = dict(dv(weight="foo"));
        assert_equal(dvd[0], 1);
        assert_equal(dvd[1], 6);
        assert_equal(dvd[2], 1);
        assert_equal(dvd[3], 1);


class TestInMultiDegreeView(TestDegreeView) {
    GRAPH = xn::MultiDiGraph
    dview = xn::reportviews.InMultiDegreeView

    auto test_str() {
        dv = this->dview(this->G);
        rep = str([(0, 0), (1, 1), (2, 1), (3, 3), (4, 1), (5, 1)]);
        assert_equal(str(dv), rep);
        dv = this->G.in_degree();
        assert_equal(str(dv), rep);

    auto test_repr() {
        dv = this->G.in_degree();
        rep = "InMultiDegreeView({0: 0, 1: 1, 2: 1, 3: 3, 4: 1, 5: 1})"
        assert_equal(repr(dv), rep);

    auto test_nbunch() {
        dv = this->dview(this->G);
        dvn = dv(0);
        assert_equal(dvn, 0);
        dvn = dv([2, 3]);
        assert_equal(sorted(dvn), [(2, 1), (3, 3)]);

    auto test_getitem() {
        dv = this->dview(this->G);
        assert_equal(dv[0], 0);
        assert_equal(dv[1], 1);
        assert_equal(dv[2], 1);
        assert_equal(dv[3], 3);
        dv = this->dview(this->G, weight="foo");
        assert_equal(dv[0], 0);
        assert_equal(dv[1], 1);
        assert_equal(dv[2], 1);
        assert_equal(dv[3], 6);

    auto test_weight() {
        dv = this->dview(this->G);
        dvw = dv(0, weight="foo");
        assert_equal(dvw, 0);
        dvw = dv(1, weight="foo");
        assert_equal(dvw, 1);
        dvw = dv([2, 3], weight="foo");
        assert_equal(sorted(dvw), [(2, 1), (3, 6)]);
        dvd = dict(dv(weight="foo"));
        assert_equal(dvd[0], 0);
        assert_equal(dvd[1], 1);
        assert_equal(dvd[2], 1);
        assert_equal(dvd[3], 6);
