#pragma once

#include <cstdlib>
#include <string>

#include "glfw_vulkan.h"

class Window
{
public:
    Window(uint32_t width, uint32_t height, bool resizable, const std::string &name = "Vulkan Example") : width(width), height(height)
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

        handle = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    };
    Window(const std::string &name = "Vulkan Example") : Window(800, 600, true, name) {}

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    Window(const Window &&) = delete;
    Window &operator=(const Window &&) = delete;

    ~Window()
    {
        glfwDestroyWindow(handle);
        handle = nullptr;
        glfwTerminate();
    }

    GLFWwindow *get_glfw_window() const
    {
        return handle;
    }

    bool should_close() const
    {
        return glfwWindowShouldClose(handle);
    }

    void poll_event() const
    {
        glfwPollEvents();
    }

private:
    GLFWwindow *handle;
    uint32_t width;
    uint32_t height;
};