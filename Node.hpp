//
// Created by DELL on 10/25/2025.
//

#ifndef SCAPEGOATTREE_NODE_HPP
#define SCAPEGOATTREE_NODE_HPP


class Node {
public:
    int value;     // stored value
    Node* left;    // left child pointer
    Node* right;   // right child pointer
    Node* parent;  // parent pointer

    // constructor
    Node(int v, Node* parentPtr = nullptr)
        : value(v), left(nullptr), right(nullptr), parent(parentPtr) {}
};


#endif //SCAPEGOATTREE_NODE_HPP
