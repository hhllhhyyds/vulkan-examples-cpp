#pragma once

#include "device_queue.h"

class Swapchain
{
public:
    Swapchain(std::shared_ptr<Window> window, std::shared_ptr<Surface> surface, std::shared_ptr<LogicalDevice> device);
    Swapchain(std::shared_ptr<Surface> surface, std::shared_ptr<LogicalDevice> device) : Swapchain(surface->get_window(), surface, device) {}

    Swapchain(const Swapchain &) = delete;
    Swapchain &operator=(const Swapchain &) = delete;
    Swapchain(const Swapchain &&) = delete;
    Swapchain &operator=(const Swapchain &&) = delete;

    ~Swapchain();

    const std::vector<VkImage> &get_swapchain_images() const
    {
        return images;
    }

    VkFormat get_image_format() const
    {
        return image_format;
    }

    VkExtent2D get_image_extent() const
    {
        return extent;
    }

    VkImageView create_swapchain_image_view(VkImage image);

private:
    VkSwapchainKHR handle;
    std::shared_ptr<LogicalDevice> device;

    VkFormat image_format;
    VkExtent2D extent;
    std::vector<VkImage> images;
};