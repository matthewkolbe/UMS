cmake_minimum_required(VERSION 3.18)
project(ums_tests)

# Include the directory in ../include
include_directories(../include)

# Use C++20
set(CMAKE_CXX_STANDARD 20)

# Enable testing
enable_testing()

set(SRC_FILES src/main.cpp 
              src/TestArray.cpp
    )

include(FetchContent)

# Check if GoogleTest was already fetched
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  # Fetch Google Test
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        main 
  )
  FetchContent_MakeAvailable(googletest)
endif()

FetchContent_Declare(Eigen3 SYSTEM
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3147391d946bb4b6c68edd901f2add6ac1f31f8c # 3.4.0
    GIT_SHALLOW TRUE
    SOURCE_SUBDIR cmake) # no CMakeLists.txt in cmake, so this turns off configure
# Recommend also add `FIND_PACKAGE_ARGS CONFIG` so that FetchContent checks to see if Eigen is installed on the system, via the o/s, or a package manager

FetchContent_MakeAvailable(Eigen3)

if(NOT TARGET Eigen3::Eigen)
    add_library(Eigen3::Eigen INTERFACE IMPORTED)
    set_target_properties(Eigen3::Eigen PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${eigen3_SOURCE_DIR})
endif()

# Add the executable target
add_executable(ums_tests ${SRC_FILES}) 

# Link the libraries to the executable
target_link_libraries(ums_tests PUBLIC 
      gtest
      gtest_main
      Eigen3::Eigen
      )

# Discover and register tests with CMake's testing infrastructure
include(GoogleTest)
gtest_discover_tests(ums_tests)
