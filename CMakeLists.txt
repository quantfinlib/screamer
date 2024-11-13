cmake_minimum_required(VERSION 3.18)
project(screamer VERSION 0.1.46 LANGUAGES CXX)

# Set the default build type to Release for optimizations
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()


# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Set macOS deployment target for compatibility" FORCE)


# Find Python and Development libraries
find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)

# Include FetchContent module
include(FetchContent)

# Define a directory for dependencies to avoid redundant downloads
set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/_deps)

# Fetch pybind11
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.13.6
)

# Make pybind11 available
FetchContent_MakeAvailable(pybind11)
if(NOT pybind11_FOUND)
    message(FATAL_ERROR "pybind11 could not be fetched or located!")
endif()

# Fetch Eigen
FetchContent_Declare(
  eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG        3.4.0  # Use the desired version here
)

# Make Eigen available
FetchContent_MakeAvailable(eigen)
if(NOT TARGET Eigen3::Eigen)
    message(FATAL_ERROR "Eigen could not be fetched or located!")
endif()

# ---------------------------------------------------------
# Create the Python module
# ---------------------------------------------------------

# Collect all .cpp files in the bindings directory
file(GLOB_RECURSE BINDING_SOURCES "bindings/*.cpp")

# Collect all source files in the src directory
file(GLOB_RECURSE SCREAMER_SOURCES "src/screamer/*.cpp")

# Create the Python extension module
pybind11_add_module(screamer_bindings MODULE ${BINDING_SOURCES} ${SCREAMER_SOURCES})

# Link pybind11 and Eigen libraries
target_link_libraries(screamer_bindings PRIVATE pybind11::module Eigen3::Eigen)


# Set include directories
target_include_directories(
    screamer_bindings PUBLIC
    "${CMAKE_SOURCE_DIR}/include"
    "${pybind11_INCLUDE_DIRS}"
)

# Add optimization flags for performance
target_compile_options(screamer_bindings PRIVATE
    -O3                # Enable maximum optimization
    -march=native      # Enable architecture-specific optimizations
    -flto              # Enable Link Time Optimization
    -ffast-math        # Enable fast math operations (may be unsafe for some code)
)

# Install the module into the Python package directory
install(TARGETS screamer_bindings DESTINATION screamer)
