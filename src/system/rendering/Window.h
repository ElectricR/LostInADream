#pragma once
#include "wrappers/WindowWrapper.h"
#include "wrappers/SurfaceWrapper.h"
#include "wrappers/InstanceWrapper.h"
#include "Instance.h"

#include <vulkan/vulkan_core.h>

#include <memory>

namespace loid {

namespace system {

namespace rendering {

class Window {
public:
    Window() {
        window_wrapper = std::make_shared<wrapper::WindowWrapper>();
    }

    void setup_surface(const Instance& instance) {
        surface_wrapper = std::make_shared<wrapper::SurfaceWrapper>(instance.instance_wrapper, window_wrapper);
    }

    const VkSurfaceKHR& get_surface() const noexcept {
        return surface_wrapper->get_surface();
    }

    GLFWwindow* get_window() noexcept {
        return window_wrapper->get_window();
    }
private:
    std::shared_ptr<wrapper::WindowWrapper> window_wrapper;
    std::shared_ptr<wrapper::SurfaceWrapper> surface_wrapper;
};


} // namespace rendering

} // namespace rendering

} // namespace loid
