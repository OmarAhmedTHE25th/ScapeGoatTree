//
// Created by DELL on 10/25/2025.
//

#ifndef SCAPEGOATTREE_NODE_HPP
#define SCAPEGOATTREE_NODE_HPP
template<typename T>

class Node {
public:
    T value{};     // stored value
    Node* left{};    // left child pointer
    Node* right{};   // right child pointer
    Node* parent{};  // parent pointer
    unsigned int size=1;      // subtree size

    /**
     * Initializes a node with a value and an optional parent pointer.
     */
    explicit Node(const T& v, Node* parentPtr = nullptr)
    : value(v), parent(parentPtr){}
    template<typename>
    friend class ScapeGoatTree;
};

#endif //SCAPEGOATTREE_NODE_HPP
