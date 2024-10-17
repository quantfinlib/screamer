cmake_minimum_required(VERSION 3.15)
project(screamer VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include FetchContent module
include(FetchContent)

# Fetch pybind11
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.13.6  # Specify the desired version
)

# Make pybind11 available
FetchContent_MakeAvailable(pybind11)

# Add source files
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/screamer/**/*.cpp")

# Create the core library
add_library(screamer_core ${SOURCES})
set_target_properties(screamer_core PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(screamer_core PUBLIC "${CMAKE_SOURCE_DIR}/include")

# Create the Python module
pybind11_add_module(screamer_bindings MODULE bindings/screamer_bindings.cpp)
target_link_libraries(screamer_bindings PRIVATE screamer_core)

# Install the module into the Python package directory
install(TARGETS screamer_bindings DESTINATION .)
