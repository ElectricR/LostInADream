#include "SurfaceWrapper.h"
#include "../utils.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

loid::system::rendering::wrapper::SurfaceWrapper::SurfaceWrapper(const std::shared_ptr<InstanceWrapper>& instance_wrapper, const std::shared_ptr<WindowWrapper>& window_wrapper):
    instance_wrapper(instance_wrapper)
{
    check_vulkan_call(glfwCreateWindowSurface(instance_wrapper->get_instance(), window_wrapper->get_window(), nullptr, &surface), "Failed to create window surface");
}

