# CMakeLists.txt

cmake_minimum_required(VERSION 3.14)
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
  GIT_TAG        v2.13.6
)

# Make pybind11 available
FetchContent_MakeAvailable(pybind11)

# Find Python (Interpreter and Development)
find_package(Python COMPONENTS Interpreter Development REQUIRED)

# Fetch Google Test
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1  # Specify the desired version
)

# Make Google Test available
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add source files
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/screamer/**/*.cpp")

# Create a C++ library for core functionality
add_library(screamer_core ${SOURCES})

# Create a C++ library for core functionality 
add_library(screamer_core ${SOURCES})
set_target_properties(screamer_core PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(screamer_core PUBLIC "${CMAKE_SOURCE_DIR}/include")

# Create Python module using pybind11, linking against screamer_core
pybind11_add_module(screamer_bindings bindings/screamer_bindings.cpp)
target_link_libraries(screamer_bindings PRIVATE screamer_core)

# Specify the output directory for the Python module
set_target_properties(screamer_bindings PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/screamer"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/screamer"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/screamer"
)

# Install the Python module to the screamer package directory
install(TARGETS screamer_bindings
        LIBRARY DESTINATION screamer
        RUNTIME DESTINATION screamer
        ARCHIVE DESTINATION screamer
)

# Add test executable, linking against screamer_core and Google Test
add_executable(test_lag tests/test_lag.cpp)
target_link_libraries(test_lag PRIVATE gtest_main screamer_core)

# Register the test
add_test(NAME LagTest COMMAND test_lag)
