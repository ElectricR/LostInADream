#pragma once

#include "InstanceWrapper.h"
#include "WindowWrapper.h"
#include "../QueueFamilies.h"

#include <memory>


namespace loid {

namespace system {

namespace rendering {

namespace wrapper {

class LogicalDeviceWrapper {
public:
    LogicalDeviceWrapper(const VkPhysicalDevice&, const VkSurfaceKHR&);

    ~LogicalDeviceWrapper() {
        vkDestroyDevice(device, nullptr);
    }

    const VkDevice& get_device() const noexcept {
        return device;
    }

    QueueFamilies get_queue_families() const noexcept {
        return *queue_families;
    }

private:
    std::vector<VkDeviceQueueCreateInfo> get_device_queues_info() const noexcept;

    VkDeviceCreateInfo get_logical_device_create_info(auto&, auto&) const noexcept;

private:
    std::unique_ptr<QueueFamilies> queue_families;
    VkDevice device;
    VkQueue graphics_queue;
    VkQueue present_queue;
};

}

}

}

}

