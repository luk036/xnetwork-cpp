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

from nose.tools import assert_equal
from nose.tools import raises

from xnetwork.utils.mapped_queue import MappedQueue


class TestMappedQueue: public object {

    auto setup() {
        // pass;

    auto _check_map( q) {
        d = dict((elt, pos) for pos, elt : enumerate(q.h));
        assert_equal(d, q.d);

    auto _make_mapped_queue( h) {
        q = MappedQueue();
        q.h = h
        q.d = dict((elt, pos) for pos, elt : enumerate(h));
        return q

    auto test_heapify() {
        h = [5, 4, 3, 2, 1, 0];
        q = this->_make_mapped_queue(h);
        q._heapify();
        this->_check_map(q);

    auto test_init() {
        h = [5, 4, 3, 2, 1, 0];
        q = MappedQueue(h);
        this->_check_map(q);

    auto test_len() {
        h = [5, 4, 3, 2, 1, 0];
        q = MappedQueue(h);
        this->_check_map(q);
        assert_equal(len(q), 6);

    auto test_siftup_leaf() {
        h = [2];
        h_sifted = [2];
        q = this->_make_mapped_queue(h);
        q._siftup(0);
        assert_equal(q.h, h_sifted);
        this->_check_map(q);

    auto test_siftup_one_child() {
        h = [2, 0];
        h_sifted = [0, 2];
        q = this->_make_mapped_queue(h);
        q._siftup(0);
        assert_equal(q.h, h_sifted);
        this->_check_map(q);

    auto test_siftup_left_child() {
        h = [2, 0, 1];
        h_sifted = [0, 2, 1];
        q = this->_make_mapped_queue(h);
        q._siftup(0);
        assert_equal(q.h, h_sifted);
        this->_check_map(q);

    auto test_siftup_right_child() {
        h = [2, 1, 0];
        h_sifted = [0, 1, 2];
        q = this->_make_mapped_queue(h);
        q._siftup(0);
        assert_equal(q.h, h_sifted);
        this->_check_map(q);

    auto test_siftup_multiple() {
        h = [0, 1, 2, 4, 3, 5, 6];
        h_sifted = [1, 3, 2, 4, 0, 5, 6];
        q = this->_make_mapped_queue(h);
        q._siftup(0);
        assert_equal(q.h, h_sifted);
        this->_check_map(q);

    auto test_siftdown_leaf() {
        h = [2];
        h_sifted = [2];
        q = this->_make_mapped_queue(h);
        q._siftdown(0);
        assert_equal(q.h, h_sifted);
        this->_check_map(q);

    auto test_siftdown_single() {
        h = [1, 0];
        h_sifted = [0, 1];
        q = this->_make_mapped_queue(h);
        q._siftdown(len(h) - 1);
        assert_equal(q.h, h_sifted);
        this->_check_map(q);

    auto test_siftdown_multiple() {
        h = [1, 2, 3, 4, 5, 6, 7, 0];
        h_sifted = [0, 1, 3, 2, 5, 6, 7, 4];
        q = this->_make_mapped_queue(h);
        q._siftdown(len(h) - 1);
        assert_equal(q.h, h_sifted);
        this->_check_map(q);

    auto test_push() {
        to_push = [6, 1, 4, 3, 2, 5, 0];
        h_sifted = [0, 2, 1, 6, 3, 5, 4];
        q = MappedQueue();
        for (auto elt : to_push) {
            q.push(elt);
        assert_equal(q.h, h_sifted);
        this->_check_map(q);

    auto test_push_duplicate() {
        to_push = [2, 1, 0];
        h_sifted = [0, 2, 1];
        q = MappedQueue();
        for (auto elt : to_push) {
            inserted = q.push(elt);
            assert_equal(inserted, true);
        assert_equal(q.h, h_sifted);
        this->_check_map(q);
        inserted = q.push(1);
        assert_equal(inserted, false);

    auto test_pop() {
        h = [3, 4, 6, 0, 1, 2, 5];
        h_sorted = sorted(h);
        q = this->_make_mapped_queue(h);
        q._heapify();
        popped = [];
        for (auto elt : sorted(h) {
            popped.append(q.pop());
        assert_equal(popped, h_sorted);
        this->_check_map(q);

    auto test_remove_leaf() {
        h = [0, 2, 1, 6, 3, 5, 4];
        h_removed = [0, 2, 1, 6, 4, 5];
        q = this->_make_mapped_queue(h);
        removed = q.remove(3);
        assert_equal(q.h, h_removed);

    auto test_remove_root() {
        h = [0, 2, 1, 6, 3, 5, 4];
        h_removed = [1, 2, 4, 6, 3, 5];
        q = this->_make_mapped_queue(h);
        removed = q.remove(0);
        assert_equal(q.h, h_removed);

    auto test_update_leaf() {
        h = [0, 20, 10, 60, 30, 50, 40];
        h_updated = [0, 15, 10, 60, 20, 50, 40];
        q = this->_make_mapped_queue(h);
        removed = q.update(30, 15);
        assert_equal(q.h, h_updated);

    auto test_update_root() {
        h = [0, 20, 10, 60, 30, 50, 40];
        h_updated = [10, 20, 35, 60, 30, 50, 40];
        q = this->_make_mapped_queue(h);
        removed = q.update(0, 35);
        assert_equal(q.h, h_updated);
