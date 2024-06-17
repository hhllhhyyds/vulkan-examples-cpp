add_library(render_system STATIC
    instance.cpp
    device_queue.cpp
    swapchain.cpp
    shader.cpp
)
target_include_directories(render_system PUBLIC ${CMAKE_SOURCE_DIR}/src)

if(APPLE)
    target_link_libraries(render_system PUBLIC Vulkan::Vulkan glfw Vulkan::glslang)
elseif(LINUX)
    target_link_libraries(render_system PUBLIC Vulkan::Vulkan glfw glslang shaderc_shared)
else()
  message(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}")
endif()
