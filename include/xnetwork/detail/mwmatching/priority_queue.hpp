/**
 * Priority queue data structure.
 */

#ifndef MWMATCHING_PRIORITY_QUEUE_H_
#define MWMATCHING_PRIORITY_QUEUE_H_

#include <cassert>
#include <limits>
#include <vector>


namespace mwmatching {


/**
 * Min-priority queue implemented as a binary heap.
 *
 * Elements in a heap have a priority and associated "data".
 *
 * The following operations can be done efficiently:
 *  - Insert an element into the queue.
 *  - Remove an element from the queue.
 *  - Change the priority of a given element.
 *  - Find the element with lowest priority in the queue.
 */
template <typename PrioType,
          typename DataType>
class PriorityQueue
{
public:
    typedef unsigned int IndexType;
    static constexpr IndexType INVALID_INDEX =
        std::numeric_limits<IndexType>::max();

    /**
     * A Node instance represents an element in a PriorityQueue.
     *
     * A Node instance must remain valid while it is contained in a queue.
     * The containing queue holds a pointer to the Node.
     *
     * A Node instance may be destructed if it is not contained in any queue.
     * Alternatively, a Node instance may be destructed after its containing
     * queue instance has been destructed.
     */
    class Node
    {
      public:
        /** Construct an invalid node, not contained in any queue. */
        Node()
          : index_(INVALID_INDEX)
        { }

        // Prevent copying.
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;

        /** Return true if this node is contained in a queue. */
        bool valid() const
        {
            return (index_ != INVALID_INDEX);
        }

        /**
         * Return the priority of this node in the queue.
         *
         * The node must be contained in a queue.
         * This function takes time O(1).
         */
        PrioType prio() const
        {
            assert(index_ != INVALID_INDEX);
            return prio_;
        }

      private:
        IndexType       index_;
        PrioType        prio_;
        DataType        data_;

        friend class PriorityQueue;
    };

    /** Construct an empty queue. */
    PriorityQueue()
    { }

    // Prevent copying.
    PriorityQueue(const PriorityQueue&) = delete;
    PriorityQueue& operator=(const PriorityQueue&) = delete;

    /**
     * Remove all elements from the queue.
     *
     * This function takes time O(n).
     */
    void clear()
    {
        for (Node* node : heap_) {
            node->index_ = INVALID_INDEX;
        }
        heap_.clear();
    }

    /** Return true if the queue is empty. */
    bool empty() const
    {
        return heap_.empty();
    }

    /**
     * Return the minimum priority of any element in the queue.
     *
     * The queue must be non-empty.
     * This function takes time O(1).
     */
    PrioType min_prio() const
    {
        assert(! heap_.empty());
        Node* top = heap_.front();
        return top->prio_;
    }

    /**
     * Return the element with minimum priority.
     *
     * The queue must be non-empty.
     * This function takes time O(1).
     */
    DataType min_elem() const
    {
        assert(! heap_.empty());
        Node* top = heap_.front();
        return top->data_;
    }

    /**
     * Insert the given node into the queue with associated data.
     *
     * The node must not currently be contained in any queue.
     * This function takes amortized time O(log(n)).
     */
    void insert(Node* node, PrioType prio, const DataType& data)
    {
        assert(node->index_ == INVALID_INDEX);

        node->index_ = heap_.size();
        node->prio_ = prio;
        node->data_ = data;

        heap_.push_back(node);
        sift_up(node->index_);
    }

    /**
     * Update priority of an existing node.
     *
     * This function takes time O(log(n)).
     */
    void set_prio(Node* node, PrioType prio)
    {
        IndexType index = node->index_;
        assert(index != INVALID_INDEX);
        assert(heap_[index] == node);

        PrioType prev_prio = node->prio_;
        node->prio_ = prio;

        if (prio < prev_prio) {
            sift_up(index);
        } else if (prio > prev_prio) {
            sift_down(index);
        }
    }

    /**
     * Remove the specified element from the queue.
     *
     * This function takes time O(log(n)).
     */
    void remove(Node* node)
    {
        IndexType index = node->index_;
        assert(index != INVALID_INDEX);
        assert(heap_[index] == node);

        node->index_ = INVALID_INDEX;

        Node* move_node = heap_.back();
        heap_.pop_back();

        if (index < heap_.size()) {
            heap_[index] = move_node;
            move_node->index_ = index;
            if (move_node->prio_ < node->prio_) {
                sift_up(index);
            } else if (move_node->prio_ > node->prio_) {
                sift_down(index);
            }
        }
    }

private:
    /** Repair the heap along an ascending path to the root. */
    void sift_up(IndexType index)
    {
        Node* node = heap_[index];
        PrioType prio = node->prio_;

        while (index > 0) {
            IndexType next_index = (index - 1) / 2;
            Node* next_node = heap_[next_index];
            if (next_node->prio_ <= prio) {
                break;
            }
            heap_[index] = next_node;
            next_node->index_ = index;
            index = next_index;
        }

        node->index_ = index;
        heap_[index] = node;
    }

    /** Repair the heap along a descending path. */
    void sift_down(IndexType index)
    {
        Node* node = heap_[index];
        PrioType prio = node->prio_;

        IndexType num_elem = heap_.size();

        while (index < num_elem / 2) {
            IndexType next_index = 2 * index + 1;
            Node* next_node = heap_[next_index];

            if (next_index + 1 < num_elem) {
                Node* tmp_node = heap_[next_index + 1];
                if (tmp_node->prio_ <= next_node->prio_) {
                    ++next_index;
                    next_node = tmp_node;
                }
            }

            if (next_node->prio_ >= prio) {
                break;
            }

            heap_[index] = next_node;
            next_node->index_ = index;

            index = next_index;
        }

        heap_[index] = node;
        node->index_ = index;
    }

    /** Heap data structure. */
    std::vector<Node*> heap_;
};


} // namespace mwmatching

#endif  // MWMATCHING_PRIORITY_QUEUE_H_
