#pragma once
#include "ContextDetails.h"
#include "wrappers/LogicalDeviceWrapper.h"

#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>

namespace loid {

namespace system {

namespace rendering {

class Device {
public:
    Device(const VkInstance&, const VkSurfaceKHR&);

    const VkDevice& get_device() const noexcept {
        return logical_device_wrapper->get_device();
    }

    ContextDetails get_context_details(const VkSurfaceKHR&) const noexcept;

    QueueFamilies get_queue_families() const noexcept {
        return logical_device_wrapper->get_queue_families();
    }

private:
    void pick_physical_device(const VkInstance&, const VkSurfaceKHR&);

    bool is_device_suitable(const VkPhysicalDevice&, const VkSurfaceKHR&) noexcept;
    
    ContextDetails query_context_details(const VkPhysicalDevice&, const VkSurfaceKHR&) const noexcept;

    bool check_device_extension_support(const VkPhysicalDevice& physical_device) const noexcept;
    
private:
    VkPhysicalDevice physical_device;
    std::shared_ptr<wrapper::LogicalDeviceWrapper> logical_device_wrapper;
};

} // namespace rendering

} // namespace system 

} // namespace loid
