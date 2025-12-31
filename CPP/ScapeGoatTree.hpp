//
// Created by DELL on 10/25/2025.
//
/**
 * ScapeGoat Tree Implementation
 *
 * A self-balancing BST that maintains balance through periodic rebuilding.
 *
 * Key Properties:
 * - α-weight-balanced: No node's subtree is heavier than α × parent's subtree
 * - α = 2/3 for this implementation
 * - Height bound: h ≤ log₁.₅(n) where n = number of nodes
 *
 * Time Complexity:
 * - Insert: O(log n) amortized, O(n) worst case during rebuild
 * - Delete: O(log n) amortized, O(n) worst case during rebuild
 * - Search: O(log n) worst case (tree stays balanced)
 *
 * Space Complexity: O(n) for tree + O(n) temporary array during rebuild
 */
#ifndef SCAPEGOATTREE_SCAPEGOATTREE_HPP
#define SCAPEGOATTREE_SCAPEGOATTREE_HPP

#include <sstream>
#include <string>
#include  <iostream>
#include "Node.hpp"
#include <cmath>
#include "vector.hpp"


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
    void DeletionRebuild();
    bool areTreesEqual(const Node* n1, const Node* n2) const;
    mutable std::string displayBuffer;
    Node* root{};
    int nNodes{};
    int size=100;
    T* array = new T[size]{};
    int max_nodes = 0;
public:

    ScapeGoatTree();
    void insert(T value);
    void insertBatch(const Vector<T> &values);
    bool deleteValue(T value);
    void deleteBatch(const Vector<T> &values);
    ~ScapeGoatTree(){postorderTraversal(root);delete[] array;root = nullptr;
    nNodes = 0;
    max_nodes = 0;}
    void clear() {postorderTraversal(root);delete[] array;root = nullptr;
    nNodes = 0;
    max_nodes = 0;}
    std::string isBalanced() const;
    std::string& getDisplayBuffer() const;
    const Node * getRoot();
    ScapeGoatTree(const ScapeGoatTree &Otree);
    ScapeGoatTree(ScapeGoatTree&& other) noexcept;
    ScapeGoatTree operator+(const ScapeGoatTree &other) const;
    ScapeGoatTree& operator=(const ScapeGoatTree& other);
    ScapeGoatTree& operator=(ScapeGoatTree&& other) noexcept;
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
