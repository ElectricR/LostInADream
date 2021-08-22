#pragma once
#include "Instance.h"

#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>

namespace loid {

namespace system {

namespace rendering {

class Window {
public:
    Window();
    ~Window();

    const VkSurfaceKHR& get_surface() const noexcept {
        return surface;
    }

    GLFWwindow* get_window() noexcept {
        return window;
    }

    void create_surface(std::shared_ptr<Instance>);
private:
    void init_window();
    friend void framebuffer_resize_callback(GLFWwindow*, int, int);
private:
    VkSurfaceKHR surface;
    GLFWwindow *window = nullptr;
    std::weak_ptr<Instance> instance;
    bool framebuffer_updated = false;
};

void framebuffer_resize_callback(GLFWwindow*, int, int);

} // namespace rendering

} // namespace rendering

} // namespace loid
