cmake_minimum_required(VERSION 3.15)
project(OrderBook CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_BENCHMARK "Build benchmark files" ON)

if (BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif()

add_subdirectory(src)
add_subdirectory(external)
