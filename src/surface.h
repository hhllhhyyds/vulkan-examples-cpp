#pragma once

#include <memory>

#include "window.h"
#include "instance.h"

using std::shared_ptr;

class Surface
{
public:
    Surface(const shared_ptr<Instance> instance, const shared_ptr<Window> window) : instance(instance), window(window)
    {
        if (glfwCreateWindowSurface(instance->get_vk_instance(), window->get_glfw_window(), nullptr, &handle) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    Surface(const Surface &) = delete;
    Surface &operator=(const Surface &) = delete;
    Surface(const Surface &&) = delete;
    Surface &operator=(const Surface &&) = delete;

    VkSurfaceKHR get_vk_surface_khr() const
    {
        return handle;
    }

    ~Surface()
    {
        vkDestroySurfaceKHR(instance->get_vk_instance(), handle, nullptr);
        handle = VK_NULL_HANDLE;
    };

private:
    VkSurfaceKHR handle;
    shared_ptr<Instance> instance;
    shared_ptr<Window> window;
};