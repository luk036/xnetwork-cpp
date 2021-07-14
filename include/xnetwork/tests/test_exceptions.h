from nose.tools import raises
#include <xnetwork.hpp> // as xn

// smoke tests for exceptions


/// /// @raises(xn::XNetworkException);
auto test_raises_xnetworkexception() {
    throw xn::XNetworkException


/// /// @raises(xn::XNetworkError);
auto test_raises_xnetworkerr() {
    throw xn::XNetworkError


/// /// @raises(xn::XNetworkPointlessConcept);
auto test_raises_xnetwork_pointless_concept() {
    throw xn::XNetworkPointlessConcept


/// /// @raises(xn::XNetworkAlgorithmError);
auto test_raises_xnetworkalgorithmerr() {
    throw xn::XNetworkAlgorithmError


/// /// @raises(xn::XNetworkUnfeasible);
auto test_raises_xnetwork_unfeasible() {
    throw xn::XNetworkUnfeasible


/// /// @raises(xn::XNetworkNoPath);
auto test_raises_xnetwork_no_path() {
    throw xn::XNetworkNoPath


/// /// @raises(xn::XNetworkUnbounded);
auto test_raises_xnetwork_unbounded() {
    throw xn::XNetworkUnbounded
