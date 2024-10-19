cmake_minimum_required(VERSION 3.18)
project(screamer VERSION 0.1.28 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Python and Development libraries
find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)

# Include FetchContent module
include(FetchContent)

# Fetch pybind11
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.13.6
)

# Make pybind11 available
FetchContent_MakeAvailable(pybind11)

# Add source files
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/screamer/*.cpp")

# ---------------------------------------------------------
# Create the core library
# ---------------------------------------------------------
add_library(screamer_core ${SOURCES})
set_target_properties(screamer_core PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(screamer_core PRIVATE pybind11::module)
target_include_directories(screamer_core PUBLIC
  "${CMAKE_SOURCE_DIR}/include"
  "${pybind11_INCLUDE_DIRS}"
)

# ---------------------------------------------------------
# Create the Python module
# ---------------------------------------------------------
pybind11_add_module(screamer_bindings MODULE bindings/screamer_bindings.cpp)
target_link_libraries(screamer_bindings PRIVATE screamer_core pybind11::module)

# Install the module into the Python package directory
install(TARGETS screamer_bindings DESTINATION screamer)
