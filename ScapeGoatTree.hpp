//
// Created by DELL on 10/25/2025.
//

#ifndef SCAPEGOATTREE_SCAPEGOATTREE_HPP
#define SCAPEGOATTREE_SCAPEGOATTREE_HPP

#include <sstream>
#include <string>

#include "Node.hpp"
template<typename T>
class ScapeGoatTree {
    using Node = ::Node<T>;
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
    mutable std::string displayBuffer;
    Node* root{};
    int nNodes{};
    int threshold = static_cast<int>(log(nNodes) / log(1.5));
    int size=100;
    T* array = new int[size]{};

public:

    ScapeGoatTree();
    void insert(T value);
    void deleteValue(T value);
    ~ScapeGoatTree(){postorderTraversal(root);delete[] array;}
    std::string isBalanced() const;
    std::string& getDisplayBuffer() const;
    ScapeGoatTree(const ScapeGoatTree &Otree);
    ScapeGoatTree operator+(const ScapeGoatTree &other) const;
    ScapeGoatTree& operator=(const ScapeGoatTree& other);
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
