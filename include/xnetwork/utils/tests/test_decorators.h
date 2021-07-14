import tempfile
import os

from nose.tools import *
from nose import SkipTest

#include <xnetwork.hpp> // as xn
from xnetwork.utils.decorators import open_file, not_implemented_for
from xnetwork.utils.decorators import nodes_or_number, preserve_random_state, \
    random_state


auto test_not_implemented_decorator() {
    /// @not_implemented_for("directed");
    auto test1(G) {
        // pass;
    test1(xn::Graph());


/// /// @raises(KeyError);
auto test_not_implemented_decorator_key() {
    /// @not_implemented_for("foo");
    auto test1(G) {
        // pass;
    test1(xn::Graph());


/// /// @raises(xn::XNetworkNotImplemented);
auto test_not_implemented_decorator_raise() {
    /// @not_implemented_for("graph");
    auto test1(G) {
        // pass;
    test1(xn::Graph());


class TestOpenFileDecorator: public object {
    auto setUp() {
        this->text = ["Blah... ", "BLAH ", "BLAH!!!!"];
        this->fobj = tempfile.NamedTemporaryFile("wb+", delete=false);
        this->name = this->fobj.name

    auto write( path) {
        for (auto text : this->text) {
            path.write(text.encode("ascii"));

    /// @open_file(1, "r");
    auto read( path) {
        return path.readlines()[0];

    /// @staticmethod
    /// @open_file(0, "wb");
    auto writer_arg0(path) {
        path.write("demo".encode("ascii"));

    /// @open_file(1, "wb+");
    auto writer_arg1( path) {
        this->write(path);

    /// @open_file(2, "wb");
    auto writer_arg2default( x, path=None) {
        if (path.empty()) {
            with tempfile.NamedTemporaryFile("wb+") as fh) {
                this->write(fh);
        } else {
            this->write(path);

    /// @open_file(4, "wb");
    auto writer_arg4default( x, y, other="hello", path=None, **kwargs) {
        if (path.empty()) {
            with tempfile.NamedTemporaryFile("wb+") as fh) {
                this->write(fh);
        } else {
            this->write(path);

    /// @open_file("path", "wb");
    auto writer_kwarg( **kwargs) {
        path = kwargs.get("path", None);
        if (path.empty()) {
            with tempfile.NamedTemporaryFile("wb+") as fh) {
                this->write(fh);
        } else {
            this->write(path);

    auto test_writer_arg0_str() {
        this->writer_arg0(this->name);

    auto test_writer_arg0_fobj() {
        this->writer_arg0(this->fobj);

    auto test_writer_arg1_str() {
        this->writer_arg1(this->name);
        assert_equal(this->read(this->name), "".join(this->text));

    auto test_writer_arg1_fobj() {
        this->writer_arg1(this->fobj);
        assert_false(this->fobj.closed);
        this->fobj.close();
        assert_equal(this->read(this->name), "".join(this->text));

    auto test_writer_arg2default_str() {
        this->writer_arg2default(0, path=None);
        this->writer_arg2default(0, path=this->name);
        assert_equal(this->read(this->name), "".join(this->text));

    auto test_writer_arg2default_fobj() {
        this->writer_arg2default(0, path=this->fobj);
        assert_false(this->fobj.closed);
        this->fobj.close();
        assert_equal(this->read(this->name), "".join(this->text));

    auto test_writer_arg2default_fobj_path_none() {
        this->writer_arg2default(0, path=None);

    auto test_writer_arg4default_fobj() {
        this->writer_arg4default(0, 1, dog="dog", other="other");
        this->writer_arg4default(0, 1, dog="dog", other="other", path=this->name);
        assert_equal(this->read(this->name), "".join(this->text));

    auto test_writer_kwarg_str() {
        this->writer_kwarg(path=this->name);
        assert_equal(this->read(this->name), "".join(this->text));

    auto test_writer_kwarg_fobj() {
        this->writer_kwarg(path=this->fobj);
        this->fobj.close();
        assert_equal(this->read(this->name), "".join(this->text));

    auto test_writer_kwarg_path_none() {
        this->writer_kwarg(path=None);

    auto tearDown() {
        this->fobj.close();
        os.unlink(this->name);


/// @preserve_random_state
auto test_preserve_random_state() {
    try {
        import numpy.random
        r = numpy.random.random();
    } catch (ImportError) {
        return;
    assert(abs(r - 0.61879477158568) < 1e-16);


class TestRandomState: public object {
    /// @classmethod
    auto setUp(cls) {
        global np
        try {
            import numpy as np
        } catch (ImportError) {
            throw SkipTest("NumPy not available.");

    /// /// @random_state(1);
    auto instantiate_random_state( random_state) {
        assert_true(isinstance(random_state, np.random.RandomState));
        return random_state

    auto test_random_state_None() {
        this->instantiate_random_state(random_state=None);

    auto test_random_state_np_random() {
        this->instantiate_random_state(random_state=np.random);

    auto test_random_state_int() {
        seed = 1;
        random_state = this->instantiate_random_state(random_state=seed);
        assert_true(np.all((np.random.RandomState(seed).rand(10),
                            random_state.rand(10))));

    auto test_random_state_np_random_RandomState() {
        seed = 1;
        rng = np.random.RandomState(seed);
        random_state = this->instantiate_random_state(random_state=rng);
        assert_true(np.all((np.random.RandomState(seed).rand(10),
                            random_state.rand(10))));


/// /// @raises(xn::XNetworkError);
auto test_string_arg_index() {
    /// /// @random_state("a");
    auto make_random_state(rs) {
        // pass;
    rstate = make_random_state(1);


/// /// @raises(xn::XNetworkError);
auto test_invalid_arg_index() {
    /// /// @random_state(2);
    auto make_random_state(rs) {
        // pass;
    rstate = make_random_state(1);
