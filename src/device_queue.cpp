#include "device_queue.h"

int device_priority(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(device, &device_properties);

    int score = 0;

    switch (device_properties.deviceType)
    {
    case VK_PHYSICAL_DEVICE_TYPE_OTHER:
        score = 1;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
        score = 2;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_CPU:
        score = 3;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
        score = 4;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        score = 5;
        break;
    default:
        break;
    }

    return score;
}

int device_match_request(VkPhysicalDevice device, const shared_ptr<Surface> surface)
{
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

    int i = 0;
    for (const auto &queue_family : queue_families)
    {
        if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            VkBool32 present_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface->get_vk_surface_khr(), &present_support);
            if (present_support)
            {
                return i;
            }
        }
        i++;
    }

    return -1;
}

PhysicalDevice::PhysicalDevice(shared_ptr<Instance> instance, shared_ptr<Surface> surface) : instance(instance)
{
    uint32_t device_count;
    vkEnumeratePhysicalDevices(instance->get_vk_instance(), &device_count, nullptr);
    if (device_count == 0)
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance->get_vk_instance(), &device_count, devices.data());

    int current_priority = 0;
    for (const auto &device : devices)
    {
        int priority = device_priority(device);
        int queue_family = device_match_request(device, surface);
        if (queue_family >= 0 && priority > current_priority)
        {
            handle = device;
            queue_family_index = queue_family;
            current_priority = priority;
        }
    }

    if (current_priority <= 0 || queue_family_index < 0)
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}