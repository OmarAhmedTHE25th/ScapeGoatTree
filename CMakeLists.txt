cmake_minimum_required(VERSION 4.0)
project(ScapeGoatTree)

set(CMAKE_CXX_STANDARD 23)

add_executable(ScapeGoatTree TreeDriver.cpp
        ScapeGoatTree.cpp
        ScapeGoatTree.hpp)
