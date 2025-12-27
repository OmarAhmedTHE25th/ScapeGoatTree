#include "iTree.hpp"
#include <limits>
#include <iostream>
using namespace std;

/* ===================== ANSI Colors ===================== */
const string RED    = "\033[31m";
const string GREEN  = "\033[32m";
const string CYAN   = "\033[36m";
const string RESET  = "\033[0m";
constexpr string WHITE = "\033[37m";
constexpr string  BLUE  =  "\033[34m";
constexpr string  MAGENTA= "\033[35m";
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

    cout << CYAN << "| " << WHITE << "1 - Insert                          " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE << "2 - Insert Batch                    " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE << "3 - Delete                          " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE << "4 - Delete Batch                    " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE << "5 - Search                          " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE << "6 - Display In-Order                " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE << "7 - Display Pre-Order               " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE << "8 - Display Post-Order              " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE << "9 - Display Level-Order             " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE << "10 - Check Balance / Stats          " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE << "11 - Operator Insert (+=)           " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE << "12 - Operator Delete (-, -=)        " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE << "13 - Operator Search ([])           " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE << "14 - Operator Empty (!)             " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE << "15 - Operator Merge (+)             " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << WHITE << "16 - Operator Compare (==, !=)      " << CYAN << " |\n" << RESET;
    cout << CYAN << "| " << RED   << "17 - Exit                           " << CYAN << " |\n" << RESET;

    cout << CYAN << "+--------------------------------------+\n" << RESET;
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

void ITree::handleInsert(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    auto& tree = selectTree(A, B);
    ElemenType value;

    cout << "Enter value to insert: ";
    cin >> value;

    if (!validateCinLine())return;

    tree.insert(value);
    printSuccess("SUCCESS: Inserted " + to_string(value));
}

void ITree::handleInsertBatch(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    auto& tree = selectTree(A, B);
    ElemenType stop;

    while (true) {
        cout << "Enter stop value: ";
        cin >> stop;
        if (validateCinLine())break;
    }

    cout << "Enter values:\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    tree.insertBatch(cin, stop);
    printSuccess("SUCCESS: Batch insertion complete.");
}

void ITree::handleDelete(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    auto& tree = selectTree(A, B);
    ElemenType value;

    cout << "Enter value to delete: ";
    cin >> value;

    if (!validateCinLine())return;

    if (tree.deleteValue(value))
        printSuccess("SUCCESS: Deleted " + to_string(value));
    else
        printInfo("INFO: Value does not exist.");
}

void ITree::handleDeleteBatch(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    auto& tree = selectTree(A, B);
    ElemenType stop;

    while (true) {
        cout << "Enter stop value: ";
        cin >> stop;
        if (validateCinLine())break;
    }

    cout << "Enter values:\n";
    tree.deleteBatch(cin, stop);
    printSuccess("SUCCESS: Batch deletion complete.");
}

void ITree::handleSearch(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    const auto& tree = selectTree(A, B);
    ElemenType value;

    cout << "Enter value to search: ";
    cin >> value;

    if (!validateCinLine())return;

    if (tree.search(value))
        printSuccess("RESULT: FOUND");
    else
        printError("RESULT: NOT FOUND");
}

/* ===================== Display ===================== */
void ITree::handleDisplayInOrder(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    auto& tree = selectTree(A, B);
    tree.displayInOrder();

    printInfo("\n--- In-Order Traversal ---");
    cout << tree.getDisplayBuffer() << "\n";
}

void ITree::handleDisplayPreOrder(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    auto& tree = selectTree(A, B);
    tree.displayPreOrder();

    printInfo("\n--- Pre-Order Traversal ---");
    cout << tree.getDisplayBuffer() << "\n";
}

void ITree::handleDisplayPostOrder(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    auto& tree = selectTree(A, B);
    tree.displayPostOrder();

    printInfo("\n--- Post-Order Traversal ---");
    cout << tree.getDisplayBuffer() << "\n";
}

void ITree::handleDisplayLevels(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    auto& tree = selectTree(A, B);
    tree.displayLevels();

    printInfo("\n--- Level-Order Traversal ---");
    cout << tree.getDisplayBuffer() << "\n";
}

/* ===================== Operators ===================== */
void ITree::handleBalance(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    const auto& tree = selectTree(A, B);
    cout <<tree.isBalanced() << "\n";
}
void ITree::handleOperatorInsert(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    auto& tree = selectTree(A, B);
    ElemenType value;

    cout << "Enter value: ";
    cin >> value;

    if (!validateCinLine())return;

    tree += value;
    printSuccess("SUCCESS: Inserted using +=");
}

void ITree::handleOperatorDelete(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    auto& tree = selectTree(A, B);
    ElemenType value;

    cout << "Enter value: ";
    cin >> value;

    if (!validateCinLine())return;

    if (tree - value)
        printSuccess("SUCCESS: Deleted using operator");
    else
        printInfo("INFO: Value does not exist.");
}

void ITree::handleOperatorSearch(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    auto& tree = selectTree(A, B);
    ElemenType value;

    cout << "Enter value: ";
    cin >> value;

    if (!validateCinLine())return;

    if (tree[value])
        printSuccess("FOUND");
    else
        printError("NOT FOUND");
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
            case 1: handleInsert(treeA, treeB); break;
            case 2: handleInsertBatch(treeA, treeB); break;
            case 3: handleDelete(treeA, treeB); break;
            case 4: handleDeleteBatch(treeA, treeB); break;
            case 5: handleSearch(treeA, treeB); break;
            case 6: handleDisplayInOrder(treeA, treeB); break;
            case 7: handleDisplayPreOrder(treeA, treeB); break;
            case 8: handleDisplayPostOrder(treeA, treeB); break;
            case 9: handleDisplayLevels(treeA, treeB); break;
            case 10: handleBalance(treeA, treeB); break;
            case 11: handleOperatorInsert(treeA, treeB); break;
            case 12: handleOperatorDelete(treeA, treeB); break;
            case 13: handleOperatorSearch(treeA, treeB); break;
            case 14: handleOperatorEmpty(treeA, treeB); break;
            case 15: handleOperatorMerge(treeA, treeB); break;
            case 16: handleOperatorCompare(treeA, treeB); break;
            default:
                printError("ERROR: Invalid operation.");
        }
    }
}
