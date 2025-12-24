//
// Created by DELL on 24/12/2025.
//

#ifndef TREE_ITREE_HPP
#define TREE_ITREE_HPP
#include "ScapeGoatTree.hpp"


class ITree {

    static void printMenu();

    static void handleInsert(ScapeGoatTree<int>& tree);
    static void handleDelete(ScapeGoatTree<int>& tree);

    static void handleSearch(const ScapeGoatTree<int>& tree);
    static void handleDisplayInOrder(ScapeGoatTree<int>& tree);
    static void handleDisplayPreOrder(ScapeGoatTree<int>& tree);
    static void handleDisplayPostOrder(ScapeGoatTree<int>& tree);
    static void handleBalance(const ScapeGoatTree<int>& tree);

    // Operator showcase
    static void handleOperatorInsert(ScapeGoatTree<int>& tree);
    static void handleOperatorDelete(ScapeGoatTree<int>& tree);
    static void handleOperatorSearch(const ScapeGoatTree<int>& tree);
    static void handleOperatorEmpty(const ScapeGoatTree<int>& tree);
    static void handleOperatorMerge();
    static void handleOperatorCompare(const ScapeGoatTree<int>& tree);
public:
    static void TreeUI();

};


#endif //TREE_ITREE_HPP