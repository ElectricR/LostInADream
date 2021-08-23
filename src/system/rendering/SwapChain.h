#pragma once
#include "Window.h"
#include "Device.h"

#include <memory>

namespace loid {

namespace system {

namespace rendering {

class SwapChain {
public:
    SwapChain(Window&, std::shared_ptr<Device>);
    
    ~SwapChain();

private:
    void create_swap_chain(Window&);
    
    VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>&) const noexcept;
    
    VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>&) const noexcept;
    
    VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR&, GLFWwindow *) const noexcept;
    std::uint32_t calc_image_count(const VkSurfaceCapabilitiesKHR&) const noexcept ;

    VkSwapchainCreateInfoKHR create_swap_chain_info(const ContextDetails&, const VkSurfaceFormatKHR&, const VkPresentModeKHR&, const VkExtent2D&, const VkSurfaceKHR&, std::uint32_t,  const std::array<std::uint32_t, 2>&) const noexcept;

    void get_swap_chain_images() noexcept;

    void create_swap_chain_image_views();
private:
    std::shared_ptr<Device> logical_device;
private:
    VkSwapchainKHR swap_chain{};
    std::vector<VkImage> swap_chain_images;
    VkFormat swap_chain_image_format{};
    VkExtent2D swap_chain_extent{};
    std::vector<VkImageView> swap_chain_image_views;
};

}

}

}
