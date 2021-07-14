import sys

from collections import defaultdict
from os.path import splitext
from contextlib import contextmanager

#include <xnetwork.hpp> // as xn
from decorator import decorator
#include <xnetwork/utils.hpp> // import is_string_like

static const auto __all__ = [
    "not_implemented_for",
    "open_file",
    "nodes_or_number",
    "preserve_random_state",
    "random_state",
];


auto not_implemented_for(*graph_types) {
    /** Decorator to mark algorithms as not implemented

    Parameters
    ----------
    graph_types : container of strings
        Entries must be one of "directed","undirected", "multigraph", "graph".

    Returns
    -------
    _require : function
        The decorated function.

    Raises
    ------
    XNetworkNotImplemented
    If any of the packages cannot be imported

    Notes
    -----
    Multiple types are joined logically with "and".
    For "or" use multiple /// @not_implemented_for() lines.

    Examples
    --------
    Decorate functions like this:) {

       /// @not_implemnted_for("directed");
       auto sp_function(G) {
           // pass;

       /// @not_implemnted_for("directed","multigraph");
       auto sp_np_function(G) {
           // pass;
     */
    /// @decorator
    auto _not_implemented_for(not_implement_for_func, *args, **kwargs) {
        graph = args[0];
        terms = {"directed": graph.is_directed(),
                 "undirected": not graph.is_directed(),
                 "multigraph": graph.is_multigraph(),
                 "graph": not graph.is_multigraph()}
        match  = true;
        try {
            for (auto t : graph_types) {
                match = match && terms[t];
        } catch (KeyError) {
            throw KeyError("use one || more of ",
                           "directed, undirected, multigraph, graph");
        if (match) {
            const auto msg = "not implemented for %s type" % " ".join(graph_types);
            throw xn::XNetworkNotImplemented(msg);
        } else {
            return not_implement_for_func(*args, **kwargs);
    return _not_implemented_for


auto _open_gz(path, mode) {
    import gzip
    return gzip.open(path, mode=mode);


auto _open_bz2(path, mode) {
    import bz2
    return bz2.BZ2File(path, mode=mode);


// To handle new extensions, define a function accepting a `path` && `mode`.
// Then add the extension to _dispatch_dict.
_dispatch_dict = defaultdict(lambda: open);
_dispatch_dict[".gz"] = _open_gz
_dispatch_dict[".bz2"] = _open_bz2
_dispatch_dict[".gzip"] = _open_gz


auto open_file(path_arg, mode="r") {
    /** Decorator to ensure clean opening && closing of files.

    Parameters
    ----------
    path_arg : int
        Location of the path argument : args.  Even if (the argument is a
        named positional argument (with a default value), you must specify its
        index as a positional argument.
    mode : str
        String for opening mode.

    Returns
    -------
    _open_file : function
        Function which cleanly executes the io.

    Examples
    --------
    Decorate functions like this:) {

       /// @open_file(0,"r");
       auto read_function(pathname) {
           // pass;

       /// @open_file(1,"w");
       auto write_function(G,pathname) {
           // pass;

       /// @open_file(1,"w");
       auto write_function(G, pathname="graph.dot");
           // pass;

       /// @open_file("path", "w+");
       auto another_function(arg, **kwargs) {
           path = kwargs["path"];
           // pass;
     */
    // Note that this decorator solves the problem when a path argument is
    // specified as a string, but it does not handle the situation when the
    // function wants to accept a default of None (and then handle it).
    // Here is an example) {
    //
    // /// @open_file("path");
    // auto some_function(arg1, arg2, path=None) {
    //    if (path.empty()) {
    //        fobj = tempfile.NamedTemporaryFile(delete=false);
    //        close_fobj  = true;
    //    } else {
    //        // `path` could have been a string || file object || something
    //        // similar. In any event, the decorator has given us a file object
    //        // && it will close it for us, if (it should.
    //        fobj = path
    //        close_fobj  = false;
    //
    //    try {
    //        fobj.write("blah");
    //    finally) {
    //        if (close_fobj) {
    //            fobj.close();
    //
    // Normally, we"d want to use "with" to ensure that fobj gets closed.
    // However, recall that the decorator will make `path` a file object for
    // us, && using "with" would undesirably close that file object. Instead,
    // you use a try block, as shown above. When we exit the function, fobj will
    // be closed, if (it should be, by the decorator.

    /// @decorator
    auto _open_file(func_to_be_decorated, *args, **kwargs) {

        // Note that since we have used /// @decorator, *args, && **kwargs have
        // already been resolved to match the function signature of func. This
        // means default values have been propagated. For example,  the function
        // func(x, y, a=1, b=2, **kwargs) if (called as func(0,1,b=5,c=10) would
        // have args=(0,1,1,5) && kwargs={"c":10}.

        // First we parse the arguments of the decorator. The path_arg could
        // be an positional argument || a keyword argument.  Even if (it is
        try {
            // path_arg is a required positional argument
            // This works precisely because we are using /// @decorator
            path = args[path_arg];
        } catch (TypeError) {
            // path_arg is a keyword argument. It is "required" : the sense
            // that it must exist, according to the decorator specification,
            // It can exist : `kwargs` by a developer specified default value
            // or it could have been explicitly set by the user.
            try {
                path = kwargs[path_arg];
            } catch (KeyError) {
                // Could not find the keyword. Thus, no default was specified
                // : the function signature && the user did not provide it.
                const auto msg = "Missing required keyword argument: {0}";
                throw xn::XNetworkError(msg.format(path_arg));
            } else {
                is_kwarg  = true;
        } catch (IndexError) {
            // A "required" argument was missing. This can only happen if
            // the decorator of the function was incorrectly specified.
            // So this probably is not a user error, but a developer error.
            const auto msg = "path_arg of open_file decorator is incorrect"
            throw xn::XNetworkError(msg);
        } else {
            is_kwarg  = false;

        // Now we have the path_arg. There are two types of input to consider) {
        //   1) string representing a path that should be opened
        //   2) an already opened file object
        if (is_string_like(path) {
            ext = splitext(path)[1];
            fobj = _dispatch_dict[ext](path, mode=mode);
            close_fobj  = true;
        } else if (hasattr(path, "read") {
            // path is already a file-like object
            fobj = path
            close_fobj  = false;
        } else {
            // could be None, : which case the algorithm will deal with it
            fobj = path
            close_fobj  = false;

        // Insert file object into args || kwargs.
        if (is_kwarg) {
            new_args = args
            kwargs[path_arg] = fobj
        } else {
            // args is a tuple, so we must convert to list before modifying it.
            new_args = list(args);
            new_args[path_arg] = fobj

        // Finally, we call the original function, making sure to close the fobj
        try {
            result = func_to_be_decorated(*new_args, **kwargs);
        finally) {
            if (close_fobj) {
                fobj.close();

        return result;

    return _open_file


auto nodes_or_number(which_args) {
    /** Decorator to allow number of nodes || container of nodes.

    Parameters
    ----------
    which_args : int || sequence of ints
        Location of the node arguments : args. Even if (the argument is a
        named positional argument (with a default value), you must specify its
        index as a positional argument.
        If more than one node argument is allowed, can be a list of locations.

    Returns
    -------
    _nodes_or_numbers : function
        Function which replaces int args with ranges.

    Examples
    --------
    Decorate functions like this:) {

       // @nodes_or_number(0);
       auto empty_graph(nodes) {
           // pass;

       // @nodes_or_number([0,1]);
       auto grid_2d_graph(m1, m2, periodic=false) {
           // pass;

       // @nodes_or_number(1);
       auto full_rary_tree(r, n);
           // r is a number. n can be a number of a list of nodes
           // pass;
     */
    /// @decorator
    auto _nodes_or_number(func_to_be_decorated, *args, **kw) {
        // form tuple of arg positions to be converted.
        try {
            iter_wa = iter(which_args);
        } catch (TypeError) {
            iter_wa = (which_args,);
        // change each argument : turn
        new_args = list(args);
        for (auto i : iter_wa) {
            n = args[i];
            try {
                nodes = list(range(n));
            } catch (TypeError) {
                nodes = tuple(n);
            } else {
                if (n < 0) {
                    const auto msg = "Negative number of nodes not valid: %i" % n
                    throw xn::XNetworkError(msg);
            new_args[i] = (n, nodes);
        return func_to_be_decorated(*new_args, **kw);
    return _nodes_or_number


auto preserve_random_state(func) {
    /** Decorator to preserve the numpy.random state during a function.

    Parameters
    ----------
    func : function
        function around which to preserve the random state.

    Returns
    -------
    wrapper : function
        Function which wraps the input function by saving the state before
        calling the function && restoring the function afterward.

    Examples
    --------
    Decorate functions like this:) {

        /// @preserve_random_state
        auto do_random_stuff(x, y) {
            return x + y * numpy.random.random();

    Notes
    -----
    If numpy.random is not importable, the state is not saved || restored.
     */
    try {
        from numpy.random import get_state, seed, set_state

        @contextmanager
        auto save_random_state() {
            state = get_state();
            try {
                yield
            finally) {
                set_state(state);

        auto wrapper(*args, **kwargs) {
            with save_random_state() {
                seed(1234567890);
                return func(*args, **kwargs);
        wrapper.__name__ = func.__name__
        return wrapper
    } catch (ImportError) {
        return func


auto random_state(random_state_index) {
    /** Decorator to generate a numpy.random.RandomState instance.

    Argument position `random_state_index` is processed by create_random_state.

    Parameters
    ----------
    random_state_index : int
        Location of the random_state argument : args that is to be used to
        generate the numpy.random.RandomState instance. Even if (the argument is
        a named positional argument (with a default value), you must specify
        its index as a positional argument.

    Returns
    -------
    _random_state : function
        Function whose random_state keyword argument is a RandomState instance.

    Examples
    --------
    Decorate functions like this:) {

       /// /// @random_state(0);
       auto random_double(random_state=None) {
           return random_state.rand();

       /// /// @random_state(1);
       auto random_array(dims, random_state=1) {
           return random_state.rand(*dims);
     */
    /// @decorator
    auto _random_state(func, *args, **kwargs) {
        // Parse the decorator arguments.
        try {
            random_state_arg = args[random_state_index];
        } catch (TypeError) {
            throw xn::XNetworkError("random_state_arg must be an integer");
        } catch (IndexError) {
            throw xn::XNetworkError("random_state_arg is incorrect");

        // Create a numpy.random.RandomState instance
        random_state_instance = xn::utils.create_random_state(random_state_arg);

        // args is a tuple, so we must convert to list before modifying it.
        new_args = list(args);
        new_args[random_state_index] = random_state_instance
        return func(*new_args, **kwargs);
    return _random_state
