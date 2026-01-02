cmake_minimum_required(VERSION 3.15)
project(ScapeGoatProject)

set(CMAKE_CXX_STANDARD 20)

# 1. Find Python and pybind11
find_package(Python COMPONENTS Interpreter Development REQUIRED)

execute_process(
        COMMAND "${Python_EXECUTABLE}" -m pybind11 --cmakedir
        OUTPUT_VARIABLE pybind11_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
find_package(pybind11 REQUIRED PATHS ${pybind11_DIR} NO_DEFAULT_PATH)

# 2. CREATE THE PYTHON MODULE (This is what creates the .pyd file)
pybind11_add_module(scapegoat_tree_py bindings.cpp)
target_link_libraries(scapegoat_tree_py PRIVATE pybind11::module)
target_include_directories(scapegoat_tree_py PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 3. YOUR EXISTING EXECUTABLE (Keep this if you want to run the C++ driver)
add_executable(tree
        TreeDriver.cpp
        iTree.cpp
        ../imgui/imgui.cpp
        ../imgui/imgui_draw.cpp
        ../imgui/imgui_tables.cpp
        ../imgui/imgui_widgets.cpp
        ../imgui/imgui_demo.cpp
        ../imgui/imgui_impl_win32.cpp
        ../imgui/imgui_impl_dx11.cpp
        Node.hpp
        vector.hpp
        stack.hpp
)

target_link_libraries(tree PRIVATE
        pybind11::module
        d3d11 dxgi d3dcompiler dwmapi
)

# 4. UNIT TESTS
add_executable(unit_tests tests.cpp)
target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})