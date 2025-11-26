# ScapeGoatTree

A simple C++ implementation of a ScapeGoat Tree — a self‑balancing binary search tree that rebuilds subtrees to maintain balance when necessary. This repo contains a compact implementation intended for learning, experimentation, and small projects.


## Features
- Basic ScapeGoat Tree operations: insert, remove, search
- Tree traversal utilities (inorder / preorder / postorder) for debugging and demonstration
- Small, easy-to-read C++ codebase using CMake for build management

## Repository layout
- CMakeLists.txt — CMake build configuration
- Node.hpp / Node.cpp — Node class / struct for tree nodes
- ScapeGoatTree.hpp / ScapeGoatTree.cpp — ScapeGoatTree implementation (insert, remove, search, balancing)
- TreeDriver.cpp — example driver / simple CLI demonstrating usage
- .idea/ — IDE config files (can be ignored)

## Build instructions

Prerequisites:
- A C++ compiler supporting C++11 (or later)
- CMake (recommended 3.10+)

From the repository root:

```bash
# create a build directory and compile
cmake -S . -B build
cmake --build build --config Release

# or using the traditional flow:
# mkdir -p build && cd build
# cmake ..
# cmake --build .
```

After build, run the example binary (name depends on CMakeLists; if the executable is `TreeDriver`):

```bash
./build/TreeDriver
```


## Example usage

Typical operations the driver demonstrates:

- Insert keys into the tree
- Search for a key (returns found/not found)
- Remove a key
- Print inorder traversal (will print keys in sorted order)

A minimal example (pseudo-interaction):

```text
> Insert 20
> Insert 10
> Insert 30
> Find 10   -> found
> Remove 20
```



## API (what to expect)

ScapeGoatTree class (ScapeGoatTree.hpp / ScapeGoatTree.cpp)
- insert(key) — insert a key into the tree
- remove(key) — remove a key from the tree (if present)
- find(key) / contains(key) — test for key presence
- inorderTraversal(callback) — visit nodes in sorted order
- size() — number of nodes in the tree
- clear() — remove all nodes

Node class/struct (Node.hpp / Node.cpp)
- Stores key (templated or concrete int/string depending on implementation)
- left and right child pointers
