//
// Created by DELL on 10/25/2025.
//

#ifndef SCAPEGOATTREE_SCAPEGOATTREE_HPP
#define SCAPEGOATTREE_SCAPEGOATTREE_HPP

#include <sstream>
#include <string>
#include  <iostream>
#include "Node.hpp"
#include <cmath>

using namespace std;
template<typename T>
class ScapeGoatTree {
    using Node = Node<T>;
    // Root node of the tree
    static int findH(const Node* node);
    static int countN(const Node* node);
    static Node* findTraitor(Node* node);
    Node* rebuildTree(int start,int end,Node* parent_node);
    void inorderTraversal(const Node*node, int &i);
    static void postorderTraversal(const Node* node);
    void preorderTraversal(const Node* node);
    void displayPreOrder(const Node* node); // for display
    void displayInOrder(const Node* node); // for display
    void  displayPostOrder(const Node* node) ; // for display
    int getThreshold() const {return static_cast<int>(log(nNodes) / log(1.5));}

    mutable std::string displayBuffer;
    Node* root{};
    int nNodes{};
    int size=100;
    T* array = new T[size]{};

public:

    ScapeGoatTree();
    void insert(T value);
    bool deleteValue(T value);
    ~ScapeGoatTree(){postorderTraversal(root);delete[] array;}
    string isBalanced() const;
    string& getDisplayBuffer() const;
    ScapeGoatTree(const ScapeGoatTree &Otree);
    ScapeGoatTree operator+(const ScapeGoatTree &other) const;
    ScapeGoatTree& operator=(const ScapeGoatTree& other);
    [[nodiscard]] bool search(T key) const;
    void displayPreOrder(); // for display
    void displayInOrder() ; // for display
    void displayPostOrder() ; // for display
    void displayLevels(); // for display
    bool operator[](T value) const;
    bool operator==(const ScapeGoatTree &tree) const;
    bool operator!=(const ScapeGoatTree &tree) const;
    bool operator!() const;
    bool operator-(T value);
    void operator+(T value);
    bool operator-=(T value);
    void operator+=(T value);
};
#include "scapegoatTree.tpp"

#endif //SCAPEGOATTREE_SCAPEGOATTREE_HPP