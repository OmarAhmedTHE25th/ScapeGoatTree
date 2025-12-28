#include <pybind11/pybind11.h>
#include <pybind11/operators.h> // <--- NEEDED FOR + and ==
#include <pybind11/stl.h>
#include "ScapeGoatTree.hpp"

namespace py = pybind11;

PYBIND11_MODULE(scapegoat_tree_py, m) {
    // 1. Bind Node
    py::class_<Node<int>>(m, "Node")
        .def_readonly("value", &Node<int>::value)
        .def_readonly("left", &Node<int>::left)
        .def_readonly("right", &Node<int>::right);

    // 2. Bind ScapeGoatTree
    py::class_<ScapeGoatTree<int>>(m, "ScapeGoatTree")
        .def(py::init<>())
        .def(py::init<const ScapeGoatTree<int>&>()) // Copy constructor

        // standard ops
        .def("insert", &ScapeGoatTree<int>::insert)
        .def("delete_value", &ScapeGoatTree<int>::deleteValue)
        .def("search", &ScapeGoatTree<int>::search)
        .def("get_root", &ScapeGoatTree<int>::getRoot, py::return_value_policy::reference_internal)

        // OPERATORS (The cool new part)
        .def(py::self + py::self)  // Allows: treeA + treeB
        .def(py::self == py::self) // Allows: treeA == treeB

        // Helper to check if empty (since C++ doesn't have is_empty public)
        .def("is_empty", [](ScapeGoatTree<int>& t) {
            return t.getRoot() == nullptr;
        })

        // Reporting & Displays
        .def("get_balance_report", &ScapeGoatTree<int>::isBalanced)
        .def("get_inorder", [](ScapeGoatTree<int> &t) { t.displayInOrder(); return t.getDisplayBuffer(); })
        .def("get_preorder", [](ScapeGoatTree<int> &t) { t.displayPreOrder(); return t.getDisplayBuffer(); })
        .def("get_postorder", [](ScapeGoatTree<int> &t) { t.displayPostOrder(); return t.getDisplayBuffer(); })
        .def("get_levels", [](ScapeGoatTree<int> &t) { t.displayLevels(); return t.getDisplayBuffer(); });
}