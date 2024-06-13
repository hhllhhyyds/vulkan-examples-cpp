#pragma once

#include <vector>
#include <stdexcept>

#include "glfw_vulkan.h"

using std::string;
using std::vector;

class Instance
{
public:
    Instance(const char *app_name = "VulkanApp");
    Instance(const Instance &) = delete;
    Instance &operator=(const Instance &) = delete;
    Instance(const Instance &&) = delete;
    Instance &operator=(const Instance &&) = delete;

    ~Instance();

    VkInstance get_vk_instance() const
    {
        return handle;
    }

private:
    VkInstance handle;
    VkDebugUtilsMessengerEXT debug_messenger;
};