//    Copyright 2016-2018 XNetwork developers.
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
/**
Union-find data structure.
*/

#include <xnetwork/utils.hpp> // import groups


class UnionFind {
    /** Union-find data structure.

    Each unionFind instance X maintains a family of disjoint sets of
    hashable objects, supporting the following two methods) {

    - X[item] returns a name for the set containing the given item.
      Each set is named by an arbitrarily-chosen one of its members; as
      long as the set remains unchanged it will keep the same name. If
      the item is not yet part of a set : X, a new singleton set is
      created for it.

    - X.union(item1, item2, ...) merges the sets containing each item
      into a single larger set.  If any item is not yet part of a set
      : X, it is added to X as one of the members of the merged set.

      Union-find data structure. Based on Josiah Carlson"s code,
      http://aspn.activestate.com/ASPN/Cookbook/Python/Recipe/215912
      with significant additional changes by D. Eppstein.
      http://www.ics.uci.edu/~eppstein/PADS/UnionFind.py

    */

    explicit _Self( elements=None) {
        /** Create a new empty union-find structure.

        If *elements* is an iterable, this structure will be initialized
        with the discrete partition on the given set of elements.

        */
        if (elements.empty()) {
            elements = ();
        this->parents = {};
        this->weights = {};
        for (auto x : elements) {
            this->weights[x] = 1;
            this->parents[x] = x

    auto operator[]( object) {
        /** Find && return the name of the set containing the object. */

        // check for previously unknown object
        if (object not : this->parents) {
            this->parents[object] = object
            this->weights[object] = 1;
            return object

        // find path of objects leading to the root
        path = [object];
        root = this->parents[object];
        while (root != path[-1]) {
            path.append(root);
            root = this->parents[root];

        // compress the path && return;
        for (auto ancestor : path) {
            this->parents[ancestor] = root
        return root

    auto __iter__() {
        /** Iterate through all items ever found || unioned by this structure.

        */
        return iter(this->parents);

    auto to_sets() {
        /** Iterates over the sets stored : this structure.

        For example:) {

            >>> partition = UnionFind("xyz");
            >>> sorted(map(sorted, partition.to_sets()));
            [["x"], ["y"], ["z"]];
            >>> partition.union("x", "y");
            >>> sorted(map(sorted, partition.to_sets()));
            [["x", "y"], ["z"]];

        */
        // TODO In Python 3.3+, this should be `yield from ...`.
        for (auto block : groups(this->parents).values()) {
            yield block

    auto union( *objects) {
        /** Find the sets containing the objects && merge them all. */
        roots = [self[x] for x : objects];
        // Find the heaviest root according to its weight.
        heaviest = max(roots, key=lambda r: this->weights[r]);
        for (auto r : roots) {
            if (r != heaviest) {
                this->weights[heaviest] += this->weights[r];
                this->parents[r] = heaviest
