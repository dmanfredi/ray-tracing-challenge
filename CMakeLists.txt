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
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files (x86)/Eigen3")
set(EIGEN3_INCLUDE_DIR "C:/Program Files (x86)/Eigen3/include/eigen3")
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(${EIGEN3_INCLUDE_DIR})
 
# add_executable (example example.cpp)
# target_link_libraries (example Eigen3::Eigen)

add_library(ray_tracer_lib
    tuple.cc
    canvas.cc
    utils.cc
    sphere.cc
    intersection.cc
    light.cc
    material.cc
    world.cc
    console.cc
)

# Update the main executable linking
add_executable(main_app main.cc)
target_link_libraries(main_app 
    ray_tracer_lib
    Eigen3::Eigen
)

# Update the test executable linking
add_executable(main_test main_test.cc)
target_link_libraries(main_test 
    gtest_main 
    ray_tracer_lib
    Eigen3::Eigen
)

# Register the test with CMake
add_test(NAME main_test COMMAND main_test)
