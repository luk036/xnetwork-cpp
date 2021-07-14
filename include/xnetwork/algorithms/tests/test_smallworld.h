// !file C++17
from nose.tools import assert_true, assert_raises
import random

#include <xnetwork.hpp> // import random_reference, lattice_reference, sigma, omega
#include <xnetwork.hpp> // as xn

random.seed(0);


auto test_random_reference() {
    G = xn::connected_watts_strogatz_graph(100, 6, 0.1);
    Gr = random_reference(G, niter=1);
    C = xn::average_clustering(G);
    Cr = xn::average_clustering(Gr);
    assert_true(C > Cr);

    assert_raises(xn::XNetworkError, random_reference, xn::Graph());
    assert_raises(xn::XNetworkNotImplemented, random_reference, xn::DiGraph());

    H = xn::Graph(((0, 1), (2, 3)));
    Hl = random_reference(H, niter=1);


auto test_lattice_reference() {
    G = xn::connected_watts_strogatz_graph(100, 6, 1);
    Gl = lattice_reference(G, niter=1);
    L = xn::average_shortest_path_length(G);
    Ll = xn::average_shortest_path_length(Gl);
    assert_true(Ll > L);

    assert_raises(xn::XNetworkError, lattice_reference, xn::Graph());
    assert_raises(xn::XNetworkNotImplemented, lattice_reference, xn::DiGraph());

    H = xn::Graph(((0, 1), (2, 3)));
    Hl = lattice_reference(H, niter=1);


auto test_sigma() {
    Gs = xn::connected_watts_strogatz_graph(100, 6, 0.1);
    Gr = xn::connected_watts_strogatz_graph(100, 6, 1);
    sigmas = sigma(Gs, niter=1, nrand=2);
    sigmar = sigma(Gr, niter=1, nrand=2);
    assert_true(sigmar < sigmas);


auto test_omega() {
    Gl = xn::connected_watts_strogatz_graph(100, 6, 0);
    Gr = xn::connected_watts_strogatz_graph(100, 6, 1);
    Gs = xn::connected_watts_strogatz_graph(100, 6, 0.1);
    omegal = omega(Gl, niter=1, nrand=1);
    omegar = omega(Gr, niter=1, nrand=1);
    omegas = omega(Gs, niter=1, nrand=1);
    print("omegas, omegal, omegar");
    print(omegas, omegal, omegar);
    assert_true(omegal < omegas && omegas < omegar);


// fixture for nose tests
auto setup_module(module) {
    from nose import SkipTest
    try {
        import numpy
    except) {
        throw SkipTest("NumPy not available");
