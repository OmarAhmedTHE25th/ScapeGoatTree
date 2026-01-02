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
template<typename T>
ScapeGoatTree<T>::~ScapeGoatTree() {
    postorderTraversal(root);
    root = nullptr;
    nNodes = 0;
    max_nodes = 0;
}
/* Move constructor
 * Purpose:
 *  - "Steal" internal resources from `other` to perform a cheap move.
 *  - Transfer ownership of pointer data (root, array) and bookkeeping
 *    fields (nNodes, size, max_nodes, displayBuffer) and leave `other`
 *    in a valid empty state.
 */
template<typename T>
ScapeGoatTree<T>::ScapeGoatTree(ScapeGoatTree &&other) noexcept
    : displayBuffer(std::move(other.displayBuffer)),
root(other.root),
nNodes(other.nNodes),
max_nodes(other.max_nodes) {
    other.root = nullptr;
    other.nNodes = 0;
    other.max_nodes = 0;
}

// =====================
// Insert
// =====================

template <typename T>
void ScapeGoatTree<T>::restructure_subtree(Node *newNode) {
    //find the scapegoat  node
    Node* goat = findTraitor(newNode->parent);
    if (goat == nullptr) return;
    int subtree_size = countN(goat);
    T* temp_array = new T[subtree_size];

    int i = 0;
    //flatten the tree into a sorted array
    inorderTraversal(goat, i, temp_array);
    const int sub_size = i; // size of subtree
    Node* balanced = rebuildTree(0, sub_size- 1, goat->parent, temp_array);
    if (!goat->parent) root = balanced;
    else if (goat == goat->parent->left) goat->parent->left = balanced;
    else goat->parent->right = balanced;
    postorderTraversal(goat);
    delete[] temp_array;
}

template<typename T>
void ScapeGoatTree<T>::insert(T value) {
    if (!root) {
        root = new Node(value, nullptr);
        nNodes++;
        if (nNodes > max_nodes) max_nodes = nNodes;
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
    if (nNodes > max_nodes) max_nodes = nNodes;

    if (depth + 1 > getThreshold()) {
        restructure_subtree(newNode);
    }
}
template<typename T>
    void ScapeGoatTree<T>::insertBatch(Vector<T>& values) {
    for (int i = 0; i < values.size(); i++) {
        insert(values[i]);
    }
}

template<typename T>
void ScapeGoatTree<T>::deleteBatch( Vector<T>& values) {
    for (int i = 0; i < values.size(); i++) {
        deleteValue(values[i]);
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
        std::cout << "Deleting: " << value << '\n';
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
        std::cout << "Deleting: " << value << '\n';
    }

    // Case 3: Two children
    else {
        // Find inorder successor (smallest in right subtree)
        Node* suc = node->right;
        while (suc->left != nullptr)
            suc = suc->left;


        T successorValue = suc->value;
        deleteValue(successorValue);
        node->value = successorValue;


        delete suc;
        std::cout << "Deleting: " << value << '\n';
    }

    // Update node count
    nNodes--;
    if (nNodes == 0) {
        root = nullptr;
        max_nodes = 0;
    } else {
        DeletionRebuild();
    }
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
ScapeGoatTree<T>::Node* ScapeGoatTree<T>::rebuildTree(const int start, const int end, Node* parent_node,T* array) {
    if (start > end) return nullptr;
    int mid = (start + end) / 2;
    Node* Nroot = new Node(array[mid], parent_node);
    Nroot->left = rebuildTree(start, mid - 1, Nroot);
    Nroot->right = rebuildTree(mid + 1, end, Nroot);
    return Nroot;
}
template<typename T>
void ScapeGoatTree<T>::DeletionRebuild(){
        if (nNodes < 0.5 * max_nodes&& nNodes > 0) {  // α = 0.5 for deletion
            T* temp_array = new T[nNodes];
            int i = 0;
            inorderTraversal(root, i, temp_array);
           const Node* oldRoot = root;
            root = rebuildTree(0, nNodes - 1, nullptr, temp_array);
            delete[] temp_array;
            postorderTraversal(oldRoot);
            max_nodes = nNodes;
        }
        }
template<typename T>
const ScapeGoatTree<T>::Node *ScapeGoatTree<T>::getRoot() {
    return root;
}

// =====================
// Traversals
// =====================

template<typename T>
void ScapeGoatTree<T>::inorderTraversal(const Node* node, int& i, T* array) {
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
std::string ScapeGoatTree<T>::getDisplayBuffer() const {
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
        displayBuffer += "Level " + std::to_string(level++) + ": ";
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
ScapeGoatTree<T> ScapeGoatTree<T>::operator+(const ScapeGoatTree& other)const  {
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
    max_nodes = 0;
    if (other.root) preorderTraversal(other.root);
    return *this;
}

template<typename T>
ScapeGoatTree<T>& ScapeGoatTree<T>::operator=(ScapeGoatTree&& other) noexcept {
    if (this == &other) return *this;
    postorderTraversal(root);
    root = other.root;
    nNodes = other.nNodes;
    max_nodes = other.max_nodes;
    displayBuffer = std::move(other.displayBuffer);

    other.root = nullptr;
    other.nNodes = 0;
    other.max_nodes = 0;

    return *this;
}

template<typename T>
void ScapeGoatTree<T>::operator+(const T& value) { insert(value); }

template<typename T>
void ScapeGoatTree<T>::operator+=(const T& value) { insert(value); }

template<typename T>
bool ScapeGoatTree<T>::operator-=(const T& value) { return deleteValue(value); }

template<typename T>
bool ScapeGoatTree<T>::operator[](T value) const {
    return search(value);
}
template<typename T>
ScapeGoatTree<T>& ScapeGoatTree<T>::operator=(const int value) {
    if (value == 0) {
        clear();
    }
    return *this;
}
template<typename T>
bool ScapeGoatTree<T>::operator==(const ScapeGoatTree& tree) const {
    return areTreesEqual(root, tree.root);
}
template<typename T>
bool ScapeGoatTree<T>::areTreesEqual(const Node* n1, const Node* n2) const {
    // Both null = equal
    if (!n1 && !n2) return true;
    
    // One null, one not = unequal
    if (!n1 || !n2) return false;
    
    // Values differ = unequal
    if (n1->value != n2->value) return false;
    
    // Recursively check subtrees (in-order comparison)
    return areTreesEqual(n1->left, n2->left) && 
           areTreesEqual(n1->right, n2->right);
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
bool ScapeGoatTree<T>::search(const T& key) const {
    Node* current = root;
    while (current != nullptr) {
        if (key == current->value) return true;
        if (key < current->value) current = current->left;

        else if (key > current->value) current = current->right;

    }
    return false;
}

template<typename T>
void ScapeGoatTree<T>::clear() {
    postorderTraversal(root);
    root = nullptr;
    nNodes = 0;
    max_nodes = 0;
}


#endif //TREE_SCAPEGOATTREE_TPP
