//
// Created by DELL on 24/12/2025.
//

#ifndef TREE_SCAPEGOATTREE_TPP
#define TREE_SCAPEGOATTREE_TPP
#include "queue.hpp"
#include "sstream"
//==================================IMPLEMENTATION========================================================
// =====================
// Constructors
// =====================

/**
 * Default constructor for an empty Scapegoat Tree.
 */
template<typename T>
ScapeGoatTree<T>::ScapeGoatTree() = default;

/**
 * Copy constructor for deep copying another ScapeGoatTree.
 */
template<typename T>
ScapeGoatTree<T>::ScapeGoatTree(const ScapeGoatTree &Otree) {
    if (!Otree.root) return;
    preorderTraversal(Otree.root);
}
/**
 * Destructor that cleans up all nodes in the tree.
 */
template<typename T>
ScapeGoatTree<T>::~ScapeGoatTree() {
    postorderTraversal(root);
    max_nodes = 0;
}
/**
 * Move constructor for transferring ownership from another ScapeGoatTree.
 */
template<typename T>
ScapeGoatTree<T>::ScapeGoatTree(ScapeGoatTree &&other) noexcept
    : root(other.root),
nNodes(other.nNodes),
max_nodes(other.max_nodes) {
    other.root = nullptr;
    other.nNodes = 0;
    other.max_nodes = 0;
}

// =====================
// Insert
// =====================

/**
 * Initiates a subtree rebuild starting from the scapegoat node.
 */
template <typename T>
void ScapeGoatTree<T>::restructure_subtree(Node *newNode) {
    //find the scapegoat  node
    Node* goat = findTraitor(newNode->parent);
    if (goat == nullptr) return;
    const int subtree_size = countN(goat);
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

/**
 * Inserts a new value into the tree and maintains balance if needed.
 */
template<typename T>
void ScapeGoatTree<T>::insert(T value) {
    if (!isUndoing) {
        undoStack.push({OpType::Insert, value});
    }
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
        ++current->size;
        depth++;
        if (value < current->value)
            current = current->left;
        else if (value > current->value)
            current = current->right;
        else {
            // value already exists, backtrack size increment
            current = root;
            while (current != parent) {
                --current->size;
                if (value < current->value) current = current->left;
                else current = current->right;
            }
            --parent->size;
            if (!isUndoing) {
                undoStack.pop();
            }
            return;
        }
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
/**
 * Inserts multiple values from a Vector into the tree.
 */
template<typename T>
    void ScapeGoatTree<T>::insertBatch(const Vector<T>& values) {
    if (!isUndoing) undoStack.push({OpType::BatchStart, T()});
    for (int i = 0; i < values.size(); i++) {
        insert(values[i]);
    }
    if (!isUndoing) undoStack.push({OpType::BatchEnd, T()});
}

/**
 * Removes multiple values from a Vector from the tree.
 */
template<typename T>
void ScapeGoatTree<T>::deleteBatch(const  Vector<T>& values) {
    if (!isUndoing) undoStack.push({OpType::BatchStart, T()});
    for (int i = 0; i < values.size(); i++) {
        deleteValue(values[i]);
    }
    if (!isUndoing) undoStack.push({OpType::BatchEnd, T()});
}


// =====================
// Delete
// =====================

/**
 * Removes a value from the tree and maintains balance if needed.
 */
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

    if (!isUndoing) {
        undoStack.push({OpType::Delete, value});
    }

    // Case 1 & 2: Leaf or one child
    // Decrement size for all nodes on the path
    Node* temp = root;
    while (temp != node) {
        --temp->size;
        if (value < temp->value) temp = temp->left;
        else temp = temp->right;
    }

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
    }

    // Case 2: One child
    else if(node->left && !node->right || !node->left && node->right) {
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
    }
    // Case 3: Two children
    else if (node->left && node->right) {
        // Find inorder successor (smallest in right subtree)
        Node* suc = node->right;
        while (suc->left != nullptr)
            suc = suc->left;

        T successorValue = suc->value;
        if (!isUndoing) {
            undoStack.pop();
        }
        deleteValue(successorValue);
        node->value = successorValue;
        return true;
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

/**
 * Calculates the height of a given node in the tree.
 */
template<typename T>
int ScapeGoatTree<T>::findH(const Node *node) {
    if (!node) return -1;
    const int rightH = findH(node->right);
    const int leftH = findH(node->left);
    const int maximum = (leftH > rightH) ? leftH : rightH;
    return maximum + 1;
}

/**
 * Counts the total number of nodes in the subtree rooted at the given node.
 */
template<typename T>
int ScapeGoatTree<T>::countN(const Node *node) {
    if (!node) return 0;
    return node->size;
}

/**
 * Finds the highest node that violates the alpha-weight-balance property.
 */
template<typename T>
ScapeGoatTree<T>::Node* ScapeGoatTree<T>::findTraitor(Node *node) {
    while (node != nullptr) {
        const int left = countN(node->left);
        const int right = countN(node->right);
        int size = node->size;

        if (left > (2.0 / 3.0) * size || right > (2.0 / 3.0) * size)
            return node;

        node = node->parent;
    }
    return nullptr;
}


/**
 * Recursively rebuilds a balanced BST from a sorted array of values.
 */
template<typename T>
ScapeGoatTree<T>::Node* ScapeGoatTree<T>::rebuildTree(const int start, const int end, Node* parent_node,T* array) {
    if (start > end) return nullptr;
    int mid = (start + end) / 2;
    Node* Nroot = new Node(array[mid], parent_node);
    Nroot->left = rebuildTree(start, mid - 1, Nroot, array);
    Nroot->right = rebuildTree(mid + 1, end, Nroot, array);
    Nroot->size = 1 + countN(Nroot->left) + countN(Nroot->right);
    return Nroot;
}
/**
 * Checks if a rebuild is needed after a deletion and performs it if necessary.
 */
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
/**
 * Returns a pointer to the root node of the tree.
 */
template<typename T>
const ScapeGoatTree<T>::Node *ScapeGoatTree<T>::getRoot() {
    return root;
}

// =====================
// Traversals
// =====================

/**
 * Performs an in-order traversal to populate a sorted array with node values.
 */
template<typename T>
void ScapeGoatTree<T>::inorderTraversal(const Node* node, int& i, T* array) const {
if (!node) return;
    inorderTraversal(node->left, i,array);
    array[i++] = node->value;
    inorderTraversal(node->right, i,array);
}

/**
 * Recursively deletes all nodes in the subtree using post-order traversal.
 */
template<typename T>
void ScapeGoatTree<T>::postorderTraversal(const Node* node) {
    if (!node) return;
    postorderTraversal(node->left);
    postorderTraversal(node->right);
    delete node;
}

/**
 * Performs a pre-order traversal for internal processing.
 */
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

/**
 * Formats the tree in pre-order.
 */
template<typename T>
void ScapeGoatTree<T>::displayPreOrder(const Node* node, std::ostream& os) {
    if (!node) return;
    os << node->value << " ";
    displayPreOrder(node->left, os);
    displayPreOrder(node->right, os);
}

/**
 * Formats the tree in in-order.
 */
template<typename T>
void ScapeGoatTree<T>::displayInOrder(const Node* node, std::ostream& os) {
    if (!node) return;
    displayInOrder(node->left, os);
    os << node->value << " ";
    displayInOrder(node->right, os);
}

/**
 * Formats the tree in post-order.
 */
template<typename T>
void ScapeGoatTree<T>::displayPostOrder(const Node* node, std::ostream& os) {
    if (!node) return;
    displayPostOrder(node->left, os);
    displayPostOrder(node->right, os);
    os << node->value << " ";
}

// =====================
// Display (public)
// =====================

/**
 * Returns a string representing the tree in pre-order traversal.
 */
template<typename T>
std::string ScapeGoatTree<T>::displayPreOrder() {
    if (!root) return "Tree is empty.";
    std::ostringstream oss;
    displayPreOrder(root, oss);
    return oss.str();
}

/**
 * Returns a string representing the tree in in-order traversal.
 */
template<typename T>
std::string ScapeGoatTree<T>::displayInOrder() {
    if (!root) return "Tree is empty.";
    std::ostringstream oss;
    displayInOrder(root, oss);
    return oss.str();
}

/**
 * Returns a string representing the tree in post-order traversal.
 */
template<typename T>
std::string ScapeGoatTree<T>::displayPostOrder() {
    if (!root) return "Tree is empty.";
    std::ostringstream oss;
    displayPostOrder(root, oss);
    return oss.str();
}

/**
 * Returns a string representing the tree in level-order traversal.
 */
template<typename T>
std::string ScapeGoatTree<T>::displayLevels() {
    if (!root) return "Tree is Empty.";
    std::string result;
    Queue<Node*> q;
    q.push(root);
    int level =0;

    while (!q.isEmpty()) {
        result += "Level " + std::to_string(level++) + ": ";
        const int nodesAtLevel = q.size();
        for (int i = 0; i < nodesAtLevel; i++) {
            Node* curr = q.front();
            q.pop();

            std::ostringstream oss;
            oss << curr->value;
            result += oss.str() + " ";

            if (curr->left)  q.push(curr->left);
            if (curr->right) q.push(curr->right);
        }
        result += "\n";
    }
    return result;
}



// =====================
// Operators
// =====================

/**
 * Creates a new tree containing elements from both trees using linear merge.
 */
template<typename T>
ScapeGoatTree<T> ScapeGoatTree<T>::operator+(const ScapeGoatTree& other)const  {
    ScapeGoatTree result;
    T* array = new T[nNodes];
    T* other_array = new T[other.nNodes];
    int i = 0;
    inorderTraversal(root, i, array);
    int i2 = 0;
    other.inorderTraversal(other.root, i2, other_array);
    T* temp_array = new T[nNodes + other.nNodes];
    int idx = 0, idx1 = 0, idx2 = 0;

    // Linear Merge (O(N+M)) to keep the array sorted
    while (idx1 < nNodes && idx2 < other.nNodes) {
        if (array[idx1] < other_array[idx2]) temp_array[idx++] = array[idx1++];
        else if (array[idx1] > other_array[idx2]) temp_array[idx++] = other_array[idx2++];
        else { // duplicates
            temp_array[idx++] = array[idx1++];
            idx2++;
        }
    }
    while (idx1 < nNodes) temp_array[idx++] = array[idx1++];
    while (idx2 < other.nNodes) temp_array[idx++] = other_array[idx2++];

    //Rebuild  (O(N+M))
    result.root = result.rebuildTree(0, idx - 1, nullptr, temp_array);
    result.nNodes = idx;
    result.max_nodes = idx;
    delete[] temp_array;
    delete[] array;
    delete[] other_array;
    return result;
}

/**
 * Assignment operator for deep copying another tree.
 */
template<typename T>
ScapeGoatTree<T>& ScapeGoatTree<T>::operator=(const ScapeGoatTree& other) {
    if (this == &other) return *this;
    postorderTraversal(root);
    max_nodes = 0;
    if (other.root) preorderTraversal(other.root);
    return *this;
}

/**
 * Move assignment operator for transferring ownership.
 */
template<typename T>
ScapeGoatTree<T>& ScapeGoatTree<T>::operator=(ScapeGoatTree&& other) noexcept {
    if (this == &other) return *this;
    postorderTraversal(root);
    root = other.root;
    nNodes = other.nNodes;
    max_nodes = other.max_nodes;

    other.root = nullptr;
    other.nNodes = 0;
    other.max_nodes = 0;

    return *this;
}

/**
 * Overloaded plus operator for inserting a value.
 */
template<typename T>
void ScapeGoatTree<T>::operator+(const T& value) { insert(value); }

/**
 * Overloaded addition assignment operator for inserting a value.
 */
template<typename T>
void ScapeGoatTree<T>::operator+=(const T& value) { insert(value); }

/**
 * Overloaded subtraction assignment operator for deleting a value.
 */
template<typename T>
bool ScapeGoatTree<T>::operator-=(const T& value) { return deleteValue(value); }

/**
 * Overloaded subscript operator to search for a value in the tree.
 */
template<typename T>
bool ScapeGoatTree<T>::operator[](T value) const {
    return search(value);
}
/**
 * Clears the current tree if the assigned value is 0.
 */
template<typename T>
ScapeGoatTree<T>& ScapeGoatTree<T>::operator=(const int value) {
    if (value == 0) {
        clear();
    }
    return *this;
}
/**
 * Checks if two trees are equal by comparing their structures and values.
 */
template<typename T>
bool ScapeGoatTree<T>::operator==(const ScapeGoatTree& tree) const {
    return areTreesEqual(root, tree.root);
}
/**
 * Compares two subtrees for structural and value equality.
 */
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
/**
 * Checks if two trees are not equal.
 */
template<typename T>
bool ScapeGoatTree<T>::operator!=(const ScapeGoatTree& tree) const {
    return !(*this == tree);
}

/**
 * Checks if the tree is empty.
 */
template<typename T>
bool ScapeGoatTree<T>::operator!() const {
    return root == nullptr;
}

/**
 * Overloaded minus operator for deleting a value.
 */
template<typename T>
bool ScapeGoatTree<T>::operator-(const T &value) {
   return  deleteValue(value);
}

// =====================
// Balance / Search
// =====================

/**
 * Returns a string report indicating if the tree is currently balanced.
 */
template<typename T>
std::string ScapeGoatTree<T>::isBalanced() const {
    std::ostringstream out;

    const double n = countN(root);
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

/**
 * Searches for a specific value in the tree.
 */
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

/**
 * Removes all nodes from the tree and resets its state.
 */
template<typename T>
void ScapeGoatTree<T>::clear() {
    postorderTraversal(root);
    root = nullptr;
    nNodes = 0;
    max_nodes = 0;
}

template<typename T>
    void ScapeGoatTree<T>::undo() {
            if (undoStack.isEmpty()) return;
            isUndoing = true;
            Command<T> cmd = undoStack.pop();
            
            if (cmd.type == OpType::BatchEnd) {
                redoStack.push(cmd); // Push End first
                while (!undoStack.isEmpty()) {
                    Command<T> batchCmd = undoStack.pop();
                    redoStack.push(batchCmd);
                    if (batchCmd.type == OpType::BatchStart) break;

                    // Execute the inverse operation
                    if (batchCmd.type == OpType::Insert) deleteValue(batchCmd.value);
                    else if (batchCmd.type == OpType::Delete) insert(batchCmd.value);
                }
            } else {
                redoStack.push(cmd);
                if (cmd.type == OpType::Insert) deleteValue(cmd.value);
                else if (cmd.type == OpType::Delete) insert(cmd.value);
            }
            isUndoing = false;
        }

    template<typename T>
    void ScapeGoatTree<T>::redo() {
            if (redoStack.isEmpty()) return;
            isUndoing = true;
            Command<T> cmd = redoStack.pop();

            if (cmd.type == OpType::BatchStart) {
                undoStack.push(cmd);
                while (!redoStack.isEmpty()) {
                    Command<T> batchCmd = redoStack.pop();
                    undoStack.push(batchCmd);
                    if (batchCmd.type == OpType::BatchEnd) break;

                    // Re-execute the original operation
                    if (batchCmd.type == OpType::Insert) insert(batchCmd.value);
                    else if (batchCmd.type == OpType::Delete) deleteValue(batchCmd.value);
                }
            } else {
                undoStack.push(cmd);
                if (cmd.type == OpType::Insert) insert(cmd.value);
                else if (cmd.type == OpType::Delete) deleteValue(cmd.value);
            }
            isUndoing = false;
        }
#endif //TREE_SCAPEGOATTREE_TPP
