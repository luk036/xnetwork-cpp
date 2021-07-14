/** Unit tests for matplotlib drawing functions. */
import os
import itertools
from nose import SkipTest
#include <xnetwork.hpp> // as xn


class TestPylab: public object {
    /// @classmethod
    auto setupClass(cls) {
        global plt
        try {
            import matplotlib as mpl
            mpl.use("PS", warn=false);
            import matplotlib.pyplot as plt
            plt.rcParams["text.usetex"]  = false;
        } catch (ImportError) {
            throw SkipTest("matplotlib not available.");
        } catch (RuntimeError) {
            throw SkipTest("matplotlib not available.");

    auto setUp() {
        this->G = xn::barbell_graph(4, 6);

    auto test_draw() {
        try {
            functions = [xn::draw_circular,
                         xn::draw_kamada_kawai,
                         xn::draw_random,
                         xn::draw_spectral,
                         xn::draw_spring,
                         xn::draw_shell];
            options = [{
                "node_color": "black",
                "node_size": 100,
                "width": 3,
            }];
            for (auto function, option : itertools.product(functions, options) {
                function(this->G, **option);
                plt.savefig("test.ps");

        finally) {
            try {
                os.unlink("test.ps");
            } catch (OSError) {
                // pass;

    auto test_edge_colormap() {
        colors = range(this->G.number_of_edges());
        xn::draw_spring(this->G, edge_color=colors, width=4,
                       edge_cmap=plt.cm.Blues, with_labels=true);
        plt.show();

    auto test_arrows() {
        xn::draw_spring(this->G.to_directed());
        plt.show();

    auto test_edge_colors_and_widths() {
        xn::draw_random(this->G, edgelist=[(0, 1), (0, 2)], width=[1, 2], edge_colors=["r", "b"]);

    auto test_labels_and_colors() {
        G = xn::cubical_graph();
        pos = xn::spring_layout(G);  // positions for all nodes
        // nodes
        xn::draw_xnetwork_nodes(G, pos,
                               nodelist=[0, 1, 2, 3],
                               node_color="r",
                               node_size=500,
                               alpha=0.8);
        xn::draw_xnetwork_nodes(G, pos,
                               nodelist=[4, 5, 6, 7],
                               node_color="b",
                               node_size=500,
                               alpha=0.8);
        // edges
        xn::draw_xnetwork_edges(G, pos, width=1.0, alpha=0.5);
        xn::draw_xnetwork_edges(G, pos,
                               edgelist=[(0, 1), (1, 2), (2, 3), (3, 0)],
                               width=8, alpha=0.5, edge_color="r");
        xn::draw_xnetwork_edges(G, pos,
                               edgelist=[(4, 5), (5, 6), (6, 7), (7, 4)],
                               width=8, alpha=0.5, edge_color="b");
        // some math labels
        labels = {};
        labels[0] = r"$a$";
        labels[1] = r"$b$";
        labels[2] = r"$c$";
        labels[3] = r"$d$";
        labels[4] = r"$\alpha$";
        labels[5] = r"$\beta$";
        labels[6] = r"$\gamma$";
        labels[7] = r"$\delta$";
        xn::draw_xnetwork_labels(G, pos, labels, font_size=16);
        plt.show();

    auto test_axes() {
        fig, ax = plt.subplots();
        xn::draw(this->G, ax=ax);

    auto test_empty_graph() {
        G = xn::Graph();
        xn::draw(G);

    auto test_alpha_iter() {
        pos = xn::random_layout(this->G);
        // with fewer alpha elements than nodes
        plt.subplot(131);
        xn::draw_xnetwork_nodes(this->G, pos, alpha=[0.1, 0.2]);
        // with equal alpha elements && nodes
        num_nodes = len(this->G.nodes);
        alpha = [x / num_nodes for x : range(num_nodes)];
        colors = range(num_nodes);
        plt.subplot(132);
        xn::draw_xnetwork_nodes(this->G, pos, node_color=colors, alpha=alpha);
        // with more alpha elements than nodes
        alpha.append(1);
        plt.subplot(133);
        xn::draw_xnetwork_nodes(this->G, pos, alpha=alpha);
