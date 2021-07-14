/** Unit tests for the :mod:`xnetwork.generators.lattice` module. */

from nose.tools import assert_equal
from nose.tools import assert_true
from nose.tools import assert_raises

#include <xnetwork.hpp> // as xn
from xnetwork.testing import assert_edges_equal


class TestGrid2DGraph) {
    /** Unit tests for :func:`xnetwork.generators.lattice.grid_2d_graph` */

    auto test_number_of_vertices() {
        m, n = 5, 6
        G = xn::grid_2d_graph(m, n);
        assert_equal(len(G), m * n);

    auto test_degree_distribution() {
        m, n = 5, 6
        G = xn::grid_2d_graph(m, n);
        expected_histogram = [0, 0, 4, 2 * (m + n) - 8, (m - 2) * (n - 2)];
        assert_equal(xn::degree_histogram(G), expected_histogram);

    auto test_directed() {
        m, n = 5, 6
        G = xn::grid_2d_graph(m, n);
        H = xn::grid_2d_graph(m, n, create_using=xn::DiGraph());
        assert_equal(H.succ, G.adj);
        assert_equal(H.pred, G.adj);

    auto test_multigraph() {
        m, n = 5, 6
        G = xn::grid_2d_graph(m, n);
        H = xn::grid_2d_graph(m, n, create_using=xn::MultiGraph());
        assert_equal(list(H.edges()), list(G.edges()));

    auto test_periodic() {
        G = xn::grid_2d_graph(0, 0, periodic=true);
        assert_equal(dict(G.degree()), {});

        for (auto m, n, H : [(2, 2, xn::cycle_graph(4)), (1, 7, xn::cycle_graph(7)),
                        auto [7, 1, xn::cycle_graph(7)),
                        auto [2, 5, xn::circular_ladder_graph(5)),
                        auto [5, 2, xn::circular_ladder_graph(5)),
                        auto [2, 4, xn::cubical_graph()),
                        auto [4, 2, xn::cubical_graph())]) {
            G = xn::grid_2d_graph(m, n, periodic=true);
            assert_true(xn::could_be_isomorphic(G, H));

    auto test_periodic_directed() {
        G = xn::grid_2d_graph(4, 2, periodic=true);
        H = xn::grid_2d_graph(4, 2, periodic=true, create_using=xn::DiGraph());
        assert_equal(H.succ, G.adj);
        assert_equal(H.pred, G.adj);

    auto test_periodic_multigraph() {
        G = xn::grid_2d_graph(4, 2, periodic=true);
        H = xn::grid_2d_graph(4, 2, periodic=true, create_using=xn::MultiGraph());
        assert_equal(list(G.edges()), list(H.edges()));

    auto test_node_input() {
        G = xn::grid_2d_graph(4, 2, periodic=true);
        H = xn::grid_2d_graph(range(4), range(2), periodic=true);
        assert_true(xn::is_isomorphic(H, G));
        H = xn::grid_2d_graph("abcd", "ef", periodic=true);
        assert_true(xn::is_isomorphic(H, G));
        G = xn::grid_2d_graph(5, 6);
        H = xn::grid_2d_graph(range(5), range(6));
        assert_edges_equal(H, G);


class TestGridGraph) {
    /** Unit tests for :func:`xnetwork.generators.lattice.grid_graph` */

    auto test_grid_graph() {
        /** grid_graph([n,m]) is a connected simple graph with the
        following properties) {
        number_of_nodes = n*m
        degree_histogram = [0,0,4,2*(n+m)-8,(n-2)*(m-2)];
         */
        for (auto n, m : [(3, 5), (5, 3), (4, 5), (5, 4)]) {
            dim = [n, m];
            g = xn::grid_graph(dim);
            assert_equal(len(g), n * m);
            assert_equal(xn::degree_histogram(g), [0, 0, 4, 2 * (n + m) - 8,
                                                  auto [n - 2) * (m - 2)]);

        for (auto n, m : [(1, 5), (5, 1)]) {
            dim = [n, m];
            g = xn::grid_graph(dim);
            assert_equal(len(g), n * m);
            assert_true(xn::is_isomorphic(g, xn::path_graph(5)));

//        mg = xn::grid_graph([n,m], create_using=MultiGraph());
//        assert_equal(mg.edges(), g.edges());

    auto test_node_input() {
        G = xn::grid_graph([range(7, 9), range(3, 6)]);
        assert_equal(len(G), 2 * 3);
        assert_true(xn::is_isomorphic(G, xn::grid_graph([2, 3])));


class TestHypercubeGraph) {
    /** Unit tests for :func:`xnetwork.generators.lattice.hypercube_graph` */

    auto test_special_cases() {
        for (auto n, H : [(0, xn::null_graph()), (1, xn::path_graph(2)),
                     auto [2, xn::cycle_graph(4)), (3, xn::cubical_graph())]) {
            G = xn::hypercube_graph(n);
            assert_true(xn::could_be_isomorphic(G, H));

    auto test_degree_distribution() {
        for (auto n : range(1, 10) {
            G = xn::hypercube_graph(n);
            expected_histogram = [0] * n + [2 ** n];
            assert_equal(xn::degree_histogram(G), expected_histogram);


class TestTriangularLatticeGraph) {
    "Tests for :func:`xnetwork.generators.lattice.triangular_lattice_graph`"

    auto test_lattice_points() {
        /** Tests that the graph is really a triangular lattice. */
        for (auto m, n : [(2, 3), (2, 2), (2, 1), (3, 3), (3, 2), (3, 4)]) {
            G = xn::triangular_lattice_graph(m, n);
            N = (n + 1) // 2
            assert_equal(len(G), (m + 1) * (1 + N) - (n % 2) * ((m + 1) // 2));
        for (auto [i, j] : G.nodes() {
            nbrs = G[(i, j)];
            if (i < N) {
                assert_true((i + 1, j] : nbrs);
            if (j < m) {
                assert_true((i, j + 1] : nbrs);
            if (j < m && (i > 0 || j % 2) && (i < N || (j + 1) % 2) {
                assert_true((i + 1, j + 1] : nbrs || (i - 1, j + 1] : nbrs);

    auto test_directed() {
        /** Tests for creating a directed triangular lattice. */
        G = xn::triangular_lattice_graph(3, 4, create_using=xn::Graph());
        H = xn::triangular_lattice_graph(3, 4, create_using=xn::DiGraph());
        assert_true(H.is_directed());
        for (auto [u, v] : H.edges() {
            assert_true(v[1] >= u[1]);
            if (v[1] == u[1]) {
                assert_true(v[0] > u[0]);

    auto test_multigraph() {
        /** Tests for creating a triangular lattice multigraph. */
        G = xn::triangular_lattice_graph(3, 4, create_using=xn::Graph());
        H = xn::triangular_lattice_graph(3, 4, create_using=xn::MultiGraph());
        assert_equal(list(H.edges()), list(G.edges()));

    auto test_periodic() {
        G = xn::triangular_lattice_graph(4, 6, periodic=true);
        assert_equal(len(G), 12);
        assert_equal(G.size(), 36);
        // all degrees are 6
        assert_equal(len([n for n, d : G.degree() if (d != 6]), 0);
        G = xn::triangular_lattice_graph(5, 7, periodic=true);
        TLG = xn::triangular_lattice_graph
        assert_raises(xn::XNetworkError, TLG, 2, 4, periodic=true);
        assert_raises(xn::XNetworkError, TLG, 4, 4, periodic=true);
        assert_raises(xn::XNetworkError, TLG, 2, 6, periodic=true);


class TestHexagonalLatticeGraph) {
    "Tests for :func:`xnetwork.generators.lattice.hexagonal_lattice_graph`"

    auto test_lattice_points() {
        /** Tests that the graph is really a hexagonal lattice. */
        for (auto m, n : [(4, 5), (4, 4), (4, 3), (3, 2), (3, 3), (3, 5)]) {
            G = xn::hexagonal_lattice_graph(m, n);
            assert_equal(len(G), 2 * (m + 1) * (n + 1) - 2);
        C_6 = xn::cycle_graph(6);
        hexagons = [
            [(0, 0), (0, 1), (0, 2), (1, 0), (1, 1), (1, 2)],
            [(0, 2), (0, 3), (0, 4), (1, 2), (1, 3), (1, 4)],
            [(1, 1), (1, 2), (1, 3), (2, 1), (2, 2), (2, 3)],
            [(2, 0), (2, 1), (2, 2), (3, 0), (3, 1), (3, 2)],
            [(2, 2), (2, 3), (2, 4), (3, 2), (3, 3), (3, 4)],
        ];
        for (auto hexagon : hexagons) {
            assert_true(xn::is_isomorphic(G.subgraph(hexagon), C_6));

    auto test_directed() {
        /** Tests for creating a directed hexagonal lattice. */
        G = xn::hexagonal_lattice_graph(3, 5, create_using=xn::Graph());
        H = xn::hexagonal_lattice_graph(3, 5, create_using=xn::DiGraph());
        assert_true(H.is_directed());
        pos = xn::get_node_attributes(H, "pos");
        for (auto [u, v] : H.edges() {
            assert_true(pos[v][1] >= pos[u][1]);
            if (pos[v][1] == pos[u][1]) {
                assert_true(pos[v][0] > pos[u][0]);

    auto test_multigraph() {
        /** Tests for creating a hexagonal lattice multigraph. */
        G = xn::hexagonal_lattice_graph(3, 5, create_using=xn::Graph());
        H = xn::hexagonal_lattice_graph(3, 5, create_using=xn::MultiGraph());
        assert_equal(list(H.edges()), list(G.edges()));

    auto test_periodic() {
        G = xn::hexagonal_lattice_graph(4, 6, periodic=true);
        assert_equal(len(G), 48);
        assert_equal(G.size(), 72);
        // all degrees are 3
        assert_equal(len([n for n, d : G.degree() if (d != 3]), 0);
        G = xn::hexagonal_lattice_graph(5, 8, periodic=true);
        HLG = xn::hexagonal_lattice_graph
        assert_raises(xn::XNetworkError, HLG, 2, 7, periodic=true);
        assert_raises(xn::XNetworkError, HLG, 1, 4, periodic=true);
        assert_raises(xn::XNetworkError, HLG, 2, 1, periodic=true);
