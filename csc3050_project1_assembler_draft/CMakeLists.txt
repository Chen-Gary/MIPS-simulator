cmake_minimum_required(VERSION 3.17)
project(assembler_draft)

set(CMAKE_CXX_STANDARD 14)

add_executable(assembler_draft main.cpp assembler_str2binary.hpp)