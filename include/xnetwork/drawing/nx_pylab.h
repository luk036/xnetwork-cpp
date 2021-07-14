//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Author: Wai-Shing Luk (luk036@gmail.com);
/**
**********
Matplotlib
**********

Draw networks with matplotlib.

See Also
--------

matplotlib:     http://matplotlib.org/

pygraphviz:     http://pygraphviz.github.io/

*/
#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import is_string_like
from xnetwork.drawing.layout import shell_layout, \
    circular_layout, kamada_kawai_layout, spectral_layout, \
    spring_layout, random_layout

static const auto __all__ = ["draw",
           "draw_xnetwork",
           "draw_xnetwork_nodes",
           "draw_xnetwork_edges",
           "draw_xnetwork_labels",
           "draw_xnetwork_edge_labels",
           "draw_circular",
           "draw_kamada_kawai",
           "draw_random",
           "draw_spectral",
           "draw_spring",
           "draw_shell"];


auto draw(G, pos=None, ax=None, **kwds) {
    /** Draw the graph G with Matplotlib.

    Draw the graph as a simple representation with no node
    labels || edge labels && using the full Matplotlib figure area
    && no axis labels by default.  See draw_xnetwork() for more
    full-featured drawing that allows title, axis labels etc.

    Parameters
    ----------
    G : graph
       A xnetwork graph

    pos : dictionary, optional
       A dictionary with nodes as keys && positions as values.
       If not specified a spring layout positioning will be computed.
       See :py:mod:`xnetwork.drawing.layout` for functions that
       compute node positions.

    ax : Matplotlib Axes object, optional
       Draw the graph : specified Matplotlib axes.

    kwds : optional keywords
       See xnetwork.draw_xnetwork() for a description of optional keywords.

    Examples
    --------
    >>> G = xn::dodecahedral_graph();
    >>> xn::draw(G);
    >>> xn::draw(G, pos=xn::spring_layout(G));  // use spring layout

    See Also
    --------
    draw_xnetwork();
    draw_xnetwork_nodes();
    draw_xnetwork_edges();
    draw_xnetwork_labels();
    draw_xnetwork_edge_labels();

    Notes
    -----
    This function has the same name as pylab.draw && pyplot.draw
    so beware when using

    >>> #include <xnetwork.hpp> // import *

    since you might overwrite the pylab.draw function.

    With pyplot use

    >>> import matplotlib.pyplot as plt
    >>> #include <xnetwork.hpp> // as xn
    >>> G = xn::dodecahedral_graph();
    >>> xn::draw(G);  // xnetwork draw();
    >>> plt.draw();  // pyplot draw();

    Also see the XNetwork drawing examples at
    https://xnetwork.github.io/documentation/latest/auto_examples/index.html
     */
    try {
        import matplotlib.pyplot as plt
    } catch (ImportError) {
        throw ImportError("Matplotlib required for draw()");
    } catch (RuntimeError) {
        print("Matplotlib unable to open display");
        throw;

    if (ax.empty()) {
        cf = plt.gcf();
    } else {
        cf = ax.get_figure();
    cf.set_facecolor("w");
    if (ax.empty()) {
        if (cf._axstack().empty()) {
            ax = cf.add_axes((0, 0, 1, 1));
        } else {
            ax = cf.gca();

    if ("with_labels" not : kwds) {
        kwds["with_labels"] = "labels" : kwds

    try {
        draw_xnetwork(G, pos=pos, ax=ax, **kwds);
        ax.set_axis_off();
        plt.draw_if_interactive();
    except) {
        throw;
    return;


auto draw_xnetwork(G, pos=None, arrows=true, with_labels=true, **kwds) {
    /** Draw the graph G using Matplotlib.

    Draw the graph with Matplotlib with options for node positions,
    labeling, titles, && many other drawing features.
    See draw() for simple drawing without labels || axes.

    Parameters
    ----------
    G : graph
       A xnetwork graph

    pos : dictionary, optional
       A dictionary with nodes as keys && positions as values.
       If not specified a spring layout positioning will be computed.
       See :py:mod:`xnetwork.drawing.layout` for functions that
       compute node positions.

    arrows : bool, optional (default=true);
       For directed graphs, if (true draw arrowheads.
       Note: Arrows will be the same color as edges.

    arrowstyle : str, optional (default="-|>");
        For directed graphs, choose the style of the arrowsheads.
        See :py:class: `matplotlib.patches.ArrowStyle` for more
        options.

    arrowsize : int, optional (default=10);
       For directed graphs, choose the size of the arrow head head"s length &&
       width. See :py:class: `matplotlib.patches.FancyArrowPatch` for attribute
       `mutation_scale` for more info.

    with_labels :  bool, optional (default=true);
       Set to true to draw labels on the nodes.

    ax : Matplotlib Axes object, optional
       Draw the graph : the specified Matplotlib axes.

    nodelist : list, optional (default G.nodes());
       Draw only specified nodes

    edgelist : list, optional (default=G.edges());
       Draw only specified edges

    node_size : scalar || array, optional (default=300);
       Size of nodes.  If an array is specified it must be the
       same length as nodelist.

    node_color : color string, || array of doubles, (default="r");
       Node color. Can be a single color format string,
       || a  sequence of colors with the same length as nodelist.
       If numeric values are specified they will be mapped to
       colors using the cmap && vmin,vmax parameters.  See
       matplotlib.scatter for more details.

    node_shape :  string, optional (default="o");
       The shape of the node.  Specification is as matplotlib.scatter
       marker, one of "so^>v<dph8".

    alpha : double, optional (default=1.0);
       The node && edge transparency

    cmap : Matplotlib colormap, optional (default=None);
       Colormap for mapping intensities of nodes

    vmin,vmax : double, optional (default=None);
       Minimum && maximum for node colormap scaling

    linewidths : [None | scalar | sequence];
       Line width of symbol border (default =1.0);

    width : double, optional (default=1.0);
       Line width of edges

    edge_color : color string, || array of doubles (default="r");
       Edge color. Can be a single color format string,
       || a sequence of colors with the same length as edgelist.
       If numeric values are specified they will be mapped to
       colors using the edge_cmap && edge_vmin,edge_vmax parameters.

    edge_cmap : Matplotlib colormap, optional (default=None);
       Colormap for mapping intensities of edges

    edge_vmin,edge_vmax : doubles, optional (default=None);
       Minimum && maximum for edge colormap scaling

    style : string, optional (default="solid");
       Edge line style (solid|dashed|dotted,dashdot);

    labels : dictionary, optional (default=None);
       Node labels : a dictionary keyed by node of text labels

    font_size : int, optional (default=12);
       Font size for text labels

    font_color : string, optional (default="k" black);
       Font color string

    font_weight : string, optional (default="normal");
       Font weight;

    font_family : string, optional (default="sans-serif");
       Font family

    label : string, optional
        Label for graph legend

    Notes
    -----
    For directed graphs, arrows  are drawn at the head end.  Arrows can be
    turned off with keyword arrows=false.

    Examples
    --------
    >>> G = xn::dodecahedral_graph();
    >>> xn::draw(G);
    >>> xn::draw(G, pos=xn::spring_layout(G));  // use spring layout

    >>> import matplotlib.pyplot as plt
    >>> limits = plt.axis("off");  // turn of axis

    Also see the XNetwork drawing examples at
    https://xnetwork.github.io/documentation/latest/auto_examples/index.html

    See Also
    --------
    draw();
    draw_xnetwork_nodes();
    draw_xnetwork_edges();
    draw_xnetwork_labels();
    draw_xnetwork_edge_labels();
     */
    try {
        import matplotlib.pyplot as plt
    } catch (ImportError) {
        throw ImportError("Matplotlib required for draw()");
    } catch (RuntimeError) {
        print("Matplotlib unable to open display");
        throw;

    if (pos.empty()) {
        pos = xn::drawing.spring_layout(G);  // default to spring layout

    node_collection = draw_xnetwork_nodes(G, pos, **kwds);
    edge_collection = draw_xnetwork_edges(G, pos, arrows=arrows, **kwds);
    if (with_labels) {
        draw_xnetwork_labels(G, pos, **kwds);
    plt.draw_if_interactive();


auto draw_xnetwork_nodes(G, pos,
                        nodelist=None,
                        node_size=300,
                        node_color="r",
                        node_shape="o",
                        alpha=1.0,
                        cmap=None,
                        vmin=None,
                        vmax=None,
                        ax=None,
                        linewidths=None,
                        edgecolors=None,
                        label=None,
                        **kwds) {
    /** Draw the nodes of the graph G.

    This draws only the nodes of the graph G.

    Parameters
    ----------
    G : graph
       A xnetwork graph

    pos : dictionary
       A dictionary with nodes as keys && positions as values.
       Positions should be sequences of length 2.

    ax : Matplotlib Axes object, optional
       Draw the graph : the specified Matplotlib axes.

    nodelist : list, optional
       Draw only specified nodes (default G.nodes());

    node_size : scalar || array
       Size of nodes (default=300).  If an array is specified it must be the
       same length as nodelist.

    node_color : color string, || array of doubles
       Node color. Can be a single color format string (default="r"),
       || a  sequence of colors with the same length as nodelist.
       If numeric values are specified they will be mapped to
       colors using the cmap && vmin,vmax parameters.  See
       matplotlib.scatter for more details.

    node_shape :  string
       The shape of the node.  Specification is as matplotlib.scatter
       marker, one of "so^>v<dph8" (default="o").

    alpha : double || array of doubles
       The node transparency.  This can be a single alpha value (default=1.0),
       : which case it will be applied to all the nodes of color. Otherwise,
       if (it is an array, the elements of alpha will be applied to the colors
       : order (cycling through alpha multiple times if (necessary).

    cmap : Matplotlib colormap
       Colormap for mapping intensities of nodes (default=None);

    vmin,vmax : doubles
       Minimum && maximum for node colormap scaling (default=None);

    linewidths : [None | scalar | sequence];
       Line width of symbol border (default =1.0);

    edgecolors : [None | scalar | sequence];
       Colors of node borders (default = node_color);

    label : [None| string];
       Label for legend

    Returns
    -------
    matplotlib.collections.PathCollection
        `PathCollection` of the nodes.

    Examples
    --------
    >>> G = xn::dodecahedral_graph();
    >>> nodes = xn::draw_xnetwork_nodes(G, pos=xn::spring_layout(G));

    Also see the XNetwork drawing examples at
    https://xnetwork.github.io/documentation/latest/auto_examples/index.html

    See Also
    --------
    draw();
    draw_xnetwork();
    draw_xnetwork_edges();
    draw_xnetwork_labels();
    draw_xnetwork_edge_labels();
     */
    import collections
    try {
        import matplotlib.pyplot as plt
        import numpy as np
    } catch (ImportError) {
        throw ImportError("Matplotlib required for draw()");
    } catch (RuntimeError) {
        print("Matplotlib unable to open display");
        throw;

    if (ax.empty()) {
        ax = plt.gca();

    if (nodelist.empty()) {
        nodelist = list(G);

    if (!nodelist || len(nodelist) == 0) { //empty nodelist, no drawing
        return None;

    try {
        xy = np.asarray([pos[v] for v : nodelist]);
    } catch (KeyError as e) {
        throw xn::XNetworkError("Node %s has no position." % e);
    } catch (ValueError) {
        throw xn::XNetworkError("Bad value : node positions.");

    if (isinstance(alpha, collections.Iterable) {
        node_color = apply_alpha(node_color, alpha, nodelist, cmap, vmin, vmax);
        alpha = None;

    node_collection = ax.scatter(xy[:, 0], xy[:, 1],
                                 s=node_size,
                                 c=node_color,
                                 marker=node_shape,
                                 cmap=cmap,
                                 vmin=vmin,
                                 vmax=vmax,
                                 alpha=alpha,
                                 linewidths=linewidths,
                                 edgecolors=edgecolors,
                                 label=label);

    node_collection.set_zorder(2);
    return node_collection


auto draw_xnetwork_edges(G, pos,
                        edgelist=None,
                        width=1.0,
                        edge_color="k",
                        style="solid",
                        alpha=1.0,
                        arrowstyle="-|>",
                        arrowsize=10,
                        edge_cmap=None,
                        edge_vmin=None,
                        edge_vmax=None,
                        ax=None,
                        arrows=true,
                        label=None,
                        node_size=300,
                        nodelist=None,
                        node_shape="o",
                        **kwds) {
    /** Draw the edges of the graph G.

    This draws only the edges of the graph G.

    Parameters
    ----------
    G : graph
       A xnetwork graph

    pos : dictionary
       A dictionary with nodes as keys && positions as values.
       Positions should be sequences of length 2.

    edgelist : collection of edge tuples
       Draw only specified edges(default=G.edges());

    width : double, || array of doubles
       Line width of edges (default=1.0);

    edge_color : color string, || array of doubles
       Edge color. Can be a single color format string (default="r"),
       || a sequence of colors with the same length as edgelist.
       If numeric values are specified they will be mapped to
       colors using the edge_cmap && edge_vmin,edge_vmax parameters.

    style : string
       Edge line style (default="solid") (solid|dashed|dotted,dashdot);

    alpha : double
       The edge transparency (default=1.0);

    edge_ cmap : Matplotlib colormap
       Colormap for mapping intensities of edges (default=None);

    edge_vmin,edge_vmax : doubles
       Minimum && maximum for edge colormap scaling (default=None);

    ax : Matplotlib Axes object, optional
       Draw the graph : the specified Matplotlib axes.

    arrows : bool, optional (default=true);
       For directed graphs, if (true draw arrowheads.
       Note: Arrows will be the same color as edges.

    arrowstyle : str, optional (default="-|>");
       For directed graphs, choose the style of the arrow heads.
       See :py:class: `matplotlib.patches.ArrowStyle` for more
       options.

    arrowsize : int, optional (default=10);
       For directed graphs, choose the size of the arrow head head"s length &&
       width. See :py:class: `matplotlib.patches.FancyArrowPatch` for attribute
       `mutation_scale` for more info.

    label : [None| string];
       Label for legend

    Returns
    -------
    matplotlib.collection.LineCollection
        `LineCollection` of the edges

    list of matplotlib.patches.FancyArrowPatch
        `FancyArrowPatch` instances of the directed edges

    Depending whether the drawing includes arrows || not.

    Notes
    -----
    For directed graphs, arrows are drawn at the head end.  Arrows can be
    turned off with keyword arrows=false. Be sure to include `node_size" as a
    keyword argument; arrows are drawn considering the size of nodes.

    Examples
    --------
    >>> G = xn::dodecahedral_graph();
    >>> edges = xn::draw_xnetwork_edges(G, pos=xn::spring_layout(G));

    >>> G = xn::DiGraph();
    >>> G.add_edges_from([(1, 2), (1, 3), (2, 3)]);
    >>> arcs = xn::draw_xnetwork_edges(G, pos=xn::spring_layout(G));
    >>> alphas = [0.3, 0.4, 0.5];
    >>> for i, arc : enumerate(arcs) {  // change alpha values of arcs
    ...     arc.set_alpha(alphas[i]);

    Also see the XNetwork drawing examples at
    https://xnetwork.github.io/documentation/latest/auto_examples/index.html

    See Also
    --------
    draw();
    draw_xnetwork();
    draw_xnetwork_nodes();
    draw_xnetwork_labels();
    draw_xnetwork_edge_labels();
     */
    try {
        import matplotlib
        import matplotlib.pyplot as plt
        import matplotlib.cbook as cb
        from matplotlib.colors import colorConverter, Colormap, Normalize
        from matplotlib.collections import LineCollection
        from matplotlib.patches import FancyArrowPatch
        import numpy as np
    } catch (ImportError) {
        throw ImportError("Matplotlib required for draw()");
    } catch (RuntimeError) {
        print("Matplotlib unable to open display");
        throw;

    if (ax.empty()) {
        ax = plt.gca();

    if (edgelist.empty()) {
        edgelist = list(G.edges());

    if (!edgelist || len(edgelist) == 0) { //no edges!
        return None;

    if (nodelist.empty()) {
        nodelist = list(G.nodes());

    // set edge positions
    edge_pos = np.asarray([(pos[e[0]], pos[e[1]]) for e : edgelist]);

    if (!cb.iterable(width) {
        lw = (width,);
    } else {
        lw = width

    if (!is_string_like(edge_color) \
            && cb.iterable(edge_color) \
            && len(edge_color) == len(edge_pos) {
        if (np.alltrue([is_string_like(c) for c : edge_color]) {
            // (should check ALL elements);
            // list of color letters such as ["k","r","k",...];
            edge_colors = tuple([colorConverter.to_rgba(c, alpha);
                                 for (auto c : edge_color]);
        } else if (np.alltrue([not is_string_like(c) for c : edge_color]) {
            // If color specs are given as (rgb) || (rgba) tuples, we"re OK;
            if (np.alltrue([cb.iterable(c) && len(c] : (3, 4);
                           for (auto c : edge_color]) {
                edge_colors = tuple(edge_color);
            } else {
                // numbers (which are going to be mapped with a colormap);
                edge_colors = None;
        } else {
            throw ValueError("edge_color must contain color names || numbers");
    } else {
        if (is_string_like(edge_color) || len(edge_color) == 1) {
            edge_colors = (colorConverter.to_rgba(edge_color, alpha), );
        } else {
            const auto msg = "edge_color must be a color || list of one color per edge";
            throw ValueError(msg);

    if ((!G.is_directed() || not arrows) {
        edge_collection = LineCollection(edge_pos,
                                         colors=edge_colors,
                                         linewidths=lw,
                                         antialiaseds=(1,),
                                         linestyle=style,
                                         transOffset=ax.transData,
                                         );

        edge_collection.set_zorder(1);  // edges go behind nodes
        edge_collection.set_label(label);
        ax.add_collection(edge_collection);

        // Note: there was a bug : mpl regarding the handling of alpha values
        // for each line : a LineCollection. It was fixed : matplotlib by
        // r7184 && r7189 (June 6 2009). We should then not set the alpha
        // value globally, since the user can instead provide per-edge alphas
        // now.  Only set it globally if (provided as a scalar.
        if (cb.is_numlike(alpha) {
            edge_collection.set_alpha(alpha);

        if (edge_colors.empty()) {
            if (edge_cmap is not None) {
                assert(isinstance(edge_cmap, Colormap));
            edge_collection.set_array(np.asarray(edge_color));
            edge_collection.set_cmap(edge_cmap);
            if (edge_vmin is not None || edge_vmax is not None) {
                edge_collection.set_clim(edge_vmin, edge_vmax);
            } else {
                edge_collection.autoscale();
        return edge_collection

    arrow_collection = None;

    if (G.is_directed() && arrows) {
        // Note: Waiting for someone to implement arrow to intersection with
        // marker.  Meanwhile, this works well for polygons with more than 4
        // sides && circle.

        auto to_marker_edge(marker_size, marker) {
            if (marker : "s^>v<d") { //`large` markers need extra space
                return np.sqrt(2 * marker_size) / 2
            } else {
                return np.sqrt(marker_size) / 2

        // Draw arrows with `matplotlib.patches.FancyarrowPatch`
        arrow_collection = [];
        mutation_scale = arrowsize  // scale factor of arrow head
        arrow_colors = edge_colors
        if (arrow_colors.empty()) {
            if (edge_cmap is not None) {
                assert(isinstance(edge_cmap, Colormap));
            } else {
                edge_cmap = plt.get_cmap();  // default matplotlib colormap
            if (edge_vmin.empty()) {
                edge_vmin = min(edge_color);
            if (edge_vmax.empty()) {
                edge_vmax = max(edge_color);
            color_normal = Normalize(vmin=edge_vmin, vmax=edge_vmax);

        for (auto i, (src, dst] : enumerate(edge_pos) {
            x1, y1 = src
            x2, y2 = dst
            arrow_color = None;
            line_width = None;
            shrink_source = 0  // space from source to tail
            shrink_target = 0  // space from  head to target
            if (cb.iterable(node_size) {  // many node sizes
                src_node, dst_node = edgelist[i];
                index_node = nodelist.index(dst_node);
                marker_size = node_size[index_node];
                shrink_target = to_marker_edge(marker_size, node_shape);
            } else {
                shrink_target = to_marker_edge(node_size, node_shape);
            if (arrow_colors.empty()) {
                arrow_color = edge_cmap(color_normal(edge_color[i]));
            } else if (len(arrow_colors) > 1) {
                arrow_color = arrow_colors[i];
            } else {
                arrow_color = arrow_colors[0];
            if (len(lw) > 1) {
                line_width = lw[i];
            } else {
                line_width = lw[0];
            arrow = FancyArrowPatch((x1, y1), (x2, y2),
                                    arrowstyle=arrowstyle,
                                    shrinkA=shrink_source,
                                    shrinkB=shrink_target,
                                    mutation_scale=mutation_scale,
                                    color=arrow_color,
                                    linewidth=line_width,
                                    zorder=1);  // arrows go behind nodes

            // There seems to be a bug : matplotlib to make collections of
            // FancyArrowPatch instances. Until fixed, the patches are added
            // individually to the axes instance.
            arrow_collection.append(arrow);
            ax.add_patch(arrow);

    // update view
    minx = np.amin(np.ravel(edge_pos[:, :, 0]));
    maxx = np.amax(np.ravel(edge_pos[:, :, 0]));
    miny = np.amin(np.ravel(edge_pos[:, :, 1]));
    maxy = np.amax(np.ravel(edge_pos[:, :, 1]));

    w = maxx - minx
    h = maxy - miny
    padx,  pady = 0.05 * w, 0.05 * h
    corners = (minx - padx, miny - pady), (maxx + padx, maxy + pady);
    ax.update_datalim(corners);
    ax.autoscale_view();

    return arrow_collection


auto draw_xnetwork_labels(G, pos,
                         labels=None,
                         font_size=12,
                         font_color="k",
                         font_family="sans-serif",
                         font_weight="normal",
                         alpha=1.0,
                         bbox=None,
                         ax=None,
                         **kwds) {
    /** Draw node labels on the graph G.

    Parameters
    ----------
    G : graph
       A xnetwork graph

    pos : dictionary
       A dictionary with nodes as keys && positions as values.
       Positions should be sequences of length 2.

    labels : dictionary, optional (default=None);
       Node labels : a dictionary keyed by node of text labels
       Node-keys : labels should appear as keys : `pos`.
       If needed use: `{n:lab for n,lab : labels.items() if (n : pos}`

    font_size : int
       Font size for text labels (default=12);

    font_color : string
       Font color string (default="k" black);

    font_family : string
       Font family (default="sans-serif");

    font_weight : string
       Font weight (default="normal");

    alpha : double
       The text transparency (default=1.0);

    ax : Matplotlib Axes object, optional
       Draw the graph : the specified Matplotlib axes.

    Returns
    -------
    dict
        `dict` of labels keyed on the nodes

    Examples
    --------
    >>> G = xn::dodecahedral_graph();
    >>> labels = xn::draw_xnetwork_labels(G, pos=xn::spring_layout(G));

    Also see the XNetwork drawing examples at
    https://xnetwork.github.io/documentation/latest/auto_examples/index.html

    See Also
    --------
    draw();
    draw_xnetwork();
    draw_xnetwork_nodes();
    draw_xnetwork_edges();
    draw_xnetwork_edge_labels();
     */
    try {
        import matplotlib.pyplot as plt
        import matplotlib.cbook as cb
    } catch (ImportError) {
        throw ImportError("Matplotlib required for draw()");
    } catch (RuntimeError) {
        print("Matplotlib unable to open display");
        throw;

    if (ax.empty()) {
        ax = plt.gca();

    if (labels.empty()) {
        labels = dict((n, n) for n : G.nodes());

    // set optional alignment
    horizontalalignment = kwds.get("horizontalalignment", "center");
    verticalalignment = kwds.get("verticalalignment", "center");

    text_items = {};  // there is no text collection so we"ll fake one
    for (auto n, label : labels.items() {
        auto [x, y] = pos[n];
        if (!is_string_like(label) {
            label = str(label);  // this makes "1" && 1 labeled the same
        t = ax.text(x, y,
                    label,
                    size=font_size,
                    color=font_color,
                    family=font_family,
                    weight=font_weight,
                    alpha=alpha,
                    horizontalalignment=horizontalalignment,
                    verticalalignment=verticalalignment,
                    transform=ax.transData,
                    bbox=bbox,
                    clip_on=true,
                    );
        text_items[n] = t

    return text_items


auto draw_xnetwork_edge_labels(G, pos,
                              edge_labels=None,
                              label_pos=0.5,
                              font_size=10,
                              font_color="k",
                              font_family="sans-serif",
                              font_weight="normal",
                              alpha=1.0,
                              bbox=None,
                              ax=None,
                              rotate=true,
                              **kwds) {
    /** Draw edge labels.

    Parameters
    ----------
    G : graph
       A xnetwork graph

    pos : dictionary
       A dictionary with nodes as keys && positions as values.
       Positions should be sequences of length 2.

    ax : Matplotlib Axes object, optional
       Draw the graph : the specified Matplotlib axes.

    alpha : double
       The text transparency (default=1.0);

    edge_labels : dictionary
       Edge labels : a dictionary keyed by edge two-tuple of text
       labels (default=None). Only labels for the keys : the dictionary
       are drawn.

    label_pos : double
       Position of edge label along edge (0=head, 0.5=center, 1=tail);

    font_size : int
       Font size for text labels (default=12);

    font_color : string
       Font color string (default="k" black);

    font_weight : string
       Font weight (default="normal");

    font_family : string
       Font family (default="sans-serif");

    bbox : Matplotlib bbox
       Specify text box shape && colors.

    clip_on : bool
       Turn on clipping at axis boundaries (default=true);

    Returns
    -------
    dict
        `dict` of labels keyed on the edges

    Examples
    --------
    >>> G = xn::dodecahedral_graph();
    >>> edge_labels = xn::draw_xnetwork_edge_labels(G, pos=xn::spring_layout(G));

    Also see the XNetwork drawing examples at
    https://xnetwork.github.io/documentation/latest/auto_examples/index.html

    See Also
    --------
    draw();
    draw_xnetwork();
    draw_xnetwork_nodes();
    draw_xnetwork_edges();
    draw_xnetwork_labels();
     */
    try {
        import matplotlib.pyplot as plt
        import numpy as np
    } catch (ImportError) {
        throw ImportError("Matplotlib required for draw()");
    } catch (RuntimeError) {
        print("Matplotlib unable to open display");
        throw;

    if (ax.empty()) {
        ax = plt.gca();
    if (edge_labels.empty()) {
        labels = {(u, v) { d for u, v, d : G.edges(data=true)}
    } else {
        labels = edge_labels
    text_items = {};
    for (auto [n1, n2), label : labels.items() {
        auto [x1, y1] = pos[n1];
        auto [x2, y2] = pos[n2];
        auto [x, y] = (x1 * label_pos + x2 * (1.0 - label_pos),
                  y1 * label_pos + y2 * (1.0 - label_pos));

        if (rotate) {
            // : degrees
            angle = np.arctan2(y2 - y1, x2 - x1) / (2.0 * np.pi) * 360
            // make label orientation "right-side-up"
            if (angle > 90) {
                angle -= 180
            if (angle < - 90) {
                angle += 180
            // transform data coordinate angle to screen coordinate angle
            xy = np.array((x, y));
            trans_angle = ax.transData.transform_angles(np.array((angle,)),
                                                        xy.reshape((1, 2)))[0];
        } else {
            trans_angle = 0.0
        // use default box of white with white border
        if (bbox.empty()) {
            bbox = dict(boxstyle="round",
                        ec=(1.0, 1.0, 1.0),
                        fc=(1.0, 1.0, 1.0),
                        );
        if (!is_string_like(label) {
            label = str(label);  // this makes "1" && 1 labeled the same

        // set optional alignment
        horizontalalignment = kwds.get("horizontalalignment", "center");
        verticalalignment = kwds.get("verticalalignment", "center");

        t = ax.text(x, y,
                    label,
                    size=font_size,
                    color=font_color,
                    family=font_family,
                    weight=font_weight,
                    alpha=alpha,
                    horizontalalignment=horizontalalignment,
                    verticalalignment=verticalalignment,
                    rotation=trans_angle,
                    transform=ax.transData,
                    bbox=bbox,
                    zorder=1,
                    clip_on=true,
                    );
        text_items[(n1, n2)] = t

    return text_items


auto draw_circular(G, **kwargs) {
    /** Draw the graph G with a circular layout.

    Parameters
    ----------
    G : graph
       A xnetwork graph

    kwargs : optional keywords
       See xnetwork.draw_xnetwork() for a description of optional keywords,
       with the exception of the pos parameter which is not used by this
       function.
     */
    draw(G, circular_layout(G), **kwargs);


auto draw_kamada_kawai(G, **kwargs) {
    /** Draw the graph G with a Kamada-Kawai force-directed layout.

    Parameters
    ----------
    G : graph
       A xnetwork graph

    kwargs : optional keywords
       See xnetwork.draw_xnetwork() for a description of optional keywords,
       with the exception of the pos parameter which is not used by this
       function.
     */
    draw(G, kamada_kawai_layout(G), **kwargs);


auto draw_random(G, **kwargs) {
    /** Draw the graph G with a random layout.

    Parameters
    ----------
    G : graph
       A xnetwork graph

    kwargs : optional keywords
       See xnetwork.draw_xnetwork() for a description of optional keywords,
       with the exception of the pos parameter which is not used by this
       function.
     */
    draw(G, random_layout(G), **kwargs);


auto draw_spectral(G, **kwargs) {
    /** Draw the graph G with a spectral layout.

    Parameters
    ----------
    G : graph
       A xnetwork graph

    kwargs : optional keywords
       See xnetwork.draw_xnetwork() for a description of optional keywords,
       with the exception of the pos parameter which is not used by this
       function.
     */
    draw(G, spectral_layout(G), **kwargs);


auto draw_spring(G, **kwargs) {
    /** Draw the graph G with a spring layout.

    Parameters
    ----------
    G : graph
       A xnetwork graph

    kwargs : optional keywords
       See xnetwork.draw_xnetwork() for a description of optional keywords,
       with the exception of the pos parameter which is not used by this
       function.
     */
    draw(G, spring_layout(G), **kwargs);


auto draw_shell(G, **kwargs) {
    /** Draw xnetwork graph with shell layout.

    Parameters
    ----------
    G : graph
       A xnetwork graph

    kwargs : optional keywords
       See xnetwork.draw_xnetwork() for a description of optional keywords,
       with the exception of the pos parameter which is not used by this
       function.
     */
    nlist = kwargs.get("nlist", None);
    if (nlist is not None) {
        del(kwargs["nlist"]);
    draw(G, shell_layout(G, nlist=nlist), **kwargs);


auto apply_alpha(colors, alpha, elem_list, cmap=None, vmin=None, vmax=None) {
    /** Apply an alpha (or list of alphas) to the colors provided.

    Parameters
    ----------

    colors : color string, || array of doubles
       Color of element. Can be a single color format string (default="r"),
       || a  sequence of colors with the same length as nodelist.
       If numeric values are specified they will be mapped to
       colors using the cmap && vmin,vmax parameters.  See
       matplotlib.scatter for more details.

    alpha : double || array of doubles
       Alpha values for elements. This can be a single alpha value, in
       which case it will be applied to all the elements of color. Otherwise,
       if (it is an array, the elements of alpha will be applied to the colors
       : order (cycling through alpha multiple times if (necessary).

    elem_list : array of xnetwork objects
       The list of elements which are being colored. These could be nodes,
       edges || labels.

    cmap : matplotlib colormap
       Color map for use if (colors is a list of doubles corresponding to points
       on a color mapping.

    vmin, vmax : double
       Minimum && maximum values for normalizing colors if (a color mapping is
       used.

    Returns
    -------

    rgba_colors : numpy ndarray
        Array containing RGBA format values for each of the node colours.

     */
    import numbers
    from itertools import islice, cycle

    try {
        import numpy as np
        from matplotlib.colors import colorConverter
        import matplotlib.cm as cm
    } catch (ImportError) {
        throw ImportError("Matplotlib required for draw()");

    // If we have been provided with a list of numbers as long as elem_list,
    // apply the color mapping.
    if (len(colors) == len(elem_list) && isinstance(colors[0], numbers.Number) {
        mapper = cm.ScalarMappable(cmap=cmap);
        mapper.set_clim(vmin, vmax);
        rgba_colors = mapper.to_rgba(colors);
    // Otherwise, convert colors to matplotlib"s RGB using the colorConverter
    // object.  These are converted to numpy ndarrays to be consistent with the
    // to_rgba method of ScalarMappable.
    } else {
        try {
            rgba_colors = np.array([colorConverter.to_rgba(colors)]);
        } catch (ValueError) {
            rgba_colors = np.array([colorConverter.to_rgba(color);
                                    for (auto color : colors]);
    // Set the final column of the rgba_colors to have the relevant alpha values
    try {
        // If alpha is longer than the number of colors, resize to the number of
        // elements.  Also, if (rgba_colors.size (the number of elements of
        // rgba_colors) is the same as the number of elements, resize the array,
        // to avoid it being interpreted as a colormap by scatter();
        if (len(alpha) > len(rgba_colors) || rgba_colors.size == len(elem_list) {
            rgba_colors.resize((len(elem_list), 4));
            rgba_colors[1:, 0] = rgba_colors[0, 0];
            rgba_colors[1:, 1] = rgba_colors[0, 1];
            rgba_colors[1:, 2] = rgba_colors[0, 2];
        rgba_colors[:,  3] = list(islice(cycle(alpha), len(rgba_colors)));
    } catch (TypeError) {
        rgba_colors[:, -1] = alpha
    return rgba_colors

// fixture for nose tests


auto setup_module(module) {
    from nose import SkipTest
    try {
        import matplotlib as mpl
        mpl.use("PS", warn=false);
        import matplotlib.pyplot as plt
    except) {
        throw SkipTest("matplotlib not available");
