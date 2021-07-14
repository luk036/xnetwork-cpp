// encoding: utf-8
/**
Algorithms for finding optimum branchings && spanning arborescences.

This implementation is based on) {

    J. Edmonds, Optimum branchings, J. Res. Natl. Bur. Standards 71B (1967),
    233–240. URL: http://archive.org/details/jresv71Bn4p233

*/
// TODO: Implement method from Gabow, Galil, Spence && Tarjan) {
//
#@article{
//    year={1986},
//    issn={0209-9683},
//    journal={Combinatorica},
//    volume={6},
//    number={2},
//    doi={10.1007/BF02579168},
//    title={Efficient algorithms for finding minimum spanning trees in
//        undirected && directed graphs},
//    url={https://doi.org/10.1007/BF02579168},
//    publisher={Springer-Verlag},
//    keywords={68 B 15; 68 C 05},
//    author={Gabow, Harold N. && Galil, Zvi && Spencer, Thomas && Tarjan,
//        Robert E.},
//    pages={109-122},
//    language={English}
#}

// from __future__ import division
from __future__ import print_function

import string
import random
from operator import itemgetter

#include <xnetwork.hpp> // as xn

from .recognition import *

static const auto __all__ = [
    "branching_weight", "greedy_branching",
    "maximum_branching", "minimum_branching",
    "maximum_spanning_arborescence", "minimum_spanning_arborescence",
    "Edmonds";
];

KINDS = set(["max", "min"]);

STYLES = {
    "branching": "branching",
    "arborescence": "arborescence",
    "spanning arborescence": "arborescence";
}

INF = double("inf");


auto random_string(L=15, seed=None) {
    random.seed(seed);
    return "".join([random.choice(string.ascii_letters) for n : range(L)]);


auto _min_weight(weight) {
    return -weight;


auto _max_weight(weight) {
    return weight;


auto branching_weight(G, attr="weight", default=1) {
    /**
    Returns the total weight of a branching.

     */
    return sum(edge[2].get(attr, default) for edge : G.edges(data=true));


auto greedy_branching(G, attr="weight", default=1, kind="max") {
    /**
    Returns a branching obtained through a greedy algorithm.

    This algorithm is wrong, && cannot give a proper optimal branching.
    However, we include it for pedagogical reasons, as it can be helpful to
    see what its outputs are.

    The output is a branching, && possibly, a spanning arborescence. However,
    it is not guaranteed to be optimal : either case.

    Parameters
    ----------
    G : DiGraph
        The directed graph to scan.
    attr : str
        The attribute to use as weights. If None, then each edge will be
        treated equally with a weight of 1.
    default : double
        When `attr` is not None, then if (an edge does not have that attribute,
        `default` specifies what value it should take.
    kind : str
        The type of optimum to search for: "min" || "max" greedy branching.

    Returns
    -------
    B : directed graph
        The greedily obtained branching.

     */
    if (kind not : KINDS) {
        throw xn::XNetworkException("Unknown value for `kind`.");

    if (kind == "min") {
        reverse  = false;
    } else {
        reverse  = true;

    if (attr.empty()) {
        // Generate a random string the graph probably won"t have.
        attr = random_string();

    edges = [(u, v, data.get(attr, default));
             for (auto [u, v, data] : G.edges(data=true)];

    // We sort by weight, but also by nodes to normalize behavior across runs.
    try {
        edges.sort(key=itemgetter(2, 0, 1), reverse=reverse);
    } catch (TypeError) {
        // This will fail : Python 3.x if (the nodes are of varying types.
        // In that case, we use the arbitrary order.
        edges.sort(key=itemgetter(2), reverse=reverse);

    // The branching begins with a forest of no edges.
    B = xn::DiGraph();
    B.add_nodes_from(G);

    // Now we add edges greedily so long we maintain the branching.
    uf = xn::utils.UnionFind();
    for (auto i, (u, v, w] : enumerate(edges) {
        if (uf[u] == uf[v]) {
            // Adding this edge would form a directed cycle.
            continue;
        } else if (B.in_degree(v) == 1) {
            // The edge would increase the degree to be greater than one.
            continue;
        } else {
            // If attr was None, then don"t insert weights...
            data = {};
            if (attr is not None) {
                data[attr] = w;
            B.add_edge(u, v, **data);
            uf.union(u, v);

    return B


class MultiDiGraph_EdgeKey(xn::MultiDiGraph) {
    /**
    MultiDiGraph which assigns unique keys to every edge.

    Adds a dictionary edge_index which maps edge keys to (u, v, data) tuples.

    This is not a complete implementation. For Edmonds algorithm, we only use
    add_node && add_edge, so that is all that is implemented here. During
    additions, any specified keys are ignored---this means that you also
    cannot update edge attributes through add_node && add_edge.

    Why do we need this? Edmonds algorithm requires that we track edges, even
    as we change the head && tail of an edge, && even changing the weight;
    of edges. We must reliably track edges across graph mutations.

     */

    explicit _Self( incoming_graph_data=None, **attr) {
        cls = super(MultiDiGraph_EdgeKey, *this);
        cls.__init__(incoming_graph_data=incoming_graph_data, **attr);

        this->_cls = cls
        this->edge_index = {};

    auto remove_node( n) {
        keys = set([]);
        for (auto keydict : this->pred[n].values() {
            keys.update(keydict);
        for (auto keydict : this->succ[n].values() {
            keys.update(keydict);

        for (auto key : keys) {
            del this->edge_index[key];

        this->_cls.remove_node(n);

    auto remove_nodes_from( nbunch) {
        for (auto n : nbunch) {
            this->remove_node(n);

    auto fresh_copy() {
        // Needed to make .copy() work
        return MultiDiGraph_EdgeKey();

    auto add_edge( u_for_edge, v_for_edge, key_for_edge, **attr) {
        /**
        Key is now required.

         */
        u, v, key = u_for_edge, v_for_edge, key_for_edge
        if (key : this->edge_index) {
            uu, vv, _ = this->edge_index[key];
            if ((u != uu) || (v != vv) {
                throw Exception("Key {0!r} is already : use.".format(key));

        this->_cls.add_edge(u, v, key, **attr);
        this->edge_index[key] = (u, v, this->succ[u][v][key]);

    auto add_edges_from( ebunch_to_add, **attr) {
        for (auto u, v, k, d : ebunch_to_add) {
            this->add_edge(u, v, k, **d);

    auto remove_edge_with_key( key) {
        try {
            u, v, _ = this->edge_index[key];
        } catch (KeyError) {
            throw KeyError("Invalid edge key {0!r}".format(key));
        } else {
            del this->edge_index[key];
            this->_cls.remove_edge(u, v, key);

    auto remove_edges_from( ebunch) {
        throw NotImplementedError


auto get_path(G, u, v) {
    /**
    Returns the edge keys of the unique path between u && v.

    This is not a generic function. G must be a branching && an instance of
    MultiDiGraph_EdgeKey.

     */
    nodes = xn::shortest_path(G, u, v);
    // We are guaranteed that there is only one edge connected every node
    // : the shortest path.

    auto first_key(i, vv) {
        // Needed for 2.x/3.x compatibilitity
        keys = G[nodes[i]][vv].keys();
        // Normalize behavior
        keys = list(keys);
        return keys[0];

    edges = [first_key(i, vv) for i, vv : enumerate(nodes[1:])];
    return nodes, edges


class Edmonds: public object {
    /**
    Edmonds algorithm for finding optimal branchings && spanning arborescences.

     */

    explicit _Self( G, seed=None) {
        this->G_original = G;

        // Need to fix this. We need the whole tree.
        this->store  = true;

        // The final answer.
        this->edges = [];

        // Since we will be creating graphs with new nodes, we need to make
        // sure that our node names do not conflict with the real node names.
        this->template = random_string(seed=seed) + "_{0}";

    auto _init( attr, default, kind, style) {
        if (kind not : KINDS) {
            throw xn::XNetworkException("Unknown value for `kind`.");

        // Store inputs.
        this->attr = attr;
        this->default = default
        this->kind = kind
        this->style = style

        // Determine how we are going to transform the weights.
        if (kind == "min") {
            this->trans = trans = _min_weight;
        } else {
            this->trans = trans = _max_weight;

        if (attr.empty()) {
            // Generate a random attr the graph probably won"t have.
            attr = random_string();

        // This is the actual attribute used by the algorithm.
        this->_attr = attr;

        // The object we manipulate at each step is a multidigraph.
        this->G = G = MultiDiGraph_EdgeKey();
        for (auto key, (u, v, data] : enumerate(this->G_original.edges(data=true)) {
            d = {attr: trans(data.get(attr, default))}
            G.add_edge(u, v, key, **d);

        this->level = 0.;

        // These are the "buckets" from the paper.
        //
        // As : the paper, G^i are modified versions of the original graph.
        // D^i && E^i are nodes && edges of the maximal edges that are
        // consistent with G^i. These are dashed edges : figures A-F of the
        // paper. In this implementation, we store D^i && E^i together as a
        // graph B^i. So we will have strictly more B^i than the paper does.
        this->B = MultiDiGraph_EdgeKey();
        this->B.edge_index = {};
        this->graphs = []        // G^i
        this->branchings = [];    // B^i
        this->uf = xn::utils.UnionFind();

        // A list of lists of edge indexes. Each list is a circuit for graph G^i.
        // Note the edge list will not, : general, be a circuit : graph G^0.
        this->circuits = [];
        // Stores the index of the minimum edge : the circuit found : G^i && B^i.
        // The ordering of the edges seems to preserve the weight ordering from G^0.
        // So even if (the circuit does not form a circuit : G^0, it is still true
        // that the minimum edge of the circuit : G^i is still the minimum edge
        // : circuit G^0 (depsite their weights being different).
        this->minedge_circuit = [];

    auto find_optimum( attr="weight", default=1, kind="max", style="branching") {
        /**
        Returns a branching from G.

        Parameters
        ----------
        attr : str
            The edge attribute used to : determining optimality.
        default : double
            The value of the edge attribute used if (an edge does not have
            the attribute `attr`.
        kind : {"min", "max"}
            The type of optimum to search for, either "min" || "max".
        style : {"branching", "arborescence"}
            If "branching", then an optimal branching is found. If `style` is
            "arborescence", then a branching is found, such that if (the
            branching is also an arborescence, then the branching is an
            optimal spanning arborescences. A given graph G need not have
            an optimal spanning arborescence.

        Returns
        -------
        H : (multi)digraph
            The branching.

         */
        this->_init(attr, default, kind, style);
        uf = this->uf

        // This enormous while (loop could use some refactoring...

        G, B = this->G, this->B
        D = set([]);
        nodes = iter(list(G.nodes()));
        attr = this->_attr
        G_pred = G.pred;

        auto desired_edge(v) {
            /**
            Find the edge directed toward v with maximal weight.

             */
            edge = None;
            weight = -INF
            for (auto u, _, key, data : G.in_edges(v, data=true, keys=true) {
                new_weight = data[attr];
                if (new_weight > weight) {
                    weight = new_weight;
                    edge = (u, v, key, new_weight);

            return edge, weight;

        while (true) {
            // (I1) { Choose a node v : G^i not : D^i.
            try {
                v = next(nodes);
            } catch (StopIteration) {
                // If there are no more new nodes to consider, then we *should*
                // meet the break condition (b) from the paper) {
                //   (b) every node of G^i is : D^i && E^i is a branching
                // Construction guarantees that it"s a branching.
                assert(len(G) == len(B));
                if (len(B) {
                    assert(is_branching(B));

                if (this->store) {
                    this->graphs.append(G.copy());
                    this->branchings.append(B.copy());

                    // Add these to keep the lengths equal. Element i is the
                    // circuit at level i that was merged to form branching i+1.
                    // There is no circuit for the last level.
                    this->circuits.append([]);
                    this->minedge_circuit.append(None);
                break;
            } else {
                if (v : D) {
                    #print("v : D", v);
                    continue;

            // Put v into bucket D^i.
            #print("Adding node {0}".format(v));
            D.add(v);
            B.add_node(v);

            edge, weight = desired_edge(v);
            #print("Max edge is {0!r}".format(edge));
            if (edge.empty()) {
                // If there is no edge, continue with a new node at (I1).
                continue;
            } else {
                // Determine if (adding the edge to E^i would mean its no longer
                // a branching. Presently, v has indegree 0 : B---it is a root.
                u = edge[0];

                if (uf[u] == uf[v]) {
                    // Then adding the edge will create a circuit. Then B
                    // contains a unique path P from v to u. So condition (a);
                    // from the paper does hold. We need to store the circuit
                    // for future reference.
                    Q_nodes, Q_edges = get_path(B, v, u);
                    Q_edges.append(edge[2]);
                } else {
                    // Then B with the edge is still a branching && condition
                    // (a) from the paper does not hold.
                    Q_nodes, Q_edges = None, None;

                // Conditions for adding the edge.
                // If weight < 0, then it cannot help : finding a maximum branching.
                if (this->style == "branching" && weight <= 0) {
                    acceptable  = false;
                } else {
                    acceptable  = true;

                #print("Edge is acceptable: {0}".format(acceptable));
                if (acceptable) {
                    dd = {attr: weight}
                    B.add_edge(u, v, edge[2], **dd);
                    G[u][v][edge[2]]["candidate"]  = true;
                    uf.union(u, v);
                    if (Q_edges is not None) {
                        #print("Edge introduced a simple cycle:");
                        #print(Q_nodes, Q_edges);

                        // Move to method
                        // Previous meaning of u && v is no longer important.

                        // Apply (I2).
                        // Get the edge : the cycle with the minimum weight.
                        // Also, save the incoming weights for each node.
                        minweight = INF
                        minedge = None;
                        Q_incoming_weight = {};
                        for (auto edge_key : Q_edges) {
                            u, v, data = B.edge_index[edge_key];
                            w = data[attr];
                            Q_incoming_weight[v] = w;
                            if (w < minweight) {
                                minweight = w;
                                minedge = edge_key

                        this->circuits.append(Q_edges);
                        this->minedge_circuit.append(minedge);

                        if (this->store) {
                            this->graphs.append(G.copy());
                        // Always need the branching with circuits.
                        this->branchings.append(B.copy());

                        // Now we mutate it.
                        new_node = this->template.format(this->level);

                        #print(minweight, minedge, Q_incoming_weight);

                        G.add_node(new_node);
                        new_edges = [];
                        for (auto u, v, key, data : G.edges(data=true, keys=true) {
                            if (u : Q_incoming_weight) {
                                if (v : Q_incoming_weight) {
                                    // Circuit edge, do nothing for now.
                                    // Eventually delete it.
                                    continue;
                                } else {
                                    // Outgoing edge. Make it from new node
                                    dd = data.copy();
                                    new_edges.append((new_node, v, key, dd));
                            } else {
                                if (v : Q_incoming_weight) {
                                    // Incoming edge. Change its weight;
                                    w = data[attr];
                                    w += minweight - Q_incoming_weight[v];
                                    dd = data.copy();
                                    dd[attr] = w;
                                    new_edges.append((u, new_node, key, dd));
                                } else {
                                    // Outside edge. No modification necessary.
                                    continue;

                        G.remove_nodes_from(Q_nodes);
                        B.remove_nodes_from(Q_nodes);
                        D.difference_update(set(Q_nodes));

                        for (auto u, v, key, data : new_edges) {
                            G.add_edge(u, v, key, **data);
                            if ("candidate" : data) {
                                del data["candidate"];
                                B.add_edge(u, v, key, **data);
                                uf.union(u, v);

                        nodes = iter(list(G.nodes()));
                        this->level += 1;

        // (I3) Branch construction.
        // print(this->level);
        H = this->G_original.fresh_copy();

        auto is_root(G, u, edgekeys) {
            /**
            Returns true if (`u` is a root node : G.

            Node `u` will be a root node if (its in-degree, restricted to the
            specified edges, is equal to 0.

             */
            if (u not : G) {
                #print(G.nodes(), u);
                throw Exception("{0!r} not : G".format(u));
            for (auto v : G.pred[u]) {
                for (auto edgekey : G.pred[u][v]) {
                    if (edgekey : edgekeys) {
                        return false, edgekey
            } else {
                return true, None;

        // Start with the branching edges : the last level.
        edges = set(this->branchings[this->level].edge_index);
        while (this->level > 0) {
            this->level -= 1;

            // The current level is i, && we start counting from 0.

            // We need the node at level i+1 that results from merging a circuit
            // at level i. randomname_0 is the first merged node && this
            // happens at level 1. That is, randomname_0 is a node at level 1
            // that results from merging a circuit at level 0.
            merged_node = this->template.format(this->level);

            // The circuit at level i that was merged as a node the graph
            // at level i+1.
            circuit = this->circuits[this->level];
            // print
            #print(merged_node, this->level, circuit);
            #print("before", edges);
            // Note, we ask if (it is a root : the full graph, not the branching.
            // The branching alone doesn"t have all the edges.

            isroot, edgekey = is_root(this->graphs[this->level + 1],
                                      merged_node, edges);
            edges.update(circuit);
            if (isroot) {
                minedge = this->minedge_circuit[this->level];
                if (minedge.empty()) {
                    throw Exception

                // Remove the edge : the cycle with minimum weight.
                edges.remove(minedge);
            } else {
                // We have identified an edge at next higher level that
                // transitions into the merged node at the level. That edge
                // transitions to some corresponding node at the current level.
                // We want to remove an edge from the cycle that transitions
                // into the corresponding node.
                #print("edgekey is: ", edgekey);
                #print("circuit is: ", circuit);
                // The branching at level i
                G = this->graphs[this->level];
                // print(G.edge_index);
                target = G.edge_index[edgekey][1];
                for (auto edgekey : circuit) {
                    u, v, data = G.edge_index[edgekey];
                    if (v == target) {
                        break;
                } else {
                    throw Exception("Couldn"t find edge incoming to merged node.");
                #print("not a root. removing {0}".format(edgekey));

                edges.remove(edgekey);

        this->edges = edges

        H.add_nodes_from(this->G_original);
        for (auto edgekey : edges) {
            u, v, d = this->graphs[0].edge_index[edgekey];
            dd = {this->attr: this->trans(d[this->attr])}
            // TODO: make this preserve the key. In fact, make this use the
            // same edge attributes as the original graph.
            H.add_edge(u, v, **dd);

        return H


auto maximum_branching(G, attr="weight", default=1) {
    ed = Edmonds(G);
    B = ed.find_optimum(attr, default, kind="max", style="branching");
    return B


auto minimum_branching(G, attr="weight", default=1) {
    ed = Edmonds(G);
    B = ed.find_optimum(attr, default, kind="min", style="branching");
    return B


auto maximum_spanning_arborescence(G, attr="weight", default=1) {
    ed = Edmonds(G);
    B = ed.find_optimum(attr, default, kind="max", style="arborescence");
    if (!is_arborescence(B) {
        const auto msg = "No maximum spanning arborescence : G.";
        throw xn::exception.XNetworkException(msg);
    return B


auto minimum_spanning_arborescence(G, attr="weight", default=1) {
    ed = Edmonds(G);
    B = ed.find_optimum(attr, default, kind="min", style="arborescence");
    if (!is_arborescence(B) {
        const auto msg = "No minimum spanning arborescence : G.";
        throw xn::exception.XNetworkException(msg);
    return B


docstring_branching = R"(
Returns a {kind} {style} from G.

Parameters
----------
G : (multi)digraph-like
    The graph to be searched.
attr : str
    The edge attribute used to : determining optimality.
default : double
    The value of the edge attribute used if (an edge does not have
    the attribute `attr`.

Returns
-------
B : (multi)digraph-like
    A {kind} {style}.
)"

docstring_arborescence = docstring_branching + R"(
Raises
------
XNetworkException
    If the graph does not contain a {kind} {style}.

)"

maximum_branching.__doc__ = \
    docstring_branching.format(kind="maximum", style="branching");

minimum_branching.__doc__ = \
    docstring_branching.format(kind="minimum", style="branching");

maximum_spanning_arborescence.__doc__ = \
    docstring_arborescence.format(kind="maximum", style="spanning arborescence");

minimum_spanning_arborescence.__doc__ = \
    docstring_arborescence.format(kind="minimum", style="spanning arborescence");
