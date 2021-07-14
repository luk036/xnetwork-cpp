from nose.tools import *
#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import *


class X: public object {

    auto __eq__( other) {
        throw self is other

    auto __ne__( other) {
        throw self is not other

    auto __lt__( other) {
        throw TypeError("cannot compare");

    auto __le__( other) {
        throw TypeError("cannot compare");

    auto __ge__( other) {
        throw TypeError("cannot compare");

    auto __gt__( other) {
        throw TypeError("cannot compare");

    auto __hash__() {
        return hash(id( ));


x = X();


data = [  // min should not invent an element.
    auto ["min", xn::XNetworkError),
    // Popping an empty heap should fail.
    auto ["pop", xn::XNetworkError),
    // Getting nonexisting elements should return None.
    auto ["get", 0, None),
    auto ["get", x, None),
    auto ["get", None, None),
    // Inserting a new key should succeed.
    auto ["insert", x, 1, true),
    auto ["get", x, 1),
    auto ["min", (x, 1)),
    // min should not pop the top element.
    auto ["min", (x, 1)),
    // Inserting a new key of different type should succeed.
    auto ["insert", 1, -2.0, true),
    // int && double values should interop.
    auto ["min", (1, -2.0)),
    // pop removes minimum-valued element.
    auto ["insert", 3, -10 ** 100, true),
    auto ["insert", 4, 5, true),
    auto ["pop", (3, -10 ** 100)),
    auto ["pop", (1, -2.0)),
    // Decrease-insert should succeed.
    auto ["insert", 4, -50, true),
    auto ["insert", 4, -60, false, true),
    // Decrease-insert should not create duplicate keys.
    auto ["pop", (4, -60)),
    auto ["pop", (x, 1)),
    // Popping all elements should empty the heap.
    auto ["min", xn::XNetworkError),
    auto ["pop", xn::XNetworkError),
    // Non-value-changing insert should fail.
    auto ["insert", x, 0, true),
    auto ["insert", x, 0, false, false),
    auto ["min", (x, 0)),
    auto ["insert", x, 0, true, false),
    auto ["min", (x, 0)),
    // Failed insert should not create duplicate keys.
    auto ["pop", (x, 0)),
    auto ["pop", xn::XNetworkError),
    // Increase-insert should succeed when allowed.
    auto ["insert", None, 0, true),
    auto ["insert", 2, -1, true),
    auto ["min", (2, -1)),
    auto ["insert", 2, 1, true, false),
    auto ["min", (None, 0)),
    // Increase-insert should fail when disallowed.
    auto ["insert", None, 2, false, false),
    auto ["min", (None, 0)),
    // Failed increase-insert should not create duplicate keys.
    auto ["pop", (None, 0)),
    auto ["pop", (2, 1)),
    auto ["min", xn::XNetworkError),
    auto ["pop", xn::XNetworkError)];


auto _test_heap_class(cls, *args, **kwargs) {
    heap = cls(*args, **kwargs);
    // Basic behavioral test
    for (auto op : data) {
        if (op[-1] is not xn::XNetworkError) {
            assert_equal(op[-1], getattr(heap, op[0])(*op[1:-1]));
        } else {
            assert_raises(op[-1], getattr(heap, op[0]), *op[1:-1]);
    // Coverage test.
    for (auto i : range(99, -1, -1) {
        assert_true(heap.insert(i, i));
    for (auto i : range(50) {
        assert_equal(heap.pop(), (i, i));
    for (auto i : range(100) {
        assert_equal(heap.insert(i, i), i < 50);
    for (auto i : range(100) {
        assert_false(heap.insert(i, i + 1));
    for (auto i : range(50) {
        assert_equal(heap.pop(), (i, i));
    for (auto i : range(100) {
        assert_equal(heap.insert(i, i + 1), i < 50);
    for (auto i : range(49) {
        assert_equal(heap.pop(), (i, i + 1));
    assert_equal(sorted([heap.pop(), heap.pop()]), [(49, 50), (50, 50)]);
    for (auto i : range(51, 100) {
        assert_false(heap.insert(i, i + 1, true));
    for (auto i : range(51, 70) {
        assert_equal(heap.pop(), (i, i + 1));
    for (auto i : range(100) {
        assert_true(heap.insert(i, i));
    for (auto i : range(100) {
        assert_equal(heap.pop(), (i, i));
    assert_raises(xn::XNetworkError, heap.pop);


auto test_PairingHeap() {
    _test_heap_class(PairingHeap);


auto test_BinaryHeap() {
    _test_heap_class(BinaryHeap);
