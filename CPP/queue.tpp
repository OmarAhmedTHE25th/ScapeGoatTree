//
// Created by DELL on 24/12/2025.
//

#ifndef TREE_QUEUE_TPP
#define TREE_QUEUE_TPP

//
// Created by DELL on 24/12/2025.
//

#include "queue.hpp"

/**
 * Adds a new value to the back of the queue.
 */
template<typename T>
void Queue<T>::push(T value) {
    if (!head) {
        auto newNode = new QNode<T>(value);
        head = newNode;
        tail = head;
        nNodes++;
        return;
    }
    QNode<T>* last = tail;
    auto newNode = new QNode<T>(value);
   last->next = newNode;
    tail = newNode;
    nNodes++;
}

/**
 * Removes the front element from the queue.
 */
template<typename T>
void Queue<T>::pop() {
    if (!head)return;
    QNode<T>* curr = head;
    head = curr->next;
    delete curr;
    if (!head) tail = nullptr;
    nNodes--;
}

/**
 * Checks if the queue is empty.
 */
template<typename T>
bool Queue<T>::isEmpty() const {
    return !head;
}

/**
 * Returns the value of the front element without removing it.
 */
template<typename T>
T Queue<T>::front() {
    if (!head) return T{};
    return head->value;
}

/**
 * Returns the current number of elements in the queue.
 */
template<typename T>
int Queue<T>::size() const {
    return nNodes;
}


#endif //TREE_QUEUE_TPP