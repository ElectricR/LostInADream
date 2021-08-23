#include "DebugMessangerWrapper.h"
#include "../utils.h"

#ifdef NDEBUG
    constexpr static bool ENABLE_VALIDATION_LAYERS = false;
#else
    constexpr static bool ENABLE_VALIDATION_LAYERS = true;
#endif

loid::system::rendering::wrapper::DebugMessangerWrapper::~DebugMessangerWrapper() {
    if constexpr (ENABLE_VALIDATION_LAYERS) {
        this->destroy_debug_utils_messenger();
    }
}


loid::system::rendering::wrapper::DebugMessangerWrapper::DebugMessangerWrapper(const std::shared_ptr<InstanceWrapper>& instance_wrapper):
    instance_wrapper(instance_wrapper)
{
    if constexpr (ENABLE_VALIDATION_LAYERS) {
        this->setup_debug_messanger();
    }
}


void loid::system::rendering::wrapper::DebugMessangerWrapper::setup_debug_messanger() {
    check_vulkan_call(this->create_debug_utils_messenger(instance_wrapper->get_debug_messanger_info()), "Failed to set up debug messenger!");
}


VkResult loid::system::rendering::wrapper::DebugMessangerWrapper::create_debug_utils_messenger(const VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_info) {
    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance_wrapper->get_instance(), "vkCreateDebugUtilsMessengerEXT"));
    if (func) {
        return func(instance_wrapper->get_instance(), &debug_messenger_info, nullptr, &debug_messenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}


void loid::system::rendering::wrapper::DebugMessangerWrapper::destroy_debug_utils_messenger() noexcept {
    auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance_wrapper->get_instance(), "vkDestroyDebugUtilsMessengerEXT"));
    if (func) {
        func(instance_wrapper->get_instance(), debug_messenger, nullptr);
    }
}

