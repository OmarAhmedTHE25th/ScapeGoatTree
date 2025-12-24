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
void ITree::handleInsert(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B) {
    auto& tree = selectTree(A, B);
    ElemenType value;
    cout << "Enter value to insert: ";
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input.\n";
        return;;
    }
    tree.insert(value);
    cout << "Inserted " << value << endl;
}

void ITree::handleDelete(ScapeGoatTree<ElemenType>& A,ScapeGoatTree<ElemenType>& B) {
   auto& tree = selectTree(A,B);
    ElemenType value;
    cout << "Enter value to delete: ";
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input.\n";
        return;;
    }
   if ( tree.deleteValue(value))cout << "Deleted " << value << endl;
    cout << "Value Doesnt Exist\n";
}

void ITree::handleSearch(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B) {
    const auto& tree = selectTree(A, B);
    ElemenType value;
    cout << "Enter value to search: ";
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input.\n";
        return;;
    }
    (tree.search(value)
        ? cout <<  value << " FOUND in tree\n"
        :  cout << value << " NOT FOUND in tree\n");
}
void ITree::handleDisplayInOrder(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B) {
    auto& tree = selectTree(A, B);
    tree.displayInOrder();
    cout << "In-Order Traversal:\n";
    cout << tree.getDisplayBuffer() << endl;
}

void ITree::handleDisplayPreOrder(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B) {
    auto& tree = selectTree(A, B);
    tree.displayPreOrder();
    cout << "Pre-Order Traversal:\n";
    cout << tree.getDisplayBuffer() << endl;
}

void ITree::handleDisplayPostOrder(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B) {
    auto& tree = selectTree(A, B);
    tree.displayPostOrder();
    cout << "Post-Order Traversal:\n";
    cout << tree.getDisplayBuffer() << endl;
}

void ITree::handleBalance(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B) {
    const auto& tree = selectTree(A, B);
    cout << tree.isBalanced() << endl;
}
void ITree::handleOperatorInsert(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B) {
    auto& tree = selectTree(A, B);
    ElemenType value;
    cout << "Enter value to insert using += : ";
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input.\n";
        return;;
    }
    tree += value;
    cout << "Inserted using operator+= : " << value << endl;
}
void ITree::handleOperatorDelete(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B) {
    auto& tree = selectTree(A, B);
    ElemenType value;
    cout << "Enter value to delete using operators: ";
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input.\n";
        return;;
    }
    if (tree - value)cout << "Deleted using operator- and operator-=\n";
    cout << "Value Doesnt Exist\n";
}
void ITree::handleOperatorSearch(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B) {
    auto& tree = selectTree(A, B);
    ElemenType value;
    cout << "Enter value to search using operator[]: ";
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input.\n";
        return;;
    }
    cout << (tree[value]
        ? "FOUND using operator[]\n"
        : "NOT FOUND using operator[]\n");
}
void ITree::handleOperatorEmpty(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B) {
    auto& tree = selectTree(A, B);
    cout << (!tree
        ? "Tree is EMPTY (operator!)\n"
        : "Tree is NOT empty (operator!)\n");
}
void ITree::handleOperatorMerge(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B) {
    cout << "Tree 1: ";
    A.displayInOrder();
    cout << A.getDisplayBuffer() << endl;

    cout << "Tree 2: ";
    B.displayInOrder();
    cout << B.getDisplayBuffer() << endl;

    ScapeGoatTree<ElemenType> merged = A + B;

    cout << "Merged Tree: ";
    merged.displayInOrder();
    cout << merged.getDisplayBuffer() << endl;
}

void ITree::handleOperatorCompare(const ScapeGoatTree<ElemenType> &A, const ScapeGoatTree<ElemenType> &B) {
    cout << (A == B
        ? "Trees are EQUAL (operator==)\n"
        : "Trees are NOT equal\n");

    cout << (A != B
        ? "Trees are NOT equal after modification (operator!=)\n"
        : "Trees are still equal\n");
}

ScapeGoatTree<ElemenType>& ITree::selectTree(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B) {
    int choice;
    while (true) {
    cout << "Choose tree:\n";
    cout << "[1] Tree A\n";
    cout << "[2] Tree B\n";
    cout << "> ";
    cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {
            case 1:
                return A;
            case 2:
                return B;
            default:
                cout << "Invalid choice.\n";
        }
    }

}

void ITree::TreeUI() {
    ScapeGoatTree<ElemenType> tree;
    ScapeGoatTree<ElemenType> otherTree;
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
            case 1: handleInsert(tree,otherTree); break;
            case 2: handleDelete(tree, otherTree); break;
            case 3: handleSearch(tree, otherTree); break;
            case 4: handleDisplayInOrder(tree, otherTree); break;
            case 5: handleDisplayPreOrder(tree, otherTree); break;
            case 6: handleDisplayPostOrder(tree, otherTree); break;
            case 7: handleBalance(tree, otherTree); break;
            case 8: handleOperatorInsert(tree, otherTree); break;
            case 9: handleOperatorDelete(tree, otherTree); break;
            case 10: handleOperatorSearch(tree, otherTree); break;
            case 11: handleOperatorEmpty(tree, otherTree); break;
            case 12: handleOperatorMerge(tree, otherTree); break;
            case 13: handleOperatorCompare(tree, otherTree); break;
            default:
                cout << "Invalid operation number.\n";
        }
    }
}
