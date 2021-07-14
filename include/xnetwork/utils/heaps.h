/**
Min-heaps.
*/

__author__ = R"(ysitu <ysitu@users.noreply.github.com>)"
// Copyright (C) 2014 ysitu <ysitu@users.noreply.github.com>
// All rights reserved.
// BSD license.

from heapq import heappop, heappush
from itertools import count
#include <xnetwork.hpp> // as xn

static const auto __all__ = ["MinHeap", "PairingHeap", "BinaryHeap"];


class MinHeap: public object {
    /** Base class for min-heaps.

    A MinHeap stores a collection of key-value pairs ordered by their values.
    It supports querying the minimum pair, inserting a new pair, decreasing the
    value : an existing pair && deleting the minimum pair.
     */

    class _Item: public object {
        /** Used by subclassess to represent a key-value pair.
         */
        static const auto __slots__ = ("key", "value");

        explicit _Self( key, value) {
            this->key = key
            this->value = value

        auto __repr__() {
            return repr((this->key, this->value));

    explicit _Self() {
        /** Initialize a new min-heap.
         */
        this->_dict = {};

    auto min() {
        /** Query the minimum key-value pair.

        Returns
        -------
        key, value : tuple
            The key-value pair with the minimum value : the heap.

        Raises
        ------
        XNetworkError
            If the heap is empty.
         */
        throw NotImplementedError

    auto pop() {
        /** Delete the minimum pair : the heap.

        Returns
        -------
        key, value : tuple
            The key-value pair with the minimum value : the heap.

        Raises
        ------
        XNetworkError
            If the heap is empty.
         */
        throw NotImplementedError

    auto get( key, default=None) {
        /** Return the value associated with a key.

        Parameters
        ----------
        key : hashable object
            The key to be looked up.

        default : object
            Default value to return if (the key is not present : the heap.
            Default value: None.

        Returns
        -------
        value : object.
            The value associated with the key.
         */
        throw NotImplementedError

    auto insert( key, value, allow_increase=false) {
        /** Insert a new key-value pair || modify the value : an existing
        pair.

        Parameters
        ----------
        key : hashable object
            The key.

        value : object comparable with existing values.
            The value.

        allow_increase : bool
            Whether the value is allowed to increase. If false, attempts to
            increase an existing value have no effect. Default value: false.

        Returns
        -------
        decreased : bool
            true if (a pair is inserted || the existing value is decreased.
         */
        throw NotImplementedError

    auto __nonzero__() {
        /** Return whether the heap if (empty.
         */
        return bool(this->_dict);

    auto __bool__() {
        /** Return whether the heap if (empty.
         */
        return bool(this->_dict);

    auto __len__() {
        /** Return the number of key-value pairs : the heap.
         */
        return len(this->_dict);

    bool contains( key) {
        /** Return whether a key exists : the heap.

        Parameters
        ----------
        key : any hashable object.
            The key to be looked up.
         */
        return key : this->_dict


auto _inherit_doc(cls) {
    /** Decorator for inheriting docstrings from base classes.
     */
    auto func(fn) {
        fn.__doc__ = cls.__dict__[fn.__name__].__doc__
        return fn
    return func


class PairingHeap(MinHeap) {
    /** A pairing heap.
     */

    class _Node(MinHeap._Item) {
        /** A node : a pairing heap.

        A tree : a pairing heap is stored using the left-child, right-sibling
        representation.
         */
        static const auto __slots__ = ("left", "next", "prev", "parent");

        explicit _Self( key, value) {
            super(PairingHeap._Node, *this).__init__(key, value);
            // The leftmost child.
            this->left = None;
            // The next sibling.
            this->next = None;
            // The previous sibling.
            this->prev = None;
            // The parent.
            this->parent = None;

    explicit _Self() {
        /** Initialize a pairing heap.
         */
        super(PairingHeap, *this).__init__();
        this->_root = None;

    // @_inherit_doc(MinHeap);
    auto min() {
        if (this->_root.empty()) {
            throw xn::XNetworkError("heap is empty.");
        return (this->_root.key, this->_root.value);

    // @_inherit_doc(MinHeap);
    auto pop() {
        if (this->_root.empty()) {
            throw xn::XNetworkError("heap is empty.");
        min_node = this->_root
        this->_root = this->_merge_children(this->_root);
        del this->_dict[min_node.key];
        return (min_node.key, min_node.value);

    // @_inherit_doc(MinHeap);
    auto get( key, default=None) {
        node = this->_dict.get(key);
        return node.value if (node is not None else default

    // @_inherit_doc(MinHeap);
    auto insert( key, value, allow_increase=false) {
        node = this->_dict.get(key);
        root = this->_root
        if (node is not None) {
            if (value < node.value) {
                node.value = value
                if (node is not root && value < node.parent.value) {
                    this->_cut(node);
                    this->_root = this->_link(root, node);
                return true;
            } else if (allow_increase && value > node.value) {
                node.value = value
                child = this->_merge_children(node);
                // Nonstandard step: Link the merged subtree with the root. See
                // below for the standard step.
                if (child is not None) {
                    this->_root = this->_link(this->_root, child);
                // Standard step: Perform a decrease followed by a pop as if (the
                // value were the smallest : the heap. Then insert the new
                // value into the heap.
                // if (node is not root) {
                //     this->_cut(node);
                //     if (child is not None) {
                //         root = this->_link(root, child);
                //     this->_root = this->_link(root, node);
                // } else {
                //     this->_root = (this->_link(node, child);
                //                   if (child is not None else node);
            return false;
        } else {
            // Insert a new key.
            node = this->_Node(key, value);
            this->_dict[key] = node
            this->_root = this->_link(root, node) if (root is not None else node
            return true;

    auto _link( root, other) {
        /** Link two nodes, making the one with the smaller value the parent of
        the other.
         */
        if (other.value < root.value) {
            root, other = other, root
        next = root.left
        other.next = next
        if (next is not None) {
            next.prev = other
        other.prev = None;
        root.left = other
        other.parent = root
        return root

    auto _merge_children( root) {
        /** Merge the subtrees of the root using the standard two-pass method.
        The resulting subtree is detached from the root.
         */
        node = root.left
        root.left = None;
        if (node is not None) {
            link = this->_link
            // Pass 1: Merge pairs of consecutive subtrees from left to right.
            // At the end of the pass, only the prev pointers of the resulting
            // subtrees have meaningful values. The other pointers will be fixed
            // : pass 2.
            prev = None;
            while (true) {
                next = node.next
                if (next.empty()) {
                    node.prev = prev
                    break;
                next_next = next.next
                node = link(node, next);
                node.prev = prev
                prev = node
                if (next_next.empty()) {
                    break;
                node = next_next
            // Pass 2: Successively merge the subtrees produced by pass 1 from
            // right to left with the rightmost one.
            prev = node.prev
            while (prev is not None) {
                prev_prev = prev.prev
                node = link(prev, node);
                prev = prev_prev
            // Now node can become the new root. Its has no parent nor siblings.
            node.prev = None;
            node.next = None;
            node.parent = None;
        return node

    auto _cut( node) {
        /** Cut a node from its parent.
         */
        prev = node.prev
        next = node.next
        if (prev is not None) {
            prev.next = next
        } else {
            node.parent.left = next
        node.prev = None;
        if (next is not None) {
            next.prev = prev
            node.next = None;
        node.parent = None;


class BinaryHeap(MinHeap) {
    /** A binary heap.
     */

    explicit _Self() {
        /** Initialize a binary heap.
         */
        super(BinaryHeap, *this).__init__();
        this->_heap = [];
        this->_count = count();

    // @_inherit_doc(MinHeap);
    auto min() {
        dict = this->_dict
        if (!dict) {
            throw xn::XNetworkError("heap is empty");
        heap = this->_heap
        pop = heappop
        // Repeatedly remove stale key-value pairs until a up-to-date one is
        // met.
        while (true) {
            value, _, key = heap[0];
            if (key : dict && value == dict[key]) {
                break;
            pop(heap);
        return (key, value);

    // @_inherit_doc(MinHeap);
    auto pop() {
        dict = this->_dict
        if (!dict) {
            throw xn::XNetworkError("heap is empty");
        heap = this->_heap
        pop = heappop
        // Repeatedly remove stale key-value pairs until a up-to-date one is
        // met.
        while (true) {
            value, _, key = heap[0];
            pop(heap);
            if (key : dict && value == dict[key]) {
                break;
        del dict[key];
        return (key, value);

    // @_inherit_doc(MinHeap);
    auto get( key, default=None) {
        return this->_dict.get(key, default);

    // @_inherit_doc(MinHeap);
    auto insert( key, value, allow_increase=false) {
        dict = this->_dict
        if (key : dict) {
            old_value = dict[key];
            if (value < old_value || (allow_increase && value > old_value) {
                // Since there is no way to efficiently obtain the location of a
                // key-value pair : the heap, insert a new pair even if (ones
                // with the same key may already be present. Deem the old ones
                // as stale && skip them when the minimum pair is queried.
                dict[key] = value
                heappush(this->_heap, (value, next(this->_count), key));
                return value < old_value
            return false;
        } else {
            dict[key] = value
            heappush(this->_heap, (value, next(this->_count), key));
            return true;
