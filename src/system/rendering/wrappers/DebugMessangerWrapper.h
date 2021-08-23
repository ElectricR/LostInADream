#pragma once 
#include "InstanceWrapper.h"

#include <memory>

namespace loid {

namespace system {

namespace rendering {

namespace wrapper {

class DebugMessangerWrapper {
public:
    DebugMessangerWrapper(const std::shared_ptr<InstanceWrapper>&);
    ~DebugMessangerWrapper();

private:
    void setup_debug_messanger();

    VkResult create_debug_utils_messenger(const VkDebugUtilsMessengerCreateInfoEXT&);

    void destroy_debug_utils_messenger() noexcept;

private:
    std::shared_ptr<InstanceWrapper> instance_wrapper;
    VkDebugUtilsMessengerEXT debug_messenger;
};

}

}

}

}
