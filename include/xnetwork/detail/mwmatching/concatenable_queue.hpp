/**
 * Concatenable queue data structure.
 */

#ifndef MWMATCHING_CONCATENABLE_QUEUE_H_
#define MWMATCHING_CONCATENABLE_QUEUE_H_

#include <algorithm>
#include <cassert>
#include <tuple>


namespace mwmatching {


/**
 * Priority queue supporting efficient merge and split operations.
 *
 * Conceptually, this is a combination of a disjoint set and a priority queue.
 * Each queue has a "name".
 * Each element has associated "data".
 * Each element has a priority.
 * Each element is contained in at most one queue at any time.
 *
 * The following operations can be done efficiently:
 *  - Find the name of the queue that contains a given element.
 *  - Change the priority of a given element.
 *  - Find the element with lowest priority in a given queue.
 *  - Merge two or more queues.
 *  - Undo a previous merge step.
 *
 * A ConcatenableQueue instance may be destructed if it is empty and not
 * currently merged into a larger queue. Alternatively, a group of related
 * ConcatenableQueue instances and their Node instances may be destructed
 * together, even if non-empty. In this case, the objects may be destructed
 * in any order. No other interactions with the objects are allowed once
 * destruction has started.
 *
 * This data structure is implemented as an AVL tree, with minimum-priority
 * tracking added to it.
 * See also
 *   https://en.wikipedia.org/wiki/Avl_tree
 * and
 *   G. Blelloch, D. Ferizovic, Y. Sun, Parallel Ordered Sets Using Join,
 *   https://arxiv.org/abs/1602.02120
 */
template <typename PrioType,
          typename NameType,
          typename DataType>
class ConcatenableQueue
{
public:

    /**
     * A Node instance represents an element in a concatenable queue.
     *
     * A Node instance must remain valid while it is contained in a queue.
     * The containing queue holds a pointer to the Node.
     *
     * A Node instance may be destructed if it is not contained in any queue.
     * Alternatively, a Node instance may be destructed just before or after
     * destructing the containing queue. In this case, no intermediate
     * interactions with the node or queue are allowed.
     */
    class Node
    {
      public:
        /** Construct an unused node, not yet contained in any queue. */
        Node()
          : owner_(nullptr)
          , parent_(nullptr)
          , left_child_(nullptr)
          , right_child_(nullptr)
          , height_(0)
        { }

        // Prevent copying.
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;

        /**
         * Return the name of the queue that contains this element.
         *
         * The node must be contained in a queue.
         * This function takes time O(log(n)).
         */
        NameType find() const
        {
            const Node* node = this;
            while (node->parent_) {
                node = node->parent_;
            }
            assert(node->owner_);
            return node->owner_->name();
        }

        /**
         * Return the priority of this element.
         *
         * The node must be contained in a queue.
         * This function takes time O(1).
         */
        PrioType prio() const
        {
            assert(height_ != 0);
            return prio_;
        }

        /**
         * Change the priority of this element.
         *
         * The node must be contained in a queue.
         * This function takes time O(log(n)).
         */
        void set_prio(PrioType new_prio)
        {
            assert(height_ != 0);

            prio_ = new_prio;

            Node* node = this;
            while (node) {
                PrioType min_prio = node->prio_;
                DataType min_data = node->data_;
                for (Node* child : { node->left_child_,
                                     node->right_child_ }) {
                    if (child && child->min_prio_ < min_prio) {
                        min_prio = child->min_prio_;
                        min_data = child->min_data_;
                    }
                }
                node->min_prio_ = min_prio;
                node->min_data_ = min_data;
                node = node->parent_;
            }
        }

      private:
        PrioType            prio_;
        DataType            data_;
        PrioType            min_prio_;
        DataType            min_data_;
        ConcatenableQueue*  owner_;
        Node*               parent_;
        Node*               left_child_;
        Node*               right_child_;
        unsigned int        height_;

        friend class ConcatenableQueue;
    };

    /** Construct an empty queue. */
    explicit ConcatenableQueue(const NameType& name)
      : name_(name)
      , tree_(nullptr)
      , first_node_(nullptr)
      , first_subqueue_(nullptr)
      , next_subqueue_(nullptr)
    { }

    // Prevent copying.
    ConcatenableQueue(const ConcatenableQueue&) = delete;
    ConcatenableQueue& operator=(const ConcatenableQueue&) = delete;

    /** Return the name of this queue. */
    NameType name() const
    {
        return name_;
    }

    /**
     * Insert the specified Node into the queue.
     *
     * The Node instance must be unused (not yet contained in any queue).
     *
     * The queue must be empty. Only one element can be inserted into
     * a queue in this way. Larger queues can only result from merging.
     *
     * The queue stores a pointer to the Node instance. The Node instance must
     * remain valid for as long as it is contained in any queue.
     *
     * This function takes time O(1).
     */
    void insert(Node* node, PrioType prio, const DataType& data)
    {
        assert(! tree_);
        assert(! first_node_);
        assert(node->height_ == 0);
        assert(! node->parent_);
        assert(! node->left_child_);
        assert(! node->right_child_);

        node->prio_ = prio;
        node->data_ = data;
        node->min_prio_ = prio;
        node->min_data_ = data;
        node->owner_ = this;
        node->height_ = 1;

        tree_ = node;
        first_node_ = node;
    }

    /**
     * Return the minimum priority of any element in the queue.
     *
     * The queue must be non-empty.
     * This function takes time O(1).
     */
    PrioType min_prio() const
    {
        assert(tree_);
        return tree_->min_prio_;
    }

    /**
     * Return the element with minimum priority.
     *
     * The queue must be non-empty.
     * This function takes time O(1).
     */
    DataType min_elem() const
    {
        assert(tree_);
        return tree_->min_data_;
    }

    /**
     * Merge the specified queues.
     *
     * This queue instance must inititially be empty.
     * All specified sub-queues must initially be non-empty.
     *
     * This function removes all elements from the specified sub-queues
     * and adds them to this queue.
     *
     * After merging, this queue retains references to the sub-queues.
     * These may be used later to split (undo the merge step).
     *
     * This function takes time O(n_sub_queues * log(n)).
     */
    template <typename QueueIterator>
    void merge(QueueIterator sub_queues_begin, QueueIterator sub_queues_end)
    {
        assert(! tree_);
        assert(! first_node_);
        assert(sub_queues_begin != sub_queues_end);

        // Pull the root node from the first sub-queue.
        ConcatenableQueue* sub = *sub_queues_begin;
        assert(sub->tree_);
        Node* merged_tree = sub->tree_;
        sub->tree_ = nullptr;

        // Clear owner pointer from tree.
        assert(merged_tree->owner_ == sub);
        merged_tree->owner_ = nullptr;

        // Copy first node to this queue.
        assert(sub->first_node_);
        first_node_ = sub->first_node_;

        // Build linked list of sub-queues, starting with the first sub-queue.
        assert(! sub->next_subqueue_);
        first_subqueue_ = sub;

        // Merge remaining sub-queues.
        QueueIterator it = sub_queues_begin;
        ++it;
        while (it != sub_queues_end) {
            ConcatenableQueue* prev_sub = sub;
            sub = *it;

            // Clear owner pointer and root node from the sub-queue.
            assert(sub->tree_);
            assert(sub->tree_->owner_ == sub);
            sub->tree_->owner_ = nullptr;

            // Merge sub-queue tree into our current tree.
            merged_tree = merge_tree(merged_tree, sub->tree_);

            // Clear root pointer from sub-queue.
            sub->tree_ = nullptr;

            // Add sub-queue to linked list of sub-queues.
            assert(! sub->next_subqueue_);
            prev_sub->next_subqueue_ = sub;

            ++it;
        }

        // Put owner pointer in the root node of the tree.
        merged_tree->owner_ = this;
        tree_ = merged_tree;
    }

    /**
     * Undo the merge step that created this queue.
     *
     * Remove all elements from this queue and put them back in
     * the sub-queues from which they came.
     *
     * After splitting, this queue will be empty.
     *
     * This function takes time O(n_sub_queues * log(n)).
     */
    void split()
    {
        assert(tree_);
        assert(first_subqueue_);

        // Clear the owner pointer from the root node.
        assert(tree_->owner_ == this);
        tree_->owner_ = nullptr;

        Node* rtree = tree_;
        ConcatenableQueue* sub = first_subqueue_;

        // Repeatedly split the tree to reconstruct each sub-queue.
        while (sub->next_subqueue_) {
            ConcatenableQueue* next_sub = sub->next_subqueue_;
            sub->next_subqueue_ = nullptr;

            // Split the tree on the first node of the next sub-queue.
            assert(next_sub->first_node_);
            Node* ltree;
            std::tie(ltree, rtree) = split_tree(next_sub->first_node_);

            // Put the left half of the tree in the current sub-queue.
            sub->tree_ = ltree;
            ltree->owner_ = sub;

            // Continue to next sub-queue.
            sub = next_sub;
        }

        // Put the remaining part of the tree in the last sub-queue.
        rtree->owner_ = sub;
        sub->tree_ = rtree;

        // Clean up this instance.
        tree_ = nullptr;
        first_node_ = nullptr;
        first_subqueue_ = nullptr;
    }

private:
    /**
     * Merge two trees and rebalance.
     *
     * This function takes time O(log(n)).
     */
    static Node* merge_tree(Node* ltree, Node* rtree)
    {
        // Remove the last node from the left tree.
        Node* node;
        std::tie(ltree, node) = split_last_node(ltree);

        // Join the trees.
        return join(ltree, node, rtree);
    }

    /**
     * Remove the last node from the tree and rebalance.
     *
     * This function takes time O(log(n)).
     */
    static std::tuple<Node*, Node*> split_last_node(Node* tree)
    {
        assert(tree);

        /*
         * Descend down the right spine of the tree to find the last node.
         *
         *      tree
         *      /  \
         *          X
         *         / \
         *            X  <-- parent
         *           / \
         *             last_node
         *              /
         *             X  <-- ltree
         */

        // Find last node.
        Node* last_node = tree;
        while (last_node->right_child_) {
            last_node = last_node->right_child_;
        }

        // Detach its left subtree.
        Node* ltree = last_node->left_child_;
        last_node->left_child_ = nullptr;
        if (ltree) {
            ltree->parent_ = nullptr;
        }

        // Detach from the parent.
        Node* parent = last_node->parent_;
        last_node->parent_ = nullptr;
        if (parent) {
            parent->right_child_ = nullptr;
        }

        // Reconstruct along the right spine of the original tree.
        while (parent) {

            // Step to parent.
            Node* cur = parent;
            parent = cur->parent_;

            // Detach from its own parent.
            cur->parent_ = nullptr;
            if (parent) {
                parent->right_child_ = nullptr;
            }

            // Join the current node to the reconstructed tree.
            ltree = join(cur->left_child_, cur, ltree);
        }

        return std::make_tuple(ltree, last_node);
    }

    /**
     * Split a tree on a specified node.
     *
     * Two new trees will be constructed.
     * Leaf nodes to the left of "split_node" will go to the left tree.
     * Leaf nodes to the right of "split_node", and "split_node" itself,
     * will go to the right tree.
     *
     * This function takes time O(log(n)).
     */
    static std::tuple<Node*, Node*> split_tree(Node* split_node)
    {
        assert(split_node);

        // All left-descendants of "split_node" belong in the left tree.
        // Detach it from "split_node".
        Node* ltree = split_node->left_child_;
        split_node->left_child_ = nullptr;
        if (ltree) {
            ltree->parent_ = nullptr;
        }

        // Start with an empty right tree.
        Node* rtree = nullptr;

        // Start at "split_node".
        // Take note of the fact that the "cut" between the two halves of
        // the tree runs down its left branch, since "split_node" itself
        // belongs to the right tree.
        Node* node = split_node;
        bool left_branch = true;

        // Work upwards through the tree, assigning each node either to
        // the new left tree or the new right tree.
        //
        // This loop runs for O(log(n)) iterations.
        // Each iteration calls join() once, taking time proportional
        // to the difference in height between the intermediate trees.
        // The total run time of all join() calls together is O(log(n)).
        while (node) {

            // Detach the current node from its parent.
            // Remember to which branch of the parent it was attached.
            Node* parent = node->parent_;
            node->parent_ = nullptr;
            bool parent_left_branch = true;
            if (parent) {
                parent_left_branch = (parent->left_child_ == node);
                if (parent_left_branch) {
                    parent->left_child_ = nullptr;
                } else {
                    assert(parent->right_child_ == node);
                    parent->right_child_ = nullptr;
                }
            }

            // Join the current node and its remaining descendents either
            // to the left tree or to the right tree.
            if (left_branch) {
                /*
                 * "node" belongs to the right tree.
                 * Join like this:
                 *
                 *             (node)  <--- new rtree
                 *             /    \
                 *        (rtree)  (node->right_child)
                 */
                assert(! node->left_child_);
                rtree = join(rtree, node, node->right_child_);
            } else {
                /*
                 * "node" belongs to the left tree.
                 * Join like this:
                 *
                 *                   (node)  <--- new ltree
                 *                   /    \
                 *   (node->left_child)   (ltree)
                 */
                assert(! node->right_child_);
                ltree = join(node->left_child_, node, ltree);
            }

            // Continue with the parent node.
            node = parent;
            left_branch = parent_left_branch;
        }

        // Done. All that remains of the old tree is the two new halves.
        return std::make_tuple(ltree, rtree);
    }

    /** Return node height, or 0 if node == nullptr. */
    static unsigned int get_node_height(const Node* node)
    {
        return node ? node->height_ : 0;
    }

    /**
     * Repair the height and min-priority information of a node
     * after modifying its children.
     *
     * After repairing a node, it is typically necessary to also repair
     * its ancestors.
     */
    static void repair_node(Node* node)
    {
        Node* lchild = node->left_child_;
        Node* rchild = node->right_child_;

        // Repair node height.
        node->height_ = 1 + std::max(get_node_height(lchild),
                                     get_node_height(rchild));

        // Repair min-priority.
        PrioType min_prio = node->prio_;
        DataType min_data = node->data_;
        for (Node* child : { lchild, rchild }) {
            if (child && child->min_prio_ < min_prio) {
                min_prio = child->min_prio_;
                min_data = child->min_data_;
            }
        }
        node->min_prio_ = min_prio;
        node->min_data_ = min_data;
    }

    /** Rotate the subtree to the left and return the new root of the subtree. */
    static Node* rotate_left(Node* node)
    {
        /*
         *     N                C
         *    / \              / \
         *   A   C    --->    N   D
         *      / \          / \
         *     B   D        A   B
         */
        Node* parent = node->parent_;
        Node* new_top = node->right_child_;
        assert(new_top);

        Node* nb = new_top->left_child_;
        node->right_child_ = nb;
        if (nb) {
            nb->parent_ = node;
        }

        new_top->left_child_ = node;
        node->parent_ = new_top;

        new_top->parent_ = parent;

        if (parent) {
            if (parent->left_child_ == node) {
                parent->left_child_ = new_top;
            } else {
                assert(parent->right_child_ == node);
                parent->right_child_ = new_top;
            }
        }

        repair_node(node);
        repair_node(new_top);

        return new_top;
    }

    /** Rotate the subtree to the right and return the new root of the subtree. */
    static Node* rotate_right(Node* node)
    {
        /*
         *       N                B
         *      / \              / \
         *     B   D    --->    A   N
         *    / \                  / \
         *   A   C                C   D
         */
        Node* parent = node->parent_;
        Node* new_top = node->left_child_;
        assert(new_top);

        Node* nc = new_top->right_child_;
        node->left_child_ = nc;
        if (nc) {
            nc->parent_ = node;
        }

        new_top->right_child_ = node;
        node->parent_ = new_top;

        new_top->parent_ = parent;

        if (parent) {
            if (parent->left_child_ == node) {
                parent->left_child_ = new_top;
            } else {
                assert(parent->right_child_ == node);
                parent->right_child_ = new_top;
            }
        }

        repair_node(node);
        repair_node(new_top);

        return new_top;
    }

    /**
     * Join a left subtree, middle node and right subtree together.
     *
     * The left subtree is higher than the right subtree.
     */
    static Node* join_right(Node* ltree, Node* node, Node* rtree)
    {
        assert(ltree);
        unsigned int lh = ltree->height_;
        unsigned int rh = get_node_height(rtree);
        assert(lh > rh + 1);

        /*
         * Descend down the right spine of "ltree".
         * Stop at a node with compatible height, then insert "node"
         * and attach "rtree".
         *
         *     ltree
         *      /  \
         *          X
         *         / \
         *            X  <-- cur
         *           / \
         *             node
         *              / \
         *             X   rtree
         */

        // Descend to a point with compatible height.
        Node* cur = ltree;
        while (cur->right_child_ && (cur->right_child_->height_ > rh + 1)) {
            cur = cur->right_child_;
        }

        // Insert "node" and "rtree".
        node->left_child_ = cur->right_child_;
        node->right_child_ = rtree;
        if (node->left_child_) {
            node->left_child_->parent_ = node;
        }
        if (rtree) {
            rtree->parent_ = node;
        }
        cur->right_child_ = node;
        node->parent_ = cur;

        // A double rotation may be necessary.
        if ((! cur->left_child_) || (cur->left_child_->height_ <= rh)) {
            node = rotate_right(node);
            cur = rotate_left(cur);
        } else {
            repair_node(node);
            repair_node(cur);
        }

        // Ascend from "cur" to the root of the tree; repair and rebalance.
        while (cur->parent_) {
            cur = cur->parent_;
            assert(cur->left_child_);
            assert(cur->right_child_);

            if (cur->left_child_->height_ + 1 < cur->right_child_->height_) {
                cur = rotate_left(cur);
            } else {
                repair_node(cur);
            }
        }

        return cur;
    }

    /**
     * Join a left subtree, middle node and right subtree together.
     *
     * The right subtree is higher than the left subtree.
     */
    static Node* join_left(Node* ltree, Node* node, Node* rtree)
    {
        assert(rtree);
        unsigned int lh = get_node_height(ltree);
        unsigned int rh = rtree->height_;
        assert(lh + 1 < rh);

        /*
         * Descend down the left spine of "rtree".
         * Stop at a node with compatible height, then insert "node"
         * and attach "ltree".
         *
         *             rtree
         *             /  \
         *            X
         *           / \
         * cur -->  X
         *         / \
         *      node
         *       / \
         *  ltree   X
         */

        // Descend to a point with compatible height.
        Node* cur = rtree;
        while (cur->left_child_ && (cur->left_child_->height_ > lh + 1)) {
            cur = cur->left_child_;
        }

        // Insert "node" and "ltree".
        node->left_child_ = ltree;
        node->right_child_ = cur->left_child_;
        if (ltree) {
            ltree->parent_ = node;
        }
        if (node->right_child_) {
            node->right_child_->parent_ = node;
        }
        cur->left_child_ = node;
        node->parent_ = cur;

        // A double rotation may be necessary.
        if ((! cur->right_child_) || (cur->right_child_->height_ <= lh)) {
            node = rotate_left(node);
            cur = rotate_right(cur);
        } else {
            repair_node(node);
            repair_node(cur);
        }

        // Ascend from "cur" to the root of the tree; repair and rebalance.
        while (cur->parent_) {
            cur = cur->parent_;
            assert(cur->left_child_);
            assert(cur->right_child_);

            if (cur->left_child_->height_ > cur->right_child_->height_ + 1) {
                cur = rotate_right(cur);
            } else {
                repair_node(cur);
            }
        }

        return cur;
    }

    /**
     * Join a left subtree, middle node and right subtree together.
     *
     * The left or right subtree may initially be a child of the middle
     * node; such links will be broken as needed.
     *
     * The left and right subtrees must be consistent, semi-balanced trees.
     * Height and priority of the middle node may initially be inconsistent;
     * this function will repair it.
     *
     * @return Root node of the joined tree.
     */
    static Node* join(Node* ltree, Node* node, Node* rtree)
    {
        unsigned int lh = get_node_height(ltree);
        unsigned int rh = get_node_height(rtree);

        if (lh > rh + 1) {
            assert(ltree);
            ltree->parent_ = nullptr;
            return join_right(ltree, node, rtree);
        } else if (lh + 1 < rh) {
            assert(rtree);
            rtree->parent_ = nullptr;
            return join_left(ltree, node, rtree);
        } else {
            /*
             * Subtree heights are compatible. Just join them.
             *
             *       node
             *       /  \
             *   ltree  rtree
             *    / \    / \
             */
            node->parent_ = nullptr;
            node->left_child_ = ltree;
            if (ltree) {
                ltree->parent_ = node;
            }
            node->right_child_ = rtree;
            if (rtree) {
                rtree->parent_ = node;
            }
            repair_node(node);
            return node;
        }
    }

    /** Name of this queue. */
    const NameType name_;

    /** Root node of the tree, or "nullptr" if the queue is empty. */
    Node* tree_;

    /** Left-most node that belongs to this queue. */
    Node* first_node_;

    /** Pointer to first sub-queue that got merged into this instance. */
    ConcatenableQueue* first_subqueue_;

    /** Linked list of sub-queues that were merged to build this queue. */
    ConcatenableQueue* next_subqueue_;
};


} // namespace mwmatching

#endif  // MWMATCHING_CONCATENABLE_QUEUE_H_
