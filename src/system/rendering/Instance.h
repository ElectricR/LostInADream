#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>

namespace loid {

namespace system {

namespace rendering {

class Instance {
public:
    Instance(); 
    ~Instance();

    const VkInstance& get_instance() const noexcept {
        return instance;
    }

private:
    VkApplicationInfo get_app_info() const noexcept;
    
    std::vector<const char *> get_required_extensions() const;
    
    VkDebugUtilsMessengerCreateInfoEXT get_debug_messanger_info() const noexcept;
    
    bool check_validation_layer_support() const noexcept; 

    VkInstanceCreateInfo get_instance_info(const VkApplicationInfo&, const std::vector<const char *>&, const VkDebugUtilsMessengerCreateInfoEXT&) const noexcept;

    void setup_debug_messanger(const VkDebugUtilsMessengerCreateInfoEXT&);

    VkResult create_debug_utils_messenger(const VkDebugUtilsMessengerCreateInfoEXT&);

    void destroy_debug_utils_messenger() noexcept;
private:
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
};

} // namespace rendering

} // namespace system

} // namespace loid
