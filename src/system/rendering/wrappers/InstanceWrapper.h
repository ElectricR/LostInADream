#pragma once

#include <vulkan/vulkan_core.h>

#include <vector>

namespace loid {

namespace system {

namespace rendering {

namespace wrapper {

class InstanceWrapper {
public:
    InstanceWrapper();

    ~InstanceWrapper() {
        vkDestroyInstance(instance, nullptr);
    }

    const VkInstance& get_instance() const noexcept {
        return instance;
    }

    VkDebugUtilsMessengerCreateInfoEXT get_debug_messanger_info() const noexcept;

private:
    VkApplicationInfo get_app_info() const noexcept;
    
    std::vector<const char *> get_required_extensions() const;
    
    VkInstanceCreateInfo get_instance_info(const VkApplicationInfo&, const std::vector<const char *>&, const VkDebugUtilsMessengerCreateInfoEXT&) const noexcept;

private:   
    VkInstance instance;
};

}

}

}

}
