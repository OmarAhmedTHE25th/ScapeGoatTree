//
// Created by DELL on 24/12/2025.
//

#ifndef TREE_ITREE_HPP
#define TREE_ITREE_HPP
#include "ScapeGoatTree.hpp"
#include "string"
using namespace std;
typedef int ElemenType;
enum opcodes {INSERT, DELETE_op, SEARCH, DISPLAY_INORDER, DISPLAY_PREORDER, DISPLAY_POSTORDER, DISPLAY_LEVELS};

class ITree {

    static void printMenu();

    static  void handleBatches(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B, opcodes op);
    static void handleOperations(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B, opcodes op);
    static void handleDisplay(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B, opcodes op);
    static void handleBalance(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);
    static void handleCoreOperators(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B, opcodes op);
    static void handleOperatorEmpty(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);
    static void handleOperatorMerge(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);
    static void handleOperatorCompare(const ScapeGoatTree<ElemenType> &A, const ScapeGoatTree<ElemenType> &B);

    static ScapeGoatTree<ElemenType>& selectTree(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);

public:
    static void TreeUI();

};


#endif //TREE_ITREE_HPP