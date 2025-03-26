if(BUILD_BENCHMARK)
  set(CMAKE_BUILD_TYPE RELEASE)
  add_executable(OBBenchmark benchmark.cpp
                             ${PROJECT_SOURCE_DIR}/src/orderbook.cpp)
  target_link_libraries(OBBenchmark PRIVATE benchmark)
  target_include_directories(OBBenchmark
                             PRIVATE ${PROJECT_SOURCE_DIR}/src/include)
endif()
