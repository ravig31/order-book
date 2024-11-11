cmake_minimum_required(VERSION 3.15)
project(OrderBook CXX)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find fmt package
find_package(fmt REQUIRED)

# Add executable
add_executable(orderbook
    src/main.cpp
    src/OrderBook.cpp
)

# Include directories
target_include_directories(orderbook PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${fmt_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(orderbook PRIVATE
    fmt::fmt
)