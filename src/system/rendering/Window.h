#pragma once
#include "VulkanInstance.h"

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

    void create_surface(std::shared_ptr<VulkanInstance>);
private:
    void init_window();
    friend void framebuffer_resize_callback(GLFWwindow*, int, int);
private:
    VkSurfaceKHR surface;
    GLFWwindow *window = nullptr;
    std::weak_ptr<VulkanInstance> instance;
    bool framebuffer_updated = false;
};

void framebuffer_resize_callback(GLFWwindow*, int, int);

} // namespace rendering

} // namespace rendering

} // namespace loid
