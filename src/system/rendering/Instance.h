#pragma once
#include "wrappers/InstanceWrapper.h"
#include "wrappers/DebugMessangerWrapper.h"

#include <vulkan/vulkan_core.h>

#include <memory>

namespace loid {

namespace system {

namespace rendering {

class Window;

class Instance {
public:
    friend Window;

    Instance(); 

    const VkInstance& get_instance() const noexcept {
        return instance_wrapper->get_instance();
    }

private:
    bool check_validation_layer_support() const noexcept; 

private:
    std::shared_ptr<wrapper::InstanceWrapper> instance_wrapper;
    std::shared_ptr<wrapper::DebugMessangerWrapper> debug_messanger_wrapper;
};

} // namespace rendering

} // namespace system

} // namespace loid
