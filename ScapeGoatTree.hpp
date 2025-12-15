//
// Created by DELL on 10/25/2025.
//

#ifndef SCAPEGOATTREE_SCAPEGOATTREE_HPP
#define SCAPEGOATTREE_SCAPEGOATTREE_HPP

#include "Node.hpp"

class ScapeGoatTree {
    // Root node of the tree
    static int findH(const Node* node);
    static int countN(const Node* node);
    static Node* findTraitor(Node* node);
    Node* rebuildTree(int start,int end,Node* parent_node);
    void inorderTraversal(const Node *node, int &i);
    static void postorderTraversal(Node* node);

    Node* root{};
    int array[100]{};
    int nNodes{};


public:
    ScapeGoatTree();
    // Inserts a new value into the tree 
    void insert(int value);
    void ScapeGoatTree::deleteValue(int value)
    ~ScapeGoatTree(){postorderTraversal(root);}

    void isBalanced() const;

};


#endif //SCAPEGOATTREE_SCAPEGOATTREE_HPP
