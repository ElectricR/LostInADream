#pragma once

#include <vulkan/vulkan_core.h>

#include <stdexcept>
#include <string>
#include <vector>

namespace loid {

namespace system {

namespace rendering {

void check_vulkan_call(VkResult, const char*);

class RenderingError:
    public std::runtime_error
{
public:
    RenderingError(const std::string& exc_text):
        std::runtime_error(exc_text)
    {}
};

class VulkanException:
    public RenderingError
{
public:
    VulkanException(VkResult result, const std::string& exc_text):
        RenderingError(exc_text + ": " + std::to_string(result))
    {}
};

static const std::vector<const char*> device_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

}

}

}
