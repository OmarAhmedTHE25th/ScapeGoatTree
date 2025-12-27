#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "ScapeGoatTree.hpp"

namespace py = pybind11;

PYBIND11_MODULE(scapegoat_tree_py, m) {
    py::class_<Node<int>>(m, "Node")
        .def_readonly("value", &Node<int>::value)
        .def_readonly("left", &Node<int>::left)
        .def_readonly("right", &Node<int>::right);

    py::class_<ScapeGoatTree<int>>(m, "ScapeGoatTree")
        .def(py::init<>())
        .def("insert", &ScapeGoatTree<int>::insert)
        .def("delete_value", &ScapeGoatTree<int>::deleteValue)
        .def("search", &ScapeGoatTree<int>::search)
        .def("get_balance_report", &ScapeGoatTree<int>::isBalanced)
        .def("get_root", &ScapeGoatTree<int>::getRoot, py::return_value_policy::reference_internal)

        // --- NEW DISPLAY WRAPPERS ---
        // These run the C++ void function, then grab the string from the buffer
        .def("get_inorder", [](ScapeGoatTree<int> &t) {
            t.displayInOrder();       // [cite: 74]
            return t.getDisplayBuffer(); // [cite: 77]
        })
        .def("get_preorder", [](ScapeGoatTree<int> &t) {
            t.displayPreOrder();      // [cite: 73]
            return t.getDisplayBuffer();
        })
        .def("get_postorder", [](ScapeGoatTree<int> &t) {
            t.displayPostOrder();     // [cite: 76]
            return t.getDisplayBuffer();
        })
        .def("get_levelorder", [](ScapeGoatTree<int> &t) {
            t.displayLevels();        // [cite: 78]
            return t.getDisplayBuffer();
        });
}