#include "Window.h"

#include <stdexcept>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

loid::system::rendering::Window::~Window() {
    vkDestroySurfaceKHR(instance.lock()->get_instance(), surface, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}


loid::system::rendering::Window::Window() {
    this->init_window();
}

void loid::system::rendering::Window::init_window() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(WIDTH, HEIGHT, "L", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
}

void loid::system::rendering::framebuffer_resize_callback(GLFWwindow *glfw_window, int, int) {
    auto window = reinterpret_cast<loid::system::rendering::Window*>(glfwGetWindowUserPointer(glfw_window));
    window->framebuffer_updated = true;
}

void loid::system::rendering::Window::create_surface(std::shared_ptr<VulkanInstance> instance_ptr) {
    instance = instance_ptr;
    auto result = glfwCreateWindowSurface(instance.lock()->get_instance(), window, nullptr, &surface);

    switch (result) {
        case VK_SUCCESS:
            return;
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            throw std::runtime_error("Failed to create window surface: VK_ERROR_EXTENSION_NOT_PRESENT");
        default:
            throw std::runtime_error("Failed to create window surface");
    }
}
