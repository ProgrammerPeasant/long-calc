cmake_minimum_required(VERSION 3.10)

project(LongArithmetic VERSION 1.9)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(lib)

add_executable(LongCalc main.cpp)

target_link_libraries(LongCalc PUBLIC InfFloatLib)


target_include_directories(LongCalc PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/lib"
)