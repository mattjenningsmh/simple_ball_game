cmake_minimum_required(VERSION 3.10)

# Project name and version
project(RaylibGame VERSION 1.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find the raylib package (requires Raylib to be installed on your system)
find_package(raylib REQUIRED)

# Set source files 

set(SOURCE src/main.c) 

add_subdirectory(project_files)

# Add an executable for the main program
add_executable(RaylibGame ${SOURCE})

# Link the executable with raylib
target_link_libraries(RaylibGame PRIVATE raylib)

# Wayland-specific configuration
# Enable Wayland backend for Raylib
target_compile_definitions(RaylibGame PRIVATE GRAPHICS_API_WAYLAND)
target_link_libraries(RaylibGame PRIVATE m dl pthread)

# Include directories for Raylib
target_include_directories(RaylibGame PRIVATE ${RAYLIB_INCLUDE_DIRS})

# Add additional libraries for Wayland (if necessary, depends on your system setup)
find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND REQUIRED wayland-client wayland-cursor wayland-egl)

target_include_directories(RaylibGame PRIVATE ${WAYLAND_INCLUDE_DIRS})
target_link_libraries(RaylibGame PRIVATE ${WAYLAND_LIBRARIES})

# Install rules
install(TARGETS RaylibGame DESTINATION bin)

