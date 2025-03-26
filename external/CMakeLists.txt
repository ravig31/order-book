set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

# Google Benchmark
if(BUILD_BENCHMARK)
  set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
  FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark
    GIT_TAG v1.9.2)
  add_subdirectory(benchmark)
endif()
