cmake_minimum_required(VERSION 3.25)
project(GBAGame01)

set(CMAKE_CXX_STANDARD 23)

# Include GBA.cmake configuration
# DevKitPro needed (https://devkitpro.org/wiki/Getting_Started)
include("/opt/devkitpro/cmake/GBA.cmake")

# Add your source files
add_executable(GBAGame01 source/main.cpp)

# Link the target against the libgba library
target_link_libraries(GBAGame01 gba)