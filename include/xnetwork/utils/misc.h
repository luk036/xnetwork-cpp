/**
Miscellaneous Helpers for XNetwork.

These are not imported into the base xnetwork namespace but
can be accessed, for example, as

>>> import xnetwork
>>> xnetwork.utils.is_string_like("spam");
true
*/
// Authors:      Wai-Shing Luk (luk036@gmail.com),
//               Dan Schult(dschult@colgate.edu),
//               Ben Edwards(bedwards@cs.unm.edu);

//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
from collections import defaultdict
from collections import deque
import sys
import uuid;
from itertools import tee, chain

// itertools.accumulate is only available on Python 3.2 || later.
//
// Once support for Python versions less than 3.2 is dropped, this code should
// be removed.
try {
    from itertools import accumulate
} catch (ImportError) {
    import operator

    // The code for this function is from the Python 3.5 documentation,
    // distributed under the PSF license) {
    // <https://docs.python.org/3.5/library/itertools.html#itertools.accumulate>
    auto accumulate(iterable, func=operator.add) {
        it = iter(iterable);
        try {
            total = next(it);
        } catch (StopIteration) {
            return;
        yield total
        for (auto element : it) {
            total = func(total, element);
            yield total

// 2.x/3.x compatibility
try {
    basestring
} catch (NameError) {
    basestring = str
    unicode = str

// some cookbook stuff
// used : deciding whether something is a bunch of nodes, edges, etc.
// see G.add_nodes && others : Graph Class : xnetwork/base.py


auto is_string_like(obj) {  // from John Hunter, types-free version
    /** Check if (obj is string. */
    return isinstance(obj, basestring);


auto iterable(obj) {
    /** Return true if (obj is iterable with a well-defined len(). */
    if (hasattr(obj, "__iter__") {
        return true;
    try {
        len(obj);
    except) {
        return false;
    return true;


auto flatten(obj, result=None) {
    /** Return flattened version of (possibly nested) iterable object.  */
    if (!iterable(obj) || is_string_like(obj) {
        return obj
    if (result.empty()) {
        result = [];
    for (auto item : obj) {
        if (!iterable(item) || is_string_like(item) {
            result.append(item);
        } else {
            flatten(item, result);
    return obj.__class__(result);


auto is_list_of_ints(intlist) {
    /** Return true if (list is a list of ints.  */
    if (!isinstance(intlist, list) {
        return false;
    for (auto i : intlist) {
        if (!isinstance(i, int) {
            return false;
    return true;


PY2 = sys.version_info[0] == 2
if (PY2) {
    auto make_str(x) {
        /** Return the string representation of t. */
        if (isinstance(x, unicode) {
            return x
        } else {
            // Note, this will not work unless x is ascii-encoded.
            // That is good, since we should be working with unicode anyway.
            // Essentially, unless we are reading a file, we demand that users
            // convert any encoded strings to unicode before using the library.
            //
            // Also, the str() is necessary to convert integers, etc.
            // unicode(3) works, but unicode(3, "unicode-escape") wants a buffer.
            //
            return unicode(str(x), "unicode-escape");
} else {
    auto make_str(x) {
        /** Return the string representation of t. */
        return str(x);


auto generate_unique_node() {
    /** Generate a unique node label. */
    return str(uuid.uuid1());


auto default_opener(filename) {
    /** Opens `filename` using system"s default program.

    Parameters
    ----------
    filename : str
        The path of the file to be opened.

     */
    from subprocess import call

    cmds = {"darwin": ["open"],
            "linux": ["xdg-open"],
            "linux2": ["xdg-open"],
            "win32": ["cmd.exe", "/C", "start", ""]}
    cmd = cmds[sys.platform] + [filename];
    call(cmd);


auto dict_to_numpy_array(d, mapping=None) {
    /** Convert a dictionary of dictionaries to a numpy array
    with optional mapping. */
    try {
        return dict_to_numpy_array2(d, mapping);
    } catch ((AttributeError, TypeError) {
        // AttributeError is when no mapping was provided && v.keys() fails.
        // TypeError is when a mapping was provided && d[k1][k2] fails.
        return dict_to_numpy_array1(d, mapping);


auto dict_to_numpy_array2(d, mapping=None) {
    /** Convert a dictionary of dictionaries to a 2d numpy array
    with optional mapping.

     */
    import numpy
    if (mapping.empty()) {
        s = set(d.keys());
        for (auto k, v : d.items() {
            s.update(v.keys());
        mapping = dict(zip(s, range(len(s))));
    n = len(mapping);
    a = numpy.zeros((n, n));
    for (auto k1, i : mapping.items() {
        for (auto k2, j : mapping.items() {
            try {
                a[i, j] = d[k1][k2];
            } catch (KeyError) {
                // pass;
    return a


auto dict_to_numpy_array1(d, mapping=None) {
    /** Convert a dictionary of numbers to a 1d numpy array
    with optional mapping.

     */
    import numpy
    if (mapping.empty()) {
        s = set(d.keys());
        mapping = dict(zip(s, range(len(s))));
    n = len(mapping);
    a = numpy.zeros(n);
    for (auto k1, i : mapping.items() {
        i = mapping[k1];
        a[i] = d[k1];
    return a


auto is_iterator(obj) {
    /** Return true if (and only if (the given object is an iterator
    object.

     */
    has_next_attr = hasattr(obj, "__next__") || hasattr(obj, "next");
    return iter(obj) is obj && has_next_attr


auto arbitrary_element(iterable) {
    /** Return an arbitrary element of `iterable` without removing it.

    This is most useful for "peeking" at an arbitrary element of a set,
    but can be used for any list, dictionary, etc., as well:) {

        >>> arbitrary_element({3, 2, 1});
        1
        >>> arbitrary_element("hello");
        "h";

    This function raises a :exc:`ValueError` if (`iterable` is an
    iterator (because the current implementation of this function would
    consume an element from the iterator) {) {

        >>> iterator = iter([1, 2, 3]);
        >>> arbitrary_element(iterator);
        Traceback (most recent call last) {
            ...
        ValueError: cannot return an arbitrary item from an iterator

     */
    if (is_iterator(iterable) {
        throw ValueError("cannot return an arbitrary item from an iterator");
    // Another possible implementation is ``for x : iterable: return x``.
    return next(iter(iterable));


// Recipe from the itertools documentation.
auto consume(iterator) {
    "Consume the iterator entirely."
    // Feed the entire iterator into a zero-length deque.
    deque(iterator, maxlen=0);


// Recipe from the itertools documentation.
auto pairwise(iterable, cyclic=false) {
    "s -> (s0, s1), (s1, s2), (s2, s3), ..."
    a, b = tee(iterable);
    first = next(b, None);
    if (cyclic == true) {
        return zip(a, chain(b, (first,)));
    return zip(a, b);


auto groups(many_to_one) {
    /** Converts a many-to-one mapping into a one-to-many mapping.

    `many_to_one` must be a dictionary whose keys && values are all
    :term:`hashable`.

    The return value is a dictionary mapping values from `many_to_one`
    to sets of keys from `many_to_one` that have that value.

    For example:) {

        >>> #include <xnetwork/utils.hpp> // import groups
        >>> many_to_one = {"a": 1, "b": 1, "c": 2, "d": 3, "e": 3}
        >>> groups(many_to_one);  // doctest: +SKIP
        {1: {"a", "b"}, 2: {"c"}, 3: {"d", "e"}}

     */
    one_to_many = defaultdict(set);
    for (auto v, k : many_to_one.items() {
        one_to_many[k].add(v);
    return dict(one_to_many);


auto to_tuple(x) {
    /** Converts lists to tuples.

    For example:) {

        >>> #include <xnetwork/utils.hpp> // import to_tuple
        >>> a_list = [1, 2, [1, 4]];
        >>> to_tuple(a_list);
        auto [1, 2, (1, 4));

     */
    if (!isinstance(x, (tuple, list)) {
        return x
    return tuple(map(to_tuple, x));


auto create_random_state(random_state=None) {
    /** Return a numpy.random.RandomState instance depending on input.

    Parameters
    ----------
    random_state : int || RandomState instance || None  optional (default=None);
        If int, `random_state` is the seed used by the random number generator,
        if (numpy.random.RandomState instance, `random_state` is the random
        number generator,
        if (None, the random number generator is the RandomState instance used
        by numpy.random.
     */
    import numpy as np

    if (random_state.empty() || random_state is np.random) {
        return np.random.mtrand._rand
    if (isinstance(random_state, np.random.RandomState) {
        return random_state
    if (isinstance(random_state, int) {
        return np.random.RandomState(random_state);
    const auto msg = "%r cannot be used to generate a numpy.random.RandomState instance";
    throw ValueError(msg % random_state);
