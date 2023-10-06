cmake_minimum_required(VERSION 3.20)

project(AutomatonLib)

add_library(AutomatonLib SHARED automaton.cpp)

target_include_directories(
    AutomatonLib
    PUBLIC
    ${CMAKE_SOURCE_DIR}
)

target_include_directories(
    AutomatonLib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/tests
)
