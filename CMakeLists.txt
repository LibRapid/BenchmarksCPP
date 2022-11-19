cmake_minimum_required(VERSION 3.16)
project(BenchmarksCPP)

set(CMAKE_CXX_STANDARD 17)

option(USING_GITHUB_ACTIONS "Using Github Actions" OFF)

# Set some options
set(XTENSOR_USE_XSIMD ON)
set(XTENSOR_USE_OPENMP ON)
set(XTENSOR_USE_FLENS_BLAS ON)

set(LIBRAPID_OPTIMISE_SMALL_ARRAYS ON)

# Add the required libraries
add_subdirectory(lib/librapid)
add_subdirectory(lib/eigen)
add_subdirectory(lib/xtl)
add_subdirectory(lib/xsimd)
add_subdirectory(lib/xtensor)

file(GLOB BENCHMARK_SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/src/*.cpp"
        )

add_executable(BenchmarksCPP main.cpp
        ${BENCHMARK_SOURCE_FILES})

target_link_libraries(BenchmarksCPP PUBLIC librapid eigen xtl xsimd xtensor)
target_include_directories(BenchmarksCPP PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/json/single_include/nlohmann")
target_include_directories(BenchmarksCPP PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/eigen/Eigen")
target_include_directories(BenchmarksCPP PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

if (USING_GITHUB_ACTIONS)
    target_compile_definitions(BenchmarksCPP PUBLIC USING_GITHUB_ACTIONS)
endif ()