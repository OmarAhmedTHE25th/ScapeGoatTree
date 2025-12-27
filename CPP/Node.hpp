//
// Created by DELL on 10/25/2025.
//

#ifndef SCAPEGOATTREE_NODE_HPP
#define SCAPEGOATTREE_NODE_HPP
template<typename T>

class Node {
    mutable T value;     // stored value
    Node* left;    // left child pointer
    Node* right;   // right child pointer
    Node* parent;  // parent pointer
    // constructor
    explicit Node(T& v, Node* parentPtr = nullptr)
        : value(v), left(nullptr), right(nullptr), parent(parentPtr) {}
    template<typename>
    friend class ScapeGoatTree;
};


#endif //SCAPEGOATTREE_NODE_HPP