# CMakeLists.txt in /experiments
cmake_minimum_required(VERSION 3.10)
project(Experiments)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/../include)

# Collect all .cpp files in the current directory
file(GLOB EXPERIMENT_SOURCES "*.cpp")

# Loop through each .cpp file and create an executable for it
foreach(SOURCE_FILE ${EXPERIMENT_SOURCES})
    # Get the file name without the extension
    get_filename_component(EXECUTABLE_NAME ${SOURCE_FILE} NAME_WE)
    
    # Add an executable target for each source file
    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILE})
endforeach()