#include "device_queue.h"

#if __APPLE__
const std::vector<const char *> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    "VK_KHR_portability_subset"};
#else
const std::vector<const char *> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#endif

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

bool device_extension_support(VkPhysicalDevice device)
{
    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

    for (const auto &required : DEVICE_EXTENSIONS)
    {
        std::string required_s(required);
        bool found = false;
        for (const auto &extension : available_extensions)
        {
            std::string extension_s(extension.extensionName);
            if (required_s == extension_s)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            return false;
        }
    }

    return true;
}

VkSurfaceCapabilitiesKHR get_surface_cap(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities);
    return capabilities;
}

std::vector<VkSurfaceFormatKHR> get_surface_formats(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);

    std::vector<VkSurfaceFormatKHR> formats;
    if (format_count != 0)
    {
        formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, formats.data());
    }

    return formats;
}

std::vector<VkPresentModeKHR> get_surface_present_modes(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &mode_count, nullptr);

    std::vector<VkPresentModeKHR> modes;
    if (mode_count != 0)
    {
        modes.resize(mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &mode_count, modes.data());
    }

    return modes;
}

int device_match_request(VkPhysicalDevice device, const shared_ptr<Surface> surface)
{
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

    if (!device_extension_support(device))
    {
        return -1;
    }

    if (get_surface_formats(device, surface->get_vk_surface_khr()).empty() ||
        get_surface_present_modes(device, surface->get_vk_surface_khr()).empty())
    {
        return -1;
    }

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

PhysicalDevice::PhysicalDevice(shared_ptr<Instance> instance, shared_ptr<Surface> surface) : instance(instance), surface(surface)
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

VkSurfaceCapabilitiesKHR PhysicalDevice::surface_cap() const
{
    return get_surface_cap(handle, surface->get_vk_surface_khr());
}

std::vector<VkSurfaceFormatKHR> PhysicalDevice::surface_formats() const
{
    return get_surface_formats(handle, surface->get_vk_surface_khr());
}

std::vector<VkPresentModeKHR> PhysicalDevice::surface_present_modes() const
{
    return get_surface_present_modes(handle, surface->get_vk_surface_khr());
}

LogicalDevice::LogicalDevice(shared_ptr<PhysicalDevice> physical_device) : physical(physical_device)
{
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = physical_device->get_queue_family_index();
    queue_create_info.queueCount = 1;
    float queue_priority = 1.0f;
    queue_create_info.pQueuePriorities = &queue_priority;

    VkPhysicalDeviceFeatures device_features{};

    VkDeviceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.pQueueCreateInfos = &queue_create_info;
    create_info.queueCreateInfoCount = 1;
    create_info.pEnabledFeatures = &device_features;

    create_info.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
    create_info.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

    if (ENABLE_VALIDATION_LAYERS)
    {
        create_info.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    }
    else
    {
        create_info.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physical_device->get_vk_physical_device(), &create_info, nullptr, &handle) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(handle, physical_device->get_queue_family_index(), 0, &queue);
}