#include "LogicalDeviceWrapper.h"
#include "../utils.h"

#include <set>

#ifdef NDEBUG
    constexpr static bool ENABLE_VALIDATION_LAYERS = false;
#else
    constexpr static bool ENABLE_VALIDATION_LAYERS = true;

    const std::vector<const char *> required_validation_layers = {
        "VK_LAYER_KHRONOS_validation"
    };

#endif

loid::system::rendering::wrapper::LogicalDeviceWrapper::LogicalDeviceWrapper(const VkPhysicalDevice& physical_device, const VkSurfaceKHR& surface):
    queue_families(std::make_unique<QueueFamilies>(physical_device, surface))
{
    auto queue_create_infos = this->get_device_queues_info();

    VkPhysicalDeviceFeatures device_features{};
    device_features.samplerAnisotropy = VK_TRUE;

    auto logical_device_create_info = this->get_logical_device_create_info(queue_create_infos, device_features);

    check_vulkan_call(vkCreateDevice(physical_device, &logical_device_create_info, nullptr, &device), "Failed to create logical device");

    vkGetDeviceQueue(device, queue_families->get_graphics_family_index(), 0, &graphics_queue);
    vkGetDeviceQueue(device, queue_families->get_present_family_index(), 0, &present_queue);
}

std::vector<VkDeviceQueueCreateInfo> loid::system::rendering::wrapper::LogicalDeviceWrapper::get_device_queues_info() const noexcept {
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::set<std::uint32_t> queue_required_families = { queue_families->get_graphics_family_index(), queue_families->get_present_family_index() };
    float queue_priority = 1.f;

    for (std::uint32_t queue_family_index : queue_required_families) {
        VkDeviceQueueCreateInfo queue_create_info{};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family_index;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(std::move(queue_create_info));
    }

    return queue_create_infos;
}

VkDeviceCreateInfo loid::system::rendering::wrapper::LogicalDeviceWrapper::get_logical_device_create_info(auto& queue_create_infos, auto& device_features) const noexcept {
    VkDeviceCreateInfo logical_device_create_info{};
    logical_device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    logical_device_create_info.pQueueCreateInfos = queue_create_infos.data();
    logical_device_create_info.queueCreateInfoCount = static_cast<std::uint32_t>(queue_create_infos.size());
    logical_device_create_info.pEnabledFeatures = &device_features;
    logical_device_create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
    logical_device_create_info.ppEnabledExtensionNames = device_extensions.data();

    if constexpr (ENABLE_VALIDATION_LAYERS) {
        logical_device_create_info.enabledLayerCount = static_cast<uint32_t>(required_validation_layers.size());
        logical_device_create_info.ppEnabledLayerNames = required_validation_layers.data();
    }

    return logical_device_create_info;
}

