add_executable(${PROJECT_NAME} main.cpp orderbook.cpp)

if(BUILD_BENCHMARK)
  target_link_libraries(${PROJECT_NAME} PRIVATE benchmark)
endif()

target_include_directories(${PROJECT_NAME}
                           PUBLIC "${PROJECT_SOURCE_DIR}/src/include")

target_compile_options(${PROJECT_NAME} PRIVATE -fexperimental-library)
