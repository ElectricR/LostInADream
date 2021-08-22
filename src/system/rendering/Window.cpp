#include "Window.h"
#include "utils.h"


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

void loid::system::rendering::Window::create_surface(std::shared_ptr<Instance> instance_ptr) {
    instance = instance_ptr;
    check_vulkan_call(glfwCreateWindowSurface(instance.lock()->get_instance(), window, nullptr, &surface), "Failed to create window surface");
}
