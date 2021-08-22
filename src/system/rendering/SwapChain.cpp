#include "SwapChain.h"


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>

loid::system::rendering::SwapChain::~SwapChain() {
    vkDestroySwapchainKHR(logical_device->get_device(), swap_chain, nullptr);
}

loid::system::rendering::SwapChain::SwapChain(Window& window, std::shared_ptr<LogicalDevice> logical_device_ptr):
    logical_device(logical_device_ptr)
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(window.get_window(), &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window.get_window(), &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(logical_device->get_device());

    this->create_swap_chain(window);
    this->get_swap_chain_images();
    this->create_swap_chain_image_views();
}

void loid::system::rendering::SwapChain::create_swap_chain(Window& window) {
    ContextDetails context_details = logical_device->get_context_details(window.get_surface());

    VkSurfaceFormatKHR surface_format = this->choose_swap_surface_format(context_details.formats);
    VkPresentModeKHR present_mode = this->choose_swap_present_mode(context_details.present_modes);
    VkExtent2D extent = this->choose_swap_extent(context_details.capabilities, window.get_window());
    std::uint32_t image_count = this->calc_image_count(context_details.capabilities);

    auto queue_families = logical_device->get_queue_families();
    std::array queue_family_indices = {queue_families.get_graphics_family_index(), queue_families.get_present_family_index()};

    VkSwapchainCreateInfoKHR swap_chain_info = this->create_swap_chain_info(context_details, surface_format, present_mode, extent, window.get_surface(), image_count, queue_family_indices);
    if (vkCreateSwapchainKHR(logical_device->get_device(), &swap_chain_info, nullptr, &swap_chain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swap chain!");
    }
    
    swap_chain_image_format = surface_format.format;
    swap_chain_extent = extent;
}

VkSurfaceFormatKHR loid::system::rendering::SwapChain::choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats) const noexcept {
    auto format = std::find_if(available_formats.cbegin(), available_formats.cend(), [](const VkSurfaceFormatKHR& form) {
            return form.format == VK_FORMAT_B8G8R8A8_SRGB && form.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; 
        });

    if (format != available_formats.end()) {
        return *format;
    }
    return available_formats[0];
}

VkPresentModeKHR loid::system::rendering::SwapChain::choose_swap_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes) const noexcept {
    auto present_mode = std::find_if(available_present_modes.cbegin(), available_present_modes.cend(), [](const VkPresentModeKHR pres) { return pres == VK_PRESENT_MODE_MAILBOX_KHR; });
    if (present_mode != available_present_modes.end()) {
        return *present_mode;
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D loid::system::rendering::SwapChain::choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow *window) const noexcept {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actual_extent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actual_extent.width = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actual_extent;
    }
}

std::uint32_t loid::system::rendering::SwapChain::calc_image_count(const VkSurfaceCapabilitiesKHR& capabilities) const noexcept {
    std::uint32_t image_count = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && image_count > capabilities.maxImageCount) {
        image_count = capabilities.maxImageCount;
    }
    return image_count;
}

VkSwapchainCreateInfoKHR loid::system::rendering::SwapChain::create_swap_chain_info(const ContextDetails& context_details, const VkSurfaceFormatKHR& surface_format, const VkPresentModeKHR& present_mode, const VkExtent2D& extent, const VkSurfaceKHR& surface, std::uint32_t image_count,  const std::array<std::uint32_t, 2>& queue_families) const noexcept {
    VkSwapchainCreateInfoKHR swap_chain_info{};
    swap_chain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swap_chain_info.surface = surface;
    swap_chain_info.minImageCount = image_count;
    swap_chain_info.imageFormat = surface_format.format;
    swap_chain_info.imageColorSpace = surface_format.colorSpace;
    swap_chain_info.imageExtent = extent;
    swap_chain_info.imageArrayLayers = 1;
    swap_chain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (queue_families[0] != queue_families[1]) {
        swap_chain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swap_chain_info.queueFamilyIndexCount = 2;
        swap_chain_info.pQueueFamilyIndices = queue_families.data();
    } else {
        swap_chain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    swap_chain_info.preTransform = context_details.capabilities.currentTransform;
    swap_chain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swap_chain_info.presentMode = present_mode;
    swap_chain_info.clipped = VK_TRUE;
    swap_chain_info.oldSwapchain = VK_NULL_HANDLE;

    return swap_chain_info;
}

void loid::system::rendering::SwapChain::get_swap_chain_images() noexcept {
    std::uint32_t image_count = 0;
    vkGetSwapchainImagesKHR(logical_device->get_device(), swap_chain, &image_count, nullptr);
    swap_chain_images.resize(image_count);
    vkGetSwapchainImagesKHR(logical_device->get_device(), swap_chain, &image_count, swap_chain_images.data());
}

void loid::system::rendering::SwapChain::create_swap_chain_image_views() {
    /*
    swap_chain_image_views.resize(swap_chain_images.size());

    for (size_t i = 0; i < swap_chain_images.size(); ++i) {
        VkImageViewCreateInfo image_view_create_info{};
        image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.image = swap_chain_images[i];
        image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.format = swap_chain_image_format;
        image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_create_info.subresourceRange.baseMipLevel = 0;
        image_view_create_info.subresourceRange.levelCount = 1;
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device, &image_view_create_info, nullptr, &swap_chain_image_views[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }
    */
}
