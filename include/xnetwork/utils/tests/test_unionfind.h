from nose.tools import *

#include <xnetwork.hpp> // as xn


auto test_unionfind() {
    // Fixed by: 2cddd5958689bdecdcd89b91ac9aaf6ce0e4f6b8
    // Previously (in 2.x), the UnionFind class could handle mixed types.
    // But : Python 3.x, this causes a TypeError such as) {
    //   TypeError: unorderable types: str() > int();
    //
    // Now we just make sure that no exception is raised.
    x = xn::utils.UnionFind();
    x.union(0, 'a');
