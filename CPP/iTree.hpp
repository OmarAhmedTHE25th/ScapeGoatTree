//
// Created by DELL on 24/12/2025.
//

#ifndef TREE_ITREE_HPP
#define TREE_ITREE_HPP
#include "ScapeGoatTree.hpp"
using namespace std;
typedef int ElemenType;

enum class opcodes {INSERT, DELETEOP, SEARCH, DISPLAY_INORDER, DISPLAY_PREORDER,
    DISPLAY_POSTORDER, DISPLAY_LEVELS,EXIT,BALANCE,COMPARE,MERGE,EMPTY,BATCH_INSERT,BATCH_DELETE,CLEAR,UNDO,REDO,SUMINRANGE};

class ITree {
    /**
     * Handles batch insertion and deletion operations.
     */
    static  void handleBatches(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B, opcodes op);

    /**
     * Handles standard tree operations like search.
     */
    static void handleOperations(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B, opcodes op);

    /**
     * Handles display operations for the trees.
     */
    static void handleDisplay(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B, opcodes op);

    /**
     * Handles checking and reporting the balance status of the trees.
     */
    static void handleBalance(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);

    /**
     * Handles core operators like insertion and deletion.
     */
    static void handleCoreOperators(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B, opcodes op);

    /**
     * Handles checking if the trees are empty.
     */
    static void handleOperatorEmpty(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);

    /**
     * Handles merging two trees together.
     */
    static void handleOperatorMerge(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);

    /**
     * Handles comparing two trees for equality.
     */
    static void handleOperatorCompare(const ScapeGoatTree<ElemenType> &A, const ScapeGoatTree<ElemenType> &B);

    /**
     * Prompts the user to select one of the two available trees.
     */
    static ScapeGoatTree<ElemenType>& selectTree(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);

    /**
     * Handles clearing the contents of the trees.
     */
    static  void handleClear(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);

    static void handleUndoRedo(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B, opcodes op);
    static void handleSuminRange(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);
public:
    /**
     * Launches the terminal-based user interface for interacting with the trees.
     */
    static void TreeUI();

};


#endif //TREE_ITREE_HPP