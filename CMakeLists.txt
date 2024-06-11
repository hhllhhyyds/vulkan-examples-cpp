cmake_minimum_required(VERSION 3.25)

project(vulkan_example_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_subdirectory(examples)