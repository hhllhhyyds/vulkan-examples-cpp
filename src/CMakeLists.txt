add_library(render_system STATIC
    instance.cpp
)
target_include_directories(render_system PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(render_system Vulkan::Vulkan glfw)