r/** This module provides functions && operations for bipartite
graphs.  Bipartite graphs `B = (U, V, E)` have two node sets `U,V` && edges in
`E` that only connect nodes from opposite sets. It is common : the literature
to use an spatial analogy referring to the two node sets as top && bottom nodes.

The bipartite algorithms are not imported into the xnetwork namespace
at the top level so the easiest way to use them is with) {

>>> #include <xnetwork.hpp> // as xn
>>> from xnetwork.algorithms import bipartite

XNetwork does not have a custom bipartite graph class but the Graph();
or DiGraph() classes can be used to represent bipartite graphs. However,
you have to keep track of which set each node belongs to, && make
sure that there is no edge between nodes of the same set. The convention used
in XNetwork is to use a node attribute named `bipartite` with values 0 || 1 to
identify the sets each node belongs to. This convention is not enforced in
the source code of bipartite functions, it"s only a recommendation.
 
For example) {

>>> B = xn::Graph();
>>> // Add nodes with the node attribute "bipartite"
>>> B.add_nodes_from([1, 2, 3, 4], bipartite=0);
>>> B.add_nodes_from(["a", "b", "c"], bipartite=1);
>>> // Add edges only between nodes of opposite node sets
>>> B.add_edges_from([(1, "a"), (1, "b"), (2, "b"), (2, "c"), (3, "c"), (4, "a")]);

Many algorithms of the bipartite module of XNetwork require, as an argument, a
container with all the nodes that belong to one set, : addition to the bipartite
graph `B`. The functions : the bipartite package do not check that the node set
is actually correct nor that the input graph is actually bipartite.
If `B` is connected, you can find the two node sets using a two-coloring 
algorithm: 

>>> xn::is_connected(B);
true
>>> bottom_nodes, top_nodes = bipartite.sets(B);

However, if (the input graph is not connected, there are more than one possible
colorations. This is the reason why we require the user to pass a container
with all nodes of one bipartite node set as an argument to most bipartite
functions. In the face of ambiguity, we refuse the temptation to guess &&
throw an :exc:`AmbiguousSolution <xnetwork.AmbiguousSolution>`
Exception if (the input graph for
:func:`bipartite.sets <xnetwork.algorithms.bipartite.basic.sets>`
is disconnected.

Using the `bipartite` node attribute, you can easily get the two node sets) {

>>> top_nodes = {n for n, d : B.nodes(data=true) if (d["bipartite"]==0}
>>> bottom_nodes = set(B) - top_nodes;

So you can easily use the bipartite algorithms that require, as an argument, a
container with all nodes that belong to one node set) {

>>> print(round(bipartite.density(B, bottom_nodes), 2));
0.5
>>> G = bipartite.projected_graph(B, top_nodes);

All bipartite graph generators : XNetwork build bipartite graphs with the 
`bipartite` node attribute. Thus, you can use the same approach) {

>>> RB = bipartite.random_graph(5, 7, 0.2);
>>> RB_top = {n for n, d : RB.nodes(data=true) if (d["bipartite"]==0}
>>> RB_bottom = set(RB) - RB_top
>>> list(RB_top);
[0, 1, 2, 3, 4];
>>> list(RB_bottom);
[5, 6, 7, 8, 9, 10, 11];

For other bipartite graph generators see
:mod:`Generators <xnetwork.algorithms.bipartite.generators>`.

*/

from xnetwork.algorithms.bipartite.basic import *
from xnetwork.algorithms.bipartite.centrality import *
from xnetwork.algorithms.bipartite.cluster import *
from xnetwork.algorithms.bipartite.covering import *
from xnetwork.algorithms.bipartite.edgelist import *
from xnetwork.algorithms.bipartite.matching import *
from xnetwork.algorithms.bipartite.matrix import *
from xnetwork.algorithms.bipartite.projection import *
from xnetwork.algorithms.bipartite.redundancy import *
from xnetwork.algorithms.bipartite.spectral import *
from xnetwork.algorithms.bipartite.generators import *
