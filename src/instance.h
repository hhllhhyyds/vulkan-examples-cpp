#pragma once

#include <vector>
#include <stdexcept>

#include "window.h"

using std::string;
using std::vector;

class Instance
{
public:
    Instance(const string &app_name = "VulkanApp");
    Instance(const Instance &) = delete;
    Instance &operator=(const Instance &) = delete;
    Instance(const Instance &&) = delete;
    Instance &operator=(const Instance &&) = delete;

    ~Instance();

private:
    VkInstance handle;
    VkDebugUtilsMessengerEXT debug_messenger;
};