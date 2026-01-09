# ScapeGoat Tree - API Documentation

## Quick Links
- @ref ScapeGoatTree - Main tree class
- @ref Node - Tree node structure
- @ref ITree - Terminal interface

## Overview

This is a comprehensive implementation of a Scapegoat Tree...

## Example Usage

@code{.cpp}
#include "ScapeGoatTree.hpp"

ScapeGoatTree<int> tree;
tree.insert(10);
tree.insert(20);
tree.insert(5);

if (tree.search(10)) {
    std::cout << "Found!\n";
}

tree.undo();  // Remove last insertion
@endcode
```

Then in your `Doxyfile`:
```
USE_MDFILE_AS_MAINPAGE = docs/mainpage.md
