cmake_minimum_required(VERSION 3.10)

# Project name (prioritizing ray-tracer naming)
project(ray-tracer-next)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ensure we're using MinGW 
# set(CMAKE_C_COMPILER "gcc")
# set(CMAKE_CXX_COMPILER "g++")

# Include Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Well there goes any portability.
# Eigen configuration
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files (x86)/Eigen3")
set(EIGEN3_INCLUDE_DIR "C:/Program Files (x86)/Eigen3/include/eigen3")
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# Add include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${EIGEN3_INCLUDE_DIR}
)
 
# This makes headers available for all targets
include_directories(${CMAKE_SOURCE_DIR})

# Library target
add_library(ray_tracer_lib
    src/tuple.cc
    src/canvas.cc
    src/utils.cc
    src/sphere.cc
    src/shape.cc
    src/testshape.cc
    src/intersection.cc
    src/light.cc
    src/material.cc
    src/world.cc
    src/console.cc
)

# Main application
add_executable(main_app src/main.cc)
target_link_libraries(main_app 
    ray_tracer_lib
    Eigen3::Eigen
)

# Tests configuration
set(TEST_SOURCES
    tests/all_tests.cc
    tests/tuple_tests.cc
)

add_executable(main_test ${TEST_SOURCES})
target_link_libraries(main_test 
    gtest_main 
    ray_tracer_lib
    Eigen3::Eigen
)

# Register the test
enable_testing()
add_test(NAME main_test COMMAND main_test)