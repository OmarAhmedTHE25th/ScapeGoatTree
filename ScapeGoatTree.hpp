//
// Created by DELL on 10/25/2025.
//

#ifndef SCAPEGOATTREE_SCAPEGOATTREE_HPP
#define SCAPEGOATTREE_SCAPEGOATTREE_HPP

#include "Node.hpp"

class ScapeGoatTree {
private:
    Node* root{};   // Root node of the tree

public:
    ScapeGoatTree();        
    Node* getRoot() const;  // get the root pointer

    // Inserts a new value into the tree 
    void insert(int value);
};


#endif //SCAPEGOATTREE_SCAPEGOATTREE_HPP
