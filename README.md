# 🌳 Scapegoat Tree Implementation

[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](https://omarahmedthe25th.github.io/ScapeGoatTree/)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE.md)
[![C++](https://img.shields.io/badge/C++-26-00599C.svg?logo=c%2B%2B)]()
[![Python](https://img.shields.io/badge/Python-3.7+-3776AB.svg?logo=python&logoColor=white)]()

### A Self-Balancing BST with Multi-Platform Support

**Academic Project**: Data Structures Course  
**Language**: C++26 with Python Bindings  
**Constraint**: Minimal STL Usage (Custom Containers)

**Note**: This is the most comprehensive publicly available Scapegoat Tree project, including multiple user interfaces, Python bindings, advanced operations, and extensive testing.

---

## 📚 Documentation

**[→ View Full API Documentation](https://omarahmedthe25th.github.io/ScapeGoatTree/)**

Complete API reference with:
- Class hierarchies and relationships
- Detailed method descriptions with complexity analysis
- Code examples and usage patterns
- Interactive search functionality

---

## ✨ Features

### Core Tree Features

* ✅ α-Weight-Balanced Scapegoat Tree  
* ✅ Automatic height-balanced rebalancing  
* ✅ Supports insert, delete, search  
* ✅ **Sum in range** — efficiently compute the sum of all values within a given range  
* ✅ **Values in range** — retrieve all elements within a specified range  
* ✅ **Kth smallest element** — find the element at a specific order in sorted sequence
* ✅ **Forward iterator** support — fully compatible with C++ range-based for loops (for(auto x : tree))
* ✅ **Get successor** — find the next higher element in the tree  
* ✅ **Get minimum / maximum** — retrieve the smallest or largest element in the tree  
* ✅ Batch operations for efficiency  
* ✅ Undo/Redo system  
* ✅ Tree merging and splitting  
* ✅ Operator overloading for intuitive syntax  

### Custom Data Structures

* **Vector**: Dynamic array, automatic resizing, minimal memory overhead  
* **Queue**: Singly-linked list for level-order traversal  
* **Stack**: Built on Vector, used for undo/redo  

### User Interfaces

* 🖥️ Terminal UI (TUI) with color-coded menus  
* 🎨 Python Tkinter GUI with animations and collapsible sidebar
* 🎮 DirectX 11 + ImGui GUI (Windows only)  

### Advanced Usage

* 🐍 Cross-language Python bindings via pybind11  
* ⚙️ Custom α parameter for tree balancing  
* 📊 Detailed balance checking and traversal outputs  

---

## 🚀 Quick Start

### Running the Python GUI (Cross-Platform)
```bash
# 1. Build the project
mkdir build && cd build
cmake .. && make

# 2. Run the animated visualizer
python ../py.py
```

![GUI Demo](https://img.shields.io/badge/GUI-Animated-ff69b4)

### Running the C++ Terminal UI
```bash
# After building, run the executable
./TUI   # Terminal-based User Interface
```

### Running via Docker

The provided Dockerfile runs the Terminal UI directly:
```bash
docker build -t scapegoat .
docker run -it --rm scapegoat
```

---

## 📖 Algorithm Overview

A Scapegoat Tree is a self-balancing BST that maintains balance through periodic rebuilding:

* **α-weight-balanced**: No subtree can exceed α × parent's size (α = 2/3)
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

## 🛠️ Prerequisites

### Required

* C++ Compiler: GCC 9+, Clang 10+, or MSVC 2019+
* CMake 3.15+
* Python 3.7+
* pybind11 (`pip install pybind11`)

### Optional

* DirectX 11 SDK (for Windows GUI)
* Tkinter (usually included with Python)
* Doxygen (for generating documentation locally)

---

## 💻 Installation

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

## 📝 Usage Examples

### Python Interface
```python
import scapegoat_tree_py as sgt

# Create tree and insert values
tree = sgt.ScapeGoatTree()
tree.insert_batch([10, 20, 30, 5, 15])

# Use undo/redo
tree.undo()
tree.redo()

# Range queries
sum_result = tree.SuminRange(10, 30)
values = tree.ValuesInRange(5, 20)

# Merge trees
tree2 = sgt.ScapeGoatTree()
tree2.insert_batch([25, 35])
merged = tree + tree2

# Split tree
left_tree, right_tree = tree.Split(15)
```

### C++ Interface
```cpp
#include "ScapeGoatTree.hpp"

ScapeGoatTree<int> tree;

// Insert values
tree.insert(100);
tree + 200;  // Operator overload

// Delete values
tree - 100;  // Operator overload

// Batch operations
tree.insertBatch({10, 20, 30});

// Undo/Redo
tree.undo();
tree.redo();

// Split tree
auto [left, right] = tree.split(50);

// Range queries
int sum = tree.sumInRange(10, 50);
Vector<int> values = tree.valuesInRange(10, 50);
```

---

## 🧪 Testing

### Run Unit Tests
```bash
cd build
./unit_tests
```

Comprehensive test suite includes:
- ✅ Basic operations and edge cases
- ✅ Automatic rebalancing verification
- ✅ Operator overloading
- ✅ Undo/Redo system
- ✅ Batch operations
- ✅ Copy and move semantics
- ✅ **Stress testing with 50,000 operations**

---

## 📁 Project Structure
```
ScapeGoatTree/
├── .github/
│   └── workflows/
│       └── docs.yml              # Auto-generate documentation
├── CPP/
│   ├── ScapeGoatTree.hpp/tpp     # Main tree implementation
│   ├── Node.hpp                  # Node structure
│   ├── vector.hpp                # Custom dynamic array
│   ├── queue.hpp/tpp             # Custom queue
│   ├── stack.hpp                 # Custom stack
│   ├── bindings.cpp              # Pybind11 bindings
│   ├── iTree.cpp/hpp             # Terminal UI
│   ├── TreeDriver.cpp            # DirectX + ImGui GUI
│   ├── RunTUI.cpp                # Entry for terminal interface
│   └── tests.cpp                 # Unit test suite  
├── py.py                         # Python Tkinter GUI
├── CMakeLists.txt                # Build configuration
├── Doxyfile                      # Documentation config
├── Dockerfile                    # Container deployment
├── LICENSE.md
└── README.md
```

---

## 🎓 Learning Resources

For more information about Scapegoat Trees:
- [Original Paper (1993)](https://people.csail.mit.edu/rivest/pubs/GR93.pdf) by Galperin & Rivest
- [API Documentation](https://omarahmedthe25th.github.io/ScapeGoatTree/)

---

## 📄 License

This project is licensed under the **MIT License** - see [LICENSE.md](LICENSE.md) for details.

---

## 👤 Author

**Omar Ahmed Abdel Hameed**

- GitHub: [@omarahmedthe25th](https://github.com/omarahmedthe25th)
- Project Link: [https://github.com/omarahmedthe25th/ScapeGoatTree](https://github.com/omarahmedthe25th/ScapeGoatTree)

---

## 🌟 Acknowledgments

- Course: Data Structures  
- Inspiration: Self-balancing tree algorithms
- Special thanks to the open-source community

---

<div align="center">

**⭐ Star this repo if you found it helpful!**

[![GitHub stars](https://img.shields.io/github/stars/omarahmedthe25th/ScapeGoatTree?style=social)](https://github.com/omarahmedthe25th/ScapeGoatTree)

</div>
