add_executable(create_window create_window.cpp)
target_include_directories(create_window PRIVATE Vulkan::Headers)
target_link_libraries(create_window Vulkan::Vulkan glfw)

add_executable(single_triangle single_triangle.cpp)
target_link_libraries(single_triangle render_system)