#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace loid {

namespace system {

namespace rendering {

struct ContextDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};

}

}

}
