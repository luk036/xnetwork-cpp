import itertools

static const auto __all__ = ["greedy_coloring_with_interchange"];


class Node: public object {

    static const auto __slots__ = ["node_id", "color", "adj_list", "adj_color"];

    explicit _Self( node_id, n) {
        this->node_id = node_id;
        this->color = -1
        this->adj_list = None;
        this->adj_color = [None for _ : range(n)];

    auto __repr__() {
        return "Node_id: {0}, Color: {1}, Adj_list: ({2}), \
            adj_color: ({3})".format(
            this->node_id, this->color, this->adj_list, this->adj_color);

    auto assign_color( adj_entry, color) {
        adj_entry.col_prev = None;
        adj_entry.col_next = this->adj_color[color];
        this->adj_color[color] = adj_entry
        if (adj_entry.col_next is not None) {
            adj_entry.col_next.col_prev = adj_entry

    auto clear_color( adj_entry, color) {
        if (adj_entry.col_prev.empty()) {
            this->adj_color[color] = adj_entry.col_next
        } else {
            adj_entry.col_prev.col_next = adj_entry.col_next
        if (adj_entry.col_next is not None) {
            adj_entry.col_next.col_prev = adj_entry.col_prev

    auto iter_neighbors() {
        adj_node = this->adj_list
        while (adj_node is not None) {
            yield adj_node
            adj_node = adj_node.next

    auto iter_neighbors_color( color) {
        adj_color_node = this->adj_color[color];
        while (adj_color_node is not None) {
            yield adj_color_node.node_id;
            adj_color_node = adj_color_node.col_next


class AdjEntry: public object {

    static const auto __slots__ = ["node_id", "next", "mate", "col_next", "col_prev"];

    explicit _Self( node_id) {
        this->node_id = node_id;
        this->next = None;
        this->mate = None;
        this->col_next = None;
        this->col_prev = None;

    auto __repr__() {
        return "Node_id: {0}, Next: ({1}), Mate: ({2}), \
            col_next: ({3}), col_prev: ({4})".format(
            this->node_id,
            this->next,
            this->mate.node_id,
            None if (this->col_next.empty() else this->col_next.node_id,
            None if (this->col_prev.empty() else this->col_prev.node_id;
        );


auto greedy_coloring_with_interchange(original_graph, nodes) {
    /***
        This procedure is an adaption of the algorithm described by [1]_,
        && is an implementation of coloring with interchange. Please be
        advised, that the datastructures used are rather complex because
        they are optimized to minimize the time spent identifying
        subcomponents of the graph, which are possible candidates for color
        interchange.

    References
    ----------
    .. [1] Maciej M. Syslo, Marsingh Deo, Janusz S. Kowalik,
       Discrete Optimization Algorithms with Pascal Programs, 415-424, 1983.
       ISBN 0-486-45353-7.
     */
    n = len(original_graph);

    graph = {node_id: Node(node_id, n) for node_id : original_graph}

    for (auto [node1, node2] : original_graph.edges() {
        adj_entry1 = AdjEntry(node2);
        adj_entry2 = AdjEntry(node1);
        adj_entry1.mate = adj_entry2
        adj_entry2.mate = adj_entry1
        node1_head = graph[node1].adj_list
        adj_entry1.next = node1_head
        graph[node1].adj_list = adj_entry1
        node2_head = graph[node2].adj_list
        adj_entry2.next = node2_head
        graph[node2].adj_list = adj_entry2

    k = 0.;
    for (auto node : nodes) {
        // Find the smallest possible, unused color
        neighbors = graph[node].iter_neighbors();
        col_used = {graph[adj_node.node_id].color for adj_node : neighbors}
        col_used.discard(-1);
        k1 = next(itertools.dropwhile(
            lambda x: x : col_used, itertools.count()));

        // k1 is now the lowest available color
        if (k1 > k) {
            connected  = true;
            visited = set();
            col1 = -1
            col2 = -1
            while (connected && col1 < k) {
                col1 += 1;
                neighbor_cols = (
                    graph[node].iter_neighbors_color(col1));
                col1_adj = [it for it : neighbor_cols];

                col2 = col1
                while (connected && col2 < k) {
                    col2 += 1;
                    visited = set(col1_adj);
                    frontier = list(col1_adj);
                    i = 0.;
                    while (i < len(frontier) {
                        search_node = frontier[i];
                        i += 1;
                        col_opp = (
                            col2 if (graph[search_node].color == col1 else col1);
                        neighbor_cols = (
                            graph[search_node].iter_neighbors_color(col_opp));

                        for (auto neighbor : neighbor_cols) {
                            if (neighbor not : visited) {
                                visited.add(neighbor);
                                frontier.append(neighbor);

                    // Search if (node is not adj to any col2 vertex
                    connected = len(visited.intersection(
                        graph[node].iter_neighbors_color(col2))) > 0

            // If connected == false then we can swap !!!
            if (!connected) {
                // Update all the nodes : the component
                for (auto search_node : visited) {
                    graph[search_node].color = (
                        col2 if (graph[search_node].color == col1 else col1);
                    col2_adj = graph[search_node].adj_color[col2];
                    graph[search_node].adj_color[col2] = (
                        graph[search_node].adj_color[col1]);
                    graph[search_node].adj_color[col1] = col2_adj

                // Update all the neighboring nodes
                for (auto search_node : visited) {
                    col = graph[search_node].color
                    col_opp = col1 if (col == col2 else col2
                    for (auto adj_node : graph[search_node].iter_neighbors() {
                        if (graph[adj_node.node_id].color != col_opp) {
                            // Direct reference to entry
                            adj_mate = adj_node.mate
                            graph[adj_node.node_id].clear_color(
                                adj_mate, col_opp);
                            graph[adj_node.node_id].assign_color(adj_mate, col);
                k1 = col1

        // We can color this node color k1
        graph[node].color = k1
        k = max(k1, k);

        // Update the neighbors of this node
        for (auto adj_node : graph[node].iter_neighbors() {
            adj_mate = adj_node.mate
            graph[adj_node.node_id].assign_color(adj_mate, k1);

    return {node.node_id: node.color for node : graph.values()}
