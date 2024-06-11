#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <string>

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
    Window() : Window(800, 600, false) {}

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    Window(const Window &&) = delete;
    Window &operator=(const Window &&) = delete;

    ~Window()
    {
        glfwDestroyWindow(handle);
        glfwTerminate();
    }

    bool should_close()
    {
        return glfwWindowShouldClose(handle);
    }

    void poll_event()
    {
        glfwPollEvents();
    }

private:
    GLFWwindow *handle;
    uint32_t width;
    uint32_t height;
};