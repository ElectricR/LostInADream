#include "LogicalDevice.h"


#include <stdexcept>
#include <vector>

const std::vector<const char*> device_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

loid::system::rendering::LogicalDevice::LogicalDevice(const VkInstance& instance, const VkSurfaceKHR& window) {
    this->pick_physical_device(instance, window);
}

loid::system::rendering::LogicalDevice::~LogicalDevice() {
}
    
void loid::system::rendering::LogicalDevice::pick_physical_device(const VkInstance &instance, const VkSurfaceKHR& window) {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    if (!device_count) {
        throw std::runtime_error("Failed to find GPU with Vulkan support!");
    }
    
    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    auto device_it = std::find_if(devices.cbegin(), devices.cend(), [&](const VkPhysicalDevice &device_entry) { return this->is_device_suitable(device_entry, window); });
    if (device_it == devices.end()) {
        throw std::runtime_error("Failed to find a suitable GPU!");
    } 
    physical_device = *device_it;
}

bool loid::system::rendering::LogicalDevice::is_device_suitable(const VkPhysicalDevice& physical_device_entry, const VkSurfaceKHR& surface) noexcept {
    queue_families = std::make_unique<QueueFamilies>(physical_device_entry, surface);

    bool extensions_supported = check_device_extension_support(physical_device_entry);

    bool swap_chain_adequate = false;
    if (extensions_supported) {
        SwapChainSupportDetails swap_chain_support = query_swap_chain_support(physical_device_entry, surface);
        swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
    }

    VkPhysicalDeviceFeatures supported_features;
    vkGetPhysicalDeviceFeatures(physical_device_entry, &supported_features);

    return queue_families->is_complete() && extensions_supported && swap_chain_adequate && supported_features.samplerAnisotropy;
} 

bool loid::system::rendering::LogicalDevice::check_device_extension_support(const VkPhysicalDevice& physical_device_entry) const noexcept {
    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(physical_device_entry, nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(physical_device_entry, nullptr, &extension_count, available_extensions.data());

    std::vector<std::string> required_extensions(device_extensions.cbegin(), device_extensions.cend());
    std::vector<std::string> available_extensions_strings;
    std::transform(available_extensions.cbegin(), available_extensions.cend(), std::back_inserter(available_extensions_strings), [](const VkExtensionProperties& ext) { return ext.extensionName; });
    return std::all_of(required_extensions.cbegin(), required_extensions.cend(), [&available_extensions_strings](const std::string& ext) { return std::find(available_extensions_strings.cbegin(), available_extensions_strings.cend(), ext) != available_extensions_strings.end(); });
}

loid::system::rendering::SwapChainSupportDetails loid::system::rendering::LogicalDevice::query_swap_chain_support(const VkPhysicalDevice& physical_device_entry, const VkSurfaceKHR& surface) const noexcept {
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device_entry, surface, &details.capabilities);

    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device_entry, surface, &format_count, nullptr);

    if (format_count != 0) {
        details.formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device_entry, surface, &format_count, details.formats.data());
    }

    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device_entry, surface, &present_mode_count, nullptr);

    if (present_mode_count != 0) {
        details.present_modes.resize(present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device_entry, surface, &present_mode_count, details.present_modes.data());
    }

    return details;
}
