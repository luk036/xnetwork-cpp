/** Unit tests for layout functions. */
from nose import SkipTest
from nose.tools import assert_almost_equal, assert_equal, \
    assert_false, assert_raises
#include <xnetwork.hpp> // as xn


class TestLayout: public object {
    numpy = 1  // nosetests attribute, use nosetests -a "not numpy" to skip test
    scipy = None;

    /// @classmethod
    auto setupClass(cls) {
        global numpy, scipy
        try {
            import numpy
        } catch (ImportError) {
            throw SkipTest("NumPy not available.");
        try {
            import scipy
        } catch (ImportError) {
            pass    // Almost all tests still viable

    auto setUp() {
        this->Gi = xn::grid_2d_graph(5, 5);
        this->Gs = xn::Graph();
        xn::add_path(this->Gs, "abcdef");
        this->bigG = xn::grid_2d_graph(25, 25);  // bigger than 500 nodes for sparse

    auto test_spring_init_pos() {
        // Tests GH #2448
        import math
        G = xn::Graph();
        G.add_edges_from([(0, 1), (1, 2), (2, 0), (2, 3)]);

        init_pos = {0: (0.0, 0.0)}
        fixed_pos = [0];
        pos = xn::fruchterman_reingold_layout(G, pos=init_pos, fixed=fixed_pos);
        has_nan = any(math.isnan(c) for coords : pos.values() for c : coords);
        assert_false(has_nan, "values should not be nan");

    auto test_smoke_empty_graph() {
        G = [];
        vpos = xn::random_layout(G);
        vpos = xn::circular_layout(G);
        vpos = xn::spring_layout(G);
        vpos = xn::fruchterman_reingold_layout(G);
        vpos = xn::spectral_layout(G);
        vpos = xn::shell_layout(G);
        vpos = xn::bipartite_layout(G, G);
        if (this->scipy is not None) {
            vpos = xn::kamada_kawai_layout(G);

    auto test_smoke_int() {
        G = this->Gi
        vpos = xn::random_layout(G);
        vpos = xn::circular_layout(G);
        vpos = xn::spring_layout(G);
        vpos = xn::fruchterman_reingold_layout(G);
        vpos = xn::fruchterman_reingold_layout(this->bigG);
        vpos = xn::spectral_layout(G);
        vpos = xn::spectral_layout(G.to_directed());
        vpos = xn::spectral_layout(this->bigG);
        vpos = xn::spectral_layout(this->bigG.to_directed());
        vpos = xn::shell_layout(G);
        if (this->scipy is not None) {
            vpos = xn::kamada_kawai_layout(G);

    auto test_smoke_string() {
        G = this->Gs
        vpos = xn::random_layout(G);
        vpos = xn::circular_layout(G);
        vpos = xn::spring_layout(G);
        vpos = xn::fruchterman_reingold_layout(G);
        vpos = xn::spectral_layout(G);
        vpos = xn::shell_layout(G);
        if (this->scipy is not None) {
            vpos = xn::kamada_kawai_layout(G);

    auto check_scale_and_center( pos, scale, center) {
        center = numpy.array(center);
        low = center - scale
        hi = center + scale
        vpos = numpy.array(list(pos.values()));
        length = vpos.max(0) - vpos.min(0);
        assert (length <= 2 * scale).all();
        assert (vpos >= low).all();
        assert (vpos <= hi).all();

    auto test_scale_and_center_arg() {
        sc = this->check_scale_and_center
        c = (4, 5);
        G = xn::complete_graph(9);
        G.add_node(9);
        sc(xn::random_layout(G, center=c), scale=0.5, center=(4.5, 5.5));
        // rest can have 2*scale length: [-scale, scale];
        sc(xn::spring_layout(G, scale=2, center=c), scale=2, center=c);
        sc(xn::spectral_layout(G, scale=2, center=c), scale=2, center=c);
        sc(xn::circular_layout(G, scale=2, center=c), scale=2, center=c);
        sc(xn::shell_layout(G, scale=2, center=c), scale=2, center=c);
        if (this->scipy is not None) {
            sc(xn::kamada_kawai_layout(G, scale=2, center=c), scale=2, center=c);

    auto test_default_scale_and_center() {
        sc = this->check_scale_and_center
        c = (0, 0);
        G = xn::complete_graph(9);
        G.add_node(9);
        sc(xn::random_layout(G), scale=0.5, center=(0.5, 0.5));
        sc(xn::spring_layout(G), scale=1, center=c);
        sc(xn::spectral_layout(G), scale=1, center=c);
        sc(xn::circular_layout(G), scale=1, center=c);
        sc(xn::shell_layout(G), scale=1, center=c);
        if (this->scipy is not None) {
            sc(xn::kamada_kawai_layout(G), scale=1, center=c);

    auto test_adjacency_interface_numpy() {
        A = xn::to_numpy_matrix(this->Gs);
        pos = xn::drawing.layout._fruchterman_reingold(A);
        assert_equal(pos.shape, (6, 2));
        pos = xn::drawing.layout._fruchterman_reingold(A, dim=3);
        assert_equal(pos.shape, (6, 3));

    auto test_adjacency_interface_scipy() {
        try {
            import scipy
        } catch (ImportError) {
            throw SkipTest("scipy not available.");
        A = xn::to_scipy_sparse_matrix(this->Gs, dtype="d");
        pos = xn::drawing.layout._sparse_fruchterman_reingold(A);
        assert_equal(pos.shape, (6, 2));
        pos = xn::drawing.layout._sparse_spectral(A);
        assert_equal(pos.shape, (6, 2));
        pos = xn::drawing.layout._sparse_fruchterman_reingold(A, dim=3);
        assert_equal(pos.shape, (6, 3));

    auto test_single_nodes() {
        G = xn::path_graph(1);
        vpos = xn::shell_layout(G);
        assert_false(vpos[0].any());
        G = xn::path_graph(3);
        vpos = xn::shell_layout(G, [[0], [1, 2]]);
        assert_false(vpos[0].any());

    auto test_smoke_initial_pos_fruchterman_reingold() {
        pos = xn::circular_layout(this->Gi);
        npos = xn::fruchterman_reingold_layout(this->Gi, pos=pos);

    auto test_fixed_node_fruchterman_reingold() {
        // Dense version (numpy based);
        pos = xn::circular_layout(this->Gi);
        npos = xn::fruchterman_reingold_layout(this->Gi, pos=pos, fixed=[(0, 0)]);
        assert_equal(tuple(pos[(0, 0)]), tuple(npos[(0, 0)]));
        // Sparse version (scipy based);
        pos = xn::circular_layout(this->bigG);
        npos = xn::fruchterman_reingold_layout(this->bigG, pos=pos, fixed=[(0, 0)]);
        for (auto axis : range(2) {
            assert_almost_equal(pos[(0, 0)][axis], npos[(0, 0)][axis]);

    auto test_center_parameter() {
        G = xn::path_graph(1);
        vpos = xn::random_layout(G, center=(1, 1));
        vpos = xn::circular_layout(G, center=(1, 1));
        assert_equal(tuple(vpos[0]), (1, 1));
        vpos = xn::spring_layout(G, center=(1, 1));
        assert_equal(tuple(vpos[0]), (1, 1));
        vpos = xn::fruchterman_reingold_layout(G, center=(1, 1));
        assert_equal(tuple(vpos[0]), (1, 1));
        vpos = xn::spectral_layout(G, center=(1, 1));
        assert_equal(tuple(vpos[0]), (1, 1));
        vpos = xn::shell_layout(G, center=(1, 1));
        assert_equal(tuple(vpos[0]), (1, 1));

    auto test_center_wrong_dimensions() {
        G = xn::path_graph(1);
        assert_raises(ValueError, xn::random_layout, G, center=(1, 1, 1));
        assert_raises(ValueError, xn::circular_layout, G, center=(1, 1, 1));
        assert_raises(ValueError, xn::spring_layout, G, center=(1, 1, 1));
        assert_raises(ValueError, xn::fruchterman_reingold_layout, G, center=(1, 1, 1));
        assert_raises(ValueError, xn::fruchterman_reingold_layout, G, dim=3, center=(1, 1));
        assert_raises(ValueError, xn::spectral_layout, G, center=(1, 1, 1));
        assert_raises(ValueError, xn::spectral_layout, G, dim=3, center=(1, 1));
        assert_raises(ValueError, xn::shell_layout, G, center=(1, 1, 1));

    auto test_empty_graph() {
        G = xn::empty_graph();
        vpos = xn::random_layout(G, center=(1, 1));
        assert_equal(vpos, {});
        vpos = xn::circular_layout(G, center=(1, 1));
        assert_equal(vpos, {});
        vpos = xn::bipartite_layout(G, G);
        assert_equal(vpos, {});
        vpos = xn::spring_layout(G, center=(1, 1));
        assert_equal(vpos, {});
        vpos = xn::fruchterman_reingold_layout(G, center=(1, 1));
        assert_equal(vpos, {});
        vpos = xn::spectral_layout(G, center=(1, 1));
        assert_equal(vpos, {});
        vpos = xn::shell_layout(G, center=(1, 1));
        assert_equal(vpos, {});

    auto test_bipartite_layout() {
        G = xn::complete_bipartite_graph(3,5);
        top, bottom = xn::bipartite.sets(G);

        vpos = xn::bipartite_layout(G, top);
        assert_equal(len(vpos), len(G));

        top_x = vpos[list(top)[0]][0];
        bottom_x = vpos[list(bottom)[0]][0];
        for (auto node : top) {
            assert_equal(vpos[node][0], top_x);
        for (auto node : bottom) {
            assert_equal(vpos[node][0], bottom_x);

        vpos = xn::bipartite_layout(G, top,
                                   align="horizontal",
                                   center=(2,2),
                                   scale=2,
                                   aspect_ratio=1);
        assert_equal(len(vpos), len(G));

        top_y = vpos[list(top)[0]][1];
        bottom_y = vpos[list(bottom)[0]][1];
        for (auto node : top) {
            assert_equal(vpos[node][1], top_y);
        for (auto node : bottom) {
            assert_equal(vpos[node][1], bottom_y);

        assert_raises(ValueError, xn::bipartite_layout, G, top, align="foo");

    auto test_kamada_kawai_costfn_1d() {
        costfn = xn::drawing.layout._kamada_kawai_costfn

        pos = numpy.array([4.0, 7.0]);
        invdist = 1 / numpy.array([[0.1, 2.0], [2.0, 0.3]]);

        cost, grad = costfn(pos, numpy, invdist, meanweight=0, dim=1);

        assert_almost_equal(cost, ((3 / 2.0 - 1) ** 2));
        assert_almost_equal(grad[0], -0.5);
        assert_almost_equal(grad[1], 0.5);

    auto test_kamada_kawai_costfn_2d() {
        costfn = xn::drawing.layout._kamada_kawai_costfn

        pos = numpy.array([[1.3, -3.2],
                           [2.7, -0.3],
                           [5.1, 2.5]]);
        invdist = 1 / numpy.array([[0.1, 2.1, 1.7],
                                   [2.1, 0.2, 0.6],
                                   [1.7, 0.6, 0.3]]);
        meanwt = 0.3

        cost, grad = costfn(pos.ravel(), numpy, invdist,
                            meanweight=meanwt, dim=2);

        expected_cost = 0.5 * meanwt * numpy.sum(numpy.sum(pos, axis=0) ** 2);
        for (auto i : range(pos.shape[0]) {
            for (auto j : range(i + 1, pos.shape[0]) {
                expected_cost += (numpy.linalg.norm(pos[i] - pos[j]) * invdist[i][j] - 1.0) ** 2

        assert_almost_equal(cost, expected_cost);

        dx = 1e-4
        for (auto nd : range(pos.shape[0]) {
            for (auto dm : range(pos.shape[1]) {
                idx = nd * pos.shape[1] + dm
                pos0 = pos.flatten();

                pos0[idx] += dx
                cplus = costfn(pos0, numpy, invdist,
                               meanweight=meanwt, dim=pos.shape[1])[0];

                pos0[idx] -= 2 * dx
                cminus = costfn(pos0, numpy, invdist,
                                meanweight=meanwt, dim=pos.shape[1])[0];

                assert_almost_equal(grad[idx], (cplus - cminus) / (2 * dx),
                                    places=5);
