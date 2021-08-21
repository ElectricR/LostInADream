#include "VulkanInstance.h"

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <ranges>
#include <vector>
#include <iostream>

#ifdef NDEBUG
    constexpr bool ENABLE_VALIDATION_LAYERS = false;
#else
    constexpr bool ENABLE_VALIDATION_LAYERS = true;

const std::vector<const char *> required_validation_layers = {
    "VK_LAYER_KHRONOS_validation"
};

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT,
        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
        void*) {
    if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
        std::cerr << "validation layer: " << callback_data->pMessage << std::endl;
    }
    return VK_FALSE;
}

#endif

loid::system::rendering::VulkanInstance::~VulkanInstance() {
    if constexpr (ENABLE_VALIDATION_LAYERS) {
        this->destroy_debug_utils_messenger();
    }

    vkDestroyInstance(instance, nullptr);
}

loid::system::rendering::VulkanInstance::VulkanInstance() {
    if constexpr (ENABLE_VALIDATION_LAYERS) {
        if (!this->check_validation_layer_support()) {
            throw std::runtime_error("Validation layers requested, but not available!");
        }
    }

    auto app_info = this->get_app_info();
    auto required_extensions = this->get_required_extensions();
    auto debug_messenger_info = this->get_debug_messanger_info();

    auto create_instance_info = this->get_instance_info(app_info, required_extensions, debug_messenger_info);

    if (vkCreateInstance(&create_instance_info, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create instance!");
    }

    if constexpr (ENABLE_VALIDATION_LAYERS) {
        this->setup_debug_messanger(debug_messenger_info);
    }
}

bool loid::system::rendering::VulkanInstance::check_validation_layer_support() const noexcept {
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    std::vector<std::string> available_layers_names{};
    std::transform(available_layers.cbegin(), available_layers.cend(), std::back_inserter(available_layers_names), [](const VkLayerProperties& layer) -> std::string { return layer.layerName; });

    for (auto layer_name : required_validation_layers) {
        if (std::find(available_layers_names.begin(), available_layers_names.end(), layer_name) == available_layers_names.end()) {
            return false;
        }
    }

    return true;
}

VkApplicationInfo loid::system::rendering::VulkanInstance::get_app_info() const noexcept {
    VkApplicationInfo app_info {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Lost In A Dream";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_2;

    return app_info;
}


std::vector<const char *> loid::system::rendering::VulkanInstance::get_required_extensions() const {
    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    if (!glfw_extensions) {
        throw std::runtime_error("GLFW couldn't query required extensions");
    }

    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

    if constexpr (ENABLE_VALIDATION_LAYERS) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

VkDebugUtilsMessengerCreateInfoEXT loid::system::rendering::VulkanInstance::get_debug_messanger_info() const noexcept {
    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info{};

    debug_messenger_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_messenger_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT 
        /*
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT 
        */
        ;
    debug_messenger_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_messenger_info.pfnUserCallback = debug_callback;

    return debug_messenger_info;
}

VkInstanceCreateInfo loid::system::rendering::VulkanInstance::get_instance_info(const VkApplicationInfo& app_info, const std::vector<const char *>& extensions, const VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_info) const noexcept {

    VkInstanceCreateInfo instance_info{};
    instance_info.pApplicationInfo = &app_info;
    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instance_info.ppEnabledExtensionNames = extensions.data();

    if constexpr (ENABLE_VALIDATION_LAYERS) {
        instance_info.enabledLayerCount = static_cast<uint32_t>(required_validation_layers.size());
        instance_info.ppEnabledLayerNames = required_validation_layers.data();
        instance_info.pNext = &debug_messenger_info;
    }

    return instance_info;
}

void loid::system::rendering::VulkanInstance::setup_debug_messanger(const VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_info) {
    if (this->create_debug_utils_messenger(debug_messenger_info) != VK_SUCCESS) {
        vkDestroyInstance(instance, nullptr);
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

VkResult loid::system::rendering::VulkanInstance::create_debug_utils_messenger(const VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_info) {
    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
    if (func) {
        return func(instance, &debug_messenger_info, nullptr, &debug_messenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void loid::system::rendering::VulkanInstance::destroy_debug_utils_messenger() noexcept {
    auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    if (func) {
        func(instance, debug_messenger, nullptr);
    }
}

