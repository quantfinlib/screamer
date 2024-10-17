# CMakeLists.txt

cmake_minimum_required(VERSION 3.15)
project(screamer VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set MSVC runtime library to MultiThreaded DLL (/MD for Release, /MDd for Debug)
if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# Include FetchContent module
include(FetchContent)

# Set PYBIND11_USE_CMAKE_OUTPUT_EXTENSION to avoid ABI-specific suffixes
set(PYBIND11_USE_CMAKE_OUTPUT_EXTENSION TRUE)

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
  GIT_TAG        release-1.12.1
)

# Prevent Google Test from overriding our compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Make Google Test available
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add source files
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/screamer/**/*.cpp")

# Create a C++ library for core functionality
add_library(screamer_core ${SOURCES})
set_target_properties(screamer_core PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(screamer_core PUBLIC "${CMAKE_SOURCE_DIR}/include")

# Create Python module using pybind11, linking against screamer_core
pybind11_add_module(screamer_bindings MODULE bindings/screamer_bindings.cpp)
target_link_libraries(screamer_bindings PRIVATE screamer_core)

# Ensure consistent naming and output directories
set_target_properties(screamer_bindings PROPERTIES
    OUTPUT_NAME "screamer_bindings"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/screamer"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/screamer"
)

if (WIN32)
    # On Windows, set the suffix to .pyd and handle output directories
    set_target_properties(screamer_bindings PROPERTIES
        SUFFIX ".pyd"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/screamer"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/screamer"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/screamer"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/screamer"
    )
else()
    # On Linux and macOS, remove the 'lib' prefix
    set_target_properties(screamer_bindings PROPERTIES PREFIX "")
endif()

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
