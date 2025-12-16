//
// Created by DELL on 10/25/2025.
//

#ifndef SCAPEGOATTREE_SCAPEGOATTREE_HPP
#define SCAPEGOATTREE_SCAPEGOATTREE_HPP

#include <sstream>
#include <string>

#include "Node.hpp"

class ScapeGoatTree {
    // Root node of the tree
    static int findH(const Node* node);
    static int countN(const Node* node);
    static Node* findTraitor(Node* node);
    Node* rebuildTree(int start,int end,Node* parent_node);
    void inorderTraversal(const Node *node, int &i);
    static void postorderTraversal(const Node* node);
    void preorderTraversal(const Node* node);
    void displayPreOrder(const Node* node); // for display
    void displayInOrder(const Node* node); // for display
    void  displayPostOrder(const Node* node) ; // for display
    mutable std::string displayBuffer;
    Node* root{};
    int array[100]{};
    int nNodes{};

public:

    ScapeGoatTree();
    // Inserts a new value into the tree 
    void insert(int value);
    void deleteValue(int value);
    ~ScapeGoatTree(){postorderTraversal(root);}

    std::string isBalanced() const;
    std::string& getDisplayBuffer() const;
    ScapeGoatTree(const ScapeGoatTree &Otree);
    ScapeGoatTree operator+(const ScapeGoatTree &goat_tree) const;
    ScapeGoatTree& operator=(const ScapeGoatTree& Otree);
    [[nodiscard]] bool search(int key) const;
    void displayPreOrder(); // for display
    void displayInOrder() ; // for display
    void displayPostOrder() ; // for display
    bool operator[](int value) const;
    bool operator==(const ScapeGoatTree &tree) const;
    bool operator!=(const ScapeGoatTree &tree) const;
    bool operator!() const;
    void operator-(int value);
    void operator+(int value);
    void operator-=(int value);
    void operator+=(int value);
};


#endif //SCAPEGOATTREE_SCAPEGOATTREE_HPP
