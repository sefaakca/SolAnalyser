cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_VERSION "1.0.0")
project(SolInstru VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_FLAGS -std=c++11)

add_subdirectory(libSolInstru)
add_subdirectory(libUtils)
add_subdirectory(soli)

