cmake_minimum_required(VERSION 3.16)
project(BenchmarksCPP)

set(CMAKE_CXX_STANDARD 23)

option(USING_GITHUB_ACTIONS "Using Github Actions" OFF)

# Set some options
set(XTENSOR_USE_XSIMD ON)
set(XTENSOR_USE_OPENMP ON)
set(XTENSOR_USE_FLENS_BLAS ON)

# If this is running in GitHub Actions, the options will be set by the workflow
if (NOT USING_GITHUB_ACTIONS)
    set(LIBRAPID_OPTIMISE_SMALL_ARRAYS OFF)
    set(LIBRAPID_BUILD_EXAMPLES ON)
    set(LIBRAPID_BUILD_TESTS ON)
    set(LIBRAPID_CODE_COV OFF)
    set(LIBRAPID_STRICT OFF)
    set(LIBRAPID_QUIET OFF)
    set(LIBRAPID_USE_BLAS ON)
    set(LIBRAPID_USE_OPENCL ON)
    set(LIBRAPID_USE_CUDA ON)
    set(LIBRAPID_USE_OMP ON)
    set(LIBRAPID_USE_MULTIPREC OFF)
    set(LIBRAPID_FAST_MATH OFF)
    set(LIBRAPID_CUDA_FLOAT_VECTOR_WIDTH 4)
    set(LIBRAPID_CUDA_DOUBLE_VECTOR_WIDTH 4)
    set(LIBRAPID_GET_FFTW OFF)
    set(LIBRAPID_GET_BLAS OFF)
    set(LIBRAPID_GET_MULTIPREC OFF)
    set(LIBRAPID_NO_WINDOWS_H OFF)
endif ()

# Add the required libraries
add_subdirectory(vendor/librapid)
add_subdirectory(vendor/eigen)
add_subdirectory(vendor/xtl)
# add_subdirectory(lib/xsimd) # LibRapid uses XSIMD
add_subdirectory(vendor/xtensor)
add_subdirectory(vendor/nanobench)

file(GLOB BENCHMARK_SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(BenchmarksCPP main.cpp
        ${BENCHMARK_SOURCE_FILES})

# OpenMP
find_package(OpenMP REQUIRED)

target_link_libraries(BenchmarksCPP PUBLIC librapid eigen xsimd xtl xtensor OpenMP::OpenMP_CXX)
target_include_directories(BenchmarksCPP PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/eigen/Eigen")
target_include_directories(BenchmarksCPP PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/nanobench/src/include")
target_include_directories(BenchmarksCPP PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

if (USING_GITHUB_ACTIONS)
    target_compile_definitions(BenchmarksCPP PUBLIC USING_GITHUB_ACTIONS)
endif ()