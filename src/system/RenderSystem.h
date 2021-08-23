#pragma once
#include "rendering/Instance.h"
#include "rendering/Device.h"
#include "rendering/Window.h"
#include "rendering/SwapChain.h"

#include <iostream>
#include <memory>

namespace loid {

namespace system {

class Renderer {
public:
    Renderer();
    ~Renderer();
    // void render(); 
private:
    // TODO
    // void _draw_temp();

    std::shared_ptr<rendering::Instance> vulkan_instance;
    std::unique_ptr<rendering::Window> window;
    std::shared_ptr<rendering::Device> logical_device;
    std::unique_ptr<rendering::SwapChain> swap_chain;
};

} // namespace system
 
} // namespace loid
