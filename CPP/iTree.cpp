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




/* ===================== Tree Selection ===================== */
/**
 * Prompts the user to select one of the two available trees (Tree A or Tree B).
 */
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


/**
 * Handles batch insertion and deletion operations.
 */
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
    if (op==opcodes::INSERT) {
        tree.insertBatch(values);
        printSuccess("SUCCESS: Batch insertion complete.");
    }
    if (op==opcodes::DELETEOP){
        tree.deleteBatch(values);
        printSuccess("SUCCESS: Batch Deletion complete.");
    }
}
/**
 * Handles standard tree operations like search.
 */
void ITree::handleOperations(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B, const opcodes op) {
     auto& tree = selectTree(A, B);
    ElemenType value;

    cout << "Enter value: ";
    cin >> value;
    if (!validateCinLine())return;
    switch (op) {
        case opcodes::INSERT:
        {    tree.insert(value);
            printSuccess("SUCCESS: Inserted " + to_string(value));
        }            break;
        case opcodes::DELETEOP: {
            if (tree.deleteValue(value))
                printSuccess("SUCCESS: Deleted " + to_string(value));
            else
                printInfo("INFO: Value does not exist.");
        }
            break;
        case opcodes::SEARCH: {
            if (tree.search(value))
                printSuccess("RESULT: FOUND");
            else
                printError("RESULT: NOT FOUND");
        }
        default: ;
    }
}
/* ===================== Display ===================== */
/**
 * Handles display operations for the trees.
 */
void ITree::handleDisplay(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B,const opcodes op) {
    auto& tree = selectTree(A, B);
    switch (op)
        {
        case opcodes::DISPLAY_INORDER: {
            printInfo("\n--- In-Order Traversal ---");
            cout << tree.displayInOrder() << "\n";
        }
            break;
        case opcodes::DISPLAY_PREORDER: {
            printInfo("\n--- Pre-Order Traversal ---");
            cout << tree.displayPreOrder() << "\n";
        }
            break;
        case opcodes::DISPLAY_POSTORDER: {
            printInfo("\n--- Post-Order Traversal ---");
            cout << tree.displayPostOrder() << "\n";
        }
        break;
        case opcodes::DISPLAY_LEVELS: {
            printInfo("\n--- Level-Order Traversal ---");
            cout << tree.displayLevels() << "\n";
        }
            default: ;
        }

}

/* ===================== Operators ===================== */
/**
 * Handles checking and reporting the balance status of the trees.
 */
void ITree::handleBalance(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    const auto& tree = selectTree(A, B);
    cout <<tree.isBalanced() << "\n";
}
/**
 * Handles checking if the trees are empty.
 */
void ITree::handleOperatorEmpty(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    const auto& tree = selectTree(A, B);
    if (!tree) printInfo("Tree is EMPTY");
    else printInfo("Tree is NOT empty");
}

/**
 * Handles merging two trees together using the + operator.
 */
void ITree::handleOperatorMerge(ScapeGoatTree<ElemenType>& A, ScapeGoatTree<ElemenType>& B) {
    printInfo("\nTree A:");
    cout << A.displayInOrder() << "\n";

    printInfo("Tree B:");
    cout << B.displayInOrder() << "\n";

    ScapeGoatTree<ElemenType> merged = A + B;

    printInfo("Merged Tree:");
    cout << merged.displayInOrder() << "\n";
}

/**
 * Handles comparing two trees for equality using the == operator.
 */
void ITree::handleOperatorCompare(const ScapeGoatTree<ElemenType>& A, const ScapeGoatTree<ElemenType>& B) {
    if (A == B)
        printSuccess("Trees are EQUAL");
    else
        printError("Trees are NOT equal");
}

/**
 * Handles core operators like insertion and deletion using overloaded + and - operators.
 */
void ITree::handleCoreOperators(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B, const opcodes op) {
    auto& tree = selectTree(A, B);
    ElemenType value;

    cout << "Enter value: ";
    cin >> value;

    if (!validateCinLine())return;
    switch (op) {
        case opcodes::INSERT:{
            tree + value;
            printSuccess("SUCCESS: Inserted " + to_string(value) + " using operator +");
        }break;
        case opcodes::DELETEOP:{
            if (tree - value)
                printSuccess("SUCCESS: Deleted " + to_string(value) + " using operator -");
            else
                printInfo("INFO: Value does not exist.");
        } break;
        case opcodes::SEARCH:{
            if (tree[value])
                printSuccess("RESULT: FOUND using operator []");
            else
                printError("RESULT: NOT FOUND using operator []");
        } break;

        default: ;
    }
    }

/**
 * Handles clearing the contents of a selected tree.
 */
void ITree::handleClear(ScapeGoatTree<ElemenType> &A, ScapeGoatTree<ElemenType> &B) {
    auto& tree = selectTree(A, B);
    tree = 0;
    printSuccess("SUCCESS: Tree cleared using operator = 0");
}

/* ===================== Main UI ===================== */

typedef void (*MenuHandler)(ScapeGoatTree<ElemenType>&, ScapeGoatTree<ElemenType>&, opcodes);
typedef const unsigned long long Long;
struct MenuItem {
    string name;
    opcodes opcode;
    MenuHandler func;
};
/**
 * Launches the terminal-based user interface for interacting with the trees.
 */
void ITree::TreeUI() {
    ScapeGoatTree<ElemenType> treeA;
    ScapeGoatTree<ElemenType> treeB;

    const MenuItem menu[] = {
        {"Insert",              opcodes::INSERT,           handleOperations},
        {"Insert Batch",        opcodes::INSERT,           handleBatches},
        {"Delete",              opcodes::DELETEOP,           handleOperations},
        {"Delete Batch",        opcodes::DELETEOP,           handleBatches},
        {"Search",              opcodes::SEARCH,           handleOperations},
        {"Display In-Order",    opcodes::DISPLAY_INORDER,  handleDisplay},
        {"Display Pre-Order",   opcodes::DISPLAY_PREORDER, handleDisplay},
        {"Display Post-Order",  opcodes::DISPLAY_POSTORDER,handleDisplay},
        {"Display Level-Order", opcodes::DISPLAY_LEVELS,   handleDisplay},
        {"Check Balance",       opcodes::BALANCE,          [](auto& A, auto& B, auto ){ handleBalance(A, B); }},
        {"Operator Insert",     opcodes::INSERT,           handleCoreOperators},
        {"Operator Delete",     opcodes::DELETEOP,           handleCoreOperators},
        {"Operator Search",     opcodes::SEARCH,           handleCoreOperators},
        {"Operator Empty",      opcodes::EMPTY,            [](auto& A, auto& B, auto){ handleOperatorEmpty(A, B); }},
        {"Operator Merge",      opcodes::MERGE,            [](auto& A, auto& B, auto){ handleOperatorMerge(A, B); }},
        {"Operator Compare",    opcodes::COMPARE,          [](auto& A, auto& B, auto){ handleOperatorCompare(A, B); }},
        {"Operator Clear",      opcodes::CLEAR,            [](auto& A, auto& B, auto ){handleClear(A,B);}}
    };

    constexpr int menuSize = std::size(menu);

    while (true) {
        constexpr int TOTAL_WIDTH = 40;
        printHeader("ScapeGoat Tree Interface");

        for (int i = 0; i < menuSize; ++i) {
            // 1. Prepare the text components
            string prefix = "| " + to_string(i + 1) + " - ";
            string name = menu[i].name;

            // 2. Calculate spaces needed (Total 40 - prefix - name - 1 for closing pipe)
            Long currentLen = prefix.length() + name.length();
            Long spacesNeeded = (TOTAL_WIDTH - 1) - currentLen;

            // 3. Print with colors
            cout << CYAN << "| " << WHITE << (i + 1) << " - " << name; // Cyan border, White text
            for(int s = 0; s < spacesNeeded; ++s) cout << " ";        // Padding
            cout << CYAN << "|" << RESET << "\n";                    // Cyan closing border
        }

        // Handle the Exit line
        string exitPrefix = "| " + to_string(menuSize + 1) + " - ";
        string exitName = "Exit";
        Long exitSpaces = (TOTAL_WIDTH - 1) - (exitPrefix.length() + exitName.length());

        cout << CYAN << "| " << RED << (menuSize + 1) << " - Exit";
        for(int s = 0; s < exitSpaces; ++s) cout << " ";
        cout << CYAN << "|" << RESET << "\n";

        cout << CYAN << "+======================================+" << RESET << "\n";
        cout << "Select an operation: ";

        int op;
        cin >> op;

        if (!validateCinLine()) continue;
        if (op == menuSize + 1) {
            printInfo("\nExiting...");
            break;
        }

        if (op >= 1 && op <= menuSize) {
            menu[op - 1].func(treeA, treeB, menu[op - 1].opcode);
        } else {
            printError("ERROR: Invalid operation.");
        }
    }
}
