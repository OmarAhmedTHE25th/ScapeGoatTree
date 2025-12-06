#include "ScapeGoatTree.hpp"

// Constructor starts with an empty tree
ScapeGoatTree::ScapeGoatTree()
    : root(nullptr) {}

// get the root node
Node* ScapeGoatTree::getRoot() const {
    return root;
}

// This function puts the new node in the tree and sets its parent.
void ScapeGoatTree::insert(int value) {

    // tree is empty so the new node becomes the root
    if (root == nullptr) {
        root = new Node(value, nullptr);   // root has no parent
        return;
    }

    Node* current = root;   // we use this pointer to move through the tree
    Node* parent = nullptr; // will store the parent of the new node

    while (current != nullptr) {
        parent = current;              // Set parent to the current node before moving on

        if (value < current->value) {
            current = current->left;  
        }
        else if (value > current->value) {
            current = current->right;  
        }
        else {
            return;
        }
    }

    // Make the new node and set its parent
    Node* newNode = new Node(value, parent);

    // connect the new node to the parent (left/right)
    if (value < parent->value) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
}
