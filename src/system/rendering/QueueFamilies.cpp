#include "QueueFamilies.h"

#include <vector>
#include <algorithm>

loid::system::rendering::QueueFamilies::QueueFamilies(const VkPhysicalDevice& physical_device, const VkSurfaceKHR& surface) {
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_family_properties(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_family_properties.data());

    auto queue_it = std::find_if(queue_family_properties.cbegin(), queue_family_properties.cend(), [](const VkQueueFamilyProperties& prop) { return prop.queueFlags & VK_QUEUE_GRAPHICS_BIT; });
    if (queue_it != queue_family_properties.end()) {
        this->graphics_family_index = std::distance(queue_family_properties.cbegin(), queue_it);
    }

    for (std::uint32_t i = 0; i != queue_family_properties.size(); ++i) {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &presentSupport);
        if (presentSupport) {
            present_family_index = i;
        }
        
        if (this->is_complete()) {
            break;
        }
    }
}
