#pragma once

#include "InstanceWrapper.h"
#include "WindowWrapper.h"

#include <memory>


namespace loid {

namespace system {

namespace rendering {

namespace wrapper {

class SurfaceWrapper {
public:
    SurfaceWrapper(const std::shared_ptr<InstanceWrapper>&, const std::shared_ptr<WindowWrapper>&);

    ~SurfaceWrapper() {
        vkDestroySurfaceKHR(instance_wrapper->get_instance(), surface, nullptr);
    }

    const VkSurfaceKHR& get_surface() const noexcept {
        return surface;
    }

private:
    std::shared_ptr<InstanceWrapper> instance_wrapper;
    VkSurfaceKHR surface;
};

}

}

}

}

