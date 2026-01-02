//
// Created by DELL on 24/12/2025.
//

#ifndef TREE_QUEUE_HPP
#define TREE_QUEUE_HPP

template<typename T>
class QNode {
    QNode* next{};
    T value{};
    explicit QNode(T value): value(value){}
    template<typename>
    friend class Queue;
};
template<typename T>
class Queue {
    QNode<T>* head{};
    QNode<T>* tail{};
    int nNodes{};
public:
    /**
     * Destroys the queue and releases memory by popping all elements.
     */
    ~Queue() {  
        while (!isEmpty()) pop();
    }

    /**
     * Adds a new value to the back of the queue.
     */
    void push(T value);

    /**
     * Removes the front element from the queue.
     */
    void pop();

    /**
     * Checks if the queue is empty.
     */
    [[nodiscard]] bool isEmpty() const;

    /**
     * Returns the value of the front element without removing it.
     */
    T front();

    /**
     * Returns the current number of elements in the queue.
     */
    [[nodiscard]] int size() const;

};

#include "queue.tpp"

#endif //TREE_QUEUE_HPP
