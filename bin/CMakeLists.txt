cmake_minimum_required(VERSION 3.10)
project(YourProjectName)

# Add the directory containing IntFloat.cpp to the project
add_library(InfFloatLib InfFloat.cpp)

# Add the executable target and link it with the IntFloatLib
add_executable(main main.cpp)
target_link_libraries(main PUBLIC InfFloatLib)
