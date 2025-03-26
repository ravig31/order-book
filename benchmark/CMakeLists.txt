if(BUILD_BENCHMARK)
  add_executable(OBBenchmark benchmark.cpp
                             ${PROJECT_SOURCE_DIR}/src/orderbook.cpp)
  target_link_libraries(OBBenchmark PRIVATE benchmark)
  target_include_directories(OBBenchmark
                             PRIVATE ${PROJECT_SOURCE_DIR}/src/include)
endif()
