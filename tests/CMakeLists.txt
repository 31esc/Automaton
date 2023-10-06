cmake_minimum_required(VERSION 3.20)

project(AutomatonTests)

find_package(GTest REQUIRED CONFIG)

add_executable(Tests main_test.cpp test.cpp)
add_dependencies(Tests AutomatonLib)

target_include_directories(
    Tests
    PUBLIC
    ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(
    Tests
    PUBLIC
    AutomatonLib
    GTest::gtest GTest::gtest_main
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
target_compile_options(Tests PRIVATE --coverage)

include(GoogleTest)
gtest_discover_tests(Tests)
