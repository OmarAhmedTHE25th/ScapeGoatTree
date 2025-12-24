//
// Created by DELL on 24/12/2025.
//

#include "iTree.hpp"
void ITree::printMenu() {
    cout << "\nChoose your operation:\n";
    cout << "[1] Insert\n";
    cout << "[2] Delete\n";
    cout << "[3] Search\n";
    cout << "[4] Display In-Order\n";
    cout << "[5] Display Pre-Order\n";
    cout << "[6] Display Post-Order\n";
    cout << "[7] Check Balance / Stats\n";
    cout << "[8] Operator: Insert (+=)\n";
    cout << "[9] Operator: Delete (-, -=)\n";
    cout << "[10] Operator: Search ([])\n";
    cout << "[11] Operator: Empty Check (!)\n";
    cout << "[12] Operator: Merge Trees (+)\n";
    cout << "[13] Operator: Compare Trees (==, !=)\n";
    cout << "[14] Exit\n";
    cout << "> ";
}
void ITree::handleInsert(ScapeGoatTree<int>& tree) {
    int value;
    cout << "Enter value to insert: ";
    cin >> value;
    tree.insert(value);
    cout << "Inserted " << value << endl;
}

void ITree::handleDelete(ScapeGoatTree<int>& tree) {
    int value;
    cout << "Enter value to delete: ";
    cin >> value;
    tree.deleteValue(value);
    cout << "Deleted " << value << endl;
}

void ITree::handleSearch(const ScapeGoatTree<int>& tree) {
    int value;
    cout << "Enter value to search: ";
    cin >> value;
    cout << (tree.search(value)
        ? to_string(value) + " FOUND in tree\n"
        : to_string(value) + " NOT FOUND in tree\n");
}
void ITree::handleDisplayInOrder(ScapeGoatTree<int>& tree) {
    tree.displayInOrder();
    cout << "In-Order Traversal:\n";
    cout << tree.getDisplayBuffer() << endl;
}

void ITree::handleDisplayPreOrder(ScapeGoatTree<int>& tree) {
    tree.displayPreOrder();
    cout << "Pre-Order Traversal:\n";
    cout << tree.getDisplayBuffer() << endl;
}

void ITree::handleDisplayPostOrder(ScapeGoatTree<int>& tree) {
    tree.displayPostOrder();
    cout << "Post-Order Traversal:\n";
    cout << tree.getDisplayBuffer() << endl;
}

void ITree::handleBalance(const ScapeGoatTree<int>& tree) {
    cout << tree.isBalanced() << endl;
}
void ITree::handleOperatorInsert(ScapeGoatTree<int>& tree) {
    int value;
    cout << "Enter value to insert using += : ";
    cin >> value;
    tree += value;
    cout << "Inserted using operator+= : " << value << endl;
}
void ITree::handleOperatorDelete(ScapeGoatTree<int>& tree) {
    int value;
    cout << "Enter value to delete using operators: ";
    cin >> value;
    tree - value;
    tree -= value;
    cout << "Deleted using operator- and operator-=\n";
}
void ITree::handleOperatorSearch(const ScapeGoatTree<int>& tree) {
    int value;
    cout << "Enter value to search using operator[]: ";
    cin >> value;
    cout << (tree[value]
        ? "FOUND using operator[]\n"
        : "NOT FOUND using operator[]\n");
}
void ITree::handleOperatorEmpty(const ScapeGoatTree<int>& tree) {
    cout << (!tree
        ? "Tree is EMPTY (operator!)\n"
        : "Tree is NOT empty (operator!)\n");
}
void ITree::handleOperatorMerge() {
    ScapeGoatTree<int> t1, t2;

    t1.insert(10);
    t1.insert(5);
    t1.insert(20);

    t2.insert(15);
    t2.insert(3);
    t2.insert(25);

    cout << "Tree 1: ";
    t1.displayInOrder();
    cout << t1.getDisplayBuffer() << endl;

    cout << "Tree 2: ";
    t2.displayInOrder();
    cout << t2.getDisplayBuffer() << endl;

    ScapeGoatTree<int> merged = t1 + t2;

    cout << "Merged Tree: ";
    merged.displayInOrder();
    cout << merged.getDisplayBuffer() << endl;
}
void ITree::handleOperatorCompare(const ScapeGoatTree<int>& tree) {
    ScapeGoatTree<int> other = tree;

    cout << (tree == other
        ? "Trees are EQUAL (operator==)\n"
        : "Trees are NOT equal\n");

    other.insert(999);

    cout << (tree != other
        ? "Trees are NOT equal after modification (operator!=)\n"
        : "Trees are still equal\n");
}
void ITree::TreeUI() {
    ScapeGoatTree<int> tree;
    int op;

    cout << "========================================\n";
    cout << "      ScapeGoat Tree User Interface      \n";
    cout << "========================================\n";

    while (true) {
        printMenu();
        cin >> op;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }

        if (op == 14) {
            cout << "Exiting... Goodbye!\n";
            break;
        }

        switch (op) {
            case 1: handleInsert(tree); break;
            case 2: handleDelete(tree); break;
            case 3: handleSearch(tree); break;
            case 4: handleDisplayInOrder(tree); break;
            case 5: handleDisplayPreOrder(tree); break;
            case 6: handleDisplayPostOrder(tree); break;
            case 7: handleBalance(tree); break;
            case 8: handleOperatorInsert(tree); break;
            case 9: handleOperatorDelete(tree); break;
            case 10: handleOperatorSearch(tree); break;
            case 11: handleOperatorEmpty(tree); break;
            case 12: handleOperatorMerge(); break;
            case 13: handleOperatorCompare(tree); break;
            default:
                cout << "Invalid operation number.\n";
        }
    }
}
