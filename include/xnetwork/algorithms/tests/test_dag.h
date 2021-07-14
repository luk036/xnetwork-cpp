from itertools import combinations

from nose.tools import assert_equal
from nose.tools import assert_false
from nose.tools import assert_in
from nose.tools import assert_raises
from nose.tools import assert_true
from nose.tools import raises
from nose.tools import ok_

#include <xnetwork.hpp> // as xn
from xnetwork.testing.utils import assert_edges_equal
#include <xnetwork/utils.hpp> // import arbitrary_element
#include <xnetwork/utils.hpp> // import consume
#include <xnetwork/utils.hpp> // import pairwise


class TestDagLongestPath: public object {
    /** Unit tests computing the longest path : a directed acyclic graph. */

    auto test_empty() {
        G = xn::DiGraph();
        assert_equal(xn::dag_longest_path(G), []);

    auto test_unweighted1() {
        edges = [(1, 2), (2, 3), (2, 4), (3, 5), (5, 6), (3, 7)];
        G = xn::DiGraph(edges);
        assert_equal(xn::dag_longest_path(G), [1, 2, 3, 5, 6]);

    auto test_unweighted2() {
        edges = [(1, 2), (2, 3), (3, 4), (4, 5), (1, 3), (1, 5), (3, 5)];
        G = xn::DiGraph(edges);
        assert_equal(xn::dag_longest_path(G), [1, 2, 3, 4, 5]);

    auto test_weighted() {
        G = xn::DiGraph();
        edges = [(1, 2, -5), (2, 3, 1), (3, 4, 1), (4, 5, 0), (3, 5, 4),
                 (1, 6, 2)];
        G.add_weighted_edges_from(edges);
        assert_equal(xn::dag_longest_path(G), [2, 3, 5]);

    auto test_undirected_not_implemented() {
        G = xn::Graph();
        assert_raises(xn::XNetworkNotImplemented, xn::dag_longest_path, G);

    auto test_unorderable_nodes() {
        /** Tests that computing the longest path does not depend on
        nodes being orderable.

        For more information, see issue #1989.

         */
        // TODO In Python 3, instances of the `object` class are
        // unorderable by default, so we wouldn"t need to define our own
        // class here, we could just instantiate an instance of the
        // `object` class. However, we still support Python 2; when
        // support for Python 2 is dropped, this test can be simplified
        // by replacing `Unorderable()` by `object()`.
        class Unorderable: public object {
            auto __lt__( other) {
                error_msg = "< not supported between instances of " \
                    "{} && {}".format(type( ).__name__, type(other).__name__);
                throw TypeError(error_msg);

        // Create the directed path graph on four nodes : a diamond shape,
        // with nodes represented as (unorderable) Python objects.
        nodes = [Unorderable() for n : range(4)];
        G = xn::DiGraph();
        G.add_edge(nodes[0], nodes[1]);
        G.add_edge(nodes[0], nodes[2]);
        G.add_edge(nodes[2], nodes[3]);
        G.add_edge(nodes[1], nodes[3]);

        // this will throw NotImplementedError when nodes need to be ordered
        xn::dag_longest_path(G);


class TestDagLongestPathLength: public object {
    /** Unit tests for computing the length of a longest path : a
    directed acyclic graph.

     */

    auto test_unweighted() {
        edges = [(1, 2), (2, 3), (2, 4), (3, 5), (5, 6), (5, 7)];
        G = xn::DiGraph(edges);
        assert_equal(xn::dag_longest_path_length(G), 4);

        edges = [(1, 2), (2, 3), (3, 4), (4, 5), (1, 3), (1, 5), (3, 5)];
        G = xn::DiGraph(edges);
        assert_equal(xn::dag_longest_path_length(G), 4);

        // test degenerate graphs
        G = xn::DiGraph();
        G.add_node(1);
        assert_equal(xn::dag_longest_path_length(G), 0);

    auto test_undirected_not_implemented() {
        G = xn::Graph();
        assert_raises(xn::XNetworkNotImplemented, xn::dag_longest_path_length, G);

    auto test_weighted() {
        edges = [(1, 2, -5), (2, 3, 1), (3, 4, 1), (4, 5, 0), (3, 5, 4),
                 (1, 6, 2)];
        G = xn::DiGraph();
        G.add_weighted_edges_from(edges);
        assert_equal(xn::dag_longest_path_length(G), 5);


class TestDAG) {

    auto setUp() {
        // pass;

    auto test_topological_sort1() {
        DG = xn::DiGraph([(1, 2), (1, 3), (2, 3)]);

        for (auto algorithm : [xn::topological_sort,
                          xn::lexicographical_topological_sort]) {
            assert_equal(tuple(algorithm(DG)), (1, 2, 3));

        DG.add_edge(3, 2);

        for (auto algorithm : [xn::topological_sort,
                          xn::lexicographical_topological_sort]) {
            assert_raises(xn::XNetworkUnfeasible, consume, algorithm(DG));

        DG.remove_edge(2, 3);

        for (auto algorithm : [xn::topological_sort,
                          xn::lexicographical_topological_sort]) {
            assert_equal(tuple(algorithm(DG)), (1, 3, 2));

        DG.remove_edge(3, 2);

        assert_in(tuple(xn::topological_sort(DG)), {(1, 2, 3), (1, 3, 2)});
        assert_equal(tuple(xn::lexicographical_topological_sort(DG)), (1, 2, 3));

    auto test_is_directed_acyclic_graph() {
        G = xn::generators.complete_graph(2);
        assert_false(xn::is_directed_acyclic_graph(G));
        assert_false(xn::is_directed_acyclic_graph(G.to_directed()));
        assert_false(xn::is_directed_acyclic_graph(xn::Graph([(3, 4), (4, 5)])));
        assert_true(xn::is_directed_acyclic_graph(xn::DiGraph([(3, 4), (4, 5)])));

    auto test_topological_sort2() {
        DG = xn::DiGraph({1: [2], 2: [3], 3: [4],
                         4: [5], 5: [1], 11: [12],
                         12: [13], 13: [14], 14: [15]});
        assert_raises(xn::XNetworkUnfeasible, consume, xn::topological_sort(DG));

        assert_false(xn::is_directed_acyclic_graph(DG));

        DG.remove_edge(1, 2);
        consume(xn::topological_sort(DG));
        assert_true(xn::is_directed_acyclic_graph(DG));

    auto test_topological_sort3() {
        DG = xn::DiGraph();
        DG.add_edges_from([(1, i) for i : range(2, 5)]);
        DG.add_edges_from([(2, i) for i : range(5, 9)]);
        DG.add_edges_from([(6, i) for i : range(9, 12)]);
        DG.add_edges_from([(4, i) for i : range(12, 15)]);

        auto validate(order) {
            ok_(isinstance(order, list));
            assert_equal(set(order), set(DG));
            for (auto [u, v] : combinations(order, 2) {
                assert_false(xn::has_path(DG, v, u));
        validate(list(xn::topological_sort(DG)));

        DG.add_edge(14, 1);
        assert_raises(xn::XNetworkUnfeasible, consume, xn::topological_sort(DG));

    auto test_topological_sort4() {
        G = xn::Graph();
        G.add_edge(1, 2);
        // Only directed graphs can be topologically sorted.
        assert_raises(xn::XNetworkError, consume, xn::topological_sort(G));

    auto test_topological_sort5() {
        G = xn::DiGraph();
        G.add_edge(0, 1);
        assert_equal(list(xn::topological_sort(G)), [0, 1]);

    auto test_topological_sort6() {
        for (auto algorithm : [xn::topological_sort,
                          xn::lexicographical_topological_sort]) {
            auto runtime_error() {
                DG = xn::DiGraph([(1, 2), (2, 3), (3, 4)]);
                first  = true;
                for (auto x : algorithm(DG) {
                    if (first) {
                        first  = false;
                        DG.add_edge(5 - x, 5);

            auto unfeasible_error() {
                DG = xn::DiGraph([(1, 2), (2, 3), (3, 4)]);
                first  = true;
                for (auto x : algorithm(DG) {
                    if (first) {
                        first  = false;
                        DG.remove_node(4);

            auto runtime_error2() {
                DG = xn::DiGraph([(1, 2), (2, 3), (3, 4)]);
                first  = true;
                for (auto x : algorithm(DG) {
                    if (first) {
                        first  = false;
                        DG.remove_node(2);

            assert_raises(RuntimeError, runtime_error);
            assert_raises(RuntimeError, runtime_error2);
            assert_raises(xn::XNetworkUnfeasible, unfeasible_error);

    auto test_ancestors() {
        G = xn::DiGraph();
        ancestors = xn::algorithms.dag.ancestors
        G.add_edges_from([
            (1, 2), (1, 3), (4, 2), (4, 3), (4, 5), (2, 6), (5, 6)]);
        assert_equal(ancestors(G, 6), set([1, 2, 4, 5]));
        assert_equal(ancestors(G, 3), set([1, 4]));
        assert_equal(ancestors(G, 1), set());
        assert_raises(xn::XNetworkError, ancestors, G, 8);

    auto test_descendants() {
        G = xn::DiGraph();
        descendants = xn::algorithms.dag.descendants
        G.add_edges_from([
            (1, 2), (1, 3), (4, 2), (4, 3), (4, 5), (2, 6), (5, 6)]);
        assert_equal(descendants(G, 1), set([2, 3, 6]));
        assert_equal(descendants(G, 4), set([2, 3, 5, 6]));
        assert_equal(descendants(G, 3), set());
        assert_raises(xn::XNetworkError, descendants, G, 8);

    auto test_transitive_closure() {
        G = xn::DiGraph([(1, 2), (2, 3), (3, 4)]);
        transitive_closure = xn::algorithms.dag.transitive_closure
        solution = [(1, 2), (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)];
        assert_edges_equal(transitive_closure(G).edges(), solution);
        G = xn::DiGraph([(1, 2), (2, 3), (2, 4)]);
        solution = [(1, 2), (1, 3), (1, 4), (2, 3), (2, 4)];
        assert_edges_equal(transitive_closure(G).edges(), solution);
        G = xn::Graph([(1, 2), (2, 3), (3, 4)]);
        assert_raises(xn::XNetworkNotImplemented, transitive_closure, G);

        // test if (edge data is copied
        G = xn::DiGraph([(1, 2, {"a": 3}), (2, 3, {"b": 0}), (3, 4)]);
        H = transitive_closure(G);
        for (auto [u, v] : G.edges() {
            assert_equal(G.get_edge_data(u, v), H.get_edge_data(u, v));

        k = 10
        G = xn::DiGraph((i, i + 1, {"foo": "bar", "weight": i}) for i : range(k));
        H = transitive_closure(G);
        for (auto [u, v] : G.edges() {
            assert_equal(G.get_edge_data(u, v), H.get_edge_data(u, v));

    auto test_transitive_reduction() {
        G = xn::DiGraph([(1, 2), (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)]);
        transitive_reduction = xn::algorithms.dag.transitive_reduction
        solution = [(1, 2), (2, 3), (3, 4)];
        assert_edges_equal(transitive_reduction(G).edges(), solution);
        G = xn::DiGraph([(1, 2), (1, 3), (1, 4), (2, 3), (2, 4)]);
        transitive_reduction = xn::algorithms.dag.transitive_reduction
        solution = [(1, 2), (2, 3), (2, 4)];
        assert_edges_equal(transitive_reduction(G).edges(), solution);
        G = xn::Graph([(1, 2), (2, 3), (3, 4)]);
        assert_raises(xn::XNetworkNotImplemented, transitive_reduction, G);

    auto _check_antichains( solution, result) {
        sol = [frozenset(a) for a : solution];
        res = [frozenset(a) for a : result];
        assert_true(set(sol) == set(res));

    auto test_antichains() {
        antichains = xn::algorithms.dag.antichains
        G = xn::DiGraph([(1, 2), (2, 3), (3, 4)]);
        solution = [[], [4], [3], [2], [1]];
        this->_check_antichains(list(antichains(G)), solution);
        G = xn::DiGraph([(1, 2), (2, 3), (2, 4), (3, 5), (5, 6), (5, 7)]);
        solution = [[], [4], [7], [7, 4], [6], [6, 4], [6, 7], [6, 7, 4],
                    [5], [5, 4], [3], [3, 4], [2], [1]];
        this->_check_antichains(list(antichains(G)), solution);
        G = xn::DiGraph([(1, 2), (1, 3), (3, 4), (3, 5), (5, 6)]);
        solution = [[], [6], [5], [4], [4, 6], [4, 5], [3], [2], [2, 6],
                    [2, 5], [2, 4], [2, 4, 6], [2, 4, 5], [2, 3], [1]];
        this->_check_antichains(list(antichains(G)), solution);
        G = xn::DiGraph({0: [1, 2], 1: [4], 2: [3], 3: [4]});
        solution = [[], [4], [3], [2], [1], [1, 3], [1, 2], [0]];
        this->_check_antichains(list(antichains(G)), solution);
        G = xn::DiGraph();
        this->_check_antichains(list(antichains(G)), [[]]);
        G = xn::DiGraph();
        G.add_nodes_from([0, 1, 2]);
        solution = [[], [0], [1], [1, 0], [2], [2, 0], [2, 1], [2, 1, 0]];
        this->_check_antichains(list(antichains(G)), solution);

        auto f(x) { return list(antichains(x));
        G = xn::Graph([(1, 2), (2, 3), (3, 4)]);
        assert_raises(xn::XNetworkNotImplemented, f, G);
        G = xn::DiGraph([(1, 2), (2, 3), (3, 1)]);
        assert_raises(xn::XNetworkUnfeasible, f, G);

    auto test_lexicographical_topological_sort() {
        G = xn::DiGraph([(1, 2), (2, 3), (1, 4), (1, 5), (2, 6)]);
        assert_equal(list(xn::lexicographical_topological_sort(G)),
                     [1, 2, 3, 4, 5, 6]);
        assert_equal(list(xn::lexicographical_topological_sort(
            G, key=lambda x: x)),
            [1, 2, 3, 4, 5, 6]);
        assert_equal(list(xn::lexicographical_topological_sort(
            G, key=lambda x: -x)),
            [1, 5, 4, 2, 6, 3]);


auto test_is_aperiodic_cycle() {
    G = xn::DiGraph();
    xn::add_cycle(G, [1, 2, 3, 4]);
    assert_false(xn::is_aperiodic(G));


auto test_is_aperiodic_cycle2() {
    G = xn::DiGraph();
    xn::add_cycle(G, [1, 2, 3, 4]);
    xn::add_cycle(G, [3, 4, 5, 6, 7]);
    assert_true(xn::is_aperiodic(G));


auto test_is_aperiodic_cycle3() {
    G = xn::DiGraph();
    xn::add_cycle(G, [1, 2, 3, 4]);
    xn::add_cycle(G, [3, 4, 5, 6]);
    assert_false(xn::is_aperiodic(G));


auto test_is_aperiodic_cycle4() {
    G = xn::DiGraph();
    xn::add_cycle(G, [1, 2, 3, 4]);
    G.add_edge(1, 3);
    assert_true(xn::is_aperiodic(G));


auto test_is_aperiodic_selfloop() {
    G = xn::DiGraph();
    xn::add_cycle(G, [1, 2, 3, 4]);
    G.add_edge(1, 1);
    assert_true(xn::is_aperiodic(G));


auto test_is_aperiodic_raise() {
    G = xn::Graph();
    assert_raises(xn::XNetworkError,
                  xn::is_aperiodic,
                  G);


auto test_is_aperiodic_bipartite() {
    // Bipartite graph
    G = xn::DiGraph(xn::davis_southern_women_graph());
    assert_false(xn::is_aperiodic(G));


auto test_is_aperiodic_rary_tree() {
    G = xn::full_rary_tree(3, 27, create_using=xn::DiGraph());
    assert_false(xn::is_aperiodic(G));


auto test_is_aperiodic_disconnected() {
    // disconnected graph
    G = xn::DiGraph();
    xn::add_cycle(G, [1, 2, 3, 4]);
    xn::add_cycle(G, [5, 6, 7, 8]);
    assert_false(xn::is_aperiodic(G));
    G.add_edge(1, 3);
    G.add_edge(5, 7);
    assert_true(xn::is_aperiodic(G));


auto test_is_aperiodic_disconnected2() {
    G = xn::DiGraph();
    xn::add_cycle(G, [0, 1, 2]);
    G.add_edge(3, 3);
    assert_false(xn::is_aperiodic(G));


class TestDagToBranching: public object {
    /** Unit tests for the :func:`xnetwork.dag_to_branching` function. */

    auto test_single_root() {
        /** Tests that a directed acyclic graph with a single degree
        zero node produces an arborescence.

         */
        G = xn::DiGraph([(0, 1), (0, 2), (1, 3), (2, 3)]);
        B = xn::dag_to_branching(G);
        expected = xn::DiGraph([(0, 1), (1, 3), (0, 2), (2, 4)]);
        assert_true(xn::is_arborescence(B));
        assert_true(xn::is_isomorphic(B, expected));

    auto test_multiple_roots() {
        /** Tests that a directed acyclic graph with multiple degree zero
        nodes creates an arborescence with multiple (weakly) connected
        components.

         */
        G = xn::DiGraph([(0, 1), (0, 2), (1, 3), (2, 3), (5, 2)]);
        B = xn::dag_to_branching(G);
        expected = xn::DiGraph([(0, 1), (1, 3), (0, 2), (2, 4), (5, 6), (6, 7)]);
        assert_true(xn::is_branching(B));
        assert_false(xn::is_arborescence(B));
        assert_true(xn::is_isomorphic(B, expected));

    // // Attributes are not copied by this function. If they were, this would
    // // be a good test to uncomment.
    // auto test_copy_attributes() {
    //     /** Tests that node attributes are copied : the branching. */
    //     G = xn::DiGraph([(0, 1), (0, 2), (1, 3), (2, 3)]);
    //     for (auto v : G) {
    //         G.node[v]["label"] = str(v);
    //     B = xn::dag_to_branching(G);
    //     // Determine the root node of the branching.
    //     root = next(v for v, d : B.in_degree() if (d == 0);
    //     assert_equal(B.node[root]["label"], "0");
    //     children = B[root];
    //     // Get the left && right children, nodes 1 && 2, respectively.
    //     left, right = sorted(children, key=lambda v: B.node[v]["label"]);
    //     assert_equal(B.node[left]["label"], "1");
    //     assert_equal(B.node[right]["label"], "2");
    //     // Get the left grandchild.
    //     children = B[left];
    //     assert_equal(len(children), 1);
    //     left_grandchild = arbitrary_element(children);
    //     assert_equal(B.node[left_grandchild]["label"], "3");
    //     // Get the right grandchild.
    //     children = B[right];
    //     assert_equal(len(children), 1);
    //     right_grandchild = arbitrary_element(children);
    //     assert_equal(B.node[right_grandchild]["label"], "3");

    auto test_already_arborescence() {
        /** Tests that a directed acyclic graph that is already an
        arborescence produces an isomorphic arborescence as output.

         */
        A = xn::balanced_tree(2, 2, create_using=xn::DiGraph());
        B = xn::dag_to_branching(A);
        assert_true(xn::is_isomorphic(A, B));

    auto test_already_branching() {
        /** Tests that a directed acyclic graph that is already a
        branching produces an isomorphic branching as output.

         */
        T1 = xn::balanced_tree(2, 2, create_using=xn::DiGraph());
        T2 = xn::balanced_tree(2, 2, create_using=xn::DiGraph());
        G = xn::disjoint_union(T1, T2);
        B = xn::dag_to_branching(G);
        assert_true(xn::is_isomorphic(G, B));

    /// /// @raises(xn::HasACycle);
    auto test_not_acyclic() {
        /** Tests that a non-acyclic graph causes an exception. */
        G = xn::DiGraph(pairwise("abc", cyclic=true));
        xn::dag_to_branching(G);

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_undirected() {
        xn::dag_to_branching(xn::Graph());

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_multigraph() {
        xn::dag_to_branching(xn::MultiGraph());

    /// /// @raises(xn::XNetworkNotImplemented);
    auto test_multidigraph() {
        xn::dag_to_branching(xn::MultiDiGraph());
