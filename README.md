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
* ✅ **Get successor** — find the next higher element in the tree  
* ✅ **Get minimum / maximum** — retrieve the smallest or largest element in the tree  
* ✅ Batch operations for efficiency  
* ✅ Undo/Redo system  
* ✅ Tree merging with duplicate handling  
* ✅ Operator overloading for intuitive syntax  

### Custom Data Structures

* **Vector**: Dynamic array, automatic resizing, minimal memory overhead  
* **Queue**: Singly-linked list for level-order traversal  
* **Stack**: Built on Vector, used for undo/redo  

### User Interfaces

* 🖥️ Terminal UI (TUI) with color-coded menus  
* 🎨 Python Tkinter GUI with animations
* 🎮 DirectX 11 + ImGui GUI (Windows only)  

### Advanced Usage

* 🐍 Cross-language Python bindings via pybind11  
* ⚙️ Custom α parameter for tree balancing  
* 📊 Detailed balance checking and traversal outputs  

---

## 🚀 Quick Start

### Running the Python GUI (Cross-Platform)
```bash
