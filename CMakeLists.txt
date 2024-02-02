cmake_minimum_required(VERSION 3.27)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_library(untitled STATIC lib/InfFloat.cpp
        main.cpp)
