#include "iTree.hpp"
#include <limits>
#include <iostream>
using namespace std;

/* ===================== ANSI Colors ===================== */
const string RED    = "\033[31m";
const string GREEN  = "\033[32m";
const string CYAN   = "\033[36m";
const string RESET  = "\033[0m";
string WHITE = "\033[37m";
string  BLUE  =  "\033[34m";
string  MAGENTA= "\033[35m";
void printError(const string& msg)   { cout << RED << msg << RESET << "\n"; }
void printSuccess(const string& msg) { cout << GREEN << msg << RESET << "\n\n"; }
void printInfo(const string& msg)    { cout << CYAN << msg << RESET << "\n"; }
/* ===================== Utility ===================== */
void printHeader(const std::string& title) {
    cout << CYAN << "+======================================+" << RESET << "\n";
    cout << CYAN << "| " << RESET << GREEN << title << RESET << CYAN << "             |" << RESET << "\n";
    cout << CYAN << "+======================================+" << RESET << "\n";
}

bool validateCinLine()
{
    if (std::cin.fail() || std::cin.peek() != '\n')
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        printError("ERROR: Invalid input.");
        return false;
    }
    return true;
}

/* ===================== Menu ===================== */


void ITree::printMenu() {
    cout << CYAN << "| " << GREEN << "MAIN MENU" << CYAN << "                            |\n" << RESET;
    cout << CYAN << "+======================================+" << RESET << "\n";
int i =0;
    cout << CYAN << "| " << WHITE<<++i << " - Insert                          " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE<<++i << " - Insert Batch                    " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE<<++i << " - Delete                          " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE<<++i << " - Delete Batch                    " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE<<++i << " - Search                          " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE<<++i << " - Display In-Order                " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE<<++i << " - Display Pre-Order               " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE<<++i << " - Display Post-Order              " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE<<++i << " - Display Level-Order             " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE<<++i << " - Check Balance / Stats          " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE<<++i << " - Operator Insert (+=)           " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE<<++i << " - Operator Delete (-, -=)        " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE<<++i << " - Operator Search ([])           " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE<<++i << " - Operator Empty (!)             " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE<<++i << " - Operator Merge (+)             " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE<<++i << " - Operator Compare (==, !=)      " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << RED<<++i  << " - Exit                           " << CYAN << " |\n" << RESET;
    cout << CYAN << "+======================================+" << RESET << "\n";
    cout << "Select an option (1-17)\n";
    cout << "> ";
}



/* ===================== Tree Selection ===================== */
ScapeGoatTree<ElemenType>& ITree::selectTree(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    int choice;
    while (true) {
        cout << "\nSelect Tree:\n";
        cout << "  [1] Tree A\n";
        cout << "  [2] Tree B\n";
        cout << "> ";
        cin >> choice;

     if (!validateCinLine())continue;

        if (choice == 1) return A;
        if (choice == 2) return B;

        printError("ERROR: Invalid choice.");
    }
}

/* ===================== Handlers ===================== */


void ITree::handleBatches(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B, const opcodes op) {
    auto& tree = selectTree(A, B);
    ElemenType stop;

    while (true) {
        cout << "Enter stop value: ";
        cin >> stop;
        if (validateCinLine())break;
    }

    cout << "Enter values:\n";
    Vector<ElemenType> values;
    while (true) {
        ElemenType value;
        cout<< "> ";
        cin >> value;
        if (!validateCinLine())continue;
        if (value == stop) break;
        values.push_back(value);
    }
    if (op==INSERT) {
        tree.insertBatch(values);
        printSuccess("SUCCESS: Batch insertion complete.");
    }
    if (op==DELETE_op){
        tree.deleteBatch(values);
        printSuccess("SUCCESS: Batch Deletion complete.");
    }
}
void ITree::handleOperations(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B, const opcodes op) {
     auto& tree = selectTree(A, B);
    ElemenType value;

    cout << "Enter value: ";
    cin >> value;
    if (!validateCinLine())return;
    switch (op) {
        case INSERT:
        {    tree.insert(value);
            printSuccess("SUCCESS: Inserted " + to_string(value));
        }            break;
        case DELETE_op: {
            if (tree.deleteValue(value))
                printSuccess("SUCCESS: Deleted " + to_string(value));
            else
                printInfo("INFO: Value does not exist.");
        }
            break;
        case SEARCH: {
            if (tree.search(value))
                printSuccess("RESULT: FOUND");
            else
                printError("RESULT: NOT FOUND");
        }
        default: ;
    }
}
/* ===================== Display ===================== */
void ITree::handleDisplay(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B,const opcodes op) {
    auto& tree = selectTree(A, B);
    switch (op)
        {
        case DISPLAY_INORDER: {
            tree.displayInOrder();
            printInfo("\n--- In-Order Traversal ---");
            cout << tree.getDisplayBuffer() << "\n";
        }
            break;
        case DISPLAY_PREORDER: {
            tree.displayPreOrder();
            printInfo("\n--- Pre-Order Traversal ---");
            cout << tree.getDisplayBuffer() << "\n";
        }
            break;
        case DISPLAY_POSTORDER: {
            tree.displayPostOrder();
            printInfo("\n--- Post-Order Traversal ---");
            cout << tree.getDisplayBuffer() << "\n";
        }
        break;
        case DISPLAY_LEVELS: {
            tree.displayLevels();
            printInfo("\n--- Level-Order Traversal ---");
            cout << tree.getDisplayBuffer() << "\n";
        }
            default: ;
        }

}

/* ===================== Operators ===================== */
void ITree::handleBalance(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    const auto& tree = selectTree(A, B);
    cout <<tree.isBalanced() << "\n";
}
void ITree::handleOperatorEmpty(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    const auto& tree = selectTree(A, B);
    if (!tree) printInfo("Tree is EMPTY");
    else printInfo("Tree is NOT empty");
}

void ITree::handleOperatorMerge(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    printInfo("\nTree A:");
    A.displayInOrder();
    cout << A.getDisplayBuffer() << "\n";

    printInfo("Tree B:");
    B.displayInOrder();
    cout << B.getDisplayBuffer() << "\n";

    ScapeGoatTree<ElemenType> merged = A + B;

    printInfo("Merged Tree:");
    merged.displayInOrder();
    cout << merged.getDisplayBuffer() << "\n";
}

void ITree::handleOperatorCompare(const ScapeGoatTree<ElemenType>& A, const ScapeGoatTree<ElemenType>& B) {
    if (A == B)
        printSuccess("Trees are EQUAL");
    else
        printError("Trees are NOT equal");
}

void ITree::handleCoreOperators(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B, opcodes op) {
    auto& tree = selectTree(A, B);
    ElemenType value;

    cout << "Enter value: ";
    cin >> value;

    if (!validateCinLine())return;
    switch (op) {
        case INSERT:{
            tree + value;
            printSuccess("SUCCESS: Inserted " + to_string(value) + " using operator +");
        }break;
        case DELETE_op:{
            if (tree - value)
                printSuccess("SUCCESS: Deleted " + to_string(value) + " using operator -");
            else
                printInfo("INFO: Value does not exist.");
        } break;
        case SEARCH:{
            if (tree[value])
                printSuccess("RESULT: FOUND using operator []");
            else
                printError("RESULT: NOT FOUND using operator []");
        } break;
        default: ;
    }
    }


/* ===================== Main UI ===================== */
void ITree::TreeUI() {
    ScapeGoatTree<ElemenType> treeA;
    ScapeGoatTree<ElemenType> treeB;
    int op;

    while (true) {
        printHeader("ScapeGoat Tree Interface");
        printMenu();
        cout << "Select an operation: ";
        cin >> op;

        if (!validateCinLine())continue;

        if (op == 17) {
            printInfo("\nExiting...");
            break;
        }

        switch (op) {
            case 1: handleOperations(treeA, treeB, INSERT); break;
            case 2: handleBatches(treeA, treeB,INSERT); break;
            case 3: handleOperations(treeA, treeB,DELETE_op); break;
            case 4: handleBatches(treeA, treeB,DELETE_op); break;
            case 5: handleOperations(treeA, treeB,SEARCH); break;
            case 6: handleDisplay(treeA, treeB,DISPLAY_INORDER); break;
            case 7: handleDisplay(treeA, treeB,DISPLAY_PREORDER); break;
            case 8: handleDisplay(treeA, treeB,DISPLAY_POSTORDER); break;
            case 9: handleDisplay(treeA, treeB,DISPLAY_LEVELS); break;
            case 10: handleBalance(treeA, treeB); break;
            case 11: handleCoreOperators(treeA, treeB,INSERT); break;
            case 12: handleCoreOperators(treeA, treeB,DELETE_op); break;
            case 13: handleCoreOperators(treeA, treeB,SEARCH); break;
            case 14: handleOperatorEmpty(treeA, treeB); break;
            case 15: handleOperatorMerge(treeA, treeB); break;
            case 16: handleOperatorCompare(treeA, treeB); break;
            default:
                printError("ERROR: Invalid operation.");
        }
    }
}
