#pragma once
#include "QueueFamilies.h"
#include "SwapChainSupportDetails.h"

#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>

namespace loid {

namespace system {

namespace rendering {

class LogicalDevice {
public:
    ~LogicalDevice();
    LogicalDevice(const VkInstance&, const VkSurfaceKHR&);
    LogicalDevice(const LogicalDevice&) = delete;

    const VkDevice& get_device() const noexcept {
        return device;
    }

private:
    void pick_physical_device(const VkInstance&, const VkSurfaceKHR&);

    bool is_device_suitable(const VkPhysicalDevice&, const VkSurfaceKHR&) noexcept;
    
    bool check_device_extension_support(const VkPhysicalDevice& physical_device) const noexcept;
    
    SwapChainSupportDetails query_swap_chain_support(const VkPhysicalDevice&, const VkSurfaceKHR&) const noexcept;

    void create_logical_device();

    std::vector<VkDeviceQueueCreateInfo> get_device_queues_info() const noexcept;

    VkDeviceCreateInfo get_logical_device_create_info(auto&, auto&) const noexcept;
private:
    VkPhysicalDevice physical_device;
    VkDevice device;
    std::unique_ptr<QueueFamilies> queue_families;
    VkQueue graphics_queue;
    VkQueue present_queue;
};

} // namespace rendering

} // namespace system 

} // namespace loid
