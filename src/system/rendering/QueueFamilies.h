#pragma once
#include <optional>
#include <cstdint>

#include <vulkan/vulkan_core.h>

namespace loid {

namespace system {

namespace rendering {

class QueueFamilies {
public:
    QueueFamilies(const VkPhysicalDevice&, const VkSurfaceKHR&);

    std::uint32_t get_graphics_family_index() const {
        return graphics_family_index.value();
    }

    std::uint32_t get_present_family_index() const {
        return present_family_index.value();
    }

    bool is_complete() const {
        return graphics_family_index.has_value() && present_family_index.has_value();
    }
private:
    std::optional<std::uint32_t> graphics_family_index;
    std::optional<std::uint32_t> present_family_index;
};

}

}

}
