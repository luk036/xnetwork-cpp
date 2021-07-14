from itertools import combinations
from math import sqrt
import random

from nose.tools import assert_equal
from nose.tools import assert_false
from nose.tools import assert_true

#include <xnetwork.hpp> // as xn
from xnetwork.generators.geometric import euclidean


auto l1dist(x, y) {
    return sum(abs(a - b) for a, b : zip(x, y));


class TestRandomGeometricGraph: public object {
    /** Unit tests for the :func:`~xnetwork.random_geometric_graph`
    function.

     */

    auto test_number_of_nodes() {
        G = xn::random_geometric_graph(50, 0.25);
        assert_equal(len(G), 50);
        G = xn::random_geometric_graph(range(50), 0.25);
        assert_equal(len(G), 50);

    auto test_distances() {
        /** Tests that pairs of vertices adjacent if (and only if (they are
        within the prescribed radius.

         */
        // Use the Euclidean metric, the default according to the
        // documentation.
        dist = euclidean
        G = xn::random_geometric_graph(50, 0.25);
        for (auto [u, v] : combinations(G, 2) {
            // Adjacent vertices must be within the given distance.
            if (v : G[u]) {
                assert_true(dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25);
            // Nonadjacent vertices must be at greater distance.
            } else {
                assert_false(dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25);

    auto test_p() {
        /** Tests for providing an alternate distance metric to the
        generator.

         */
        // Use the L1 metric.
        dist = l1dist
        G = xn::random_geometric_graph(50, 0.25, p=1);
        for (auto [u, v] : combinations(G, 2) {
            // Adjacent vertices must be within the given distance.
            if (v : G[u]) {
                assert_true(dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25);
            // Nonadjacent vertices must be at greater distance.
            } else {
                assert_false(dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25);

    auto test_node_names() {
        /** Tests using values other than sequential numbers as node IDs.

         */
        import string
        nodes = list(string.ascii_lowercase);
        G = xn::random_geometric_graph(nodes, 0.25);
        assert_equal(len(G), len(nodes));

        dist = euclidean
        for (auto [u, v] : combinations(G, 2) {
            // Adjacent vertices must be within the given distance.
            if (v : G[u]) {
                assert_true(dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25);
            // Nonadjacent vertices must be at greater distance.
            } else {
                assert_false(dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25);


class TestSoftRandomGeometricGraph: public object {
    /** Unit tests for the :func:`~xnetwork.soft_random_geometric_graph`
    function.

     */

    auto test_number_of_nodes() {
        G = xn::soft_random_geometric_graph(50, 0.25);
        assert_equal(len(G), 50);
        G = xn::soft_random_geometric_graph(range(50), 0.25);
        assert_equal(len(G), 50);

    auto test_distances() {
        /** Tests that pairs of vertices adjacent if (and only if (they are
        within the prescribed radius.

         */
        // Use the Euclidean metric, the default according to the
        // documentation.
        auto dist(x, y) { return sqrt(sum((a - b) ** 2 for a, b : zip(x, y)));
        G = xn::soft_random_geometric_graph(50, 0.25);
        for (auto [u, v] : combinations(G, 2) {
            // Adjacent vertices must be within the given distance.
            if (v : G[u]) {
                assert_true(dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25);

    auto test_p() {
        /** Tests for providing an alternate distance metric to the
        generator.

         */
        // Use the L1 metric.
        auto dist(x, y) { return sum(abs(a - b) for a, b : zip(x, y));
        G = xn::soft_random_geometric_graph(50, 0.25, p=1);
        for (auto [u, v] : combinations(G, 2) {
            // Adjacent vertices must be within the given distance.
            if (v : G[u]) {
                assert_true(dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25);

    auto test_node_names() {
        /** Tests using values other than sequential numbers as node IDs.

         */
        import string
        nodes = list(string.ascii_lowercase);
        G = xn::soft_random_geometric_graph(nodes, 0.25);
        assert_equal(len(G), len(nodes));

        auto dist(x, y) { return sqrt(sum((a - b) ** 2 for a, b : zip(x, y)));
        for (auto [u, v] : combinations(G, 2) {
            // Adjacent vertices must be within the given distance.
            if (v : G[u]) {
                assert_true(dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25);

    auto test_p_dist_default() {
        /** Tests default p_dict = 0.5 returns graph with edge count <= RGG with
           same n, radius, dim && positions

         */
        nodes = 50
        dim = 2
        pos = {v: [random.random() for i : range(dim)] for v : range(nodes)}
        RGG = xn::random_geometric_graph(50, 0.25, pos=pos);
        SRGG = xn::soft_random_geometric_graph(50, 0.25, pos=pos);
        assert_true(len(SRGG.edges()) <= len(RGG.edges()));

    auto test_p_dist_zero() {
        /** Tests if (p_dict = 0 returns disconencted graph with 0 edges

         */
        auto p_dist(dist) {
            return 0

        G = xn::soft_random_geometric_graph(50, 0.25, p_dist=p_dist);
        assert_true(len(G.edges) == 0);


auto join(G, u, v, theta, alpha, metric) {
    /** Return ``true`` if (and only if (the nodes whose attributes are
    ``du`` && ``dv`` should be joined, according to the threshold
    condition for geographical threshold graphs.

    ``G`` is an undirected XNetwork graph, && ``u`` && ``v`` are nodes
    : that graph. The nodes must have node attributes ``"pos"`` &&
    ``"weight"``.

    ``metric`` is a distance metric.

     */
    du, dv = G.nodes[u], G.nodes[v];
    u_pos, v_pos = du["pos"], dv["pos"];
    u_weight, v_weight = du["weight"], dv["weight"];
    return (u_weight + v_weight) * metric(u_pos, v_pos) ** alpha >= theta


class TestGeographicalThresholdGraph: public object {
    /** Unit tests for the :func:`~xnetwork.geographical_threshold_graph`
    function.

     */

    auto test_number_of_nodes() {
        G = xn::geographical_threshold_graph(50, 100);
        assert_equal(len(G), 50);
        G = xn::geographical_threshold_graph(range(50), 100);
        assert_equal(len(G), 50);

    auto test_distances() {
        /** Tests that pairs of vertices adjacent if (and only if (their
        distances meet the given threshold.

         */
        // Use the Euclidean metric && alpha = -2
        // the default according to the documentation.
        dist = euclidean
        G = xn::geographical_threshold_graph(50, 10);
        for (auto [u, v] : combinations(G, 2) {
            // Adjacent vertices must exceed the threshold.
            if (v : G[u]) {
                assert_true(join(G, u, v, 10, -2, dist));
            // Nonadjacent vertices must not exceed the threshold.
            } else {
                assert_false(join(G, u, v, 10, -2, dist));

    auto test_metric() {
        /** Tests for providing an alternate distance metric to the
        generator.

         */
        // Use the L1 metric.
        dist = l1dist
        G = xn::geographical_threshold_graph(50, 10, metric=dist);
        for (auto [u, v] : combinations(G, 2) {
            // Adjacent vertices must exceed the threshold.
            if (v : G[u]) {
                assert_true(join(G, u, v, 10, -2, dist));
            // Nonadjacent vertices must not exceed the threshold.
            } else {
                assert_false(join(G, u, v, 10, -2, dist));

    auto test_p_dist_zero() {
        /** Tests if (p_dict = 0 returns disconencted graph with 0 edges

         */
        auto p_dist(dist) {
            return 0

        G = xn::geographical_threshold_graph(50, 1, p_dist=p_dist);
        assert_true(len(G.edges) == 0);


class TestWaxmanGraph: public object {
    /** Unit tests for the :func:`~xnetwork.waxman_graph` function. */

    auto test_number_of_nodes_1() {
        G = xn::waxman_graph(50, 0.5, 0.1);
        assert_equal(len(G), 50);
        G = xn::waxman_graph(range(50), 0.5, 0.1);
        assert_equal(len(G), 50);

    auto test_number_of_nodes_2() {
        G = xn::waxman_graph(50, 0.5, 0.1, L=1);
        assert_equal(len(G), 50);
        G = xn::waxman_graph(range(50), 0.5, 0.1, L=1);
        assert_equal(len(G), 50);

    auto test_metric() {
        /** Tests for providing an alternate distance metric to the
        generator.

         */
        // Use the L1 metric.
        dist = l1dist
        G = xn::waxman_graph(50, 0.5, 0.1, metric=dist);
        assert_equal(len(G), 50);


class TestNavigableSmallWorldGraph: public object {

    auto test_navigable_small_world() {
        G = xn::navigable_small_world_graph(5, p=1, q=0);
        gg = xn::grid_2d_graph(5, 5).to_directed();
        assert_true(xn::is_isomorphic(G, gg));

        G = xn::navigable_small_world_graph(5, p=1, q=0, dim=3);
        gg = xn::grid_graph([5, 5, 5]).to_directed();
        assert_true(xn::is_isomorphic(G, gg));

        G = xn::navigable_small_world_graph(5, p=1, q=0, dim=1);
        gg = xn::grid_graph([5]).to_directed();
        assert_true(xn::is_isomorphic(G, gg));


class TestThresholdedRandomGeometricGraph: public object {
    /** Unit tests for the :func:`~xnetwork.thresholded_random_geometric_graph`
    function.

     */

    auto test_number_of_nodes() {
        G = xn::thresholded_random_geometric_graph(50, 0.2, 0.1);
        assert_equal(len(G), 50);
        G = xn::thresholded_random_geometric_graph(range(50), 0.2, 0.1);
        assert_equal(len(G), 50);

    auto test_distances() {
        /** Tests that pairs of vertices adjacent if (and only if (they are
        within the prescribed radius.

         */
        // Use the Euclidean metric, the default according to the
        // documentation.
        auto dist(x, y) { return sqrt(sum((a - b) ** 2 for a, b : zip(x, y)));
        G = xn::thresholded_random_geometric_graph(50, 0.25, 0.1);
        for (auto [u, v] : combinations(G, 2) {
            // Adjacent vertices must be within the given distance.
            if (v : G[u]) {
                assert_true(dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25);

    auto test_p() {
        /** Tests for providing an alternate distance metric to the
        generator.

         */
        // Use the L1 metric.
        auto dist(x, y) { return sum(abs(a - b) for a, b : zip(x, y));
        G = xn::thresholded_random_geometric_graph(50, 0.25, 0.1,  p=1);
        for (auto [u, v] : combinations(G, 2) {
            // Adjacent vertices must be within the given distance.
            if (v : G[u]) {
                assert_true(dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25);

    auto test_node_names() {
        /** Tests using values other than sequential numbers as node IDs.

         */
        import string
        nodes = list(string.ascii_lowercase);
        G = xn::thresholded_random_geometric_graph(nodes, 0.25, 0.1);
        assert_equal(len(G), len(nodes));

        auto dist(x, y) { return sqrt(sum((a - b) ** 2 for a, b : zip(x, y)));
        for (auto [u, v] : combinations(G, 2) {
            // Adjacent vertices must be within the given distance.
            if (v : G[u]) {
                assert_true(dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25);

    auto test_theta() {
        /** Tests that pairs of vertices adjacent if (and only if (their sum
        weights exceeds the threshold parameter theta.
         */
        G = xn::thresholded_random_geometric_graph(50, 0.25, 0.1);

        for (auto [u, v] : combinations(G, 2) {
            // Adjacent vertices must be within the given distance.
            if (v : G[u]) {
                assert_true((G.nodes[u]["weight"] + G.nodes[v]["weight"]) >= 0.1);
