#include "Device.h"
#include "utils.h"

#include <set>
#include <vector>

loid::system::rendering::Device::Device(const VkInstance& instance, const VkSurfaceKHR& surface) {
    this->pick_physical_device(instance, surface);
    logical_device_wrapper = std::make_shared<wrapper::LogicalDeviceWrapper>(physical_device, surface);
}


loid::system::rendering::ContextDetails loid::system::rendering::Device::get_context_details(const VkSurfaceKHR& surface) const noexcept {
    return this->query_context_details(this->physical_device, surface);
}
    
void loid::system::rendering::Device::pick_physical_device(const VkInstance &instance, const VkSurfaceKHR& surface) {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    if (!device_count) {
        throw RenderingError("Failed to find GPU with Vulkan support!");
    }
    
    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    auto device_it = std::find_if(devices.cbegin(), devices.cend(), [&](const VkPhysicalDevice &device_entry) { return this->is_device_suitable(device_entry, surface); });
    if (device_it == devices.end()) {
        throw RenderingError("Failed to find a suitable GPU!");
    } 
    physical_device = *device_it;
}

bool loid::system::rendering::Device::is_device_suitable(const VkPhysicalDevice& physical_device_entry, const VkSurfaceKHR& surface) noexcept {
    QueueFamilies queue_families = QueueFamilies(physical_device_entry, surface);

    bool extensions_supported = check_device_extension_support(physical_device_entry);

    bool swap_chain_adequate = false;
    if (extensions_supported) {
        ContextDetails swap_chain_support = query_context_details(physical_device_entry, surface);
        swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
    }

    VkPhysicalDeviceFeatures supported_features;
    vkGetPhysicalDeviceFeatures(physical_device_entry, &supported_features);

    return queue_families.is_complete() && extensions_supported && swap_chain_adequate && supported_features.samplerAnisotropy;
} 

bool loid::system::rendering::Device::check_device_extension_support(const VkPhysicalDevice& physical_device_entry) const noexcept {
    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(physical_device_entry, nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(physical_device_entry, nullptr, &extension_count, available_extensions.data());

    std::vector<std::string> required_extensions(device_extensions.cbegin(), device_extensions.cend());
    std::vector<std::string> available_extensions_strings;
    std::transform(available_extensions.cbegin(), available_extensions.cend(), std::back_inserter(available_extensions_strings), [](const VkExtensionProperties& ext) { return ext.extensionName; });
    return std::all_of(required_extensions.cbegin(), required_extensions.cend(), [&available_extensions_strings](const std::string& ext) { return std::find(available_extensions_strings.cbegin(), available_extensions_strings.cend(), ext) != available_extensions_strings.end(); });
}

loid::system::rendering::ContextDetails loid::system::rendering::Device::query_context_details(const VkPhysicalDevice& physical_device_entry, const VkSurfaceKHR& surface) const noexcept {
    ContextDetails details;
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
