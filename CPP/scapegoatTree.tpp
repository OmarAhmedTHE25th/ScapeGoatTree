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

template<typename T>
ScapeGoatTree<T>::ScapeGoatTree(const double alpha)  {
    if (alpha > 1 or alpha < 0.5)return;
    ALPHA = alpha;
}

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
void ScapeGoatTree<T>::restructure_subtree(TreeNode *newNode) {
    //find the scapegoat  node
    TreeNode* goat = findTraitor(newNode->parent);
    if (goat == nullptr) return;
    T* temp_array = new T[nNodes];
    int i = 0;
    //flatten the tree into a sorted array
    inorderTraversal(goat, i, temp_array);
    const int sub_size = i; // size of subtree
    //rebuild the subtree
    TreeNode* balanced = rebuildTree(0, sub_size- 1, goat->parent, temp_array);
    rebuildCount++;
    //reattach the rebuilt subtree
    if (!goat->parent) root = balanced; // if goat is root then update root
    else if (goat == goat->parent->left) goat->parent->left = balanced; //if goat is left child then update left pointer
    else goat->parent->right = balanced; //if goat is right child then update right pointer
    postorderTraversal(goat);// delete old subtree
    delete[] temp_array;
}

/**
 * Inserts a new value into the tree and maintains balance if needed.
 */
template<typename T>
void ScapeGoatTree<T>::insert(T value) {
    // Record the operation for undo if not currently undoing/redoing
    if (!isUndoing) {
        undoStack.push({OpType::Insert, value});
    }
    Vector<TreeNode*> path;
    if (!root) {
        root = new TreeNode(value, nullptr);
        nNodes++;
        if (nNodes > max_nodes) max_nodes = nNodes;
        return;
    }

    TreeNode* current = root;
    TreeNode* parent = nullptr;
    int depth = 0;

    while (current) {
        path.push_back(current);
        parent = current;
        ++current->size;
        depth++;
        if (value < current->value)
            current = current->left;
        else if (value > current->value)
            current = current->right;
        else {
            // value already exists, backtrack size increment
            for (int i = 0; i < path.size(); i++) {
                --path[i]->size;
            }
            // If the value already exists, we didn't actually change the tree,
            // so we remove the command from the undo stack.
            if (!isUndoing) {
                undoStack.pop();
            }
            return;
        }
    }
    auto* newNode = new TreeNode(value, parent);
    if (value < parent->value)
        parent->left = newNode;
    else
        parent->right = newNode;

    nNodes++;
    if (nNodes > max_nodes) max_nodes = nNodes;

    if (depth + 1 <= getThreshold()) return;
    restructure_subtree(newNode);
}
/**
 * Inserts multiple values from a Vector into the tree.
 */
template<typename T>
    void ScapeGoatTree<T>::insertBatch(const Vector<T>& values) {
    // Group multiple insertions into a single undo/redo unit
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
    // Group multiple deletions into a single undo/redo unit
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
    TreeNode* node = root;
    TreeNode* parent = nullptr;

    // Step 1: Search for the node
    while (node != nullptr and node->value != value) {
        parent = node;
        if (value < node->value)
            node = node->left;
        else
            node = node->right;
    }

    // Value not found
    if (!node) return false;

    // Record the operation for undo if not currently undoing/redoing
    if (not isUndoing) {
        undoStack.push({OpType::Delete, value});
    }
    bool originalIsUndoing = isUndoing;
    isUndoing = true;
    // Case 1: Leaf node
    if (!node->left && !node->right) {
        // Decrement size for all nodes on the path
        TreeNode* temp = root;
        while (temp != node) {
            --temp->size;
            if (value < temp->value) temp = temp->left;
            else temp = temp->right;
        }
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
        // Decrement size for all nodes on the path
        TreeNode* temp = root;
        while (temp != node) {
            --temp->size;
            if (value < temp->value) temp = temp->left;
            else temp = temp->right;
        }
        TreeNode* child = nullptr;

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
        TreeNode* suc = node->right;
        while (suc->left != nullptr)
            suc = suc->left;

        T successorValue = suc->value;
        deleteValue(successorValue);
        node->value = successorValue;
        isUndoing = originalIsUndoing;
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
    isUndoing = originalIsUndoing;
    return true;
}

// =====================
// Utility / Static helpers
// =====================

/**
 * Calculates the height of a given node in the tree.
 */
template<typename T>
int ScapeGoatTree<T>::findH(const TreeNode *node) {
    if (!node) return -1;
    const int max = findH(node->left) > findH(node->right) ? findH(node->left) :findH(node->right);
   return 1+ max;
}

/**
 * Counts the total number of nodes in the subtree rooted at the given node.
 */
template<typename T>
unsigned int ScapeGoatTree<T>::countN(const TreeNode *node) {
    if (!node) return 0;
    return node->size;
}

/**
 * Finds the highest node that violates the alpha-weight-balance property.
 */
template<typename T>
ScapeGoatTree<T>::TreeNode* ScapeGoatTree<T>::findTraitor(TreeNode *node) {
    while (node != nullptr) {
        const int left = countN(node->left);
        const int right = countN(node->right);
        int size = node->size;

        if (left > ALPHA * size || right > ALPHA * size)
            return node;

        node = node->parent;
    }
    return nullptr;
}

/**
 * Recursively rebuilds a balanced BST from a sorted array of values.
 */
template<typename T>
ScapeGoatTree<T>::TreeNode* ScapeGoatTree<T>::rebuildTree(const int start, const int end, TreeNode* parent_node,T* array) {
    if (start > end) return nullptr; // base case
    int mid = (start + end) / 2; // find mid index
    auto* Nroot = new TreeNode(array[mid], parent_node); // create node with mid value
    Nroot->left = rebuildTree(start, mid - 1, Nroot, array); // build left subtree
    Nroot->right = rebuildTree(mid + 1, end, Nroot, array);// build right subtree
    Nroot->size = 1 + countN(Nroot->left) + countN(Nroot->right);// update size
    return Nroot;
}
/**
 * Checks if a rebuild is needed after a deletion and performs it if necessary.
 */
template<typename T>
void ScapeGoatTree<T>::DeletionRebuild(){
        if (nNodes < 0.5 * max_nodes&& nNodes > 0) {  // α = 0.5 for deletion
            auto temp_array = new T[nNodes];
            int i = 0;
            inorderTraversal(root, i, temp_array);
           const TreeNode* oldRoot = root;
            root = rebuildTree(0, nNodes - 1, nullptr, temp_array);
            rebuildCount++;
            postorderTraversal(oldRoot);
            max_nodes = nNodes;
            delete[] temp_array;
        }
        }
/**
 * Returns a pointer to the root node of the tree.
 */
template<typename T>
const ScapeGoatTree<T>::TreeNode *ScapeGoatTree<T>::getRoot() {
    return root;
}

// =====================
// Traversals
// =====================

/**
 * Performs an in-order traversal to populate a sorted array with node values.
 */
template<typename T>
void ScapeGoatTree<T>::inorderTraversal(const TreeNode* node, int& i, T*& array) const {
if (!node) return;
    inorderTraversal(node->left, i,array);
   array[i++]= node->value;
    inorderTraversal(node->right, i,array);
}

/**
 * Recursively deletes all nodes in the subtree using post-order traversal.
 */
template<typename T>
void ScapeGoatTree<T>::postorderTraversal(const TreeNode* node) {
    if (!node) return;
    postorderTraversal(node->left);
    postorderTraversal(node->right);
    delete node;
}

/**
 * Performs a pre-order traversal for internal processing.
 */
template<typename T>
void ScapeGoatTree<T>::preorderTraversal(const TreeNode* node) {
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
// diplay order w ostream (output stream)
void ScapeGoatTree<T>::displayPreOrder(const TreeNode* node, std::ostream& os) {
    if (!node) return;  // lw mfesh node bn return
    os << node->value << " ";
    displayPreOrder(node->left, os);
    displayPreOrder(node->right, os);
}

/**
 * Formats the tree in in-order.
 */
template<typename T>
void ScapeGoatTree<T>::displayInOrder(const TreeNode* node, std::ostream& os) {
    if (!node) return;
    displayInOrder(node->left, os);
    os << node->value << " ";
    displayInOrder(node->right, os);
}

/**
 * Formats the tree in post-order.
 */
template<typename T>
void ScapeGoatTree<T>::displayPostOrder(const TreeNode* node, std::ostream& os) {
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
    Queue<TreeNode*> q;
    q.push(root);
    int level =0;

    while (!q.isEmpty()) {
        result += "Level " + std::to_string(level++) + ": ";    // mn int l string b3dha bnro7 ll lvl ele ablo
        const int nodesAtLevel = q.size();
        for (int i = 0; i < nodesAtLevel; i++) {
            TreeNode* curr = q.front(); // current node
            q.pop(); // pop 3shan nshof el node ele b3dha

            std::ostringstream oss;
            oss << curr->value;
            result += oss.str() + " ";  // str 3shan n7wlo string

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
    /**
         * if first is smaller then take it
         * and increment the indicies for the merged list and the current array
         * else if second is smaller then take it and increment the indicies for the merged list and the current array
         * else (duplicates) take one of them and skip the duplicate in the other array
         */
    while (idx1 < nNodes && idx2 < other.nNodes) { //run as long as both arrays have elements
        if (array[idx1] < other_array[idx2]) temp_array[idx++] = array[idx1++];
        else if (array[idx1] > other_array[idx2]) temp_array[idx++] = other_array[idx2++];
        else { // duplicates
            temp_array[idx++] = array[idx1++];
            idx2++;
        }
    }
    while (idx1 < nNodes) temp_array[idx++] = array[idx1++]; //append remaining elements from first array
    while (idx2 < other.nNodes) temp_array[idx++] = other_array[idx2++]; //append remaining elements from second array

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
bool ScapeGoatTree<T>::areTreesEqual(const TreeNode* n1, const TreeNode* n2) const {
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
    out << "Height bound: " << bound << "\n";
    out << "Total Rebuilds: " << rebuildCount << "\n\n";

    if (height<= bound)
        out << "^_____^ Tree is balanced. \nCongratulations, It's not a Linked List.\n";
    else
        out << "~_~ Tree is NOT balanced. A scapegoat must be sacrificed.\n";

    return out.str();
}

/**
 * Searches for a specific value in the tree.
 */
template<typename T>
bool ScapeGoatTree<T>::search(const T& key) const {
    TreeNode* current = root;
    while (current != nullptr) {
        if (key == current->value) return true;
        if (key < current->value) current = current->left;

        else if (key > current->value) current = current->right;

    }
    return false;
}

template<typename T>
ScapeGoatTree<T>::TreeNode *ScapeGoatTree<T>::search(T &key) const {
    TreeNode* current = root;
    while (current != nullptr) {
        if (key == current->value) return current;
        if (key < current->value) current = current->left;

        else if (key > current->value) current = current->right;

    }
    return nullptr;
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
/**
 * Undo the last operation (insert or delete).
 * If the last operation was a batch, it undoes the entire batch.
 */

template<typename T>
    void ScapeGoatTree<T>::undo() {
            if (undoStack.isEmpty()) return;
            // Set flag to prevent undo actions from being recorded as new operations
            isUndoing = true;
            Command<T> cmd = undoStack.pop();

            if (cmd.type == OpType::BatchEnd) {
                // Handle batch operation: undo everything until BatchStart
                redoStack.push(cmd); // Push End first to maintain order for redo
                while (!undoStack.isEmpty()) {
                    Command<T> batchCmd = undoStack.pop();
                    redoStack.push(batchCmd);
                    if (batchCmd.type == OpType::BatchStart) break;

                    // Execute the inverse of the recorded operation
                    if (batchCmd.type == OpType::Insert) deleteValue(batchCmd.value);
                    else if (batchCmd.type == OpType::Delete) insert(batchCmd.value);
                }
            } else {
                // Handle single operation
                redoStack.push(cmd);
                if (cmd.type == OpType::Insert) deleteValue(cmd.value);
                else if (cmd.type == OpType::Delete) insert(cmd.value);
            }
            isUndoing = false;
        }
/**
 * Redo the last undone operation.
 * If the last undone operation was a batch, it redoes the entire batch.
 */
    template<typename T>
    void ScapeGoatTree<T>::redo() {
            if (redoStack.isEmpty()) return;
            // Set flag to prevent redo actions from being recorded in undoStack incorrectly
            isUndoing = true;
            Command<T> cmd = redoStack.pop();

            if (cmd.type == OpType::BatchStart) {
                // Handle batch operation: redo everything until BatchEnd
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
                // Handle single operation
                undoStack.push(cmd);
                if (cmd.type == OpType::Insert) insert(cmd.value);
                else if (cmd.type == OpType::Delete) deleteValue(cmd.value);
            }
            isUndoing = false;
        }

template<typename T>
T ScapeGoatTree<T>::sumHelper(TreeNode *node,T min,T max) {
    T sum {};
    if (!node)return 0;
    if (node->value >= min)sum+=sumHelper(node->left,min,max);
    if (node->value >= min && node->value <= max )sum+=node->value;
    if (node->value <= max)sum+=sumHelper(node->right,min,max);
    return sum;

}

template<typename T>
T ScapeGoatTree<T>::sumInRange(T min, T max) {
    return sumHelper(root,min,max);
}

template<typename T>
T ScapeGoatTree<T>::getMin() {
    if (!root)throw std::exception("Tree is Empty");
    TreeNode* current = root;
    while (current->left) current =current->left;
    return current->value;

}
template<typename T>
T ScapeGoatTree<T>::getMax() {
    if (!root)throw std::exception("Tree is Empty");
    TreeNode* current = root;
    while (current->right) current =current->right;
    return current->value;
}

template<typename T>
void ScapeGoatTree<T>::rangeHelper(TreeNode *node,T min,T max,Vector<T>& range) {
    if (!node)return;
    if (node->value > min)rangeHelper(node->left,min,max,range);
    if (node->value >= min && node->value <= max )range.push_back(node->value);
    if (node->value < max)rangeHelper(node->right,min,max,range);
}
template<typename T>
Vector<T> ScapeGoatTree<T>::valuesInRange(T min, T max) {
    Vector<T>range;
    rangeHelper(root,min,max,range);
    return  std::move(range);
}
//leftmost in the right subtree.
template<typename T>
T ScapeGoatTree<T>::getSuccessor(T value) const {
    TreeNode* current = root;
    TreeNode* successor = nullptr;
    while (current) {
        if (current->value > value) {
            successor = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }
    if (!successor) throw std::runtime_error("No successor found");
    return successor->value;
}

template<typename T>
T ScapeGoatTree<T>::kthSmallestHelper(TreeNode* node, int k) const {
    int leftSize = countN(node->left);
    if (k == leftSize + 1) return node->value;
    if (k <= leftSize) return kthSmallestHelper(node->left, k);
    return kthSmallestHelper(node->right, k - leftSize - 1);
}
template<typename T>
T ScapeGoatTree<T>::kthSmallest(int k) const {
    if (k < 1 || k > nNodes) throw std::out_of_range("k is out of bounds");
    return kthSmallestHelper(root, k);
}

template<typename T>
 ScapeGoatTree<T>::iterator ScapeGoatTree<T>::begin() {
    if (!root) return end();
    TreeNode* curr = root;
    while (curr->left)curr = curr->left;
    return iterator(curr);
}
template<typename T>
 ScapeGoatTree<T>::iterator ScapeGoatTree<T>::end() {
    return iterator(nullptr);
 }

template<typename T>
 ScapeGoatTree<T>::TreeNode *ScapeGoatTree<T>::findSuccessor(TreeNode *node) {
    if (!node)return nullptr;

    if (node->right) {
        TreeNode* suc = node->right;
        while (suc->left != nullptr)
            suc = suc->left;
        return suc;
    }
    TreeNode* p = node->parent;
    while ( p && node == p->right) {
        node = p;
        p = p->parent;
    }
    return p;
}

template<typename T>
int ScapeGoatTree<T>::updateSize(TreeNode*& node) {
    if (node == nullptr)
        return 0;

    // Find the size of left and right
    // subtree.
    const int left = updateSize(node->left);
    const int right = updateSize(node->right);

    //  the size of curr subtree.
    node->size = left+right+1;
    return node->size;
}

template<typename T>
std::pair<ScapeGoatTree<T>, ScapeGoatTree<T> > ScapeGoatTree<T>::split(T value) {
    TreeNode* node = search(value);
    if (!node)return {ScapeGoatTree{}, ScapeGoatTree{}};
    ScapeGoatTree tree1;
    ScapeGoatTree tree2;
    if (TreeNode* parent = node->parent) {
        if (parent->left == node) parent->left = nullptr;
        if (parent->right == node) parent->right = nullptr;
    }
    tree1.root = node->left;
    tree2.root = node->right;
    updateSize(tree1.root);
    updateSize(tree2.root);
    int size1 = tree1.root ? tree1.root->size : 0;
    int size2 = tree2.root ? tree2.root->size : 0;

    T* array1 = size1 ? new T[size1] : nullptr;
    T* array2 = size2 ? new T[size2] : nullptr;
    int i = 0, j =0;
    inorderTraversal(tree1.root,i,array1);
    tree1.root= rebuildTree(0, i-1,nullptr,array1);

    inorderTraversal(tree2.root,j,array2);
    tree2.root = rebuildTree(0, j-1,nullptr,array2);
    delete[] array1;
    delete[] array2;
    return {tree1,tree2};
}

#endif //TREE_SCAPEGOATTREE_TPP
