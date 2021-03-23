cmake_minimum_required(VERSION 3.17)
project(simulator)

set(CMAKE_CXX_STANDARD 14)

add_executable(simulator main.cpp assembler_src.hpp io_related.hpp simulator_start.hpp simulator_instruction_functions.hpp simulator_simulateToExecute.hpp)