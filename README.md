## UMS

An acronym for Unified Math Stats. An experimental library to see if some C++20 features can be used to unite the many libraries and containers under one "it just works (though maybe not optimally)" kind of framework.

The goal is to make C++ data analysis a little bit more "like Python" in the sense that Python just works. The type restrictions in C++, while wonderful for performance, make compatibility between differing types difficult to navigate, and push people who are stronger in data analysis and weaker in programming towards Python, where things "just work."

## Quick Guide

Add this to your CMake file:

```
# Use MINIMUM C++20 (adjust based on your needs)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the FetchContent module
include(FetchContent)

# Declare the external library using FetchContent
FetchContent_Declare(
    UMS
    GIT_REPOSITORY https://github.com/matthewkolbe/ums.git
    GIT_TAG main  # Replace 'main' with the desired branch, tag, or commit hash
)

# Make the library available
FetchContent_MakeAvailable(UMS)

# Add the include directory of the fetched library to the include paths
target_include_directories([YOUR_PROJECT] PRIVATE ${UMS_SOURCE_DIR}/include)
```

Replace [YOUR_PROJECT] with the name of your project.

# Using UMS

To use UMS, simply add the following include:

```
#include "ums.hh"
```

Example code:

```
#include <vector>
#include <array>
#include "ums.hh"

int main() {

    // Choose any pair containers
    std::array<double, 4> a = {1.0, 2.0, 3.0, 4.0};
    std::vector<int>      b = {5, 6, 7, 8};

    // It just works... probably
    auto dot = ums::dot(a, b);

}
```

This example computes the dot product of an std::array and an std::vector using UMS, showcasing the library's ability to work with different container types.

# Building Your Project with UMS

After adding the UMS dependency to your CMakeLists.txt, build your project with:

```
mkdir build
cd build
cmake ..
make
```
