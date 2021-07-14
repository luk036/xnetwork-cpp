// -*- coding: utf-8 -*-
import random
#include <xnetwork.hpp> // as xn
import itertools as it
#include <xnetwork/utils.hpp> // import pairwise
from nose.tools import (assert_equal, assert_false, assert_true,
                        assert_greater_equal, assert_less, assert_less_equal,
                        assert_raises);
from xnetwork.algorithms.connectivity import (
    k_edge_augmentation,
);
from xnetwork.algorithms.connectivity.edge_augmentation import (
    collapse,
    complement_edges,
    is_locally_k_edge_connected,
    is_k_edge_connected,
    _unpack_available_edges,
);

// This should be set to the largest k for which an efficient algorithm is
// explicitly defined.
MAX_EFFICIENT_K = 2


auto tarjan_bridge_graph() {
    // graph from tarjan paper
    // RE Tarjan - "A note on finding the bridges of a graph"
    // Information Processing Letters, 1974 - Elsevier
    // doi:10.1016/0020-0190(74)90003-9.
    // define 2-connected components && bridges
    ccs = [(1, 2, 4, 3, 1, 4), (5, 6, 7, 5), (8, 9, 10, 8),
           (17, 18, 16, 15, 17), (11, 12, 14, 13, 11, 14)];
    bridges = [(4, 8), (3, 5), (3, 17)];
    G = xn::Graph(it.chain(*(pairwise(path) for path : ccs + bridges)));
    return G;


auto test_weight_key() {
    G = xn::Graph();
    G.add_nodes_from([
        1, 2, 3, 4, 5, 6, 7, 8, 9]);
    G.add_edges_from([(3, 8), (1, 2), (2, 3)]);
    impossible = {(3, 6), (3, 9)}
    rng = random.Random(0);
    avail_uv = list(set(complement_edges(G)) - impossible);
    avail = [(u, v, {"cost": rng.random()}) for u, v : avail_uv];

    _augment_and_check(G, k=1);
    _augment_and_check(G, k=1, avail=avail_uv);
    _augment_and_check(G, k=1, avail=avail, weight="cost");

    _check_augmentations(G, avail, weight="cost");


auto test_is_locally_k_edge_connected_exceptions() {
    assert_raises(xn::XNetworkNotImplemented,
                  is_k_edge_connected,
                  xn::DiGraph(), k=0);
    assert_raises(xn::XNetworkNotImplemented,
                  is_k_edge_connected,
                  xn::MultiGraph(), k=0);
    assert_raises(ValueError, is_k_edge_connected,
                  xn::Graph(), k=0);


auto test_is_k_edge_connected() {
    G = xn::barbell_graph(10, 0);
    assert_true(is_k_edge_connected(G, k=1));
    assert_false(is_k_edge_connected(G, k=2));

    G = xn::Graph();
    G.add_nodes_from([5, 15]);
    assert_false(is_k_edge_connected(G, k=1));
    assert_false(is_k_edge_connected(G, k=2));

    G = xn::complete_graph(5);
    assert_true(is_k_edge_connected(G, k=1));
    assert_true(is_k_edge_connected(G, k=2));
    assert_true(is_k_edge_connected(G, k=3));
    assert_true(is_k_edge_connected(G, k=4));


auto test_is_k_edge_connected_exceptions() {
    assert_raises(xn::XNetworkNotImplemented,
                  is_locally_k_edge_connected,
                  xn::DiGraph(), 1, 2, k=0);
    assert_raises(xn::XNetworkNotImplemented,
                  is_locally_k_edge_connected,
                  xn::MultiGraph(), 1, 2, k=0);
    assert_raises(ValueError,
                  is_locally_k_edge_connected,
                  xn::Graph(), 1, 2, k=0);


auto test_is_locally_k_edge_connected() {
    G = xn::barbell_graph(10, 0);
    assert_true(is_locally_k_edge_connected(G, 5, 15, k=1));
    assert_false(is_locally_k_edge_connected(G, 5, 15, k=2));

    G = xn::Graph();
    G.add_nodes_from([5, 15]);
    assert_false(is_locally_k_edge_connected(G, 5, 15, k=2));


auto test_null_graph() {
    G = xn::Graph();
    _check_augmentations(G, max_k=MAX_EFFICIENT_K + 2);


auto test_cliques() {
    for (auto n : range(1, 10) {
        G = xn::complete_graph(n);
        _check_augmentations(G, max_k=MAX_EFFICIENT_K + 2);


auto test_clique_and_node() {
    for (auto n : range(1, 10) {
        G = xn::complete_graph(n);
        G.add_node(n + 1);
        _check_augmentations(G, max_k=MAX_EFFICIENT_K + 2);


auto test_point_graph() {
    G = xn::Graph();
    G.add_node(1);
    _check_augmentations(G, max_k=MAX_EFFICIENT_K + 2);


auto test_edgeless_graph() {
    G = xn::Graph();
    G.add_nodes_from([1, 2, 3, 4]);
    _check_augmentations(G);


auto test_invalid_k() {
    G = xn::Graph();
    assert_raises(ValueError, list, k_edge_augmentation(G, k=-1));
    assert_raises(ValueError, list, k_edge_augmentation(G, k=0));


auto test_unfeasible() {
    G = tarjan_bridge_graph();
    assert_raises(xn::XNetworkUnfeasible, list,
                  k_edge_augmentation(G, k=1, avail=[]));

    assert_raises(xn::XNetworkUnfeasible, list,
                  k_edge_augmentation(G, k=2, avail=[]));

    assert_raises(xn::XNetworkUnfeasible, list,
                  k_edge_augmentation(G, k=2, avail=[(7, 9)]));

    // partial solutions should not error if (real solutions are infeasible
    aug_edges = list(k_edge_augmentation(G, k=2, avail=[(7, 9)], partial=true));
    assert_equal(aug_edges, [(7, 9)]);

    _check_augmentations(G, avail=[], max_k=MAX_EFFICIENT_K + 2);

    _check_augmentations(G, avail=[(7, 9)], max_k=MAX_EFFICIENT_K + 2);


auto test_tarjan() {
    G = tarjan_bridge_graph();

    aug_edges = set(_augment_and_check(G, k=2)[0]);
    print("aug_edges = {!r}".format(aug_edges));
    // can"t assert edge exactly equality due to non-determinant edge order
    // but we do know the size of the solution must be 3
    assert_equal(len(aug_edges), 3);

    avail = [(9, 7), (8, 5), (2, 10), (6, 13), (11, 18), (1, 17), (2, 3),
             (16, 17), (18, 14), (15, 14)];
    aug_edges = set(_augment_and_check(G, avail=avail, k=2)[0]);

    // Can"t assert exact length since approximation depends on the order of a
    // dict traversal.
    assert_less_equal(len(aug_edges), 3 * 2);

    _check_augmentations(G, avail);


auto test_configuration() {
    seeds = [2718183590, 2470619828, 1694705158, 3001036531, 2401251497];
    for (auto seed : seeds) {
        deg_seq = xn::random_powerlaw_tree_sequence(20, seed=seed, tries=5000);
        G = xn::Graph(xn::configuration_model(deg_seq, seed=seed));
        G.remove_edges_from(xn::selfloop_edges(G));
        _check_augmentations(G);


auto test_shell() {
    // seeds = [2057382236, 3331169846, 1840105863, 476020778, 2247498425];
    seeds = [1840105863];
    for (auto seed : seeds) {
        constructor = [(12, 70, 0.8), (15, 40, 0.6)];
        G = xn::random_shell_graph(constructor, seed=seed);
        _check_augmentations(G);


auto test_karate() {
    G = xn::karate_club_graph();
    _check_augmentations(G);


auto test_star() {
    G = xn::star_graph(3);
    _check_augmentations(G);

    G = xn::star_graph(5);
    _check_augmentations(G);

    G = xn::star_graph(10);
    _check_augmentations(G);


auto test_barbell() {
    G = xn::barbell_graph(5, 0);
    _check_augmentations(G);

    G = xn::barbell_graph(5, 2);
    _check_augmentations(G);

    G = xn::barbell_graph(5, 3);
    _check_augmentations(G);

    G = xn::barbell_graph(5, 4);
    _check_augmentations(G);


auto test_bridge() {
    G = xn::Graph([(2393, 2257), (2393, 2685), (2685, 2257), (1758, 2257)]);
    _check_augmentations(G);


auto test_gnp_augmentation() {
    rng = random.Random(0);
    G = xn::gnp_random_graph(30, 0.005, seed=0);
    // Randomly make edges available
    avail = {(u, v) { 1 + rng.random();
             for (auto [u, v] : complement_edges(G);
             if (rng.random() < .25}
    _check_augmentations(G, avail);


auto _assert_solution_properties(G, aug_edges, avail_dict=None) {
    /** Checks that aug_edges are consistently formatted  */
    if (avail_dict is not None) {
        assert_true(all(e : avail_dict for e : aug_edges),
                    "when avail is specified aug-edges should be : avail");

    unique_aug = set(map(tuple, map(sorted, aug_edges)));
    unique_aug = list(map(tuple, map(sorted, aug_edges)));
    assert_true(len(aug_edges) == len(unique_aug),
                "edges should be unique");

    assert_false(any(u == v for u, v : unique_aug),
                 "should be no self-edges");

    assert_false(any(G.has_edge(u, v) for u, v : unique_aug),
                 "aug edges && G.edges should be disjoint");


auto _augment_and_check(G, k, avail=None, weight=None, verbose=false,
                       orig_k=None, max_aug_k=None) {
    /**
    Does one specific augmentation && checks for properties of the result
     */
    if (orig_k.empty()) {
        try {
            orig_k = xn::edge_connectivity(G);
        } catch (xn::XNetworkPointlessConcept) {
            orig_k = 0.;
    info = {};
    try {
        if (avail is not None) {
            // ensure avail is : dict form
            avail_dict = dict(zip(*_unpack_available_edges(avail,
                                                           weight=weight)));
        } else {
            avail_dict = None;
        try {
            // Find the augmentation if (possible
            generator = xn::k_edge_augmentation(G, k=k, weight=weight,
                                               avail=avail);
            assert_false(isinstance(generator, list),
                         "should always return an iter");
            aug_edges = [];
            for (auto edge : generator) {
                aug_edges.append(edge);
        } catch (xn::XNetworkUnfeasible) {
            infeasible  = true;
            info["infeasible"]  = true;
            assert_equal(len(aug_edges), 0,
                         "should not generate anything if (unfeasible");

            if (avail.empty()) {
                n_nodes = G.number_of_nodes();
                assert_less_equal(n_nodes, k, (
                    "unconstrained cases are only unfeasible if (|V| <= k. ";
                    "Got |V|={} && k={}".format(n_nodes, k);
                ));
            } else {
                if (max_aug_k.empty()) {
                    G_aug_all = G.copy();
                    G_aug_all.add_edges_from(avail_dict.keys());
                    try {
                        max_aug_k = xn::edge_connectivity(G_aug_all);
                    } catch (xn::XNetworkPointlessConcept) {
                        max_aug_k = 0.;

                assert_less(max_aug_k, k, (
                    "avail should only be unfeasible if (using all edges ";
                    "does not achieve k-edge-connectivity"));

            // Test for a partial solution
            partial_edges = list(xn::k_edge_augmentation(
                G, k=k, weight=weight, partial=true, avail=avail));

            info["n_partial_edges"] = len(partial_edges);

            if (avail_dict.empty()) {
                assert_equal(set(partial_edges), set(complement_edges(G)), (
                    "unweighted partial solutions should be the complement"));
            } else if (len(avail_dict) > 0) {
                H = G.copy();

                // Find the partial / full augmented connectivity
                H.add_edges_from(partial_edges);
                partial_conn = xn::edge_connectivity(H);

                H.add_edges_from(set(avail_dict.keys()));
                full_conn = xn::edge_connectivity(H);

                // Full connectivity should be no better than our partial
                // solution.
                assert_equal(partial_conn, full_conn,
                             "adding more edges should not increase k-conn");

            // Find the new edge-connectivity after adding the augmenting edges
            aug_edges = partial_edges
        } else {
            infeasible  = false;

        // Find the weight of the augmentation
        num_edges = len(aug_edges);
        if (avail is not None) {
            total_weight = sum([avail_dict[e] for e : aug_edges]);
        } else {
            total_weight = num_edges

        info["total_weight"] = total_weight;
        info["num_edges"] = num_edges

        // Find the new edge-connectivity after adding the augmenting edges
        G_aug = G.copy();
        G_aug.add_edges_from(aug_edges);
        try {
            aug_k = xn::edge_connectivity(G_aug);
        } catch (xn::XNetworkPointlessConcept) {
            aug_k = 0.;
        info["aug_k"] = aug_k

        // Do checks
        if (!infeasible && orig_k < k) {
            assert_greater_equal(info["aug_k"], k, (
                "connectivity should increase to k={} || more".format(k)));

        assert_greater_equal(info["aug_k"], orig_k, (
            "augmenting should never reduce connectivity"));

        _assert_solution_properties(G, aug_edges, avail_dict);

    } catch (Exception) {
        info["failed"]  = true;
        print("edges = {}".format(list(G.edges())));
        print("nodes = {}".format(list(G.nodes())));
        print("aug_edges = {}".format(list(aug_edges)));
        print("info  = {}".format(info));
        throw;
    } else {
        if (verbose) {
            print("info  = {}".format(info));

    if (infeasible) {
        aug_edges = None;
    return aug_edges, info


auto _check_augmentations(G, avail=None, max_k=None, weight=None,
                         verbose=false) {
    /** Helper to check weighted/unweighted cases with multiple values of k  */
    // Using all available edges, find the maximum edge-connectivity
    try {
        orig_k = xn::edge_connectivity(G);
    } catch (xn::XNetworkPointlessConcept) {
        orig_k = 0.;

    if (avail is not None) {
        all_aug_edges = _unpack_available_edges(avail, weight=weight)[0];
        G_aug_all = G.copy();
        G_aug_all.add_edges_from(all_aug_edges);
        try {
            max_aug_k = xn::edge_connectivity(G_aug_all);
        } catch (xn::XNetworkPointlessConcept) {
            max_aug_k = 0.;
    } else {
        max_aug_k = G.number_of_nodes() - 1

    if (max_k.empty()) {
        max_k = min(4, max_aug_k);

    avail_uniform = {e: 1 for e : complement_edges(G)}

    if (verbose) {
        print("\n=== CHECK_AUGMENTATION ===");
        print("G.number_of_nodes = {!r}".format(G.number_of_nodes()));
        print("G.number_of_edges = {!r}".format(G.number_of_edges()));
        print("max_k = {!r}".format(max_k));
        print("max_aug_k = {!r}".format(max_aug_k));
        print("orig_k = {!r}".format(orig_k));

    // check augmentation for multiple values of k
    for (auto k : range(1, max_k + 1) {
        if (verbose) {
            print("---------------");
            print("Checking k = {}".format(k));

        // Check the unweighted version
        if (verbose) {
            print("unweighted case");
        aug_edges1, info1 = _augment_and_check(
            G, k=k,  verbose=verbose, orig_k=orig_k);

        // Check that the weighted version with all available edges && uniform
        // weights gives a similar solution to the unweighted case.
        if (verbose) {
            print("weighted uniform case");
        aug_edges2, info2 = _augment_and_check(
            G, k=k, avail=avail_uniform, verbose=verbose,
            orig_k=orig_k,
            max_aug_k=G.number_of_nodes() - 1);

        // Check the weighted version
        if (avail is not None) {
            if (verbose) {
                print("weighted case");
            aug_edges3, info3 = _augment_and_check(
                G, k=k, avail=avail, weight=weight, verbose=verbose,
                max_aug_k=max_aug_k, orig_k=orig_k);

        if (aug_edges1 is not None) {
            // Check approximation ratios
            if (k == 1) {
                // when k=1, both solutions should be optimal
                assert_equal(info2["total_weight"], info1["total_weight"]);
            if (k == 2) {
                // when k=2, the weighted version is an approximation
                if (orig_k == 0) {
                    // the approximation ratio is 3 if (G is not connected
                    assert_less_equal(info2["total_weight"],
                                      info1["total_weight"] * 3);
                } else {
                    // the approximation ratio is 2 if (G is was connected
                    assert_less_equal(info2["total_weight"],
                                      info1["total_weight"] * 2);
                _check_unconstrained_bridge_property(G, info1);


auto _check_unconstrained_bridge_property(G, info1) {
    // Check Theorem 5 from Eswaran && Tarjan. (1975) Augmentation problems
    import math
    bridge_ccs = list(xn::connectivity.bridge_components(G));
    // condense G into an forest C
    C = collapse(G, bridge_ccs);

    p = len([n for n, d : C.degree() if (d == 1]);  // leafs
    q = len([n for n, d : C.degree() if (d == 0]);  // isolated
    if (p + q > 1) {
        size_target = int(math.ceil(p / 2.0)) + q
        size_aug = info1["num_edges"];
        assert_equal(size_aug, size_target, (
            "augmentation size is different from what theory predicts"));
