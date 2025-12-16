#include "ScapeGoatTree.hpp"
#include <algorithm>
#include <cmath>
#include <sstream>


using namespace std;
// Constructor starts with an empty tree
ScapeGoatTree::ScapeGoatTree()= default;
// This function puts the new node in the tree and sets its parent.
void ScapeGoatTree::insert(const int value) {

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
        depth++;
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
    const auto newNode = new Node(value, parent);
    // connect the new node to the parent (left/right)
    if (value < parent->value)
        parent->left = newNode;
    else
        parent->right = newNode;

    nNodes++;


    int threshold = static_cast<int>(log(nNodes) / log(1.5));
    if (depth+1>threshold) {
        Node* goat = findTraitor(newNode->parent);
        if (goat == nullptr) return;
        int i = 0;
        std::fill_n(array, nNodes, 0); // clear it
        inorderTraversal(goat,i);
        int size = i; // size of subtree
        Node* balanced = rebuildTree(0, size - 1,goat->parent);
        if (!goat->parent) root = balanced;
        else if (goat == goat->parent->left) goat->parent->left = balanced;
        else goat->parent->right = balanced;
        if (goat->left)  goat->left->parent = nullptr;
        if (goat->right) goat->right->parent = nullptr;

        postorderTraversal(goat);
    }
}
void ScapeGoatTree::deleteValue(const int value) {

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
    if (!node) return;

    //Deletion cases

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
    }

    // Case 3: Two children
    else {
        // Find inorder successor (smallest in right subtree)
        Node* suc = node->right;
        while (suc->left != nullptr)
            suc = suc->left;

        const int sucValue = suc->value;

        // Delete successor
        deleteValue(sucValue);

        // Replace value
        node->value = sucValue;
        return;
    }

    //Update node count
    nNodes--;
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
     while (node != nullptr) {
        int left = countN(node->left);
        int right = countN(node->right);
        int size = left + right + 1;

        if (left > (2.0/3.0) * size || right > (2.0/3.0) * size)
            return node;

        node = node->parent;
    }
    return nullptr;
}
Node* ScapeGoatTree::rebuildTree(const int start, const int end,Node* parent_node)  {
    if (start>end)return  nullptr;
    int mid =(start+end)/2;
    Node* Nroot = new Node(array[mid],parent_node);
    Nroot->left = rebuildTree(start, mid-1,Nroot);
    Nroot->right = rebuildTree(mid+1,end,Nroot);
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
void ScapeGoatTree::postorderTraversal(const Node *node) {
    if (!node)return;
    postorderTraversal(node->left);
    postorderTraversal(node->right);
    delete node;
}
void ScapeGoatTree::preorderTraversal(const Node *node) {
    if (!node)return;
    insert(node->value);
    preorderTraversal(node->left);
    preorderTraversal(node->right);
}


std::string ScapeGoatTree::isBalanced() const {
        std::ostringstream out;

        double n = countN(root);
        if (n == 0) {
            out << "Tree empty. Of course it's balanced 😎";
            return out.str();
        }

        int height = findH(root);
        double bound = log(n) / log(1.5);

        out << "Node count: " << n << "\n";
        out << "Height: " << height << "\n";
        out << "Height bound: " << bound << "\n\n";

        if (height <= bound)
            out << ":D Tree is balanced -- congratulations. It's not a Linked List.";
        else
            out << "~_~ Tree is NOT balanced. A scapegoat must be sacrificed.";

        return out.str();
    }




ScapeGoatTree::ScapeGoatTree(const ScapeGoatTree &Otree) {
    if (!Otree.root)return;
    preorderTraversal(Otree.root);
}
ScapeGoatTree ScapeGoatTree::operator+(const ScapeGoatTree& goat_tree ) const {
    ScapeGoatTree newTree;
    newTree.preorderTraversal(root);
    newTree.preorderTraversal(goat_tree.root);
    return newTree;
}
ScapeGoatTree& ScapeGoatTree::operator=(const ScapeGoatTree &Otree) {
    if (this == &Otree) return *this;

    postorderTraversal(root);
    root = nullptr;
    nNodes = 0;
    std::fill_n(array, 100, 0);

    if (Otree.root) {
        preorderTraversal(Otree.root);
    }
    return *this;
}
void ScapeGoatTree::operator+(const int value) {
    insert(value);
}
bool ScapeGoatTree::search(const int key) const {
    Node* current=root;
    while (current != nullptr) {
        if (key==current->value)return true;
        if (key < current->value) {
            current = current->left;
        }
        else if (key > current->value) {
            current = current->right;
        }

    }
    return false;
}
bool ScapeGoatTree::operator[](const int value) const {
    return search(value);
}
bool ScapeGoatTree::operator==(const ScapeGoatTree &tree) const {
    if (nNodes != tree.nNodes)return false;
    for (int i = 0; i < nNodes; i++)if (array[i]!=tree.array[i])return false;
    return true;
}
bool ScapeGoatTree::operator!=(const ScapeGoatTree &tree) const {
    return this != &tree;
}
bool ScapeGoatTree::operator!() const {
    if (root)return true;
    return  false;
}
void ScapeGoatTree::operator-(const int value) {
    deleteValue(value);
}
void ScapeGoatTree::operator-=(const int value) {
    deleteValue(value);
}
void ScapeGoatTree::operator+=(const int value) {
    insert(value);
}

// For display
// Pre-Order helper
void ScapeGoatTree::displayPreOrder(const Node* node) {
    if (!node) return;
    displayBuffer += std::to_string(node->value) + " ";
    displayPreOrder(node->left);
    displayPreOrder(node->right);
}

// Public
void ScapeGoatTree::displayPreOrder() {
    displayBuffer.clear();
    if (!root) {
        displayBuffer = "Tree is empty.";
        return;
    }
    displayPreOrder(root);
}
void ScapeGoatTree::displayInOrder(const Node* node) {
    if (!node) return;
    displayInOrder(node->left);
    displayBuffer += std::to_string(node->value) + " ";
    displayInOrder(node->right);
}

void ScapeGoatTree::displayInOrder() {
    displayBuffer.clear();
    if (!root) {
        displayBuffer = "Tree is empty.";
        return;
    }
    displayInOrder(root);
}

void ScapeGoatTree::displayPostOrder(const Node* node) {
    if (!node) return;
    displayPostOrder(node->left);
    displayPostOrder(node->right);
    displayBuffer += std::to_string(node->value) + " ";
}

void ScapeGoatTree::displayPostOrder()  {
    displayBuffer.clear();
    if (!root) {
        displayBuffer = "Tree is empty.";
        return;
    }
    displayPostOrder(root);
}
   std::string& ScapeGoatTree::getDisplayBuffer() const {
    return displayBuffer;
}
