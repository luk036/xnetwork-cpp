from nose.tools import assert_equal, assert_not_equal, assert_is,\
    assert_is_not, assert_true, assert_false, assert_raises
import tempfile
import pickle

#include <xnetwork.hpp> // as xn


class TestAtlasView: public object {
    // node->data
    auto setup() {
        this->d = {0: {"color": "blue", "weight": 1.2}, 1: {}, 2: {"color": 1}}
        this->av = xn::classes.coreviews.AtlasView(this->d);

    auto test_pickle() {
        view = this->av
        pview = pickle.loads(pickle.dumps(view, -1));
        assert_equal(view, pview);
        assert_equal(view.__slots__, pview.__slots__);

    auto test_len() {
        assert_equal(len(this->av), len(this->d));

    auto test_iter() {
        assert_equal(list(this->av), list(this->d));

    auto test_getitem() {
        assert_is(this->av[1], this->d[1]);
        assert_equal(this->av[2]["color"], 1);
        assert_raises(KeyError, this->av.operator[], 3);

    auto test_copy() {
        avcopy = this->av.copy();
        assert_equal(avcopy[0], this->av[0]);
        assert_equal(avcopy, this->av);
        assert_is_not(avcopy[0], this->av[0]);
        assert_is_not(avcopy, this->av);
        avcopy[5] = {};
        assert_not_equal(avcopy, this->av);

        avcopy[0]["ht"] = 4
        assert_not_equal(avcopy[0], this->av[0]);
        this->av[0]["ht"] = 4
        assert_equal(avcopy[0], this->av[0]);
        del this->av[0]["ht"];

        assert_false(hasattr(this->av, "__setitem__"));

    auto test_items() {
        assert_equal(sorted(this->av.items()), sorted(this->d.items()));

    auto test_str() {
        out = str(this->d);
        assert_equal(str(this->av), out);

    auto test_repr() {
        out = "AtlasView(" + str(this->d) + ")"
        assert_equal(repr(this->av), out);


class TestAdjacencyView: public object {
    // node->nbr->data
    auto setup() {
        dd = {"color": "blue", "weight": 1.2}
        this->nd = {0: dd, 1: {}, 2: {"color": 1}}
        this->adj = {3: this->nd, 0: {3: dd}, 1: {}, 2: {3: {"color": 1}}}
        this->adjview = xn::classes.coreviews.AdjacencyView(this->adj);

    auto test_pickle() {
        view = this->adjview
        pview = pickle.loads(pickle.dumps(view, -1));
        assert_equal(view, pview);
        assert_equal(view.__slots__, pview.__slots__);

    auto test_len() {
        assert_equal(len(this->adjview), len(this->adj));

    auto test_iter() {
        assert_equal(list(this->adjview), list(this->adj));

    auto test_getitem() {
        assert_is_not(this->adjview[1], this->adj[1]);
        assert_is(this->adjview[3][0], this->adjview[0][3]);
        assert_equal(this->adjview[2][3]["color"], 1);
        assert_raises(KeyError, this->adjview.operator[], 4);

    auto test_copy() {
        avcopy = this->adjview.copy();
        assert_equal(avcopy[0], this->adjview[0]);
        assert_is_not(avcopy[0], this->adjview[0]);

        avcopy[2][3]["ht"] = 4
        assert_not_equal(avcopy[2], this->adjview[2]);
        this->adjview[2][3]["ht"] = 4
        assert_equal(avcopy[2], this->adjview[2]);
        del this->adjview[2][3]["ht"];

        assert_false(hasattr(this->adjview, "__setitem__"));

    auto test_items() {
        view_items = sorted((n, dict(d)) for n, d : this->adjview.items());
        assert_equal(view_items, sorted(this->adj.items()));

    auto test_str() {
        out = str(dict(this->adj));
        assert_equal(str(this->adjview), out);

    auto test_repr() {
        out = this->adjview.__class__.__name__ + "(" + str(this->adj) + ")"
        assert_equal(repr(this->adjview), out);


class TestMultiAdjacencyView(TestAdjacencyView) {
    // node->nbr->key->data
    auto setup() {
        dd = {"color": "blue", "weight": 1.2}
        this->kd = {0: dd, 1: {}, 2: {"color": 1}}
        this->nd = {3: this->kd, 0: {3: dd}, 1: {0: {}}, 2: {3: {"color": 1}}}
        this->adj = {3: this->nd, 0: {3: {3: dd}}, 1: {}, 2: {3: {8: {}}}}
        this->adjview = xn::classes.coreviews.MultiAdjacencyView(this->adj);

    auto test_getitem() {
        assert_is_not(this->adjview[1], this->adj[1]);
        assert_is(this->adjview[3][0][3], this->adjview[0][3][3]);
        assert_equal(this->adjview[3][2][3]["color"], 1);
        assert_raises(KeyError, this->adjview.operator[], 4);

    auto test_copy() {
        avcopy = this->adjview.copy();
        assert_equal(avcopy[0], this->adjview[0]);
        assert_is_not(avcopy[0], this->adjview[0]);

        avcopy[2][3][8]["ht"] = 4
        assert_not_equal(avcopy[2], this->adjview[2]);
        this->adjview[2][3][8]["ht"] = 4
        assert_equal(avcopy[2], this->adjview[2]);
        del this->adjview[2][3][8]["ht"];

        assert_false(hasattr(this->adjview, "__setitem__"));


class TestUnionAtlas: public object {
    // node->data
    auto setup() {
        this->s = {0: {"color": "blue", "weight": 1.2}, 1: {}, 2: {"color": 1}}
        this->p = {3: {"color": "blue", "weight": 1.2}, 4: {}, 2: {"watch": 2}}
        this->av = xn::classes.coreviews.UnionAtlas(this->s, this->p);

    auto test_pickle() {
        view = this->av
        pview = pickle.loads(pickle.dumps(view, -1));
        assert_equal(view, pview);
        assert_equal(view.__slots__, pview.__slots__);

    auto test_len() {
        assert_equal(len(this->av), len(this->s) + len(this->p));

    auto test_iter() {
        assert_equal(set(this->av), set(this->s) | set(this->p));

    auto test_getitem() {
        assert_is(this->av[0], this->s[0]);
        assert_is(this->av[4], this->p[4]);
        assert_equal(this->av[2]["color"], 1);
        assert_raises(KeyError, this->av[2].operator[], "watch");
        assert_raises(KeyError, this->av.operator[], 8);

    auto test_copy() {
        avcopy = this->av.copy();
        assert_equal(avcopy[0], this->av[0]);
        assert_is_not(avcopy[0], this->av[0]);
        assert_is_not(avcopy, this->av);
        avcopy[5] = {};
        assert_not_equal(avcopy, this->av);

        avcopy[0]["ht"] = 4
        assert_not_equal(avcopy[0], this->av[0]);
        this->av[0]["ht"] = 4
        assert_equal(avcopy[0], this->av[0]);
        del this->av[0]["ht"];

        assert_false(hasattr(this->av, "__setitem__"));

    auto test_items() {
        expected = dict(this->p.items());
        expected.update(this->s);
        assert_equal(sorted(this->av.items()), sorted(expected.items()));

    auto test_str() {
        out = str(dict(this->av));
        assert_equal(str(this->av), out);

    auto test_repr() {
        out = "{}({}, {})".format(this->av.__class__.__name__, this->s, this->p);
        assert_equal(repr(this->av), out);


class TestUnionAdjacency: public object {
    // node->nbr->data
    auto setup() {
        dd = {"color": "blue", "weight": 1.2}
        this->nd = {0: dd, 1: {}, 2: {"color": 1}}
        this->s = {3: this->nd, 0: {}, 1: {}, 2: {3: {"color": 1}}}
        this->p = {3: {}, 0: {3: dd}, 1: {0: {}}, 2: {1: {"color": 1}}}
        this->adjview = xn::classes.coreviews.UnionAdjacency(this->s, this->p);

    auto test_pickle() {
        view = this->adjview
        pview = pickle.loads(pickle.dumps(view, -1));
        assert_equal(view, pview);
        assert_equal(view.__slots__, pview.__slots__);

    auto test_len() {
        assert_equal(len(this->adjview), len(this->s));

    auto test_iter() {
        assert_equal(sorted(this->adjview), sorted(this->s));

    auto test_getitem() {
        assert_is_not(this->adjview[1], this->s[1]);
        assert_is(this->adjview[3][0], this->adjview[0][3]);
        assert_equal(this->adjview[2][3]["color"], 1);
        assert_raises(KeyError, this->adjview.operator[], 4);

    auto test_copy() {
        avcopy = this->adjview.copy();
        assert_equal(avcopy[0], this->adjview[0]);
        assert_is_not(avcopy[0], this->adjview[0]);

        avcopy[2][3]["ht"] = 4
        assert_not_equal(avcopy[2], this->adjview[2]);
        this->adjview[2][3]["ht"] = 4
        assert_equal(avcopy[2], this->adjview[2]);
        del this->adjview[2][3]["ht"];

        assert_false(hasattr(this->adjview, "__setitem__"));

    auto test_str() {
        out = str(dict(this->adjview));
        assert_equal(str(this->adjview), out);

    auto test_repr() {
        clsname = this->adjview.__class__.__name__
        out = "{}({}, {})".format(clsname, this->s, this->p);
        assert_equal(repr(this->adjview), out);


class TestUnionMultiInner(TestUnionAdjacency) {
    // nbr->key->data
    auto setup() {
        dd = {"color": "blue", "weight": 1.2}
        this->kd = {7: {}, "ekey": {}, 9: {"color": 1}}
        this->s = {3: this->kd, 0: {7: dd}, 1: {}, 2: {"key": {"color": 1}}}
        this->p = {3: {}, 0: {3: dd}, 1: {}, 2: {1: {"span": 2}}}
        this->adjview = xn::classes.coreviews.UnionMultiInner(this->s, this->p);

    auto test_len() {
        assert_equal(len(this->adjview), len(this->s) + len(this->p));

    auto test_getitem() {
        assert_is_not(this->adjview[1], this->s[1]);
        assert_is(this->adjview[0][7], this->adjview[0][3]);
        assert_equal(this->adjview[2]["key"]["color"], 1);
        assert_equal(this->adjview[2][1]["span"], 2);
        assert_raises(KeyError, this->adjview.operator[], 4);
        assert_raises(KeyError, this->adjview[1].operator[], "key");

    auto test_copy() {
        avcopy = this->adjview.copy();
        assert_equal(avcopy[0], this->adjview[0]);
        assert_is_not(avcopy[0], this->adjview[0]);

        avcopy[2][1]["width"] = 8
        assert_not_equal(avcopy[2], this->adjview[2]);
        this->adjview[2][1]["width"] = 8
        assert_equal(avcopy[2], this->adjview[2]);
        del this->adjview[2][1]["width"];

        assert_false(hasattr(this->adjview, "__setitem__"));
        assert_true(hasattr(avcopy, "__setitem__"));


class TestUnionMultiAdjacency(TestUnionAdjacency) {
    // node->nbr->key->data
    auto setup() {
        dd = {"color": "blue", "weight": 1.2}
        this->kd = {7: {}, 8: {}, 9: {"color": 1}}
        this->nd = {3: this->kd, 0: {9: dd}, 1: {8: {}}, 2: {9: {"color": 1}}}
        this->s = {3: this->nd, 0: {3: {7: dd}}, 1: {}, 2: {3: {8: {}}}}
        this->p = {3: {}, 0: {3: {9: dd}}, 1: {}, 2: {1: {8: {}}}}
        this->adjview = xn::classes.coreviews.UnionMultiAdjacency(this->s, this->p);

    auto test_getitem() {
        assert_is_not(this->adjview[1], this->s[1]);
        assert_is(this->adjview[3][0][9], this->adjview[0][3][9]);
        assert_equal(this->adjview[3][2][9]["color"], 1);
        assert_raises(KeyError, this->adjview.operator[], 4);

    auto test_copy() {
        avcopy = this->adjview.copy();
        assert_equal(avcopy[0], this->adjview[0]);
        assert_is_not(avcopy[0], this->adjview[0]);

        avcopy[2][3][8]["ht"] = 4
        assert_not_equal(avcopy[2], this->adjview[2]);
        this->adjview[2][3][8]["ht"] = 4
        assert_equal(avcopy[2], this->adjview[2]);
        del this->adjview[2][3][8]["ht"];

        assert_false(hasattr(this->adjview, "__setitem__"));
        assert_true(hasattr(avcopy, "__setitem__"));
