//
// Created by DELL on 24/12/2025.
//

#ifndef TREE_ITREE_HPP
#define TREE_ITREE_HPP
#include "ScapeGoatTree.hpp"
#include "string"
using namespace std;
typedef int ElemenType;
class ITree {

    static void printMenu();

    static void handleInsert(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);
    static void handleDelete(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);

    static void handleSearch(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);
    static void handleDisplayInOrder(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);
    static void handleDisplayPreOrder(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);
    static void handleDisplayPostOrder(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);
    static void handleBalance(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);
    static void handleDisplayLevels(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);

    // Operator showcase
    static void handleOperatorInsert(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);
    static void handleOperatorDelete(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);
    static void handleOperatorSearch(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);
    static void handleOperatorEmpty(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);
    static void handleOperatorMerge(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);
    static void handleOperatorCompare(const ScapeGoatTree<ElemenType> &A, const ScapeGoatTree<ElemenType> &B);

    static ScapeGoatTree<ElemenType>& selectTree(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B);

public:
    static void TreeUI();

};


#endif //TREE_ITREE_HPP