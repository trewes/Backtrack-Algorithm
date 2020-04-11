cmake_minimum_required(VERSION 3.15)
project(Backtrack_Algorithm)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Backtrack_Algorithm
        backtracking.cpp
        backtracking.h
        graph.cpp
        graph.h
        main.cpp)
