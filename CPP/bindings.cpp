#include <pybind11/pybind11.h>
#include <pybind11/operators.h> // <--- NEEDED FOR + and ==
#include <pybind11/stl.h>
#include "scapegoat_tree.hpp"

namespace py = pybind11;
typedef long long Type;
/**
 * Pybind11 module for exposing the ScapeGoatTree implementation to Python.
 */
PYBIND11_MODULE(scapegoat_tree_py, m) {
    // 1. Bind Node
    py::class_<Node<Type>>(m, "Node")   // node class
        .def_readonly("value", &Node<Type>::value)
        .def_readonly("left", &Node<Type>::left)
        .def_readonly("right", &Node<Type>::right);

    // 2. Bind ScapeGoatTree
    py::class_<ScapeGoatTree<Type>>(m, "ScapeGoatTree")
        .def(py::init<>())
        .def(py::init<const ScapeGoatTree<Type>&>()) // Copy constructor

        // standard ops
        .def("insert", &ScapeGoatTree<Type>::insert)
    .def("insert_batch", [](ScapeGoatTree<Type> &self, const std::vector<Type> &values) { // makes an insert batch method that takes, a vector as a parameter
            Vector<Type> customVec; // custom vector (Vector.hpp)
            for (const auto &v : values) customVec.push_back(v); //copy values from STL vector to our custom Vector.
            self.insertBatch(customVec);
        }, py::arg("values"))
        .def("delete_batch", [](ScapeGoatTree<Type> &self, const std::vector<Type> &values) {
            Vector<Type> customVec;
            for (const auto &v : values) customVec.push_back(v);
            self.deleteBatch(customVec);
        }, py::arg("values"))
        .def("delete_value", &ScapeGoatTree<Type>::deleteValue)
        .def("search_node", [](const ScapeGoatTree<Type>& t, Type val) -> Node<Type>* {
return t.find_node(val);   // call the Node* version
})
        .def("search_bool", [](const ScapeGoatTree<Type>& t, Type val) -> bool {
return t.search(val);   // call the bool version
})

        .def("get_root", &ScapeGoatTree<Type>::getRoot, py::return_value_policy::reference_internal)
        .def("clear", &ScapeGoatTree<Type>::clear)
        .def("undo", &ScapeGoatTree<Type>::undo)
        .def("redo", &ScapeGoatTree<Type>::redo)
        .def("SuminRange",&ScapeGoatTree<Type>::sumInRange)
        .def("ValuesInRange", &ScapeGoatTree<Type>::valuesInRange)
        .def("KthSmallest", &ScapeGoatTree<Type>::kthSmallest)
        .def("GetSuccessor", &ScapeGoatTree<Type>::getSuccessor)
        .def("GetMin", &ScapeGoatTree<Type>::getMin)
        .def("GetMax", &ScapeGoatTree<Type>::getMax)
        .def("Split",&ScapeGoatTree<Type>::split)
         .def("SetAlpha", &ScapeGoatTree<Type>::changeAlpha)
        .def(py::self + py::self)
        .def(py::self == py::self)


        .def("is_empty", [](const ScapeGoatTree<Type>& t) {
            return !t;
        })

        // Reporting & Displays
        .def("get_balance_report", &ScapeGoatTree<Type>::isBalanced)
        .def("get_inorder", [](ScapeGoatTree<Type> &t) { return t.displayInOrder(); })
        .def("get_preorder", [](ScapeGoatTree<Type> &t) { return t.displayPreOrder(); })
        .def("get_postorder", [](ScapeGoatTree<Type> &t) { return t.displayPostOrder(); })
        .def("get_levels", &ScapeGoatTree<Type>::displayLevels);


}