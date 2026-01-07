# Scapegoat Tree Implementation

### A Self-Balancing BST with Multi-Platform Support

**Academic Project**: Data Structures Course
**Language**: C++20 with Python Bindings
**Constraint**: Minimal STL Usage (Custom Containers)

**Note**: This is the most comprehensive publicly available Scapegoat Tree project, including multiple user interfaces, Python bindings, advanced operations, and extensive testing.

---

## Quick Start

### Running the Python GUI (Cross-Platform)

```bash
# 1. Build the project
mkdir build && cd build
cmake .. && make

# 2. Run the visualizer
python ../py.py
```

### Running the C++ Terminal UI

```bash
# After building, run the executable
./RunTU   # Terminal-based User Interface
```

### Running via Docker

The provided Dockerfile runs the Terminal UI directly:

```bash
docker build -t scapegoat .
docker run -it --rm scapegoat
```

---

## Features

### Core Tree Features

* α-Weight-Balanced Scapegoat Tree
* Automatic height-balanced rebalancing
* Supports insert, delete, search
* Batch operations for efficiency
* Undo/Redo system
* Tree merging with duplicate handling
* Operator overloading for intuitive syntax

### Custom Data Structures

* Vector: Dynamic array, automatic resizing, minimal memory overhead
* Queue: Singly-linked list for level-order traversal
* Stack: Built on Vector, used for undo/redo

### User Interfaces

* Terminal UI (TUI) with color-coded menus
* Python Tkinter GUI
* DirectX 11 + ImGui GUI (Windows only)

### Advanced Usage

* Cross-language Python bindings via pybind11
* Custom α parameter for tree balancing
* Detailed balance checking and traversal outputs

### Testing

* Comprehensive unit tests covering:

  * Basic operations and edge cases
  * Automatic rebalancing
  * Operator overloading
  * Undo/Redo system
  * Batch operations
  * Copy and move semantics

---

## Algorithm Overview

A Scapegoat Tree is a self-balancing BST that maintains balance through periodic rebuilding:

* **α-weight-balanced**: No subtree can exceed α × parent’s size (α = 2/3)
* **Height bound**: h ≤ log₁.₅(n), where n = number of nodes
* **Lazy rebalancing**: Rebuilds only when balance is violated

**Advantages**:

* Simpler than AVL or Red-Black trees (no color/height metadata)
* Amortized efficiency: rebuilds are rare, fast average-case operations
* Space-efficient: minimal per-node overhead

### Complexity Analysis

| Operation | Time Complexity     | Space                |
| --------- | ------------------- | -------------------- |
| Search    | O(log n) worst-case | O(1)                 |
| Insert    | O(log n) amortized  | O(1)                 |
| Delete    | O(log n) amortized  | O(1)                 |
| Rebuild   | O(n) occasional     | O(n) temporary       |
| Traversal | O(n)                | O(n) for level-order |

---

## Prerequisites

### Required

* C++ Compiler: GCC 9+, Clang 10+, or MSVC 2019+
* CMake 3.15+
* Python 3.7+
* pybind11 (`pip install pybind11`)

### Optional

* DirectX 11 SDK (for Windows GUI)
* Tkinter (usually included with Python)

---

## Installation

### Windows

```bash
pip install pybind11

mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### Linux/Mac

```bash
pip install pybind11

mkdir build && cd build
cmake ..
make
```

---

## Usage Examples

### Python Interface

```python
import scapegoat_tree_py as sgt

tree = sgt.ScapeGoatTree()
tree.insert_batch([10, 20, 30])
tree.undo()
tree.redo()

tree2 = sgt.ScapeGoatTree()
tree2.insert_batch([5, 15])
merged = tree + tree2
```

### C++ Interface

```cpp
ScapeGoatTree<int> tree;
tree.insert(100);
tree + 200;  // Insert
tree - 100;  // Delete
tree.insertBatch({10,20,30});
tree.undo();
tree.redo();
```

---

## Project Structure

```
CPP/
├── ScapeGoatTree.hpp/tpp    # Main tree implementation
├── Node.hpp                  # Node structure
├── vector.hpp                # Custom dynamic array
├── queue.hpp/tpp             # Custom queue
├── stack.hpp                 # Custom stack
├── bindings.cpp              # Pybind11 bindings
├── iTree.cpp/hpp             # Terminal UI
├── TreeDriver.cpp            # DirectX + ImGui GUI
├── RunTU.cpp                 # Entry for terminal interface
└──  tests.cpp                 # Unit test suite  
py.py                     # Python Tkinter GUI
CMakeLists.txt            # Build configuration
README.md
```

---

## License

This project is licensed under the **MIT License**.

