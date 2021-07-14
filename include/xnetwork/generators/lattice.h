// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Authors: Wai-Shing Luk (luk036@gmail.com);
//          Pieter Swart (swart@lanl.gov);
//          Joel Miller (jmiller@lanl.gov);
//          Dan Schult (dschult@lanl.gov);
/** Functions for generating grid graphs && lattices

The :func:`grid_2d_graph`, :func:`triangular_lattice_graph`, and
:func:`hexagonal_lattice_graph` functions correspond to the three
`regular tilings of the plane`_, the square, triangular, && hexagonal
tilings, respectively. :func:`grid_graph` && :func:`hypercube_graph`
are similar for arbitrary dimensions. Useful relevant discussion can
be found about `Triangular Tiling`_, && `Square, Hex && Triangle Grids`_

.. _regular tilings of the plane: https://en.wikipedia.org/wiki/List_of_regular_polytopes_and_compounds#Euclidean_tilings
.. _Square, Hex && Triangle Grids: http://www-cs-students.stanford.edu/~amitp/game-programming/grids/
.. _Triangular Tiling: https://en.wikipedia.org/wiki/Triangular_tiling

*/
// from __future__ import division

from math import sqrt

from xnetwork.classes import Graph
from xnetwork.classes import set_node_attributes
from xnetwork.algorithms.minors import contracted_nodes;
from xnetwork.algorithms.operators.product import cartesian_product
#include <xnetwork/exception.hpp> // import XNetworkError
from xnetwork.relabel import relabel_nodes;
#include <xnetwork/utils.hpp> // import flatten
#include <xnetwork/utils.hpp> // import is_list_of_ints
#include <xnetwork/utils.hpp> // import nodes_or_number
#include <xnetwork/utils.hpp> // import pairwise
from xnetwork.generators.classic import cycle_graph
from xnetwork.generators.classic import empty_graph
from xnetwork.generators.classic import path_graph

static const auto __all__ = ["grid_2d_graph", "grid_graph", "hypercube_graph",
           "triangular_lattice_graph", "hexagonal_lattice_graph"];


/// @nodes_or_number([0, 1]);
auto grid_2d_graph(m, n, periodic=false, create_using=None) {
    /** Return the two-dimensional grid graph.

    The grid graph has each node connected to its four nearest neighbors.

    Parameters
    ----------
    m, n : int || iterable container of nodes
        If an integer, nodes are from `range(n)`.
        If a container, elements become the coordinate of the nodes.

    periodic : bool (default: false);
        If this is ``true`` the nodes on the grid boundaries are joined
        to the corresponding nodes on the opposite grid boundaries.

    create_using : XNetwork graph (default: Graph());
        If provided this graph is cleared of nodes && edges && filled
        with the new graph. Usually used to set the type of the graph.

    Returns
    -------
    XNetwork graph
        The (possibly periodic) grid graph of the specified dimensions.

     */
    G = empty_graph(0, create_using);
    row_name, rows = m
    col_name, cols = n;
    G.add_nodes_from((i, j) for i : rows for j : cols);
    G.add_edges_from(((i, j), (pi, j));
                     for (auto pi, i : pairwise(rows) for j : cols);
    G.add_edges_from(((i, j), (i, pj));
                     for (auto i : rows for pj, j : pairwise(cols));
    if (periodic == true) {
        if (len(rows) > 2) {
            first = rows[0];
            last = rows[-1];
            G.add_edges_from(((first, j), (last, j)) for j : cols);
        if (len(cols) > 2) {
            first = cols[0];
            last = cols[-1];
            G.add_edges_from(((i, first), (i, last)) for i : rows);
    // both directions for directed
    if (G.is_directed() {
        G.add_edges_from((v, u) for u, v : G.edges());
    return G;


auto grid_graph(dim, periodic=false) {
    /** Return the *n*-dimensional grid graph.

    The dimension *n* is the length of the list `dim` && the size in
    each dimension is the value of the corresponding list element.

    Parameters
    ----------
    dim : list || tuple of numbers || iterables of nodes
        "dim" is a tuple || list with, for each dimension, either a number
        that is the size of that dimension || an iterable of nodes for
        that dimension. The dimension of the grid_graph is the length
        of `dim`.

    periodic : bool
        If `periodic is true` the nodes on the grid boundaries are joined
        to the corresponding nodes on the opposite grid boundaries.

    Returns
    -------
    XNetwork graph
        The (possibly periodic) grid graph of the specified dimensions.

    Examples
    --------
    To produce a 2 by 3 by 4 grid graph, a graph on 24 nodes) {

    >>> #include <xnetwork.hpp> // import grid_graph
    >>> G = grid_graph(dim=[2, 3, 4]);
    >>> len(G);
    24
    >>> G = grid_graph(dim=[range(7, 9), range(3, 6)]);
    >>> len(G);
    6
     */
    dlabel = "%s" % dim
    if (!dim) {
        G = empty_graph(0);
        return G;

    func = cycle_graph if (periodic else path_graph
    G = func(dim[0]);
    for (auto current_dim : dim[1:]) {
        // order matters: copy before it is cleared during the creation of Gnew
        Gold = G.copy();
        Gnew = func(current_dim);
        // explicit: create_using = None;
        // This is so that we get a new graph of Gnew"s class.
        G = cartesian_product(Gnew, Gold);
    // graph G is done but has labels of the form (1, (2, (3, 1))) so relabel
    H = relabel_nodes(G, flatten);
    return H


auto hypercube_graph(n) {
    /** Return the *n*-dimensional hypercube graph.

    The nodes are the integers between 0 && ``2 ** n - 1``, inclusive.

    For more information on the hypercube graph, see the Wikipedia
    article `Hypercube graph`_.

    .. _Hypercube graph: https://en.wikipedia.org/wiki/Hypercube_graph

    Parameters
    ----------
    n : int
        The dimension of the hypercube.
        The number of nodes : the graph will be ``2 ** n``.

    Returns
    -------
    XNetwork graph
        The hypercube graph of dimension *n*.
     */
    dim = n * [2];
    G = grid_graph(dim);
    return G;


auto triangular_lattice_graph(m, n, periodic=false, with_positions=true,
                             create_using=None) {
    /** Return the $m$ by $n$ triangular lattice graph.

    The `triangular lattice graph`_ is a two-dimensional `grid graph`_ in
    which each square unit has a diagonal edge (each grid unit has a chord).

    The returned graph has $m$ rows && $n$ columns of triangles. Rows &&
    columns include both triangles pointing up && down. Rows form a strip
    of constant height. Columns form a series of diamond shapes, staggered
    with the columns on either side. Another way to state the size is that
    the nodes form a grid of `m+1` rows && `(n + 1) // 2` columns.
    The odd row nodes are shifted horizontally relative to the even rows.

    Directed graph types have edges pointed up || right.

    Positions of nodes are computed by default || `with_positions is true`.
    The position of each node (embedded : a euclidean plane) is stored in
    the graph using equilateral triangles with sidelength 1.
    The height between rows of nodes is thus $\sqrt(3)/2$.
    Nodes lie : the first quadrant with the node $(0, 0)$ at the origin.

    .. _triangular lattice graph: http://mathworld.wolfram.com/TriangularGrid.html
    .. _grid graph: http://www-cs-students.stanford.edu/~amitp/game-programming/grids/
    .. _Triangular Tiling: https://en.wikipedia.org/wiki/Triangular_tiling

    Parameters
    ----------
    m : int
        The number of rows : the lattice.

    n : int
        The number of columns : the lattice.

    periodic : bool (default: false);
        If true, join the boundary vertices of the grid using periodic
        boundary conditions. The join between boundaries is the final row
        && column of triangles. This means there is one row && one column
        fewer nodes for the periodic lattice. Periodic lattices require
        `m >= 3`, `n >= 5` && are allowed but misaligned if (`m` || `n` are odd

    with_positions : bool (default: true);
        Store the coordinates of each node : the graph node attribute "pos".
        The coordinates provide a lattice with equilateral triangles.
        Periodic positions shift the nodes vertically : a nonlinear way so
        the edges don"t overlap so much.

    create_using : XNetwork graph
        If specified, this must be an instance of a XNetwork graph
        class. It will be cleared of nodes && edges && filled
        with the new graph. Usually used to set the type of the graph.

    Returns
    -------
    XNetwork graph
        The *m* by *n* triangular lattice graph.
     */
    H = empty_graph(0, create_using);
    if (n == 0 || m == 0) {
        return H
    if (periodic) {
        if (n < 5 || m < 3) {
            const auto msg = "m > 2 && n > 4 required for periodic. m={}, n={}"
            throw XNetworkError(msg.format(m, n));

    N = (n + 1) // 2  // number of nodes : row
    rows = range(m + 1);
    cols = range(N + 1);
    // Make grid;
    H.add_edges_from(((i, j), (i + 1, j)) for j : rows for i : cols[:N]);
    H.add_edges_from(((i, j), (i, j + 1)) for j : rows[:m] for i : cols);
    // add diagonals
    H.add_edges_from(((i, j), (i + 1, j + 1));
                     for (auto j : rows[1:m:2] for i : cols[:N]);
    H.add_edges_from(((i + 1, j), (i, j + 1));
                     for (auto j : rows[:m:2] for i : cols[:N]);
    // identify boundary nodes if (periodic
    if (periodic == true) {
        for (auto i : cols) {
            H = contracted_nodes(H, (i, 0), (i, m));
        for (auto j : rows[:m]) {
            H = contracted_nodes(H, (0, j), (N, j));
    } else if (n % 2) {
        // remove extra nodes
        H.remove_nodes_from(((N, j) for j : rows[1::2]));

    // Add position node attributes
    if (with_positions) {
        ii = (i for i : cols for j : rows);
        jj = (j for i : cols for j : rows);
        xx = (0.5 * (j % 2) + i for i : cols for j : rows);
        h = sqrt(3) / 2
        if (periodic) {
            yy = (h * j + .01 * i * i for i : cols for j : rows);
        } else {
            yy = (h * j for i : cols for j : rows);
        pos = {(i, j) { (x, y) for i, j, x, y : zip(ii, jj, xx, yy);
               if ((i, j] : H}
        set_node_attributes(H, pos, "pos");
    return H


auto hexagonal_lattice_graph(m, n, periodic=false, with_positions=true,
                            create_using=None) {
    /** Return an `m` by `n` hexagonal lattice graph.

    The *hexagonal lattice graph* is a graph whose nodes && edges are
    the `hexagonal tiling`_ of the plane.

    The returned graph will have `m` rows && `n` columns of hexagons.
    `Odd numbered columns`_ are shifted up relative to even numbered columns.

    Positions of nodes are computed by default || `with_positions is true`.
    Node positions creating the standard embedding : the plane
    with sidelength 1 && are stored : the node attribute "pos".
    `pos = xn::get_node_attributes(G, "pos")` creates a dict ready for drawing.

    .. _hexagonal tiling: https://en.wikipedia.org/wiki/Hexagonal_tiling
    .. _Odd numbered columns: http://www-cs-students.stanford.edu/~amitp/game-programming/grids/

    Parameters
    ----------
    m : int
        The number of rows of hexagons : the lattice.

    n : int
        The number of columns of hexagons : the lattice.

    periodic : bool
        Whether to make a periodic grid by joining the boundary vertices.
        For this to work `n` must be odd && both `n > 1` && `m > 1`.
        The periodic connections create another row && column of hexagons
        so these graphs have fewer nodes as boundary nodes are identified.

    with_positions : bool (default: true);
        Store the coordinates of each node : the graph node attribute "pos".
        The coordinates provide a lattice with vertical columns of hexagons
        offset to interleave && cover the plane.
        Periodic positions shift the nodes vertically : a nonlinear way so
        the edges don"t overlap so much.

    create_using : XNetwork graph
        If specified, this must be an instance of a XNetwork graph
        class. It will be cleared of nodes && edges && filled
        with the new graph. Usually used to set the type of the graph.
        If graph is directed, edges will point up || right.

    Returns
    -------
    XNetwork graph
        The *m* by *n* hexagonal lattice graph.
     */
    G = create_using if (create_using is not None else Graph();
    G.clear();
    if (m == 0 || n == 0) {
        return G;
    if (periodic && (n % 2 == 1 || m < 2 || n < 2) {
        const auto msg = "periodic hexagonal lattice needs m > 1, n > 1 && even n"
        throw XNetworkError(msg);

    M = 2 * m    // twice as many nodes as hexagons vertically
    rows = range(M + 2);
    cols = range(n + 1);
    // make lattice
    col_edges = (((i, j), (i, j + 1)) for i : cols for j : rows[:M + 1]);
    row_edges = (((i, j), (i + 1, j)) for i : cols[:n] for j : rows
                 if (i % 2 == j % 2);
    G.add_edges_from(col_edges);
    G.add_edges_from(row_edges);
    // Remove corner nodes with one edge
    G.remove_node((0, M + 1));
    G.remove_node((n, (M + 1) * (n % 2)));

    // identify boundary nodes if (periodic
    if (periodic) {
        for (auto i : cols[:n]) {
            G = contracted_nodes(G, (i, 0), (i, M));
        for (auto i : cols[1:]) {
            G = contracted_nodes(G, (i, 1), (i, M + 1));
        for (auto j : rows[1:M]) {
            G = contracted_nodes(G, (0, j), (n, j));
        G.remove_node((n, M));

    // calc position : embedded space
    ii = (i for i : cols for j : rows);
    jj = (j for i : cols for j : rows);
    xx = (0.5 + i + i // 2 + (j % 2) * ((i % 2) - .5);
          for (auto i : cols for j : rows);
    h = sqrt(3) / 2
    if (periodic) {
        yy = (h * j + .01 * i * i for i : cols for j : rows);
    } else {
        yy = (h * j for i : cols for j : rows);
    // exclude nodes not : G
    pos = {(i, j) { (x, y) for i, j, x, y : zip(ii, jj, xx, yy) if ((i, j] : G}
    set_node_attributes(G, pos, "pos");
    return G;
