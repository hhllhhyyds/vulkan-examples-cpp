#include <limits> // Necessary for std::numeric_limits

#include "swapchain.h"

VkSurfaceFormatKHR choose_surface_format(const std::vector<VkSurfaceFormatKHR> &available_formats)
{
    for (const auto &format : available_formats)
    {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return format;
        }
    }

    return available_formats.at(0);
}

VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR> &available_present_modes)
{
    for (const auto &mode : available_present_modes)
    {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return mode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

uint32_t clamp(uint32_t x, uint32_t min, uint32_t max)
{
    if (x < min)
    {
        return min;
    }
    if (x > max)
    {
        return max;
    }
    return x;
}

VkExtent2D choose_surface_extent(const VkSurfaceCapabilitiesKHR &capabilities, std::shared_ptr<Window> window)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        glfwGetFramebufferSize(window->get_glfw_window(), &width, &height);

        VkExtent2D actual_extent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)};

        actual_extent.width = clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actual_extent.height = clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actual_extent;
    }
}

Swapchain::Swapchain(std::shared_ptr<Window> window, std::shared_ptr<Surface> surface,
                     std::shared_ptr<LogicalDevice> device) : device(device)
{
    auto surface_format = choose_surface_format(device->get_physical()->surface_formats());
    auto present_mode = choose_present_mode(device->get_physical()->surface_present_modes());
    auto cap = device->get_physical()->surface_cap();
    auto surface_extent = choose_surface_extent(cap, window);

    uint32_t image_count = 0;
    {
        image_count = cap.minImageCount + 1;
        if (cap.maxImageCount > 0 && image_count > cap.maxImageCount)
        {
            image_count = cap.maxImageCount;
        }
    }

    VkSwapchainCreateInfoKHR create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = surface->get_vk_surface_khr();
    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.presentMode = present_mode;
    create_info.imageExtent = surface_extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    create_info.preTransform = cap.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = VK_NULL_HANDLE; // TODO: use old swapchain

    if (vkCreateSwapchainKHR(device->get_vk_device(), &create_info, nullptr, &handle) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain!");
    }

    image_format = surface_format.format;
    extent = surface_extent;

    vkGetSwapchainImagesKHR(device->get_vk_device(), handle, &image_count, nullptr);
    images.resize(image_count);
    vkGetSwapchainImagesKHR(device->get_vk_device(), handle, &image_count, images.data());
}

Swapchain::~Swapchain()
{
    vkDestroySwapchainKHR(device->get_vk_device(), handle, nullptr);
}

VkImageView Swapchain::create_swapchain_image_view(VkImage image)
{
    VkImageViewCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.image = image;
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = image_format;
    create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;

    VkImageView view;
    if (vkCreateImageView(device->get_vk_device(), &create_info, nullptr, &view) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image views!");
    }
    return view;
}