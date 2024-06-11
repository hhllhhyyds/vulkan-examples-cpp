add_executable(create_window create_window.cpp)
target_include_directories(create_window PRIVATE Vulkan::Headers)
target_link_libraries(create_window Vulkan::Vulkan glfw)