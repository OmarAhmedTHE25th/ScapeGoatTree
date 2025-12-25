//
// Created by DELL on 24/12/2025.
//

#ifndef TREE_QUEUE_TPP
#define TREE_QUEUE_TPP

//
// Created by DELL on 24/12/2025.
//

#include "queue.hpp"

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

template<typename T>
void Queue<T>::pop() {
    if (!head)return;
    QNode<T>* curr = head;
    head = curr->next;
    delete curr;
    if (!head) tail = nullptr;
    nNodes--;
}

template<typename T>
bool Queue<T>::isEmpty() const {
    return !head;
}

template<typename T>
T Queue<T>::front() {
    if (!head) return T{};
    return head->value;
}

template<typename T>
int Queue<T>::size() const {
    return nNodes;
}


#endif //TREE_QUEUE_TPP