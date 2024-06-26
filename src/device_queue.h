#pragma once

#include <memory>
#include <cstdlib>
#include <vector>

#include "instance.h"
#include "surface.h"
#include "glfw_vulkan.h"

using std::shared_ptr;

class PhysicalDevice
{
public:
    PhysicalDevice(shared_ptr<Instance> instance, shared_ptr<Surface> surface);

    PhysicalDevice(const PhysicalDevice &) = delete;
    PhysicalDevice &operator=(const PhysicalDevice &) = delete;
    PhysicalDevice(const PhysicalDevice &&) = delete;
    PhysicalDevice &operator=(const PhysicalDevice &&) = delete;

    ~PhysicalDevice()
    {
        handle = VK_NULL_HANDLE;
    }

    VkPhysicalDevice get_vk_physical_device() const
    {
        return handle;
    }

    uint32_t get_queue_family_index() const
    {
        return queue_family_index;
    }

    VkSurfaceCapabilitiesKHR surface_cap() const;
    std::vector<VkSurfaceFormatKHR> surface_formats() const;
    std::vector<VkPresentModeKHR> surface_present_modes() const;

private:
    VkPhysicalDevice handle;
    shared_ptr<Instance> instance;
    shared_ptr<Surface> surface;
    uint32_t queue_family_index;
};

class LogicalDevice
{
public:
    LogicalDevice(shared_ptr<PhysicalDevice> physical_device);

    LogicalDevice(const LogicalDevice &) = delete;
    LogicalDevice &operator=(const LogicalDevice &) = delete;
    LogicalDevice(const LogicalDevice &&) = delete;
    LogicalDevice &operator=(const LogicalDevice &&) = delete;

    ~LogicalDevice()
    {
        vkDestroyDevice(handle, nullptr);
        handle = VK_NULL_HANDLE;
        queue = VK_NULL_HANDLE;
    }

    VkDevice get_vk_device() const
    {
        return handle;
    }

    VkQueue get_vk_queue() const
    {
        return queue;
    }

    shared_ptr<PhysicalDevice> get_physical() const
    {
        return physical;
    }

private:
    VkDevice handle;
    VkQueue queue;
    shared_ptr<PhysicalDevice> physical;
};
