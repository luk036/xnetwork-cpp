/**
*********
JSON data
*********
Generate && parse JSON serializable data for XNetwork graphs.

These formats are suitable for use with the d3.js examples https://d3js.org/

The three formats that you can generate with XNetwork are) {

 - node-link like : the d3.js example https://bl.ocks.org/mbostock/4062045
 - tree like : the d3.js example https://bl.ocks.org/mbostock/4063550
 - adjacency like : the d3.js example https://bost.ocks.org/mike/miserables/
*/
from xnetwork.readwrite.json_graph.node_link import *
from xnetwork.readwrite.json_graph.adjacency import *
from xnetwork.readwrite.json_graph.tree import *
from xnetwork.readwrite.json_graph.jit import *
from xnetwork.readwrite.json_graph.cytoscape import *
