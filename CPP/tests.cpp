#include <iostream>
#include <cassert>
#include <vector>
#include "ScapeGoatTree.hpp"

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
    ScapeGoatTree<int> tree;
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
    ScapeGoatTree<int> tree;
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
    ScapeGoatTree<int> tree;
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
    ScapeGoatTree<int> tree1;
    tree1.insert(10);
    tree1.insert(20);

    ScapeGoatTree<int> tree2;
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
    ScapeGoatTree<int> emptyTree;
    assert(!emptyTree);
    assert(!!tree1);

    std::cout << "Operators Passed!" << std::endl;
}

void testBatchOperations() {
    std::cout << "Testing Batch Operations..." << std::endl;
    ScapeGoatTree<int> tree;
    Vector<int> values;
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
    ScapeGoatTree<int> tree1;
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
    ScapeGoatTree<int> tree3 = tree1;
    assert(tree3 == tree1);

    // Move constructor
    const ScapeGoatTree tree4(std::move(tree2));
    assert(tree4.search(10));
    assert(tree4.search(20));
    assert(tree4.search(30));
    // tree2 should be empty now
    assert(!tree2);

    // Move assignment
    const ScapeGoatTree<int> tree5 = std::move(tree3);
    assert(tree5.search(10));
    assert(!tree3);

    std::cout << "Copy and Move Passed!" << std::endl;
}

void testUandR() {
    std::cout << "Testing Undo..." << std::endl;
    ScapeGoatTree<int> tree;

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
    Vector<int> values;
    values.push_back(1);
    values.push_back(2);
    values.push_back(3);
    tree.insertBatch(values);
    assert(tree.search(1) && tree.search(2) && tree.search(3));
    tree.undo();
    assert(!tree.search(1) && !tree.search(2) && !tree.search(3));
    assert(tree.search(20));
    // Test batch redo
    tree.insertBatch(values);
    assert(tree.search(1) && tree.search(2) && tree.search(3));
    tree.redo();
    assert(tree.search(1) && tree.search(2) && tree.search(3));
    assert(tree.search(20));

    std::cout << "Undo and Redo Passed!" << std::endl;
}

int main() {
    try {
        testBasicInsertion();
        testDeletion();
        testRebuilding();
        testOperators();
        testBatchOperations();
        testCopyAndMove();
        testUandR();
        std::cout << "\nALL TESTS PASSED SUCCESSFULLY!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
