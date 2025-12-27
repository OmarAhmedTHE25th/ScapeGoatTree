# 🐐 ScapeGoat Tree (with GUI)

A Windows-based C++ implementation of a **ScapeGoat Tree**, featuring both a real-time **ImGui + DirectX 11 GUI visualizer** and a **console fallback mode**.

---

## ✨ Features

- Self-balancing BST using rebuild-based rebalancing
- GUI tree visualization (ImGui + DirectX 11)
- Terminal mode available inside the GUI
- Tree operations:
  - Insert (single + batch)
  - Delete (single + batch)
  - Search / Find
  - Traversals:
    - In-order, Pre-order, Post-order, Level-order
- Operator overload support:
  ```cpp
  tree + 10;
  tree - 5;
  if (tree == otherTree) ...
  ```

---

## 🧰 Requirements

| Component | Minimum |
|---|---|
| OS | **Windows 10/11** |
| Compiler | Modern C++ (C++17+) |
| Build Tool | **CMake 3.20+** |
| Graphics | **DirectX 11** |

---

## 📁 Project Structure

```
ScapeGoat-Tree-GUI/
│
├── CMakeLists.txt
├── CPP/ or root/
│   ├── TreeDriver.cpp
│   ├── iTree.cpp
│   ├── iTree.hpp
│   ├── ScapeGoatTree.hpp
│   └── Node.hpp
│
└── imgui/  (ImGui library files)
```

---

## 🛠 Build Instructions

```bash
cmake -S . -B build
cmake --build build --config Release
```

---

## ▶️ Run Instructions

After building, run the executable from:

```
build/Release/tree.exe
```

Or directly from terminal:

```bash
./build/Release/tree.exe
```

---

## 🖥 Modes

- **GUI Mode:** launches automatically
- **Console Mode:** triggered by clicking:
  ```
  Run Tree in Console
  ```
  (Closes GUI, switches to terminal)

---

## 🧪 Example Usage

Insert values:

```cpp
tree + 5;
tree + 10;
tree + 3;
```

Search:

```cpp
tree.find(10);
```

---

## 🏅 Badges

![Windows Only](https://img.shields.io/badge/OS-Windows-blue?style=flat-square)
![CMake](https://img.shields.io/badge/Built%20with-CMake-brightgreen?style=flat-square)
![C++17](https://img.shields.io/badge/C++-17-orange?style=flat-square)
![Balanced BST](https://img.shields.io/badge/Tree-Balanced%20BST-purple?style=flat-square)

---
No goats were harmed during the making of this project.

