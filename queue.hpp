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
public:
    void push(T value);
    void pop();
    [[nodiscard]] bool isEmpty() const;
    T front();

};

#include "queue.tpp"
#endif //TREE_QUEUE_HPP