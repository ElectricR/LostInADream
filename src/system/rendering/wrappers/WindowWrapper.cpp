#include "WindowWrapper.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

loid::system::rendering::wrapper::WindowWrapper::WindowWrapper() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(WIDTH, HEIGHT, "L", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
}

void loid::system::rendering::wrapper::framebuffer_resize_callback(GLFWwindow *glfw_window, int, int) {
    auto window = reinterpret_cast<WindowWrapper*>(glfwGetWindowUserPointer(glfw_window));
    window->framebuffer_updated = true;
}
