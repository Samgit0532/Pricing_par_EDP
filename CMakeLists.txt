cmake_minimum_required(VERSION 3.16)
project(bs_pde_pricer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Allows includes like "grid/FdGrid.hpp"
set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src)

# Common .cpp files to compile into both executables
set(SRC_CPP
    src/solvers/Solver.cpp
)

# --------- App executable (interactive) ---------
add_executable(bs_app
    src/main.cpp
    ${SRC_CPP}
)
target_include_directories(bs_app PRIVATE ${PROJECT_INCLUDE_DIR})

# --------- Tests executable ---------
add_executable(bs_tests
    src/tests/TestPricing.cpp
    ${SRC_CPP}
)
target_include_directories(bs_tests PRIVATE ${PROJECT_INCLUDE_DIR})
