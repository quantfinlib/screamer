cmake_minimum_required(VERSION 3.15)
project(screamer VERSION 0.1.5 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

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

# Fetch Google Test
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1  # Specify the desired version
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Make Google Test available
FetchContent_MakeAvailable(googletest)

# Find Python and Development libraries
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Add source files
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/screamer/*.cpp")

# Debug: Print the SOURCES variable
message(STATUS "SOURCES found: ${SOURCES}")

# Create the core library
add_library(screamer_core ${SOURCES})
set_target_properties(screamer_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Link the core library with pybind11
target_link_libraries(screamer_core PRIVATE pybind11::module Python3::Python)

# Set include directories for screamer_core
target_include_directories(screamer_core PUBLIC
  "${CMAKE_SOURCE_DIR}/include"
  "${pybind11_INCLUDE_DIRS}"
)

# Create the Python module
pybind11_add_module(screamer_bindings MODULE bindings/screamer_bindings.cpp)

# Link the Python module with the core library and pybind11
target_link_libraries(screamer_bindings PRIVATE screamer_core pybind11::module Python3::Python)

# Install the module into the Python package directory
install(TARGETS screamer_bindings DESTINATION .)

# Enable testing
enable_testing()

# Add the tests
add_subdirectory(tests)
