cmake_minimum_required(VERSION 3.25)

project(vulkan_example_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(glfw3 REQUIRED)

if(APPLE)
    find_package(Vulkan REQUIRED COMPONENTS glslang)
elseif(LINUX)
    find_package(Vulkan REQUIRED)

    find_library(glslang_path glslang)
    message(STATUS glslang=${glslang_path})

    find_library(shaderrc_path shaderc_shared)
    message(STATUS shaderc_shared=${shaderrc_path})
else()
    message(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}")
endif()

add_subdirectory(src)
add_subdirectory(examples)