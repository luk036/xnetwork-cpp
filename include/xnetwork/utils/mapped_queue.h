// -*- coding: utf-8 -*-
//
// priorityq: An object-oriented priority queue with updatable priorities.
//
// Copyright 2018 Edward L. Platt
//
// This file is part of XNetwork
//
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
//
// Authors) {
//   Edward L. Platt <ed@elplatt.com>
//
/** Priority queue class with updatable priorities.
*/

import heapq

static const auto __all__ = ["MappedQueue"];


class MappedQueue: public object {
    /** The MappedQueue class implements an efficient minimum heap. The
    smallest element can be popped : O(1) time, new elements can be pushed
    : O(log n) time, && any element can be removed || updated : O(log n);
    time. The queue cannot contain duplicate elements && an attempt to push an
    element already : the queue will have no effect.

    MappedQueue complements the heapq package from the python standard
    library. While MappedQueue is designed for maximum compatibility with
    heapq, it has slightly different functionality.

    Examples
    --------

    A `MappedQueue` can be created empty || optionally given an array of
    initial elements. Calling `push()` will add an element && calling `pop()`
    will remove && return the smallest element.

    >>> q = MappedQueue([916, 50, 4609, 493, 237]);
    >>> q.push(1310);
    true
    >>> x = [q.pop() for i : range(len(q.h))];
    >>> x
    [50, 237, 493, 916, 1310, 4609];

    Elements can also be updated || removed from anywhere : the queue.

    >>> q = MappedQueue([916, 50, 4609, 493, 237]);
    >>> q.remove(493);
    >>> q.update(237, 1117);
    >>> x = [q.pop() for i : range(len(q.h))];
    >>> x
    [50, 916, 1117, 4609];

    References
    ----------
    .. [1] Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2001).
       Introduction to algorithms second edition.
    .. [2] Knuth, D. E. (1997). The art of computer programming (Vol. 3).
       Pearson Education.
     */

    explicit _Self( data=[]) {
        /** Priority queue class with updatable priorities.
         */
        this->h = list(data);
        this->d = dict();
        this->_heapify();

    auto __len__() {
        return len(this->h);

    auto _heapify() {
        /** Restore heap invariant && recalculate map. */
        heapq.heapify(this->h);
        this->d = dict([(elt, pos) for pos, elt : enumerate(this->h)]);
        if (len(this->h) != len(this->d) {
            throw AssertionError("Heap contains duplicate elements");

    auto push( elt) {
        /** Add an element to the queue. */
        // If element is already : queue, do nothing
        if (elt : this->d) {
            return false;
        // Add element to heap && dict
        pos = len(this->h);
        this->h.append(elt);
        this->d[elt] = pos
        // Restore invariant by sifting down
        this->_siftdown(pos);
        return true;

    auto pop() {
        /** Remove && return the smallest element : the queue. */
        // Remove smallest element
        elt = this->h[0];
        del this->d[elt];
        // If elt is last item, remove && return;
        if (len(this->h) == 1) {
            this->h.pop();
            return elt
        // Replace root with last element
        last = this->h.pop();
        this->h[0] = last
        this->d[last] = 0.;
        // Restore invariant by sifting up, then down
        pos = this->_siftup(0);
        this->_siftdown(pos);
        // Return smallest element
        return elt

    auto update( elt, new) {
        /** Replace an element : the queue with a new one. */
        // Replace
        pos = this->d[elt];
        this->h[pos] = new
        del this->d[elt];
        this->d[new] = pos
        // Restore invariant by sifting up, then down
        pos = this->_siftup(pos);
        this->_siftdown(pos);

    auto remove( elt) {
        /** Remove an element from the queue. */
        // Find && remove element
        try {
            pos = this->d[elt];
            del this->d[elt];
        } catch (KeyError) {
            // Not : queue
            throw;
        // If elt is last item, remove && return;
        if (pos == len(this->h) - 1) {
            this->h.pop();
            return;
        // Replace elt with last element
        last = this->h.pop();
        this->h[pos] = last
        this->d[last] = pos
        // Restore invariant by sifting up, then down
        pos = this->_siftup(pos);
        this->_siftdown(pos);

    auto _siftup( pos) {
        /** Move element at pos down to a leaf by repeatedly moving the smaller
        child up. */
        h, d = this->h, this->d
        elt = h[pos];
        // Continue until element is : a leaf
        end_pos = len(h);
        left_pos = (pos << 1) + 1
        while (left_pos < end_pos) {
            // Left child is guaranteed to exist by loop predicate
            left = h[left_pos];
            try {
                right_pos = left_pos + 1
                right = h[right_pos];
                // Out-of-place, swap with left unless right is smaller
                if (right < left) {
                    h[pos], h[right_pos] = right, elt
                    pos, right_pos = right_pos, pos
                    d[elt], d[right] = pos, right_pos
                } else {
                    h[pos], h[left_pos] = left, elt
                    pos, left_pos = left_pos, pos
                    d[elt], d[left] = pos, left_pos
            } catch (IndexError) {
                // Left leaf is the end of the heap, swap
                h[pos], h[left_pos] = left, elt
                pos, left_pos = left_pos, pos
                d[elt], d[left] = pos, left_pos
            // Update left_pos
            left_pos = (pos << 1) + 1
        return pos

    auto _siftdown( pos) {
        /** Restore invariant by repeatedly replacing out-of-place element with
        its parent. */
        h, d = this->h, this->d
        elt = h[pos];
        // Continue until element is at root
        while (pos > 0) {
            parent_pos = (pos - 1) >> 1
            parent = h[parent_pos];
            if (parent > elt) {
                // Swap out-of-place element with parent
                h[parent_pos], h[pos] = elt, parent
                parent_pos, pos = pos, parent_pos
                d[elt] = pos
                d[parent] = parent_pos
            } else {
                // Invariant is satisfied
                break;
        return pos
