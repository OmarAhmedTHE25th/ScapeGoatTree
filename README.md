# Scapegoat Tree Implementation (C++ & Python)

A robust implementation of a **Scapegoat Tree**, a self-balancing Binary Search Tree (BST), written in C++ with Python bindings.

This project was built with a specific constraint: **Minimal STL usage**. Apart from `std::string` and `iostream`, custom container implementations (Vector, Queue) are used to manage memory and tree operations.

## 📝 Features

* **-Weight-Balanced:** Uses  to maintain height balance ().
* **Custom Containers:** Includes hand-written `Vector` and `Queue` classes to replace standard STL containers.
* **Python Bindings:** Fully exposed API using `pybind11` for seamless integration with Python scripts and UIs.
* **Batch Operations:** Support for inserting and deleting values in batch for efficiency.
* **Tree Traversal & Visualization:** Built-in methods for In-Order, Pre-Order, Post-Order, and Level-Order traversals, returned as buffered strings.
* **Balance Reporting:** Detailed `isBalanced()` report checking height vs. theoretical bounds.

## 🛠️ Prerequisites

* **C++ Compiler:** (GCC, Clang, or MSVC) supporting C++11 or later.
* **CMake:** For building the project.
* **Python 3.x:** Required for using the bindings.
* **Pybind11:** Required to compile the Python module.

## 📂 Project Structure

* **Core C++ Logic**
* `ScapeGoatTree.hpp` / `.tpp`: Main tree implementation.
* `Node.hpp`: Tree node structure.
* `vector.hpp`: Custom dynamic array implementation.
* `queue.hpp` / `.tpp`: Custom queue implementation for level-order traversal.


* **Bindings**
* `bindings.cpp`: Pybind11 glue code exposing the C++ class to Python.



## 🚀 Build Instructions

This project uses CMake. To build the shared library (Python extension):

```bash
mkdir build
cd build
cmake ..
make

```

*Note: On Windows, the output file will be something like `scapegoat_tree_py.cp3x-win_amd64.pyd`. On Linux/Mac, it will be a `.so` file.*

## 💻 Usage

### 1. Python (via Bindings)

Once built, ensure the generated `.pyd` or `.so` file is in your Python path.

```python
import scapegoat_tree_py as sgt

# Initialize the tree
tree = sgt.ScapeGoatTree()

# Insert specific values
tree.insert(50)
tree.insert(30)
tree.insert(70)

# Batch operations
tree.insert_batch([20, 40, 60, 80])

# Check balance
print(tree.get_balance_report())

# Display traversals
print("In-Order:", tree.get_inorder())
print("Level-Order:\n", tree.get_levels())

# Search
if tree.search(40):
    print("Found 40!")

# Delete
tree.delete_value(20)
print("After deletion:", tree.get_inorder())

```

### 2. C++ (Direct Usage)

```cpp
#include "ScapeGoatTree.hpp"
#include <iostream>

int main() {
    ScapeGoatTree<int> tree;
    
    // Insertions
    tree.insert(100);
    tree.insert(50);
    tree.insert(150);
    
    // Check if balanced
    std::cout << tree.isBalanced() << std::endl;
    
    // Display
    tree.displayInOrder();
    std::cout << tree.getDisplayBuffer() << std::endl;
    
    // Custom Operators
    tree + 200;       // Insert 200
    tree - 50;        // Delete 50
    
    return 0;
}

```

## ⚠️ Implementation Notes

* **Memory Management:** This implementation manually manages memory for nodes and auxiliary arrays.
* **Mutable Display Buffer:** The `get_inorder` (and similar) methods update an internal string buffer. The string is returned by reference in C++ (and value in Python) to minimize overhead during frequent UI updates.
* **Operator Overloading:** * `tree + value`: Insert
* `tree -= value`: Delete
* `tree[value]`: Search (returns bool)



## 📄 License

This project is open-source. Feel free to use and modify.
