#include "InstanceWrapper.h"
#include "../utils.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#ifdef NDEBUG
    constexpr static bool ENABLE_VALIDATION_LAYERS = false;
#else
    constexpr static bool ENABLE_VALIDATION_LAYERS = true;

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

loid::system::rendering::wrapper::InstanceWrapper::InstanceWrapper() {
    auto app_info = this->get_app_info();
    auto required_extensions = this->get_required_extensions();
    auto debug_messenger_info = this->get_debug_messanger_info();

    auto create_instance_info = this->get_instance_info(app_info, required_extensions, debug_messenger_info);

    check_vulkan_call(vkCreateInstance(&create_instance_info, nullptr, &instance), "Failed to create instance!");
}


VkApplicationInfo loid::system::rendering::wrapper::InstanceWrapper::get_app_info() const noexcept {
    VkApplicationInfo app_info {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Lost In A Dream";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_2;

    return app_info;
}


std::vector<const char *> loid::system::rendering::wrapper::InstanceWrapper::get_required_extensions() const {
    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    if (!glfw_extensions) {
        throw RenderingError("GLFW couldn't query required extensions");
    }

    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

    if constexpr (ENABLE_VALIDATION_LAYERS) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}


VkDebugUtilsMessengerCreateInfoEXT loid::system::rendering::wrapper::InstanceWrapper::get_debug_messanger_info() const noexcept {
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


VkInstanceCreateInfo loid::system::rendering::wrapper::InstanceWrapper::get_instance_info(const VkApplicationInfo& app_info, const std::vector<const char *>& extensions, const VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_info) const noexcept {

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

