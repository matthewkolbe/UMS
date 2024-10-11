# UMS

UMS stands for Unified Math Stats. It is an experimental library aimed at exploring how C++20 features can help unify various math and statistics libraries and containers into a framework where "it just works" (though maybe not optimally). The core idea is that there are many math and statistics containers in free and open-source software (FOSS), as well as in the STL, that have similar functionality but slightly different syntax and are treated as distinct types.

UMS "wraps" these containers in a `concept`, rather than using an adapter pattern, and leverages `constexpr` checks on common access patterns to determine which methods are supported by each container. It then promotes the appropriate method for use—-all behind the scenes. This means that users can simply call `ums::mean(data)` (or other functions), and the library automatically handles the calculation, regardless of the underlying container, and without runtime overhead.

The goal is to make C++ data analysis feel a little more "like Python" in the sense that things "just work." While C++'s type restrictions are great for performance, they can make it challenging to ensure compatibility between different types. UMS aims to bridge this gap, making C++ more approachable for data analysts by smoothing out these compatibility issues.

# Quick Guide

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

### Using UMS

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

    // Choose any pair containers (even Eigen, Blaze, pointers, etc)
    std::array<double, 4> a = {1.0, 2.0, 3.0, 4.0};
    std::vector<int>      b = {5, 6, 7, 8};

    // It just works... probably
    auto dot = ums::dot(a, b);

}
```