cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(OrderBook VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(OrderBook main.cpp)

# Include directories
target_include_directories(OrderBook PUBLIC "${PROJECT_BINARY_DIR}")