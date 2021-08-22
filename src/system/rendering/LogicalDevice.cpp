#include "LogicalDevice.h"
#include "utils.h"


#include <set>
#include <vector>

#ifdef NDEBUG
    constexpr static bool ENABLE_VALIDATION_LAYERS = false;
#else
    constexpr static bool ENABLE_VALIDATION_LAYERS = true;

    const std::vector<const char *> required_validation_layers = {
        "VK_LAYER_KHRONOS_validation"
    };

#endif

const std::vector<const char*> device_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

loid::system::rendering::LogicalDevice::LogicalDevice(const VkInstance& instance, const VkSurfaceKHR& window) {
    this->pick_physical_device(instance, window);
    this->create_logical_device();
}

loid::system::rendering::LogicalDevice::~LogicalDevice() {
    vkDestroyDevice(device, nullptr);
}

loid::system::rendering::ContextDetails loid::system::rendering::LogicalDevice::get_context_details(const VkSurfaceKHR& surface) const noexcept {
    return this->query_context_details(this->physical_device, surface);
}
    
void loid::system::rendering::LogicalDevice::pick_physical_device(const VkInstance &instance, const VkSurfaceKHR& window) {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    if (!device_count) {
        throw RenderingError("Failed to find GPU with Vulkan support!");
    }
    
    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    auto device_it = std::find_if(devices.cbegin(), devices.cend(), [&](const VkPhysicalDevice &device_entry) { return this->is_device_suitable(device_entry, window); });
    if (device_it == devices.end()) {
        throw RenderingError("Failed to find a suitable GPU!");
    } 
    physical_device = *device_it;
}

bool loid::system::rendering::LogicalDevice::is_device_suitable(const VkPhysicalDevice& physical_device_entry, const VkSurfaceKHR& surface) noexcept {
    queue_families = std::make_unique<QueueFamilies>(physical_device_entry, surface);

    bool extensions_supported = check_device_extension_support(physical_device_entry);

    bool swap_chain_adequate = false;
    if (extensions_supported) {
        ContextDetails swap_chain_support = query_context_details(physical_device_entry, surface);
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

loid::system::rendering::ContextDetails loid::system::rendering::LogicalDevice::query_context_details(const VkPhysicalDevice& physical_device_entry, const VkSurfaceKHR& surface) const noexcept {
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

void loid::system::rendering::LogicalDevice::create_logical_device() {
    auto queue_create_infos = this->get_device_queues_info();

    VkPhysicalDeviceFeatures device_features{};
    device_features.samplerAnisotropy = VK_TRUE;

    auto logical_device_create_info = this->get_logical_device_create_info(queue_create_infos, device_features);

    check_vulkan_call(vkCreateDevice(physical_device, &logical_device_create_info, nullptr, &device), "Failed to create logical device");

    vkGetDeviceQueue(device, queue_families->get_graphics_family_index(), 0, &graphics_queue);
    vkGetDeviceQueue(device, queue_families->get_present_family_index(), 0, &present_queue);
}

std::vector<VkDeviceQueueCreateInfo> loid::system::rendering::LogicalDevice::get_device_queues_info() const noexcept {
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::set<uint32_t> queue_required_families = { queue_families->get_graphics_family_index(), queue_families->get_present_family_index() };
    float queue_priority = 1.f;

    for (uint32_t queue_family_index : queue_required_families) {
        VkDeviceQueueCreateInfo queue_create_info{};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family_index;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(std::move(queue_create_info));
    }

    return queue_create_infos;
}

VkDeviceCreateInfo loid::system::rendering::LogicalDevice::get_logical_device_create_info(auto& queue_create_infos, auto& device_features) const noexcept {
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

