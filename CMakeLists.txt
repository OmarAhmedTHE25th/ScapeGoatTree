cmake_minimum_required(VERSION 3.20)
project(tree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# EXECUTABLE
# =========================
add_executable(tree
        CPP/TreeDriver.cpp
        CPP/iTree.cpp

        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_demo.cpp   # ← THIS ONE
        imgui/imgui_impl_win32.cpp
        imgui/imgui_impl_dx11.cpp
)

target_include_directories(tree PRIVATE imgui)
target_link_libraries(tree
        d3d11
        dxgi
        d3dcompiler
        dwmapi
)


# =========================
# INCLUDE DIRECTORIES
# =========================
target_include_directories(tree PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/CPP
)
