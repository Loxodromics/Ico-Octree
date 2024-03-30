cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(Ico-OcteeProject VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable
add_executable(Icosphere src/main.cpp src/icosphere.cpp src/vector3.cpp)

# Include directories
target_include_directories(Icosphere PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
