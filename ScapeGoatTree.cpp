#include "ScapeGoatTree.hpp"
#include <algorithm>
#include <iostream>
#include <cmath>
using namespace std;
// Constructor starts with an empty tree
ScapeGoatTree::ScapeGoatTree()
= default;

// This function puts the new node in the tree and sets its parent.
void ScapeGoatTree::insert(int value) {

    // tree is empty so the new node becomes the root
    if (root == nullptr) {
        root = new Node(value, nullptr);   // root has no parent
        nNodes++;
        return;
    }

    Node* current = root;   // we use this pointer to move through the tree
    Node* parent = nullptr; // will store the parent of the new node
    int  depth = 0;
    while (current != nullptr) {
        parent = current;              // Set parent to the current node before moving on

        if (value < current->value) {
            current = current->left;
            depth++;

        }
        else if (value > current->value) {
            current = current->right;
            depth++;
        }
        else {
            return;
        }
    }

    // Make the new node and set its parent
    const auto newNode = new Node(value, parent);
    int i =0;
    // connect the new node to the parent (left/right)
    if (value < parent->value) {
        parent->left = newNode;
        nNodes++;
    } else {
        parent->right = newNode;
        nNodes++;
    }
    int threshold = static_cast<int>(log(nNodes) / log(1.5));
    if (depth>threshold) {
        Node* goat = findTraitor(newNode);
        if (goat == nullptr) return;
        inorderTraversal(goat,i);
        Node* balanced = rebuildTree(0,i-1);
        balanced->parent=goat->parent;
        if (!goat->parent) root = balanced;
        else if (goat == goat->parent->left) goat->parent->left = balanced;
        else goat->parent->right = balanced;
        postorderTraversal(goat);
    }
}

int ScapeGoatTree::findH(const Node *node) {
    if (!node)return 0;
    const int rightH = findH(node->right);
    const int leftH = findH(node->left);
    return max(leftH,rightH)+1;
}
int ScapeGoatTree::countN(const Node *node) {
    if (!node)return 0;
    return 1 + countN(node->left) + countN(node->right);
}
Node *ScapeGoatTree::findTraitor(Node *node) {
    if (!node)return nullptr;
    int size = countN(node);
    int leftSize = countN(node->left);
    int rightSize =  size - leftSize - 1;
    if (leftSize > (0.6666666)*size ||rightSize > (0.6666666)*size) {
        return node;
    }
  return findTraitor(node->parent);
}
Node* ScapeGoatTree::rebuildTree(const int start, const int end) const {
    if (start>end)return  nullptr;
    int mid =(start+end)/2;
    Node* Nroot = new Node(array[mid]);
    Nroot->left = nullptr;
    Nroot->right = nullptr;
    Nroot->parent = nullptr;
    Nroot->left = rebuildTree(start, mid-1);
    Nroot->right = rebuildTree(mid+1,end);
    if (Nroot->left) Nroot->left->parent = Nroot;
    if (Nroot->right)Nroot->right->parent=Nroot;
    return Nroot;
}
void ScapeGoatTree::inorderTraversal(const Node *node,int &i) {
    if (!node)return;
    inorderTraversal(node->left,i);
    array[i++]= node->value;
    inorderTraversal(node->right,i);
}
void ScapeGoatTree::postorderTraversal(Node *node) {
    if (!node)return;
    postorderTraversal(node->left);
    postorderTraversal(node->right);
    delete node;
}





