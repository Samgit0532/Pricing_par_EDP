cmake_minimum_required(VERSION 3.16)
project(bs_pde_pricer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(bs_pde_pricer
    src/main.cpp
    src/solvers/Solver.cpp
)

target_include_directories(bs_pde_pricer PRIVATE src)
