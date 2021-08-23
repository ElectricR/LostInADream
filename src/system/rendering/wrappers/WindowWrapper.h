#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>

namespace loid {

namespace system {

namespace rendering {

namespace wrapper {

class WindowWrapper {
public:
    WindowWrapper();
    ~WindowWrapper() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    GLFWwindow* get_window() noexcept {
        return window;
    }

    friend void framebuffer_resize_callback(GLFWwindow*, int, int);
private:
    GLFWwindow *window = nullptr;
    bool framebuffer_updated = false;
};

void framebuffer_resize_callback(GLFWwindow*, int, int);

}

}

}

}

