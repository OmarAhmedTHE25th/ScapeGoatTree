//
// Created by DELL on 24/12/2025.
//

#ifndef TREE_SCAPEGOATTREE_TPP
#define TREE_SCAPEGOATTREE_TPP
#include "ScapeGoatTree.hpp"
#include "queue.hpp"
//==================================IMPLEMENTATION========================================================

// =====================
// Constructors
// =====================

template<typename T>
ScapeGoatTree<T>::ScapeGoatTree() = default;

template<typename T>
ScapeGoatTree<T>::ScapeGoatTree(const ScapeGoatTree &Otree) {
    if (!Otree.root) return;
    preorderTraversal(Otree.root);
}

// =====================
// Insert
// =====================

template<typename T>
void ScapeGoatTree<T>::insert(T value) {
    if (!root) {
        root = new Node(value, nullptr);
        nNodes++;
        return;
    }

    Node* current = root;
    Node* parent = nullptr;
    int depth = 0;

    while (current) {
        parent = current;
        depth++;
        if (value < current->value)
            current = current->left;
        else if (value > current->value)
            current = current->right;
        else
            return;
    }

    Node* newNode = new Node(value, parent);
    if (value < parent->value)
        parent->left = newNode;
    else
        parent->right = newNode;

    nNodes++;

    if (nNodes >= size) {
        size *= 2;
        T* newArr = new T[size]{};
        for (int i = 0; i < nNodes; ++i)
            newArr[i] = array[i];
        delete[] array;
        array = newArr;
    }

    if (depth + 1 > getThreshold()) {
        Node* goat = findTraitor(newNode->parent);
        if (goat == nullptr) return;
        int i = 0;
        inorderTraversal(goat, i);
        const int sub_size = i; // size of subtree
        Node* balanced = rebuildTree(0, sub_size- 1, goat->parent);
        if (!goat->parent) root = balanced;
        else if (goat == goat->parent->left) goat->parent->left = balanced;
        else goat->parent->right = balanced;
        postorderTraversal(goat);
    }
}

// =====================
// Delete
// =====================

template<typename T>
bool ScapeGoatTree<T>::deleteValue(T value) {
    Node* node = root;
    Node* parent = nullptr;

    // Step 1: Search for the node
    while (node != nullptr && node->value != value) {
        parent = node;
        if (value < node->value)
            node = node->left;
        else
            node = node->right;
    }

    // Value not found
    if (!node) return false;

    // Deletion cases

    // Case 1: Leaf node
    if (!node->left && !node->right) {
        if (!parent) {
            root = nullptr;
        }
        else if (parent->left == node) {
            parent->left = nullptr;
        }
        else {
            parent->right = nullptr;
        }
        delete node;

        if (nNodes < getThreshold()) {
            int i = 0;
            inorderTraversal(root, i);
            root = rebuildTree(0, nNodes, root);
        }
    }

    // Case 2: One child
    else if (!node->left || !node->right) {
        Node* child = nullptr;

        if (node->left != nullptr)
            child = node->left;
        else
            child = node->right;

        child->parent = parent;

        if (!parent) {
            root = child;
        }
        else if (parent->left == node) {
            parent->left = child;
        }
        else {
            parent->right = child;
        }

        delete node;

        if (nNodes < getThreshold()) {
            int i = 0;
            inorderTraversal(root, i);
            root = rebuildTree(0, nNodes, root);
        }
    }

    // Case 3: Two children
    else {
        // Find inorder successor (smallest in right subtree)
        Node* suc = node->right;
        while (suc->left != nullptr)
            suc = suc->left;

        const T sucValue = suc->value;

        // Delete successor
        deleteValue(sucValue);

        // Replace value
        node->value = sucValue;

        if (nNodes < getThreshold()) {
            int i = 0;
            inorderTraversal(root, i);
            root = rebuildTree(0, nNodes, root);
        }

    }

    // Update node count
    nNodes--;
    return true;
}

// =====================
// Utility / Static helpers
// =====================

template<typename T>
int ScapeGoatTree<T>::findH(const Node *node) {
    if (!node) return -1;
    const int rightH = findH(node->right);
    const int leftH = findH(node->left);
    const int maximum = (leftH > rightH) ? leftH : rightH;
    return maximum + 1;
}

template<typename T>
int ScapeGoatTree<T>::countN(const Node *node) {
    if (!node) return 0;
    return 1 + countN(node->left) + countN(node->right);
}

template<typename T>
 ScapeGoatTree<T>::Node* ScapeGoatTree<T>::findTraitor(Node *node) {
    while (node != nullptr) {
        const int left = countN(node->left);
        const int right = countN(node->right);
        int size = left + right + 1;

        if (left > (2.0 / 3.0) * size || right > (2.0 / 3.0) * size)
            return node;

        node = node->parent;
    }
    return nullptr;
}

template<typename T>
 ScapeGoatTree<T>::Node* ScapeGoatTree<T>::rebuildTree(const int start, const int end, Node* parent_node) {
    if (start > end) return nullptr;
    int mid = (start + end) / 2;
    Node* Nroot = new Node(array[mid], parent_node);
    Nroot->left = rebuildTree(start, mid - 1, Nroot);
    Nroot->right = rebuildTree(mid + 1, end, Nroot);
    if (Nroot->left) Nroot->left->parent = Nroot;
    if (Nroot->right) Nroot->right->parent = Nroot;
    return Nroot;
}

// =====================
// Traversals
// =====================

template<typename T>
void ScapeGoatTree<T>::inorderTraversal(const Node* node, int& i) {
    if (!node) return;
    inorderTraversal(node->left, i);
    array[i++] = node->value;
    inorderTraversal(node->right, i);
}

template<typename T>
void ScapeGoatTree<T>::postorderTraversal(const Node* node) {
    if (!node) return;
    postorderTraversal(node->left);
    postorderTraversal(node->right);
    delete node;
}

template<typename T>
void ScapeGoatTree<T>::preorderTraversal(const Node* node) {
    if (!node) return;
    insert(node->value);
    preorderTraversal(node->left);
    preorderTraversal(node->right);
}

// =====================
// Display (internal)
// =====================1

template<typename T>
void ScapeGoatTree<T>::displayPreOrder(const Node* node) {
    if (!node) return;
    std::ostringstream value;
    value << node->value;
    displayBuffer += value.str() + " ";
    displayPreOrder(node->left);
    displayPreOrder(node->right);
}

template<typename T>
void ScapeGoatTree<T>::displayInOrder(const Node* node) {
    if (!node) return;
    displayInOrder(node->left);
    std::ostringstream value;
    value << node->value;
    displayBuffer += value.str() + " ";
    displayInOrder(node->right);
}

template<typename T>
void ScapeGoatTree<T>::displayPostOrder(const Node* node) {
    if (!node) return;
    displayPostOrder(node->left);
    displayPostOrder(node->right);
    std::ostringstream value;
    value << node->value;
    displayBuffer += value.str() + " ";
}

// =====================
// Display (public)
// =====================

template<typename T>
void ScapeGoatTree<T>::displayPreOrder() {
    displayBuffer.clear();
    if (!root) { displayBuffer = "Tree is empty."; return; }
    displayPreOrder(root);
}

template<typename T>
void ScapeGoatTree<T>::displayInOrder() {
    displayBuffer.clear();
    if (!root) { displayBuffer = "Tree is empty."; return; }
    displayInOrder(root);
}

template<typename T>
void ScapeGoatTree<T>::displayPostOrder() {
    displayBuffer.clear();
    if (!root) { displayBuffer = "Tree is empty."; return; }
    displayPostOrder(root);
}

template<typename T>
std::string& ScapeGoatTree<T>::getDisplayBuffer() const {
    return displayBuffer;
}

template<typename T>
void ScapeGoatTree<T>::displayLevels() {
    displayBuffer.clear();
    if (!root){displayBuffer = "Tree is Empty.";return;}
    Queue<Node*> q;
    q.push(root);
    int level =0;

    while (!q.isEmpty()) {
        displayBuffer += "Level " + to_string(level++) + ": ";
        const int nodesAtLevel = q.size();
        for (int i = 0; i < nodesAtLevel; i++) {
            Node* curr = q.front();
            q.pop();

            std::ostringstream oss;
            oss << curr->value;
            displayBuffer += oss.str() + " ";

            if (curr->left)  q.push(curr->left);
            if (curr->right) q.push(curr->right);
        }
        displayBuffer += "\n";

    }
}

// =====================
// Operators
// =====================

template<typename T>
ScapeGoatTree<T> ScapeGoatTree<T>::operator+(const ScapeGoatTree& other) const {
    ScapeGoatTree result;
    result.preorderTraversal(root);
    result.preorderTraversal(other.root);
    return result;
}

template<typename T>
ScapeGoatTree<T>& ScapeGoatTree<T>::operator=(const ScapeGoatTree& other) {
    if (this == &other) return *this;
    postorderTraversal(root);
    root = nullptr;
    nNodes = 0;
    if (other.root) preorderTraversal(other.root);
    delete[] array;
    return *this;
}

template<typename T>
void ScapeGoatTree<T>::operator+(T value) { insert(value); }

template<typename T>
bool ScapeGoatTree<T>::operator-(T value) {return  deleteValue(value); }

template<typename T>
void ScapeGoatTree<T>::operator+=(T value) { insert(value); }

template<typename T>
bool ScapeGoatTree<T>::operator-=(T value) { return deleteValue(value); }

template<typename T>
bool ScapeGoatTree<T>::operator[](T value) const {
    return search(value);
}

template<typename T>
bool ScapeGoatTree<T>::operator==(const ScapeGoatTree& tree) const {
    if (nNodes != tree.nNodes) return false;
    for (int i = 0; i < nNodes; ++i)
        if (array[i] != tree.array[i]) return false;
    return true;
}

template<typename T>
bool ScapeGoatTree<T>::operator!=(const ScapeGoatTree& tree) const {
    return !(*this == tree);
}

template<typename T>
bool ScapeGoatTree<T>::operator!() const {
    return root == nullptr;
}

// =====================
// Balance / Search
// =====================

template<typename T>
std::string ScapeGoatTree<T>::isBalanced() const {
    std::ostringstream out;

    double n = countN(root);
    if (n == 0) {
        out << "Tree empty. Of course it's balanced ";
        return out.str();
    }

    int height = findH(root);
    double bound = log(n) / log(1.5);


    out << "Node count: " << n << "\n";
    out << "Height: " << height << "\n";
    out << "Height bound: " << bound << "\n\n";

    if (height<= bound)
        out << "^_____^ Tree is balanced -- congratulations. It's not a Linked List.\n";
    else
        out << "~_~ Tree is NOT balanced. A scapegoat must be sacrificed.\n";

    return out.str();
}

template<typename T>
bool ScapeGoatTree<T>::search(const T key) const {
    Node* current = root;
    while (current != nullptr) {
        if (key == current->value) return true;
        if (key < current->value) {
            current = current->left;
        }
        else if (key > current->value) {
            current = current->right;
        }
    }
    return false;
}





#endif //TREE_SCAPEGOATTREE_TPP