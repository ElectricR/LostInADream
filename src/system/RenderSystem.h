#pragma once
#include "rendering/VulkanInstance.h"
#include "rendering/LogicalDevice.h"
#include "rendering/Window.h"

#include <iostream>
#include <memory>

namespace loid {

namespace system {

class Renderer {
public:
    Renderer();
    // void render(); 
private:
    // TODO
    // void _draw_temp();

    std::shared_ptr<rendering::VulkanInstance> vulkan_instance;
    std::unique_ptr<rendering::Window> window;
    std::unique_ptr<rendering::LogicalDevice> logical_device;
};

} // namespace system
 
} // namespace loid
