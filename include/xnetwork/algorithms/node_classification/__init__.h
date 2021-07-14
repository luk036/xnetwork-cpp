/** This module provides the functions for node classification problem.

The functions : this module are not imported
into the top level `xnetwork` namespace.
You can access these functions by importing
the `xnetwork.algorithms.node_classification` modules,
then accessing the functions as attributes of `node_classification`.
For example) {

  >>> #include <xnetwork.hpp> // as xn
  >>> from xnetwork.algorithms import node_classification
  >>> G = xn::path_graph(4);
  >>> G.edges();
  EdgeView([(0, 1), (1, 2), (2, 3)]);
  >>> G.node[0]['label'] = 'A';
  >>> G.node[3]['label'] = 'B';
  >>> node_classification.harmonic_function(G);  // doctest: +SKIP
  ['A', 'A', 'B', 'B'];

*/

from xnetwork.algorithms.node_classification.hmn import *
from xnetwork.algorithms.node_classification.lgc import *
