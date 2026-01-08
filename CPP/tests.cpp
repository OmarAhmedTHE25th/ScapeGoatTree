#include <iostream>
#include <cassert>
#include <vector>
#include <random>
#include <algorithm>
#include <set>
#include "ScapeGoatTree.hpp"
typedef int Type;
// Helper function to check if the tree contains all values in a vector
template<typename T>
bool containsAll(const ScapeGoatTree<T>& tree, const std::vector<T>& values) {
    for (const auto& val : values) {
        if (!tree.search(val)) return false;
    }
    return true;
}

void testBasicInsertion() {
    std::cout << "Testing Basic Insertion..." << std::endl;
    ScapeGoatTree<Type> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(5);

    assert(tree.search(10));
    assert(tree.search(20));
    assert(tree.search(5));
    assert(!tree.search(15));
    std::cout << "Basic Insertion Passed!" << std::endl;
}

void testDeletion() {
    std::cout << "Testing Deletion..." << std::endl;
    ScapeGoatTree<Type> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(5);
    tree.insert(15);
    tree.insert(25);

    // Delete leaf
    assert(tree.deleteValue(25));
    assert(!tree.search(25));

    // Delete node with one child
    assert(tree.deleteValue(20));
    assert(!tree.search(20));
    assert(tree.search(15));

    // Delete node with two children
    tree.insert(20);
    tree.insert(25);
    assert(tree.deleteValue(10));
    assert(!tree.search(10));
    assert(tree.search(15));
    assert(tree.search(5));
    assert(tree.search(20));
    assert(tree.search(25));

    std::cout << "Deletion Passed!" << std::endl;
}

void testRebuilding() {
    std::cout << "Testing Rebuilding (Balancing)..." << std::endl;
    ScapeGoatTree<Type> tree;
    // Insert values in increasing order to force imbalance
    for (int i = 1; i <= 10; ++i) {
        tree.insert(i);
    }

    // After 10 insertions, it should have triggered some rebuilds
    // Scapegoat tree with alpha = 2/3 should be relatively balanced
    for (int i = 1; i <= 10; ++i) {
        assert(tree.search(i));
    }

    std::string balancedStatus = tree.isBalanced();
    std::cout << "Balance status: " << balancedStatus << std::endl;
    // We expect it to be balanced according to the internal alpha property
    assert(balancedStatus.find("Yes") != std::string::npos || balancedStatus.find("YES") != std::string::npos || balancedStatus.find("is balanced") != std::string::npos);

    std::cout << "Rebuilding Passed!" << std::endl;
}

void testOperators() {
    std::cout << "Testing Operators..." << std::endl;
    ScapeGoatTree<Type> tree1;
    tree1.insert(10);
    tree1.insert(20);

    ScapeGoatTree<Type> tree2;
    tree2.insert(10);
    tree2.insert(20);

    assert(tree1 == tree2);

    tree2.insert(30);
    assert(tree1 != tree2);

    // Test operator+
    tree1 + 30;
    assert(tree1.search(30));
    assert(tree1 == tree2);

    // Test operator-
    tree1 - 30;
    assert(!tree1.search(30));
    assert(tree1 != tree2);

    // Test operator!
    ScapeGoatTree<Type> emptyTree;
    assert(!emptyTree);
    assert(!!tree1);

    std::cout << "Operators Passed!" << std::endl;
}

void testBatchOperations() {
    std::cout << "Testing Batch Operations..." << std::endl;
    ScapeGoatTree<Type> tree;
    Vector<Type> values;
    values.push_back(1);
    values.push_back(2);
    values.push_back(3);
    values.push_back(4);
    values.push_back(5);

    tree.insertBatch(values);
    for (int i = 1; i <= 5; ++i) {
        assert(tree.search(i));
    }

    tree.deleteBatch(values);
    for (int i = 1; i <= 5; ++i) {
        assert(!tree.search(i));
    }

    std::cout << "Batch Operations Passed!" << std::endl;
}

void testCopyAndMove() {
    std::cout << "Testing Copy and Move..." << std::endl;
    ScapeGoatTree<Type> tree1;
    tree1.insert(10);
    tree1.insert(20);
    tree1.insert(30);

    // Copy constructor
    ScapeGoatTree tree2(tree1);
    assert(tree2 == tree1);
    assert(tree2.search(10));
    assert(tree2.search(20));
    assert(tree2.search(30));

    // Copy assignment
    ScapeGoatTree<Type> tree3 = tree1;
    assert(tree3 == tree1);

    // Move constructor
    const ScapeGoatTree tree4(std::move(tree2));
    assert(tree4.search(10));
    assert(tree4.search(20));
    assert(tree4.search(30));
    // tree2 should be empty now
    assert(!tree2);

    // Move assignment
    const ScapeGoatTree<Type> tree5 = std::move(tree3);
    assert(tree5.search(10));
    assert(!tree3);

    std::cout << "Copy and Move Passed!" << std::endl;
}

void testUandR() {
    std::cout << "Testing Undo and Redo..." << std::endl;
    ScapeGoatTree<Type> tree;

    // Test undo insertion
    tree.insert(10);
    assert(tree.search(10));
    tree.undo();
    assert(!tree.search(10));
// Test redo insertion
    tree.insert(10);
    assert(tree.search(10));
    tree.redo();
    assert(tree.search(10));

    // Test undo deletion
    tree.insert(20);
    assert(tree.search(20));
    tree.deleteValue(20);
    assert(!tree.search(20));
    tree.undo();
    assert(tree.search(20));
    // Test redo deletion
    tree.deleteValue(20);
    assert(!tree.search(20));
    tree.redo();
    assert(!tree.search(20));


    // Test batch undo
    Vector<Type> values;
    values.push_back(1);
    values.push_back(2);
    values.push_back(3);
    tree.insertBatch(values);
    assert(tree.search(1) && tree.search(2) && tree.search(3));
    tree.undo();
    assert(!tree.search(1) && !tree.search(2) && !tree.search(3));

    // Test batch redo
    tree.insertBatch(values);
    assert(tree.search(1) && tree.search(2) && tree.search(3));
    tree.redo();
    assert(tree.search(1) && tree.search(2) && tree.search(3));


    std::cout << "Undo and Redo Passed!" << std::endl;
}
void testNewMethods() {
    std::cout << "Testing New Query Methods (Min, Max, Successor, Kth, Range)..." << std::endl;
    ScapeGoatTree<Type> tree;
    std::vector<int> values = {10, 5, 15, 3, 7, 12, 18};
    for (int v : values) tree.insert(v);

    // Min/Max
    assert(tree.getMin() == 3);
    assert(tree.getMax() == 18);

    // Successor
    assert(tree.getSuccessor(7) == 10);
    assert(tree.getSuccessor(12) == 15);
    try { tree.getSuccessor(18); assert(false); } catch (...) {}

    // Kth Smallest (1-indexed)
    assert(tree.kthSmallest(1) == 3);
    assert(tree.kthSmallest(4) == 10);
    assert(tree.kthSmallest(7) == 18);

    // Sum and Values in Range
    assert(tree.sumInRange(5, 12) == (5 + 7 + 10 + 12));

    std::cout << "New Query Methods Passed!" << std::endl;
}

void stressTest() {
    std::cout << "Starting Extreme Stress Test (Large scale randomized operations)..." << std::endl;
    ScapeGoatTree<Type> tree;
    std::set<Type> reference_set;

    constexpr int OPERATIONS = 50000;
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1, 100000);
    std::uniform_int_distribution<int> op_dist(0, 2); // 0: Insert, 1: Delete, 2: Search

    for (int i = 0; i < OPERATIONS; ++i) {
        int val = dist(rng);
        int op = op_dist(rng);

        if (op == 0) { // Insert
            tree.insert(val);
            reference_set.insert(val);
        } else if (op == 1) { // Delete
            tree.deleteValue(val);
            reference_set.erase(val);
        } else { // Search
            assert(tree.search(val) == (reference_set.contains(val)));
        }

        // Periodically check balance and order statistics
        if (i % 10000 == 0 && !reference_set.empty()) {
            assert(tree.getMin() == *reference_set.begin());
            assert(tree.getMax() == *reference_set.rbegin());

            // Check if tree actually respects the height bound
            std::string report = tree.isBalanced();
            assert(report.find("NOT balanced") == std::string::npos);
        }
    }

    // Final sanity check: Total count
    std::cout << "Stress Test: Rebuilding verification..." << std::endl;
    Vector<Type> inorder_vals;
    // (You'd need a way to get all values, for now we verify via kthSmallest)
    assert(tree.kthSmallest(1) == *reference_set.begin());
    assert(tree.kthSmallest(reference_set.size()) == *reference_set.rbegin());

    std::cout << "Extreme Stress Test Passed with " << OPERATIONS << " operations!" << std::endl;
}

void testOrderedInsertion() {
    std::cout << "Testing Ordered Insertion (Forcing Scapegoat rebuilds)..." << std::endl;
    ScapeGoatTree<Type> tree;
    constexpr int N = 2000;
    for (int i = 1; i <= N; ++i) {
        tree.insert(i);
    }

    // Verify all are there and the tree is balanced
    assert(tree.getMin() == 1);
    assert(tree.getMax() == N);
    assert(tree.isBalanced().find("NOT balanced") == std::string::npos);
    std::cout << "Ordered Insertion Passed!" << std::endl;
}

int main() {
    try {
        testBasicInsertion();
        testDeletion();
        testNewMethods(); // New!
        testOrderedInsertion(); // New!
        testRebuilding();
        testOperators();
        testBatchOperations();
        testCopyAndMove();
        testUandR();
        stressTest(); // New!
        std::cout << "\n=======================================" << std::endl;
        std::cout << "🏆 ALL EXTREME TESTS PASSED SUCCESSFULLY!" << std::endl;
        std::cout << "=======================================" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}