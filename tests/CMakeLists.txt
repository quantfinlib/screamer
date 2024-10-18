# tests/CMakeLists.txt


# AddAdd test source files using wildcard pattern
file(GLOB TEST_SOURCES "test*.cpp")

# If no test files are found, output a message
if(NOT TEST_SOURCES)
    message(WARNING "No test source files found in tests/ directory.")
endif()

# CreateCreate the test executable
add_executable(screamer_tests ${TEST_SOURCES})

# Link the test executable with the core library and Google Test libraries
target_link_libraries(screamer_tests PRIVATE
    screamer_core
    gtest_main
)

# Include directories for the test executable
target_include_directories(screamer_tests PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/src"
)

# Add the test
add_test(NAME screamer_tests COMMAND screamer_tests)
