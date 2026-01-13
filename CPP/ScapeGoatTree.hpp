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

#include <string>
#include "Node.hpp"
#include <cmath>
#include "vector.hpp"
#include "stack.hpp"
/**
 * Represents the type of operation performed on the tree for undo/redo purposes.
 */
enum class OpType {
    Insert,     // Insertion of a single value
    Delete,     // Deletion of a single value
    BatchStart, // Marker for the beginning of a batch operation
    BatchEnd    // Marker for the end of a batch operation
};

/**
 * Encapsulates a command that can be undone or redone.
 */
template<typename T>
struct Command {
    OpType type; // Type of the operation
    T value;     // Value associated with the operation
};

template<typename T>
class ScapeGoatTree {

    using TreeNode = Node<T>;
    TreeNode* root{};
    int nNodes{};
    int rebuildCount = 0;
    /**
     * Stack to store commands that can be undone.
     */
    Stack<Command<T>> undoStack;
    /**
     * Stack to store commands that have been undone and can be redone.
     */
    Stack<Command<T>> redoStack;
    /**
     * Flag to prevent operations triggered by undo/redo from being recorded.
     * This avoids infinite recursion and keeps the undo history clean.
     */
    bool isUndoing = false;
    int max_nodes = 0;
    // iterator class
    class iterator {
        TreeNode* curr;  // stores current node

    public:
        // constructor
        iterator(TreeNode* node) : curr(node) {}

        // dereference
        T& operator*() { return curr->value; }

        // pre-increment
        iterator& operator++() {
            curr = getSuccessor(curr);
            return *this;
        }

        // post-increment
        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        // comparison
        bool operator!=(const iterator& other) const { return curr != other.curr; }
    };

    /**
     * Calculates the height of a given node in the tree.
     */
    static int findH(const TreeNode* node);
    /**
     * Counts the total number of nodes in the subtree rooted at the given node.
     */
    static  unsigned int countN(const TreeNode* node);
    /**
     * Finds the highest node that violates the alpha-weight-balance property.
     */
    static TreeNode* findTraitor(TreeNode* node);
    /**
     * Recursively rebuilds a balanced BST from a sorted array of values.
     */
    TreeNode* rebuildTree(int start,int end,TreeNode* parent_node,T* array);
    /**
     * Performs an in-order traversal to populate a sorted array with node values.
     */
    void inorderTraversal(const TreeNode*node, int &i,T*& array) const;
    /**
     * Recursively deletes all nodes in the subtree using post-order traversal.
     */
    static void postorderTraversal(const TreeNode* node);
    /**
     * Performs a pre-order traversal for internal processing.
     */
    void preorderTraversal(const TreeNode* node);
    /**
     * Formats the tree in pre-order.
     */
    void displayPreOrder(const TreeNode* node, std::ostream& os);
    /**
     * Formats the tree in in-order.
     */
    void displayInOrder(const TreeNode* node, std::ostream& os);
    /**
     * Formats the tree in post-order.
     */
    void displayPostOrder(const TreeNode* node, std::ostream& os);
    /**
     * Calculates the maximum allowed height before a rebuild is triggered.
     */
    [[nodiscard]] int getThreshold() const {return static_cast<int>(log(nNodes) / log(1.5));}
    /**
     * Checks if a rebuild is needed after a deletion and performs it if necessary.
     */
    void DeletionRebuild();
    /**
     * Compares two subtrees for structural and value equality.
     */
    bool areTreesEqual(const TreeNode* n1, const TreeNode* n2) const;
    /**
     * Initiates a subtree rebuild starting from the scapegoat node.
     */
    void restructure_subtree(TreeNode *newNode);
    T sumHelper(TreeNode* node,T min,T max);
    void rangeHelper(TreeNode* node,T min,T max,Vector<T>& range);
    T kthSmallestHelper(TreeNode *node, int k) const;
  static TreeNode* getSuccessor(TreeNode* node);




public:

    /**
     * Default constructor for an empty Scapegoat Tree.
     */
    ScapeGoatTree();

    /**
     * Inserts a new value into the tree and maintains balance if needed.
     */
    void insert(T value);

    /**
     * Inserts multiple values from a Vector into the tree.
     */
    void insertBatch( const Vector<T> &values);

    /**
     * Removes a value from the tree and maintains balance if needed.
     */
    bool deleteValue(T value);

    /**
     * Removes multiple values from a Vector from the tree.
     */
    void deleteBatch(const Vector<T> &values);

    /**
     * Searches for a specific value in the tree.
     */
    [[nodiscard]] bool search(const T & key) const;

    /**
     * Removes all nodes from the tree and resets its state.
     */
    void clear();
    void undo();
    void redo();
    T sumInRange(T min, T max);
    T getMin();
    T getMax();
    Vector<T> valuesInRange(T min,T max);
    T getSuccessor(T value) const;
    T kthSmallest(int k) const;


    /**
     * Returns a string report indicating if the tree is currently balanced.
     */
    [[nodiscard]] std::string isBalanced() const;


    /**
     * Returns a pointer to the root node of the tree.
     */
    const TreeNode* getRoot();
    iterator begin();

    static iterator end();

    /**
     * Copy constructor for deep copying another ScapeGoatTree.
     */
    ScapeGoatTree(const ScapeGoatTree &Otree);

    /**
     * Move constructor for transferring ownership from another ScapeGoatTree.
     */
    ScapeGoatTree(ScapeGoatTree&& other) noexcept;

    /**
     * Destructor that cleans up all nodes in the tree.
     */
    ~ScapeGoatTree();

    /**
     * Returns a string representing the tree in pre-order traversal.
     */
    std::string displayPreOrder(); // for display

    /**
     * Returns a string representing the tree in in-order traversal.
     */
    std::string displayInOrder() ; // for display

    /**
     * Returns a string representing the tree in post-order traversal.
     */
    std::string displayPostOrder() ; // for display

    /**
     * Returns a string representing the tree in level-order traversal.
     */
    std::string displayLevels(); // for display

    /**
     * Overloaded subscript operator to search for a value in the tree.
     */
    bool operator[](T value) const;

    /**
     * Creates a new tree containing elements from both trees.
     */
    ScapeGoatTree operator+(const ScapeGoatTree &other) const;

    /**
     * Assignment operator for deep copying.
     */
    ScapeGoatTree& operator=(const ScapeGoatTree& other);

    /**
     * Move assignment operator.
     */
    ScapeGoatTree& operator=(ScapeGoatTree&& other) noexcept;

    /**
     * Clears the current tree and initializes it with a single value.
     */
    ScapeGoatTree &operator=(int value);

    /**
     * Checks if two trees are equal.
     */
    bool operator==(const ScapeGoatTree &tree) const;

    /**
     * Checks if two trees are not equal.
     */
    bool operator!=(const ScapeGoatTree &tree) const;

    /**
     * Checks if the tree is empty.
     */
    bool operator!() const;

    /**
     * Overloaded plus operator for inserting a value.
     */
    void operator+(const T& value);

    /**
     * Overloaded minus operator for deleting a value.
     */
    bool operator-(const T& value);

    /**
     * Overloaded subtraction assignment operator for deleting a value.
     */
    bool operator-=(const T& value);

    /**
     * Overloaded addition assignment operator for inserting a value.
     */
    void operator+=(const T& value);



};
#include "scapegoatTree.tpp"

#endif //SCAPEGOATTREE_SCAPEGOATTREE_HPP
