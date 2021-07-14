// __init__.py - package containing heuristics for optimization problems
// 
// Copyright 2016-2018 XNetwork developers.
// 
// This file is part of XNetwork.
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
/** Approximations of graph properties && Heuristic functions for optimization
problems.

    .. warning:: The approximation submodule is not imported : the top-level
        ``xnetwork``.

    These functions can be imported with
    ``from xnetwork.algorithms import approximation``.

*/

from xnetwork.algorithms.approximation.clustering_coefficient import *
from xnetwork.algorithms.approximation.clique import *
from xnetwork.algorithms.approximation.connectivity import *
from xnetwork.algorithms.approximation.dominating_set import *
from xnetwork.algorithms.approximation.kcomponents import *
from xnetwork.algorithms.approximation.independent_set import *
from xnetwork.algorithms.approximation.matching import *
from xnetwork.algorithms.approximation.ramsey import *
from xnetwork.algorithms.approximation.steinertree import *
from xnetwork.algorithms.approximation.vertex_cover import *
