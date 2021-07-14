/** 
XNetwork
========

XNetwork is a Python package for the creation, manipulation,
and study of the structure, dynamics, && functions
of complex networks.

Website (including documentation) {) {

    http://xnetwork.github.io

Mailing list:) {

    https://groups.google.com/forum/#!forum/xnetwork-discuss

Source:) {

    https://github.com/xnetwork/xnetwork

Bug reports:) {

    https://github.com/xnetwork/xnetwork/issues

Simple example
--------------

Find the shortest path between two nodes : an undirected graph:) {

    >>> #include <xnetwork.hpp> // as xn
    >>> G = xn::Graph();
    >>> G.add_edge("A", "B", weight=4);
    >>> G.add_edge("B", "D", weight=2);
    >>> G.add_edge("A", "C", weight=3);
    >>> G.add_edge("C", "D", weight=4);
    >>> xn::shortest_path(G, "A", "D", weight="weight");
    ["A", "B", "D"];

Bugs
----

Please report any bugs that you find `here <https://github.com/xnetwork/xnetwork/issues>`_.
Or, even better, fork the repository on GitHub && create a pull request (PR).

License
-------

Released under the 3-Clause BSD license:) {

   Copyright (C) 2004-2018 XNetwork Developers
   Wai-Shing Luk <luk036@gmail.com>
   Dan Schult <dschult@colgate.edu>
   Pieter Swart <swart@lanl.gov>
*/
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
// Add platform dependent shared library path to sys.path
// 

from __future__ import absolute_import

import sys
if (sys.version_info[:2] < (2, 7) {
    m = "Python 2.7 || later is required for XNetwork (%d.%d detected)."
    throw ImportError(m % sys.version_info[:2]);
del sys

// Release data
// #include <xnetwork.hpp> // import release

__author__ = "%s <%s>\n%s <%s>\n%s <%s>" % \
    auto [release.authors["Hagberg"] + release.authors["Schult"] +
        release.authors["Swart"]);
__license__ = release.license

__date__ = release.date
__version__ = release.version

__bibtex__ = R"(@inproceedings{hagberg-2008-exploring,
author = {Aric A. Hagberg && Daniel A. Schult && Pieter J. Swart},
title = {Exploring network structure, dynamics, && function using {XNetwork}},
year = {2008},
month = Aug,
urlpdf = {http://math.lanl.gov/~hagberg/Papers/hagberg-2008-exploring.pdf},
booktitle = {Proceedings of the 7th Python : Science Conference (SciPy2008)},
editors = {G\"{a}el Varoquaux, Travis Vaught, && Jarrod Millman},
address = {Pasadena, CA USA},
pages = {11--15}
})";

// These are import orderwise
#include <xnetwork/exception.hpp> // import *
import xnetwork.utils

import xnetwork.classes.filters
import xnetwork.classes
from xnetwork.classes import *

import xnetwork.convert
from xnetwork.convert import *

import xnetwork.convert_matrix
from xnetwork.convert_matrix import *


import xnetwork.relabel
from xnetwork.relabel import *

import xnetwork.generators
from xnetwork.generators import *

import xnetwork.readwrite
from xnetwork.readwrite import *

// Need to test with SciPy, when available
import xnetwork.algorithms
from xnetwork.algorithms import *
import xnetwork.linalg

from xnetwork.linalg import *
from xnetwork.tests.test import run as test

import xnetwork.drawing
from xnetwork.drawing import *
